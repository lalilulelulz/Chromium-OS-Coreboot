/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Google Inc.
 * Copyright (C) 2015 Intel Corporation.
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

#include <arch/io.h>
#include <bootstate.h>
#include <console/console.h>
#include <console/post_codes.h>
#include <cpu/x86/smm.h>
#include <reg_script.h>
#include <spi-generic.h>
#include <stdlib.h>
#include <soc/pci_devs.h>
#include <soc/lpc.h>
#include <soc/rcba.h>
#include <soc/spi.h>
#include <soc/systemagent.h>

const struct reg_script system_agent_finalize_script[] = {
	REG_PCI_OR16(0x50, 1 << 0),				/* GGC */
	REG_PCI_OR32(0x5c, 1 << 0),				/* DPR */
	REG_PCI_OR32(0x78, 1 << 10),				/* ME */
	REG_PCI_OR32(0x90, 1 << 0),				/* REMAPBASE */
	REG_PCI_OR32(0x98, 1 << 0),				/* REMAPLIMIT */
	REG_PCI_OR32(0xa0, 1 << 0),				/* TOM */
	REG_PCI_OR32(0xa8, 1 << 0),				/* TOUUD */
	REG_PCI_OR32(0xb0, 1 << 0),				/* BDSM */
	REG_PCI_OR32(0xb4, 1 << 0),				/* BGSM */
	REG_PCI_OR32(0xb8, 1 << 0),				/* TSEGMB */
	REG_PCI_OR32(0xbc, 1 << 0),				/* TOLUD */
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x5500, 1 << 0),	/* PAVP */
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x5f00, 1 << 31),	/* SA PM */
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x6020, 1 << 0),	/* UMA GFX */
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x63fc, 1 << 0),	/* VTDTRK */
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x6800, 1 << 31),
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x7000, 1 << 31),
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x77fc, 1 << 0),
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x50fc, 0x8f),
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x7ffc, 1 << 0),
	REG_MMIO_OR32(MCH_BASE_ADDRESS + 0x5880, 1 << 5),
	REG_MMIO_WRITE8(MCH_BASE_ADDRESS + 0x50fc, 0x8f),	/* MC */

	REG_SCRIPT_END
};


static void skylake_finalize(void *unused)
{
	printk(BIOS_DEBUG, "Finalizing chipset.\n");

	/* Indicate finalize step with post code */
	post_code(POST_OS_BOOT);
}

BOOT_STATE_INIT_ENTRIES(finalize) = {
	BOOT_STATE_INIT_ENTRY(BS_OS_RESUME, BS_ON_ENTRY,
			      skylake_finalize, NULL),
	BOOT_STATE_INIT_ENTRY(BS_PAYLOAD_LOAD, BS_ON_EXIT,
			      skylake_finalize, NULL),
};
