/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Google Inc.
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

#define BOARD_TOUCHPAD_I2C_ADDR			0x20
#define BOARD_TOUCHPAD_IRQ			TOUCHPAD_INT_L

#define BOARD_TOUCHSCREEN_I2C_ADDR		0x20
#define BOARD_TOUCHSCREEN_IRQ			TOUCHSCREEN_INT_L
#define BOARD_TOUCHSCREEN_WAKE			GPE_TOUCHSCREEN_WAKE

#define BOARD_HP_MIC_CODEC_I2C_ADDR		0x1a
#define BOARD_HP_MIC_CODEC_IRQ			MIC_INT_L
#define BOARD_LEFT_SPEAKER_AMP_I2C_ADDR		0x34
#define BOARD_RIGHT_SPEAKER_AMP_I2C_ADDR	0x35

#define BOARD_EMR_I2C_ADDR	0x09
#define BOARD_EMR_IRQ		EMR_INT_L

#define BOARD_VIBRATOR_I2C_ADDR     0x5a
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

		#include <drivers/intel/wifi/acpi/wrdd.asl>
	}
}

Scope (\_SB.PCI0.I2C0)
{
	Name (FMCN, Package () { 87, 197, 26 })

	/* Touchscreen */
	Device (SNTS)
	{
		Name (_HID, "SYNA7817")
		Name (_CID, "PNP0C50")
		Name (_UID, 1)
		Name (_S0W, 4) /* Allow device to power off in S0 */
		Name (ISTP, 0) /* TouchScreen */

		/* Fetch HidDescriptorAddress, Register offset in the
		 * I2C device at which the HID descriptor can be read
		 */
		Method (_DSM, 4, NotSerialized)
		{
			If (LEqual (Arg0, ToUUID (
				"3cdff6f7-4267-4555-ad05-b30a3d8938de")))
			{
				// DSM Revision
				If (LEqual (Arg2, Zero))
				{
					If (LEqual (Arg1, One))
					{
						Return (Buffer (One)
						{
							0x03
						})
					}
					Else
					{
						Return (Buffer (One)
						{
							0x00
						})
					}
				}
				// HID Function
				If (LEqual (Arg2, One))
				{
					Return (0x20)
				}
			}
			Else
			{
				Return (Buffer (One)
				{
					0x00
				})
			}

			Return (Zero)
		}

		Method(_CRS, 0x0, NotSerialized)
		{
			Name (BUF0, ResourceTemplate ()
			{
				I2cSerialBus(
					BOARD_TOUCHSCREEN_I2C_ADDR, /* SlaveAddress */
					ControllerInitiated,        /* SlaveMode */
					400000,                     /* ConnectionSpeed */
					AddressingMode7Bit,         /* AddressingMode */
					"\\_SB.I2C0",               /* ResourceSource */
				)
				Interrupt (ResourceConsumer, Edge, ActiveLow)
				{
					BOARD_TOUCHSCREEN_IRQ
				}
			})
			Return (BUF0)
		}

		Method (_STA)
		{
			Return (0xF)
		}

		Name (_PRW, Package() { BOARD_TOUCHSCREEN_WAKE, 0x3 })
	}
}

Scope (\_SB.PCI0.I2C1)
{
	Name (FMCN, Package () { 87, 197, 26 })

	/* Virtual keyboard and touchpad */
	Device (VRKB)
	{
		Name (_HID, "SYNA7817")
		Name (_CID, "PNP0C50")
		Name (_UID, 2)
		Name (_S0W, 4) /* Allow device to power off in S0 */
		Name (ISTP, 1) /* Touchpad */

		/* Fetch HidDescriptorAddress, Register offset in the
		 * I2C device at which the HID descriptor can be read
		 */
		Method (_DSM, 4, NotSerialized)
		{
			If (LEqual (Arg0, ToUUID (
				"3cdff6f7-4267-4555-ad05-b30a3d8938de")))
			{
				// DSM Revision
				If (LEqual (Arg2, Zero))
				{
					If (LEqual (Arg1, One))
					{
						Return (Buffer (One)
						{
							0x03
						})
					}
					Else
					{
						Return (Buffer (One)
						{
							0x00
						})
					}
				}
				// HID Function
				If (LEqual (Arg2, One))
				{
					Return (0x20)
				}
			}
			Else
			{
				Return (Buffer (One)
				{
					0x00
				})
			}

			Return (Zero)
		}

		Method(_CRS, 0x0, NotSerialized)
		{
			Name (BUF0, ResourceTemplate ()
			{
				I2cSerialBus(
					BOARD_TOUCHPAD_I2C_ADDR,    /* SlaveAddress */
					ControllerInitiated,        /* SlaveMode */
					400000,                     /* ConnectionSpeed */
					AddressingMode7Bit,         /* AddressingMode */
					"\\_SB.I2C1",               /* ResourceSource */
				)
				Interrupt (ResourceConsumer, Edge, ActiveLow)
				{
					BOARD_TOUCHPAD_IRQ
				}
			})
			Return (BUF0)
		}

		Method (_STA)
		{
			Return (0xF)
		}
	}
	Device (DIGI)
	{
		Name (_HID, "ACPI0C50")
		Name (_CID, "PNP0C50")
		Name (_UID, 3)
		Name (_S0W, 4)

		Name (_CRS, ResourceTemplate ()
		{
			I2cSerialBus (
				BOARD_EMR_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C1",
			)
			Interrupt (ResourceConsumer, Level, ActiveLow)
			{
				BOARD_EMR_IRQ
			}
		})

		/*
		 * Function 1 returns the offset in the I2C device register
		 * address space at which the HID descriptor can be read.
		 *
		 * Arg0 = UUID
		 * Arg1 = revision number of requested function
		 * Arg2 = requested function number
		 * Arg3 = function specific parameter
		 */
		Method (_DSM, 4, NotSerialized)
		{
			If (LEqual (Arg0, ToUUID
				("3cdff6f7-4267-4555-ad05-b30a3d8938de"))) {
				If (LEqual (Arg2, Zero)) {
					/* Function 0 - Query */
					If (LEqual (Arg1, One)) {
						/* Revision 1 Function 1 */
						Return (Buffer (One) { 0x03 })
					} Else {
						/* Revision 2+ not supported */
						Return (Buffer (One) { 0x00 })
					}
				} ElseIf (LEqual (Arg2, One)) {
					/* Function 1 - HID Descriptor Addr */
					Return (0x0001)
				} Else {
					/* Function 2+ not supported */
					Return (Buffer (One) { 0x00 })
				}
			} Else {
				Return (Buffer (One) { 0x00 })
			}
		}

		Method (_STA)
		{
			Return (0xF)
		}
	}
}

Scope (\_SB.PCI0.I2C2)
{
	Name (FMCN, Package () { 87, 197, 26 })
	Device (VBRL)
	{
		Name (_HID, "PRP0001")
		Name (_CID, "DRV2604L")
		Name (_DDN, "TI Vibrator")
		Name (_UID, 0)
		/*
		 * Add properties with _DSD
		 * Device property values are documented in kernel doc
		 * Documentation/devicetree/bindings/input/ti,drv260x.txt
		 */
                Name (_DSD, Package () {
			ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package () {
				Package () {"compatible", Package () {"ti,drv2604l"}},
				/* Power up mode of the chip */
				Package () {"mode", 0x0},
				/* The library to use at power up */
				Package () {"library-sel", 0x0},
				/* The rated voltage of the actuator in millivolts */
				Package () {"vib-rated-mv", 2000},
				/* The overdrive voltage of the actuator in millivolts */
				Package () {"vib-overdrive-mv", 2000},
			}
		})
		Name (_CRS, ResourceTemplate ()
		{
			I2cSerialBus (
				BOARD_VIBRATOR_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C2",
			)
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}
}

Scope (\_SB.PCI0.I2C3)
{
	Name (FMCN, Package () { 87, 197, 26 })
	/* DRV2604L right */
	Device (VBRR)
	{
		Name (_HID, "PRP0001")
		Name (_CID, "DRV2604L")
		Name (_DDN, "TI Vibrator")
		Name (_UID, 1)

		/*
		 * Add properties with _DSD
		 * Device property values are documented in kernel doc
		 * Documentation/devicetree/bindings/input/ti,drv260x.txt
		 */
		Name (_DSD, Package () {
			ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package () {
				Package () {"compatible", Package () {"ti,drv2604l"}},
				/* Power up mode of the chip */
				Package () {"mode", 0x0},
				/* The library to use at power up */
				Package () {"library-sel", 0x0},
				/* The rated voltage of the actuator in millivolts */
				Package () {"vib-rated-mv", 2000},
				/* The overdrive voltage of the actuator in millivolts */
				Package () {"vib-overdrive-mv", 2000},
			}
		})

		Name (_CRS, ResourceTemplate ()
		{
			I2cSerialBus (
				BOARD_VIBRATOR_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C3",
			)
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}
}

Scope (\_SB.PCI0.I2C4)
{
	Name (FMCN, Package () { 87, 197, 26 })

	/* Headphone Codec */
	Device (HPMC)
	{
		Name (_HID, "10508825")
		Name (_DDN, "NAU88L25 Codec")
		Name (_UID, 1)

		/*
		 * Add DT style bindings with _DSD
		 * Device property values are documented in kernel doc
		 * Documentation/devicetree/bindings/sound/nau8825.txt
		 */
		Name (_DSD, Package () {
			ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
			Package () {
				/* Enable jack detection via JKDET pin */
				Package () {"nuvoton,jkdet-enable", 1},
				/*
				 * JKDET pin is pulled up by R389 on board.
				 * JKDET pin polarity = active low
				 */
				Package () {"nuvoton,jkdet-polarity", 1},
				/* VREF Impedance = 125 kOhm */
				Package () {"nuvoton,vref-impedance", 2},
				/* VDDA(1.8) * 1.53 = 2.754 */
				Package () {"nuvoton,micbias-voltage", 6},
				/*
				 * Setup 4 buttons impedance according to
				 * Android specification
				 */
				Package () {"nuvoton,sar-threshold-num", 4},
				Package () {"nuvoton,sar-threshold",
					Package () {0xc, 0x1e, 0x38, 0x60}},
				Package () {"nuvoton,sar-hysteresis", 1},
				/* VDDA for button impedance measurement */
				Package () {"nuvoton,sar-voltage", 0},
				/* 100ms short key press debounce */
				Package () {"nuvoton,short-key-debounce", 2},
				/* 2^(7+2) = 512 ms insert/eject debounce */
				Package () {"nuvoton,jack-insert-debounce", 7},
				Package () {"nuvoton,jack-eject-debounce", 7},
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

	/* Left Speaker Amp */
	Device (SPKL)
	{
		Name (_HID, "INT343B")
		Name (_DDN, "SSM4567 Speaker Amp")
		Name (_UID, 0)

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				BOARD_LEFT_SPEAKER_AMP_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C4",
			)
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}

	/* Right Speaker Amp */
	Device (SPKR)
	{
		Name (_HID, "INT343B")
		Name (_DDN, "SSM4567 Speaker Amp")
		Name (_UID, 1)

		Name (_CRS, ResourceTemplate()
		{
			I2cSerialBus (
				BOARD_RIGHT_SPEAKER_AMP_I2C_ADDR,
				ControllerInitiated,
				400000,
				AddressingMode7Bit,
				"\\_SB.PCI0.I2C4",
			)
		})

		Method (_STA)
		{
			Return (0xF)
		}
	}
}
