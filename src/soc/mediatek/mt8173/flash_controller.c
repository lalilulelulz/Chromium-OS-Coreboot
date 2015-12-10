/*
 * This file is part of the coreboot project.
 *
 * Copyright 2015 MediaTek Inc.
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

/* NOR Flash is clocked with 26MHz, from CLK26M -> TOP_SPINFI_IFR */

#include <arch/io.h>
#include <assert.h>
#include <console/console.h>
#include <spi_flash.h>
#include <spi-generic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <timer.h>
#include <soc/flash_controller.h>

#define get_nth_byte(d, n)	((d >> (8 * n)) & 0xff)

static int polling_cmd(u32 val)
{
	struct stopwatch sw;

	stopwatch_init_usecs_expire(&sw, SFLASH_POLLINGREG_US);

	while ((read32(&mt8173_nor->cmd) & val) != 0) {
		if (stopwatch_expired(&sw))
			return -1;
	}

	return 0;
}

static int mt8173_nor_execute_cmd(u8 cmdval)
{
	u8 val = cmdval & ~(SFLASH_AUTOINC);

	write8(&mt8173_nor->cmd, cmdval);
	return polling_cmd(val);
}

static int sflashhw_read_flash_status(u8 *value)
{
	if (mt8173_nor_execute_cmd(SFLASH_READSTATUS))
		return -1;

	*value = read8(&mt8173_nor->rdsr);
	return 0;
}

static int wait_for_write_done(void)
{
	struct stopwatch sw;
	u8 reg;

	stopwatch_init_usecs_expire(&sw, SFLASH_POLLINGREG_US);

	while (sflashhw_read_flash_status(&reg) == 0) {
		if (!(reg & SFLASH_WRITE_IN_PROGRESS))
			return 0;
		if (stopwatch_expired(&sw))
			return -1;
	}

	return -1;
}

/* set serial flash program address */
static void set_sfpaddr(u32 addr)
{
	write8(&mt8173_nor->radr[2], get_nth_byte(addr, 2));
	write8(&mt8173_nor->radr[1], get_nth_byte(addr, 1));
	write8(&mt8173_nor->radr[0], get_nth_byte(addr, 0));
}

static int sector_erase(int offset)
{
	if (wait_for_write_done())
		return -1;

	write8(&mt8173_nor->prgdata[5], SFLASH_OP_WREN);
	write8(&mt8173_nor->cnt, 8);
	mt8173_nor_execute_cmd(SFLASH_PRG_CMD);

	write8(&mt8173_nor->prgdata[5], SECTOR_ERASE_CMD);
	write8(&mt8173_nor->prgdata[4], get_nth_byte(offset, 2));
	write8(&mt8173_nor->prgdata[3], get_nth_byte(offset, 1));
	write8(&mt8173_nor->prgdata[2], get_nth_byte(offset, 0));
	write8(&mt8173_nor->cnt, 32);
	mt8173_nor_execute_cmd(SFLASH_PRG_CMD);

	if (wait_for_write_done())
		return -1;

	return 0;
}

unsigned int spi_crop_chunk(unsigned int cmd_len, unsigned int buf_len)
{
	return min(65535, buf_len);
}

static int nor_read(struct spi_flash *flash, u32 addr, size_t len, void *buf)
{
	u8 *buffer = (u8 *)buf;

	set_sfpaddr(addr);
	while (len) {
		if (mt8173_nor_execute_cmd(SFLASH_RD_TRIGGER | SFLASH_AUTOINC))
			return -1;

		*buffer++ = read8(&mt8173_nor->rdata);
		len--;
	}
	return 0;
}

static int nor_write(struct spi_flash *flash, u32 addr, size_t len,
		       const void *buf)
{
	const u8 *buffer = (const u8 *)buf;

	set_sfpaddr(addr);
	while (len) {
		write8(&mt8173_nor->wdata, *buffer);
		if (mt8173_nor_execute_cmd(SFLASH_WR_TRIGGER | SFLASH_AUTOINC))
			return -1;

		if (wait_for_write_done())
			return -1;
		buffer++;
		len--;
	}
	return 0;
}

static int nor_erase(struct spi_flash *flash, u32 offset, size_t len)
{
	int sector_start = offset;
	int sector_num = (u32)len / flash->sector_size;

	while (sector_num) {
		if (!sector_erase(sector_start)) {
			sector_start += flash->sector_size;
			sector_num--;
		} else {
			printk(BIOS_WARNING, "Erase failed at 0x%x!\n",
			       sector_start);
			return -1;
		}
	}
	return 0;
}

struct spi_flash *mt8173_nor_flash_probe(struct spi_slave *spi)
{
	static struct spi_flash flash = {0};

	if (flash.spi)
		return &flash;

	write32(&mt8173_nor->wrprot, SFLASH_COMMAND_ENABLE);
	flash.spi = spi;
	flash.name = "mt8173 flash controller";
	flash.write = nor_write;
	flash.erase = nor_erase;
	flash.read = nor_read;
	flash.status = 0;
	flash.sector_size = 0x1000;
	flash.erase_cmd = SECTOR_ERASE_CMD;
	flash.size = CONFIG_ROM_SIZE;
	return &flash;
}
