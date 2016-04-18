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
#include <kern/thrd.h>
#include <kern/ring.h>

struct utils {
	char *name;
	void (*fn)();
} const utils[];

static void
intepretter(char c)
{
	static char line[128];
	static u8 len;
	const struct utils *util;
	char *args;

	line[len] = c;
	
	if (c == '\b') {
		line[len] = 0;
		if (len)
			line[--len] = 0;
		printf("\b \b");		  /* clear previus character */
		goto out2;
	}

	if (c  == 0x3) {
		printf("^C\r\n");
		goto out;
	}

	if (c  == 0x4) {
		printf("^D\r\n");
		goto out;
	}

	if (c  == '\r') {
		printf("\r\n");
		line[len] = 0;

		args = strchrz(line, ' ');		    /* point to args */
		
		for (util = utils; util->fn; util++)
			if (!strcmp(line, util->name)) {
				util->fn(args);
				break;
			}

		if (!util->name)
			printf("command [%s] not found\r\n", line);

out:
		printf("\\% ");
		len = 0;
		return;
	}

	serial_putc(NULL, c);
	len++;
out2:	;
}

void __noreturn
ui()
{
	int ch;

	printf("\\% ");
	while (1) {
		if ((ch = serial_getc(0)) != -1)
				intepretter(ch);
	}
}

void
help()
{
	for (const struct utils *u = utils; u->fn; u++)
		printf("[%s], ", u->name);
	printf("\b\b \r\n");
}


/*
 * similar to strtol() with base = 16
 * delimiter null or space char
 */
char *
atoh(char *line)
{
	void *tohex = 0;
       	u8 ascii_off, len;

	for (len = 0; (line[len] != ' ') && line[len]; len++);

	while (*line) {
		if (*line == ' ')
			break;
		ascii_off = *line <= '9' ? '0' : 0x57;
		tohex += ((*line++ - ascii_off) << (--len * 4));
	}

	return tohex;
}

/*
 * gotox{0,1,2} : set $pc to @addr
 * helpers for x0 when called from shell
 * naked : calle routine's 'bx lr' return to the funcition call x{0,1,2}()
 */
void __attribute__((naked)) 
gotox0(void *a)
{
	__asm volatile("bx r0");
}

void __attribute__((naked))
gotox1(void *a, void *b)
{
	__asm volatile("bx r1");
}

void __attribute__((naked))
gotox2(void *a, void *b, void *c)
{
	__asm volatile("bx r2");
}

void 
x0(char *line)
{
	gotox0((void *)atoh(line));	/* function entry */
}

void 
x1(char *line)
{
	gotox1(atoh(strchr(line, ' ') + 1),	/* 1st argument */
	      (void *)atoh(line));		/* function entry */	
}

void 
x2(char *line)
{
	gotox2(atoh(strchr(line, ' ') + 1),		     /* 1st argument */
	       atoh(strchr(strchr(line, ' ') + 1, ' ') + 1), /* 2nd argument */
	       atoh(line));				  /* funcition entry */
}

void
top()
{
	u8 spaces;

	printf("NAME\t\tID\tADDRESS\tSP\r\n");
	for (u8 i = 0; i <= thrdnum; i++) {
		printf("[%s]", thrdlist[i].td_name);
		spaces = strlen(thrdlist[i].td_name);
		while (spaces++ < 15) /* fix indedation */
			serial_putc(NULL, ' ');
		printf("%x\t0x%x\t0x%x\r\n", i, 0, thrdlist[i].sp);
	}

}

void __attribute__ ((naked))
do_svc() 
{
	__asm volatile("push	{lr}	\t\n"
		       "svc	#1	\t\n"
		       "pop	{lr}	\t\n");
}

#define DEFINE_UTIL(_x) { .name = #_x, .fn = _x, }
const struct utils utils[] = {
	DEFINE_UTIL(help),
	DEFINE_UTIL(top),
	DEFINE_UTIL(ui),
	DEFINE_UTIL(gotox0),
	DEFINE_UTIL(gotox1),
	DEFINE_UTIL(gotox2),
	DEFINE_UTIL(do_svc),
	{ }
};
