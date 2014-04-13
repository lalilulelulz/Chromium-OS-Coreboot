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

//	HDA data from fwts
//	Vendor Name    : IDT
//	Vendor ID      : 0x111d76e5
//	Subsystem ID   : 0x103c18f9
//	Revision ID    : 0x100303
//	BIOS pin configurations:
//	  Pin  Setting
//	  0x000a 0x04 a1 10 20
//	  0x000b 0x04 21 10 1f
//	  0x000c 0x40f000f0
//	  0x000d 0x90170110
//	  0x000f 0x40f000f0
//	  0x0011 0xd5a30140


static const u32 mainboard_cim_verb_data[] = {
	/* coreboot specific header */
	0x111D76E5,	// Codec Vendor / Device ID: IDT 92HD99
	0x103C18F9,	// Subsystem ID
	0x00000009,	// Number of IDs

/* NID 0x01 */
	0x001720F9,
	0x00172118,
	0x0017223C,
	0x00172310,

/* NID 0x0A */
	0x00A71C20,
	0x00A71D10,
	0x00A71EA1,
	0x00A71F04,

/* NID 0x0B */
	0x00B71C1F,
	0x00B71D10,
	0x00B71E21,
	0x00B71F04,

/* NID 0x0C */
	0x00C71CF0,
	0x00C71D00,
	0x00C71EF0,
	0x00C71F40,

/* NID 0x0D */
	0x00D71C10,
	0x00D71D01,
	0x00D71E17,
	0x00D71F90,

/* NID 0x0F */
	0x00F71CF0,
	0x00F71D00,
	0x00F71EF0,
	0x00F71F40,

/* NID 0x11 */
	0x01171C40,
	0x01171D01,
	0x01171EA3,
	0x01171FD5,

/* Need to verifiy these two entries */
	0x00170500,
	0x00D70740,
	0x0017EE31,
	0x00177400,

	0x02277338,
	0x02279506,
	0x0017C621,
	0x0017F506,

//	HDA data from fwts
//	Vendor Name    : Intel
//	Vendor ID      : 0x80862806
//	Subsystem ID   : 0x80860101
//	Revision ID    : 0x100000
//	BIOS pin configurations:
//	  Pin  Setting
//	  0x0005 0x18560010
//	  0x0006 0x58560020
//	  0x0007 0x58560030

	/* coreboot specific header */
	0x80862806,	// Codec Vendor / Device ID: Intel PantherPoint HDMI
	0x80860101,	// Subsystem ID
	0x00000004,	// Number of IDs

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
	0x30671f18, // doesn't match fwts output

	/* Pin Complex (NID 0x07) Digital Out at Int HDMI */
	0x30771c30,
	0x30771d00,
	0x30771e56,
	0x30771f18	// doesn't match fwts output
};

