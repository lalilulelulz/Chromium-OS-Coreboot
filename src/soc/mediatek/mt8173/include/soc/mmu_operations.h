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

#ifndef __SOC_MEDIATEK_MT8173_MMU_OPERATIONS_H__
#define __SOC_MEDIATEK_MT8173_MMU_OPERATIONS_H__

#include <arch/mmu.h>

enum {
	DEV_MEM		= MA_DEV | MA_S  | MA_RW,
	CACHED_MEM	= MA_MEM | MA_NS | MA_RW,
	SECURE_MEM	= MA_MEM | MA_S  | MA_RW,
	UNCACHED_MEM	= MA_MEM | MA_NS | MA_RW | MA_MEM_NC,
};

extern unsigned char _sram_l2c[];
extern unsigned char _esram_l2c[];
#define _sram_l2c_size (_esram_l2c - _sram_l2c)

void mt8173_mmu_init(void);
void mt8173_mmu_after_dram(void);

#endif //__SOC_MEDIATEK_MT8173_MMU_OPERATIONS_H__
