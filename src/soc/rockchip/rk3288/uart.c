/*
 * This file is part of the coreboot project.
 *
 * Copyright 2014 Rockchip Inc.
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

#include <uart.h>
#include <arch/io.h>
#include <console/console.h>	/* for __console definition */
#include <stdint.h>
#include <uart8250.h>

struct rk3288_uart {
	union {
		uint32_t thr; /* Transmit holding register. */
		uint32_t rbr; /* Receive buffer register. */
		uint32_t dll; /* Divisor latch lsb. */
	};
	union {
		uint32_t ier; /* Interrupt enable register. */
		uint32_t dlm; /* Divisor latch msb. */
	};
	union {
		uint32_t iir; /* Interrupt identification register. */
		uint32_t fcr; /* FIFO control register. */
	};
	uint32_t lcr; /* Line control register. */
	uint32_t mcr; /* Modem control register. */
	uint32_t lsr; /* Line status register. */
	uint32_t msr; /* Modem status register. */
	uint32_t scr;
	uint32_t reserved1[(0x30 - 0x20) / 4];
	uint32_t srbr[(0x70 - 0x30) / 4];
	uint32_t far;
	uint32_t tfr;
	uint32_t rfw;
	uint32_t usr;
	uint32_t tfl;
	uint32_t rfl;
	uint32_t srr;
	uint32_t srts;
	uint32_t sbcr;
	uint32_t sdmam;
	uint32_t sfe;
	uint32_t srt;
	uint32_t stet;
	uint32_t htx;
	uint32_t dmasa;
	uint32_t reserver2[(0xf4 - 0xac) / 4];
	uint32_t cpr;
	uint32_t ucv;
	uint32_t ctr;
} __attribute__ ((packed));

static struct rk3288_uart * const uart_ptr =
	(void *)CONFIG_CONSOLE_SERIAL_UART_ADDRESS;

static void rk3288_uart_tx_flush(void);
static int rk3288_uart_tst_byte(void);

static void rk3288_uart_init(void)
{
	// Use a hardcoded divisor for now.
	const unsigned divisor = 13;
	const uint8_t line_config = UART8250_LCR_WLS_8; // 8n1

	rk3288_uart_tx_flush();

	// Disable interrupts.
	writel(0, &uart_ptr->ier);
	// Force DTR and RTS to high.
	writel(UART8250_MCR_DTR | UART8250_MCR_RTS, &uart_ptr->mcr);
	// Set line configuration, access divisor latches.
	writel(UART8250_LCR_DLAB | line_config, &uart_ptr->lcr);
	// Set the divisor.
	writel(divisor & 0xff, &uart_ptr->dll);
	writel((divisor >> 8) & 0xff, &uart_ptr->dlm);
	// Hide the divisor latches.
	writel(line_config, &uart_ptr->lcr);
	// Enable FIFOs, and clear receive and transmit.
	writel(UART8250_FCR_FIFO_EN |
		UART8250_FCR_CLEAR_RCVR |
		UART8250_FCR_CLEAR_XMIT, &uart_ptr->fcr);
}

static void rk3288_uart_tx_byte(unsigned char data)
{
	while (!(readl(&uart_ptr->lsr) & UART8250_LSR_THRE));
	writel(data, &uart_ptr->thr);
}

static void rk3288_uart_tx_flush(void)
{
	while (!(readl(&uart_ptr->lsr) & UART8250_LSR_TEMT));
}

static unsigned char rk3288_uart_rx_byte(void)
{
	if (!rk3288_uart_tst_byte())
		return 0;
	return readl(&uart_ptr->rbr);
}

static int rk3288_uart_tst_byte(void)
{
	return (readl(&uart_ptr->lsr) & UART8250_LSR_DR) == UART8250_LSR_DR;
}

#if !defined(__PRE_RAM__)

static const struct console_driver rk3288_uart_console __console = {
	.init     = rk3288_uart_init,
	.tx_byte  = rk3288_uart_tx_byte,
	.tx_flush = rk3288_uart_tx_flush,
	.rx_byte  = rk3288_uart_rx_byte,
	.tst_byte = rk3288_uart_tst_byte,
};

uint32_t uartmem_getbaseaddr(void)
{
	return (uintptr_t)uart_ptr;
}

#else

void uart_init(void)
{
	rk3288_uart_init();
}

void uart_tx_byte(unsigned char data)
{
	rk3288_uart_tx_byte(data);
}

void uart_tx_flush(void)
{
	rk3288_uart_tx_flush();
}

unsigned char uart_rx_byte(void)
{
	return rk3288_uart_rx_byte();
}

#endif
