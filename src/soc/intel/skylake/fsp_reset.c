/*
 * This file is part of the coreboot project.
 *
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
#include <bootstate.h>
#include <vendorcode/google/chromeos/vboot_handoff.h>

static int is_recovery; /* flag to identify recovery mode */

/*
 * CB used to clear recovery status towards romstage end after FSP
 * memory init. Later inside FSP silicon init due to HSIO CRC mismatch
 * or other silicon related programming may request for an additional
 * reset. Thus on next boot system resume in normal mode rather than
 * recovery because lost its original state due to FSP silicon init reset.
 * Hence it needs an addition reset to get into old state and continue
 * booting into recovery mode. This function will set recovery reason
 * during Silicon init, in case of recovery mode booting,
 * so, system will not loose its original context.
 */
static void set_recovery_request(void *unused)
{
	is_recovery = recovery_mode_enabled();
	/*
	* Set recovery flag during Recovery Mode Silicon Init
	* & store recovery request into VBNV
	*/
	if (is_recovery)
		set_recovery_mode_into_vbnv(vboot_recovery_reason());

}

static void clear_recovery_request(void *unused)
{
	/*
	 * Done with Silicon Init, its safe to clear
	 * reset request now with assumption that no reset hereafter
	 * so we will not miss original data.
	 */
	if (is_recovery)
		set_recovery_mode_into_vbnv(0);
}
/*
 * On Recovery Path Set Recovery Request during early RAMSTAGE
 * before initiated Silicon Init
 */
BOOT_STATE_INIT_ENTRY(BS_PRE_DEVICE, BS_ON_ENTRY, set_recovery_request, NULL);
/*
 * On Recovery Path Clear Recovery Request during early RAMSTAGE
 * end of Silicon Init
 */
BOOT_STATE_INIT_ENTRY(BS_DEV_INIT_CHIPS, BS_ON_EXIT, clear_recovery_request, NULL);
