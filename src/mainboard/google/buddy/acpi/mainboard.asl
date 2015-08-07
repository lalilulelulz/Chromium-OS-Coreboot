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

#include <mainboard/google/buddy/onboard.h>

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
}

Scope (\_SB.PCI0.RP01)
{
	Device (WLAN)
	{
		Name (_ADR, 0x00000000)

		/* GPIO10 is WLAN_WAKE_L_Q */
		Name (GPIO, 10)

		Name (_PRW, Package() { GPIO, 3 })

		Method (_DSW, 3, NotSerialized)
		{
			If (LEqual (Arg0, 1)) {
				// Enable GPIO as wake source
				\_SB.PCI0.LPCB.GPIO.GWAK (^GPIO)
			}
		}
	}
}
