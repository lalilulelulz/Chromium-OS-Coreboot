/*
 * This file is part of the coreboot project.
 *
 * Copyright 2007-2009 coresystems GmbH
 * Copyright 2011 Google Inc.
 * Copyright 2015 Intel Corp.
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

#include <console/console.h>
#include <device/pci_def.h>

#include <soc/ramstage.h>

typedef struct {
  uint8_t bus;
  uint8_t dev;
  uint8_t func;
} DEVICE_PCI_PFA;

#define BIT0   0x00000001

#define PCIE_L1OFF_ENABLE BIT0

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

int board_pciedev_l1substates_supported(device_t dev);
int board_pciedev_l1substates_supported(device_t dev)
{
	/* Intel WLAN 7265 Bus#2, Dev#0, Fun#0 */
	DEVICE_PCI_PFA PCIE_WLAN = {
		.bus = 0x02,
		.dev = 0x00,
		.func = 0x00
	};
	if ((dev->bus->secondary == PCIE_WLAN.bus) &&
	    (PCI_SLOT(dev->path.pci.devfn) == PCIE_WLAN.dev) &&
	    (PCI_FUNC(dev->path.pci.devfn) == PCIE_WLAN.func)){
		printk(BIOS_INFO, "Disable PCIE device :%02x %02x %02x L1Sub\n",
			(PCIE_WLAN.bus & 0xff),
			(PCIE_WLAN.dev & 0xff),
			(PCIE_WLAN.func & 0xff));
		/*clear bit[0] to disable PCIE L1 sub */
		return 0xf & ~(PCIE_L1OFF_ENABLE);
	}
	return 0xf;
}

void board_silicon_USB2_override(SILICON_INIT_UPD *params)
{
	if (SocStepping() >= SocD0) {
		//D-Stepping
		//USB2[1] right external port
		params->Usb2Port1PerPortPeTxiSet = 7;
		params->Usb2Port1PerPortTxiSet = 3;
		params->Usb2Port1IUsbTxEmphasisEn = 2;
		params->Usb2Port1PerPortTxPeHalf = 1;

		//USB2[2] left external port
		params->Usb2Port2PerPortPeTxiSet = 7;
		params->Usb2Port2PerPortTxiSet = 0;
		params->Usb2Port2IUsbTxEmphasisEn = 2;
		params->Usb2Port2PerPortTxPeHalf = 1;

		//USB2[3] CCD
		params->Usb2Port3PerPortPeTxiSet = 7;
		params->Usb2Port3PerPortTxiSet = 0;
		params->Usb2Port3IUsbTxEmphasisEn = 2;
		params->Usb2Port3PerPortTxPeHalf = 1;
	}
}
