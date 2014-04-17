/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011-2012 The ChromiumOS Authors.  All rights reserved.
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

#include <string.h>
#include <vendorcode/google/chromeos/chromeos.h>
#include <arch/io.h>
#ifdef __PRE_RAM__
#include <arch/romcc_io.h>
#else
#include <device/device.h>
#include <device/pci.h>
#endif

#include <southbridge/intel/bd82x6x/pch.h>
#include "ec.h"

#ifndef __PRE_RAM__
#include <boot/coreboot_tables.h>
#include <arch/coreboot_tables.h>
#include <ec/quanta/it8518/ec.h>

#define GPIO_COUNT	6
#define ACTIVE_LOW	0
#define ACTIVE_HIGH	1

void fill_lb_gpios(struct lb_gpios *gpios)
{
	gpios->size = sizeof(*gpios) + (GPIO_COUNT * sizeof(struct lb_gpio));
	gpios->count = GPIO_COUNT;

	/*
	 * TODO(kimarie) - Finish recovery mode/developer mode support.
	 *
	 * Write protect, recovery, developer mode, lid switch and power button are
	 * all virtualized by EC.  Hard-code read/write mode, and power button not
	 * asserted.
	 */
	/* Write Protect: Virtual switch */
	gpios->gpios[0].port = -1;
	gpios->gpios[0].polarity = ACTIVE_HIGH;
	gpios->gpios[0].value = 0; /* Hard-code to read/write mode */
	strncpy((char *)gpios->gpios[0].name,"write protect",
							GPIO_MAX_NAME_LENGTH);

	/* Recovery: Virtual switch */
	gpios->gpios[1].port = -1;
	gpios->gpios[1].polarity = ACTIVE_HIGH;
	gpios->gpios[1].value = get_recovery_mode_switch();
	strncpy((char *)gpios->gpios[1].name,"recovery", GPIO_MAX_NAME_LENGTH);

	/* Developer: Virtual switch */
	gpios->gpios[2].port = -1;
	gpios->gpios[2].polarity = ACTIVE_HIGH;
	gpios->gpios[2].value = get_developer_mode_switch();
	strncpy((char *)gpios->gpios[2].name,"developer", GPIO_MAX_NAME_LENGTH);

	/* Lid Switch: Virtual switch */
	gpios->gpios[3].port = -1;
	gpios->gpios[3].polarity = ACTIVE_HIGH;
	gpios->gpios[3].value = 1; /* Hard-code to open */
	strncpy((char *)gpios->gpios[3].name,"lid", GPIO_MAX_NAME_LENGTH);

	/* Power Button: Virtual switch */
	gpios->gpios[4].port = -1;
	gpios->gpios[4].polarity = ACTIVE_HIGH;
	gpios->gpios[4].value = 0; /* Hard-code to de-asserted */
	strncpy((char *)gpios->gpios[4].name,"power", GPIO_MAX_NAME_LENGTH);

	/* Was VGA Option ROM loaded? */
	gpios->gpios[5].port = -1; /* Indicate that this is a pseudo GPIO */
	gpios->gpios[5].polarity = ACTIVE_HIGH;
	gpios->gpios[5].value = oprom_is_loaded;
	strncpy((char *)gpios->gpios[5].name,"oprom", GPIO_MAX_NAME_LENGTH);
}
#endif

/* The dev-switch is virtual on Stout (and so handled elsewhere). */
int get_developer_mode_switch(void)
{
	return 0;
}

/* The recovery-switch is virtual on Stout and is handled via the EC */
int get_recovery_mode_switch(void)
{
	int rec_mode = 0;

#ifndef __PRE_RAM__
	rec_mode |= ((ec_read(EC_STATUS_REG) & 3) == EC_IN_RECOVERY_MODE);
#endif

	return rec_mode;
}
