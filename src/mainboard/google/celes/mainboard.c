/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 * Copyright (C) 2011 Google Inc.
 * Copyright (C) 2015 Intel Corp.
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

#include <bootstate.h>
#include <device/device.h>
#include <soc/gpio.h>
#include "ec.h"

#include <soc/ramstage.h>

static void mainboard_init(device_t dev)
{
	mainboard_ec_init();
}

/*
 * mainboard_enable is executed as first thing after
 * enumerate_buses().
 */
static void mainboard_enable(device_t dev)
{
	dev->ops->init = mainboard_init;
}


struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};

void board_silicon_USB2_override(SILICON_INIT_UPD *params)
{
	if (SocStepping() >= SocD0) {

		params->Usb2Port0PerPortPeTxiSet = 7;
		params->Usb2Port0PerPortTxiSet = 0;
		params->Usb2Port0IUsbTxEmphasisEn = 3;
		params->Usb2Port0PerPortTxPeHalf = 1;

		params->Usb2Port1PerPortPeTxiSet = 7;
		params->Usb2Port1PerPortTxiSet = 0;
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
	}
}
