/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 The ChromiumOS Authors.  All rights reserved.
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

#include <types.h>

#include <armv7.h>
#include <cbfs.h>
#include <cbmem.h>
#include <timestamp.h>

#include <arch/cache.h>
#include <soc/samsung/exynos5250/i2c.h>
#include <soc/samsung/exynos5250/clk.h>
#include <soc/samsung/exynos5250/cpu.h>
#include <soc/samsung/exynos5250/dmc.h>
#include <soc/samsung/exynos5250/gpio.h>
#include <soc/samsung/exynos5250/setup.h>
#include <soc/samsung/exynos5250/periph.h>
#include <soc/samsung/exynos5250/power.h>
#include <soc/samsung/exynos5250/trustzone.h>
#include <soc/samsung/exynos5250/wakeup.h>
#include <console/console.h>
#include <arch/stages.h>

#include <drivers/maxim/max77686/max77686.h>
#include <device/i2c.h>

#include "exynos5250.h"

#define PMIC_BUS	0

static void setup_power(int is_resume)
{
	int error = 0;

	power_init();

	if (is_resume) {
		return;
	}

	/* Initialize I2C bus to configure PMIC. */
	exynos_pinmux_i2c0();
	i2c_init(0, I2C_0_SPEED, 0x00);

	printk(BIOS_DEBUG, "%s: Setting up PMIC...\n", __func__);
	/*
	 * We're using CR1616 coin cell battery that is non-rechargeable
	 * battery. But, BBCHOSTEN bit of the BBAT Charger Register in
	 * MAX77686 is enabled by default for charging coin cell.
	 *
	 * Also, we cannot meet the coin cell reverse current spec. in UL
	 * standard if BBCHOSTEN bit is enabled.
	 *
	 * Disable Coin BATT Charging
	 */
	error = max77686_disable_backup_batt(PMIC_BUS);

	error |= max77686_volsetting(PMIC_BUS, PMIC_BUCK2, VDD_ARM_MV,
						REG_ENABLE, MAX77686_MV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_BUCK3, VDD_INT_UV,
						REG_ENABLE, MAX77686_UV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_BUCK1, VDD_MIF_MV,
						REG_ENABLE, MAX77686_MV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_BUCK4, VDD_G3D_MV,
						REG_ENABLE, MAX77686_MV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_LDO2, VDD_LDO2_MV,
						REG_ENABLE, MAX77686_MV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_LDO3, VDD_LDO3_MV,
						REG_ENABLE, MAX77686_MV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_LDO5, VDD_LDO5_MV,
						REG_ENABLE, MAX77686_MV);
	error |= max77686_volsetting(PMIC_BUS, PMIC_LDO10, VDD_LDO10_MV,
						REG_ENABLE, MAX77686_MV);

	error |= max77686_enable_32khz_cp(PMIC_BUS);

	if (error) {
		printk(BIOS_CRIT, "%s: PMIC error: %#x\n", __func__, error);
		die("Failed to intialize PMIC.\n");
	}
}

static void setup_graphics(void)
{
	exynos_pinmux_dphpd();
}

static void setup_gpio(void)
{
	gpio_direction_input(GPIO_D16); // WP_GPIO
	gpio_set_pull(GPIO_D16, GPIO_PULL_NONE);

	gpio_direction_input(GPIO_Y10); // RECMODE_GPIO
	gpio_set_pull(GPIO_Y10, GPIO_PULL_NONE);

	gpio_direction_input(GPIO_X35); // LID_GPIO
	gpio_set_pull(GPIO_X35, GPIO_PULL_NONE);

	gpio_direction_input(GPIO_X13); // POWER_GPIO
	gpio_set_pull(GPIO_X13, GPIO_PULL_NONE);
}

static void setup_memory(struct mem_timings *mem, int is_resume)
{
	printk(BIOS_SPEW, "man: 0x%x type: 0x%x, div: 0x%x, mhz: %d\n",
	       mem->mem_manuf,
	       mem->mem_type,
	       mem->mpll_mdiv,
	       mem->frequency_mhz);

	if (ddr3_mem_ctrl_init(mem, DMC_INTERLEAVE_SIZE, !is_resume)) {
		die("Failed to initialize memory controller.\n");
	}
}

static struct mem_timings *setup_clock(void)
{
	struct mem_timings *mem = get_mem_timings();
	struct arm_clk_ratios *arm_ratios = get_arm_clk_ratios();
	if (!mem) {
		die("Unable to auto-detect memory timings\n");
	}
	system_clock_init(mem, arm_ratios);
	return mem;
}

void main(void)
{
	struct mem_timings *mem;
	void *entry;
	int is_resume = (get_wakeup_state() != IS_NOT_WAKEUP);
#if CONFIG_COLLECT_TIMESTAMPS
	uint64_t start_romstage_time;
	uint64_t before_dram_time;
	uint64_t after_dram_time;
	uint64_t base_time = timestamp_get();

	start_romstage_time = timestamp_get();
#endif

	/* Clock must be initialized before console_init, otherwise you may need
	 * to re-initialize serial console drivers again. */
	mem = setup_clock();

	console_init();

	setup_power(is_resume);
#if CONFIG_COLLECT_TIMESTAMPS
	before_dram_time = timestamp_get();
#endif
	setup_memory(mem, is_resume);

#if CONFIG_COLLECT_TIMESTAMPS
	after_dram_time = timestamp_get();
#endif

	/* This needs to happen on normal boots and on resume. */
	trustzone_init();

	if (is_resume) {
		wakeup();
	}

	setup_gpio();
	setup_graphics();

	/* Set SPI (primary CBFS media) clock to 50MHz and configure pinmux. */
	exynos_pinmux_spi1();
	clock_set_rate(PERIPH_ID_SPI1, 50000000);

	cbmem_initialize_empty();

#if CONFIG_COLLECT_TIMESTAMPS
	timestamp_init(base_time);
	timestamp_add(TS_START_ROMSTAGE, start_romstage_time );
	timestamp_add(TS_BEFORE_INITRAM, before_dram_time );
	timestamp_add(TS_AFTER_INITRAM, after_dram_time );
#endif

	entry = cbfs_load_stage(CBFS_DEFAULT_MEDIA, "fallback/coreboot_ram");

#if CONFIG_COLLECT_TIMESTAMPS
	timestamp_add_now(TS_END_ROMSTAGE);
#endif

	stage_exit(entry);
}
