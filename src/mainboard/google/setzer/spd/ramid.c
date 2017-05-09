/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Google Inc.
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
#include <stdlib.h>
#include <gpio.h>
#include <mainboard/google/setzer/spd/spd.h>

uint8_t get_ramid(void)
{
	gpio_t spd_gpios[] = {
		GP_SW_64,	/* I2C3_SDA, RAMID0 */
		GP_SE_02,   /* MF_PLT_CLK1, RAMID1 */
		GP_SW_67,	/* I2C3_SCL, RAMID2 */
		GP_SW_80,	/* SATA_GP3, RAMID3 */
	};

	return gpio_base2_value(spd_gpios, ARRAY_SIZE(spd_gpios));
}
