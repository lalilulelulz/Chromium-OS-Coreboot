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

#include <console/console.h>
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
#include <ec/compal/ene932/ec.h>
#include "ec.h"

#define ACTIVE_LOW	0
#define ACTIVE_HIGH	1

#ifndef __PRE_RAM__
#include <boot/coreboot_tables.h>
#include <arch/coreboot_tables.h>

#define GPIO_COUNT	6

void fill_lb_gpios(struct lb_gpios *gpios)
{
	device_t dev = dev_find_slot(0, PCI_DEVFN(0x1f,0));
	u16 gpio_base = pci_read_config32(dev, GPIOBASE) & 0xfffe;
	u16 gen_pmcon_1 = pci_read_config32(dev, GEN_PMCON_1);

	if (!gpio_base)
		return;

	u32 gp_lvl = inl(gpio_base + GP_LVL);
	u32 gp_lvl3 = inl(gpio_base + GP_LVL3);

	gpios->size = sizeof(*gpios) + (GPIO_COUNT * sizeof(struct lb_gpio));
	gpios->count = GPIO_COUNT;

	/* Write Protect: GPIO70 active high */
	gpios->gpios[0].port = 70;
	gpios->gpios[0].polarity = ACTIVE_LOW;
	gpios->gpios[0].value = (gp_lvl3 >> (70 - 64)) & 1;
	strncpy((char *)gpios->gpios[0].name,"write protect", GPIO_MAX_NAME_LENGTH);

	/* Recovery: Virtual GPIO in the EC (Servo GPIO69 active low) */
	gpios->gpios[1].port = -1;
	gpios->gpios[1].polarity = ACTIVE_HIGH;
	gpios->gpios[1].value = get_recovery_mode_switch();
	strncpy((char *)gpios->gpios[1].name,"recovery", GPIO_MAX_NAME_LENGTH);

	/* Developer: Virtual GPIO in the EC ( Servo GPIO17 active low) */
	gpios->gpios[2].port = -1;
	gpios->gpios[2].polarity = ACTIVE_HIGH;
	gpios->gpios[2].value = get_developer_mode_switch();
	strncpy((char *)gpios->gpios[2].name,"developer", GPIO_MAX_NAME_LENGTH);

	/* Lid switch GPIO active high (open). */
	gpios->gpios[3].port = 15;
	gpios->gpios[3].polarity = ACTIVE_HIGH;
	gpios->gpios[3].value = ((gp_lvl >> 15) & 1);;
	strncpy((char *)gpios->gpios[3].name,"lid", GPIO_MAX_NAME_LENGTH);

	/* Power Button */
	gpios->gpios[4].port = 101;
	gpios->gpios[4].polarity = ACTIVE_LOW;
	gpios->gpios[4].value = (gen_pmcon_1 >> 9) & 1;
	strncpy((char *)gpios->gpios[4].name,"power", GPIO_MAX_NAME_LENGTH);

	/* Did we load the VGA Option ROM? */
	gpios->gpios[5].port = -1; /* Indicate that this is a pseudo GPIO */
	gpios->gpios[5].polarity = ACTIVE_HIGH;
	gpios->gpios[5].value = oprom_is_loaded;
	strncpy((char *)gpios->gpios[5].name,"oprom", GPIO_MAX_NAME_LENGTH);

	/* EC RW: GPIO68 active low */
	/* TODO: Add this once this is added to uboot and inc the GPIO count above.
	 * gpios->gpios[6].port = 68;
	 * gpios->gpios[6].polarity = ACTIVE_LOW;
	 * gpios->gpios[6].value = (gp_lvl3 >> (68 - 64)) & 1;
	 * strncpy((char *)gpios->gpios[1].name,"ec_in_rw", GPIO_MAX_NAME_LENGTH);
	 */
}
#endif


int get_developer_mode_switch(void)
{
	device_t dev;
#ifdef __PRE_RAM__
	dev = PCI_DEV(0, 0x1f, 0);
#else
	dev = dev_find_slot(0, PCI_DEVFN(0x1f,0));
#endif
	u16 gpio_base = pci_read_config32(dev, GPIOBASE) & 0xfffe;

	if (!gpio_base)
		return(0);

/*
 * Dev mode is controled by EC and uboot stores a flag in TPM. This GPIO is only
 * for the debug header. It is AND'd to the EC request.
 */

	u32 gp_lvl = inl(gpio_base + GP_LVL);
	printk(BIOS_DEBUG,"DEV MODE GPIO 17: %x\n", !((gp_lvl >> 17) & 1));

	/* GPIO17, active low -- return active high reading and let
	 * it be inverted by the caller if needed. */
	return !((gp_lvl >> 17) & 1);
}

int get_recovery_mode_switch(void)
{
	u8 rec_mode;

	device_t dev;
#ifdef __PRE_RAM__
	dev = PCI_DEV(0, 0x1f, 0);
#else
	dev = dev_find_slot(0, PCI_DEVFN(0x1f,0));
#endif
	u16 gpio_base = pci_read_config32(dev, GPIOBASE) & 0xfffe;

	if (!gpio_base)
		return(0);

	/* GPIO69, active low. For Servo support
	 * Treat as active high and let the caller invert if needed. */
	u32 gp_lvl3 = inl(gpio_base + GP_LVL3);
	rec_mode = !((gp_lvl3 >> (69 - 64)) & 1);
	printk(BIOS_DEBUG,"REC MODE GPIO 69: %x\n", rec_mode);

/*
 * Check EC B3(command) FC(data) 2C(data) to EC. And then EC will return a byte.
 * Bit0 – 0: Recovery mode (active low)
 * Bit0 – 1: Normal mode
 *
 * OR this together to go to REC MODE from EC and Servo.
 */
#ifndef __PRE_RAM__
	ec_kbc_write_cmd(0xB3);
	ec_kbc_write_ib(0xFC);
	ec_kbc_write_ib(0x2C);
	rec_mode |= !(ec_kbc_read_ob() & 1);
	printk(BIOS_DEBUG,"REC MODE (EC OR'd with GPIO): %x\n", rec_mode);
#endif

	return (rec_mode);
}

int parrot_ec_running_ro(void)
{
	device_t dev;
#ifdef __PRE_RAM__
	dev = PCI_DEV(0, 0x1f, 0);
#else
	dev = dev_find_slot(0, PCI_DEVFN(0x1f,0));
#endif
	u16 gpio_base = pci_read_config32(dev, GPIOBASE) & 0xfffe;

	if (!gpio_base)
		return(0);

	/* GPIO68 EC_RW is active low.
	 * Treat as active high and let the caller invert if needed. */
	u32 gp_lvl3 = inl(gpio_base + GP_LVL3);
	return !((gp_lvl3 >> (68 - 64)) & 1);
}
