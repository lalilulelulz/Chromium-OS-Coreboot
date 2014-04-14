/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 The Chromium OS Authors. All rights reserved.
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

#ifndef KIEV_GPIO_H
#define KIEV_GPIO_H

#include "southbridge/intel/bd82x6x/gpio.h"

/*
 * GPIO SET 1 includes GPIO0 to GPIO31
 */

const struct pch_gpio_set1 pch_gpio_set1_mode = {
	.gpio0  = GPIO_MODE_GPIO,	/* DEVELOPER SWITCH */
	.gpio1  = GPIO_MODE_GPIO,	/* USB3_SMI#  */
	.gpio2  = GPIO_MODE_NONE,	/* PIRQE# */
	.gpio3  = GPIO_MODE_NONE,	/* PIRQF# */
	.gpio4  = GPIO_MODE_NONE,	/* PIRQG# */
	.gpio5  = GPIO_MODE_NONE,	/* PIRQH# */
	.gpio6  = GPIO_MODE_NONE,	/* FAN TACH2 */
	.gpio7  = GPIO_MODE_NONE,	/* FAN TACH3 */
	.gpio8  = GPIO_MODE_GPIO,	/* GPIO_WAKE# */
	.gpio9  = GPIO_MODE_NATIVE,	/* OC5# USB */
	.gpio10 = GPIO_MODE_NATIVE,	/* OC6# USB */
	.gpio11 = GPIO_MODE_GPIO,	/* PANEL_ID2 */
	.gpio12 = GPIO_MODE_GPIO,	/* RECOVERY BUTTON */
	.gpio13 = GPIO_MODE_GPIO,	/* LPC_PME# */
	.gpio14 = GPIO_MODE_NATIVE,	/* OC7# USB */
	.gpio15 = GPIO_MODE_GPIO,	/* SMI# */
	.gpio16 = GPIO_MODE_NONE,	/* SATA4GP ? SHOULD THIS BE NATIVE */
	.gpio17 = GPIO_MODE_NONE,	/* FAN TACK0 */
	.gpio18 = GPIO_MODE_NONE,	/* NC - MOBILE ONLY */
	.gpio19 = GPIO_MODE_NONE,	/* SATA1GP */
	.gpio20 = GPIO_MODE_NONE,	/* PCIECLKRQ2# */
	.gpio21 = GPIO_MODE_NONE,	/* SATA0GP */
	.gpio22 = GPIO_MODE_GPIO,	/* DEBUG2 */
	.gpio23 = GPIO_MODE_NONE,	/* NC - LDRQ1 ? SHOULD THIS BE NATIVE */
	.gpio24 = GPIO_MODE_GPIO,	/* SKTOCC# */
	.gpio25 = GPIO_MODE_NONE,	/* NC - MOBILE ONLY */
	.gpio26 = GPIO_MODE_NONE,	/* NC - MOBILE ONLY */
	.gpio27 = GPIO_MODE_GPIO,	/* LPC_PME# */
	.gpio28 = GPIO_MODE_NONE,	/* NC */
	.gpio29 = GPIO_MODE_NONE,	/* NC - SLP_LAN# ? SHOULD BE NATIVE */
	.gpio30 = GPIO_MODE_NATIVE,	/* SUS_WARN# */
	.gpio31 = GPIO_MODE_NONE,	/* NC - PULLED UP */
};

const struct pch_gpio_set1 pch_gpio_set1_direction = {
	.gpio0  = GPIO_DIR_INPUT,
	.gpio1  = GPIO_DIR_INPUT,
	.gpio2  = GPIO_DIR_INPUT,
	.gpio3  = GPIO_DIR_INPUT,
	.gpio4  = GPIO_DIR_INPUT,
	.gpio5  = GPIO_DIR_INPUT,
	.gpio6  = GPIO_DIR_INPUT,
	.gpio7  = GPIO_DIR_INPUT,
	.gpio8  = GPIO_DIR_INPUT,
	.gpio9  = GPIO_DIR_INPUT,
	.gpio10 = GPIO_DIR_INPUT,
	.gpio11 = GPIO_DIR_INPUT,
	.gpio12 = GPIO_DIR_INPUT,
	.gpio13 = GPIO_DIR_INPUT,
	.gpio14 = GPIO_DIR_INPUT,
	.gpio15 = GPIO_DIR_INPUT,
	.gpio16 = GPIO_DIR_INPUT,
	.gpio17 = GPIO_DIR_INPUT,
	.gpio18 = GPIO_DIR_INPUT,
	.gpio19 = GPIO_DIR_INPUT,
	.gpio20 = GPIO_DIR_INPUT,
	.gpio21 = GPIO_DIR_INPUT,
	.gpio22 = GPIO_DIR_INPUT,
	.gpio23 = GPIO_DIR_INPUT,
	.gpio24 = GPIO_DIR_INPUT,
	.gpio25 = GPIO_DIR_INPUT,
	.gpio26 = GPIO_DIR_INPUT,
	.gpio27 = GPIO_DIR_INPUT,
	.gpio28 = GPIO_DIR_INPUT,
	.gpio29 = GPIO_DIR_INPUT,
	.gpio30 = GPIO_DIR_OUTPUT,
	.gpio31 = GPIO_DIR_INPUT,
};

const struct pch_gpio_set1 pch_gpio_set1_level = {
	.gpio0  = GPIO_LEVEL_LOW,
	.gpio1  = GPIO_LEVEL_LOW,
	.gpio2  = GPIO_LEVEL_LOW,
	.gpio3  = GPIO_LEVEL_LOW,
	.gpio4  = GPIO_LEVEL_LOW,
	.gpio5  = GPIO_LEVEL_LOW,
	.gpio6  = GPIO_LEVEL_LOW,
	.gpio7  = GPIO_LEVEL_LOW,
	.gpio8  = GPIO_LEVEL_HIGH,
	.gpio9  = GPIO_LEVEL_LOW,
	.gpio10 = GPIO_LEVEL_LOW,
	.gpio11 = GPIO_LEVEL_LOW,
	.gpio12 = GPIO_LEVEL_LOW,
	.gpio13 = GPIO_LEVEL_HIGH,
	.gpio14 = GPIO_LEVEL_LOW,
	.gpio15 = GPIO_LEVEL_HIGH,
	.gpio16 = GPIO_LEVEL_LOW,
	.gpio17 = GPIO_LEVEL_LOW,
	.gpio18 = GPIO_LEVEL_LOW,
	.gpio19 = GPIO_LEVEL_LOW,
	.gpio20 = GPIO_LEVEL_LOW,
	.gpio21 = GPIO_LEVEL_LOW,
	.gpio22 = GPIO_LEVEL_LOW,
	.gpio23 = GPIO_LEVEL_LOW,
	.gpio24 = GPIO_LEVEL_LOW,
	.gpio25 = GPIO_LEVEL_LOW,
	.gpio26 = GPIO_LEVEL_LOW,
	.gpio27 = GPIO_LEVEL_LOW,
	.gpio28 = GPIO_LEVEL_LOW,
	.gpio29 = GPIO_LEVEL_LOW,
	.gpio30 = GPIO_LEVEL_LOW,
	.gpio31 = GPIO_LEVEL_LOW,
};

const struct pch_gpio_set1 pch_gpio_set1_invert = {
	.gpio8   = GPIO_INVERT,
	.gpio13  = GPIO_INVERT,
	.gpio15  = GPIO_INVERT,
};

/*
 * GPIO SET 2 includes GPIO32 to GPIO63
 */

const struct pch_gpio_set2 pch_gpio_set2_mode = {
	.gpio32 = GPIO_MODE_GPIO,	/* BIOSWP */
	.gpio33 = GPIO_MODE_NONE,	/* NC */
	.gpio34 = GPIO_MODE_NONE,	/* NC */
	.gpio35 = GPIO_MODE_NONE,	/* NC - LED_LAN_ACT */
	.gpio36 = GPIO_MODE_NONE,	/* NC - SATA2GP */
	.gpio37 = GPIO_MODE_NONE,	/* NC - SATA3GP */
	.gpio38 = GPIO_MODE_GPIO,	/* DEBUG1 */
	.gpio39 = GPIO_MODE_NONE,	/* NC */
	.gpio40 = GPIO_MODE_NATIVE,	/* USB_OC1# */
	.gpio41 = GPIO_MODE_NATIVE,	/* USB_OC2# */
	.gpio42 = GPIO_MODE_NATIVE,	/* USB_OC3# */
	.gpio43 = GPIO_MODE_NATIVE,	/* USB_OC4# */
	.gpio44 = GPIO_MODE_GPIO,	/* CH_GPO_CTL# */
	.gpio45 = GPIO_MODE_GPIO,	/* PE1_ON# */
	.gpio46 = GPIO_MODE_GPIO,	/* PE2_ON# FOR DEBUG*/
	.gpio47 = GPIO_MODE_NONE,	/* NC */
	.gpio48 = GPIO_MODE_NONE,	/* NC */
	.gpio49 = GPIO_MODE_GPIO,	/* TEMP_ALERT */
	.gpio50 = GPIO_MODE_NONE,	/* NC */
	.gpio51 = GPIO_MODE_NONE,	/* NC */
	.gpio52 = GPIO_MODE_NONE,	/* NC */
	.gpio53 = GPIO_MODE_NONE,	/* NC */
	.gpio54 = GPIO_MODE_NONE,	/* NC */
	.gpio55 = GPIO_MODE_NONE,	/* NC */
	.gpio56 = GPIO_MODE_NONE,	/* NC - NOT FOUND */
	.gpio57 = GPIO_MODE_NONE,	/* NC */
	.gpio58 = GPIO_MODE_NATIVE,	/* SML1_SIO_CLK */
	.gpio59 = GPIO_MODE_NATIVE,	/* USB_OC0# */
	.gpio60 = GPIO_MODE_NONE,	/* NC */
	.gpio61 = GPIO_MODE_NONE,	/* NC */
	.gpio62 = GPIO_MODE_NONE,	/* NC */
	.gpio63 = GPIO_MODE_NONE,	/* NC */
};

const struct pch_gpio_set2 pch_gpio_set2_direction = {
	.gpio32 = GPIO_DIR_INPUT,
	.gpio33 = GPIO_DIR_INPUT,
	.gpio34 = GPIO_DIR_INPUT,
	.gpio35 = GPIO_DIR_OUTPUT,
	.gpio36 = GPIO_DIR_INPUT,
	.gpio37 = GPIO_DIR_INPUT,
	.gpio38 = GPIO_DIR_INPUT,
	.gpio39 = GPIO_DIR_INPUT,
	.gpio40 = GPIO_DIR_INPUT,
	.gpio41 = GPIO_DIR_INPUT,
	.gpio42 = GPIO_DIR_INPUT,
	.gpio43 = GPIO_DIR_INPUT,
	.gpio44 = GPIO_DIR_OUTPUT,
	.gpio45 = GPIO_DIR_OUTPUT,
	.gpio46 = GPIO_DIR_OUTPUT,
	.gpio47 = GPIO_DIR_INPUT,
	.gpio48 = GPIO_DIR_INPUT,
	.gpio49 = GPIO_DIR_INPUT,
	.gpio50 = GPIO_DIR_INPUT,
	.gpio51 = GPIO_DIR_INPUT,
	.gpio52 = GPIO_DIR_INPUT,
	.gpio53 = GPIO_DIR_INPUT,
	.gpio54 = GPIO_DIR_INPUT,
	.gpio55 = GPIO_DIR_INPUT,
	.gpio56 = GPIO_DIR_INPUT,
	.gpio57 = GPIO_DIR_INPUT,
	.gpio58 = GPIO_DIR_INPUT,
	.gpio59 = GPIO_DIR_INPUT,
	.gpio60 = GPIO_DIR_INPUT,
	.gpio61 = GPIO_DIR_INPUT,
	.gpio62 = GPIO_DIR_INPUT,
	.gpio63 = GPIO_DIR_INPUT,
};

const struct pch_gpio_set2 pch_gpio_set2_level = {
	.gpio32 = GPIO_LEVEL_LOW,
	.gpio33 = GPIO_LEVEL_LOW,
	.gpio34 = GPIO_LEVEL_LOW,
	.gpio35 = GPIO_LEVEL_HIGH,
	.gpio36 = GPIO_LEVEL_LOW,
	.gpio37 = GPIO_LEVEL_LOW,
	.gpio38 = GPIO_LEVEL_LOW,
	.gpio39 = GPIO_LEVEL_LOW,
	.gpio40 = GPIO_LEVEL_LOW,
	.gpio41 = GPIO_LEVEL_LOW,
	.gpio42 = GPIO_LEVEL_LOW,
	.gpio43 = GPIO_LEVEL_LOW,
	.gpio44 = GPIO_LEVEL_LOW,
	.gpio45 = GPIO_LEVEL_LOW,
	.gpio46 = GPIO_LEVEL_LOW,
	.gpio47 = GPIO_LEVEL_LOW,
	.gpio48 = GPIO_LEVEL_LOW,
	.gpio49 = GPIO_LEVEL_LOW,
	.gpio50 = GPIO_LEVEL_LOW,
	.gpio51 = GPIO_LEVEL_LOW,
	.gpio52 = GPIO_LEVEL_LOW,
	.gpio53 = GPIO_LEVEL_LOW,
	.gpio54 = GPIO_LEVEL_LOW,
	.gpio55 = GPIO_LEVEL_LOW,
	.gpio56 = GPIO_LEVEL_LOW,
	.gpio57 = GPIO_LEVEL_LOW,
	.gpio58 = GPIO_LEVEL_LOW,
	.gpio59 = GPIO_LEVEL_LOW,
	.gpio60 = GPIO_LEVEL_LOW,
	.gpio61 = GPIO_LEVEL_LOW,
	.gpio62 = GPIO_LEVEL_LOW,
	.gpio63 = GPIO_LEVEL_LOW,
};

/*
 * GPIO SET 3 includes GPIO64 to GPIO75
 */

const struct pch_gpio_set3 pch_gpio_set3_mode = {
	.gpio64 = GPIO_MODE_NONE,	/* NC */
	.gpio65 = GPIO_MODE_NONE,	/* NC */
	.gpio66 = GPIO_MODE_NONE,	/* NC */
	.gpio67 = GPIO_MODE_NATIVE,	/* PCH_CLK_48M */
	.gpio68 = GPIO_MODE_NONE,	/* NC - FAN TACK4 */
	.gpio69 = GPIO_MODE_NONE,	/* NC - FAN TACK5 */
	.gpio70 = GPIO_MODE_NONE,	/* NC - FAN TACK7 */
	.gpio71 = GPIO_MODE_NONE,	/* NC - FAN TACK8 */
	.gpio72 = GPIO_MODE_NONE,	/* NC - BATLOW# (pullup) */
	.gpio73 = GPIO_MODE_NONE,	/* NC */
	.gpio74 = GPIO_MODE_NONE,	/* NC */
	.gpio75 = GPIO_MODE_NATIVE,	/* SML1DATA */
};

const struct pch_gpio_set3 pch_gpio_set3_direction = {
	.gpio64 = GPIO_DIR_INPUT,
	.gpio65 = GPIO_DIR_INPUT,
	.gpio66 = GPIO_DIR_INPUT,
	.gpio67 = GPIO_DIR_OUTPUT,
	.gpio68 = GPIO_DIR_INPUT,
	.gpio69 = GPIO_DIR_INPUT,
	.gpio70 = GPIO_DIR_INPUT,
	.gpio71 = GPIO_DIR_INPUT,
	.gpio72 = GPIO_DIR_INPUT,
	.gpio73 = GPIO_DIR_INPUT,
	.gpio74 = GPIO_DIR_INPUT,
	.gpio75 = GPIO_DIR_INPUT,
};

const struct pch_gpio_set3 pch_gpio_set3_level = {
	.gpio64 = GPIO_LEVEL_LOW,
	.gpio65 = GPIO_LEVEL_LOW,
	.gpio66 = GPIO_LEVEL_LOW,
	.gpio67 = GPIO_LEVEL_LOW,
	.gpio68 = GPIO_LEVEL_LOW,
	.gpio69 = GPIO_LEVEL_LOW,
	.gpio70 = GPIO_LEVEL_LOW,
	.gpio71 = GPIO_LEVEL_LOW,
	.gpio72 = GPIO_LEVEL_LOW,
	.gpio73 = GPIO_LEVEL_LOW,
	.gpio74 = GPIO_LEVEL_LOW,
	.gpio75 = GPIO_LEVEL_LOW,
};

const struct pch_gpio_map kiev_gpio_map = {
	.set1 = {
		.mode      = &pch_gpio_set1_mode,
		.direction = &pch_gpio_set1_direction,
		.level     = &pch_gpio_set1_level,
		.invert    = &pch_gpio_set1_invert,
	},
	.set2 = {
		.mode      = &pch_gpio_set2_mode,
		.direction = &pch_gpio_set2_direction,
		.level     = &pch_gpio_set2_level,
	},
	.set3 = {
		.mode      = &pch_gpio_set3_mode,
		.direction = &pch_gpio_set3_direction,
		.level     = &pch_gpio_set3_level,
	},
};

#endif
