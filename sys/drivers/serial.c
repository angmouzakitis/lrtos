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

static char strbuf[64];

void
serial_puts(struct dev_serial *dev, char *s)
{
	do
		serial_putc(dev, *s);
	while (*++s);
}

#ifndef NO_PRINTF
void
serial_printf(struct dev_serial *dev, char *fmt, ...)
{
	va_list ap;

	if (!*fmt) { 
		serial_puts(dev, "\r\nNULL passed to serial_printf()\n\r");
		return;
	}

	va_start(ap, fmt);
	vsprintf(strbuf, fmt, ap);
	va_end(ap);

	if (!(*strbuf)) {
		serial_puts(dev, "\r\nNULL passed as strbuf\r\n");
		return;
	}

	serial_puts(dev, strbuf);
}
#else
void serial_printf(struct dev_serial *dev, char *fmt, ...) {}
#endif
