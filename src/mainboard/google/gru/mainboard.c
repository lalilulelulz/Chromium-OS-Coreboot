/*
 * This file is part of the coreboot project.
 *
 * Copyright 2016 Rockchip Inc.
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
 */

#include <arch/cache.h>
#include <arch/io.h>
#include <boardid.h>
#include <boot/coreboot_tables.h>
#include <console/console.h>
#include <delay.h>
#include <device/device.h>
#include <device/i2c.h>
#include <gpio.h>
#include <soc/clock.h>
#include <soc/display.h>
#include <soc/emmc.h>
#include <soc/grf.h>
#include <soc/i2c.h>

static void configure_emmc(void)
{
	/* Host controller does not support programmable clock generator.
	 * If we don't do this setting, when we use phy to control the
	 * emmc clock(when clock exceed 50MHz), it will get wrong clock.
	 *
	 * Refer to TRM V0.3 Part 1 Chapter 15 PAGE 782 for this register.
	 * Please search "_CON11[7:0]" to locate register description.
	 */
	write32(&rk3399_grf->emmccore_con[11], RK_CLRSETBITS(0xff, 0));

	rkclk_configure_emmc();

	enable_emmc_clk();
}

static void configure_sdmmc(void)
{
	gpio_output(GPIO(4, D, 5), 1);  /* SDMMC_PWR_EN */
	gpio_output(GPIO(2, A, 2), 1);  /* SDMMC_SDIO_PWR_EN */

	/* SDMMC_DET_L is different on Kevin board revision 0. */
	if (IS_ENABLED(CONFIG_BOARD_GOOGLE_KEVIN) && (board_id() == 0))
		gpio_input(GPIO(4, D, 2));
	else
		gpio_input(GPIO(4, D, 0));

	gpio_output(GPIO(2, D, 4), 0);  /* Keep the max voltage */

	/*
	 * The SD card on this board is connected to port SDMMC0, which is
	 * multiplexed with GPIO4B pins 0..5.
	 *
	 * Disable all pullups on these pins. For pullup configuration
	 * register layout stacks banks 2 through 4 together, hence [2] means
	 * group 4, [1] means bank B. This register is described on page 342
	 * of section 1 of the TRM.
	 *
	 * Each GPIO pin's pull config takes two bits, writing zero to the
	 * field disables pull ups/downs, as described on page 342 of rk3399
	 * TRM Version 0.3 Part 1.
	 */
	write32(&rk3399_grf->gpio2_p[2][1], RK_CLRSETBITS(0xfff, 0));

	/*
	 * Set all outputs' drive strength to 8 mA. Group 4 bank B driver
	 * strength requires three bits per pin. Value of 2 written in that
	 * three bit field means '8 mA', as deduced from the kernel code.
	 *
	 * Thus the six pins involved in SDMMC interface require 18 bits to
	 * configure drive strength, but each 32 bit register provides only 16
	 * bits for this setting, this covers 5 pins fully and one bit from
	 * the 6th pin. Two more bits spill over to the next register. This is
	 * described on page 378 of rk3399 TRM Version 0.3 Part 1.
	 */
	write32(&rk3399_grf->gpio4b_e01,
		RK_CLRSETBITS(0xffff,
			      (2 << 0) | (2 << 3) |
			      (2 << 6) | (2 << 9) | (2 << 12)));
	write32(&rk3399_grf->gpio4b_e2, RK_CLRSETBITS(3, 1));

	/* And now set the multiplexor to enable SDMMC0. */
	write32(&rk3399_grf->iomux_sdmmc, IOMUX_SDMMC);
}

static void configure_display(void)
{
	/* set pinmux for edp HPD*/
	gpio_input_pulldown(GPIO(4, C, 7));
	write32(&rk3399_grf->iomux_edp_hotplug, IOMUX_EDP_HOTPLUG);

	gpio_output(GPIO(4, D, 3), 1); /* CPU3_EDP_VDDEN for P3.3V_DISP */
}

static void mainboard_init(device_t dev)
{
	configure_sdmmc();
	configure_emmc();
	configure_display();
}

static void enable_backlight_booster(void)
{
	const struct {
		uint8_t reg;
		uint8_t value;
	} i2c_writes[] = {
		{1, 0x84},
		{1, 0x85},
		{0, 0x26}
	};
	int i;
	const int booster_i2c_port = 0;
	uint8_t i2c_buf[2];
	struct i2c_seg i2c_command = { .read = 0, .chip = 0x2c,
				       .buf = i2c_buf, .len = sizeof(i2c_buf)
	};

	/*
	 * This function is called on Gru right after BL_EN is asserted. It
	 * takes time for the switcher chip to come online, let's wait a bit
	 * to let the voltage settle, so that the chip can be accessed.
	 */
	udelay(1000);

	/* Select pinmux for i2c0, which is the display backlight booster. */
	write32(&rk3399_pmugrf->iomux_i2c0_sda, IOMUX_I2C0_SDA);
	write32(&rk3399_pmugrf->iomux_i2c0_scl, IOMUX_I2C0_SCL);
	i2c_init(0, 100*KHz);

	for (i = 0; i < ARRAY_SIZE(i2c_writes); i++) {
		i2c_buf[0] = i2c_writes[i].reg;
		i2c_buf[1] = i2c_writes[i].value;
		i2c_transfer(booster_i2c_port, &i2c_command, 1);
	}
}

void mainboard_power_on_backlight(void)
{
	gpio_output(GPIO(1, C, 1), 1);  /* BL_EN */

	if (IS_ENABLED(CONFIG_BOARD_GOOGLE_GRU))
		enable_backlight_booster();
}

static void mainboard_enable(device_t dev)
{
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.name = CONFIG_MAINBOARD_PART_NUMBER,
	.enable_dev = mainboard_enable,
};
