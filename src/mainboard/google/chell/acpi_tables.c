/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Google Inc.
 * Copyright (C) 2015 Intel Corporation
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
#include <string.h>
#include <vendorcode/google/chromeos/cros_vpd.h>

void acpi_mainboard_gnvs(global_nvs_t *gnvs)
{
	const char *vpd_key = "new_d_cover";
	char vpd_val[8];

	/* If VPD indicates new D-panel set appropriate PL1 minimum */
	if (cros_vpd_gets(vpd_key, vpd_val, ARRAY_SIZE(vpd_val)) &&
	    !strncmp(vpd_val, "true", 4)) {
		printk(BIOS_INFO, "New D-Cover found, set new thermal parameters\n");
		gnvs->pl1l = 3600;
		gnvs->tr1p = 57;
		gnvs->tr2p = 58;
		gnvs->cuif = 300;
		gnvs->t2if = 200;
		gnvs->t2sp = 900;
	}
}
