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

#ifndef __ARM_A9_THRD
#define __ARM_A9_THRD

#include <types.h>

#define THRD_SIZE 1024
#define THRD_STACK_WORDS_SIZE (THRD_SIZE / 4 - 3)

struct thrd {
	u32 sp;
	u32 yieldfor;
	char *td_name;
	u32 td_stack[THRD_STACK_WORDS_SIZE];
} volatile thrdlist[MAX_THREADS];

COMPILE_ASSERT(sizeof(struct thrd) == THRD_SIZE);

static void
__thrd_add(void (*fnentry)())
{
	struct thrd volatile *new = &thrdlist[thrdnum];
	
	/* set the stack pointer at the start of next struct - 14 words
	 *  for saved registers on svc handler push{r0-r12, lr} */
	new->sp = (int) &(new->td_stack[THRD_STACK_WORDS_SIZE -1 - 13]);
	new->td_stack[THRD_STACK_WORDS_SIZE - 1] = (int) fnentry;
}

static u32
__thrd_switch()
{

	thrdcur++;
	if (thrdcur == (thrdnum + 1))
		thrdcur = 0;

	return thrdlist[thrdcur].sp;
}


#endif /* __ARM_A9_THRD */
