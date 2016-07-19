/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Google Inc.
 * Copyright (C) 2015 Intel Corp.
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

#ifndef _SOC_LPC_H_
#define _SOC_LPC_H_

/* PCI config registers in LPC bridge. */
#define REVID		0x08
#define ABASE		0x40
#define PBASE		0x44
#define GBASE		0x48
#define IOBASE		0x4c
#define IBASE		0x50
#define SBASE		0x54
#define MPBASE		0x58
#define PUBASE		0x5c
#define UART_CONT	0x80
#define RCBA		0xf0


#define RID_A_STEPPING_START 1
#define RID_B_STEPPING_START 5
#define RID_C_STEPPING_START 0xe
enum soc_stepping {
	STEP_A0,
	STEP_A1,
	STEP_B0,
	STEP_B1,
	STEP_B2,
	STEP_B3,
	STEP_C0,
};

/* Registers behind the RCBA_BASE_ADDRESS bar. */
#define GCS		0x00
#define BILD		(1 << 0)


typedef enum {
  SocA0          = 0,
  SocA1          = 1,
  SocA2          = 2,
  SocA3          = 3,
  SocA4          = 4,
  SocA5          = 5,
  SocA6          = 6,
  SocA7          = 7,
  SocB0          = 8,
  SocB1          = 9,
  SocB2          = 10,
  SocB3          = 11,
  SocB4          = 12,
  SocB5          = 13,
  SocB6          = 14,
  SocB7          = 15,
  SocC0          = 16,
  SocC1          = 17,
  SocC2          = 18,
  SocC3          = 19,
  SocC4          = 20,
  SocC5          = 21,
  SocC6          = 22,
  SocC7          = 23,
  SocD0          = 24,
  SocD1          = 25,
  SocD2          = 26,
  SocD3          = 27,
  SocD4          = 28,
  SocD5          = 29,
  SocD6          = 30,
  SocD7          = 31,
  SocSteppingMax
} SOC_STEPPING;

#define B_PCH_LPC_RID_STEPPING_MASK               0xFC  // SoC Stepping Mask (Ignoring Package Type)
#define V_PCH_LPC_RID_A0                          0x00  // A0 Stepping
#define V_PCH_LPC_RID_A1                          0x04  // A1 Stepping
#define V_PCH_LPC_RID_A2                          0x08  // A2 Stepping
#define V_PCH_LPC_RID_A3                          0x0C  // A3 Stepping
#define V_PCH_LPC_RID_A4                          0x80  // A4 Stepping
#define V_PCH_LPC_RID_A5                          0x84  // A5 Stepping
#define V_PCH_LPC_RID_A6                          0x88  // A6 Stepping
#define V_PCH_LPC_RID_A7                          0x8C  // A7 Stepping
#define V_PCH_LPC_RID_B0                          0x10  // B0 Stepping
#define V_PCH_LPC_RID_B1                          0x14  // B1 Stepping
#define V_PCH_LPC_RID_B2                          0x18  // B2 Stepping
#define V_PCH_LPC_RID_B3                          0x1C  // B3 Stepping
#define V_PCH_LPC_RID_B4                          0x90  // B4 Stepping
#define V_PCH_LPC_RID_B5                          0x94  // B5 Stepping
#define V_PCH_LPC_RID_B6                          0x98  // B6 Stepping
#define V_PCH_LPC_RID_B7                          0x9C  // B7 Stepping
#define V_PCH_LPC_RID_C0                          0x20  // C0 Stepping
#define V_PCH_LPC_RID_C1                          0x24  // C1 Stepping
#define V_PCH_LPC_RID_C2                          0x28  // C2 Stepping
#define V_PCH_LPC_RID_C3                          0x2C  // C3 Stepping
#define V_PCH_LPC_RID_C4                          0xA0  // C4 Stepping
#define V_PCH_LPC_RID_C5                          0xA4  // C5 Stepping
#define V_PCH_LPC_RID_C6                          0xA8  // C6 Stepping
#define V_PCH_LPC_RID_C7                          0xAC  // C7 Stepping
#define V_PCH_LPC_RID_D0                          0x30  // D0 Stepping
#define V_PCH_LPC_RID_D1                          0x34  // D1 Stepping
#define V_PCH_LPC_RID_D2                          0x38  // D2 Stepping
#define V_PCH_LPC_RID_D3                          0x3C  // D3 Stepping
#define V_PCH_LPC_RID_D4                          0xB0  // D4 Stepping
#define V_PCH_LPC_RID_D5                          0xB4  // D5 Stepping
#define V_PCH_LPC_RID_D6                          0xB8  // D6 Stepping
#define V_PCH_LPC_RID_D7                          0xBC  // D7 Stepping

int	SocStepping(void);

#endif /* _SOC_LPC_H_ */
