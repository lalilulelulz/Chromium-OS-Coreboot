/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <device/pnp_def.h>
#include <northbridge/amd/agesa/state_machine.h>
#include <superio/winbond/common/winbond.h>
#include <superio/winbond/w83627dhg/w83627dhg.h>

#define SERIAL_DEV PNP_DEV(0x4e, W83627DHG_SP1)

void board_BeforeAgesa(struct sysinfo *cb)
{
	winbond_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}

#if 0
	post_code(0x40);
	/* Reboots with outb(3,0x92), outb(4,0xcf9) or triple-fault all
	 * hang, looks like DRAM re-init goes wrong, don't know why. */
	val = agesawrapper_amdinitpost();
	if (val == 7) /* fatal, amdinitenv below is going to hang */
		outb(0x06, 0x0cf9); /* reset system harder instead */

	post_code(0x42);
	agesawrapper_amdinitenv();
#endif
