/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Intel Corporation
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

#include <soc/ramstage.h>
#include <boardid.h>
#include "onboard.h"

void mainboard_silicon_init_params(SILICON_INIT_UPD *params)
{
	uint8_t	boardid = 0;
	uint8_t	projectid = 0;

	params->ChvSvidConfig = SVID_PMIC_CONFIG;
	params->PMIC_I2CBus = PMIC_I2C_BUS;

	boardid = board_id();
	projectid = (boardid >> 3) & 0x01;

	if (projectid == TERRA2_PROJECT_ID) {
		params->Usb2Port0PerPortPeTxiSet = 7;
		params->Usb2Port0PerPortTxiSet = 6;
		params->Usb2Port0IUsbTxEmphasisEn = 3;
		params->Usb2Port0PerPortTxPeHalf = 1;
		params->Usb2Port1PerPortPeTxiSet = 7;
		params->Usb2Port1PerPortTxiSet = 6;
		params->Usb2Port1IUsbTxEmphasisEn = 3;
		params->Usb2Port1PerPortTxPeHalf = 1;
		params->Usb2Port2PerPortPeTxiSet = 7;
		params->Usb2Port2PerPortTxiSet = 6;
		params->Usb2Port2IUsbTxEmphasisEn = 3;
		params->Usb2Port2PerPortTxPeHalf = 1;
		params->Usb2Port3PerPortPeTxiSet = 7;
		params->Usb2Port3PerPortTxiSet = 6;
		params->Usb2Port3IUsbTxEmphasisEn = 3;
		params->Usb2Port3PerPortTxPeHalf = 1;
		params->Usb2Port4PerPortPeTxiSet = 7;
		params->Usb2Port4PerPortTxiSet = 6;
		params->Usb2Port4IUsbTxEmphasisEn = 3;
		params->Usb2Port4PerPortTxPeHalf = 1;
	} else {
		params->Usb2Port0PerPortPeTxiSet = 7;
		params->Usb2Port0PerPortTxiSet = 5;
		params->Usb2Port0IUsbTxEmphasisEn = 2;
		params->Usb2Port0PerPortTxPeHalf = 1;
		params->Usb2Port1PerPortPeTxiSet = 7;
		params->Usb2Port1PerPortTxiSet = 3;
		params->Usb2Port1IUsbTxEmphasisEn = 2;
		params->Usb2Port1PerPortTxPeHalf = 1;
		params->Usb2Port2PerPortPeTxiSet = 7;
		params->Usb2Port2PerPortTxiSet = 3;
		params->Usb2Port2IUsbTxEmphasisEn = 2;
		params->Usb2Port2PerPortTxPeHalf = 1;
		params->Usb2Port3PerPortPeTxiSet = 7;
		params->Usb2Port3PerPortTxiSet = 6;
		params->Usb2Port3IUsbTxEmphasisEn = 3;
		params->Usb2Port3PerPortTxPeHalf = 1;
		params->Usb2Port4PerPortPeTxiSet = 7;
		params->Usb2Port4PerPortTxiSet = 3;
		params->Usb2Port4IUsbTxEmphasisEn = 2;
		params->Usb2Port4PerPortTxPeHalf = 1;
	}
}
