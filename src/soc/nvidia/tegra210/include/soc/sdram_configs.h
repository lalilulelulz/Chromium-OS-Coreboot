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

#ifndef __SOC_NVIDIA_TEGRA210_SDRAM_CONFIGS_H__
#define __SOC_NVIDIA_TEGRA210_SDRAM_CONFIGS_H__

#include <soc/sdram.h>

/* Loads SDRAM configurations for current system. */
const struct sdram_params *get_sdram_config(void);
/* Returns SDRAM params for current system, R/W so BCT can be updated if needed */
struct sdram_params *get_sdram_params(void);

#endif  /* __SOC_NVIDIA_TEGRA210_SDRAM_CONFIGS_H__ */
