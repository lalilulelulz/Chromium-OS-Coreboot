/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 The ChromiumOS Authors.  All rights reserved.
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

#include <boot/coreboot_tables.h>
#include <boot_device.h>
#include <cbmem.h>
#include <console/cbmem_console.h>
#include <console/console.h>
#include <arch/early_variables.h>
#include <fmap.h>
#include <reset.h>
#include <stddef.h>
#include <string.h>

#define NEED_VB20_INTERNALS  /* TODO: remove me! */
#include <vb2_api.h>
#include "chromeos.h"
#include "vboot_common.h"
#include "vboot_handoff.h"

static int gbb_enable_external_console CAR_GLOBAL;

int vboot_named_region_device(const char *name, struct region_device *rdev)
{
	return fmap_locate_area_as_rdev(name, rdev);
}

int vboot_region_device(const struct region *reg, struct region_device *rdev)
{
	return boot_device_ro_subregion(reg, rdev);
}

int vboot_get_handoff_info(void **addr, uint32_t *size)
{
	struct vboot_handoff *vboot_handoff;

	vboot_handoff = cbmem_find(CBMEM_ID_VBOOT_HANDOFF);

	if (vboot_handoff == NULL)
		return -1;

	*addr = vboot_handoff;
	*size = sizeof(*vboot_handoff);
	return 0;
}

static int vboot_handoff_flag(uint32_t flag)
{
	struct vboot_handoff *vbho;

	vbho = cbmem_find(CBMEM_ID_VBOOT_HANDOFF);

	if (vbho == NULL)
		return 0;

	return !!(vbho->init_params.out_flags & flag);
}

int vboot_skip_display_init(void)
{
	return !vboot_handoff_flag(VB_INIT_OUT_ENABLE_DISPLAY);
}

int vboot_enable_developer(void)
{
	return vboot_handoff_flag(VB_INIT_OUT_ENABLE_DEVELOPER);
}

int vboot_enable_recovery(void)
{
	return vboot_handoff_flag(VB_INIT_OUT_ENABLE_RECOVERY);
}

int vboot_enable_external_console(void)
{
	return car_get_var(gbb_enable_external_console);
}

void vboot_reboot(void)
{
	if (IS_ENABLED(CONFIG_CONSOLE_CBMEM_DUMP_TO_UART))
		cbmem_dump_console();
	hard_reset();
	die("failed to reboot");
}

static int vboot_get_gbb_flags(void)
{
	struct region_device rdev;
	struct vb2_gbb_header *gbb;
	uint32_t gbb_flags = 0;

	if (vboot_named_region_device("GBB", &rdev))
		return 0;

	gbb = rdev_mmap(&rdev, 0, EXPECTED_VB2_GBB_HEADER_SIZE);

	/* Check that gbb_header matches gbb signature */
	if (gbb != NULL && !memcmp(gbb->signature, VB2_GBB_SIGNATURE,
				   VB2_GBB_SIGNATURE_SIZE))
		gbb_flags = gbb->flags;

	rdev_munmap(&rdev, gbb);

	return gbb_flags;
}

void vboot_setup_enable_external_console(void)
{
	car_set_var(gbb_enable_external_console,
		    !!(vboot_get_gbb_flags() & VB2_GBB_FLAG_ENABLE_SERIAL));
}
