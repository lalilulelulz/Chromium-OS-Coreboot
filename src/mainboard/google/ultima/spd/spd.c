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

/* We use RAM_ID3 to indicate dual channel config.
 *
 * 0b0000 - 2GiB total - 1 x 2GiB Samsung K4E8E304EE-EGCF 1600MHz
 * 0b0001 - 2GiB total - 1 x 2GiB Hynix H9CCNNN8GTMLAR-NUD 1600MHz
 * 0b0010 - 2GiB total - 1 x 2GiB Micron EDF8132A3MA-JD-F 1600MHz
 *
 * 0b1000 - 4GiB total - 2 x 2GiB Samsung K4E8E304EE-EGCF 1600MHz
 * 0b1001 - 4GiB total - 2 x 2GiB Hynix H9CCNNN8GTMLAR-NUD 1600MHz
 * 0b1010 - 4GiB total - 2 x 2GiB Micron EDF8132A3MA-JD-F 1600MHz
 *
 */

static void *get_spd_pointer(char *spd_file_content, int total_spds, int *dual)
{
	int ram_id = 0;

	gpio_t spd_gpios[] = {
		GP_SW_80,	/* SATA_GP3,RAMID0 */
		GP_SW_67,	/* I2C3_SCL,RAMID1 */
		GP_SE_02,	/* MF_PLT_CLK1, RAMID2 */
		GP_SW_64,       /* I2C3_SDA RAMID3 */
	};

	ram_id = gpio_base2_value(spd_gpios, ARRAY_SIZE(spd_gpios));
	int spd_index = ram_id & 0x7;

	/*
	 * Use 0 in case over total spds
	 */
	printk(BIOS_DEBUG, "ram_id=%d, total_spds: %d\n", ram_id, total_spds);

	if (spd_index >= total_spds)
		spd_index = 0;

	/* channel configs */
	*dual = !!(ram_id & (1<<3));

	printk(BIOS_DEBUG, "channel_config=%d\n", *dual);

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
