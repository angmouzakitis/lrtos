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

#include <board.h>
#include <interrupt.h>

#define AXI_LED_R	(*(volatile u32 *)0x41000008)
#define led_mask	((u16) 0xE000)

static void
leds_init()
{
	fn_entry
	//AXI_LED_R = 0;
}

static void
buttons_init()
{
}

void
board_init()
{
	fn_entry

	gic_init();
	serial_init(0);
	//perfmon_init();

	//leds_init();
	//buttons_init();

	fn_exit
}


/* hardware platform has to have a gpio controller on leds */
void
set_led(u16 req)
{
	static u16 led_state = 0; /* 0 returned when axi read on gpio contr */

	fn_entry

	switch (req & led_mask) {
	case LED_ON:
		AXI_LED_R = led_state |= BIT(req & 0xff);
		break;
	case LED_OFF:
		AXI_LED_R = led_state &= ~BIT(req & 0xff);
		break;
	case LED_TOOGLE:
		AXI_LED_R = led_state ^= BIT(req & 0xff);
		break;
	}

	fn_exit
}

u8 inline get_buttons() { return (0); }
