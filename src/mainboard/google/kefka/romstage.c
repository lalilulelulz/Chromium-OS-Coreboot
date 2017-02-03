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

#include <cbfs.h>
#include <console/console.h>
#include <lib.h>
#include <soc/gpio.h>
#include <soc/pci_devs.h>
#include <soc/romstage.h>
#include <string.h>
#include <chip.h>
#include <gpio.h>
#include "onboard.h"
#include <boardid.h>

/* All FSP specific code goes in this block */
void mainboard_romstage_entry(struct romstage_params *rp)
{
	struct pei_data *ps = rp->pei_data;

	mainboard_fill_spd_data(ps);

	/* Call back into chipset code with platform values updated. */
	romstage_common(rp);
}

void mainboard_memory_init_params(struct romstage_params *params,
	MEMORY_INIT_UPD *memory_params)
{

	int ram_id = 0;

	gpio_t spd_gpios[] = {
		GP_SW_80,	/* SATA_GP3,RAMID0 */
		GP_SW_67,	/* I2C3_SCL,RAMID1 */
		GP_SE_02,	/* MF_PLT_CLK1, RAMID2 */
		GP_SW_64,	/* I2C3_SDA RAMID3 */
	};

	ram_id = gpio_base2_value(spd_gpios, ARRAY_SIZE(spd_gpios));

	/*
	 *  RAMID = 2 - 4GiB Micron MT52L256M32D1PF-107
	 *  RAMID = 3 - 2GiB Micron MT52L256M32D1PF-107
	 */
	if (ram_id == 2 || ram_id == 3) {

		/*
		 * For new micron part, it requires read/receive
		 * enable training before sending cmds to get MR8.
		 * To override dram geometry settings as below:
		 *
		 * PcdDramWidth = x32
		 * PcdDramDensity = 8Gb
		 * PcdDualRankDram = disable
		 */
		memory_params->PcdRxOdtLimitChannel0 = 1;
		memory_params->PcdRxOdtLimitChannel1 = 1;
		memory_params->PcdDisableAutoDetectDram = 1;
		memory_params->PcdDramWidth = 2;
		memory_params->PcdDramDensity = 3;
		memory_params->PcdDualRankDram = 0;
	}

	memory_params->PcdMemoryTypeEnable = MEM_LPDDR3;
	memory_params->PcdMemChannel0Config =
					params->pei_data->spd_ch0_config;
	memory_params->PcdMemChannel1Config =
					params->pei_data->spd_ch1_config;

}
