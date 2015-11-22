/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 * Copyright (C) 2015 Google Inc.
 * Copyright (C) 2015 Intel Corporation
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

#include <arch/acpi.h>
#include <cbfs.h>
#include <cbmem.h>
#include <console/console.h>
#include <device/device.h>
#include <soc/acpi.h>
#include <stdlib.h>
#include <string.h>
#include "ec.h"

static void mainboard_init(device_t dev)
{
	mainboard_ec_init();
}

static unsigned long mainboard_write_acpi_tables(
	device_t device, unsigned long current, acpi_rsdp_t *rsdp)
{
	acpi_header_t *header = (acpi_header_t *) (uintptr_t)current;
	global_nvs_t *gnvs = cbmem_find(CBMEM_ID_ACPI_GNVS);
	unsigned char *nhlt_bin;
	size_t nhlt_bin_len;
	void *nhlt;

	if (!gnvs)
		return current;

	nhlt_bin = cbfs_boot_map_with_leak("nhlt.bin", CBFS_TYPE_RAW,
					   &nhlt_bin_len);
	if (!nhlt_bin || !nhlt_bin_len)
		return current;

	printk(BIOS_DEBUG, "ACPI:    * NHLT\n");

	/* Create header */
	memset(header, 0, sizeof(acpi_header_t));
	memcpy(header->signature, "NHLT", 4);
	memcpy(header->oem_id, OEM_ID, 6);
	memcpy(header->oem_table_id, ACPI_TABLE_CREATOR, 8);
	memcpy(header->asl_compiler_id, ASLC, 4);

	header->revision = 5;
	header->length = sizeof(acpi_header_t) + nhlt_bin_len;
	current += sizeof(acpi_header_t);

	/* Add NHLT binary data */
	nhlt = (void *)(uintptr_t)current;
	memcpy(nhlt, nhlt_bin, nhlt_bin_len);
	current += nhlt_bin_len;
	ALIGN_CURRENT;

	acpi_add_table(rsdp, header);

	/* Update NHLT GNVS Data */
	gnvs->nhla = (uintptr_t)header;
	gnvs->nhll = header->length;

	return current;
}

/*
 * mainboard_enable is executed as first thing after
 * enumerate_buses().
 */
static void mainboard_enable(device_t dev)
{
	dev->ops->init = mainboard_init;
	dev->ops->write_acpi_tables = mainboard_write_acpi_tables;
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
