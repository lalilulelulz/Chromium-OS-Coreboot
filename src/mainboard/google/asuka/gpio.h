/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Google Inc.
 * Copyright (C) 2015 Intel Corporation
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

/* GPP_B5 is TOUCHPAD WAKE. GPP_B group is routed to DW0 in the GPE0 block */
#define GPE_TOUCHPAD_WAKE       GPE0_DW0_05

/* Input device interrupt configuration */
#define TOUCHPAD_INT_L		GPP_B3_IRQ
#define TOUCHSCREEN_INT_L	GPP_E7_IRQ
#define MIC_INT_L		GPP_F10_IRQ

/* GPP_E16 is EC_SCI_L. GPP_E group is routed to DW2 in the GPE0 block */
#define EC_SCI_GPI		GPE0_DW2_16
#define EC_SMI_GPI		GPP_E15

/*
 * GPP_E3 is AUDIO_DB_ID.
 * It is a dual purpose GPIO, used for Audio Daughter
 * Board Identification & to control the shutdown mode pin
 * of the Maxim amp.
 */
#define AUDIO_DB_ID		GPP_E3

#ifndef __ACPI__
/* Pad configuration in ramstage. */
static const struct pad_config gpio_table[] = {
/* EC_PCH_RCIN */	PAD_CFG_NF(GPP_A0, NONE, DEEP, NF1),
/* LPC_LAD_0 */		PAD_CFG_NF(GPP_A1, 20K_PU, DEEP, NF1),
/* LPC_LAD_1 */		PAD_CFG_NF(GPP_A2, 20K_PU, DEEP, NF1),
/* LPC_LAD_2 */		PAD_CFG_NF(GPP_A3, 20K_PU, DEEP, NF1),
/* LPC_LAD_3 */		PAD_CFG_NF(GPP_A4, 20K_PU, DEEP, NF1),
/* LPC_FRAME */		PAD_CFG_NF(GPP_A5, NONE, DEEP, NF1),
/* LPC_SERIRQ */	PAD_CFG_NF(GPP_A6, NONE, DEEP, NF1),
/* SD_CD_WAKE */	PAD_CFG_GPI(GPP_A7, 20K_PU, DEEP),
/* LPC_CLKRUN */	PAD_CFG_NF(GPP_A8, NONE, DEEP, NF1),
/* EC_LPC_CLK */	PAD_CFG_NF(GPP_A9, NONE, DEEP, NF1),
/* PCH_LPC_CLK */	PAD_CFG_NC(GPP_A10),
/* EC_HID_INT */	PAD_CFG_NC(GPP_A11),
/* ISH_KB_PROX_INT */	PAD_CFG_NC(GPP_A12),
/* PCH_SUSPWRACB */	PAD_CFG_NF(GPP_A13, NONE, DEEP, NF1),
/* PM_SUS_STAT */	PAD_CFG_NC(GPP_A14),
/* PCH_SUSACK */	PAD_CFG_NF(GPP_A15, NONE, DEEP, NF1),
/* SD_1P8_SEL */	PAD_CFG_NC(GPP_A16),
/* SD_PWR_EN */  	PAD_CFG_NC(GPP_A17),
/* ACCEL INTERRUPT */	PAD_CFG_NC(GPP_A18),
/* ISH_GP1 */		PAD_CFG_NC(GPP_A19),
/* GYRO_DRDY */ 	PAD_CFG_NC(GPP_A20),
/* FLIP_ACCEL_INT */	PAD_CFG_NC(GPP_A21),
/* GYRO_INT */		PAD_CFG_NC(GPP_A22),
/* ISH_GP5 */		PAD_CFG_NC(GPP_A23),
/* CORE_VID0 */ 	PAD_CFG_NC(GPP_B0),
/* CORE_VID1 */ 	PAD_CFG_NC(GPP_B1),
/* HSJ_MIC_DET */	PAD_CFG_GPI(GPP_B2, NONE, DEEP),
/* TRACKPAD_INT */	PAD_CFG_GPI_APIC(GPP_B3, NONE, PLTRST),
/* BT_RF_KILL */	PAD_CFG_NC(GPP_B4),
/* SRCCLKREQ0# */	PAD_CFG_GPI_ACPI_SCI(GPP_B5, NONE, DEEP, YES), /* TOUCHPAD WAKE */
/* WIFI_CLK_REQ */	PAD_CFG_NF(GPP_B6, NONE, DEEP, NF1),
/* SRCCLKREQ2 */	PAD_CFG_NC(GPP_B7),
/* AUDIO_INT_WAK */	PAD_CFG_GPI_ACPI_SCI(GPP_B8, NONE, DEEP, YES),
/* SRCCLKREQ4 */	PAD_CFG_NC(GPP_B9),
/* SRCCLKREQ5# */	PAD_CFG_NC(GPP_B10),
/* MPHY_EXT_PWR_GATE */ PAD_CFG_NC(GPP_B11),
/* PM_SLP_S0 */  	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
/* PCH_PLT_RST */	PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
/* PCH_BUZZER */	PAD_CFG_GPI(GPP_B14, NONE, DEEP),
/* GSPI0_CS# */ 	PAD_CFG_NC(GPP_B15),
/* WLAN_PCIE_WAKE */	PAD_CFG_GPI_ACPI_SCI(GPP_B16, NONE, DEEP, YES),
/* SSD_PCIE_WAKE */	PAD_CFG_NC(GPP_B17),
/* GSPI0_MOSI */	PAD_CFG_NC(GPP_B18),
/* CCODEC_SPI_CS */	PAD_CFG_NC(GPP_B19),
/* CODEC_SPI_CLK */	PAD_CFG_NC(GPP_B20),
/* CODEC_SPI_MISO */	PAD_CFG_NC(GPP_B21),
/* CODEC_SPI_MOSI */	PAD_CFG_NC(GPP_B22),
/* SM1ALERT# */ 	PAD_CFG_NC(GPP_B23),
/* SMB_CLK */		PAD_CFG_NF(GPP_C0, NONE, DEEP, NF1),
/* SMB_DATA */		PAD_CFG_NF(GPP_C1, NONE, DEEP, NF1),
/* SMBALERT# */  	PAD_CFG_GPO(GPP_C2, 0, DEEP),
/* M2_WWAN_PWREN */	PAD_CFG_NC(GPP_C3),
/* SML0DATA */		PAD_CFG_NC(GPP_C4),
/* SML0ALERT# */	PAD_CFG_NC(GPP_C5),
/* EC_IN_RW */		PAD_CFG_GPI(GPP_C6, NONE, DEEP),
/* USB_CTL */		PAD_CFG_NC(GPP_C7),
/* UART0_RXD */		PAD_CFG_NC(GPP_C8),
/* UART0_TXD */		PAD_CFG_NC(GPP_C9),
/* NFC_RST* */		PAD_CFG_NC(GPP_C10),
/* EN_PP3300_KEPLER */	PAD_CFG_NC(GPP_C11),
/* PCH_MEM_CFG0 */	PAD_CFG_GPI(GPP_C12, NONE, DEEP),
/* PCH_MEM_CFG1 */	PAD_CFG_GPI(GPP_C13, NONE, DEEP),
/* PCH_MEM_CFG2 */	PAD_CFG_GPI(GPP_C14, NONE, DEEP),
/* PCH_MEM_CFG3 */	PAD_CFG_GPI(GPP_C15, NONE, DEEP),
/* I2C0_SDA */		PAD_CFG_NF(GPP_C16, 5K_PU, DEEP, NF1),
/* I2C0_SCL */		PAD_CFG_NF(GPP_C17, 5K_PU, DEEP, NF1),
/* I2C1_SDA */		PAD_CFG_NF(GPP_C18, NONE, DEEP, NF1),
/* I2C1_SCL */		PAD_CFG_NF(GPP_C19, NONE, DEEP, NF1),
/* GD_UART2_RXD */	PAD_CFG_NF(GPP_C20, NONE, DEEP, NF1),
/* GD_UART2_TXD */	PAD_CFG_NF(GPP_C21, NONE, DEEP, NF1),
/* TCH_PNL_PWREN */	PAD_CFG_GPO(GPP_C22, 1, DEEP),
/* SPI_WP_STATUS */	PAD_CFG_GPI(GPP_C23, 20K_PU, DEEP),
/* ITCH_SPI_CS */	PAD_CFG_NC(GPP_D0),
/* ITCH_SPI_CLK */	PAD_CFG_NC(GPP_D1),
/* ITCH_SPI_MISO_1 */	PAD_CFG_NC(GPP_D2),
/* ITCH_SPI_MISO_0 */	PAD_CFG_NC(GPP_D3),
/* CAM_FLASH_STROBE */	PAD_CFG_NC(GPP_D4),
/* EN_PP3300_DX_EMMC */ PAD_CFG_NC(GPP_D5),
/* EN_PP1800_DX_EMMC */ PAD_CFG_NC(GPP_D6),
/* SH_I2C1_SDA */	PAD_CFG_NC(GPP_D7),
/* SH_I2C1_SCL */	PAD_CFG_NC(GPP_D8),
/* ISH_SPI_CSB */ 	PAD_CFG_NC(GPP_D9),
/* USB_A0_ILIM_SEL */	PAD_CFG_GPO(GPP_D10, 0, DEEP),
/* USB_A1_ILIM_SEL */	PAD_CFG_GPO(GPP_D11, 0, DEEP),
/* EN_PP3300_DX_CAM */	PAD_CFG_NC(GPP_D12),
/* EN_PP1800_DX_AUDIO */PAD_CFG_NC(GPP_D13),
/* ISH_UART0_TXD */	PAD_CFG_NC(GPP_D14),
/* ISH_UART0_RTS */	PAD_CFG_NC(GPP_D15),
/* ISH_UART0_CTS */	PAD_CFG_NC(GPP_D16),
/* DMIC_CLK_1 */	PAD_CFG_NC(GPP_D17),
/* DMIC_DATA_1 */	PAD_CFG_NC(GPP_D18),
/* DMIC_CLK_0 */	PAD_CFG_NF(GPP_D19, NONE, DEEP, NF1),
/* DMIC_DATA_0 */	PAD_CFG_NF(GPP_D20, NONE, DEEP, NF1),
/* ITCH_SPI_D2 */	PAD_CFG_NC(GPP_D21),
/* ITCH_SPI_D3 */	PAD_CFG_NC(GPP_D22),
/* I2S_MCLK */		PAD_CFG_NF(GPP_D23, NONE, DEEP, NF1),
/* SPI_TPM_IRQ */	PAD_CFG_GPI_APIC(GPP_E0, NONE, PLTRST),
/* SATAXPCIE1 */	PAD_CFG_NC(GPP_E1),
/* SSD_PEDET */ 	PAD_CFG_NC(GPP_E2),
/* AUDIO_DB_ID */	PAD_CFG_GPI(GPP_E3, NONE, DEEP),
/* SSD_SATA_DEVSLP */	PAD_CFG_NC(GPP_E4),
/* SATA_DEVSLP1 */	PAD_CFG_NC(GPP_E5),
/* SATA_DEVSLP2 */	PAD_CFG_NC(GPP_E6),
/* TCH_PNL_INTR* */	PAD_CFG_GPI_APIC(GPP_E7, NONE, PLTRST),
/* SATALED# */		PAD_CFG_NC(GPP_E8),
/* USB2_OC_0 */  	PAD_CFG_NF(GPP_E9, NONE, DEEP, NF1),
/* USB2_OC_1 */  	PAD_CFG_NF(GPP_E10, NONE, DEEP, NF1),
/* USB2_OC_2 */  	PAD_CFG_NF(GPP_E11, NONE, DEEP, NF1),
/* USB2_OC_3 */  	PAD_CFG_NF(GPP_E12, NONE, DEEP, NF1),
/* DDI1_HPD */		PAD_CFG_NF(GPP_E13, NONE, DEEP, NF1),
/* DDI2_HPD */		PAD_CFG_NF(GPP_E14, NONE, DEEP, NF1),
/* EC_SMI */		PAD_CFG_GPI_ACPI_SMI(GPP_E15, NONE, DEEP, YES),
/* EC_SCI */		PAD_CFG_GPI_ACPI_SCI(GPP_E16, NONE, DEEP, YES),
/* EDP_HPD */		PAD_CFG_NF(GPP_E17, NONE, DEEP, NF1),
/* DDPB_CTRLCLK */	PAD_CFG_NF(GPP_E18, NONE, DEEP, NF1),
/* DDPB_CTRLDATA */	PAD_CFG_NF(GPP_E19, NONE, DEEP, NF1),
/* DDPC_CTRLCLK */	PAD_CFG_NC(GPP_E20),
/* DDPC_CTRLDATA */	PAD_CFG_NF(GPP_E21, NONE, DEEP, NF1),

/* DDPD_CTRLCLK */	PAD_CFG_NC(GPP_E22),
/* TCH_PNL_RST */	PAD_CFG_GPO(GPP_E23, 1, DEEP),
/* I2S2_SCLK */  	PAD_CFG_NC(GPP_F0),
/* I2S2_SFRM */  	PAD_CFG_NC(GPP_F1),
/* I2S2_TXD */		PAD_CFG_NC(GPP_F2),
/* I2S2_RXD */		PAD_CFG_NC(GPP_F3),
/* I2C2_SDA */		PAD_CFG_NC(GPP_F4),
/* I2C2_SCL */		PAD_CFG_NC(GPP_F5),
/* I2C3_SDA */		PAD_CFG_NC(GPP_F6),
/* I2C3_SCL */		PAD_CFG_NC(GPP_F7),
/* I2C4_SDA */		PAD_CFG_NF(GPP_F8, NONE, DEEP, NF1),
/* I2C4_SDA */		PAD_CFG_NF(GPP_F9, NONE, DEEP, NF1),
/* AUDIO_IRQ */  	PAD_CFG_GPI_APIC(GPP_F10, NONE, PLTRST),
/* AUDIO_IRQ */  	PAD_CFG_GPI_ACPI_SCI(GPP_F11, NONE, DEEP, YES),
/* EMMC_CMD */		PAD_CFG_NF(GPP_F12, NONE, DEEP, NF1),
/* EMMC_DATA0 */	PAD_CFG_NF(GPP_F13, NONE, DEEP, NF1),
/* EMMC_DATA1 */	PAD_CFG_NF(GPP_F14, NONE, DEEP, NF1),
/* EMMC_DATA2 */	PAD_CFG_NF(GPP_F15, NONE, DEEP, NF1),
/* EMMC_DATA3 */	PAD_CFG_NF(GPP_F16, NONE, DEEP, NF1),
/* EMMC_DATA4 */	PAD_CFG_NF(GPP_F17, NONE, DEEP, NF1),
/* EMMC_DATA5 */	PAD_CFG_NF(GPP_F18, NONE, DEEP, NF1),
/* EMMC_DATA6 */	PAD_CFG_NF(GPP_F19, NONE, DEEP, NF1),
/* EMMC_DATA7 */	PAD_CFG_NF(GPP_F20, NONE, DEEP, NF1),
/* EMMC_RCLK */		PAD_CFG_NF(GPP_F21, NONE, DEEP, NF1),
/* EMMC_CLK */		PAD_CFG_NF(GPP_F22, NONE, DEEP, NF1),
/* BOOT_BEEP */		PAD_CFG_GPO(GPP_F23, 0, DEEP),
/* SD_CMD */		PAD_CFG_NC(GPP_G0),
/* SD_DATA0 */		PAD_CFG_NC(GPP_G1),
/* SD_DATA1 */		PAD_CFG_NC(GPP_G2),
/* SD_DATA2 */		PAD_CFG_NC(GPP_G3),
/* SD_DATA3 */		PAD_CFG_NC(GPP_G4),
/* SD_CD# */		PAD_CFG_NC(GPP_G5),
/* SD_CLK */		PAD_CFG_NC(GPP_G6),
/* SD_WP */		PAD_CFG_NC(GPP_G7),
/* PCH_BATLOW */	PAD_CFG_NF(GPD0, NONE, DEEP, NF1),
/* EC_PCH_ACPRESENT */	PAD_CFG_NF(GPD1, NONE, DEEP, NF1),
/* EC_PCH_WAKE */	PAD_CFG_NF(GPD2, NONE, DEEP, NF1),
/* EC_PCH_PWRBTN */	PAD_CFG_NF(GPD3, NONE, DEEP, NF1),
/* PM_SLP_S3# */	PAD_CFG_NF(GPD4, NONE, DEEP, NF1),
/* PM_SLP_S4# */	PAD_CFG_NF(GPD5, NONE, DEEP, NF1),
/* PM_SLP_SA# */	PAD_CFG_NF(GPD6, NONE, DEEP, NF1),
/* GPD7 */		PAD_CFG_NC(GPD7),
/* PM_SUSCLK */  	PAD_CFG_NF(GPD8, NONE, DEEP, NF1),
/* PCH_SLP_WLAN# */	PAD_CFG_NC(GPD9),
/* PM_SLP_S5# */	PAD_CFG_NC(GPD10),
/* LANPHYC */		PAD_CFG_NC(GPD11),
};

/* Early pad configuration in romstage. */
static const struct pad_config early_gpio_table[] = {
/* SPI_WP_STATUS */	PAD_CFG_GPI(GPP_C23, 20K_PU, DEEP),
};

#endif

#endif
