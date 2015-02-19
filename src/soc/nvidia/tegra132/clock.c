/*
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <console/console.h>
#include <delay.h>
#include <arch/io.h>
#include <soc/addressmap.h>
#include <soc/clock.h>
#include <stdlib.h>
#include <arch/clock.h>
#include "clk_rst.h"
#include "clst_clk.h"
#include "flow.h"
#include "maincpu.h"
#include "pmc.h"
#include "sysctr.h"

static struct clst_clk_ctlr *clst_clk = (void *)TEGRA_CLUSTER_CLOCK_BASE;
static struct flow_ctlr *flow = (void *)TEGRA_FLOW_BASE;
static struct tegra_pmc_regs *pmc = (void *)TEGRA_PMC_BASE;
static struct sysctr_regs *sysctr = (void *)TEGRA_SYSCTR0_BASE;

struct pll_dividers {
	u32	n : 10;
	u32	m : 8;
	u32	p : 4;
	u32	cpcon : 4;
	u32	lfcon : 4;
	u32	: 2;
};

/* Some PLLs have more restrictive divider bit lengths or are missing some
 * fields. Make sure to use the right struct in the osc_table definition to get
 * compile-time checking, but keep the bits aligned with struct pll_dividers so
 * they can be used interchangeably at run time. Add new formats as required. */
struct pllcx_dividers {
	u32	n : 8;
	u32	: 2;
	u32	m : 8;
	u32	p : 4;
	u32	: 10;
};
struct pllpad_dividers {
	u32	n : 10;
	u32	m : 5;
	u32	: 3;
	u32	p : 3;
	u32	: 1;
	u32	cpcon : 4;
	u32	: 6;
};
struct pllu_dividers {
	u32	n : 10;
	u32	m : 5;
	u32	: 3;
	u32	p : 1;
	u32	: 3;
	u32	cpcon : 4;
	u32	lfcon : 4;
	u32	: 2;
};

union __attribute__((transparent_union)) pll_fields {
	u32 raw;
	struct pll_dividers div;
	struct pllcx_dividers cx;
	struct pllpad_dividers pad;
	struct pllu_dividers u;
};

/* This table defines the frequency dividers for every PLL to turn the external
 * OSC clock into the frequencies defined by TEGRA_PLL*_KHZ in soc/clock.h.
 * All PLLs have three dividers (n, m and p), with the governing formula for
 * the output frequency being CF = (IN / m), VCO = CF * n and OUT = VCO / (2^p).
 * All divisor configurations must meet the PLL's constraints for VCO and CF:
 * PLLX:  12 MHz < CF < 50 MHz, 700 MHz < VCO < 3000 MHz
 * PLLC:  12 MHz < CF < 50 MHz, 600 MHz < VCO < 1400 MHz
 * PLLM:  12 MHz < CF < 50 MHz, 400 MHz < VCO < 1066 MHz
 * PLLP:   1 MHz < CF <  6 MHz, 200 MHz < VCO <  700 MHz
 * PLLD:   1 MHz < CF <  6 MHz, 500 MHz < VCO < 1000 MHz
 * PLLU:   1 MHz < CF <  6 MHz, 480 MHz < VCO <  960 MHz
 * PLLDP: 12 MHz < CF < 38 MHz, 600 MHz < VCO < 1200 MHz
 * (values taken from Linux' drivers/clk/tegra/clk-tegra124.c). */
struct {
	int khz;
	struct pllcx_dividers	pllx;	/* target:  CONFIG_PLLX_KHZ */
	struct pllcx_dividers	pllc;	/* target:  600 MHz */
	/* PLLM is set up dynamically by clock_sdram(). */
	/* PLLP is hardwired to 408 MHz in HW (unless we set BASE_OVRD). */
	struct pllu_dividers	pllu;	/* target;  960 MHz */
	struct pllcx_dividers	plldp;	/* target;  270 MHz */
	/* PLLDP treats p differently (OUT = VCO / (p + 1) for p < 6). */
} static const osc_table[16] = {
	[OSC_FREQ_12]{
		.khz = 12000,
		.pllx = {.n = TEGRA_PLLX_KHZ / 12000, .m =  1, .p = 0},
		.pllc = {.n =  50, .m =  1, .p = 0},
		.pllu = {.n = 960, .m = 12, .p = 0, .cpcon = 12, .lfcon = 2},
		.plldp = {.n = 90, .m =  1, .p = 3},
	},
	[OSC_FREQ_13]{
		.khz = 13000,
		.pllx = {.n = TEGRA_PLLX_KHZ / 13000, .m =  1, .p = 0},
		.pllc = {.n =  46, .m =  1, .p = 0},		 /* 598.0 MHz */
		.pllu = {.n = 960, .m = 13, .p = 0, .cpcon = 12, .lfcon = 2},
		.plldp = {.n = 83, .m =  1, .p = 3},		 /* 269.8 MHz */
	},
	[OSC_FREQ_16P8]{
		.khz = 16800,
		.pllx = {.n = TEGRA_PLLX_KHZ / 16800, .m =  1, .p = 0},
		.pllc = {.n =  71, .m =  1, .p = 1},		 /* 596.4 MHz */
		.pllu = {.n = 400, .m =  7, .p = 0, .cpcon = 5, .lfcon = 2},
		.plldp = {.n = 64, .m =  1, .p = 3},		 /* 268.8 MHz */
	},
	[OSC_FREQ_19P2]{
		.khz = 19200,
		.pllx = {.n = TEGRA_PLLX_KHZ / 19200, .m =  1, .p = 0},
		.pllc = {.n =  62, .m =  1, .p = 1},		 /* 595.2 MHz */
		.pllu = {.n = 200, .m =  4, .p = 0, .cpcon = 3, .lfcon = 2},
		.plldp = {.n = 56, .m =  1, .p = 3},		 /* 268.8 MHz */
	},
	[OSC_FREQ_26]{
		.khz = 26000,
		.pllx = {.n = TEGRA_PLLX_KHZ / 26000, .m =  1, .p = 0},
		.pllc = {.n =  23, .m =  1, .p = 0},		 /* 598.0 MHz */
		.pllu = {.n = 960, .m = 26, .p = 0, .cpcon = 12, .lfcon = 2},
		.plldp = {.n = 83, .m =  2, .p = 3},		 /* 269.8 MHz */
	},
	/* These oscillators get predivided as PLL inputs... n/m/p divisors for
	 * 38.4 should always match 19.2, and 48 should always match 12. */
	[OSC_FREQ_38P4]{
		.khz = 38400,
		.pllx = {.n = TEGRA_PLLX_KHZ / 19200, .m =  1, .p = 0},
		.pllc = {.n =  62, .m =  1, .p = 1},		 /* 595.2 MHz */
		.pllu = {.n = 200, .m =  4, .p = 0, .cpcon = 3, .lfcon = 2},
		.plldp = {.n = 56, .m =  1, .p = 3},		 /* 268.8 MHz */
	},
	[OSC_FREQ_48]{
		.khz = 48000,
		.pllx = {.n = TEGRA_PLLX_KHZ / 12000, .m =  1, .p = 0},
		.pllc = {.n =  50, .m =  1, .p = 0},
		.pllu = {.n = 960, .m = 12, .p = 0, .cpcon = 12, .lfcon = 2},
		.plldp = {.n = 90, .m =  1, .p = 3},
	},
};

/* Get the oscillator frequency, from the corresponding hardware
 * configuration field. This is actually a per-soc thing. Avoid the
 * temptation to make it common.
 */
static u32 clock_get_osc_bits(void)
{
	return (readl(CLK_RST_REG(osc_ctrl)) & OSC_FREQ_MASK) >> OSC_FREQ_SHIFT;
}

int clock_get_osc_khz(void)
{
	return osc_table[clock_get_osc_bits()].khz;
}

int clock_get_pll_input_khz(void)
{
	u32 osc_ctrl = readl(CLK_RST_REG(osc_ctrl));
	u32 osc_bits = (osc_ctrl & OSC_FREQ_MASK) >> OSC_FREQ_SHIFT;
	u32 pll_ref_div = (osc_ctrl & OSC_PREDIV_MASK) >> OSC_PREDIV_SHIFT;
	return osc_table[osc_bits].khz >> pll_ref_div;
}

void clock_init_arm_generic_timer(void)
{
	uint32_t freq = clock_get_osc_khz() * 1000;
	// Set the cntfrq register.
	set_cntfrq(freq);

	// Record the system timer frequency.
	writel(freq, &sysctr->cntfid0);
	// Enable the system counter.
	uint32_t cntcr = read32(&sysctr->cntcr);
	cntcr |= SYSCTR_CNTCR_EN | SYSCTR_CNTCR_HDBG;
	writel(cntcr, &sysctr->cntcr);
}

#define SOR0_CLK_SEL0			(1 << 14)
#define SOR0_CLK_SEL1			(1 << 15)

void sor_clock_stop(void)
{
	/* The Serial Output Resource clock has to be off
	 * before we start the plldp. Learned the hard way.
	 * FIXME: this has to be cleaned up a bit more.
	 * Waiting on some new info from Nvidia.
	 */
	clrbits_le32(CLK_RST_REG(clk_src_sor), SOR0_CLK_SEL0 | SOR0_CLK_SEL1);
}

void sor_clock_start(void)
{
	/* uses PLLP, has a non-standard bit layout. */
	setbits_le32(CLK_RST_REG(clk_src_sor), SOR0_CLK_SEL0);
}

static void init_pll(u32 *base, u32 *misc, const union pll_fields pll, u32 lock)
{
	u32 dividers =  pll.div.n << PLL_BASE_DIVN_SHIFT |
			pll.div.m << PLL_BASE_DIVM_SHIFT |
			pll.div.p << PLL_BASE_DIVP_SHIFT;
	u32 misc_con = pll.div.cpcon << PLL_MISC_CPCON_SHIFT |
		       pll.div.lfcon << PLL_MISC_LFCON_SHIFT;

	/* Write dividers but BYPASS the PLL while we're messing with it. */
	writel(dividers | PLL_BASE_BYPASS, base);
	/*
	 * Set Lock bit, CPCON and LFCON fields (default to 0 if it doesn't
	 * exist for this PLL)
	 */
	writel(lock | misc_con, misc);

	/* Enable PLL and take it back out of BYPASS */
	writel(dividers | PLL_BASE_ENABLE, base);

	/* Wait for lock ready */
	while (!(readl(base) & PLL_BASE_LOCK));
}

static void init_utmip_pll(void)
{
	int khz = clock_get_pll_input_khz();

	/* Shut off PLL crystal clock while we mess with it */
	clrbits_le32(CLK_RST_REG(utmip_pll_cfg2), 1 << 30); /* PHY_XTAL_CLKEN */
	udelay(1);

	writel(80 << 16 | 1 << 8 | 0, CLK_RST_REG(utmip_pll_cfg0));/* 960MHz * 1 / 80 == 12 MHz */

	writel(div_round_up(khz, 8000) << 27 | 0 << 16 | 0 << 14 | 0 << 12 | div_round_up(khz, 102) << 0 | 0,
	       CLK_RST_REG(utmip_pll_cfg1));

	/* TODO: TRM can't decide if actv is 5us or 10us, keep an eye on it */
	writel(0 << 24 | div_round_up(khz, 3200) << 18 | div_round_up(khz, 256) << 6 | 0 << 4 | 0 << 2 | 0 << 0 | 0,
	       CLK_RST_REG(utmip_pll_cfg2));

	setbits_le32(CLK_RST_REG(utmip_pll_cfg2), 1 << 30); /* PHY_XTAL_CLKEN */
}

/* Graphics just has to be different. There's a few more bits we
 * need to set in here, but it makes sense just to restrict all the
 * special bits to this one function.
 */
static void graphics_pll(void)
{
	int osc = clock_get_osc_bits();
	u32 *cfg = CLK_RST_REG(plldp_ss_cfg);
	/* the vendor code sets the dither bit (28)
	 * an undocumented bit (24)
	 * and clamp while we mess with it (22)
	 * Dither is pretty important to display port
	 * so we really do need to handle these bits.
	 * I'm not willing to not clamp it, even if
	 * it might "mostly work" with it not set,
	 * I don't want to find out in a few months
	 * that it is needed.
	 */
	u32 scfg = (1<<28) | (1<<24) | (1<<22);
	writel(scfg, cfg);
	init_pll(CLK_RST_REG(plldp_base), CLK_RST_REG(plldp_misc),
		osc_table[osc].plldp, PLLDPD2_MISC_LOCK_ENABLE);
	/* leave dither and undoc bits set, release clamp */
	scfg = (1<<28) | (1<<24);
	writel(scfg, cfg);

	/* disp1 will be set when panel information (pixel clock) is
	 * retrieved (clock_display).
	 */
}

/*
 * Init PLLD clock source.
 *
 * @frequency: the requested plld frequency
 *
 * Return the plld frequency if success, otherwise return 0.
 */
u32
clock_display(u32 frequency)
{
	/**
	 * plld (fo) = vco >> p, where 500MHz < vco < 1000MHz
	 *           = (cf * n) >> p, where 1MHz < cf < 6MHz
	 *           = ((ref / m) * n) >> p
	 *
	 * Iterate the possible values of p (3 bits, 2^7) to find out a minimum
	 * safe vco, then find best (m, n). since m has only 5 bits, we can
	 * iterate all possible values.  Note Tegra 124 supports 11 bits for n,
	 * but our pll_fields has only 10 bits for n.
	 *
	 * Note values undershoot or overshoot target output frequency may not
	 * work if the values are not in "safe" range by panel specification.
	 */
	struct pllpad_dividers plld = { 0 };
	u32 ref = clock_get_pll_input_khz() * 1000, m, n, p = 0;
	u32 cf, vco, rounded_rate = frequency;
	u32 diff, best_diff;
	const u32 max_m = 1 << 5, max_n = 1 << 10, max_p = 1 << 3,
		  mhz = 1000 * 1000, min_vco = 500 * mhz, max_vco = 1000 * mhz,
		  min_cf = 1 * mhz, max_cf = 6 * mhz;

	for (vco = frequency; vco < min_vco && p < max_p; p++)
		vco <<= 1;

	if (vco < min_vco || vco > max_vco) {
		printk(BIOS_ERR, "%s: Cannot find out a supported VCO"
			" for Frequency (%u).\n", __func__, frequency);
		return 0;
	}

	plld.p = p;
	best_diff = vco;

	for (m = 1; m < max_m && best_diff; m++) {
		cf = ref / m;
		if (cf < min_cf)
			break;
		if (cf > max_cf)
			continue;

		n = vco / cf;
		if (n >= max_n)
			continue;

		diff = vco - n * cf;
		if (n + 1 < max_n && diff > cf / 2) {
			n++;
			diff = cf - diff;
		}

		if (diff >= best_diff)
			continue;

		best_diff = diff;
		plld.m = m;
		plld.n = n;
	}

	if (plld.n < 50)
		plld.cpcon = 2;
	else if (plld.n < 300)
		plld.cpcon = 3;
	else if (plld.n < 600)
		plld.cpcon = 8;
	else
		plld.cpcon = 12;

	if (best_diff) {
		printk(BIOS_WARNING, "%s: Failed to match output frequency %u, "
		       "best difference is %u.\n", __func__, frequency,
		       best_diff);
		rounded_rate = (ref / plld.m * plld.n) >> plld.p;
	}

	printk(BIOS_DEBUG, "%s: PLLD=%u ref=%u, m/n/p/cpcon=%u/%u/%u/%u\n",
	       __func__, rounded_rate, ref, plld.m, plld.n, plld.p, plld.cpcon);

	init_pll(CLK_RST_REG(plld_base), CLK_RST_REG(plld_misc), plld,
		 (PLLUD_MISC_LOCK_ENABLE | PLLD_MISC_CLK_ENABLE));

	return rounded_rate;
}

/* Initialize the UART and put it on CLK_M so we can use it during clock_init().
 * Will later move it to PLLP in clock_config(). The divisor must be very small
 * to accomodate 12KHz OSCs, so we override the 16.0 UART divider with the 15.1
 * CLK_SOURCE divider to get more precision. (This might still not be enough for
 * some OSCs... if you use 13KHz, be prepared to have a bad time.) The 1900 has
 * been determined through trial and error (must lead to div 13 at 24MHz). */
void clock_early_uart(void)
{
	writel(CLK_SRC_DEV_ID(UARTA, CLK_M) << CLK_SOURCE_SHIFT | CLK_UART_DIV_OVERRIDE | CLK_DIVIDER(TEGRA_CLK_M_KHZ, 1900),
	       CLK_RST_REG(clk_src_uarta));

	clock_enable_clear_reset_l(CLK_L_UARTA);
}

/* Enable output clock (CLK1~3) for external peripherals. */
void clock_external_output(int clk_id)
{
	switch (clk_id) {
	case 1:
		setbits_le32(&pmc->clk_out_cntrl, 1 << 2);
		break;
	case 2:
		setbits_le32(&pmc->clk_out_cntrl, 1 << 10);
		break;
	case 3:
		setbits_le32(&pmc->clk_out_cntrl, 1 << 18);
		break;
	default:
		printk(BIOS_CRIT, "ERROR: Unknown output clock id %d\n",
		       clk_id);
		break;
	}
}

/* Start PLLM for SDRAM. */
void clock_sdram(u32 m, u32 n, u32 p, u32 setup, u32 ph45, u32 ph90,
		 u32 ph135, u32 kvco, u32 kcp, u32 stable_time, u32 emc_source,
		 u32 same_freq)
{
	u32 misc1 = ((setup << PLLM_MISC1_SETUP_SHIFT) |
		     (ph45 << PLLM_MISC1_PD_LSHIFT_PH45_SHIFT) |
		     (ph90 << PLLM_MISC1_PD_LSHIFT_PH90_SHIFT) |
		     (ph135 << PLLM_MISC1_PD_LSHIFT_PH135_SHIFT)),
	    misc2 = ((kvco << PLLM_MISC2_KVCO_SHIFT) |
		     (kcp << PLLM_MISC2_KCP_SHIFT)),
	    base;

	if (same_freq)
		emc_source |= CLK_SOURCE_EMC_MC_EMC_SAME_FREQ;
	else
		emc_source &= ~CLK_SOURCE_EMC_MC_EMC_SAME_FREQ;

	/*
	 * Note PLLM_BASE.PLLM_OUT1_RSTN must be in RESET_ENABLE mode, and
	 * PLLM_BASE.ENABLE must be in DISABLE state (both are the default
	 * values after coldboot reset).
	 */

	writel(misc1, CLK_RST_REG(pllm_misc1));
	writel(misc2, CLK_RST_REG(pllm_misc2));

	/* PLLM.BASE needs BYPASS=0, different from general init_pll */
	base = readl(CLK_RST_REG(pllm_base));
	base &= ~(PLLCMX_BASE_DIVN_MASK | PLLCMX_BASE_DIVM_MASK |
		  PLLM_BASE_DIVP_MASK | PLL_BASE_BYPASS);
	base |= ((m << PLL_BASE_DIVM_SHIFT) | (n << PLL_BASE_DIVN_SHIFT) |
		 (p << PLL_BASE_DIVP_SHIFT));
	writel(base, CLK_RST_REG(pllm_base));

	setbits_le32(CLK_RST_REG(pllm_base), PLL_BASE_ENABLE);
	/* stable_time is required, before we can start to check lock. */
	udelay(stable_time);

	while (!(readl(CLK_RST_REG(pllm_base)) & PLL_BASE_LOCK))
		udelay(1);

	/*
	 * After PLLM reports being locked, we have to delay 10us before
	 * enabling PLLM_OUT.
	 */
	udelay(10);

	/* Put OUT1 out of reset state (start to output). */
	setbits_le32(CLK_RST_REG(pllm_out), PLLM_OUT1_RSTN_RESET_DISABLE);

	/* Enable and start MEM(MC) and EMC. */
	clock_enable_clear_reset(0, CLK_H_MEM | CLK_H_EMC, 0, 0, 0, 0);
	writel(emc_source, CLK_RST_REG(clk_src_emc));
	udelay(IO_STABILIZATION_DELAY);
}

void clock_cpu0_config(void)
{
	u32 reg;
	u32 osc = clock_get_osc_bits();
	u32 timeout = 0;

	/* disable IDDQ */
	reg = readl(&clst_clk->pllx_misc3);
	reg &= ~PLLX_IDDQ;
	writel(reg, &clst_clk->pllx_misc3);

	/* init pllx */
	init_pll(&clst_clk->pllx_base, &clst_clk->pllx_misc,
		osc_table[osc].pllx, PLLPAXS_MISC_LOCK_ENABLE);

	/*
	 * Change CPU clock source to PLLX_OUT0_LJ
	 * when above pllx programming has taken effect.
	 */
	do {
		if (readl(&clst_clk->misc_ctrl) & CLK_SWITCH_MATCH) {
			writel((CC_CCLK_BRST_POL_PLLX_OUT0_LJ << 28),
			       &clst_clk->cclk_brst_pol);
			break;
		}

		/* wait and try again */
		if (timeout >= CLK_SWITCH_TIMEOUT_US) {
			printk(BIOS_ERR, "%s: PLLX programming timeout. "
				"Switching cpu clock has falied.\n",
				__func__);
			break;
		}
		udelay(10);
		timeout += 10;

	} while (1);
}

void clock_halt_avp(void)
{
	for (;;) {
		writel(FLOW_EVENT_JTAG | FLOW_EVENT_LIC_IRQ | FLOW_EVENT_GIC_IRQ | FLOW_MODE_WAITEVENT,
		       &flow->halt_cop_events);
	}
}

void clock_init(void)
{
	u32 osc = clock_get_osc_bits();

	/* Set PLLC dynramp_step A to 0x2b and B to 0xb (from U-Boot -- why? */
	writel(0x2b << 17 | 0xb << 9, CLK_RST_REG(pllc_misc2));

	/* Max out the AVP clock before everything else (need PLLC for that). */
	init_pll(CLK_RST_REG(pllc_base), CLK_RST_REG(pllc_misc),
		osc_table[osc].pllc, PLLC_MISC_LOCK_ENABLE);

	/* Typical ratios are 1:2:2 or 1:2:3 sclk:hclk:pclk (See: APB DMA
	 * features section in the TRM). */
	writel(1 << HCLK_DIVISOR_SHIFT | 0 << PCLK_DIVISOR_SHIFT,
	       CLK_RST_REG(clk_sys_rate));	/* pclk = hclk = sclk/2 */
	writel(CLK_DIVIDER(TEGRA_PLLC_KHZ, 300000) << PLL_OUT_RATIO_SHIFT | PLL_OUT_CLKEN | PLL_OUT_RSTN,
	       CLK_RST_REG(pllc_out));
	writel(SCLK_SYS_STATE_RUN << SCLK_SYS_STATE_SHIFT | SCLK_SOURCE_PLLC_OUT1 << SCLK_RUN_SHIFT,
	       CLK_RST_REG(sclk_brst_pol));		/* sclk = 300 MHz */

	/* Change the oscillator drive strength (from U-Boot -- why?) */
	clrsetbits_le32(CLK_RST_REG(osc_ctrl), OSC_XOFS_MASK,
			OSC_DRIVE_STRENGTH << OSC_XOFS_SHIFT);

	/*
	 * Ambiguous quote from u-boot. TODO: what's this mean?
	 * "should update same value in PMC_OSC_EDPD_OVER XOFS
	 * field for warmboot "
	 */
	clrsetbits_le32(&pmc->osc_edpd_over, PMC_OSC_EDPD_OVER_XOFS_MASK,
			OSC_DRIVE_STRENGTH << PMC_OSC_EDPD_OVER_XOFS_SHIFT);

	/* Set up PLLP_OUT(1|2|3|4) divisor to generate (9.6|48|102|204)MHz */
	writel((CLK_DIVIDER(TEGRA_PLLP_KHZ, 9600) << PLL_OUT_RATIO_SHIFT | PLL_OUT_OVR | PLL_OUT_CLKEN | PLL_OUT_RSTN) << PLL_OUT1_SHIFT | (CLK_DIVIDER(TEGRA_PLLP_KHZ, 48000) << PLL_OUT_RATIO_SHIFT | PLL_OUT_OVR | PLL_OUT_CLKEN | PLL_OUT_RSTN) << PLL_OUT2_SHIFT,
	       CLK_RST_REG(pllp_outa));
	writel((CLK_DIVIDER(TEGRA_PLLP_KHZ, 102000) << PLL_OUT_RATIO_SHIFT | PLL_OUT_OVR | PLL_OUT_CLKEN | PLL_OUT_RSTN) << PLL_OUT3_SHIFT | (CLK_DIVIDER(TEGRA_PLLP_KHZ, 204000) << PLL_OUT_RATIO_SHIFT | PLL_OUT_OVR | PLL_OUT_CLKEN | PLL_OUT_RSTN) << PLL_OUT4_SHIFT,
	       CLK_RST_REG(pllp_outb));

	/* init pllu */
	init_pll(CLK_RST_REG(pllu_base), CLK_RST_REG(pllu_misc),
		osc_table[osc].pllu, PLLUD_MISC_LOCK_ENABLE);

	init_utmip_pll();
	graphics_pll();
}

void clock_grp_enable_clear_reset(u32 val, u32* clk_enb_set_reg,
				  u32 *rst_dev_clr_reg)
{
	writel(val, clk_enb_set_reg);
	udelay(IO_STABILIZATION_DELAY);
	writel(val, rst_dev_clr_reg);
}

static u32 * const clk_enb_set_arr[DEV_CONFIG_BLOCKS] = {
	CLK_RST_REG(clk_enb_l_set),
	CLK_RST_REG(clk_enb_h_set),
	CLK_RST_REG(clk_enb_u_set),
	CLK_RST_REG(clk_enb_v_set),
	CLK_RST_REG(clk_enb_w_set),
	CLK_RST_REG(clk_enb_x_set),
};

static u32 * const clk_enb_clr_arr[DEV_CONFIG_BLOCKS] = {
	CLK_RST_REG(clk_enb_l_clr),
	CLK_RST_REG(clk_enb_h_clr),
	CLK_RST_REG(clk_enb_u_clr),
	CLK_RST_REG(clk_enb_v_clr),
	CLK_RST_REG(clk_enb_w_clr),
	CLK_RST_REG(clk_enb_x_clr),
};

static u32 * const rst_dev_set_arr[DEV_CONFIG_BLOCKS] = {
	CLK_RST_REG(rst_dev_l_set),
	CLK_RST_REG(rst_dev_h_set),
	CLK_RST_REG(rst_dev_u_set),
	CLK_RST_REG(rst_dev_v_set),
	CLK_RST_REG(rst_dev_w_set),
	CLK_RST_REG(rst_dev_x_set),
};

static u32 * const rst_dev_clr_arr[DEV_CONFIG_BLOCKS] = {
	CLK_RST_REG(rst_dev_l_clr),
	CLK_RST_REG(rst_dev_h_clr),
	CLK_RST_REG(rst_dev_u_clr),
	CLK_RST_REG(rst_dev_v_clr),
	CLK_RST_REG(rst_dev_w_clr),
	CLK_RST_REG(rst_dev_x_clr),
};

static void clock_write_regs(u32 * const regs[DEV_CONFIG_BLOCKS],
			     u32 bits[DEV_CONFIG_BLOCKS])
{
	int i = 0;

	for (; i < DEV_CONFIG_BLOCKS; i++)
		if (bits[i])
			writel(bits[i], regs[i]);
}

void clock_enable_regs(u32 bits[DEV_CONFIG_BLOCKS])
{
	clock_write_regs(clk_enb_set_arr, bits);
}

void clock_disable_regs(u32 bits[DEV_CONFIG_BLOCKS])
{
	clock_write_regs(clk_enb_clr_arr, bits);
}

void clock_set_reset_regs(u32 bits[DEV_CONFIG_BLOCKS])
{
	clock_write_regs(rst_dev_set_arr, bits);
}

void clock_clr_reset_regs(u32 bits[DEV_CONFIG_BLOCKS])
{
	clock_write_regs(rst_dev_clr_arr, bits);
}

void clock_enable_clear_reset(u32 l, u32 h, u32 u, u32 v, u32 w, u32 x)
{
	clock_enable(l, h, u, v, w, x);

	/* Give clocks time to stabilize. */
	udelay(IO_STABILIZATION_DELAY);

	clock_clr_reset(l, h, u, v, w, x);
}

static void clock_reset_dev(u32 *setaddr, u32 *clraddr, u32 bit)
{
	writel(bit, setaddr);
	udelay(LOGIC_STABILIZATION_DELAY);
	writel(bit, clraddr);
}

void clock_reset_l(u32 bit)
{
	clock_reset_dev(CLK_RST_REG(rst_dev_l_set), CLK_RST_REG(rst_dev_l_clr),
			bit);
}

void clock_reset_h(u32 bit)
{
	clock_reset_dev(CLK_RST_REG(rst_dev_h_set), CLK_RST_REG(rst_dev_h_clr),
			bit);
}

void clock_reset_u(u32 bit)
{
	clock_reset_dev(CLK_RST_REG(rst_dev_u_set), CLK_RST_REG(rst_dev_u_clr),
			bit);
}

void clock_reset_v(u32 bit)
{
	clock_reset_dev(CLK_RST_REG(rst_dev_v_set), CLK_RST_REG(rst_dev_v_clr),
			bit);
}

void clock_reset_w(u32 bit)
{
	clock_reset_dev(CLK_RST_REG(rst_dev_w_set), CLK_RST_REG(rst_dev_w_clr),
			bit);
}

void clock_reset_x(u32 bit)
{
	clock_reset_dev(CLK_RST_REG(rst_dev_x_set), CLK_RST_REG(rst_dev_x_clr),
			bit);
}
