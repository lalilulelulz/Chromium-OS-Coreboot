/*
 * This file is part of the coreboot project.
 *
 * Copyright 2016 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <arch/acpi.h>
#include <baseboard/variants.h>
#include <boardid.h>
#include <console/console.h>
#include <device/device.h>
#include <ec/ec.h>
#include <gpio.h>
#include <nhlt.h>
#include <smbios.h>
#include <soc/gpio.h>
#include <soc/nhlt.h>
#include <string.h>
#include <vendorcode/google/chromeos/chromeos.h>
#include <variant/ec.h>
#include <variant/gpio.h>

static void mainboard_init(void *chip_info)
{
	int boardid;
	const struct pad_config *pads;
	size_t num;

	boardid = board_id();
	printk(BIOS_INFO, "Board ID: %d\n", boardid);

	pads = variant_gpio_table(&num);
	gpio_configure_pads(pads, num);

	mainboard_ec_init();
}

/*
 * There are 2 pins on reef-like boards that can be used for SKU'ing
 * board differences. They each have optional stuffing for a pullup and
 * a pulldown. This way we can generate 9 different values with the
 * 2 pins.
 */
static int board_sku(void)
{
	static int board_sku_num = -1;
	gpio_t board_sku_gpios[] = {
		[1] = GPIO_17, [0] = GPIO_16,
	};
	const size_t num = ARRAY_SIZE(board_sku_gpios);

	if (board_sku_num < 0)
		board_sku_num = gpio_base3_value(board_sku_gpios, num);

	return board_sku_num;
}

const char *smbios_mainboard_sku(void)
{
	static char sku_str[5]; /* sku[0-8] */

	snprintf(sku_str, sizeof(sku_str), "sku%d", board_sku());

	return sku_str;
}

void __attribute__((weak)) variant_nhlt_oem_overrides(const char **oem_id,
						const char **oem_table_id,
						uint32_t *oem_revision)
{
	*oem_id = "reef";
	*oem_table_id = CONFIG_VARIANT_DIR;
	*oem_revision = board_sku();
}

static unsigned long mainboard_write_acpi_tables(
	device_t device, unsigned long current, acpi_rsdp_t *rsdp)
{
	uintptr_t start_addr;
	uintptr_t end_addr;
	struct nhlt *nhlt;
	const char *oem_id = NULL;
	const char *oem_table_id = NULL;
	uint32_t oem_revision = 0;

	start_addr = current;

	nhlt = nhlt_init();

	if (nhlt == NULL)
		return start_addr;

	variant_nhlt_init(nhlt);
	variant_nhlt_oem_overrides(&oem_id, &oem_table_id, &oem_revision);

	end_addr = nhlt_soc_serialize_oem_overrides(nhlt, start_addr,
			oem_id, oem_table_id, oem_revision);

	if (end_addr != start_addr)
		acpi_add_table(rsdp, (void *)start_addr);

	return end_addr;
}

static void mainboard_enable(device_t dev)
{
	dev->ops->write_acpi_tables = mainboard_write_acpi_tables;
	dev->ops->acpi_inject_dsdt_generator = chromeos_dsdt_generator;
}

struct chip_operations mainboard_ops = {
	.init = mainboard_init,
	.enable_dev = mainboard_enable,
};