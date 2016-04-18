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

#ifndef __SYS_DEV_STRUCTS
#define __SYS_DEV_STRUCTS

#define RING_LEN 8

struct ring {
	u32 cpos;
	u32 ppos;
	u8 buf[RING_LEN];
	u32 const __null_padding;
};

struct dev_serial {
	u8 id;
	struct ring rx_ring;
};

struct dev_i2c {
	u8 id;
	int tmp;
};

enum GPIO_STATES {
	GPIO_LOW,
	GPIO_HIGH,
};

enum GPIO_DIRECITON {
	GPIO_INPUT,
	GPIO_OUTPUT,
};

struct dev_gpioc {
	;
};

struct gpio {
	u8 module;
	u8 pin		: 6;
	u8 state	: 1;
	u8 dir		: 1;
};

#endif /* __SYS_DEV_STRUCTS */
