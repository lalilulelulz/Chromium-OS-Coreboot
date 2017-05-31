/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 The Chromium OS Authors.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef CPU_FSP_MODEL406DX_CHIP_H
#define CPU_FSP_MODEL406DX_CHIP_H

/* Magic value used to locate this chip in the device tree */
#define SPEEDSTEP_APIC_MAGIC 0xACAC

struct cpu_intel_fsp_model_406dx_config {
	int c1_battery;		/* ACPI C1 on Battery Power */
	int c2_battery;		/* ACPI C2 on Battery Power */
	int c3_battery;		/* ACPI C3 on Battery Power */

	int c1_acpower;		/* ACPI C1 on AC Power */
	int c2_acpower;		/* ACPI C2 on AC Power */
	int c3_acpower;		/* ACPI C3 on AC Power */
};

#endif /* CPU_FSP_MODEL406DX_CHIP_H */