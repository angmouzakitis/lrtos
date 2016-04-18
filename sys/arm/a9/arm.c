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

/* Varius helper routines called from .S files */

#include <types.h>

void
a9_print_state()
{
	u32 cpsr;
	u8 mode = 0;
	char *modes[] = { "user", "fiq", "irq", "svc", "monitor", "abort",
			  "hyp", "undef", "system" };

	__asm volatile("mrs %0, cpsr\n\t" : "=r" (cpsr));

	if ((cpsr & 0x1f) == 16)
		mode = 0;
	else if ((cpsr & 0x1f) == 17)
		mode = 1;
	else if ((cpsr & 0x1f) == 18)
		mode = 2;
	else if ((cpsr & 0x1f) == 19)
		mode = 3;
	else if ((cpsr & 0x1f) == 22)
		mode = 4;
	else if ((cpsr & 0x1f) == 23)
		mode = 5;
	else if ((cpsr & 0x1f) == 26)
		mode = 6;
	else if ((cpsr & 0x1f) == 27)
		mode = 7;
	else if ((cpsr & 0x1f) == 31)
		mode = 8;

	printf("mode is %s\n\r", modes[mode]);
}
