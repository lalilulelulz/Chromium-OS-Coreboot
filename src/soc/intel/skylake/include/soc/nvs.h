/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2008-2009 coresystems GmbH
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

#ifndef _SOC_NVS_H_
#define _SOC_NVS_H_

#include <rules.h>
#include <vendorcode/google/chromeos/gnvs.h>

typedef struct {
	/* Miscellaneous */
	u16	osys; /* 0x00 - Operating System */
	u8	smif; /* 0x02 - SMI function call ("TRAP") */
	u8	prm0; /* 0x03 - SMI function call parameter */
	u8	prm1; /* 0x04 - SMI function call parameter */
	u8	scif; /* 0x05 - SCI function call (via _L00) */
	u8	prm2; /* 0x06 - SCI function call parameter */
	u8	prm3; /* 0x07 - SCI function call parameter */
	u8	lckf; /* 0x08 - Global Lock function for EC */
	u8	prm4; /* 0x09 - Lock function parameter */
	u8	prm5; /* 0x0a - Lock function parameter */
	u8      pcnt; /* 0x0b - Processor Count */
	u8	ppcm; /* 0x0c - Max PPC State */
	u8	tmps; /* 0x0d - Temperature Sensor ID */
	u8	tlvl; /* 0x0e - Throttle Level Limit */
	u8	flvl; /* 0x0f - Current FAN Level */
	u8	tcrt; /* 0x10 - Critical Threshold */
	u8	tpsv; /* 0x11 - Passive Threshold */
	u8	tmax; /* 0x12 - CPU Tj_max */
	u8	s5u0; /* 0x13 - Enable USB in S5 */
	u8	s3u0; /* 0x14 - Enable USB in S3 */
	u8	s33g; /* 0x15 - Enable 3G in S3 */
	u8	lids; /* 0x16 - LID State */
	u8	pwrs; /* 0x17 - AC Power State */
	u32	cmem; /* 0x18 - 0x1b - CBMEM TOC */
	u32	cbmc; /* 0x1c - 0x1f - Coreboot Memory Console */
	u64	pm1i; /* 0x20 - 0x27 - PM1 wake status bit */
	u64	gpei; /* 0x28 - 0x2f - GPE wake status bit */
	u8	dpte; /* 0x30 - Enable DPTF */
	u64	nhla; /* 0x31 - NHLT Address */
	u32	nhll; /* 0x39 - NHLT Length */
	u16	cid1; /* 0x3d - Wifi Country Identifier */
	u32	pl1l; /* 0x3f - DPTF PL1 Minimum */
	u32	pl1h; /* 0x43 - DPTF PL1 Maximum */
	u8	tr1p; /* 0x47 - DPTF TSR1 Passive */
	u8	tr2p; /* 0x48 - DPTF TSR2 Passive */
	u32	cuif; /* 0x49 - DPTF TCPU influence */
	u32	cusp; /* 0x4D - DPTF TCPU sample period */
	u32	t2if; /* 0x51 - DPTF TSR2 influence */
	u32	t2sp; /* 0x55 - DPTF TSR2 sample period */
	u8	alse; /* 0x59 - ALS device present */
	u8	kble; /* 0x5a - Keyboard backlight present */
	u8	unused[165];

	/* ChromeOS specific (0x100 - 0xfff) */
	chromeos_acpi_t chromeos;
} __attribute__((packed)) global_nvs_t;

#if ENV_SMM
/* Used in SMM to find the ACPI GNVS address */
global_nvs_t *smm_get_gnvs(void);
#endif

#endif
