/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Google Inc.
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

#include <stdlib.h>
#include <ec/google/chromeec/ec.h>
#include "board_version.h"

const char *samus_board_version(void)
{
	const char *version_string[] = { "EVT", "DVT", "PVT", "MP.A",
					 "MP.B", "MP.C", "MP.D", "MP.E" };
	u8 board_version = google_chromeec_get_board_version();

	if (board_version >= ARRAY_SIZE(version_string))
		return "Unknown";

	return version_string[board_version];
}
