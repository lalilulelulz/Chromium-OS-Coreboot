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

/*	Vendor Name    : IDT
 *	Vendor ID      : 0x111d76e5
 *	Subsystem ID   : 0x103c18f9
 *	Revision ID    : 0x100303
 */


static const u32 mainboard_cim_verb_data[] = {
	/* coreboot specific header */
	0x111D76E5,	// Codec Vendor / Device ID: IDT 92HD99
	0x103C18F9,	// Subsystem ID
	0x00000009,	// Number of IDs

/* Bits 31:28 - Codec Address */
/* Bits 27:20 - NID */
/* Bits 19:8 - Verb ID */
/* Bits 7:0  - Payload */

/* NID 0x01 - NodeInfo */
	0x001720F9,
	0x00172118,
	0x0017223C,
	0x00172310,

/* NID 0x0A - External Microphone Connector */
	0x00A71C20,
	0x00A71D10,
	0x00A71EA1,
	0x00A71F04,

/* NID 0x0B - Headphone Connector */
	0x00B71C1F,
	0x00B71D10,
	0x00B71E21,
	0x00B71F04,

/* NID 0x0C - Not connected */
	0x00C71CF0,
	0x00C71D00,
	0x00C71EF0,
	0x00C71F40,

/* NID 0x0D - Internal Speakers */
	0x00D71C10,
	0x00D71D01,
	0x00D71E17,
	0x00D71F90,

/* NID 0x0F - Not connected */
	0x00F71CF0,
	0x00F71D00,
	0x00F71EF0,
	0x00F71F40,

/* NID 0x11 - Internal Microphone */
	0x01171C40,
	0x01171D01,
	0x01171EA3,
	0x01171FD5,

/* Misc entries */
	0x00170500, /* Set power state to D0 */
	0x00D70740, /* Enable Port D */
	0x0017EE31, /* Enable PC Beep */
	0x00177400, /* Reserved */
	0x02277338, /* Set Left & Right speaker volume */
	0x02279506, /* Select IIR coefficients for BTL
	               amplifier high pass filter corner frequency */
	0x0017C621, /* Slave Port - Port A used as microphone input for
	                            combo Jack
	               Master Port - Port B used for Jack Presence Detect
	               Enable Combo Jack Detection */
	0x0017F506, /* Set TSTripHish to 170 Degrees C */


	/* --- Next Codec --- */

/*	Vendor Name    : Intel
 *	Vendor ID      : 0x80862806
 *	Subsystem ID   : 0x80860101
 *	Revision ID    : 0x100000
 */
	/* coreboot specific header */
	0x80862806,	// Codec Vendor / Device ID: Intel PantherPoint HDMI
	0x80860101,	// Subsystem ID
	0x00000004,	// Number of IDs

	/* NID 0x01, HDA Codec Subsystem ID Verb Table: 0x80860101 */
	0x30172001,
	0x30172101,
	0x30172286,
	0x30172380,

	/* Pin Complex (NID 0x05) Digital Out at Int HDMI */
	0x30571c10,
	0x30571d00,
	0x30571e56,
	0x30571f18,

	/* Pin Complex (NID 0x06) Not Connected */
	0x30671c20,
	0x30671d00,
	0x30671e56,
	0x30671f58,

	/* Pin Complex (NID 0x07) Not Connected */
	0x30771c30,
	0x30771d00,
	0x30771e56,
	0x30771f58
};

