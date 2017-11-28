/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Google Inc.
 * Copyright (C) 2016 Intel Corporation
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
 * Foundation, Inc.
 */

#include <arch/acpi.h>
#include <console/console.h>
#include <soc/acpi.h>
#include <soc/nvs.h>
#include <boardid.h>

void acpi_mainboard_gnvs(global_nvs_t *gnvs)
{
	/* Enable ALS and keyboard backlight if the board revision
	 * supports them. */
	if (board_id() >= 0x06) {
		printk(BIOS_INFO, "System supports ALS and keyboard backlight devices\n");
		gnvs->alse = 1;
		gnvs->kble = 1;
	}
}
