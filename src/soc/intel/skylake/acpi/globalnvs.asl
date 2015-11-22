/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
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

/* Global Variables */

Name (\PICM, 0)		// IOAPIC/8259

/*
 * Global ACPI memory region. This region is used for passing information
 * between coreboot (aka "the system bios"), ACPI, and the SMI handler.
 * Since we don't know where this will end up in memory at ACPI compile time,
 * we have to fix it up in coreboot's ACPI creation phase.
 */

External (NVSA)

OperationRegion (GNVS, SystemMemory, NVSA, 0x2000)
Field (GNVS, ByteAcc, NoLock, Preserve)
{
	/* Miscellaneous */
	Offset (0x00),
	OSYS,	16,	// 0x00 - Operating System
	SMIF,	8,	// 0x02 - SMI function
	PRM0,	8,	// 0x03 - SMI function parameter
	PRM1,	8,	// 0x04 - SMI function parameter
	SCIF,	8,	// 0x05 - SCI function
	PRM2,	8,	// 0x06 - SCI function parameter
	PRM3,	8,	// 0x07 - SCI function parameter
	LCKF,	8,	// 0x08 - Global Lock function for EC
	PRM4,	8,	// 0x09 - Lock function parameter
	PRM5,	8,	// 0x0a - Lock function parameter
	PCNT,	8,	// 0x0b - Processor Count
	PPCM,	8,	// 0x0c - Max PPC State
	TMPS,	8,	// 0x0d - Temperature Sensor ID
	TLVL,	8,	// 0x0e - Throttle Level Limit
	FLVL,	8,	// 0x0f - Current FAN Level
	TCRT,	8,	// 0x10 - Critical Threshold
	TPSV,	8,	// 0x11 - Passive Threshold
	TMAX,	8,	// 0x12 - CPU Tj_max
	S5U0,	8,	// 0x13 - Enable USB in S5
	S3U0,	8,	// 0x14 - Enable USB in S3
	S33G,	8,	// 0x15 - Enable 3G in S3
	LIDS,	8,	// 0x16 - LID State
	PWRS,	8,	// 0x17 - AC Power State
	CMEM,	32,	// 0x18 - 0x1b - CBMEM TOC
	CBMC,	32,	// 0x1c - 0x1f - Coreboot Memory Console
	PM1I,	64,	// 0x20 - 0x27 - PM1 wake status bit
	GPEI,	64,	// 0x28 - 0x2f - GPE wake status bit
	DPTE,	8,	// 0x30 - Enable DPTF
	NHLA,	64,	// 0x31 - NHLT Address
	NHLL,	32,	// 0x39 - NHLT Length

	/* ChromeOS specific */
	Offset (0x100),
	#include <vendorcode/google/chromeos/acpi/gnvs.asl>
}

/* Set flag to enable USB charging in S3 */
Method (S3UE)
{
	Store (One, \S3U0)
}

/* Set flag to disable USB charging in S3 */
Method (S3UD)
{
	Store (Zero, \S3U0)
}

/* Set flag to enable USB charging in S5 */
Method (S5UE)
{
	Store (One, \S5U0)
}

/* Set flag to disable USB charging in S5 */
Method (S5UD)
{
	Store (Zero, \S5U0)
}
