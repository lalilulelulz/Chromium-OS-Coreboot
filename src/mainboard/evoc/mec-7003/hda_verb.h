/*
 * This file is part of the coreboot project.
 * 
 * Copyright (C) 2007-2009 coresystems GmbH
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

static const u32 mainboard_cim_verb_data[] = {
	/* coreboot specific header */
	0x10ec0272,	// Codec Vendor / Device ID: Realtek ALC262
	0x11702786,	// Subsystem ID
	0x00000014,	// Number of jacks

	/* NID 0x01, HDA Codec Subsystem ID Verb Table: 0x11702786 */
	0x00172086,
	0x00172127,
	0x00172270,
	0x00172311,
	
	/* Pin Widget Verb Table */

	/* Pin Complex (NID 0x20) */
	0x02050002,
	0x0204A2A0,
	0x02050003,
	0x02044843,

	/* Pin Complex (NID 0x01) */
	0x0017FF00,
	0x0017FF00,
	0x0017FF00,
	0x0017FF00,

	/* Pin Complex (NID 0x11) */
	0x01171CF0,
	0x01171D11,
	0x01171E11,
	0x01171F41,

	/* Pin Complex (NID 0x12) */
	0x01271C30,
	0x01271D09,
	0x01271EA3,
	0x01271F99,

	/* Pin Complex (NID 0x13) */
	0x01371CF0,
	0x01371D11,
	0x01371E11,
	0x01371F41,

	/* Pin Complex (NID 0x14) */
	0x01471C10,
	0x01471D01,
	0x01471E13,
	0x01471F99,

	/* Pin Complex (NID 0x15) */
	0x01571CF0,
	0x01571D11,
	0x01571E11,
	0x01571F41,

	/* Pin Complex (NID 0x16) */
	0x01671CF0,
	0x01671D11,
	0x01671E11,
	0x01671F41,

	/* Pin Complex (NID 0x17) */
	0x01771CF0,
	0x01771D11,
	0x01771E11,
	0x01771F41,

	/* Pin Complex (NID 0x18) */
	0x01871C20,
	0x01871D18,
	0x01871EA1,
	0x01871F01,

	/* Pin Complex (NID 0x19) */
	0x01971CF0,
	0x01971D11,
	0x01971E11,
	0x01971F41,

	/* Pin Complex (NID 0x1A) */
	0x01A71CF0,
	0x01A71D11,
	0x01A71E11,
	0x01A71F41,
	
	/* Pin Complex (NID 0x1B) */
	0x01B71CF0,
	0x01B71D11,
	0x01B71E11,
	0x01B71F41,
	
	/* Pin Complex (NID 0x1D) */
	0x01D71C2D,
	0x01D71D9A,
	0x01D71E17,
	0x01D71F40,
	
	/* Pin Complex (NID 0x1E) */
	0x01E71CF0,
	0x01E71D11,
	0x01E71E11,
	0x01E71F41,
	
	/* Pin Complex (NID 0x21) */
	0x02171C1F,
	0x02171D10,
	0x02171E21,
	0x02171F01,
	
	/* Pin Complex (NID 0x20) */
	0x02050003,
	0x02044003,
	0x02050003,
	0x02044003,

	/* PC Speaker */
	0x0205000D,
	0x0205000D,
	0x02042050,
	0x00C37080,

	0x00C37100,
	0x014707C0,
	0x0143B000,
	0x00B37416
};

