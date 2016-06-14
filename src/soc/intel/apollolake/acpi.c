/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Intel Corp.
 * (Written by Lance Zhao <lijian.zhao@intel.com> for Intel Corp.)
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
 */

#include <arch/acpi.h>
#include <arch/acpigen.h>
#include <arch/ioapic.h>
#include <arch/smp/mpspec.h>
#include <cbmem.h>
#include <cpu/x86/smm.h>
#include <cpu/cpu.h>
#include <soc/acpi.h>
#include <soc/intel/common/acpi.h>
#include <soc/iomap.h>
#include <soc/pm.h>
#include <soc/nvs.h>

#define CSTATE_RES(address_space, width, offset, address)		\
	{								\
	.space_id = address_space,					\
	.bit_width = width,						\
	.bit_offset = offset,						\
	.addrl = address,						\
	}

unsigned long acpi_fill_mcfg(unsigned long current)
{
	/* PCI Segment Group 0, Start Bus Number 0, End Bus Number is 255 */
	current += acpi_create_mcfg_mmconfig((void *) current,
					     CONFIG_MMCONF_BASE_ADDRESS, 0, 0,
					     255);
	return current;
}

static int acpi_sci_irq(void)
{
	int sci_irq = 9;
	return sci_irq;
}

static unsigned long acpi_madt_irq_overrides(unsigned long current)
{
	int sci = acpi_sci_irq();
	uint16_t flags = MP_IRQ_TRIGGER_LEVEL | MP_IRQ_POLARITY_LOW;;

	/* INT_SRC_OVR */
	current += acpi_create_madt_irqoverride((void *)current, 0, 0, 2, 0);

	/* SCI */
	current += acpi_create_madt_irqoverride((void *)current, 0, sci, sci, flags);

	return current;
}

unsigned long acpi_fill_madt(unsigned long current)
{
	/* Local APICs */
	current = acpi_create_madt_lapics(current);

	/* IOAPIC */
	current += acpi_create_madt_ioapic((void *) current,
					     2, IO_APIC_ADDR, 0);

	return acpi_madt_irq_overrides(current);
}

void acpi_fill_fadt(acpi_fadt_t * fadt)
{
	const uint16_t pmbase = ACPI_PMIO_BASE;

	/* Use ACPI 5.0 revision. */
	fadt->header.revision = ACPI_FADT_REV_ACPI_5_0;

	fadt->sci_int = acpi_sci_irq();
	fadt->smi_cmd = 0;	/* No Smi Handler as SMI_CMD is 0*/

	fadt->pm1a_evt_blk = pmbase + PM1_STS;
	fadt->pm1a_cnt_blk = pmbase + PM1_CNT;
	fadt->pm_tmr_blk = pmbase + PM1_TMR;
	fadt->gpe0_blk = pmbase + GPE0_STS(0);

	fadt->pm1_evt_len = 4;
	fadt->pm1_cnt_len = 2;
	fadt->pm_tmr_len = 4;
	/* There are 4 GPE0 STS/EN pairs each 32 bits wide. */
	fadt->gpe0_blk_len = 2 * GPE0_REG_MAX * sizeof(uint32_t);
	fadt->p_lvl2_lat = ACPI_FADT_C2_NOT_SUPPORTED;
	fadt->p_lvl3_lat = ACPI_FADT_C3_NOT_SUPPORTED;
	fadt->flush_size = 0x400;	/* twice of cache size*/
	fadt->flush_stride = 0x10;	/* Cache line width  */
	fadt->duty_offset = 1;
	fadt->duty_width = 3;
	fadt->day_alrm = 0xd;
	fadt->iapc_boot_arch = ACPI_FADT_LEGACY_DEVICES | ACPI_FADT_8042;

	fadt->flags = ACPI_FADT_WBINVD | ACPI_FADT_C1_SUPPORTED |
	    ACPI_FADT_C2_MP_SUPPORTED | ACPI_FADT_SLEEP_BUTTON |
	    ACPI_FADT_RESET_REGISTER | ACPI_FADT_SEALED_CASE |
	    ACPI_FADT_S4_RTC_WAKE | ACPI_FADT_PLATFORM_CLOCK;

	fadt->reset_reg.space_id = 1;
	fadt->reset_reg.bit_width = 8;
	fadt->reset_reg.addrl = 0xcf9;
	fadt->reset_value = 6;

	fadt->x_pm1a_evt_blk.space_id = 1;
	fadt->x_pm1a_evt_blk.bit_width = fadt->pm1_evt_len * 8;
	fadt->x_pm1a_evt_blk.addrl = pmbase + PM1_STS;

	fadt->x_pm1b_evt_blk.space_id = 1;

	fadt->x_pm1a_cnt_blk.space_id = 1;
	fadt->x_pm1a_cnt_blk.bit_width = fadt->pm1_cnt_len * 8;
	fadt->x_pm1a_cnt_blk.addrl = pmbase + PM1_CNT;

	fadt->x_pm1b_cnt_blk.space_id = 1;

	fadt->x_pm_tmr_blk.space_id = 1;
	fadt->x_pm_tmr_blk.bit_width = fadt->pm_tmr_len * 8;
	fadt->x_pm_tmr_blk.addrl = pmbase + PM1_TMR;

	fadt->x_gpe1_blk.space_id = 1;
}

unsigned long southbridge_write_acpi_tables(device_t device,
		unsigned long current,
		struct acpi_rsdp *rsdp)
{
	return acpi_write_hpet(device, current, rsdp);
}

static void acpi_create_gnvs(struct global_nvs_t *gnvs)
{
	if (IS_ENABLED(CONFIG_CONSOLE_CBMEM))
		gnvs->cbmc = (uintptr_t)cbmem_find(CBMEM_ID_CONSOLE);

	if (IS_ENABLED(CONFIG_CHROMEOS)) {
		/* Initialize Verified Boot data */
		chromeos_init_vboot(&gnvs->chromeos);
		gnvs->chromeos.vbt2 = ACTIVE_ECFW_RO;
	}
}

void southbridge_inject_dsdt(device_t device)
{
	struct global_nvs_t *gnvs;

	gnvs = cbmem_find(CBMEM_ID_ACPI_GNVS);

	if (gnvs) {
		acpi_create_gnvs(gnvs);
		acpi_save_gnvs((uintptr_t)gnvs);
		/* And tell SMI about it */
		smm_setup_structures(gnvs, NULL, NULL);

		/* Add it to DSDT.  */
		acpigen_write_scope("\\");
		acpigen_write_name_dword("NVSA", (uintptr_t)gnvs);
		acpigen_pop_len();
	}
}
static acpi_cstate_t cstate_map[] = {
	{
		/* C1 */
		.ctype = 1, /* ACPI C1 */
		.latency = 1,
		.power = 1000,
		.resource = CSTATE_RES(ACPI_ADDRESS_SPACE_FIXED, 0, 0, 0),
	},
	{
		.ctype = 2, /* ACPI C2 */
		.latency = 50,
		.power = 10,
		.resource = CSTATE_RES(ACPI_ADDRESS_SPACE_IO, 8, 0, 0x415),
	},
	{
		.ctype = 3, /* ACPI C3 */
		.latency = 150,
		.power = 10,
		.resource = CSTATE_RES(ACPI_ADDRESS_SPACE_IO, 8, 0, 0x419),
	}
};

acpi_cstate_t *soc_get_cstate_map(int *entries)
{
	*entries = ARRAY_SIZE(cstate_map);
	return cstate_map;
}

uint16_t soc_get_acpi_base_address(void)
{
	return ACPI_PMIO_BASE;
}
