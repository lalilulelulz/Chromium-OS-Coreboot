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
#include <gpio.h>
#include <soc/gpio.h>
#include <soc/romstage.h>
#include <string.h>

#define SPD_SIZE 256

#define MEM_SINGLE_CHANNEL	1
#define MEM_DUAL_CHANNEL	0

/*
 * Usage of RAMID straps
 *
 *  RAMID1 - Single/Dual channel configuration
 *   0 - Dual channel, 1 - Single channel
 *
 *  Combination of RAMID3, RAMID2, RAMID0 - Index of SPD table
 *   Index 0 - Samsung K4E8E304EE-EGCE 1600MHz 23nm
 *   Index 1 - Samsung K4E8E324EB-EGCF 1866MHz 20nm
 */

static void *get_spd_pointer(char *spd_file_content, int total_spds, int *dual)
{
	int spd_index = 0;
	int single_channel_conf = 0;

	gpio_t spd_gpios[] = {
		GP_SW_80,	/* SATA_GP3,RAMID0 */
		GP_SE_02,	/* MF_PLT_CLK1, RAMID2 */
		GP_SW_64,	/* I2C3_SDA RAMID3 */
	};

	spd_index = gpio_base2_value(spd_gpios, ARRAY_SIZE(spd_gpios));

	single_channel_conf = gpio_get(GP_SW_67); /* I2C3_SCL,RAMID1 */

	printk(BIOS_DEBUG, "spd_index=%d, total_spds: %d\n", spd_index, total_spds);
	printk(BIOS_DEBUG, "single_channel_conf=%d\n", single_channel_conf);

	if (spd_index >= total_spds)
		return NULL;

	/* Single/Dual channel configs */
	if (single_channel_conf == MEM_DUAL_CHANNEL)
		*dual = 1;

	return &spd_file_content[SPD_SIZE * spd_index];
}

/* Copy SPD data for on-board memory */
void mainboard_fill_spd_data(struct pei_data *ps)
{
	struct cbfs_file *spd_file;
	void *spd_content;
	int dual_channel = 0;

	/* Find the SPD data in CBFS. */
	spd_file = cbfs_get_file(CBFS_DEFAULT_MEDIA, "spd.bin");
	if (!spd_file)
		die("SPD data not found.");

	/*
	 * Both channels are always present in SPD data. Always use matched
	 * DIMMs so use the same SPD data for each DIMM.
	 */
	spd_content = get_spd_pointer(CBFS_SUBHEADER(spd_file),
				      ntohl(spd_file->len) / SPD_SIZE,
				      &dual_channel);
	if (IS_ENABLED(CONFIG_DISPLAY_SPD_DATA) && spd_content != NULL) {
		printk(BIOS_DEBUG, "SPD Data:\n");
		hexdump(spd_content, SPD_SIZE);
		printk(BIOS_DEBUG, "\n");
	}

	/*
	 * Set SPD and memory configuration:
	 * Memory type: 0=DimmInstalled,
	 *              1=SolderDownMemory,
	 *              2=DimmDisabled
	 */
	if (spd_content != NULL) {
		ps->spd_data_ch0 = spd_content;
		ps->spd_ch0_config = 1;
		if (dual_channel) {
			ps->spd_data_ch1 = spd_content;
			ps->spd_ch1_config = 1;
		} else {
			ps->spd_ch1_config = 2;
		}
	}
}
