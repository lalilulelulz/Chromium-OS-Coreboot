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

#include <arch/early_variables.h>
#include <assert.h>
#include <console/console.h>
#include <spi_flash.h>
#include <string.h>
#include <vb2_api.h>
#include <vboot_nvstorage.h>
#include "chromeos.h"
#include "vbnv.h"
#include "vbnv_layout.h"

#define BLOB_SIZE VB2_NVDATA_SIZE

struct vbnv_flash_ctx {
	/* VBNV flash is initialized */
	int initialized;

	/* Offset of the current nvdata in SPI flash */
	int blob_offset;

	/* Offset of the topmost nvdata blob in SPI flash */
	int top_offset;

	/* SPI flash handler used when saving data */
	struct spi_flash *flash;

	/* FMAP descriptor of the NVRAM area */
	struct region_device region;

	/* Cache of the current nvdata */
	uint8_t cache[BLOB_SIZE];
};
static struct vbnv_flash_ctx vbnv_flash CAR_GLOBAL;

/*
 * This code assumes that flash is erased to 1-bits, and write operations can
 * only change 1-bits to 0-bits. So if the new contents only change 1-bits to
 * 0-bits, we can reuse the current blob.
 */
static inline uint8_t erase_value(void)
{
	return 0xff;
}

static inline int can_overwrite(uint8_t current, uint8_t new)
{
	return (current & new) == new;
}

static int init_vbnv(void)
{
	struct vbnv_flash_ctx *ctx = car_get_var_ptr(&vbnv_flash);
	uint8_t buf[BLOB_SIZE];
	uint8_t empty_blob[BLOB_SIZE];
	int offset;
	int i;

	if (vboot_named_region_device("RW_NVRAM", &ctx->region) ||
	    region_device_sz(&ctx->region) < BLOB_SIZE) {
		printk(BIOS_ERR, "%s: failed to locate NVRAM\n", __func__);
		return 1;
	}

	/* Prepare an empty blob to compare against. */
	for (i = 0; i < BLOB_SIZE; i++)
		empty_blob[i] = erase_value();

	offset = 0;
	ctx->top_offset = region_device_sz(&ctx->region) - BLOB_SIZE;

	/*
	 * after the loop, offset is supposed to point the blob right before
	 * the first empty blob, the last blob in the nvram if there is no
	 * empty blob, or the base of the region if the nvram has never been
	 * used.
	 */
	for (i = 0; i <= ctx->top_offset; i += BLOB_SIZE) {
		if (rdev_readat(&ctx->region, buf, i, BLOB_SIZE) < 0) {
			printk(BIOS_ERR, "failed to read nvdata\n");
			return 1;
		}
		if (!memcmp(buf, empty_blob, BLOB_SIZE))
			break;
		offset = i;
	}

	/* reread the nvdata and write it to the cache */
	if (rdev_readat(&ctx->region, ctx->cache, offset, BLOB_SIZE) < 0) {
		printk(BIOS_ERR, "failed to read nvdata\n");
		return 1;
	}

	ctx->blob_offset = offset;
	ctx->initialized = 1;

	return 0;
}

static void vbnv_is_erasable(void)
{
	/*
	 * We check whether the region is aligned or not in advance to ensure
	 * we can erase the region when it's all used up.
	 *
	 * The region offset & size are determined by fmap.dts yet the check can
	 * be confidently done only by the spi flash driver. We use the same
	 * check as the one used by spi_flash_cmd_erase, which happens to be
	 * common to all the spi flash parts we support.
	 *
	 * TODO: Check by calling can_erase implemented by each spi flash driver
	 */
	struct vbnv_flash_ctx *ctx = car_get_var_ptr(&vbnv_flash);

	assert(!(region_device_offset(&ctx->region) % ctx->flash->sector_size));
	assert(!(region_device_sz(&ctx->region) % ctx->flash->sector_size));
}

static int vbnv_flash_probe(void)
{
	struct vbnv_flash_ctx *ctx = car_get_var_ptr(&vbnv_flash);

	if (!ctx->flash) {
		ctx->flash = spi_flash_probe(CONFIG_BOOT_MEDIA_SPI_BUS, 0);
		if (!ctx->flash) {
			printk(BIOS_ERR, "failed to probe spi flash\n");
			return 1;
		}
		/*
		 * Called here instead of init_vbnv to reduce impact on boot
		 * speed.
		 */
		vbnv_is_erasable();
	}

	/*
	 * Handle the case where spi_flash_probe returns a CAR_GLOBAL
	 * in early execution on x86 but then later is moved to RAM.
	 */
	ctx->flash = car_get_var_ptr(ctx->flash);

	return 0;
}

static int erase_nvram(void)
{
	struct vbnv_flash_ctx *ctx = car_get_var_ptr(&vbnv_flash);

	if (vbnv_flash_probe())
		return 1;

	if (ctx->flash->erase(ctx->flash, region_device_offset(&ctx->region),
			      region_device_sz(&ctx->region))) {
		printk(BIOS_ERR, "failed to erase nvram\n");
		return 1;
	}

	printk(BIOS_INFO, "nvram is cleared\n");
	return 0;
}

void read_vbnv_flash(uint8_t *vbnv_copy)
{
	struct vbnv_flash_ctx *ctx = car_get_var_ptr(&vbnv_flash);

	if (!ctx->initialized)
		if (init_vbnv())
			return;  /* error */

	memcpy(vbnv_copy, ctx->cache, BLOB_SIZE);
}

void save_vbnv_flash(const uint8_t *vbnv_copy)
{
	struct vbnv_flash_ctx *ctx = car_get_var_ptr(&vbnv_flash);
	int new_offset;
	int i;

	if (!ctx->initialized)
		if (init_vbnv())
			return;  /* error */

	/* Bail out if there have been no changes. */
	if (!memcmp(vbnv_copy, ctx->cache, BLOB_SIZE))
		return;

	new_offset = ctx->blob_offset;

	/* See if we can overwrite the current blob with the new one */
	for (i = 0; i < BLOB_SIZE; i++) {
		if (!can_overwrite(ctx->cache[i], vbnv_copy[i])) {
			/* unable to overwrite. need to use the next blob */
			new_offset += BLOB_SIZE;
			if (new_offset > ctx->top_offset) {
				if (erase_nvram())
					return;  /* error */
				new_offset = 0;
			}
			break;
		}
	}

	if (!vbnv_flash_probe() &&
	    !ctx->flash->write(ctx->flash,
			       region_device_offset(&ctx->region) + new_offset,
			       BLOB_SIZE, vbnv_copy)) {
		/* write was successful. safely move pointer forward */
		ctx->blob_offset = new_offset;
		memcpy(ctx->cache, vbnv_copy, BLOB_SIZE);
	} else {
		printk(BIOS_ERR, "failed to save nvdata\n");
	}
}
