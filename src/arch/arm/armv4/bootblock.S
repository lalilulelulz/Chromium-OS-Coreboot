/*
 * Early initialization code for ARM architecture.
 *
 * This file is based off of the OMAP3530/ARM Cortex start.S file from Das
 * U-Boot, which itself got the file from armboot.
 *
 * Copyright (c) 2004	Texas Instruments <r-woodruff2@ti.com>
 * Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 * Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 * Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 * Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 * Copyright (c) 2003	Kshitij <kshitij@ti.com>
 * Copyright (c) 2006-2008 Syed Mohammed Khasim <x0khasim@ti.com>
 * Copyright (c) 2013   The Chromium OS Authors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

.section ".start", "a", %progbits
.globl _start
_start: b	reset
	.balignl 16,0xdeadbeef

_cbfs_master_header:
	/* The CBFS master header is inserted by cbfstool at the first
	 * aligned offset after the above anchor string is found.
	 * Hence, we leave some space for it.
	 */
	.skip 128			@ Assumes 64-byte alignment

reset:
	/*
	 * Set the cpu to System mode with IRQ and FIQ disabled. Prefetch/Data
	 * aborts may happen early and crash before the abort handlers are
	 * installed, but at least the problem will show up near the code that
	 * causes it.
	 */
	msr	cpsr_cxf, #0xdf

	/*
	 * Initialize the stack to a known value. This is used to check for
	 * stack overflow later in the boot process.
	 */
	ldr	r0, .Stack
	ldr	r1, .Stack_size
	sub	r0, r0, r1
	ldr	r1, .Stack
	ldr	r2, =0xdeadbeef
init_stack_loop:
	str	r2, [r0]
	add	r0, #4
	cmp	r0, r1
	bne	init_stack_loop

/* Set stackpointer in internal RAM to call bootblock main() */
call_bootblock:
	ldr	sp, .Stack /* Set up stack pointer */
	ldr	r0,=0x00000000
	 /*
	  * The current design of cpu_info places the
	  * struct at the top of the stack. The number of
	  * words pushed must be at least as large as that
	  * struct.
	  */
	push	{r0-r2}
	bic	sp, sp, #7 /* 8-byte alignment for ABI compliance */
	/*
	 * Use "bl" instead of "b" even though we do not intend to return.
	 * "bl" gets compiled to "blx" if we're transitioning from ARM to
	 * Thumb. However, "b" will not and GCC may attempt to create a
	 * wrapper which is currently broken.
	 */
	bl	main

/* we do it this way because it's a 32-bit constant and
 * in some cases too far away to be loaded as just an offset
 * from IP
 */
.align 2
.Stack:
	.word CONFIG_STACK_TOP
.align 2
/* create this size the same way we do in coreboot_ram.ld: top-bottom */
.Stack_size:
	.word CONFIG_STACK_TOP - CONFIG_STACK_BOTTOM
