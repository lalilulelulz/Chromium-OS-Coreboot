/*
 * This file is part of the coreboot project.
 *
 * Copyright 2014 Google Inc.
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __SOC_NVIDIA_TEGRA132_CPU_H__
#define __SOC_NVIDIA_TEGRA132_CPU_H__

/*
 * Start a core in 64-bit mode at the entry_64 address. Note that entry_64
 * should be a 32-bit address.
 */
void start_cpu(int cpu, void *entry_64);

void reset_entry_32bit(void);

#endif /* __SOC_NVIDIA_TEGRA132_CPU_H__ */

