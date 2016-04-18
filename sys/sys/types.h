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

#ifndef __TYPES
#define __TYPES

#define NULL ((void *)0)
#define BIT(x)	(1UL << (x))
#define BITS(high, low) \
	(((1 << ((high) + 1 - (low))) - 1) << (low))
#define reg_set(var, mask, val)		\
	(var) = ((var) & ~(mask)) | ((val) << (__builtin_ffs((mask)) - 1));
#define reg_write(var, mask, val)	\
	(var) = (val) << (__builtin_ffs((mask)) - 1);
#define reg_clear(var, mask) (var) &= ~(mask);

#ifdef SOC_ZYNQ
 #define __noreturn
 #define __naked
#else
 #define __noreturn __attribute__((noreturn))
 #define __naked __attribute__((naked))
#endif /* SOC_ZYNQ */

typedef unsigned char		u8;
typedef unsigned short int	u16;
typedef unsigned int		u32;
typedef char			s8;
typedef short int		s16;
typedef int			s32;

typedef __builtin_va_list va_list;
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_start(a, f) __builtin_va_start(a, f)
#define va_end(a) __builtin_va_end(a)
#define offsetof(s, a) __builtin_offsetof(s, a)

#define COMPILE_ASSERT(expr) typedef int __COMPILE_ASSERT__[(expr) ? 1 : -1]

#define udelay(u) for (volatile int i = 0; i < (u * 1000); i++)
#include <dev_structs.h>
#include <drivers/serial.h> /* XXX for printf everywhere */
#define px(x) printf("%x\n\r", x)
#define w while(1) { __asm ("nop"); }


#define fn_entry	printf("%s() start\r\n", __func__);
#define fn_exit		printf("%s() end\r\n", __func__);
#define asm __asm volatile
#define BR(x) __asm volatile(".func\n "x":\n .endfunc\n")
#define BR_STR(x) __asm volatile(".func\n "x":\n")
#define BR_END() __asm volatile(".endfunc\n")
#define ASM_ISOLATE(x, c)	\
	BR_STR(x);		\
	c;			\
	BR_END();

#define MAX_THREADS 16

#endif /* __TYPES */
