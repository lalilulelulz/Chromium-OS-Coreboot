/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2008-2009 coresystems GmbH
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

#include <arch/io.h>
#include <arch/romcc_io.h>
#include <console/console.h>
#include <cpu/x86/smm.h>
#include <southbridge/intel/bd82x6x/nvs.h>
#include <southbridge/intel/bd82x6x/pch.h>
#include <southbridge/intel/bd82x6x/me.h>
#include <northbridge/intel/sandybridge/sandybridge.h>
#include <cpu/intel/model_206ax/model_206ax.h>

/* Include romstage serial for SIO helper functions */
#include <superio/ite/it8772f/early_serial.c>

/* The southbridge SMI handler checks whether gnvs has a
 * valid pointer before calling the trap handler
 */
extern global_nvs_t *gnvs;

int mainboard_io_trap_handler(int smif)
{
	switch (smif) {
	case 0x99:
		printk(BIOS_DEBUG, "Sample\n");
		gnvs->smif = 0;
		break;
	default:
		return 0;
	}

	/* On success, the IO Trap Handler returns 0
	 * On failure, the IO Trap Handler returns a value != 0
	 *
	 * For now, we force the return value to 0 and log all traps to
	 * see what's going on.
	 */
	//gnvs->smif = 0;
	return 1;
}

/*
 * Change LED_POWER# (SIO GPIO 45 and GPIO 22) state based on sleep type.
 * The IO address is hardcoded as we don't have device path in SMM.
 */
void mainboard_smi_sleep(u8 slp_typ)
{
	u8 reg8;

	switch (slp_typ) {
	case SLP_TYP_S3:
	case SLP_TYP_S4:
		/* Suspend LED */
		it8772f_enter_conf();
		it8772f_sio_write(IT8772F_CONFIG_REG_LDN, IT8772F_GPIO);
		/* Set up GPIO 22, PLED2 */
		reg8 = it8772f_sio_read(GPIO_REG_SELECT(1));
		reg8 |= (1 << 2);
		it8772f_sio_write(GPIO_REG_SELECT(1), reg8);
		reg8 = it8772f_sio_read(GPIO_REG_OUTPUT(1));
		reg8 |= (1 << 2);
		/* Enable GPIO 22, PLED2 */
		it8772f_sio_write(GPIO_REG_OUTPUT(1), reg8);
		reg8 = it8772f_sio_read(GPIO_REG_ENABLE(1));
		reg8 |= (1 << 2);
		it8772f_sio_write(GPIO_REG_ENABLE(1), reg8);
		/* Disable GPIO 45, PLED1 */
		reg8 = it8772f_sio_read(GPIO_REG_ENABLE(3));
		reg8 &= ~(1 << 5);
		it8772f_sio_write(GPIO_REG_ENABLE(3), reg8);
		it8772f_exit_conf();
		break;

	case SLP_TYP_S5:
		break;
	}
}

#define APMC_FINALIZE 0xcb

static int mainboard_finalized = 0;

void mainboard_smi_apmc(u8 apmc)
{
	switch (apmc) {
	case APMC_FINALIZE:
		if (mainboard_finalized) {
			printk(BIOS_DEBUG, "SMI#: Already finalized\n");
			return;
		}

		intel_me_finalize_smm();
		intel_pch_finalize_smm();
		intel_sandybridge_finalize_smm();
		intel_model_206ax_finalize_smm();

		mainboard_finalized = 1;
		break;
	}
}
