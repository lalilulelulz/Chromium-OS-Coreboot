/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011-2012 The ChromiumOS Authors.  All rights reserved.
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

#include <string.h>
#include <vendorcode/google/chromeos/chromeos.h>
#include <arch/io.h>
#ifdef __PRE_RAM__
#include <arch/romcc_io.h>
#else
#include <device/device.h>
#include <device/pci.h>
#endif

#include <southbridge/intel/bd82x6x/pch.h>
#include "ec.h"

#ifndef __PRE_RAM__
#include <boot/coreboot_tables.h>
#include <arch/coreboot_tables.h>

#define GPIO_COUNT	6
#define ACTIVE_LOW	0
#define ACTIVE_HIGH	1

void fill_lb_gpios(struct lb_gpios *gpios)
{
	device_t dev = dev_find_slot(0, PCI_DEVFN(0x1f,0));
	u16 gpio_base = pci_read_config32(dev, GPIOBASE) & 0xfffe;
	u16 gen_pmcon_1 = pci_read_config32(dev, GEN_PMCON_1);

	if (!gpio_base)
		return;

	u32 gp_lvl3 = inl(gpio_base + GP_LVL3);

	gpios->size = sizeof(*gpios) + (GPIO_COUNT * sizeof(struct lb_gpio));
	gpios->count = GPIO_COUNT;

	/* Write Protect: GPIO70 active high */
	gpios->gpios[0].port = 70;
	gpios->gpios[0].polarity = ACTIVE_HIGH;
	gpios->gpios[0].value = (gp_lvl3 >> (70 - 64)) & 1;
	strncpy((char *)gpios->gpios[0].name,"write protect",
							GPIO_MAX_NAME_LENGTH);

	/* Recovery: GPIO69 active low */
	gpios->gpios[1].port = 69;
	gpios->gpios[1].polarity = ACTIVE_LOW;
	gpios->gpios[1].value = get_recovery_mode_switch();
	strncpy((char *)gpios->gpios[1].name,"recovery", GPIO_MAX_NAME_LENGTH);

	/* Developer: GPIO17 active low */
	gpios->gpios[2].port = 17;
	gpios->gpios[2].polarity = ACTIVE_LOW;
	gpios->gpios[2].value = get_developer_mode_switch();
	strncpy((char *)gpios->gpios[2].name,"developer", GPIO_MAX_NAME_LENGTH);

	/* Hard code the lid switch GPIO to open. */
	gpios->gpios[3].port = 100;
	gpios->gpios[3].polarity = ACTIVE_LOW;
	gpios->gpios[3].value = 0;
	strncpy((char *)gpios->gpios[3].name,"lid", GPIO_MAX_NAME_LENGTH);

	/* Power Button */
	gpios->gpios[4].port = 101;
	gpios->gpios[4].polarity = ACTIVE_LOW;
	gpios->gpios[4].value = (gen_pmcon_1 >> 9) & 1;
	strncpy((char *)gpios->gpios[4].name,"power", GPIO_MAX_NAME_LENGTH);

	/* Was VGA Option ROM loaded? */
	gpios->gpios[5].port = -1; /* Indicate that this is a pseudo GPIO */
	gpios->gpios[5].polarity = ACTIVE_HIGH;
	gpios->gpios[5].value = oprom_is_loaded;
	strncpy((char *)gpios->gpios[5].name,"oprom", GPIO_MAX_NAME_LENGTH);
}
#endif


int get_developer_mode_switch(void)
{
	/*
	 * TODO(kimarie): Stout will have a "virtual" developer mode switch which
	 *                will be handled elsewhere.  Hard code return to "0" for
	 *                the time being until the "virtual" developer mode handling
	 *                is complete.
	 */
	return 0; /* Hard code developer mode on */
}


int get_recovery_mode_switch(void)
{
	/*
	 * TODO(kimarie): Stout will have a "virtual" recovery mode switch which
	 *                will be handled elsewhere.  Hard code return to "1" for
	 *                the time being until the "virtual" recovery mode handling
	 *                is complete.
	 */
	return 1; /* Hard code recovery mode off */
}

