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

#include <bootblock_common.h>
#include <soc/mmu_operations.h>
#include <soc/mt6391.h>
#include <soc/pll.h>
#include <soc/rtc.h>
#include <soc/wdt.h>

void bootblock_soc_init(void)
{
	mt_pll_init();

	/* init pmic wrap SPI interface and pmic */
	mt6391_init();

	/* post init pll */
	mt_pll_post_init();

	mt8173_mmu_init();

	/* init watch dog, will disable AP watch dog */
	mtk_wdt_init();

	rtc_boot();
}
