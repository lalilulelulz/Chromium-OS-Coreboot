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

#ifndef STOUT_GPIO_H
#define STOUT_GPIO_H

#include "southbridge/intel/bd82x6x/gpio.h"

const struct pch_gpio_set1 pch_gpio_set1_mode = {
	.gpio0  = GPIO_MODE_NONE,	/* NOT USED */
	.gpio1  = GPIO_MODE_GPIO,	/* SIO_EXT_SMI# / TACH1 */
	.gpio2  = GPIO_MODE_GPIO,	/* DOCK_ID0 / PIRQE# */
	.gpio3  = GPIO_MODE_GPIO,	/* DOCK_ID1 / PIRQF# */
	.gpio4  = GPIO_MODE_GPIO,	/* DOCK_ID2 / PIRQG# */
	.gpio5  = GPIO_MODE_GPIO,	/* INTH# / PIRQH# */
	.gpio6  = GPIO_MODE_GPIO,	/* SIO_EXT_SCI# / FAN TACH2 */
	.gpio7  = GPIO_MODE_GPIO,	/* EXPRCRD_PWREN# / FAN TACH3 */
	.gpio8  = GPIO_MODE_NONE,	/* NOT USED */
	.gpio9  = GPIO_MODE_NATIVE,	/* USB_OC5# / OC5# USB */
	.gpio10 = GPIO_MODE_NATIVE,	/* USB_OC6# / OC6# USB */
	.gpio11 = GPIO_MODE_NATIVE,	/* SMBALERT# / SMB_ALERT*/
	.gpio12 = GPIO_MODE_NONE,	/* NOT USED / LAN_PHY_PWR_CTRL */
	.gpio13 = GPIO_MODE_NONE,	/* NOT USED / HDA_DOCK_RST */
	.gpio14 = GPIO_MODE_NATIVE,	/* USB_OC7# / OC7# USB */
	.gpio15 = GPIO_MODE_GPIO,	/* TLS Confidentiality */
	.gpio16 = GPIO_MODE_GPIO,	/* WWAN_LED_ON / SATA4GP */
	.gpio17 = GPIO_MODE_GPIO,	/* WLAN_LED_ON / FAN TACH0 */
	.gpio18 = GPIO_MODE_NATIVE,	/* PCIE_CLKREQ_WLAN# / PCIECLKRQ1# */
	.gpio19 = GPIO_MODE_GPIO,	/* BBS_BIT0 / SATA1GP */
	.gpio20 = GPIO_MODE_NATIVE,	/* PCIE_CLKREQ_CARD# / PCIECLKRQ2# */
	.gpio21 = GPIO_MODE_GPIO,	/* BT_DET# / SATA0GP */
	.gpio22 = GPIO_MODE_GPIO,	/* MODEL_ID0 / SCLOCK */
	.gpio23 = GPIO_MODE_GPIO,	/* LCD_BK_OFF / LDRQ1# */
	.gpio24 = GPIO_MODE_NONE,	/* NOT USED / MEM_LED */
	.gpio25 = GPIO_MODE_NATIVE,	/* PCI_CLKREQ_WWAN# / PCIECLKRQ3# */
	.gpio26 = GPIO_MODE_NATIVE,	/* CLK_PCIE_REQ4# / PCIECLKRQ4# */
	.gpio27 = GPIO_MODE_GPIO,	/* MSATA_DTCT# */
	.gpio28 = GPIO_MODE_GPIO,	/* On-Die PLL Voltage Regulator */
	.gpio29 = GPIO_MODE_GPIO,	/* WLAN_AOAC_ON / SLP_LAN# */
	.gpio30 = GPIO_MODE_GPIO,	/* SUS_PWR_ACK / SUS_WARN# */
	.gpio31 = GPIO_MODE_NATIVE,	/* AC_PRESENT / ACPRESENT */
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
	.gpio16 = GPIO_DIR_OUTPUT,
	.gpio17 = GPIO_DIR_OUTPUT,
	.gpio18 = GPIO_DIR_INPUT,
	.gpio19 = GPIO_DIR_INPUT,
	.gpio20 = GPIO_DIR_INPUT,
	.gpio21 = GPIO_DIR_INPUT,
	.gpio22 = GPIO_DIR_INPUT,
	.gpio23 = GPIO_DIR_OUTPUT,
	.gpio24 = GPIO_DIR_INPUT,
	.gpio25 = GPIO_DIR_INPUT,
	.gpio26 = GPIO_DIR_INPUT,
	.gpio27 = GPIO_DIR_INPUT,
	.gpio28 = GPIO_DIR_OUTPUT,
	.gpio29 = GPIO_DIR_OUTPUT,
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
	.gpio8  = GPIO_LEVEL_LOW,
	.gpio9  = GPIO_LEVEL_LOW,
	.gpio10 = GPIO_LEVEL_LOW,
	.gpio11 = GPIO_LEVEL_LOW,
	.gpio12 = GPIO_LEVEL_LOW,
	.gpio13 = GPIO_LEVEL_LOW,
	.gpio14 = GPIO_LEVEL_LOW,
	.gpio15 = GPIO_LEVEL_LOW,
	.gpio16 = GPIO_LEVEL_HIGH,
	.gpio17 = GPIO_LEVEL_HIGH,
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
	.gpio28 = GPIO_LEVEL_HIGH,
	.gpio29 = GPIO_LEVEL_LOW,
	.gpio30 = GPIO_LEVEL_LOW,
	.gpio31 = GPIO_LEVEL_LOW,
};

const struct pch_gpio_set2 pch_gpio_set2_mode = {
	.gpio32 = GPIO_MODE_NATIVE, /* PCI_CLKRUN# / CLKRUN# */
	.gpio33 = GPIO_MODE_NONE,   /* NOT USED / HDA_DOCK_EN# */
	.gpio34 = GPIO_MODE_GPIO,   /* CCD_ON / STP_PCI# */
	.gpio35 = GPIO_MODE_GPIO,   /* BT_ON */
	.gpio36 = GPIO_MODE_GPIO,   /* BOARD_ID0 / SATA2GP */
	.gpio37 = GPIO_MODE_GPIO,   /* BOARD_ID1 / SATA3GP */
	.gpio38 = GPIO_MODE_GPIO,   /* BOARD_ID2 / SLOAD */
	.gpio39 = GPIO_MODE_GPIO,   /* BOARD_ID3 / SDATAOUT0 */
	.gpio40 = GPIO_MODE_NATIVE, /* USB_OC1# / OC1# */
	.gpio41 = GPIO_MODE_NATIVE, /* USB_OC2# / OC2# */
	.gpio42 = GPIO_MODE_NATIVE, /* USB_OC3# / OC3# */
	.gpio43 = GPIO_MODE_NATIVE, /* USB_OC4_AUO4# / OC4# */
	.gpio44 = GPIO_MODE_NATIVE, /* PCIE_CLKREQ_LAN# / PCIECLKRQ5# */
	.gpio45 = GPIO_MODE_NATIVE, /* PCIECLKRQ6# */
	.gpio46 = GPIO_MODE_NATIVE, /* PCICLKRQ7# */
	.gpio47 = GPIO_MODE_NATIVE, /* CLK_PEGA_REQ# / PEG_A_CLKRQ# */
	.gpio48 = GPIO_MODE_GPIO,   /* DIS_BT_ON# / SDATAOUT1 */
	.gpio49 = GPIO_MODE_NONE,   /* NOT USED / SATA5GP */
	.gpio50 = GPIO_MODE_NATIVE, /* PCI_REQ1# / REQ1# */
	.gpio51 = GPIO_MODE_GPIO,   /* BBS_BIT1 / GNT1# */
	.gpio52 = GPIO_MODE_NATIVE, /* PCI_REQ2# / REQ2# */
	.gpio53 = GPIO_MODE_GPIO,   /* PWM_SELECT# / GNT2# */
	.gpio54 = GPIO_MODE_NATIVE, /* PCI_REQ3# / REQ3# */
	.gpio55 = GPIO_MODE_NATIVE, /* PCI_GNT3# / GNT3# */
	.gpio56 = GPIO_MODE_NATIVE, /* CLK_PEGB_REQ# / PEG_B_CLKRQ# */
	.gpio57 = GPIO_MODE_NONE,   /* NOT USED? (PCH_GPIO57) */
	.gpio58 = GPIO_MODE_NATIVE, /* SMB_ME1_CLK / SML1CLK */
	.gpio59 = GPIO_MODE_NATIVE, /* USB_OC0_1# / OC0# */
	.gpio60 = GPIO_MODE_GPIO,   /* DRAMRST_CNTRL_PCH / SML0ALERT# */
	.gpio61 = GPIO_MODE_GPIO,   /* LPCPD# / SUS_STAT# */
	.gpio62 = GPIO_MODE_NATIVE, /* PCH_SUSCLK_L / SUSCLK */
	.gpio63 = GPIO_MODE_NONE,   /* NOT USED / SLP_S5# */
};

const struct pch_gpio_set2 pch_gpio_set2_direction = {
	.gpio32 = GPIO_DIR_INPUT,
	.gpio33 = GPIO_DIR_INPUT,
	.gpio34 = GPIO_DIR_OUTPUT,
	.gpio35 = GPIO_DIR_OUTPUT,
	.gpio36 = GPIO_DIR_INPUT,
	.gpio37 = GPIO_DIR_INPUT,
	.gpio38 = GPIO_DIR_INPUT,
	.gpio39 = GPIO_DIR_INPUT,
	.gpio40 = GPIO_DIR_INPUT,
	.gpio41 = GPIO_DIR_INPUT,
	.gpio42 = GPIO_DIR_INPUT,
	.gpio43 = GPIO_DIR_INPUT,
	.gpio44 = GPIO_DIR_INPUT,
	.gpio45 = GPIO_DIR_INPUT,
	.gpio46 = GPIO_DIR_INPUT,
	.gpio47 = GPIO_DIR_INPUT,
	.gpio48 = GPIO_DIR_OUTPUT,
	.gpio49 = GPIO_DIR_INPUT,
	.gpio50 = GPIO_DIR_INPUT,
	.gpio51 = GPIO_DIR_OUTPUT,
	.gpio52 = GPIO_DIR_INPUT,
	.gpio53 = GPIO_DIR_OUTPUT,
	.gpio54 = GPIO_DIR_INPUT,
	.gpio55 = GPIO_DIR_INPUT,
	.gpio56 = GPIO_DIR_INPUT,
	.gpio57 = GPIO_DIR_INPUT,
	.gpio58 = GPIO_DIR_INPUT,
	.gpio59 = GPIO_DIR_INPUT,
	.gpio60 = GPIO_DIR_OUTPUT,
	.gpio61 = GPIO_DIR_OUTPUT,
	.gpio62 = GPIO_DIR_INPUT,
	.gpio63 = GPIO_DIR_INPUT,
};

const struct pch_gpio_set2 pch_gpio_set2_level = {
	.gpio32 = GPIO_LEVEL_LOW,
	.gpio33 = GPIO_LEVEL_LOW,
	.gpio34 = GPIO_LEVEL_LOW,
	.gpio35 = GPIO_LEVEL_LOW,
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
	.gpio53 = GPIO_LEVEL_HIGH,
	.gpio54 = GPIO_LEVEL_LOW,
	.gpio55 = GPIO_LEVEL_LOW,
	.gpio56 = GPIO_LEVEL_LOW,
	.gpio57 = GPIO_LEVEL_LOW,
	.gpio58 = GPIO_LEVEL_LOW,
	.gpio59 = GPIO_LEVEL_LOW,
	.gpio60 = GPIO_LEVEL_LOW,
	.gpio61 = GPIO_LEVEL_HIGH,
	.gpio62 = GPIO_LEVEL_LOW,
	.gpio63 = GPIO_LEVEL_LOW,
};

const struct pch_gpio_set3 pch_gpio_set3_mode = {
	.gpio64 = GPIO_MODE_NATIVE,	/* CLK_FLEX0 */
	.gpio65 = GPIO_MODE_NATIVE,	/* CLK_FLEX1 */
	.gpio66 = GPIO_MODE_NATIVE,	/* CLK_FLEX2 */
	.gpio67 = GPIO_MODE_NATIVE,	/* CLK_FLEX3 */
	.gpio68 = GPIO_MODE_GPIO,	/* WWAN_DTCT# / FAN TACK4 */
	.gpio69 = GPIO_MODE_NONE,	/* NOT USED / FAN TACK5 */
	.gpio70 = GPIO_MODE_GPIO,	/* WLAN_OFF# / FAN TACK7 */
	.gpio71 = GPIO_MODE_GPIO,	/* WWAN_OFF# / FAN TACK8 */
	.gpio72 = GPIO_MODE_NATIVE,	/* PM_BATLOW# / BATLOW# */
	.gpio73 = GPIO_MODE_NATIVE,	/* PCIECLKRQ0#*/
	.gpio74 = GPIO_MODE_NATIVE,	/* SML1ALERT#_R / SML1ALERT# /PCHHOT# */
	.gpio75 = GPIO_MODE_NATIVE,	/* SMB_ME1_DAT / SML1DATA */
};

const struct pch_gpio_set3 pch_gpio_set3_direction = {
	.gpio64 = GPIO_DIR_INPUT,
	.gpio65 = GPIO_DIR_INPUT,
	.gpio66 = GPIO_DIR_INPUT,
	.gpio67 = GPIO_DIR_INPUT,
	.gpio68 = GPIO_DIR_OUTPUT,
	.gpio69 = GPIO_DIR_INPUT,
	.gpio70 = GPIO_DIR_OUTPUT,
	.gpio71 = GPIO_DIR_OUTPUT,
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
	.gpio70 = GPIO_LEVEL_HIGH, /* Set default to WLAN enabled */
	.gpio71 = GPIO_LEVEL_LOW,
	.gpio72 = GPIO_LEVEL_LOW,
	.gpio73 = GPIO_LEVEL_LOW,
	.gpio74 = GPIO_LEVEL_LOW,
	.gpio75 = GPIO_LEVEL_LOW,
};

const struct pch_gpio_map stout_gpio_map = {
	.set1 = {
		.mode      = &pch_gpio_set1_mode,
		.direction = &pch_gpio_set1_direction,
		.level     = &pch_gpio_set1_level,
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
