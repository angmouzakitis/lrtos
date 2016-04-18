SOC	=SOC_ZYNQ
BOARD	=BOARD_ZEDBOARD

OUTPUT	=a.out

include conf/config_chooser.mk

%.o:%.S
	$(AS) $(CPU) $(AFLAGS) -D${SOC} -D${BOARD} $(INCLUDEDIR) -c -o ${@} ${<}
%.o:%.c
	$(CC) $(CPU) $(FPU) $(CFLAGS) -D${SOC} -D${BOARD} $(INCLUDEDIR) -c -o ${@} ${<}

#CFLAGS +=-fno-inline

CFLAGS +=-std=c11
CFLAGS +=-Os
CFLAGS +=-Wall
CFLAGS +=-Wno-unused-function
CFLAGS +=-Wno-unused-variable
#CFLAGS +=-Wno-empty-body
#CFLAGS +=-Wno-unused-but-set-variable
CFLAGS +=-fno-builtin

#CFLAGS +=-DNO_PRINTF
#AFLAGS +=-DNO_PRINTF

INCLUDEDIR +=-Isys
INCLUDEDIR +=-Isys/sys

all: ${OUTPUT}.elf

c: 	clean
clean:
	@find . -iname "*.o" | while read line ; do rm -rf $$line ; done
	@rm -rf ${OUTPUT}.{elf,bin,hex}

r: 	c all

o:
	@arm-none-eabi-objdump -D ${OUTPUT}.elf | vim -


include conf/config_objects.mk

${OUTPUT}.elf: sys/drivers/serial.o

${OUTPUT}.elf: sys/kern/init.o
${OUTPUT}.elf: sys/kern/ring.o
${OUTPUT}.elf: sys/kern/thrd.o

${OUTPUT}.elf: lib/libc/string.o

### target application ###
${OUTPUT}.elf: main.o
