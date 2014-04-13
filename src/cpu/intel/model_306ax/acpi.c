/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2009 coresystems GmbH
 * Copyright (C) 2011 The Chromium OS Authors. All rights reserved.
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
#include <cpu/intel/turbo.h>
#include <device/device.h>
#include <device/pci.h>
#include "chip.h"

#define MIN_POWER 16000
#define MAX_POWER 35000

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

static int generate_CST_table(struct cpu_intel_model_206ax_config *conf)
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
	len += acpigen_emit_byte(0xa1);		/* ElseOp */
	lenif = acpigen_write_len_f();		/* PkgLength */
	lenif += acpigen_emit_byte(0xa4);	/* ReturnOp */
	lenif += generate_cstate_entries(cpu->cstates, conf->c1_battery,
					 conf->c2_battery, conf->c3_battery);
	acpigen_patch_len(lenif - 1);
	len += lenif;

	acpigen_patch_len(len - 1);
	return len;
}

void generate_cpu_entries(void)
{
	int len_pr, len_ps;
	int coreID, cpuID, pcontrol_blk = PMB0_BASE, plen = 6;
	msr_t msr;
	int totalcores = dev_count_cpu();
	int cores_per_package = get_cores_per_package();
	int numcpus = totalcores/cores_per_package;
	int fsb = 100; /* BCLK fixed at 100MHz for SandyBridge */
	struct cpu_intel_model_206ax_config *conf = NULL;
	u8 coord_type = HW_ALL;
	device_t cpu;

	/* Find the SpeedStep CPU in the device tree using magic APIC ID */
	cpu = dev_find_lapic(SPEEDSTEP_APIC_MAGIC);
	if (!cpu)
		return;
	conf = cpu->chip_info;
	coord_type = conf->pstate_coord_type;

	if (conf->disable_acpi) {
		printk(BIOS_DEBUG, "ACPI: P-state and C-state tables "
		       "disabled by devicetree\n");
		return;
	}

	printk(BIOS_DEBUG, "Found %d CPU(s) with %d core(s) each.\n",
	       numcpus, cores_per_package);

	for (cpuID=1; cpuID <=numcpus; cpuID++) {
		for (coreID=1; coreID<=cores_per_package; coreID++) {
			if (coreID>1) {
				pcontrol_blk = 0;
				plen = 0;
			}
			len_pr = acpigen_write_processor(
				(cpuID-1)*cores_per_package+coreID-1,
				pcontrol_blk, plen);
			len_pr += acpigen_write_empty_PCT();
			len_pr += acpigen_write_PSD_package(
				cpuID-1,cores_per_package,coord_type);
			len_pr += acpigen_write_name("_PSS");

			int max_states=8;
			int bus_step=2;
			msr = rdmsr(0xce);
			int bus_min=(msr.hi >> (40-32)) & 0xff;
			int bus_max=(msr.lo >> 8) & 0xff;
			int clock_max=fsb*bus_max;
			int clock_min=fsb*bus_min;
			printk(BIOS_DEBUG, "clocks between %d and %d MHz.\n",
			       clock_min, clock_max);
			int power_max=MAX_POWER;
			int power_min=MIN_POWER;
			int num_states=(bus_max-bus_min)/bus_step;
			while (num_states > max_states-1) {
				bus_step <<= 1;
				num_states >>= 1;
			}
			printk(BIOS_DEBUG, "adding %x P-States between "
			       "busratio %x and %x, incl. P0\n",
			       num_states+1, bus_min, bus_max);
			int power_step=(power_max-power_min)/num_states;
			int clock_step=(clock_max-clock_min)/num_states;

			/* P[0] is Turbo if enabled */
			if (get_turbo_state() == TURBO_ENABLED) {
				len_ps = acpigen_write_package(num_states+2);
				msr = rdmsr(0x1ad);
				int bus_turbo = msr.lo & 0xff;
				len_ps += acpigen_write_PSS_package(
					clock_max+1 /*mhz*/,
					power_max+1 /*mW*/,
					10 /*lat1*/,
					10 /*lat2*/,
					bus_turbo<<8 /*control*/,
					bus_turbo<<8 /*status*/);
			} else {
				len_ps = acpigen_write_package(num_states+1);
			}

			len_ps += acpigen_write_PSS_package(
				clock_max /*mhz*/,
				power_max /*mW*/,
				10 /*lat1*/,
				10 /*lat2*/,
				bus_max<<8 /*control*/,
				bus_max<<8 /*status*/);
			int current_bus=bus_min+((num_states-1)*bus_step);
			int current_power=power_min+((num_states-1)*power_step);
			int current_clock=clock_min+((num_states-1)*clock_step);
			int i;
			for (i=0;i<num_states; i++) {
				len_ps += acpigen_write_PSS_package(
					current_clock /*mhz*/,
					current_power /*mW*/,
					10 /*lat1*/,
					10 /*lat2*/,
					current_bus<<8 /*control*/,
					current_bus<<8 /*status*/);
				current_bus -= bus_step;
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

struct chip_operations cpu_intel_model_206ax_ops = {
	CHIP_NAME("Intel SandyBridge CPU")
};
