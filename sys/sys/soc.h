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

#ifndef __SOC
#define __SOC

#include <types.h>

#ifdef SOC_TI_TM4C1294NCPDT
 #include <arm/m4f/tm4c1294/tm4c1294.h>
 #include <arm/m4f/tm4c1294/isr.h>
 #include <arm/m4f/core_peripheral.h>
 #define SOC_TIMERS_AVAILABLE	8
 #include <drivers/serial.h>
 struct dev_serial uart[8];
#endif /* SOC_TI_TM4C1294NCPDT */

#ifdef SOC_ZYNQ
 struct dev_serial uart[2];
#endif /* SOC_ZYNQ */

#ifdef SOC_MK20DX256
 #include <arm/m4/mk20dx256/mk20dx128.h>
 #include <arm/m4f/core_peripheral.h>
 struct dev_serial uart[3];
#endif /* SOC_MK20DX256 */

#endif /* __SOC */
