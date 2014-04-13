/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 The Chromium OS Authors. All rights reserved.
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

#include <arch/acpi.h>
#include <vendorcode/google/chromeos/chromeos.h>
#include <types.h>
#include <console/console.h>
#include <ec/quanta/it8518/ec.h>
#include <arch/io.h>
#include "ec.h"

void stout_ec_init(void)
{
	printk(BIOS_DEBUG, "stout_ec_init\n");

	/*
	 * TODO(kimarie) Configure EC for S3 resume, ensure wake events are
	 *               disabled, etc.
	 */

	/* Report EC info */
	/* EC FW version: 2 bytes */
	printk(BIOS_DEBUG,"  EC FW version %x%x\n",
			ec_read(EC_FW_VER),
			ec_read(EC_FW_VER + 1));

	/* Unmute */
	ec_kbc_write_cmd(EC_KBD_CMD_UNMUTE);

	// TODO If warm reset reset EC
	//ec_cmd_write(EC_CMD_WARM_RESET);
	// TODO Fan Error Check, Thermal Error Check, Critical Low Battery Check,
	//      First Use Day Check, Power Limit Setting
}
