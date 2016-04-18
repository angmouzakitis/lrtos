/*
 * Copyright (c) 2016, Angelos Mouzakitis <angmouzakitis@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <soc.h>
#include <drivers/serial.h>

#define UART_CTL_RX_RES		BIT(0)
#define UART_CTL_TX_RES		BIT(1)
#define UART_CTL_RX_ENABLE	BIT(2)
#define UART_CTL_RX_DISABLE	BIT(3)
#define UART_CTL_TX_ENABLE	BIT(4)
#define UART_CTL_TX_DISABLE	BIT(5)
#define UART_CTL_RSTTO		BIT(6)
#define UART_CTL_STTBRK		BIT(7)
#define UART_CTL_STPBRK		BIT(8)

#define UART_INT_RXTRIG		BIT(0)
#define UART_INT_RXEMPTY	BIT(1)
#define UART_INT_RXFULL		BIT(2)
#define UART_INT_TXEMPTY	BIT(3)
#define UART_INT_TXFULL		BIT(4)
#define UART_INT_RXOVER		BIT(5)
#define UART_INT_FRAME		BIT(6)
#define UART_INT_PARITY		BIT(7)
#define UART_INT_TIMEOUT	BIT(8)
#define UART_INT_DMSI		BIT(9)
#define UART_INT_TTRIG		BIT(10)
#define UART_INT_TNFUL		BIT(11)
#define UART_INT_TOVR		BIT(12)

#define UART_CHSTS_RXTRIG	BIT(0)
#define UART_CHSTS_RXEMPTY	BIT(1)
#define UART_CHSTS_RXFULL	BIT(2)
#define UART_CHSTS_TXEMPTY	BIT(3)
#define UART_CHSTS_TXFULL	BIT(4)
#define UART_CHSTS_RXOVER	BIT(5)
#define UART_CHSTS_FRAME	BIT(6)
#define UART_CHSTS_PARITY	BIT(7)
#define UART_CHSTS_TIMEOUT	BIT(8)
#define UART_CHSTS_DMSI		BIT(9)
#define UART_CHSTS_RXACTIVE	BIT(10)
#define UART_CHSTS_TXACTIVE	BIT(11)
#define UART_CHSTS_FDELT	BIT(12)
#define UART_CHSTS_TTRIG	BIT(13)
#define UART_CHSTS_TNFUL	BIT(14)

struct uart_module {
	u32 control;			/* 0x0 */
        u32 mode;			/* 0x4 */
	u32 ier;			/* 0x8 */
	u32 idr;			/* 0xc */
        u32 const imr;			/* 0x10 */
        u32 cisr;			/* 0x14 */
        u32 baud_rate_gen;		/* 0x18 */
        u32 rcvr_timeout;		/* 0x1c */
        u32 rcvr_fifo_trigger_level;	/* 0x20 */
        u32 modem_ctrl;			/* 0x24 */
        u32 const modem_sts;		/* 0x28 */
	u32 const channel_sts;		/* 0x2c */
        u32 tx_rx_fifo;			/* 0x30 */
        u32 baud_rate_divider;		/* 0x34 */
        u32 flow_delay;			/* 0x38 */
        u32 ir_min_rcv_pulse_wdth;	/* 0x3c */
        u32 ir_transmitted_pulse_wdth;	/* 0x40 */
        u32 tx_fifo_trigger_level;	/* 0x44 */
} volatile *uart_module[] = {
	(void *)0xe0000000,
	(void *)0xe0001000
};

u32 irq_mask;

static void __attribute__ ((noinline))
__flush_rx_fifo()
{
	for (volatile int i = 0; i < 128; i++)
		uart_module[1]->tx_rx_fifo;
}

#if 0
int
hnd()
{
	
	reg_write(uart_module[1]->cisr, irq_mask, 1);
	return 0;
}
#endif


void
serial_init(struct dev_serial *dev)
{

	/* post_config init serial for now */
#if 0
	irq_mask = UART_INT_TIMEOUT; /* uart timeout interrupt */
	uart_module[1]->ier = irq_mask;
	uart_module[1]->idr = ~irq_mask;
	reg_set(uart_module[1]->mode, BITS(9, 8), 0);

	//gic_register_handler(82, hnd);
	//gic_set_irq(82);

	__flush_rx_fifo();
#endif

}

void
serial_putc(struct dev_serial *dev, u8 ch)
{
	while (uart_module[1]->channel_sts & UART_CHSTS_TXFULL);
	uart_module[1]->tx_rx_fifo = ch;
}

int
serial_getc(struct dev_serial *dev)
{
	while ((uart_module[1]->channel_sts & UART_CHSTS_RXEMPTY));
	return  uart_module[1]->tx_rx_fifo;
}
