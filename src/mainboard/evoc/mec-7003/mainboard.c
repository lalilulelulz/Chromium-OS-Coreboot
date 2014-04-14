/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
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

#include <types.h>
#include <device/device.h>
#include <console/console.h>
#if defined(CONFIG_PCI_OPTION_ROM_RUN_YABEL) && CONFIG_PCI_OPTION_ROM_RUN_YABEL
#include <x86emu/x86emu.h>
#endif
#include <pc80/mc146818rtc.h>
#include <arch/io.h>
#include <arch/interrupt.h>
#include "hda_verb.h"
#include "chip.h"

#if defined(CONFIG_PCI_OPTION_ROM_RUN_REALMODE) && CONFIG_PCI_OPTION_ROM_RUN_REALMODE
static int evoc_mec_7003_int15_handler(struct eregs *regs)
{
	int res=-1;

	printk(BIOS_DEBUG, "%s\n", __func__);

	switch(regs->eax & 0xffff) {
	case 0x5f21: /* Get Display Core Clock in MHz */
		regs->eax = (regs->eax & 0xffff0000 ) | 0x5f;
		regs->ecx = (regs->ecx & 0xffff0000 ) | 167; // TODO read from HW
		res=0;
		break;
	case 0x5f22: /* Get FSB in MHz */
		regs->eax = (regs->eax & 0xffff0000 ) | 0x5f;
		regs->ecx = (regs->ecx & 0xffff0000 ) | 800; // TODO read from HW
		res=0;
		break;
        default:
		printk(BIOS_DEBUG, "Unknown INT15 function %04x!\n",
				regs->eax & 0xffff);
		break;
	}
	return res;
}
#endif

#if defined(CONFIG_PCI_OPTION_ROM_RUN_YABEL) && CONFIG_PCI_OPTION_ROM_RUN_YABEL
static int int15_handler(void)
{
	printk(BIOS_DEBUG, "%s: AX=%04x BX=%04x CX=%04x DX=%04x\n",
			  __func__, M.x86.R_AX, M.x86.R_BX, M.x86.R_CX, M.x86.R_DX);

	switch (M.x86.R_AX) {
	case 0x5f21:
		M.x86.R_AX = 0x005f; // Success
		M.x86.R_CX = 167; // Display Core Clock in MHz
		break;
	case 0x5f22:
		M.x86.R_AX = 0x005f; // Success
		M.x86.R_CX = 800; // FSB in MHz
		break;
	default:
		/* Interrupt was not handled */
		return 0;
	}

	/* Interrupt handled */
	return 1;
}

static void int15_install(void)
{
	typedef int (* yabel_handleIntFunc)(void);
	extern yabel_handleIntFunc yabel_intFuncArray[256];
	yabel_intFuncArray[0x15] = int15_handler;
}
#endif

/* Audio Setup */

extern const u32 * cim_verb_data;
extern u32 cim_verb_data_size;

static void verb_setup(void)
{
	cim_verb_data = mainboard_cim_verb_data;
	cim_verb_data_size = sizeof(mainboard_cim_verb_data);
}

// mainboard_enable is executed as first thing after
// enumerate_buses().

static void mainboard_enable(device_t dev)
{
#if defined(CONFIG_PCI_OPTION_ROM_RUN_YABEL) && CONFIG_PCI_OPTION_ROM_RUN_YABEL
	/* Install custom int15 handler for VGA OPROM */
	int15_install();
#endif
#if defined(CONFIG_PCI_OPTION_ROM_RUN_REALMODE) && CONFIG_PCI_OPTION_ROM_RUN_REALMODE
	mainboard_interrupt_handlers(0x15, &evoc_mec_7003_int15_handler);
#endif
	verb_setup();
}

struct chip_operations mainboard_ops = {
	CHIP_NAME("Evoc MEC-7003 mainboard")
	.enable_dev = mainboard_enable,
};

