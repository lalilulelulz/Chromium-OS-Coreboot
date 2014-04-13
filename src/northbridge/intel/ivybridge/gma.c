/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Chromium OS Authors
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <arch/io.h>
#include <console/console.h>
#include <delay.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ids.h>

#include "chip.h"
#include "sandybridge.h"

/* some vga option roms are used for several chipsets but they only have one
 * PCI ID in their header. If we encounter such an option rom, we need to do
 * the mapping ourselfes
 */

u32 map_oprom_vendev(u32 vendev)
{
	u32 new_vendev=vendev;

	switch (vendev) {
	case 0x01028086:		/* GT1 Desktop */
	case 0x010a8086:		/* GT1 Server */
	case 0x01128086:		/* GT2 Desktop */
	case 0x01168086:		/* GT2 Mobile */
	case 0x01228086:		/* GT2 Desktop >=1.3GHz */
	case 0x01268086:		/* GT2 Mobile >=1.3GHz */
		new_vendev=0x01068086;	/* GT1 Mobile */
		break;
	}

	return new_vendev;
}

static struct resource *gtt_res = NULL;

static inline u32 gtt_read(u32 reg)
{
	return read32(gtt_res->base + reg);
}

static inline void gtt_write(u32 reg, u32 data)
{
	write32(gtt_res->base + reg, data);
}

#define GTT_RETRY 1000
static int gtt_poll(u32 reg, u32 mask, u32 value)
{
	unsigned try = GTT_RETRY;
	u32 data;

	while (try--) {
		data = gtt_read(reg);
		if ((data & mask) == value)
			return 1;
		udelay(10);
	}

	printk(BIOS_ERR, "GT init timeout\n");
	return 0;
}

static void gma_pm_init_pre_vbios(struct device *dev)
{
	u32 reg32;

	printk(BIOS_DEBUG, "GT Power Management Init\n");

	gtt_res = find_resource(dev, PCI_BASE_ADDRESS_0);
	if (!gtt_res || !gtt_res->base)
		return;

	/* 1: Enable force wake */
	gtt_write(0xa18c, 0x00000001);
	if (!gtt_poll(0x130090, (1 << 0), (1 << 0)))
		return;

	/* 1d: Set GTT+0x42004 [15:14]=11 (SnB C1+) */
	reg32 = gtt_read(0x42004);
	reg32 |= (1 << 14) | (1 << 15);
	gtt_write(0x42004, reg32);

	/* 2: Get GT SKU from GTT+0x911c[13] */
	reg32 = gtt_read(0x911c);
	if (reg32 & (1 << 13)) {
		printk(BIOS_DEBUG, "GT1 Power Meter Weights\n");
		gtt_write(0xa200, 0xcc000000);
		gtt_write(0xa204, 0x07000040);
		gtt_write(0xa208, 0x0000fe00);
		gtt_write(0xa20c, 0x00000000);
		gtt_write(0xa210, 0x17000000);
		gtt_write(0xa214, 0x00000021);
		gtt_write(0xa218, 0x0817fe19);
		gtt_write(0xa21c, 0x00000000);
		gtt_write(0xa220, 0x00000000);
		gtt_write(0xa224, 0xcc000000);
		gtt_write(0xa228, 0x07000040);
		gtt_write(0xa22c, 0x0000fe00);
		gtt_write(0xa230, 0x00000000);
		gtt_write(0xa234, 0x17000000);
		gtt_write(0xa238, 0x00000021);
		gtt_write(0xa23c, 0x0817fe19);
		gtt_write(0xa240, 0x00000000);
		gtt_write(0xa244, 0x00000000);
		gtt_write(0xa248, 0x8000421e);
	} else {
		printk(BIOS_DEBUG, "GT2 Power Meter Weights\n");
		gtt_write(0xa200, 0x330000a6);
		gtt_write(0xa204, 0x402d0031);
		gtt_write(0xa208, 0x00165f83);
		gtt_write(0xa20c, 0xf1000000);
		gtt_write(0xa210, 0x00000000);
		gtt_write(0xa214, 0x00160016);
		gtt_write(0xa218, 0x002a002b);
		gtt_write(0xa21c, 0x00000000);
		gtt_write(0xa220, 0x00000000);
		gtt_write(0xa224, 0x330000a6);
		gtt_write(0xa228, 0x402d0031);
		gtt_write(0xa22c, 0x00165f83);
		gtt_write(0xa230, 0xf1000000);
		gtt_write(0xa234, 0x00000000);
		gtt_write(0xa238, 0x00160016);
		gtt_write(0xa23c, 0x002a002b);
		gtt_write(0xa240, 0x00000000);
		gtt_write(0xa244, 0x00000000);
		gtt_write(0xa248, 0x8000421e);
	}

	/* 3: Gear ratio map */
	gtt_write(0xa004, 0x00000010);

	/* 4: GFXPAUSE */
	gtt_write(0xa000, 0x00070020);

	/* 5: Dynamic EU trip control */
	gtt_write(0xa080, 0x00000004);

	/* 6: ECO bits (bit 20=1 for step D1+) */
	gtt_write(0xa180, 0x84100000);

	/* 6a: SnB step D2+ */
	reg32 = gtt_read(0x9400);
	reg32 |= (1 << 7);
	gtt_write(0x9400, reg32);

	reg32 = gtt_read(0x941c);
	reg32 |= (1 << 1);
	gtt_write(0x941c, reg32);
	if (!gtt_poll(0x941c, (1 << 1), (0 << 1)))
		return;

	/* 6b: Clocking reset controls */
	gtt_write(0x9424, 0x00000000);

	/* 7 */
	if (!gtt_poll(0x138124, (1 << 31), (0 << 31)))
		return;
	gtt_write(0x138128, 0x00000029); /* Mailbox Data */
	gtt_write(0x138124, 0x80000004); /* Mailbox Cmd for RC6 VID */
	if (!gtt_poll(0x138124, (1 << 31), (0 << 31)))
		return;
	gtt_write(0x138124, 0x8000000a); /* Mailbox Cmd to clear RC6 count */
	if (!gtt_poll(0x138124, (1 << 31), (0 << 31)))
		return;

	/* 8 */
	gtt_write(0xa090, 0x00000000); /* RC Control */
	gtt_write(0xa098, 0x03e80000); /* RC1e Wake Rate Limit */
	gtt_write(0xa09c, 0x0028001e); /* RC6/6p Wake Rate Limit */
	gtt_write(0xa0a0, 0x0000001e); /* RC6pp Wake Rate Limit */
	gtt_write(0xa0a8, 0x0001e848); /* RC Evaluation Interval */
	gtt_write(0xa0ac, 0x00000019); /* RC Idle Hysteresis */

	/* 9 */
	gtt_write(0x2054, 0x0000000a); /* Render Idle Max Count */
	gtt_write(0x12054,0x0000000a); /* Video Idle Max Count */
	gtt_write(0x22054,0x0000000a); /* Blitter Idle Max Count */

	/* 10 */
	gtt_write(0xa0b0, 0x00000000); /* Unblock Ack to Busy */
	gtt_write(0xa0b4, 0x000003e8); /* RC1e Threshold */
	gtt_write(0xa0b8, 0x0000c350); /* RC6 Threshold */
	gtt_write(0xa0bc, 0x000186a0); /* RC6p Threshold */
	gtt_write(0xa0c0, 0x0000fa00); /* RC6pp Threshold */

	/* 11 */
	gtt_write(0xa010, 0x000f4240); /* RP Down Timeout */
	gtt_write(0xa014, 0x12060000); /* RP Interrupt Limits */
	gtt_write(0xa02c, 0x00015f90); /* RP Up Threshold */
	gtt_write(0xa030, 0x000186a0); /* RP Down Threshold */
	gtt_write(0xa068, 0x000186a0); /* RP Up EI */
	gtt_write(0xa06c, 0x000493e0); /* RP Down EI */
	gtt_write(0xa070, 0x0000000a); /* RP Idle Hysteresis */

	/* 11a: Enable Render Standby (RC6) */
	gtt_write(0xa090, 0x88040000); /* HW RC Control */

	/* 12: Normal Frequency Request */
	/* RPNFREQ_VAL comes from MCHBAR 0x5998 23:16 (8 bits!? use 7) */
	reg32 = MCHBAR32(0x5998);
	reg32 >>= 16;
	reg32 &= 0xef;
	reg32 <<= 25;
	gtt_write(0xa008, reg32);

	/* 13: RP Control */
	gtt_write(0xa024, 0x00000592);

	/* 14: Enable PM Interrupts */
	gtt_write(0x4402c, 0x03000076);

	/* Clear 0x6c024 [8:6] */
	reg32 = gtt_read(0x6c024);
	reg32 &= ~0x000001c0;
	gtt_write(0x6c024, reg32);
}

static void gma_pm_init_post_vbios(struct device *dev)
{
	struct northbridge_intel_sandybridge_config *config = dev->chip_info;

	printk(BIOS_DEBUG, "GT Power Management Init (post VBIOS)\n");

	/* 15: Deassert Force Wake */
	gtt_write(0xa18c, gtt_read(0xa18c) & ~1);
	if (!gtt_poll(0x130090, (1 << 0), (0 << 0)))
		return;

	/* 16: SW RC Control */
	gtt_write(0xa094, 0x00060000);

	if (config->lvds_blc_freq) {
		/* Enable backlight */
		gtt_write(0xc8250, 0x80000000);
		gtt_write(0xc8254, config->lvds_blc_freq);
	}
}

static void gma_func0_init(struct device *dev)
{
	u32 reg32;

	/* IGD needs to be Bus Master */
	reg32 = pci_read_config32(dev, PCI_COMMAND);
	reg32 |= PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY | PCI_COMMAND_IO;
	pci_write_config32(dev, PCI_COMMAND, reg32);

	/* Init graphics power management */
	gma_pm_init_pre_vbios(dev);

	/* PCI Init, will run VBIOS */
	pci_dev_init(dev);

	/* Post VBIOS init */
	gma_pm_init_post_vbios(dev);
}

static void gma_set_subsystem(device_t dev, unsigned vendor, unsigned device)
{
	if (!vendor || !device) {
		pci_write_config32(dev, PCI_SUBSYSTEM_VENDOR_ID,
				pci_read_config32(dev, PCI_VENDOR_ID));
	} else {
		pci_write_config32(dev, PCI_SUBSYSTEM_VENDOR_ID,
				((device & 0xffff) << 16) | (vendor & 0xffff));
	}
}

static struct pci_operations gma_pci_ops = {
	.set_subsystem    = gma_set_subsystem,
};

static struct device_operations gma_func0_ops = {
	.read_resources		= pci_dev_read_resources,
	.set_resources		= pci_dev_set_resources,
	.enable_resources	= pci_dev_enable_resources,
	.init			= gma_func0_init,
	.scan_bus		= 0,
	.enable			= 0,
	.ops_pci		= &gma_pci_ops,
};

static const struct pci_driver gma_func0_driver_1 __pci_driver = {
	.ops	= &gma_func0_ops,
	.vendor	= PCI_VENDOR_ID_INTEL,
	.device	= 0x0106,
};

static const struct pci_driver gma_func0_driver_2 __pci_driver = {
	.ops	= &gma_func0_ops,
	.vendor	= PCI_VENDOR_ID_INTEL,
	.device	= 0x0116,
};

