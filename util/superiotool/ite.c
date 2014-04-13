/*
 * This file is part of the superiotool project.
 *
 * Copyright (C) 2007 Carl-Daniel Hailfinger
 * Copyright (C) 2007-2008 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "superiotool.h"

#define CHIP_ID_BYTE1_REG	0x20
#define CHIP_ID_BYTE2_REG	0x21
#define CHIP_VERSION_REG	0x22
#define ISA_PNP_ADDR		0x279

static const struct superio_registers reg_table[] = {
	{0x8228, "IT8228E", {
		{EOT}}},
	{0x8500, "IT8500B/E", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x25,0x2d,0x2e,0x2f,0x30,EOT},
			{0x85,0x00,0x01,0x01,0x00,0x00,NANA,NANA,0x00,EOT}},
		{0x04, "System Wake-Up Control (SWUC)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x01,EOT}},
		{0x05, "KBC/Mouse Interface",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x0c,0x01,EOT}},
		{0x06, "KBC/Keyboard Interface",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x60,0x00,0x64,0x01,0x01,EOT}},
		{0x0f, "Shared Memory/Flash Interface (SMFI)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf4,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,NANA,EOT}},
		{0x10, "BRAM",
			{0x30,0x62,0x63,0x70,0x71,0xf3,0xf4,0xf5,EOT},
			{0x00,0x00,0x72,0x08,0x01,NANA,NANA,NANA,EOT}},
		{0x11, "Power Management I/F Channel 1 (PMC1)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x62,0x00,0x66,0x01,0x01,EOT}},
		{0x12, "Power Management I/F Channel 2 (PMC2)",
			{0x30,0x60,0x61,0x62,0x63,0x64,0x65,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x68,0x00,0x6c,0x00,0x00,0x01,0x01,NANA,EOT}},
		{EOT}}},
	{0x8502, "IT8502E/TE/G", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
			 0x2A,0x2B,0x2C,0x2D,0x2E,EOT},
			{0x85,0x02,0x71,0x01,NANA,0x00,0x00,NANA,NANA,NANA,
			 NANA,NANA,NANA,0x00,NANA,EOT}},
		{0x1, "UART1",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x00,0x03,0xf8,0x00,0x00,0x04,0x02,0x00,EOT}},
		{0x4, "System Wake-Up",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,EOT}},
		{0x5, "Mouse",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x0C,0x01,NANA,EOT}},
		{0x6, "Keyboard",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x01,NANA,EOT}},
		{0xf, "Shared Memory/Flash",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf4,0xf5,
			0xf6,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,EOT}},
		{0x10, "BRAM",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf3,0xf4,
			0xf5,EOT},
			{0x00,0x00,0x70,0x00,0x72,0x08,0x01,NANA,NANA,
			NANA,EOT}},
		{0x11, "Power Channel 1",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},

			{0x00,0x00,0x62,0x00,0x66,0x01,0x01,EOT}},
		{0x12, "Power Channel 2",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x68,0x00,0x6c,0x01,0x01,EOT}},
		{0x17, "Power Channel 3",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x6a,0x00,0x6e,0x01,0x01,EOT}},
		{EOT}}},
	{0x8510, "IT8510E/TE/G", {
		{NOLDN, "Chip ID",
			{0x20,0x21, EOT},
			{0x85,0x10, EOT}},
		{NOLDN, "Chip Version",
			{0x22,EOT},
			{0x21,EOT}},
		{NOLDN, "Super I/O Control Reigster (SIOCTRL)",
			{0x23,EOT},
			{0x01,EOT}},
		{NOLDN, "Super I/O Configuration Register (SIOIRQ)",
			{0x25,EOT},
			{0x00,EOT}},
		{NOLDN, "Super I/O General Purpose Register (SIOGP)",
			{0x26,EOT},
			{0x00,EOT}},
		{NOLDN, "Super I/O Power Mode Register (SIOPWR)",
			{0x2d,EOT},
			{0x00,EOT}},
		{NOLDN, "Logical Device Activate Register (LDA)",
			{0x30,EOT},
			{0x00,EOT}},
		{NOLDN, "I/O Port Base Address for Descriptor 0 (IOBAD0)",
			{0x60,0x61,EOT},
			{NANA,NANA,EOT}},
		{NOLDN, "I/O Port Base Address for Descriptor 1 (IOBAD1)",
			{0x62,0x63,EOT},
			{NANA,NANA,EOT}},
		{NOLDN, "Interupt Request Number and Wake-Up on IRQ Enable (IRQNUMX)",
			{0x70,EOT},
			{NANA,EOT}},
		{NOLDN, "Interrupt Request Type Select (IRQTP)",
			{0x71,EOT},
			{NANA,EOT}},
		{NOLDN, "DMA Channel Select 0 (DMAS0)",
			{0x74,EOT},
			{0x04,EOT}},
		{NOLDN, "DMA Channel Select 1 (DMAS1)",
			{0x75,EOT},
			{0x04,EOT}},
		{0x4, "System Wakup-Up (SWUC)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x03,EOT}},
		{0x5, "Keyboard/Mouse",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x0c,0x03,EOT}},
		{0x6, "Keyboard/Mouse",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x60,0x00,0x64,0x01,0x03,EOT}},
		{0xf, "Shared Memory/Flash Interface (SMFI)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x10, "Real Time Clock (RTC)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,0xf1,0xf2,EOT},
			{0x00,0x00,0x70,0x00,0x72,0x08,0x00,0x00,0x49,0x4a,EOT}},
		{0x11, "Power Management Interface Channel 1",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x62,0x00,0x66,0x01,0x03,EOT}},
		{0x12, "Power Management Interface Channel 2",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x68,0x00,0x6c,0x01,0x03,EOT}},
		{EOT}}},
	{0x8511, "IT8511E/TE/G", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
			 0x2A,0x2B,0x2C,0x2D,0x2E,EOT},
			{0x85,0x11,0x10,0x01,NANA,0x00,0x00,NANA,NANA,NANA,
			 NANA,NANA,NANA,0x00,NANA,EOT}},
		{0x4, "System Wake-Up",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,EOT}},
		{0x5, "Mouse",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x0C,0x03,NANA,EOT}},
		{0x6, "Keyboard",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x03,NANA,EOT}},
		{0xf, "Shared Memory/Flash",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf4,0xf5,
			0xf6,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,EOT}},
		{0x10, "Real-Time Clock",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,0xf1,
			0xf2,0xf3,0xf4,0xf5,EOT},
			{0x00,0x00,0x70,0x00,0x72,0x08,0x00,0x00,0x49,
			0x4A,NANA,NANA,NANA,EOT}},
		{0x11, "Power Channel 1",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x62,0x00,0x66,0x01,0x03,EOT}},
		{0x12, "Power Channel 2",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x68,0x00,0x6c,0x01,0x03,EOT}},
		{EOT}}},
	{0x8512, "IT8512E/F/G", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
			 0x2A,0x2B,0x2C,0x2D,0x2E,EOT},
			{0x85,0x12,0x22,0x01,NANA,0x00,0x00,NANA,NANA,NANA,
			 NANA,NANA,NANA,0x00,NANA,EOT}},
		{0x4, "System Wake-Up",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x03,EOT}},
		{0x5, "Mouse",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x0C,0x03,NANA,EOT}},
		{0x6, "Keyboard",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x60,0x00,0x64,0x01,0x03,NANA,EOT}},
		{0xf, "Shared Memory/Flash",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf4,0xf5,
			0xf6,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,EOT}},
		{0x10, "BRAM",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf3,0xf4,
			0xf5,EOT},
			{0x00,0x00,0x70,0x00,0x72,0x08,0x00,NANA,NANA,
			NANA,EOT}},
		{0x11, "Power Channel 1",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x62,0x00,0x66,0x01,0x03,EOT}},
		{0x12, "Power Channel 2",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,EOT},
			{0x00,0x00,0x68,0x00,0x6c,0x01,0x03,EOT}},
		{EOT}}},
	{0x8513, "IT8513E/F/G", {
		{EOT}}},
	{0x8661, "IT8661F/IT8770F", {
		{NOLDN, NULL,
			{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x20,0x21,0x22,
			 0x23,0x24,EOT},
			{NANA,NANA,NANA,NANA,NANA,NANA,0x00,0x86,0x61,0x00,
			 0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x31,0x60,0x61,0x70,0x71,0x74,0xf0,EOT},
			{0x00,0x00,0x03,0xf0,0x06,0x02,0x02,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x31,0x60,0x61,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x03,0xf8,0x04,0x02,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x31,0x60,0x61,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x02,0xf8,0x03,0x02,0x00,EOT}},
		{0x3, "Parallel port",
			{0x30,0x31,0x60,0x61,0x62,0x63,0x70,0x71,0x74,
			 0xf0,EOT},
			{0x00,0x00,0x03,0x78,0x07,0x78,0x07,0x02,0x03,
			 0x03,EOT}},
		{0x4, "IR",
			{0x30,0x31,0x60,0x61,0x62,0x63,0x70,0x71,0x72,0x73,
			 0x74,0x75,0xf0,EOT},
			{0x00,0x00,0x02,0xe8,0x03,0x00,0x0a,0x02,0x0b,0x02,
			 0x01,0x00,0x00,EOT}},
		{0x5, "GPIO",
			{0x25,0x26,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
			 0x70,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
			 0xf9,0xfa,0xfb,0xfc,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,EOT}},
		{EOT}}},
	{0x8673, "IT8673F", {
		{EOT}}},
	{0x8681, "IT8671F/IT8687R", {
		{NOLDN, NULL,
			{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x20,0x21,0x22,
			 0x23,0x24,0x25,0x26,0x2e,0x2f,EOT},
			{NANA,NANA,NANA,NANA,NANA,NANA,0x00,0x86,0x81,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x31,0x60,0x61,0x70,0x71,0x74,0xf0,EOT},
			{0x00,0x00,0x03,0xf0,0x06,0x02,0x02,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x31,0x60,0x61,0x70,0x71,0xf0,EOT},
			{0x00,0x00,0x03,0xf8,0x04,0x02,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x31,0x60,0x61,0x62,0x63,0x70,0x71,0x72,0x73,
			 0x74,0x75,0xf0,0xf1,EOT},
			{0x00,0x00,0x02,0xf8,0x03,0x00,0x03,0x02,0x0a,0x02,
			 0x00,0x01,0x00,0x00,EOT}},
		{0x3, "Parallel port",
			{0x30,0x31,0x60,0x61,0x62,0x63,0x70,0x71,0x74,
			 0xf0,EOT},
			{0x00,0x00,0x03,0x78,0x07,0x78,0x07,0x02,0x03,
			 0x03,EOT}},
		{0x4, "APC",
			{0x30,0xf0,0xf1,0xf2,0xf4,0xf5,0xf6,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x5, "Keyboard",
			{0x30,0x31,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{MISC,0x00,0x00,0x60,0x00,0x64,0x01,0x02,0x00,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO",
			{0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
			 0x70,0x71,0x72,0x73,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,
			 0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
			 0xe0,0xe1,0xe2,0xe3,0xe4,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,EOT}},
		{EOT}}},
	{0x8701, "IT8703F", {
		{NOLDN, NULL,
			{0x20,0x21,0x23,0x24,0x26,0x29,0x2a,0x2b,EOT},
			{0x87,0x00,0x00,0x80,0x00,0x00,0x7c,0xc0,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,0xf2,0xf3,0xf4,
			 0xf5,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x0e,0x00,0xff,0x00,0x00,
			 0x00,EOT}},
		{0x1, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x74,0xf0,EOT},
			{0x00,0x03,0x78,0x00,0x80,0x07,0x03,0x03,EOT}},
		{0x2, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0xf8,0x04,0x00,EOT}},
		{0x3, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x00,0x00,0x7f,EOT}},
		{0x5, "Keyboard",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x0c,0x80,EOT}},
		{0x6, "Consumer IR",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x7, "Game port, MIDI, GPIO set 1",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,EOT},
			{0x00,0x02,0x01,0x03,0x30,0x00,0xff,0x00,0x00,EOT}},
		{0x8, "GPIO set 2",
			{0x30,0xf0,0xf1,0xf2,0xf3,0xf5,EOT},
			{0x00,0xff,0x00,0x00,0x00,0x00,EOT}},
		{0x9, "GPIO set 3 and 4",
			{0x30,0x60,0x61,0xf0,0xf1,0xf2,0xf3,0xf4,EOT},
			{0x00,0x02,0x90,0xff,0x00,0x00,0x00,0x00,EOT}},
		{0xa, "ACPI",
			{0x30,0x70,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xf3,0xf4,0xf6,0xf7,0xf9,EOT},
			{0x00,0x00,0x00,0x00,NANA,NANA,0x00,0x00,0x00,0x00,
			 NANA,NANA,0x00,0x00,0x00,EOT}},
		{0xc, "GPIO set 5, 6 and 7",
			{0x30,0x60,0x61,0xf0,0xf3,0xf6,EOT},
			{0x00,0x03,0x70,0x00,0xff,0xff,EOT}},
		{EOT}}},
	{0x8702, "IT8702F", {
		{EOT}}},
	{0x8705, "IT8705F/AF / IT8700F", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,EOT},
			{0x87,0x05,0x00,0x00,NANA,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,0x00,0x7f,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x64,0x65,0x70,0x74,
			 0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x00,0x80,0x07,0x03,
			 0x03,EOT}},
		{0x4, "Environment controller",
			{0x2b,0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,
			 0xf3,0xf4,0xf5,0xf6,EOT},
			{0x00,0x00,0x02,0x90,0x02,0x30,0x09,0x00,0x00,0x00,
			 0x00,0x00,NANA,NANA,EOT}},
		{0x5, "GPIO",
			{0x25,0x26,0x27,0x28,0x29,0x2a,0x60,0x61,0x62,0x63,
			 0x64,0x65,0x70,0x71,0x72,0xb0,0xb1,0xb2,0xb3,0xb4,
			 0xb5,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xc0,0xc1,0xc2,
			 0xc3,0xc4,0xc5,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xd0,
			 0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd8,0xd9,0xda,0xf0,
			 0xf1,0xf2,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,
			 0xfd,0xfe,0xff,EOT},
			{0x00,0x00,0x00,0xff,0xe0,0xff,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,NANA,0x00,EOT}},
		{0x6, "Game port",
			{0x30,0x60,0x61,EOT},
			{0x00,0x02,0x01,EOT}},
		{0x7, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x00,EOT}},
		{0x8, "MIDI port",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x00,0x0a,0x00,EOT}},
		{EOT}}},
	{0x8706, "IT8706R", {	/* TODO: Not yet in sensors-detect */
		/* This is a "Special General Purpose I/O chip". */
		{EOT}}},
	{0x8707, "IT8707F", {
		{EOT}}},
	{0x8708, "IT8708F", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
			 0x2a,0x2e,0x2f,EOT},
			{0x87,0x08,0x00,0x00,NANA,0x3f,0x00,0xff,0xff,0xff,
			 0xff,0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0xf8,0x04,0x00,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,0x00,0x7f,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x64,0x65,0x70,0x74,
			 0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x00,0x80,0x07,0x03,
			 0x03,EOT}},
		{0x4, "SWC",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,EOT},
			{NANA,NANA,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,NANA,NANA,EOT}},
		{0x5, "Keyboard",
			/* Note: 0x30 can actually be 0x00 _or_ 0x01. */
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x02,0x00,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO",
			{0x70,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb8,0xb9,0xba,
			 0xbb,0xbc,0xbd,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc8,
			 0xc9,0xca,0xcb,0xcc,0xcd,0xd0,0xd1,0xd2,0xd3,0xd4,
			 0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xf0,0xf1,
			 0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,
			 0xfc,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,NANA,NANA,NANA,NANA,NANA,NANA,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,NANA,
			 0x00,EOT}},
		{0x8, "Game port",
			{0x30,0x60,0x61,EOT},
			{0x00,0x02,0x01,EOT}},
		{0x9, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x00,EOT}},
		{0xa, "MIDI port",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x00,0x0a,0x00,EOT}},
		{EOT}}},
	{0x8710, "IT8710F", {	/* TODO: Not yet in sensors-detect */
		{EOT}}},
	{0x8711, "IT8711F", {	/* 0x8711 is a guess, not found in datasheet. */
		{EOT}}},
	{0x8712, "IT8712F", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x2b,EOT},
			{0x87,0x12,0x08,0x00,0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x03,0xf8,0x04,0x00,0x50,0x00,0x7f,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,0x00,0x7f,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x74,0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x07,0x03,0x03,EOT}},
		{0x4, "Environment controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,0xf3,
			 0xf4,0xf5,0xf6,EOT},
			{0x00,0x02,0x90,0x02,0x30,0x09,0x00,0x00,0x00,0x00,
			 0x00,NANA,NANA,EOT}},
		{0x5, "Keyboard",
			/* TODO: 0xf0: Error in datasheet? */
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x02,0x08,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO", /* TODO: 0x72, 0x73: Errors in datasheet? */
			{0x25,0x26,0x27,0x28,0x29,0x2a,0x2c,0x60,0x61,0x62,
			 0x63,0x64,0x65,0x70,0x71,0x72,0x73,0x74,0xb0,0xb1,
			 0xb2,0xb3,0xb4,0xb5,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,
			 0xc0,0xc1,0xc2,0xc3,0xc4,0xc8,0xc9,0xca,0xcb,0xcc,
			 0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,0xf1,0xf2,0xf3,0xf4,
			 0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,EOT},
			{0x01,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x01,0x00,0x00,0x40,0x00,0x01,0x00,0x00,0x40,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,NANA,0x00,EOT}},
		{0x8, "MIDI port",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x00,0x0a,0x00,EOT}},
		{0x9, "Game port",
			{0x30,0x60,0x61,EOT},
			{0x00,0x02,0x01,EOT}},
		{0xa, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x00,EOT}},
		{EOT}}},
	{0x8716, "IT8716F", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x2b,EOT},
			{0x87,0x16,0x01,0x00,0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x03,0xf8,0x04,0x00,0x50,0x00,0x7f,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,0x00,0x7f,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x74,0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x07,0x03,0x03,EOT}},
		{0x4, "Environment controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,0xf3,
			 0xf4,0xf5,0xf6,EOT},
			{0x00,0x02,0x90,0x02,0x30,0x09,0x00,0x00,0x00,0x00,
			 0x00,NANA,NANA,EOT}},
		{0x5, "Keyboard",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x02,0x00,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO",
			{0x25,0x26,0x27,0x28,0x29,0x2a,0x2c,0x60,0x61,0x62,
			 0x63,0x64,0x65,0x70,0x71,0x72,0x73,0x74,0xb0,0xb1,
			 0xb2,0xb3,0xb4,0xb5,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,
			 0xc0,0xc1,0xc2,0xc3,0xc4,0xc8,0xc9,0xca,0xcb,0xcc,
			 0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,0xf1,0xf2,0xf3,0xf4,
			 0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,EOT},
			{0x01,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x20,0x38,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x01,0x00,0x00,0x40,0x00,0x01,0x00,0x00,0x40,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,NANA,0x00,EOT}},
		{0x8, "MIDI port",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x00,0x0a,0x00,EOT}},
		{0x9, "Game port",
			{0x30,0x60,0x61,EOT},
			{0x00,0x02,0x01,EOT}},
		{0xa, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x00,EOT}},
		{EOT}}},
	{0x8718, "IT8718F", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x2b,EOT},
			{0x87,0x18,0x01,0x00,0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x03,0xf8,0x04,0x00,0x50,0x00,0x7f,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,0x00,0x7f,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x74,0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x07,0x03,0x03,EOT}},
		{0x4, "Environment controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,0xf3,
			 0xf4,0xf5,0xf6,EOT},
			{0x00,0x02,0x90,0x02,0x30,0x09,0x00,0x00,0x00,0x00,
			 0x00,NANA,NANA,EOT}},
		{0x5, "Keyboard",
			/* 0xf0: Datasheet page 33: 0x00; page 56: 0x08. */
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x02,0x00,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO",
			/* 0x2c: Datasheet page 30: 0x00; page 43: 0x1f. */
			{0x25,0x26,0x27,0x28,0x29,0x2a,0x2c,0x60,0x61,0x62,
			 0x63,0x64,0x65,0x70,0x71,0x72,0x73,0x74,0xb0,0xb1,
			 0xb2,0xb3,0xb4,0xb5,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,
			 0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc8,0xc9,0xca,0xcb,
			 0xcc,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,
			 0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0x01,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x20,0x38,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x01,0x00,0x00,0x40,0x00,0x00,0x01,0x00,0x00,0x40,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,NANA,0x00,0x00,0x00,EOT}},
		{0xa, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x00,EOT}},
		{EOT}}},
	{0x8720, "IT8720F", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x2b,EOT},
			{0x87,0x20,0x05,0x00,0x00,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x00,0x03,0xf8,0x04,0x00,0x50,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x74,0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x07,0x03,0x03,EOT}},
		{0x4, "Environment controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,0xf3,
			 0xf4,0xf5,0xf6,EOT},
			{0x00,0x02,0x90,0x02,0x30,0x09,0x00,0x00,0x00,0x00,
			 0x00,NANA,NANA,EOT}},
		{0x5, "Keyboard",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x02,0x48,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO",
			{0x25,0x26,0x27,0x28,0x29,0x2a,0x2c,0x60,0x61,0x62,
			 0x63,0x64,0x65,0x70,0x71,0x72,0x73,0x74,0xb0,0xb1,
			 0xb2,0xb3,0xb4,0xb5,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,
			 0xc0,0xc1,0xc2,0xc3,0xc4,0xc8,0xc9,0xca,0xcb,0xcc,
			 0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,
			 0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0x00,0xff,0x00,0x40,0x00,0x00,0x03,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,NANA,0x38,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,
			 0x01,0x00,0x00,0x40,0x00,0x01,0x00,0x00,0x40,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,NANA,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,NANA,0x00,0x00,0x00,EOT}},
		{0xa, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x06,EOT}},
		{EOT}}},
	{0x8721, "IT8721F", {
		{EOT}}},
	{0x8722, "IT8722F", {
		{EOT}}},
	{0x8726, "IT8726F", {
		/* Datasheet wrongly says that the ID is 0x8716. */
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x2b,EOT},
			{0x87,0x26,0x01,0x00,MISC,0x00,EOT}},
		{0x0, "Floppy",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,EOT},
			{0x00,0x03,0xf0,0x06,0x02,0x00,0x00,EOT}},
		{0x1, "COM1",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x03,0xf8,0x04,0x00,0x50,0x00,0x7f,EOT}},
		{0x2, "COM2",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x02,0xf8,0x03,0x00,0x50,0x00,0x7f,EOT}},
		{0x3, "Parallel port",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x74,0xf0,EOT},
			{0x00,0x03,0x78,0x07,0x78,0x07,0x03,0x03,EOT}},
		{0x4, "Environment controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,0xf3,
			 0xf4,0xf5,0xf6,EOT},
			{0x00,0x02,0x90,0x02,0x30,0x09,0x00,0x00,0x00,0x00,
			 0x00,MISC,MISC,EOT}},
		{0x5, "Keyboard",
			/* 0xf0: Datasheet page 35: 0x00; page 59: 0x08. */
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0xf0,EOT},
			{0x01,0x00,0x60,0x00,0x64,0x01,0x02,0x08,EOT}},
		{0x6, "Mouse",
			{0x30,0x70,0x71,0xf0,EOT},
			{0x00,0x0c,0x02,0x00,EOT}},
		{0x7, "GPIO",
			/* 0x2c: Datasheet page 33: 0x00; page 45: 0x1f. */
			{0x25,0x26,0x27,0x28,0x29,0x2a,0x2c,0x60,0x61,0x62,
			 0x63,0x64,0x65,0x70,0x71,0x72,0x73,0x74,0xb0,0xb1,
			 0xb2,0xb3,0xb4,0xb5,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,
			 0xc0,0xc1,0xc2,0xc3,0xc4,0xc8,0xc9,0xca,0xcb,0xcc,
			 0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,0xf1,0xf2,0xf3,0xf4,
			 0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,EOT},
			{0x01,0x00,0x00,0x40,0x00,0x00,0x1f,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,MISC,0x38,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x01,0x00,0x00,0x40,0x00,0x01,0x00,0x00,0x40,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,MISC,0x00,EOT}},
		{0x8, "MIDI port",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x00,0x0a,0x00,EOT}},
		{0x9, "Game port",
			{0x30,0x60,0x61,EOT},
			{0x00,0x02,0x01,EOT}},
		{0xa, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x00,0x03,0x10,0x0b,0x00,EOT}},
		{EOT}}},
	{0x8761, "IT8761E", {
		{EOT}}},
	{0x8772, "IT8772F", {
		{EOT}}},
	{0x8780, "IT8780F", {
		{EOT}}},
	{EOT}
};

static const struct superio_registers ec_table[] = {
	{0x8716, "IT8716F", {
		{NOLDN, NULL,
			{0x00,0x04,0x05,0x06,0x07,0x08,0x09,0x0b,0x0c,0x10,
			 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x1b,0x1c,0x1d,
			 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
			 0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,
			 0x44,0x45,0x48,0x50,0x51,0x52,0x53,0x54,0x56,0x57,
			 0x59,0x5c,
			 0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x68,
			 0x69,0x6a,0x6b,0x6c,0x6d,0x70,0x71,0x72,0x73,0x74,
			 0x75,0x84,0x85,0x86,0x87,0x88,0x89,0x8c,0x8d,0x8e,
			 0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x98,0x99,0x9a,
			 0x9b,0x9c,0x9d,EOT},
			{0x18,0x00,0x00,0x00,0x00,0x00,0x80,0x09,0x00,NANA,
			 NANA,NANA,0x07,0x50,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,RSVD,0x00,0x00,0x7f,0x7f,0x7f,0x00,0x00,
			 0x00,0x00,
			 0x00,0x00,0x00,0x7f,0x7f,0x7f,0x00,0x00,0x7f,0x7f,
			 0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x7f,0x00,0x00,
			 0x7f,NANA,NANA,NANA,NANA,0x00,0x00,0x02,0x00,0x99,
			 0x99,0x7f,0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x7f,
			 0x00,0x00,0x7f,EOT}},
		{EOT}}},
	{0x8718, "IT8718F", {
		{NOLDN, NULL,
			{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
			 0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,
			 0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,
			 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
			 0x2a,0x2b,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
			 0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,
			 0x42,0x43,0x44,0x45,0x50,0x51,0x52,0x53,0x54,0x56,
			 0x57,0x58,0x59,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,
			 0x62,0x63,0x64,0x65,0x68,0x69,0x6a,0x6b,0x6c,0x6d,
			 0x70,0x71,0x72,0x73,0x74,0x75,0x80,0x81,0x82,0x83,
			 0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,
			 0x92,0x94,0x95,0x96,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,
			 0xa6,EOT},
			{0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
			 0x40,0x09,0x00,NANA,NANA,NANA,NANA,NANA,NANA,0x07,
			 0x50,MISC,MISC,MISC,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,NANA,
			 NANA,NANA,NANA,NANA,0x00,0x00,0x7f,0x7f,0x7f,0x00,
			 0x00,0x90,0x00,0x12,0x00,0x00,0x00,0x00,0x7f,0x7f,
			 0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x7f,0x00,0x00,0x7f,
			 0x7f,0x7f,0x7f,0x00,0x00,0x7f,NANA,NANA,NANA,NANA,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0xff,0x00,
			 0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,EOT}},
		{EOT}}},
	{EOT}
};

/* Works for: IT8661F/IT8770F */
static const uint8_t initkey_it8661f[][4] = {
	{0x86, 0x61, 0x55, 0x55},	/* 0x3f0 */
	{0x86, 0x61, 0x55, 0xaa},	/* 0x3bd */
	{0x86, 0x61, 0xaa, 0x55},	/* 0x370 */
};

/* Works for: IT8671F/IT8687R, IT8673F */
static const uint8_t initkey_it8671f[][4] = {
	{0x86, 0x80, 0x55, 0x55},	/* 0x3f0 */
	{0x86, 0x80, 0x55, 0xaa},	/* 0x3bd */
	{0x86, 0x80, 0xaa, 0x55},	/* 0x370 */
};

/* Works for: IT8661F/IT8770F, IT8671F/IT8687R, IT8673F. */
static const uint8_t initkey_mbpnp[] = {
	0x6a, 0xb5, 0xda, 0xed, 0xf6, 0xfb, 0x7d, 0xbe, 0xdf, 0x6f, 0x37,
	0x1b, 0x0d, 0x86, 0xc3, 0x61, 0xb0, 0x58, 0x2c, 0x16, 0x8b, 0x45,
	0xa2, 0xd1, 0xe8, 0x74, 0x3a, 0x9d, 0xce, 0xe7, 0x73, 0x39,
};

/* Works for: IT8661F/IT8770F, IT8671F/IT8687R, IT8673F. */
static void enter_conf_mode_ite_legacy(uint16_t port, const uint8_t init[][4])
{
	int i, idx;

	/* Determine Super I/O config port. */
	idx = (port == 0x3f0) ? 0 : ((port == 0x3bd) ? 1 : 2);
	for (i = 0; i < 4; i++)
		OUTB(init[idx][i], ISA_PNP_ADDR);

	/* Sequentially write the 32 MB PnP init values. */
	for (i = 0; i < 32; i++)
		OUTB(initkey_mbpnp[i], port);
}

static void enter_conf_mode_ite(uint16_t port)
{
	OUTB(0x87, port);
	OUTB(0x01, port);
	OUTB(0x55, port);
	OUTB((port == 0x2e) ? 0x55 : 0xaa, port);
}

static void enter_conf_mode_ite_it8502e(uint16_t port)
{
	OUTB(0x85, port);
	OUTB(0x02, port);
	OUTB(0x55, port);
	OUTB((port == 0x2e) ? 0x55 : 0xaa, port);
}

static void enter_conf_mode_ite_it8761e(uint16_t port)
{
	OUTB(0x87, port);
	OUTB(0x61, port);
	OUTB(0x55, port);
	OUTB((port == 0x2e) ? 0x55 : 0xaa, port);
}

static void enter_conf_mode_ite_it8228e(uint16_t port)
{
	OUTB(0x82, port);
	OUTB(0x28, port);
	OUTB(0x55, port);
	OUTB((port == 0x2e) ? 0x55 : 0xaa, port);
}

static void exit_conf_mode_ite(uint16_t port)
{
	regwrite(port, 0x02, 0x02);
}

static int chip_found_at_port;

static void probe_idregs_ite_helper(const char *init, uint16_t port)
{
	uint16_t id, chipver, ecport;

	probing_for("ITE", init, port);

	id = regval(port, CHIP_ID_BYTE1_REG) << 8;
	id |= regval(port, CHIP_ID_BYTE2_REG);
	chipver = regval(port, CHIP_VERSION_REG) & 0x0f; /* Only bits 3..0 */

	if (superio_unknown(reg_table, id)) {
		if (verbose)
			printf(NOTFOUND "id=0x%04x, rev=0x%01x\n", id, chipver);
		return;
	}

	printf("Found ITE %s (id=0x%04x, rev=0x%01x) at 0x%x\n",
	       get_superio_name(reg_table, id), id, chipver, port);
	chip_found = 1;
	chip_found_at_port = 1;

	dump_superio("ITE", reg_table, port, id, LDN_SEL);

	if (extra_dump) {
		regwrite(port, LDN_SEL, 0x04); /* Select LDN 4 (EC). */

		/* Get EC base address (stored in LDN 4, index 0x60/0x61). */
		ecport = regval(port, 0x60) << 8;
		ecport |= regval(port, 0x61);

		/* EC address register = EC base address + 5. */
		ecport += 5;

		printf("Environment controller (0x%04x)\n", ecport);
		dump_superio("ITE-EC", ec_table, ecport, id, LDN_SEL);
	}
}

void probe_idregs_ite(uint16_t port)
{
	chip_found_at_port = 0;

	if (port == 0x3f0 || port == 0x3bd || port == 0x370) {
		enter_conf_mode_ite_legacy(port, initkey_it8661f);
		probe_idregs_ite_helper("(init=legacy/it8661f) ", port);
		exit_conf_mode_ite(port);
		if (chip_found_at_port) return;

		enter_conf_mode_ite_legacy(port, initkey_it8671f);
		probe_idregs_ite_helper("(init=legacy/it8671f) ", port);
		exit_conf_mode_ite(port);
		if (chip_found_at_port) return;
	} else {
		enter_conf_mode_ite(port);
		probe_idregs_ite_helper("(init=standard) ", port);
		exit_conf_mode_ite(port);
		if (chip_found_at_port) return;

		enter_conf_mode_ite_it8502e(port);
		probe_idregs_ite_helper("(init=it8502e) ", port);
		exit_conf_mode_ite(port);
		if (chip_found_at_port) return;

		enter_conf_mode_ite_it8761e(port);
		probe_idregs_ite_helper("(init=it8761e) ", port);
		exit_conf_mode_ite(port);
		if (chip_found_at_port) return;

		enter_conf_mode_ite_it8228e(port);
		probe_idregs_ite_helper("(init=it8228e) ", port);
		exit_conf_mode_ite(port);
		if (chip_found_at_port) return;

		enter_conf_mode_winbond_fintek_ite_8787(port);
		probe_idregs_ite_helper("(init=0x87,0x87) ", port);
		exit_conf_mode_winbond_fintek_ite_8787(port);
		if (chip_found_at_port) return;
	}
}

void print_ite_chips(void)
{
	print_vendor_chips("ITE", reg_table);
	print_vendor_chips("ITE-EC", ec_table);
}
