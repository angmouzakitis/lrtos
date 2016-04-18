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

#include <types.h>

#include "string.h"

static const unsigned char xdigs[] = "0123456789abcdef";

#ifndef NO_PRINTF
void
vsprintf(char *str, char *fmt, va_list ap)
{
	u8 buf[14], i;
	u32 n;

	do {
		while (*fmt != '%') {
			if (*fmt == 0x5c) /* '\' escape character */
				fmt++;
			if (!(*str++ = *fmt++)) /* actual text copy */
				return;
		}

		switch (*++fmt) {
		case 'c' :
			buf[0] = va_arg(ap, int);
			*str++ = buf[0];
			break;
		case 's' :
			for (char *c = va_arg(ap, char *); *c; *str++ = *c++);
			break;
		case 'd' :
			n = va_arg(ap, u32);
			if (!n) {
				*str++ = '0';
				break;
			}

			memset(buf, 0, sizeof(buf));
			for (i = 13; (buf[i] = n % 10) || (n > 9); n /= 10)
				buf[i--] += '0'; 
			while (*str++ = buf[++i], i < 13);
			break;
		case 'x' :
			n = va_arg(ap, u32);
			if (!n) {
				*str++ = '0';
				break;
			}
#define nibble(i, n) (n >> ((7 - i) * 4)) & 0xf
			for (i = 8; i--; buf[i] = xdigs[nibble(i, n)]);
			while (buf[++i] == '0');
			for (buf[8] = 0; i != 8; *str++ = buf[i++]);
			break;
		}
	} while (*fmt++);
}

void
sprintf(char *str, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);
}
#else
void vsprintf(char *str, char *fmt, va_list ap);
void sprintf(char *str, char *fmt, ...);
#endif

int
strcmp(char *s1, char *s2)
{
	while ((*s1++ == *s2++) && ((*s2) || (*s1)));
	return *--s1 - *--s2;
}

unsigned
strlen(char *s)
{
	unsigned ret = 0;

	for (; *s++; ret++);
	return ret;
}

void *
memset(void *s, int c, int n)
{
	u8 *ref = (u8 *) s;

	do
		*ref++ = c;
	while (--n);
	return s;
}

char *
strchr(char *s, int c)
{
	while (*s++)
		if (*s == c) 
			return s;
	return NULL;
}

char *
memchr(char *s, int c, u32 n)
{
	while (*s++ && --n)
		if (*s == c) 
			return (s);
	return NULL;
}

char *
rawmemchr(char *s, int c)
{
	while (*s++ != c);
	return s - 1;
}

/*
 * Non string.h function; Find the first occurance of byte @c,
 *  place '\0' and return the next address.
 */
char *
strchrz(char *s, char c)
{
	while (*s != c && *s)
		s++;
	*s = 0;
	return s + 1;
}
