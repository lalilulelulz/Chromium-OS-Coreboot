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
#include <cbmem.h>
#include <console/console.h>
#include <gpio.h>
#include <lib.h>
#include <memory_info.h>
#include <smbios.h>
#include <spd.h>
#include <soc/gpio.h>
#include <soc/romstage.h>
#include <string.h>

#define SPD_SIZE 256


static const uint32_t dual_channel_config = (1 << 1); 

/* Copy SPD data for on-board memory */
void mainboard_fill_spd_data(struct pei_data *ps)
{
	int dual_channel = 0;
	int ram_id = 0;

	gpio_t spd_gpios[] = {
		/*GP_SW_80,*/	/* SATA_GP3,RAMID0 */
		/*GP_SW_67,*/	/* I2C3_SCL,RAMID1 */
		/*GP_SE_02,*/	/* MF_PLT_CLK1, RAMID2 */
		GP_SW_64,	/* I2C3_SDA RAMID3 */
	};

	ram_id = gpio_base2_value(spd_gpios, ARRAY_SIZE(spd_gpios));
	printk(BIOS_DEBUG, "T.H. ram_id=%d\n", ram_id);
	
	/* Determine if this is a single or dual channel memory system */
	if (dual_channel_config & (1 << ram_id))
		dual_channel = 1;

	printk(BIOS_DEBUG, "T.H. dual_channel=%d\n", dual_channel);

		ps->spd_ch0_config = 1;
		printk(BIOS_DEBUG, "Channel 0 DIMM soldered down\n");
		if (dual_channel) {
			printk(BIOS_DEBUG, "Channel 1 DIMM not soldered down\n");
			ps->spd_ch1_config = 1;
		} else {
			printk(BIOS_DEBUG, "Channel 1 DIMM installed\n");
			ps->spd_ch1_config = 2;
		}
}


