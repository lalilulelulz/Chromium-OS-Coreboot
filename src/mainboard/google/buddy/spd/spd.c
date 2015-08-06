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

#include <arch/byteorder.h>
#include <cbfs.h>
#include <console/console.h>
#include <string.h>
#include <broadwell/gpio.h>
#include <broadwell/pei_data.h>
#include <broadwell/romstage.h>
#include <mainboard/google/buddy/spd/spd.h>

/* Copy SPD data for on-board memory */
void mainboard_fill_spd_data(struct pei_data *pei_data)
{
	pei_data->spd_addresses[0] = 0xa0;
	pei_data->spd_addresses[1] = 0x00;
	pei_data->spd_addresses[2] = 0xa4;
	pei_data->spd_addresses[3] = 0x00;
	pei_data->dimm_channel0_disabled = 2;
	pei_data->dimm_channel1_disabled = 2;
	/* Enable 2x refresh mode */
	pei_data->ddr_refresh_2x = 1;
	pei_data->dq_pins_interleaved = 1;
}
