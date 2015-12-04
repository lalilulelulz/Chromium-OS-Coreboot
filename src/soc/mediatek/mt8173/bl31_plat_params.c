/*
 * This file is part of the coreboot project.
 *
 * Copyright 2015 MediaTek Inc.
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
#include <arm_tf.h>
#include <assert.h>
#include <soc/bl31_plat_params.h>

static struct bl31_plat_param *plat_params;

void register_bl31_param(struct bl31_plat_param *param)
{
	param->next = (void *)plat_params;
	plat_params = param;
}

void *soc_get_bl31_plat_params(bl31_params_t *bl31_params)
{
	return (void *)plat_params;
}
