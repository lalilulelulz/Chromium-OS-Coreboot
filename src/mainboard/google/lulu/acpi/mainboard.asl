/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Google Inc.
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <mainboard/google/lulu/onboard.h>

Scope (\_SB)
{
	Device (LID0)
	{
		Name(_HID, EisaId("PNP0C0D"))
		Method(_LID, 0)
		{
			Store (\_SB.PCI0.LPCB.EC0.LIDS, \LIDS)
			Return (\LIDS)
		}


		// There is no GPIO for LID, the EC pulses WAKE# pin instead.
		// There is no GPE for WAKE#, so fake it with PCI_EXP_WAKE
		Name (_PRW, Package(){ 0x69, 5 }) // PCI_EXP
	}

	Device (PWRB)
	{
		Name(_HID, EisaId("PNP0C0C"))
	}

	/* Keyboard Backlight interface via EC */
	Device (KBLT)
	{
		Name (_HID, "GOOG0002")
		Name (_UID, 1)
		Name (_ADR, 0)

		/* Read current backlight value */
		Method (KBQC, 0)
		{
			Return (\_SB.PCI0.LPCB.EC0.KBLV)
		}

		/* Write new backlight value */
		Method (KBCM, 1)
		{
			Store (Arg0, \_SB.PCI0.LPCB.EC0.KBLV)
		}
	}
}

Scope (\_SB.PCI0.I2C0)
{
	Device (STPA)
	{
		Name (_HID, "SYNA0000")
		Name (_CID, "PNP0C50")
		Name (_DDN, "Synaptics Touchpad")
		Name (_UID, 1)
		Name (ISTP, 1) /* Touchpad */

		Method(_CRS, 0x0, NotSerialized)
		{
			Name (RBUF, ResourceTemplate()
			{
				I2cSerialBus (
					0x2C,                     // SlaveAddress
					ControllerInitiated,      // SlaveMode
					400000,                   // ConnectionSpeed
					AddressingMode7Bit,       // AddressingMode
					"\\_SB.PCI0.I2C0",        // ResourceSource
				)
				Interrupt (ResourceConsumer, Edge, ActiveLow)
				{
					BOARD_TRACKPAD_IRQ
				}
			})
			Return(RBUF)
		}

		Method(_DSM, 0x4, NotSerialized)
		{
			switch(ToBuffer(Arg0))
			{
				case(ToUUID("3CDFF6F7-4267-4555-AD05-B30A3D8938DE")) /* I2C-HID UUID */
				{
					switch(ToInteger(Arg2)) /* DSM Function */
					{
						case(0) /* Function 0: Query function, return based on revision */
						{
							switch(ToInteger(Arg1)) /* Arg1 DSM Revision */
							{
								case(1) /* Revision 1: Function 1 supported */
								{
									Return(Buffer(One) { 0x03 })
								}
								default /* Revision 2+: no functions supported */
								{
									Return(Buffer(One) { 0x00 })
								}
							}
						}
						case(1) /* Function 1 : HID Function */
						{
							Return(0x0020) /* HID Descriptor Address */
						}
						default {} /* Functions 2+: not supported */
					}
				}
				default
				{
					Return(Buffer(One) { 0x00 }) /* No other GUIDs supported */
				}
			}
		}

		Method (_STA)
		{
			If (LEqual (\S1EN, 1)) {
				If (LEqual (\TID1, 0)) {
					Return (0xF)
				} Else {
					If(LEqual (\TID1, 1)) {
						Return (0xF)
					}
					Return (0x0)
				}
			} Else {
				Return (0x0)
			}
		}

		Name (_PRW, Package() { BOARD_TRACKPAD_WAKE_GPIO, 0x3 })

		Method (_DSW, 3, NotSerialized)
		{
			Store (BOARD_TRACKPAD_WAKE_GPIO, Local0)
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (Local0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}

	Device (ETPA)
	{
		Name (_HID, "ELAN0000")
		Name (_DDN, "Elan Touchpad")
		Name (_UID, 1)
		Name (ISTP, 1) /* Touchpad */

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x15,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.PCI0.I2C0",        // ResourceSource
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				BOARD_TRACKPAD_IRQ
			}
		})

		Method (_STA)
		{
			If (LEqual (\S1EN, 1)) {
				If (LEqual (\TID1, 0)) {
					Return (0xF)
				} Else {
					If(LEqual (\TID1, 1)) {
						Return (0xF)
					}
					Return (0x0)
				}
			} Else {
				Return (0x0)
			}
		}

		Name (_PRW, Package() { BOARD_TRACKPAD_WAKE_GPIO, 0x3 })

		Method (_DSW, 3, NotSerialized)
		{
			Store (BOARD_TRACKPAD_WAKE_GPIO, Local0)
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (Local0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}

	Device (CTPA)
	{
		Name (_HID, "CYAP0000")
		Name (_DDN, "Cypress Touchpad")
		Name (_UID, 3)
		Name (ISTP, 1) /* Touchpad */

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x67,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.PCI0.I2C0",        // ResourceSource
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				BOARD_TRACKPAD_IRQ
			}
		})

		Method (_STA)
		{
			If (LEqual (\S1EN, 1)) {
				Return (0xF)
			} Else {
				Return (0x0)
			}
		}

		Name (_PRW, Package() { BOARD_TRACKPAD_WAKE_GPIO, 0x3 })

		Method (_DSW, 3, NotSerialized)
		{
			Store (BOARD_TRACKPAD_WAKE_GPIO, Local0)
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (Local0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}

	Device (CTPB)
	{
		Name (_HID, "CYAP0001")
		Name (_DDN, "Cypress Touchpad")
		Name (_UID, 3)
		Name (ISTP, 1) /* Touchpad */

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x24,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.PCI0.I2C0",        // ResourceSource
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				BOARD_TRACKPAD_IRQ
			}
		})

		Method (_STA)
		{
			If (LEqual (\S1EN, 1)) {
				If (LEqual (\TID1, 0)) {
					Return (0xF)
				} Else {
					If(LEqual (\TID1, 2)) {
						Return (0xF)
					}
					Return (0x0)
				}
			} Else {
				Return (0x0)
			}
		}

		Name (_PRW, Package() { BOARD_TRACKPAD_WAKE_GPIO, 0x3 })

		Method (_DSW, 3, NotSerialized)
		{
			Store (BOARD_TRACKPAD_WAKE_GPIO, Local0)
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (Local0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}
}
Scope (\_SB.PCI0.I2C1)
{
	Device (ATSA)
	{
		Name (_HID, "ATML0001")
		Name (_DDN, "Atmel Touchscreen")
		Name (_UID, 5)
		Name (_S0W, 4)
		Name (ISTP, 0) /* TouchScreen */

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x4a,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.PCI0.I2C1",        // ResourceSource
			)

			// GPIO54 (ball L3) is PIRQW: PIRQL_GSI + PIRQL - PIRQW = PIRQW_GSI
			// 27 + 3 - 14 = 38
			Interrupt (ResourceConsumer, Edge, ActiveLow) { 38 }
		})

		Name (_PRW, Package() { BOARD_TOUCHSCREEN_WAKE_GPIO, 0x3 })

		Method (_DSW, 3, NotSerialized)
		{
			Store (BOARD_TOUCHSCREEN_WAKE_GPIO, Local0)
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (Local0)
			}
		}

		Method (_STA)
		{
			If (LEqual (\S2EN, 1)) {
				If (LEqual (\TID2, 0)) {
					Return (0xF)
				} Else {
					If(LEqual (\TID2, 1)) {
						Return (0xF)
					}
					Return (0x0)
				}
			} Else {
				Return (0x0)
			}
		}
	}

	Device (ETSA)
	{
		Name (_HID, "ELAN0001")
		Name (_DDN, "Elan Touchscreen")
		Name (_UID, 6)
		Name (ISTP, 0) /* Touchscreen */

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				0x10,                     // SlaveAddress
				ControllerInitiated,      // SlaveMode
				400000,                   // ConnectionSpeed
				AddressingMode7Bit,       // AddressingMode
				"\\_SB.PCI0.I2C1",        // ResourceSource
			)
			Interrupt (ResourceConsumer, Edge, ActiveLow)
			{
				BOARD_TOUCHSCREEN_IRQ
			}
		})

		Method (_STA)
		{
			If (LEqual (\S2EN, 1)) {
				Return (0xF)
			} Else {
				Return (0x0)
			}
		}

		Name (_PRW, Package() { BOARD_TOUCHSCREEN_WAKE_GPIO, 0x3 })

		Method (_DSW, 3, NotSerialized)
		{
			Store (BOARD_TOUCHSCREEN_WAKE_GPIO, Local0)
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (Local0)
			}
		}

		/* Allow device to power off in S0 */
		Name (_S0W, 4)
	}
}
