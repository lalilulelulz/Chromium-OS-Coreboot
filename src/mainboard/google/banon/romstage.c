/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Google Inc.
 * Copyright (C) 2015 Intel Corp.
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

#include <boardid.h>
#include <cbfs.h>
#include <chip.h>
#include <console/console.h>
#include <gpio.h>
#include <lib.h>
#include "onboard.h"
#include <soc/gpio.h>
#include <soc/pci_devs.h>
#include <soc/romstage.h>
#include <string.h>


/* All FSP specific code goes in this block */
void mainboard_romstage_entry(struct romstage_params *rp)
{
	struct pei_data *ps = rp->pei_data;

	int single_channel = 0;

	gpio_t spd_gpios[] = {
		GP_SW_64,	/* I2C3_SDA RAMID3 */
	};

	single_channel = gpio_get(spd_gpios[0]);

	ps->spd_ch0_config = 1;
	if (single_channel) {
		ps->spd_ch1_config = 2;
	} else {
		ps->spd_ch1_config = 1;
	}

	/* Call back into chipset code with platform values updated. */
	romstage_common(rp);
}


void mainboard_memory_init_params(struct romstage_params *params,
	MEMORY_INIT_UPD *memory_params)
{
	memory_params->PcdMemoryTypeEnable = MEM_LPDDR3;

	memory_params->PcdMemChannel0Config = params->pei_data->spd_ch0_config;
	memory_params->PcdMemChannel1Config = params->pei_data->spd_ch1_config;

}
