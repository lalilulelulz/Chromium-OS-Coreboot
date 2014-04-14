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
 * Kiev audio port configuration data structure. Also known as: Verb Table.
 *
 * Two codecs:
 *   #0:  RealTek ALC663 (most regular devices)
 *   #3:  Intel CougarPoint PCH (HDMI devices)
 *
 *  Out:
 *   Node 0x15: Internal Speakers, Port A
 *   Node 0x1D: Line out, Aux HDMI, Port C
 *   Node 0x1E: SPDIF out
 *   Node 0x21: Headphone, Port I
 *
 *  In:
 *   Node 0x12: Digital Mic, internal camera module
 *   Node 0x18: Analog Mic, external. Mic1, Port B
 *
 *  All other ports are no-connect.
 *
 * --------------------------------------------------------------------------
 *
 *  HDA Configuration Default Register:
 *  31:30      29:24  23:20   19:16    15:12  11:8  7:4       3:0
 *  Port       Locatn Default Connectn Color  Misc  Default   Sequence
 *  Connectvty        Device  Type                  Associatn
 *
 * The register and its field definitions is fully documented in,
 * Intel "High Definition Audio Specification", Revision 1.0a, June 17, 2010.
 * Refer to section "7.3.3.31 - Configuration Default", Pages 177-181.
 *
 */

static const u32 mainboard_cim_verb_data[] = {
	/* Codec Subsystem Header
	 *   for configuring ALC663, non-HDMI, nodes
	 */
	0x10Ec0663,	// Vendor/Device: Realtek ALC663
	0x10250640,	// Subsystem ID
	0x0000000F,	// Number of node entries that follow.

	/* Pin Widget Verb Table */

	/* NID 0x01, HDA Codec Subsystem ID Verb Table: 0x10ec0000 */
	0x00172000,
	0x00172100,
	0x001722EC,
	0x00172310,

	/* NID 0x11, S/PDIF-OUT2 */
	0x01171CF0,	/* No connect, default 411111F0 */
	0x01171D11,
	0x01171E11,
	0x01171F41,

	/* NID 0x12, Digital MIC in Camera Module */
	0x01271C40,	/* Mic Input, 99A30940 */
	0x01271D09,
	0x01271EA3,
	0x01271F99,

	/* NID 0x13 */
	0x01471CF0,	/* No connect, default 411111F0 */
	0x01471D11,
	0x01471E11,
	0x01471F41,

	/* NID 0x14, Spkr Ext Rear */
	0x01471CF0,	/* No connect, default 411111F0 */
	0x01471D11,
	0x01471E11,
	0x01471F41,

	/* NID 0x15, Internal Speakers (port A): 0x99130110 */
	0x01571C10,	/* Front Left and Right */
	0x01571D01,	/* Color unknown. Jack Detect Override */
	0x01571E13,	/* Speaker Out */
	0x01571F99,

	/* NID 0x16, Spkr Ext Rear */
	0x01671CF0,	/* No connect, default 411111F0 */
	0x01671D11,
	0x01671E11,
	0x01671F41,

	/* NID 0x17, Spkr Ext Rear */
	0x01771CF0,	/* No connect, default 411111F0 */
	0x01771D11,
	0x01771E11,
	0x01771F41,

	/* NID 0x18, MIC1: 0x04A19830 */
	0x01871C30,	/* Group 3 & Sequence 0 */
	0x01871D98,	/* Pink */
	0x01871EA1,	/* MicIn & 1/8” Stereo/mono */
	0x01871F04,	/* Side panel jack */

	/* NID 0x19, Spkr Ext Rear */
	0x01971CF0,	/* No connect, default 411111F0 */
	0x01971D11,
	0x01971E11,
	0x01971F41,

	/* NID 0x1A, Line In, Port C, Ext: 0x90800050 */
	0x01A71C50, /* Device Group 5, Sequence 0 */
	0x01A71D00, /* Color: unknown */
	0x01A71E80, /* Line 1 In */
	0x01A71F90, /* Internal */

	/* NID 0x1B, Spkr Ext Rear */
	0x01B71CF0,	/* No connect, default 411111F0 */
	0x01B71D11,
	0x01B71E11,
	0x01B71F41,

	/* NID 0x1D, Line Out, Port C, Ext: 0x40079a2d */
	0x01D71C2d,
	0x01D71D9a,
	0x01D71E07,
	0x01D71F40,

	/* NID 0x1E, S/PDIF Out Ext: 0x04451120 */
	0x01E71C20,	/* Device Group 2 & Front Left and Right */
	0x01E71D11,	/* Black. Jack Detect Override */
	0x01E71E45,	/* S/PDIF Out & Other Digital Out */
	0x01E71F04,	/* Rear Panel connector */

	/* NID 0x21, HP-OUT, port I: 0421101F */
	0x02171C1F,	/* Device Group 2 & Front Left and Right */
	0x02171D10,	/* Black. Jack detect */
	0x02171E21,	/* HP Out & 1/8” Stereo */
	0x02171F04,	/* Side Panel Connector */


	/* Codec Subsystem Header
	 *   for configuring Intel CougarPoint HDMI nodes
	 */
	0x80862805,	// Vendor/Device: Intel CougarPoint HDMI
	0x80862805,	// Subsystem ID
	0x00000004,	// Number of node entries that follow.

	/* Node 0x01. HDA Codec Subsystem ID Verb Table: 0x80860101 */
	0x00172001,
	0x00172101,
	0x00172286,
	0x00172380,

	/* Node 0x05. Digital Out, Int HDMI: 0x58560010 */
	0x30571C10,
	0x30571D00,
	0x30571E56,
	0x30571F58,

	/* Node 0x06. Digital Out, Int HDMI: 0x58560020 */
	0x30671C20,
	0x30671D00,
	0x30671E56,
	0x30671F58,

	/* Node 0x07. Digital Out, Int HDMI: 0x18560030 */
	0x30771C30,
	0x30771D00,
	0x30771E56,
	0x30771F18
};

