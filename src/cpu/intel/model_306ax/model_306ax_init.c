/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 * Copyright (C) 2011 The ChromiumOS Authors.  All rights reserved.
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

#include <console/console.h>
#include <device/device.h>
#include <device/pci.h>
#include <string.h>
#include <arch/acpi.h>
#include <cpu/cpu.h>
#include <cpu/x86/mtrr.h>
#include <cpu/x86/msr.h>
#include <cpu/x86/lapic.h>
#include <cpu/intel/microcode.h>
#include <cpu/intel/speedstep.h>
#include <cpu/intel/turbo.h>
#include <cpu/x86/cache.h>
#include <cpu/x86/name.h>
#include <usbdebug.h>

/*
 * List of suported C-states in this processor
 *
 * Latencies are typical worst-case package exit time in uS
 * taken from the SandyBridge BIOS specification.
 */
static acpi_cstate_t cstate_map[] = {
	{	/* 0: C0 */
	},{	/* 1: C1 */
		.latency = 1,
		.power = 1000,
		.resource = {
			.addrl = 0x00,	/* MWAIT State 0 */
			.space_id = ACPI_ADDRESS_SPACE_FIXED,
			.bit_width = ACPI_FFIXEDHW_VENDOR_INTEL,
			.bit_offset = ACPI_FFIXEDHW_CLASS_MWAIT,
			.resv = ACPI_FFIXEDHW_FLAG_HW_COORD,
		}
	},
	{	/* 2: C1E */
		.latency = 1,
		.power = 1000,
		.resource = {
			.addrl = 0x01,	/* MWAIT State 0 Sub-state 1 */
			.space_id = ACPI_ADDRESS_SPACE_FIXED,
			.bit_width = ACPI_FFIXEDHW_VENDOR_INTEL,
			.bit_offset = ACPI_FFIXEDHW_CLASS_MWAIT,
			.resv = ACPI_FFIXEDHW_FLAG_HW_COORD,
		}
	},
	{	/* 3: C3 */
		.latency = 63,
		.power = 500,
		.resource = {
			.addrl = 0x10,	/* MWAIT State 1 */
			.space_id = ACPI_ADDRESS_SPACE_FIXED,
			.bit_width = ACPI_FFIXEDHW_VENDOR_INTEL,
			.bit_offset = ACPI_FFIXEDHW_CLASS_MWAIT,
			.resv = ACPI_FFIXEDHW_FLAG_HW_COORD |
				ACPI_FFIXEDHW_FLAG_BM_STS,
		}
	},
	{	/* 4: C6 */
		.latency = 87,
		.power = 350,
		.resource = {
			.addrl = 0x20,	/* MWAIT State 2 */
			.space_id = ACPI_ADDRESS_SPACE_FIXED,
			.bit_width = ACPI_FFIXEDHW_VENDOR_INTEL,
			.bit_offset = ACPI_FFIXEDHW_CLASS_MWAIT,
			.resv = ACPI_FFIXEDHW_FLAG_HW_COORD |
				ACPI_FFIXEDHW_FLAG_BM_STS,
		}
	},
	{	/* 5: C7 */
		.latency = 90,
		.power = 200,
		.resource = {
			.addrl = 0x30,	/* MWAIT State 3 */
			.space_id = ACPI_ADDRESS_SPACE_FIXED,
			.bit_width = ACPI_FFIXEDHW_VENDOR_INTEL,
			.bit_offset = ACPI_FFIXEDHW_CLASS_MWAIT,
			.resv = ACPI_FFIXEDHW_FLAG_HW_COORD |
				ACPI_FFIXEDHW_FLAG_BM_STS,
		}
	},
	{	/* 6: C7S */
		.latency = 90,
		.power = 200,
		.resource = {
			.addrl = 0x31,	/* MWAIT State 3 Sub-state 1 */
			.space_id = ACPI_ADDRESS_SPACE_FIXED,
			.bit_width = ACPI_FFIXEDHW_VENDOR_INTEL,
			.bit_offset = ACPI_FFIXEDHW_CLASS_MWAIT,
			.resv = ACPI_FFIXEDHW_FLAG_HW_COORD |
				ACPI_FFIXEDHW_FLAG_BM_STS,
		}
	},
	{ 0 }
};

static const uint32_t microcode_updates[] = {
	#include "microcode-M12206A7_0000001A.h"

	/*  Dummy terminator  */
        0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0,
};

#define IA32_FEATURE_CONTROL 0x003a

#define CPUID_VMX (1 << 5)
#define CPUID_SMX (1 << 6)
static void enable_vmx(void)
{
	struct cpuid_result regs;
	msr_t msr;

	msr = rdmsr(IA32_FEATURE_CONTROL);

	if (msr.lo & (1 << 0)) {
		/* VMX locked. If we set it again we get an illegal
		 * instruction
		 */
		return;
	}

	regs = cpuid(1);
	if (regs.ecx & CPUID_VMX) {
		msr.lo |= (1 << 2);
		if (regs.ecx & CPUID_SMX)
			msr.lo |= (1 << 1);
	}

	wrmsr(IA32_FEATURE_CONTROL, msr);

	msr.lo |= (1 << 0); /* Set lock bit */

	wrmsr(IA32_FEATURE_CONTROL, msr);
}

#define PMG_CST_CONFIG_CONTROL	0xe2
#define PMG_IO_CAPTURE_BASE	0xe4
#define MISC_PWR_MGMT		0x1aa
#define POWER_CTL		0x1fc
#define PP0_CURRENT_CONFIG	0x601
#define PP1_CURRENT_CONFIG	0x602
#define PKGC3_IRTL		0x60a
#define PKGC6_IRTL		0x60b
#define PKGC7_IRTL		0x60c
#define  IRTL_VALID		(1 << 15)
#define  IRTL_1_NS		(0 << 10)
#define  IRTL_32_NS		(1 << 10)
#define  IRTL_1024_NS		(2 << 10)
#define  IRTL_32768_NS		(3 << 10)
#define  IRTL_1048576_NS	(4 << 10)
#define  IRTL_33554432_NS	(5 << 10)
#define  IRTL_RESPONSE_MASK	(0x3ff)

static void configure_c_states(void)
{
	msr_t msr;

	msr = rdmsr(PMG_CST_CONFIG_CONTROL);
	msr.lo |= (1 << 28);	// C1 Auto Undemotion Enable
	msr.lo |= (1 << 27);	// C3 Auto Undemotion Enable
	msr.lo |= (1 << 26);	// C1 Auto Demotion Enable
	msr.lo |= (1 << 25);	// C3 Auto Demotion Enable
	msr.lo |= (1 << 15);	// config lock until next reset
	msr.lo |= (1 << 10);	// Enable IO MWAIT redirection
	msr.lo |= 7;		// No package C-state limit
	wrmsr(PMG_CST_CONFIG_CONTROL, msr);

	msr = rdmsr(PMG_IO_CAPTURE_BASE);
	msr.lo &= ~0x7ffff;
	msr.lo |= (PMB0_BASE + 4);	// LVL_2 base address
	msr.lo |= (2 << 16);		// CST Range: C7 is max C-state
	wrmsr(PMG_IO_CAPTURE_BASE, msr);

	msr = rdmsr(MISC_PWR_MGMT);
	msr.lo &= ~(1 << 0);	// Enable P-state HW_ALL coordination
	msr.lo |= (1 << 22);	// Lock TM interrupt
	wrmsr(MISC_PWR_MGMT, msr);

	msr = rdmsr(POWER_CTL);
	msr.lo |= (1 << 18);	// Enable Energy Perf Bias MSR 0x1b0
	msr.lo |= (1 << 1);	// C1E Enable
	msr.lo |= (1 << 0);	// Bi-directional PROCHOT#
	wrmsr(POWER_CTL, msr);

	/* C3 Interrupt Response Time Limit */
	msr.hi = 0;
	msr.lo = IRTL_VALID | IRTL_1024_NS | 0x50;
	wrmsr(PKGC3_IRTL, msr);

	/* C6 Interrupt Response Time Limit */
	msr.hi = 0;
	msr.lo = IRTL_VALID | IRTL_1024_NS | 0x68;
	wrmsr(PKGC6_IRTL, msr);

	/* C7 Interrupt Response Time Limit */
	msr.hi = 0;
	msr.lo = IRTL_VALID | IRTL_1024_NS | 0x6D;
	wrmsr(PKGC7_IRTL, msr);

	/* Primary Plane Current Limit */
	msr = rdmsr(PP0_CURRENT_CONFIG);
	msr.lo &= ~0x1fff;
	msr.lo |= 1000;
	wrmsr(PP0_CURRENT_CONFIG, msr);

	/* Secondary Plane Current Limit */
	msr = rdmsr(PP1_CURRENT_CONFIG);
	msr.lo &= ~0x1fff;
	msr.lo |= 300;
	wrmsr(PP1_CURRENT_CONFIG, msr);
}

#define IA32_MISC_ENABLE	0x1a0
#define IA32_THERM_INTERRUPT		0x19b
#define IA32_PACKAGE_THERM_INTERRUPT	0x1b2
static void configure_misc(void)
{
	msr_t msr;

	msr = rdmsr(IA32_MISC_ENABLE);
	msr.lo |= (1 << 0);	  /* Fast String enable */
	msr.lo |= (1 << 3); 	  /* TM1/TM2/EMTTM enable */
	msr.lo |= (1 << 16);	  /* Enhanced SpeedStep Enable */
	wrmsr(IA32_MISC_ENABLE, msr);

	/* Disable Thermal interrupts */
	msr.lo = 0;
	msr.hi = 0;
	wrmsr(IA32_THERM_INTERRUPT, msr);

	/* Enable package critical interrupt only */
	msr.lo = 1 << 4;
	msr.hi = 0;
	wrmsr(IA32_PACKAGE_THERM_INTERRUPT, msr);
}

#define MSR_PIC_MSG_CONTROL	0x2e
static void enable_lapic_tpr(void)
{
	msr_t msr;

	msr = rdmsr(MSR_PIC_MSG_CONTROL);
	msr.lo &= ~(1 << 10);	/* Enable APIC TPR updates */
	wrmsr(MSR_PIC_MSG_CONTROL, msr);
}

#define IA32_PLATFORM_DCA_CAP	0x1f8
static void configure_dca_cap(void)
{
	struct cpuid_result cpuid_regs;
	msr_t msr;

	/* Check feature flag in CPUID.(EAX=1):ECX[18]==1 */
	cpuid_regs = cpuid(1);
	if (cpuid_regs.ecx & (1 << 18)) {
		msr = rdmsr(IA32_PLATFORM_DCA_CAP);
		msr.lo |= 1;
		wrmsr(IA32_PLATFORM_DCA_CAP, msr);
	}
}

#define MSR_PLATFORM_INFO	0xce
#define IA32_PERF_CTL 		0x199

static void set_max_ratio(void)
{
	msr_t msr;

	/* Platform Info bits 15:8 give max ratio */
	msr = rdmsr(MSR_PLATFORM_INFO);
	msr.hi = 0;
	msr.lo &= 0xff00;
	wrmsr(IA32_PERF_CTL, msr);

	printk(BIOS_DEBUG, "model_206ax: frequency set to %d\n",
	       ((msr.lo >> 8) & 0xff) * 100);
}

#define IA32_ENERGY_PERFORMANCE_BIAS	0x1b0
#define ENERGY_POLICY_PERFORMANCE	0
#define ENERGY_POLICY_NORMAL		6
#define ENERGY_POLICY_POWERSAVE		15

static void set_energy_perf_bias(u8 policy)
{
	msr_t msr;

	/* Energy Policy is bits 3:0 */
	msr = rdmsr(IA32_ENERGY_PERFORMANCE_BIAS);
	msr.lo &= ~0xf;
	msr.lo |= policy & 0xf;
	wrmsr(IA32_ENERGY_PERFORMANCE_BIAS, msr);

	printk(BIOS_DEBUG, "model_206ax: energy policy set to %u\n",
	       policy);
}

#define IA32_MC0_STATUS 0x401
static void configure_mca(void)
{
	msr_t msr;
	int i;

	msr.lo = msr.hi = 0;
	/* This should only be done on a cold boot */
	for (i = 0; i < 7; i++)
		wrmsr(IA32_MC0_STATUS + (i * 4), msr);
}

#if CONFIG_USBDEBUG
static unsigned ehci_debug_addr;
#endif

/*
 * Initialize any extra cores/threads in this package.
 */
static void intel_cores_init(device_t cpu)
{
	struct cpuid_result result;
	unsigned cores, threads, i;

	result = cpuid_ext(0xb, 0); /* Threads per core */
	threads = result.ebx & 0xff;

	result = cpuid_ext(0xb, 1); /* Cores per package */
	cores = result.ebx & 0xff;

	/* Only initialize extra cores from BSP */
	if (cpu->path.apic.apic_id)
		return;

	printk(BIOS_DEBUG, "CPU: %u has %u cores %u threads\n",
	       cpu->path.apic.apic_id, cores, threads);

	for (i = 1; i < cores; ++i) {
		struct device_path cpu_path;
		device_t new;

		/* Build the cpu device path */
		cpu_path.type = DEVICE_PATH_APIC;
		cpu_path.apic.apic_id =
			cpu->path.apic.apic_id + i;

		/* Update APIC ID if no hyperthreading */
		if (threads == 1)
			cpu_path.apic.apic_id <<= 1;

		/* Allocate the new cpu device structure */
		new = alloc_dev(cpu->bus, &cpu_path);
		if (!new)
			continue;

		printk(BIOS_DEBUG, "CPU: %u has core %u\n",
		       cpu->path.apic.apic_id,
		       new->path.apic.apic_id);

		/* Start the new cpu */
		if (!start_cpu(new)) {
			/* Record the error in cpu? */
			printk(BIOS_ERR, "CPU %u would not start!\n",
			       new->path.apic.apic_id);
		}
	}
}

static void model_206ax_init(device_t cpu)
{
	char processor_name[49];
	struct cpuid_result cpuid_regs;

	/* Turn on caching if we haven't already */
	x86_enable_cache();

	/* Update the microcode */
	intel_update_microcode(microcode_updates);

	/* Clear out pending MCEs */
	configure_mca();

	/* Print processor name */
	fill_processor_name(processor_name);
	printk(BIOS_INFO, "CPU: %s.\n", processor_name);

#if CONFIG_USBDEBUG
	// Is this caution really needed?
	if(!ehci_debug_addr)
		ehci_debug_addr = get_ehci_debug();
	set_ehci_debug(0);
#endif

	/* Setup MTRRs based on physical address size */
	cpuid_regs = cpuid(0x80000008);
	x86_setup_mtrrs(cpuid_regs.eax & 0xff);
	x86_mtrr_check();

	/* Setup Page Attribute Tables (PAT) */
	// TODO set up PAT

#if CONFIG_USBDEBUG
	set_ehci_debug(ehci_debug_addr);
#endif

	/* Enable the local cpu apics */
	enable_lapic_tpr();
	setup_lapic();

	/* Enable virtualization */
	enable_vmx();

	/* Configure C States */
	configure_c_states();

	/* Configure Enhanced SpeedStep and Thermal Sensors */
	configure_misc();

	/* Enable Direct Cache Access */
	configure_dca_cap();

	/* Set energy policy */
	set_energy_perf_bias(ENERGY_POLICY_NORMAL);

	/* Set Max Ratio */
	set_max_ratio();

	/* Enable Turbo */
	enable_turbo();

	/* Start up extra cores */
	intel_cores_init(cpu);
}

static struct device_operations cpu_dev_ops = {
	.init     = model_206ax_init,
};

static struct cpu_device_id cpu_table[] = {
	{ X86_VENDOR_INTEL, 0x206a0 }, /* Intel Sandybridge */
	{ X86_VENDOR_INTEL, 0x206a6 }, /* Intel Sandybridge D1 */
	{ X86_VENDOR_INTEL, 0x206a7 }, /* Intel Sandybridge D2/J1 */
	{ 0, 0 },
};

static const struct cpu_driver driver __cpu_driver = {
	.ops      = &cpu_dev_ops,
	.id_table = cpu_table,
	.cstates  = cstate_map,
};

