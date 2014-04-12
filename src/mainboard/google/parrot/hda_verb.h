/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 The ChromiumOS Authors.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * Parrot audio ports:
 * ALC269:
 *  out:
 *   Internal Speaker: PORT D (0x14)
 *   Headphone: PORT A - (0x21) (SenseA)
 *
 *  in:
 *   Mic2: PORT F (0x19) (SenseB)
 *   Line2 (internal Mic): PORT E (0x1B)
 *   PCBeep
 *
 *  HDMI PatherPoint
 */

static const u32 mainboard_cim_verb_data[] = {
	/* coreboot specific header */
	0x10ec0269,	// Codec Vendor / Device ID: Realtek ALC269
	0x10250742,	// Subsystem ID
	0x0000000B,	// Number of jacks (NID entries)


	/* NID 0x01, HDA Codec Subsystem ID Verb Table: 0x10250742 */
	0x00172042,
	0x00172107,
	0x00172225,
	0x00172310,

	/* Pin Widget Verb Table */

	/* Pin Complex (NID 0x12) DMIC */
	0x01271cf0,
	0x01271d11,
	0x01271e11,
	0x01271f41,

	/* Pin Complex (NID 0x14) SPKR-OUT PORTD */
	0x01471c10, // group 1, front left/right
	0x01471d01, // no connector, no jack detect
	0x01471e17, // speaker out, analog
	0x01471f90, // fixed function, internal

	/* Pin Complex (NID 0x17)  */
	0x01771cf0,
	0x01771d11,
	0x01771e11,
	0x01771f41,

	/* Pin Complex (NID 0x18)  MIC1 PORTB */
	0x01871cf0,
	0x01871d11,
	0x01871e11,
	0x01871f41,

	/* Pin Complex (NID 0x19)  MIC2 PORTF */
	0x01971c21, // group 2, cap 1
	0x01971d10, // black, jack detect
	0x01971ea7, // mic in, analog
	0x01971f04, // connector, right panel

	/* Pin Complex (NID 0x1A)  LINE1 PORTC */
	0x01a71cf0,
	0x01a71d11,
	0x01a71e11,
	0x01a71f41,

	/* Pin Complex (NID 0x1B)  LINE2 PORTE */
	0x01b71c20, // group 2, cap 0
	0x01b71d01, // no connector, no jack detect
	0x01b71ea7, // mic in, analog
	0x01b71f90, // fixed function, internal

	/* Pin Complex (NID 0x1d)  PCBeep */
	0x01d71c2d, // eapd low on ex-amp, laptop, custom enable
	0x01d71d81, // mute spkr on hpout
	0x01d71e15, // pcbeep en able, checksum
	0x01d71f40, // no physical, internal

	/* Pin Complex (NID 0x1E)  SPDIF-OUT */
	0x01e71cf0,
	0x01e71d11,
	0x01e71e11,
	0x01e71f41,

	/* Pin Complex (NID 0x21) HPOUT PORTA? */
	0x02171c1f, // group1,
	0x02171d10, // black, jack detect
	0x02171e21, // HPOut, 1/8 stereo
	0x02171f04, // connector, right panel

	/* --- Next Codec --- */

	/* coreboot specific header */
	0x80862806,	// Codec Vendor / Device ID: Intel PantherPoint HDMI
	0x80860101,	// Subsystem ID
	0x00000004,	// Number of jacks

	/* NID 0x01, HDA Codec Subsystem ID Verb Table: 0x80860101 */
	0x00172001,
	0x00172101,
	0x00172286,
	0x00172380,

	/* Pin Complex (NID 0x05) Digital Out at Int HDMI */
	0x30571c10,
	0x30571d00,
	0x30571e56,
	0x30571f18,

	/* Pin Complex (NID 0x06) Digital Out at Int HDMI */
	0x30671c20,
	0x30671d00,
	0x30671e56,
	0x30671f18,

	/* Pin Complex (NID 0x07) Digital Out at Int HDMI */
	0x30771c30,
	0x30771d00,
	0x30771e56,
	0x30771f18
};

