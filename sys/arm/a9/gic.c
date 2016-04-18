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

#include "gic.h"

/* (primecell pl390) GIC v1 <ARM IHI 0048A> */
struct {
	u32 icddcr;             /* 0x000 */
	u32 const icdictr;	/* 0x004 */
	u32 const icdiidr;      /* 0x008 */
        u32 const pad0[29];
        u32 icdisr[32];         /* 0x080 */
        u32 icdiser[32];        /* 0x100 */
        u32 icdicer[32];        /* 0x180 */
        u32 icdispr[32];        /* 0x200 */
        u32 icdicpr[32];        /* 0x280 */
        u32 icdabr[32];         /* 0x300 */
        u32 const pad1[32];
        u32 icdipr[24];         /* 0x400 */
        u32 const pad2[232];
        u32 icdiptr[24];        /* 0x800 */
        u32 const pad3[232];
        u32 icdicfr[6];         /* 0xc00 */
        u32 const pad4[58];
        u32 ppi_status;         /* 0xd00 */
        u32 spi_status[3];      /* 0xd04 */
        u32 const pad5[124];
        u32 icdsgir;            /* 0xf00 */
        u32 const pad6[51];
        u32 icpidr4;            /* 0xfd0 */
        u32 icpidr5;            /* 0xfd4 */
        u32 icpidr6;            /* 0xfd8 */
        u32 icpidr7;            /* 0xfdc */
        u32 icpidr0;            /* 0xfe0 */
        u32 icpidr1;            /* 0xfe4 */
        u32 icpidr2;            /* 0xfe8 */
        u32 icpidr3;            /* 0xfec */
        u32 iccidr[4];          /* 0xff0 */
} volatile *dist = (void *)0xf8f01000; // TODO get the address from Makefile

struct {
        u32 iccicr;             /* 0x000 */
        u32 iccpmr;             /* 0x004 */
        u32 iccbpr;             /* 0x008 */
	u32 icciar;		/* 0x00c */
	u32 icceoir;		/* 0x010 */
        u32 iccrpr;             /* 0x014 */
        u32 icchpir;            /* 0x018 */
        u32 iccabpr;            /* 0x01c */
        u32 const pad0[55];
        u32 iccidr;             /* 0x0fc */
} volatile *cpu = (void *)0xf8f00100; // TODO get the address from Makefile

u8 mode_svc_stack[1024] __attribute__ ((aligned(4096)));

#define HANDLER_VECTOR_TABLE_LEN 128
static int (*handler_vector_table[HANDLER_VECTOR_TABLE_LEN])();

int __attribute__ ((noinline))
handler_dfl()
{
       	printf("unhandled irqerrupt\n\r");
	return (-1);
}

void
gic_set_dfl_handler(u16 irq_id)
{
	handler_vector_table[irq_id] = handler_dfl;
}

void
gic_set_handler(u16 irq_id, void *handler)
{
	handler_vector_table[irq_id] = handler;
	printf("%s(): irq #%d at 0x%x\r\n", __func__, irq_id, handler);
}

void
gic_set_irq(u16 irq_id)
{
	dist->icdiser[irq_id / 32] |= BIT(irq_id % 32);
}

void
gic_clear_irq(u16 irq_id)
{
	dist->icdicer[irq_id / 32] |= BIT(irq_id % 32);
}

static u8 volatile irq_complete;

static void __attribute__((noinline)) 
handler_test_ipi()
{
	irq_complete = 1;
	printf("gic_init ok, IPI complete\n\r");
}

#define DIST_CR_ENABLE BIT(0)
#define CPU_CR_ENABLE BIT(0)

void
gic_init()
{
	fn_entry

	__asm volatile("cpsid ifa");

	/* register handler_vector_table with default_unhandled() */
	for (int i = 0; i < 128; i++)
		handler_vector_table[i] = handler_dfl;

	reg_write(dist->icddcr, DIST_CR_ENABLE, 1);
	cpu->iccicr = 0;

	for(int i = 32; i<95; i += 16)
		dist->icdicfr[i / 16] = 0;

	for(int i = 0; i < 95; i += 4)
		dist->icdipr[i / 4] = 0x05050505;

	for(int i = 32; i < 95; i += 4)
		dist->icdiptr[i / 4] = 0x01010101; /* target first processor */

	for(int i = 0; i < 95; i += 32)
		dist->icdicer[i / 32] = 0xffffffff;
	
	reg_write(dist->icddcr, CPU_CR_ENABLE, 1);

	cpu->iccpmr = 0xf0;
	cpu->iccicr = 0x01;

	dist->icdiser[0] = 0xffff;

	__asm volatile("cpsie ifa");

	/* test gic functionality */
	gic_set_handler(1, handler_test_ipi);
	printf("%s testing IPI...\n\r", __func__);
	dist->icdsgir = 0x02010001;
	while (!irq_complete);

	fn_exit
}

void
gic_prefetch_abort_handler()
{
	u32 irq_id, cpuid, __iar;

	__iar = cpu->icciar;
	irq_id = __iar & BITS(9, 0);
	cpuid = __iar & BITS(12, 10);

	printf("%s FROM CPU%d id %d\n\r", __func__, cpuid, irq_id);
	w
}

void
gic_data_abort_handler()
{
	u32 irqid, cpuid,__iar;

	__iar = cpu->icciar;
	irqid = __iar & BITS(9, 0);
	cpuid = __iar & BITS(12, 10);

	printf("%s FROM CPU%d id %d\n\r", __func__, cpuid, irqid);
	w

}

void gic_ispr(int id)
{
	dist->icdispr[id/32] |= BIT(id %32);
}

void
gic_irq_handler()
{
	u32 irqid, cpuid, reg;

	reg = cpu->icciar;

	irqid = reg & BITS(9, 0);
	cpuid = reg & BITS(12, 10);
	
	printf("%s() got irq #%d on cpu%d\r\n", __func__, irqid, cpuid);

	handler_vector_table[irqid]();
	cpu->icceoir = irqid;
}

void
gic_svc_handler()
{
	printf("svn called\n\r");
}

#if 0
	if (irq_id == 1023) {
		printf("spurius irq on CPU%d\n\r", cpuid);
		return;
	}


	printf("irq FROM CPU%d id %d\n\r", cpuid, irq_id);

	if (handler_vector_table[irq_id]() == -1) {
		printf("unhandled interrupt\r\n");
	} else {
		printf("irq handled OK");
	}

	cpu->icceoir = (cpuid << 9) | irq_id;
}
#endif
