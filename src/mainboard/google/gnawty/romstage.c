/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Google Inc.
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

#include <stdint.h>
#include <string.h>
#include <cbfs.h>
#include <console/console.h>
#include <baytrail/gpio.h>
#include <baytrail/mrc_wrapper.h>
#include <baytrail/romstage.h>

/*
 * RAM_ID[2:0] are on GPIO_SSUS[39:37]
 * 0b0000 - 4GiB total - 2 x 2GiB Samsung K4B4G1646Q-HYK0 1600MHz
 * 0b0001 - 4GiB total - 2 x 2GiB Hynix  H5TC4G63AFR-PBA 1600MHz
 * 0b0010 - 2GiB total - 2 x 1GiB Micron MT41K128M16JT-125:K 1600MHz
 * 0b0011 - 2GiB total - 1 x 2GiB Hynix  H5TC4G63MFR-PBA 1600MHz
 * 0b0100 - 2GiB total - 1 x 2GiB Hynix  H5TC4G63CFR-PBA 1600MHz
 * 0b0101 - 2GiB total - 1 x 2GiB Hynix  H5TC4G63AFR-PBA 1600MHz
 * 0b0110 - 4GiB total - 2 x 2GiB Hynix  H5TC4G63CFR-PBA 1600MHz
 * 0b0111 - 4GiB total - 2 x 2GiB Hynix  H5TC4G63MFR-PBA 1600MHz
 * 0b1000 - 2GiB total - 1 x 2Gib Micro  MT41K256M16TW-107
 * 0b1001 - 4GiB total - 2 x 2Gib Micro  MT41K256M16TW-107
*/
static const uint32_t dual_channel_config =
	(1 << 0) | (1 << 1) | (1 << 2) | (1 << 6) | (1 << 7) | (1 << 9) ;

#define SPD_SIZE 256
#define GPIO_SSUS_37_PAD 57
#define GPIO_SSUS_38_PAD 50
#define GPIO_SSUS_39_PAD 58
#define GPIO_SSUS_40_PAD 52

static void *get_spd_pointer(char *spd_file_content, int total_spds, int *dual)
{
	int ram_id = 0;

	/* The ram_id[2:0] pullups on rambi are too large for the default 20K
	 * pulldown on the pad. Therefore, disable the internal pull resistor to
	 * read high values correctly. */
	ssus_disable_internal_pull(GPIO_SSUS_37_PAD);
	ssus_disable_internal_pull(GPIO_SSUS_38_PAD);
	ssus_disable_internal_pull(GPIO_SSUS_39_PAD);
	ssus_enable_internal_pull(GPIO_SSUS_40_PAD, PAD_PULL_DOWN | PAD_PU_20K);

	ram_id |= (ssus_get_gpio(GPIO_SSUS_37_PAD) << 0);
	ram_id |= (ssus_get_gpio(GPIO_SSUS_38_PAD) << 1);
	ram_id |= (ssus_get_gpio(GPIO_SSUS_39_PAD) << 2);
	ram_id |= (ssus_get_gpio(GPIO_SSUS_40_PAD) << 3);

	printk(BIOS_DEBUG, "ram_id=%d, total_spds: %d\n", ram_id, total_spds);

	if (ram_id >= total_spds)
		return NULL;

	/* Single channel configs */
	if (dual_channel_config & (1 << ram_id))
		*dual = 1;

	return &spd_file_content[SPD_SIZE * ram_id];
}

void mainboard_romstage_entry(struct romstage_params *rp)
{
	struct cbfs_file *spd_file;
	void *spd_content;
	int dual_channel = 0;

	struct mrc_params mp = {
		.mainboard = {
			.dram_type = DRAM_DDR3L,
			.dram_info_location = DRAM_INFO_SPD_MEM,
			.weaker_odt_settings = 1,
		},
	};

	spd_file = cbfs_get_file(CBFS_DEFAULT_MEDIA, "spd.bin");
	if (!spd_file)
		die("SPD data not found.");

	/* Both channels are always present. */
	spd_content = get_spd_pointer(CBFS_SUBHEADER(spd_file),
	                              ntohl(spd_file->len) / SPD_SIZE,
	                              &dual_channel);
	mp.mainboard.dram_data[0] = spd_content;
	if (dual_channel)
		mp.mainboard.dram_data[1] = spd_content;

	rp->mrc_params = &mp;
	romstage_common(rp);
}
