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

.text
.section .vector_base_addr

#include "../../sys/arm.h"

vector_base_address: @ non secure
	b	reset_handler
	b	undefined_handler
	b	svc_handler
	b	prefetch_abort_handler
	b	data_abort_handler
	b	NOT_USED
	b	irq_handler
	b	fiq_handler

irq_handler:
	push	{r0-r12, lr}
	bl	gic_irq_handler
	pop	{r0-r12, lr}
	subs	pc, lr, #4
fiq_handler:
	mov	r0, #0
	ldr	r1, =str_fiq
	bl	serial_printf
	b	panic
undefined_handler:
	mov	r0, #0
	ldr	r1, =str_undef
	bl	serial_printf
	b	panic
svc_handler:
	nop
	str	lr, saved_lr
	cpsid	ifa, #MODE_SYS
	ldr	lr, saved_lr
	push	{r0-r12, lr}

	ldr	r0, =thrdlist		@ write new sp back to thrdlist
	ldr	r1, =thrdcur
	ldr	r1, [r1]
	add	r0, r0, r1, lsl #10
	str	sp, [r0] 

	
	bl	thrd_switch 
	mov	sp, r0	@ set the new stack pointer

	cpsie	ifa, #MODE_SYS
	pop	{r0-r12, lr}
	subs	pc, lr, #0
saved_lr: .word 0

prefetch_abort_handler:
	b prefetch_abort_handler
	mov r0, #0
	ldr r1, =str_prefetchabort
	bl	serial_printf
data_abort_handler:
	mov r0, #0
	ldr r1, =str_dataabort
	bl	serial_printf
	b	panic
NOT_USED:
	ldr r1, =str_notused
	bl serial_printf
	b	panic
panic:
	b panic

reset_handler:
	mrc	p15, 0, r5, c0, c0, 5
	cpsie	ifa

	and	r5, r5, #0xF
	cmp	r5, #0
	bne	noncpu0_wait

	movw r1, #:lower16:_sbss
	movt r1, #:upper16:_sbss
	movw r2, #:lower16:_ebss
	movt r2, #:upper16:_ebss

	sub r2, r2, r1
	mov r3, #0
bssloop:
	subs r2, r2, #1
	strb r3, [r1], #1
	bne bssloop

	mov	r0, #0
	mcr	p15, 0, r0, c12, c0, 1	/* write vbar */

	ldr	sp, =mode_svc_stack + 1024 @ sizeof svc stack

	@ set irq stack pointer
	cps	#MODE_IRQ
	movw	sp, 0x0000
	movt	sp, 0x0110

        @ goto MODE_SYS and set the stack
	cps	#MODE_SYS
	ldr	r0, =thrdlist
	add	sp, r0, #1024
	str	sp, [r0]

	@mov	sp, r0
	@movw	sp, 0x0000
	@movt	sp, 0x0100

	ldr r1, =str_reset
	bl serial_printf

	b kern_start

str_irq: .asciz "\r\nirq handler\n\r"
str_fiq: .asciz "\r\nfiq handler\n\r"
str_undef: .asciz "\r\nundef handler\r\n"
str_svc: .asciz "\r\nsvc handler\r\n"
str_prefetchabort: .asciz "\r\nprefetch abort handler\r\n"
str_dataabort: .asciz "\r\ndataabort handler\r\n"
str_reset: .asciz "\r\nreset\r\n"
str_notused: .asciz "\r\nnot used handler\r\n"

.align 2

noncpu0_wait:
	wfe
	b	noncpu0_wait
