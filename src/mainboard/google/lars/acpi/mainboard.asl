/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Google Inc.
 * Copyright (C) 2015 Intel Corporation.
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

#include "../gpio.h"

#define BOARD_TOUCHPAD_I2C_ADDR			0x15
#define BOARD_TOUCHPAD_IRQ			TOUCHPAD_INT_L

#define BOARD_TOUCHSCREEN_I2C_ADDR		0x10
#define BOARD_TOUCHSCREEN_IRQ			TOUCHSCREEN_INT_L

#define BOARD_HP_MIC_CODEC_I2C_ADDR		0x1a
#define BOARD_HP_MIC_CODEC_IRQ			MIC_INT_L
#define BOARD_LEFT_SPEAKER_AMP_I2C_ADDR		0x34
#define BOARD_RIGHT_SPEAKER_AMP_I2C_ADDR	0x35

Scope (\_SB)
{
	Device (LID0)
	{
		Name (_HID, EisaId ("PNP0C0D"))
		Method (_LID, 0)
		{
			Return (\_SB.PCI0.LPCB.EC0.LIDS)
		}

		Name (_PRW, Package () { GPE_EC_WAKE, 5 })
	}

	Device (PWRB)
	{
		Name (_HID, EisaId ("PNP0C0C"))
	}

	Device (MAXM)
	{
		Name (_HID, "MX98357A")
		Name (_DDN, "Maxim Integrated 98357A Amplifier")
		Name (_UID, 1)
		Name (_CRS, ResourceTemplate()
		{
			GpioIo (Exclusive, PullDefault, 0x0000, 0x0000,
				IoRestrictionNone,
				"\\_SB.PCI0.GPIO", 0x00, ResourceConsumer,,)
			{
				GPIO_SPEAKER_MAXIM_AMP_SDMODE,
			}
		})
		Name (_DSD, Package ()
		{
			ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package ()
			{
				Package () {
					/*
					 * Create a named GPIO
					 * "sdmode-gpio" for the
					 * kernel codec driver
					 * to use.
					 */

					"sdmode-gpio", Package () {
								^MAXM, 0, 0, 0
							}
				},
			}
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}
}

/*
 * LPC Trusted Platform Module
 */
Scope (\_SB.PCI0.LPCB)
{
	#include <drivers/pc80/tpm/acpi/tpm.asl>
}

/*
 * WLAN connected to Root Port 1
 */
Scope (\_SB.PCI0.RP01)
{
	Device (WLAN)
	{
		Name (_ADR, 0x00000000)
		Name (_DDN, "Wireless LAN")
		Name (_PRW, Package () { GPE_WLAN_WAKE, 3 })
	}
}

Scope (\_SB.PCI0.I2C0)
{
	/* Touchscreen */
	Device (ELTS)
	{
		Name (_HID, "ELAN0001")
		Name (_DDN, "Elan Touchscreen")
		Name (_UID, 1)
		Name (_S0W, 4)

		Name (_CRS, ResourceTemplate ()
		{
			I2cSerialBus (
				BOARD_TOUCHSCREEN_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C0",
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				BOARD_TOUCHSCREEN_IRQ
			}
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}
}

Scope (\_SB.PCI0.I2C1)
{
	/* Touchpad */
	Device (ELTP)
	{
		Name (_HID, "ELAN0000")
		Name (_DDN, "Elan Touchpad")
		Name (_UID, 1)
		Name (_S0W, 4)

		Name (_CRS, ResourceTemplate ()
		{
			I2cSerialBus (
				BOARD_TOUCHPAD_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C1",
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				BOARD_TOUCHPAD_IRQ
			}
		})

		Name (_PRW, Package() { GPE_TOUCHPAD_WAKE, 0x3 })

		Method (_STA)
		{
			Return (0xF)
		}
	}
}

Scope (\_SB.PCI0.I2C4)
{
	/* Headphone Codec */
	Device (HPMC)
	{
		Name (_HID, "10508825")
		Name (_DDN, "NAU88L25 Codec")
		Name (_UID, 1)
		Name (_S0W, 4)
		Name (_DSD, Package () {
			ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package () {
			/* Enable jack detection via JKDET pin */
			Package () { "nuvoton,jkdet-enable", 1 },
			/*
			 * JKDET pin is pulled up by R389 on board.
			 * JKDET pin polarity = active low
			 */
			Package () { "nuvoton,jkdet-pull-enable", 1 },
			Package () { "nuvoton,jkdet-pull-up", 1 },
			Package () { "nuvoton,jkdet-polarity", 1 },
			/* VDDA(1.8) * 1.53 = 2.754 */
			Package () { "nuvoton,micbias-voltage", 6 },
			/* VREF Impedance = 125 kOhm */
			Package () { "nuvoton,vref-impedance", 2 },
			/*
			 * Setup 4 buttons impedance according to
			 * Android specification
			 */
			Package () { "nuvoton,sar-threshold-num", 4 },
			Package () { "nuvoton,sar-threshold",
				Package() { 0x0a, 0x14, 0x26, 0x73 } },
			/*
			 * Coeff 0-15 used to adjust threshold level
			 * 0 for low resist range
			 */
			Package () { "nuvoton,sar-hysteresis", 0 },
			/* SAR tracking gain based on 2.754 micbias-voltage */
			Package () { "nuvoton,sar-voltage", 6 },
			/* 100ms short key press debounce */
			Package () { "nuvoton,short-key-debounce", 3 },
			/* 2^(7+2) = 512 ms insert/eject debounce */
			Package () { "nuvoton,jack-insert-debounce", 7 },
			/* debounce not needed for eject normally */
			Package () { "nuvoton,jack-eject-debounce", 0 },
			}
		})

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				BOARD_HP_MIC_CODEC_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C4",
			)
			Interrupt (ResourceConsumer, Level, ActiveLow)
			{
				BOARD_HP_MIC_CODEC_IRQ
			}
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}
}
