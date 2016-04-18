AS	=arm-none-eabi-gcc
CC	=arm-none-eabi-gcc
LD	=arm-none-eabi-ld
OBJCOPY	=arm-none-eabi-objcopy

CPU	=-mcpu=cortex-a9
#FPU	=-mfpu=vfpv3
FPU	=-mfpu=neon

AFLAGS	=${FPU} -nostdlib
#AFLAGS	=-mthumb ${CPU} ${FPU} -nostdlib

CFLAGS	+=-march=armv7-a -marm
