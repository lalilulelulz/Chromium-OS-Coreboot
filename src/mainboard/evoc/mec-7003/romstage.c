/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2010 coresystems GmbH
 * Copyright (C) 2010 Google Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <arch/io.h>
#include <arch/romcc_io.h>
#include <device/pci_def.h>
#include <device/pnp_def.h>
#include <cpu/x86/lapic.h>
#include <pc80/mc146818rtc.h>
#include "option_table.h"
#include <console/console.h>
#include "superio/nuvoton/nct6776f/nct6776f.h"
#include "superio/nuvoton/nct6776f/early_init.c"
#include <cpu/x86/bist.h>
#include "northbridge/intel/sandybridge/sandybridge.h"
#include "northbridge/intel/sandybridge/raminit.h"
#include "southbridge/intel/bd82x6x/pch.h"

void setup_pch_gpios(void)
{
	// printk(BIOS_DEBUG, " GPIOS...");
	/* General Registers */
	outl(0xffebf7ff, DEFAULT_GPIOBASE + 0x00);	/* GPIO_USE_SEL */
	outl(0x00402862, DEFAULT_GPIOBASE + 0x04);	/* GP_IO_SEL */
	outl(0x18002862, DEFAULT_GPIOBASE + 0x0c);	/* GP_LVL */
	/* Output Control Registers */
	outl(0x00000000, DEFAULT_GPIOBASE + 0x18);	/* GPO_BLINK */
	/* Input Control Registers */
	outl(0x000020c0, DEFAULT_GPIOBASE + 0x2c);	/* GPI_INV */
	outl(0x000100fe, DEFAULT_GPIOBASE + 0x30);	/* GPIO_USE_SEL2 */
	outl(0x00010000, DEFAULT_GPIOBASE + 0x34);	/* GP_IO_SEL2 */
	outl(0x00000042, DEFAULT_GPIOBASE + 0x38);	/* GP_LVL2 */
}

static void pch_enable_lpc(void)
{
	// Enable Serial IRQ
	pci_write_config8(PCI_DEV(0, 0x1f, 0), 0x64, 0xd0);
	// Set COM1/COM2 decode range
	pci_write_config16(PCI_DEV(0, 0x1f, 0), 0x80, 0x0010);
	// Enable COM1/COM2/KBD/SuperIO1+2
	pci_write_config16(PCI_DEV(0, 0x1f, 0), 0x82, 0x3f03);
	// ??? Decode
	// pci_write_config32(PCI_DEV(0, 0x1f, 0), 0x84, 0x00040311);
	// EC Mailbox Decode
	//pci_write_config32(PCI_DEV(0, 0x1f, 0), 0x88, 0x00040701);
	// Disable
	pci_write_config32(PCI_DEV(0, 0x1f, 0), 0x8c, 0x00000000);
	// Disable
	pci_write_config32(PCI_DEV(0, 0x1f, 0), 0x90, 0x00000000);
}

static void rcba_config(void)
{
	u32 reg32;

	/* Set up virtual channel 0 */
	//RCBA32(0x0014) = 0x80000001;
	//RCBA32(0x001c) = 0x03128010;

	/* Device interrupt pin register (board specific) */
	RCBA32(0x3100) = 0x00042210;
	RCBA32(0x3104) = 0x00002100;
	RCBA32(0x3108) = 0x10004321;
	RCBA32(0x310c) = 0x00214321;

	/* dev irq route register (board specific) */
	RCBA16(0x3140) = 0x3210;
	RCBA16(0x3142) = 0x3210;
	RCBA16(0x3144) = 0x3210;
	RCBA16(0x3146) = 0x3210;
	RCBA16(0x3148) = 0x3210;

	/* Enable IOAPIC (generic) */
	RCBA8(0x31ff) = 0x03;

	/* Enable upper 128bytes of CMOS (generic) */
	RCBA32(0x3400) = (1 << 2);

	/* Disable unused devices (board specific) */
	reg32 = 0x00300063; // FIXME put in the right defines
	RCBA32(0x3418) = reg32;

	/* Enable PCIe Root Port Clock Gate */
	// RCBA32(0x341c) = 0x00000001;

	// Disable PCI dynamic gating
	RCBA32(0x341c) &= ~(1 << 16);
	RCBA32(0x341c) = 0xcfa8000f; // for now just hard code the right value
}

// FIXME, this function is generic code that should go to sb/... or
// nb/../early_init.c
static void early_pch_init(void)
{
	uint8_t reg8;
	uint32_t reg32;

	// program secondary mlt XXX byte?
	pci_write_config8(PCI_DEV(0, 0x1e, 0), 0x1b, 0x20);

	// reset rtc power status
	reg8 = pci_read_config8(PCI_DEV(0, 0x1f, 0), 0xa4);
	reg8 &= ~(1 << 2);
	pci_write_config8(PCI_DEV(0, 0x1f, 0), 0xa4, reg8);

	// usb transient disconnect
	reg8 = pci_read_config8(PCI_DEV(0, 0x1f, 0), 0xad);
	reg8 |= (3 << 0);
	pci_write_config8(PCI_DEV(0, 0x1f, 0), 0xad, reg8);

	// sata
	reg8 = pci_read_config8(PCI_DEV(0, 0x1f, 2), 0x94);
	reg8 |= (1 << 8) | (1 << 7);
	pci_write_config8(PCI_DEV(0, 0x1f, 0), 0x94, reg8);

	reg32 = pci_read_config32(PCI_DEV(0, 0x1d, 7), 0xfc);
	reg32 |= (1 << 29) | (1 << 17);
	pci_write_config32(PCI_DEV(0, 0x1d, 7), 0xfc, reg32);

	reg32 = pci_read_config32(PCI_DEV(0, 0x1d, 7), 0xdc);
	reg32 |= (1 << 31) | (1 << 27);
	pci_write_config32(PCI_DEV(0, 0x1d, 7), 0xdc, reg32);

	RCBA32(0x0088) = 0x0011d000;
	RCBA16(0x01fc) = 0x060f;
	RCBA32(0x01f4) = 0x86000040;
	RCBA32(0x0214) = 0x10030509; // different from ich7
	RCBA32(0x0218) = 0x00020504;
	RCBA8(0x0220) = 0xc5;
	reg32 = RCBA32(0x3410);
	reg32 |= (1 << 6);
	RCBA32(0x3410) = reg32;
	reg32 = RCBA32(0x3430);
	reg32 &= ~(3 << 0);
	reg32 |= (1 << 0);
	RCBA32(0x3430) = reg32;
	RCBA32(0x3418) |= (1 << 0);
	RCBA16(0x0200) = 0x2008;
	RCBA8(0x2027) = 0x0d;
	RCBA16(0x3e08) |= (1 << 7);
	RCBA16(0x3e48) |= (1 << 7);
	RCBA32(0x3e0e) |= (1 << 7);
	RCBA32(0x3e4e) |= (1 << 7);
	reg32 = RCBA32(0x2034);
	reg32 &= ~(0x0f << 16);
	reg32 |= (5 << 16);
	RCBA32(0x2034) = reg32;
	RCBA32(0x3418) |= (1 << 6) | (1 << 5) | (1 << 1);
	// after pci enumeration:
	//RCBA32(0x1d40) |= (1 << 0);
}

#include <cbmem.h>

void main(unsigned long bist)
{
	u32 reg32;
	int boot_mode = 0;

	if (bist == 0)
		enable_lapic();

	pch_enable_lpc();

	pci_write_config32(PCI_DEV(0, 0x1f, 0), GPIOBASE, DEFAULT_GPIOBASE | 1);
	pci_write_config8(PCI_DEV(0, 0x1f, 0), 0x4c /* GC */ , 0x10);	/* Enable GPIOs */
	setup_pch_gpios();

	/* Set up the console */
#define SERIAL_DEV PNP_DEV(0x2e, NCT6776F_SP1)
 	nct6776f_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);

	console_init();

	/* Halt if there was a built in self test failure */
	report_bist_failure(bist);

#if 1
	if (MCHBAR16(SSKPD) == 0xCAFE) {
		printk(BIOS_DEBUG, "soft reset detected, rebooting properly\n");
		outb(0x6, 0xcf9);
		while (1) asm("hlt");
	}
#endif

	/* Perform some early chipset initialization required
	 * before RAM initialization can work
	 */
	sandybridge_early_initialization();

	// Detect if we're coming from a warm reset.
	if (MCHBAR32(0xf14) & (1 << 8)) {
		printk(BIOS_DEBUG, "soft reset detected.\n");
		boot_mode = 1;
	}

	/* Read PM1_CNT */
	reg32 = inl(DEFAULT_PMBASE + 0x04);
	printk(BIOS_DEBUG, "PM1_CNT: %08x\n", reg32);
	if (((reg32 >> 10) & 7) == 5) {
#if CONFIG_HAVE_ACPI_RESUME
		printk(BIOS_DEBUG, "Resume from S3 detected.\n");
		boot_mode = 2;
		/* Clear SLP_TYPE. This will break stage2 but
		 * we care for that when we get there.
		 */
		outl(reg32 & ~(7 << 10), DEFAULT_PMBASE + 0x04);

#else
		printk(BIOS_DEBUG, "Resume from S3 detected, but disabled.\n");
#endif
	}
	post_code(0x38);

	/* Enable SPD ROMs and DDR-III DRAM */
	enable_smbus();

	post_code(0x39);
	sdram_initialize(boot_mode);

	post_code(0x3a);
	/* Perform some initialization that must run before stage2 */
	early_pch_init();
	post_code(0x3b);

	/* This should probably go away. Until now it is required
	 * and mainboard specific
	 */
	rcba_config();
	post_code(0x3c);

	/* Chipset Errata! */
	//fixup_sandybridge_errata();

	/* Initialize the internal PCIe links before we go into stage2 */
	sandybridge_late_initialization();

//#if !CONFIG_HAVE_ACPI_RESUME
//#if CONFIG_DEFAULT_CONSOLE_LOGLEVEL > 8
	post_code(0x3d);
	// FIXME if these ramchecks are commented out, the CAR code will fail
	// to copy the USBDEBUG data structure from cache to ram. Weird.
	ram_check(0x00000000, 0x000a0000);
	//ram_check(0x00100000, 0x3f800000);
	ram_check(CONFIG_RAMBASE, CONFIG_RAMTOP);
//#endif
//#endif

	post_code(0x3e);
	quick_ram_check();

	MCHBAR16(SSKPD) = 0xCAFE;

#if CONFIG_HAVE_ACPI_RESUME
	/* Start address of high memory tables */
	unsigned long high_ram_base = get_top_of_ram() - HIGH_MEMORY_SIZE;

	/* If there is no high memory area, we didn't boot before, so
	 * this is not a resume. In that case we just create the cbmem toc.
	 */
	if ((boot_mode == 2) && cbmem_reinit((u64)high_ram_base)) {
		void *resume_backup_memory = cbmem_find(CBMEM_ID_RESUME);

		/* copy 1MB - 64K to high tables ram_base to prevent memory corruption
		 * through stage 2. We could keep stuff like stack and heap in high tables
		 * memory completely, but that's a wonderful clean up task for another
		 * day.
		 */
		if (resume_backup_memory)
			memcpy(resume_backup_memory, (void *)CONFIG_RAMBASE, HIGH_MEMORY_SAVE);

		/* Magic for S3 resume */
		pci_write_config32(PCI_DEV(0, 0x00, 0), SKPAD, 0xcafed00d);
	} else {
		pci_write_config32(PCI_DEV(0, 0x00, 0), SKPAD, 0xcafebabe);
	}
#endif
	post_code(0x3f);
}
