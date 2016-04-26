/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

#ifndef MAINBOARD_GPIO_H
#define MAINBOARD_GPIO_H

#include <soc/gpe.h>
#include <soc/gpio.h>

/* MAX98357A SD_MODE */
#define GPIO_SPEAKER_MAXIM_AMP_SDMODE	GPP_E3

/* EC in RW */
#define GPIO_EC_IN_RW		GPP_C6

/* BIOS Flash Write Protect */
#define GPIO_PCH_WP		GPP_C23

/* Memory configuration board straps */
#define GPIO_MEM_CONFIG_0	GPP_C12
#define GPIO_MEM_CONFIG_1	GPP_C13
#define GPIO_MEM_CONFIG_2	GPP_C14
#define GPIO_MEM_CONFIG_3	GPP_C15

/* EC wake is LAN_WAKE# which is a special DeepSX wake pin */
#define GPE_EC_WAKE		GPE0_LAN_WAK

/* GPP_B16 is WLAN_WAKE. GPP_B group is routed to DW0 in the GPE0 block */
#define GPE_WLAN_WAKE		GPE0_DW0_16

/* Input device interrupt configuration */
#define TOUCHPAD_INT_L		GPP_B3_IRQ
#define TOUCHSCREEN_INT_L	GPP_E7_IRQ
#define MIC_INT_L		GPP_F10_IRQ

/* GPP_E16 is EC_SCI_L. GPP_E group is routed to DW2 in the GPE0 block */
#define EC_SCI_GPI		GPE0_DW2_16
#define EC_SMI_GPI		GPP_E15

/* Power rail control signals. */
#define EN_PP3300_KEPLER	GPP_C11
#define EN_PP3300_DX_TOUCH	GPP_C22
#define EN_PP3300_DX_EMMC	GPP_D5
#define EN_PP1800_DX_EMMC	GPP_D6
#define EN_PP3300_DX_CAM	GPP_D12

#ifndef __ACPI__
/* Pad configuration in ramstage. */
static const struct pad_config gpio_table[] = {
/* RCIN# */		PAD_CFG_NF(GPP_A0, NONE, DEEP, NF1),
/* LAD0 */		PAD_CFG_NF(GPP_A1, 20K_PU, DEEP, NF1),
/* LAD1 */		PAD_CFG_NF(GPP_A2, 20K_PU, DEEP, NF1),
/* LAD2 */		PAD_CFG_NF(GPP_A3, 20K_PU, DEEP, NF1),
/* LAD3 */		PAD_CFG_NF(GPP_A4, 20K_PU, DEEP, NF1),
/* LFRAME# */		PAD_CFG_NF(GPP_A5, NONE, DEEP, NF1),
/* SERIRQ */		PAD_CFG_NF(GPP_A6, NONE, DEEP, NF1),
/* SD_CD_INT_L */	PAD_CFG_GPI(GPP_A7, NONE, DEEP),
/* CLKRUN# */		PAD_CFG_NF(GPP_A8, NONE, DEEP, NF1),
/* CLKOUT_LPC0 */	PAD_CFG_NF(GPP_A9, NONE, DEEP, NF1),
/* CLKOUT_LPC1 */	PAD_CFG_NC(GPP_A10),
/* PME# */		PAD_CFG_NF(GPP_A11, NONE, DEEP, NF1),
/* BM_BUSY# */		PAD_CFG_NC(GPP_A12),
/* SUSWARN# */		PAD_CFG_NF(GPP_A13, NONE, DEEP, NF1),
/* SUS_STAT# */		PAD_CFG_NF(GPP_A14, NONE, DEEP, NF1),
/* SUSACK# */		PAD_CFG_NF(GPP_A15, NONE, DEEP, NF1),
/* SD_1P8_SEL */	PAD_CFG_NF(GPP_A16, NONE, DEEP, NF1),
/* SD_PWR_EN# */	PAD_CFG_NF(GPP_A17, 20K_PU, DEEP, NF1),
/* ISH_GP0 */		PAD_CFG_GPI(GPP_A18, 20K_PU, DEEP),
/* ISH_GP1 */		PAD_CFG_NC(GPP_A19),
/* ISH_GP2 */		PAD_CFG_GPI(GPP_A20, 20K_PU, DEEP),
/* ISH_GP3 */		PAD_CFG_GPI(GPP_A21, 20K_PU, DEEP),
/* ISH_GP4 */		PAD_CFG_GPI(GPP_A22, 20K_PU, DEEP),
/* ISH_GP5 */		PAD_CFG_NC(GPP_A23),
/* CORE_VID0 */		PAD_CFG_NF(GPP_B0, NONE, DEEP, NF1),
/* CORE_VID1 */		PAD_CFG_NF(GPP_B1, NONE, DEEP, NF1),
/* VRALERT# */		PAD_CFG_GPI(GPP_B2, NONE, DEEP),
/* CPU_GP2 */		PAD_CFG_GPI_APIC(GPP_B3, NONE, PLTRST), /* TRACKPAD */
/* CPU_GP3 */		PAD_CFG_NC(GPP_B4),
/* SRCCLKREQ0# */	PAD_CFG_GPI_ACPI_SCI(GPP_B5, NONE, DEEP, YES), /* TRACKPAD WAKE */
/* SRCCLKREQ1# */	PAD_CFG_GPI(GPP_B6, NONE, DEEP), /* WLAN */
/* SRCCLKREQ2# */	PAD_CFG_NC(GPP_B7),
/* SRCCLKREQ3# */	PAD_CFG_NC(GPP_B8),
/* SRCCLKREQ4# */	PAD_CFG_NC(GPP_B9),
/* SRCCLKREQ5# */	PAD_CFG_NC(GPP_B10),
/* EXT_PWR_GATE# */	PAD_CFG_NC(GPP_B11),
/* SLP_S0# */		PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
/* PLTRST# */		PAD_CFG_NF(GPP_B13, 20K_PU, DEEP, NF1),
/* SPKR */		PAD_CFG_GPO(GPP_B14, 0, DEEP),
/* GSPI0_CS# */		PAD_CFG_NC(GPP_B15),
/* GSPI0_CLK */		PAD_CFG_GPI_ACPI_SCI(GPP_B16, NONE, DEEP, YES), /* WLAN WAKE */
/* GSPI0_MISO */	PAD_CFG_NC(GPP_B17),
/* GSPI0_MOSI */	PAD_CFG_GPI(GPP_B18, 20K_PD, DEEP),
/* GSPI1_CS# */		PAD_CFG_NC(GPP_B19),
/* GSPI1_CLK */		PAD_CFG_NC(GPP_B20),
/* GSPI1_MISO */	PAD_CFG_NC(GPP_B21),
/* GSPI1_MOSI */	PAD_CFG_GPI(GPP_B22, 20K_PD, DEEP),
/* SM1ALERT# */		PAD_CFG_GPO(GPP_B23, 0, DEEP),
/* SMBCLK */		PAD_CFG_NF(GPP_C0, NONE, DEEP, NF1), /* XDP */
/* SMBDATA */		PAD_CFG_NF(GPP_C1, NONE, DEEP, NF1), /* XDP */
/* SMBALERT# */		PAD_CFG_GPO(GPP_C2, 0, DEEP),
/* SML0CLK */		PAD_CFG_NC(GPP_C3),
/* SML0DATA */		PAD_CFG_NC(GPP_C4),
/* SML0ALERT# */	PAD_CFG_GPO(GPP_C5, 0, DEEP),
/* SM1CLK */		PAD_CFG_GPI(GPP_C6, 20K_PU, DEEP), /* EC_IN_RW */
/* SM1DATA */		PAD_CFG_NC(GPP_C7),
/* UART0_RXD */		PAD_CFG_NC(GPP_C8),
/* UART0_TXD */		PAD_CFG_NC(GPP_C9),
/* UART0_RTS# */	PAD_CFG_NC(GPP_C10),
/* UART0_CTS# */	PAD_CFG_NC(GPP_C11),
/* UART1_RXD */		PAD_CFG_GPI(GPP_C12, NONE, DEEP), /* MEM_CONFIG[0] */
/* UART1_TXD */		PAD_CFG_GPI(GPP_C13, NONE, DEEP), /* MEM_CONFIG[1] */
/* UART1_RTS# */	PAD_CFG_GPI(GPP_C14, NONE, DEEP), /* MEM_CONFIG[2] */
/* UART1_CTS# */	PAD_CFG_GPI(GPP_C15, NONE, DEEP), /* MEM_CONFIG[3] */
/* I2C0_SDA */		PAD_CFG_NF(GPP_C16, NONE, DEEP, NF1), /* TOUCHSCREEN */
/* I2C0_SCL */		PAD_CFG_NF(GPP_C17, NONE, DEEP, NF1), /* TOUCHSCREEN */
/* I2C1_SDA */		PAD_CFG_NF(GPP_C18, NONE, DEEP, NF1), /* TRACKPAD */
/* I2C1_SCL */		PAD_CFG_NF(GPP_C19, NONE, DEEP, NF1), /* TRACKPAD */
/* UART2_RXD */		PAD_CFG_NF(GPP_C20, NONE, DEEP, NF1), /* SERVO */
/* UART2_TXD */		PAD_CFG_NF(GPP_C21, NONE, DEEP, NF1), /* SERVO */
/* UART2_RTS# */	PAD_CFG_GPO(GPP_C22, 1, DEEP), /* EN_PP3300_DX_TOUCH */
/* UART2_CTS# */	PAD_CFG_GPI(GPP_C23, 20K_PU, DEEP), /* PCH_WP */
/* SPI1_CS# */		PAD_CFG_NC(GPP_D0),
/* SPI1_CLK */		PAD_CFG_NC(GPP_D1),
/* SPI1_MISO */		PAD_CFG_NC(GPP_D2),
/* SPI1_MOSI */		PAD_CFG_NC(GPP_D3),
/* FASHTRIG */		PAD_CFG_NC(GPP_D4),
/* ISH_I2C0_SDA */	PAD_CFG_GPO(GPP_D5, 1, DEEP), /* EN_PP3300_DX_EMMC */
/* ISH_I2C0_SCL */	PAD_CFG_GPO(GPP_D6, 1, DEEP), /* EN_PP1800_DX_EMMC */
/* ISH_I2C1_SDA */	PAD_CFG_NF(GPP_D7, NONE, DEEP, NF1),
/* ISH_I2C1_SCL */	PAD_CFG_NF(GPP_D8, NONE, DEEP, NF1),
			PAD_CFG_NC(GPP_D9),
			PAD_CFG_NC(GPP_D10),
			PAD_CFG_NC(GPP_D11),
			PAD_CFG_GPO(GPP_D12, 1, DEEP), /* EN_PP3300_DX_CAM */
/* ISH_UART0_RXD */	PAD_CFG_NC(GPP_D13),
/* ISH_UART0_TXD */	PAD_CFG_NC(GPP_D14),
/* ISH_UART0_RTS# */	PAD_CFG_NC(GPP_D15),
/* ISH_UART0_CTS# */	PAD_CFG_NC(GPP_D16),
/* DMIC_CLK1 */		PAD_CFG_NF(GPP_D17, NONE, DEEP, NF1),
/* DMIC_DATA1 */	PAD_CFG_NF(GPP_D18, NONE, DEEP, NF1),
/* DMIC_CLK0 */		PAD_CFG_NF(GPP_D19, NONE, DEEP, NF1),
/* DMIC_DATA0 */	PAD_CFG_NF(GPP_D20, NONE, DEEP, NF1),
/* TS_SPI_IO2 */	PAD_CFG_NF(GPP_D21, NONE, DEEP, NF1),
/* TS_SPI_IO3 */	PAD_CFG_NF(GPP_D22, NONE, DEEP, NF1),
/* I2S_MCLK */		PAD_CFG_NF(GPP_D23, NONE, DEEP, NF1),
/* SATAXPCI0 */		PAD_CFG_GPI_APIC(GPP_E0, NONE, PLTRST), /* TPM_PIRQ_L */
/* SATAXPCIE1 */	PAD_CFG_NC(GPP_E1),
/* SATAXPCIE2 */	PAD_CFG_NC(GPP_E2),
/* CPU_GP0 */		PAD_CFG_GPO(GPP_E3, 0, DEEP), /* AUDIO_DB_ID */
/* SATA_DEVSLP0 */	PAD_CFG_GPO(GPP_E4, 1, DEEP), /* TOUCH_RESET */
/* SATA_DEVSLP1 */	PAD_CFG_NC(GPP_E5),
/* SATA_DEVSLP2 */	PAD_CFG_NC(GPP_E6),
/* CPU_GP1 */		PAD_CFG_GPI_APIC(GPP_E7, NONE, PLTRST), /* TOUCHSCREEN */
/* SATALED# */		PAD_CFG_NC(GPP_E8),
/* USB2_OCO# */		PAD_CFG_NC(GPP_E9),
/* USB2_OC1# */		PAD_CFG_NC(GPP_E10),
/* USB2_OC2# */		PAD_CFG_GPI(GPP_E11, 20K_PU, DEEP),
/* USB2_OC3# */		PAD_CFG_GPI(GPP_E12, 20K_PU, DEEP),
/* DDPB_HPD0 */		PAD_CFG_NF(GPP_E13, NONE, DEEP, NF1),
/* DDPC_HPD1 */		PAD_CFG_NF(GPP_E14, NONE, DEEP, NF1),
/* DDPD_HPD2 */		PAD_CFG_GPI_ACPI_SMI(GPP_E15, NONE, DEEP, YES), /* EC_SMI_L */
/* DDPE_HPD3 */		PAD_CFG_GPI_ACPI_SCI(GPP_E16, NONE, DEEP, YES), /* EC_SCI_L */
/* EDP_HPD */		PAD_CFG_GPI(GPP_E17, NONE, DEEP),
/* DDPB_CTRLCLK */	PAD_CFG_NF(GPP_E18, NONE, DEEP, NF1),
/* DDPB_CTRLDATA */	PAD_CFG_NF(GPP_E19, NONE, DEEP, NF1),
/* DDPC_CTRLCLK */	PAD_CFG_NC(GPP_E20),
/* DDPC_CTRLDATA */	PAD_CFG_NF(GPP_E21, NONE, DEEP, NF1),
			PAD_CFG_NC(GPP_E22),
			PAD_CFG_NC(GPP_E23),
/*
 * The next 4 pads are for bit banging the amplifiers. They are connected
 * together with i2s0 signals. For default behavior of i2s make these
 * gpio inupts.
 */
/* I2S2_SCLK */		PAD_CFG_NF(GPP_F0, NONE, DEEP, NF1),
/* I2S2_SFRM */		PAD_CFG_NF(GPP_F1, NONE, DEEP, NF1),
/* I2S2_TXD */		PAD_CFG_NF(GPP_F2, NONE, DEEP, NF1),
/* I2S2_RXD */		PAD_CFG_NF(GPP_F3, NONE, DEEP, NF1),
/* I2C2_SDA */		PAD_CFG_NC(GPP_F4),
/* I2C2_SCL */		PAD_CFG_NC(GPP_F5),
/* I2C3_SDA */		PAD_CFG_NC(GPP_F6),
/* I2C3_SCL */		PAD_CFG_NC(GPP_F7),
/* I2C4_SDA */		PAD_CFG_NF(GPP_F8, NONE, DEEP, NF1), /* Amplifiers */
/* I2C4_SCL */		PAD_CFG_NF(GPP_F9, NONE, DEEP, NF1), /* Amplifiers */
/* I2C5_SDA */		PAD_CFG_GPI_APIC(GPP_F10, NONE, PLTRST), /* MIC_INT_L */
/* I2C5_SCL */		PAD_CFG_NF(GPP_F11, 20K_PU, DEEP, NF1),
/* EMMC_CMD */		PAD_CFG_NF(GPP_F12, 20K_PU, DEEP, NF1),
/* EMMC_DATA0 */	PAD_CFG_NF(GPP_F13, 20K_PU, DEEP, NF1),
/* EMMC_DATA1 */	PAD_CFG_NF(GPP_F14, 20K_PU, DEEP, NF1),
/* EMMC_DATA2 */	PAD_CFG_NF(GPP_F15, 20K_PU, DEEP, NF1),
/* EMMC_DATA3 */	PAD_CFG_NF(GPP_F16, 20K_PU, DEEP, NF1),
/* EMMC_DATA4 */	PAD_CFG_NF(GPP_F17, 20K_PU, DEEP, NF1),
/* EMMC_DATA5 */	PAD_CFG_NF(GPP_F18, 20K_PU, DEEP, NF1),
/* EMMC_DATA6 */	PAD_CFG_NF(GPP_F19, 20K_PU, DEEP, NF1),
/* EMMC_DATA7 */	PAD_CFG_NF(GPP_F20, 20K_PU, DEEP, NF1),
/* EMMC_RCLK */		PAD_CFG_NF(GPP_F21, 20K_PD, DEEP, NF1),
/* EMMC_CLK */		PAD_CFG_NF(GPP_F22, 20K_PD, DEEP, NF1),
/* BOOT_BEEP */		PAD_CFG_GPO(GPP_F23, 0, DEEP),
/* SD_CMD */		PAD_CFG_NF(GPP_G0, 20K_PU, DEEP, NF1),
/* SD_DATA0 */		PAD_CFG_NF(GPP_G1, 20K_PU, DEEP, NF1),
/* SD_DATA1 */		PAD_CFG_NF(GPP_G2, 20K_PU, DEEP, NF1),
/* SD_DATA2 */		PAD_CFG_NF(GPP_G3, 20K_PU, DEEP, NF1),
/* SD_DATA3 */		PAD_CFG_NF(GPP_G4, 20K_PU, DEEP, NF1),
/* SD_CD# */		PAD_CFG_NF(GPP_G5, NONE, DEEP, NF1),
/* SD_CLK */		PAD_CFG_NF(GPP_G6, 20K_PD, DEEP, NF1),
/* SD_WP */		PAD_CFG_NF(GPP_G7, 20K_PU, DEEP, NF1),
/* BATLOW# */		PAD_CFG_NF(GPD0, NONE, DEEP, NF1),
/* ACPRESENT */		PAD_CFG_NF(GPD1, NONE, DEEP, NF1),
/* LAN_WAKE# */		PAD_CFG_NF(GPD2, 20K_PU, DEEP, NF1), /* EC_PCH_WAKE_L */
/* PWRBTN# */		PAD_CFG_NF(GPD3, 20K_PU, DEEP, NF1),
/* SLP_S3# */		PAD_CFG_NF(GPD4, 20K_PU, DEEP, NF1),
/* SLP_S4# */		PAD_CFG_NF(GPD5, 20K_PU, DEEP, NF1),
/* SLP_A# */		PAD_CFG_NF(GPD6, 20K_PU, DEEP, NF1),
			PAD_CFG_NC(GPD7),
/* SUSCLK */		PAD_CFG_NF(GPD8, NONE, DEEP, NF1),
/* SLP_WLAN# */		PAD_CFG_NF(GPD9, 20K_PU, DEEP, NF1),
/* SLP_S5# */		PAD_CFG_NF(GPD10, 20K_PU, DEEP, NF1),
/* LANPHYC */		PAD_CFG_NC(GPD11),
};

/* Early pad configuration in romstage. */
static const struct pad_config early_gpio_table[] = {
/* UART2_CTS# */	PAD_CFG_GPI(GPP_C23, 20K_PU, DEEP), /* PCH_WP */
};

#endif

#endif
