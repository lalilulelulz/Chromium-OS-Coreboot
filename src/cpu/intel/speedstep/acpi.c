/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2009 coresystems GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <types.h>
#include <console/console.h>
#include <arch/acpi.h>
#include <arch/acpigen.h>
#include <arch/cpu.h>
#include <cpu/x86/msr.h>
#include <cpu/intel/acpi.h>
#include <cpu/intel/speedstep.h>
#include <device/device.h>
#include <device/pci.h>
#include "chip.h"

// XXX: PSS table values for power consumption are for Merom only

static int determine_total_number_of_cores(void)
{
	device_t cpu;
	int count = 0;
	for(cpu = all_devices; cpu; cpu = cpu->next) {
		if ((cpu->path.type != DEVICE_PATH_APIC) ||
			(cpu->bus->dev->path.type != DEVICE_PATH_APIC_CLUSTER)) {
			continue;
		}
		if (!cpu->enabled) {
			continue;
		}
		count++;
	}
	return count;
}

static int get_cores_per_package(void)
{
	struct cpuinfo_x86 c;
	struct cpuid_result result;
	int cores = 1;

	get_fms(&c, cpuid_eax(1));
	if (c.x86 != 6)
		return 1;

	switch (c.x86_model) {
	case 0x2a: /* SandyBridge */
		result = cpuid_ext(0xb, 1);
		cores = result.ebx & 0xff;
		break;
	default:
		cores = (cpuid_ebx(1) >> 16) & 0xff;
		break;
	}

	return cores;
}

static int get_fsb(void)
{
	struct cpuinfo_x86 c;
	int core_fsb[8] = { -1, 133, -1, 166, -1, 100, -1, -1 };
	int core2_fsb[8] = { 266, 133, 200, 166, -1, 100, -1, -1 };

	get_fms(&c, cpuid_eax(1));
	if (c.x86 != 6)
		return -1;

	switch (c.x86_model) {
	case 0xe:  /* Core Solo/Duo */
	case 0x1c: /* Atom */
		return core_fsb[rdmsr(0xcd).lo & 7];
	case 0xf:  /* Core 2*/
	case 0x17: /* Enhanced Core */
		return core2_fsb[rdmsr(0xcd).lo & 7];
	case 0x2a: /* SandyBridge BCLK fixed at 100MHz*/
		return 0; // DISABLED
	}

	printk(BIOS_DEBUG, "Warning: No supported FSB frequency\n");
	return -1;
}

static int generate_cstate_entries(acpi_cstate_t *cstates,
				   int c1, int c2, int c3)
{
	int length, cstate_count = 0;

	/* Count number of active C-states */
	if (c1 > 0)
		++cstate_count;
	if (c2 > 0)
		++cstate_count;
	if (c3 > 0)
		++cstate_count;
	if (!cstate_count)
		return 0;

	length = acpigen_write_package(cstate_count + 1);
	length += acpigen_write_byte(cstate_count);

	/* Add an entry if the level is enabled */
	if (c1 > 0)
		length += acpigen_write_CST_package(1, &cstates[c1]);
	if (c2 > 0)
		length += acpigen_write_CST_package(2, &cstates[c2]);
	if (c3 > 0)
		length += acpigen_write_CST_package(3, &cstates[c3]);

	acpigen_patch_len(length - 1);
	return length;
}

static int generate_CST_table(struct cpu_intel_speedstep_config *conf)
{
	struct cpu_info *info;
	struct cpu_driver *cpu;
	int len, lenif;

	if (!conf)
		return 0;

	/* Find CPU map of supported C-states */
	info = cpu_info();
	if (!info)
		return 0;
	cpu = find_cpu_driver(info->cpu);
	if (!cpu || !cpu->cstates)
		return 0;

	len = acpigen_emit_byte(0x14);		/* MethodOp */
	len += acpigen_write_len_f();		/* PkgLength */
	len += acpigen_emit_namestring("_CST");
	len += acpigen_emit_byte(0x00);		/* No Arguments */

	/* If running on AC power */
	len += acpigen_emit_byte(0xa0);		/* IfOp */
	lenif = acpigen_write_len_f();		/* PkgLength */
	lenif += acpigen_emit_namestring("PWRS");
	lenif += acpigen_emit_byte(0xa4);	/* ReturnOp */
	lenif += generate_cstate_entries(cpu->cstates, conf->c1_acpower,
					 conf->c2_acpower, conf->c3_acpower);
	acpigen_patch_len(lenif - 1);
	len += lenif;

	/* Else on battery power */
	len += acpigen_emit_byte(0xa4);	/* ReturnOp */
	len += generate_cstate_entries(cpu->cstates, conf->c1_battery,
					conf->c2_battery, conf->c3_battery);
	acpigen_patch_len(len - 1);
	return len;
}

void generate_cpu_entries(void)
{
	int len_pr, len_ps;
	int coreID, cpuID, pcontrol_blk = PMB0_BASE, plen = 6;
	msr_t msr;
	int totalcores = determine_total_number_of_cores();
	int cores_per_package = get_cores_per_package();
	int numcpus = totalcores/cores_per_package; // this assumes that all CPUs share the same layout
	int fsb = get_fsb();
	struct cpu_intel_speedstep_config *conf = NULL;
	u8 coord_type = SW_ANY;
	device_t cpu;

	if (fsb <= 0)
		return;

	/* Find the SpeedStep CPU in the device tree using magic APIC ID */
	cpu = dev_find_lapic(SPEEDSTEP_APIC_MAGIC);
	if (cpu) {
		conf = cpu->chip_info;
		coord_type = conf->pstate_coord_type;
	}

	printk(BIOS_DEBUG, "Found %d CPU(s) with %d core(s) each.\n", numcpus, cores_per_package);

	for (cpuID=1; cpuID <=numcpus; cpuID++) {
		for (coreID=1; coreID<=cores_per_package; coreID++) {
		if (coreID>1) {
			pcontrol_blk = 0;
			plen = 0;
		}
		len_pr = acpigen_write_processor((cpuID-1)*cores_per_package+coreID-1, pcontrol_blk, plen);
			len_pr += acpigen_write_empty_PCT();
			len_pr += acpigen_write_PSD_package(
				cpuID-1,cores_per_package,coord_type);
			len_pr += acpigen_write_name("_PSS");

			int max_states=8;
			int busratio_step=2;
			msr = rdmsr(IA32_PERF_STS);
			int busratio_min=(msr.lo >> 24) & 0x1f;
			int busratio_max=(msr.hi >> (40-32)) & 0x1f;
			int vid_min=msr.lo & 0x3f;
			msr = rdmsr(IA32_PLATFORM_ID);
			int vid_max=msr.lo & 0x3f;
			int clock_max=fsb*busratio_max;
			int clock_min=fsb*busratio_min;
			printk(BIOS_DEBUG, "clocks between %d and %d MHz.\n", clock_min, clock_max);
#define MEROM_MIN_POWER 16000
#define MEROM_MAX_POWER 35000
			int power_max=MEROM_MAX_POWER;
			int power_min=MEROM_MIN_POWER;

			int num_states=(busratio_max-busratio_min)/busratio_step;
			while (num_states > max_states-1) {
				busratio_step <<= 1;
				num_states >>= 1;
			}
			printk(BIOS_DEBUG, "adding %x P-States between busratio %x and %x, incl. P0\n", num_states+1, busratio_min, busratio_max);
			int vid_step=(vid_max-vid_min)/num_states;
			int power_step=(power_max-power_min)/num_states;
			int clock_step=(clock_max-clock_min)/num_states;
			len_ps = acpigen_write_package(num_states+1); // for Super LFM, this must be increases by another one
			len_ps += acpigen_write_PSS_package(clock_max /*mhz*/, power_max /*mW*/, 0 /*lat1*/, 0 /*lat2*/, (busratio_max<<8)|(vid_max) /*control*/, (busratio_max<<8)|(vid_max) /*status*/);
			int current_busratio=busratio_min+((num_states-1)*busratio_step);
			int current_vid=vid_min+((num_states-1)*vid_step);
			int current_power=power_min+((num_states-1)*power_step);
			int current_clock=clock_min+((num_states-1)*clock_step);
			int i;
			for (i=0;i<num_states; i++) {
				len_ps += acpigen_write_PSS_package(current_clock /*mhz*/, current_power /*mW*/, 0 /*lat1*/, 0 /*lat2*/, (current_busratio<<8)|(current_vid) /*control*/, (current_busratio<<8)|(current_vid) /*status*/);
				current_busratio -= busratio_step;
				current_vid -= vid_step;
				current_power -= power_step;
				current_clock -= clock_step;
			}
			len_ps--;
			acpigen_patch_len(len_ps);
			len_pr += acpigen_write_PPC(0);
			/* Generate C-state tables */
			len_pr += generate_CST_table(conf);
		len_pr += len_ps;
		len_pr--;
		acpigen_patch_len(len_pr);
		}
	}
}

