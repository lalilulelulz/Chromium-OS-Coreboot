/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Google Inc.
 * Copyright (C) 2015 Intel Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
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

#include "onboard.h"

Scope (\_SB)
{
	Device (LID0)
	{
		Name (_HID, EisaId ("PNP0C0D"))
		Method (_LID, 0)
		{
			Store (\_SB.PCI0.LPCB.EC0.LIDS, \LIDS)
			Return (\LIDS)
		}
	}

	Device (PWRB)
	{
		Name (_HID, EisaId ("PNP0C0C"))
		Name (_UID, 1)
	}
}

/*
 * LPC Trusted Platform Module
 */
Scope (\_SB.PCI0.LPCB)
{
	#include <drivers/pc80/tpm/acpi/tpm.asl>
}

Scope (\_SB.I2C5)
{

	/* Realtek Audio Codec */
	Device (RTEK)   /* Audio Codec driver I2C */
	{
		Name (_ADR, 0)
		Name (_HID, AUDIO_CODEC_HID)
		Name (_CID, AUDIO_CODEC_CID)
		Name (_DDN, AUDIO_CODEC_DDN)
		Name (_UID, 1)

		Method(_CRS, 0x0, NotSerialized)
		{
			Name(SBUF,ResourceTemplate ()
			{
				I2CSerialBus(
					AUDIO_CODEC_I2C_ADDR,	/* SlaveAddress: bus address */
					ControllerInitiated,	/* SlaveMode: default to ControllerInitiated */
					400000,			/* ConnectionSpeed: in Hz */
					AddressingMode7Bit,	/* Addressing Mode: default to 7 bit */
					"\\_SB.I2C5",		/* ResourceSource: I2C bus controller name */
				)


			 /* Jack Detect (index 0) */
			 GpioInt (Edge, ActiveLow, ExclusiveAndWake, PullNone,,
				  "\\_SB.GPSW") { JACK_DETECT_GPIO_INDEX }
			} )
			Return (SBUF)
		}

		Method (_STA)
		{
			If (LEqual (\S5EN, 1)) {
				If (LEqual (\BDID, BOARD_BCRD2)) {
					Return (0xF)
				}
			}
			Return (0x0)
		}
	}
}

Scope (\_SB.I2C6)
{
	Device (ATPB)
	{
		Name (_HID, "ATML0000")
		Name (_DDN, "Atmel Touchpad Bootloader")
		Name (_UID, 1)
		Name (ISTP, 1) /* Touchpad */

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x26,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.I2C6",             // ResourceSource
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				183
			}
		})

		Method (_STA)
		{
			If (LEqual (\S6EN, 1)) {
				Return (0xF)
			} Else {
				Return (0x0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}

	Device (ATPA)
	{
		Name (_HID, "ATML0000")
		Name (_DDN, "Atmel Touchpad")
		Name (_UID, 2)
		Name (ISTP, 1) /* Touchpad */
		Name (_PRW, Package() { BOARD_TRACKPAD_WAKE_GPIO, 0x3 })

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x4a,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.I2C6",             // ResourceSource
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				183
			}
		})

		Method (_STA)
		{
			If (LEqual (\S6EN, 1)) {
				Return (0xF)
			} Else {
				Return (0x0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}
}

Scope (\_SB.LPEA)
{
	Name (GBUF, ResourceTemplate ()
	{
		/* Jack Detect (index 0) */
		GpioInt (Edge, ActiveLow, ExclusiveAndWake, PullNone,,
			 "\\_SB.GPSW") { JACK_DETECT_GPIO_INDEX }
	})
}

Scope (\_SB.GPNC)
{
	Method (_AEI, 0, NotSerialized)  // _AEI: ACPI Event Interrupts
	{
		Name (RBUF, ResourceTemplate ()
		{
			GpioInt (Edge, ActiveLow, ExclusiveAndWake, PullNone,,
				"\\_SB.GPNC") { BOARD_SCI_GPIO_INDEX }
		})
		Return (RBUF)
	}

	Method (_E0F, 0, NotSerialized)  // _Exx: Edge-Triggered GPE
	{
	}
}
