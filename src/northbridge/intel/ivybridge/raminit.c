/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Google Inc.
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <console/console.h>
#include <string.h>
#include <arch/io.h>
#include <arch/romcc_io.h>
#include <cbmem.h>
#include <arch/cbfs.h>
#include <cbfs.h>
#include <pc80/mc146818rtc.h>
#include "raminit.h"
#include "pei_data.h"
#include "sandybridge.h"

/* Management Engine is in the southbridge */
#if CONFIG_SOUTHBRIDGE_INTEL_BD82X6X
#include "southbridge/intel/bd82x6x/me.h"
#endif

#if CONFIG_CHROMEOS
#include <vendorcode/google/chromeos/chromeos.h>
#endif
#include <fdt/libfdt.h>

/*
 * MRC scrambler seed offsets should be reserved in
 * mainboard cmos.layout and not covered by checksum.
 */
#if CONFIG_USE_OPTION_TABLE
#include "option_table.h"
#define CMOS_OFFSET_MRC_SEED     (CMOS_VSTART_mrc_scrambler_seed >> 3)
#define CMOS_OFFSET_MRC_SEED_S3  (CMOS_VSTART_mrc_scrambler_seed_s3 >> 3)
#else
#define CMOS_OFFSET_MRC_SEED     112
#define CMOS_OFFSET_MRC_SEED_S3  116
#endif

struct mrc_data_container {
	u32	mrc_data_size;	// Actual total size of this structure
	u8	mrc_data[0];	// Variable size, platform/run time dependent.
};

static void save_mrc_data(struct pei_data *pei_data)
{
#if CONFIG_EARLY_CBMEM_INIT
	struct mrc_data_container *mrcdata;

	/* Save the MRC S3 restore data to cbmem */
	cbmem_initialize();
	mrcdata = cbmem_add
		(CBMEM_ID_MRCDATA,
		 pei_data->mrc_output_len + sizeof(struct mrc_data_container));

	printk(BIOS_DEBUG, "Relocate MRC DATA from %p to %p (%u bytes)\n",
	       pei_data->mrc_output, mrcdata, pei_data->mrc_output_len);

	mrcdata->mrc_data_size = pei_data->mrc_output_len +
		sizeof(struct mrc_data_container);
	memcpy(mrcdata->mrc_data, pei_data->mrc_output,
	       pei_data->mrc_output_len);
#endif

	/* Save the MRC seed values to CMOS */
	cmos_write32(CMOS_OFFSET_MRC_SEED, pei_data->scrambler_seed);
	printk(BIOS_DEBUG, "Save scrambler seed    0x%08x to CMOS 0x%02x\n",
	       pei_data->scrambler_seed, CMOS_OFFSET_MRC_SEED);

	cmos_write32(CMOS_OFFSET_MRC_SEED_S3, pei_data->scrambler_seed_s3);
	printk(BIOS_DEBUG, "Save s3 scrambler seed 0x%08x to CMOS 0x%02x\n",
	       pei_data->scrambler_seed_s3, CMOS_OFFSET_MRC_SEED_S3);
}

#if CONFIG_CHROMEOS
static void prepare_mrc_cache(struct pei_data *pei_data)
{
	const struct fdt_header *fdt_header;
	const struct fdt_property *fdtp;
	int offset, len;
	const char *compatible = "chromeos,flashmap";
	const char *subnode = "rw-mrc-cache";
	const char *property = "reg";
	u32 *data, *mrc_cache;
	u64 flashrom_base = 0;

	// preset just in case there is an error
	pei_data->mrc_input = NULL;
	pei_data->mrc_input_len = 0;

	fdt_header = cbfs_find_file(CONFIG_FDT_FILE_NAME, CBFS_TYPE_FDT);

	if (!fdt_header) {
		printk(BIOS_ERR, "%s: no FDT found!\n", __func__);
		return;
	}

	offset = fdt_node_offset_by_compatible(fdt_header, 0, compatible);
	if (offset < 0) {
		printk(BIOS_ERR, "%s: no %s  node found!\n",
		       __func__, compatible);
		return;
	}

	if (fdt_get_base_addr(fdt_header, offset, &flashrom_base) < 0) {
		printk(BIOS_ERR, "%s: no base address in node name!\n",
		       __func__);
		return;
	}

	offset = fdt_subnode_offset(fdt_header, offset, subnode);
	if (offset < 0) {
		printk(BIOS_ERR, "%s: no %s found!\n", __func__, subnode);
		return;
	}

	fdtp = fdt_get_property(fdt_header, offset, property, &len);
	if (!fdtp || (len != 8)) {
		printk(BIOS_ERR, "%s: property %s at %p, len %d!\n",
		       __func__, property, fdtp, len);
		return;
	}

	data = (u32 *)fdtp->data;

	// Calculate actual address of the MRC cache in memory
	mrc_cache = (u32 *)
		((unsigned long)flashrom_base + fdt32_to_cpu(data[0]));

	// First 4 byte word of the MRC cache space is its size.
	if (mrc_cache[0] > data[1]) {
		printk(BIOS_ERR, "%s: MRC cache not initialized?\n",  __func__);
		return;
	}

	pei_data->mrc_input = (u8 *)(mrc_cache + 1);
	pei_data->mrc_input_len = mrc_cache[0];

	printk(BIOS_DEBUG, "%s: at %p, size %x\n",
	       __func__, pei_data->mrc_input, pei_data->mrc_input_len);

	/* Read scrambler seeds from CMOS */
	pei_data->scrambler_seed = cmos_read32(CMOS_OFFSET_MRC_SEED);
	printk(BIOS_DEBUG, "Read scrambler seed    0x%08x from CMOS 0x%02x\n",
	       pei_data->scrambler_seed, CMOS_OFFSET_MRC_SEED);

	pei_data->scrambler_seed_s3 = cmos_read32(CMOS_OFFSET_MRC_SEED_S3);
	printk(BIOS_DEBUG, "Read S3 scrambler seed 0x%08x from CMOS 0x%02x\n",
	       pei_data->scrambler_seed_s3, CMOS_OFFSET_MRC_SEED_S3);
}
#endif

/**
 * Find PEI executable in coreboot filesystem and execute it.
 *
 * @param pei_data: configuration data for UEFI PEI reference code
 */
void sdram_initialize(struct pei_data *pei_data)
{
	struct sys_info sysinfo;
	const char *target = "mrc.bin";
	unsigned long entry;

#if CONFIG_SOUTHBRIDGE_INTEL_BD82X6X
	/* Wait for ME to be ready */
	intel_early_me_init();
	intel_early_me_uma_size();
#endif

	printk(BIOS_DEBUG, "Starting UEFI PEI System Agent\n");

	memset(&sysinfo, 0, sizeof(sysinfo));

	sysinfo.boot_path = pei_data->boot_mode;

#if CONFIG_CHROMEOS
	/*
	 * Do not pass MRC data in for recovery mode boot,
	 * Always pass it in for S3 resume.
	 */
	if (!recovery_mode_enabled() || pei_data->boot_mode == 2)
		prepare_mrc_cache(pei_data);
#endif

	//findstage seems to be faster but requires an ELF at compile time.
	//entry = findstage((char *)target);
	entry = (unsigned long)cbfs_find_file(target, 0xab);

	if (entry) {
		//call(entry, pei_data);
		asm volatile ("call *%%ecx\n\t" : : "c" (entry), "a" (pei_data));
	} else {
		die("UEFI PEI System Agent not found.\n");
	}

	/* For reference print the System Agent version
	 * after executing the UEFI PEI stage.
	 */
	u32 version = MCHBAR32(0x5034);
	printk(BIOS_DEBUG, "System Agent Version %d.%d.%d Build %d\n",
		version >> 24 , (version >> 16) & 0xff,
		(version >> 8) & 0xff, version & 0xff);

#if CONFIG_SOUTHBRIDGE_INTEL_BD82X6X
	/* Tell the ME that DRAM init is done and perform
	 * the requested action, which may be a reset.
	 */
	intel_early_me_dram_init_done(ME_DRAM_STATUS_SUCCESS);
#endif

	/* S3 resume: don't save scrambler seed or MRC data */
	if (pei_data->boot_mode != 2)
		save_mrc_data(pei_data);
}

struct cbmem_entry *get_cbmem_toc(void)
{
	return (struct cbmem_entry *)(get_top_of_ram() - HIGH_MEMORY_SIZE);
}

unsigned long get_top_of_ram(void)
{
	/* Base of TSEG is top of usable DRAM */
	u32 tom = pci_read_config32(PCI_DEV(0,0,0), TSEG);
	return (unsigned long) tom;
}
