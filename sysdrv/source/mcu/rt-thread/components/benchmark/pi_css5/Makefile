# Makefile for pi_fftcs5, Takuyo Ooura's # Pi calculation for *nix or DOS
# Warning: this file has tabs in it.  It is required for GNU Make.
#
# This version may not produce optimal runs.  You'll need to experiment
# with the compiler options.

#Set this to your remove command.
RM= rm -f

#Set this to your copy command.
CP= cp

#Set this to you directory creation command
MKDIR= mkdir -p

#Set this to your strip command.
STRIP= strip

#Set this to the proper math libraries to link against
MATH_LIBS= -lm

#Set this to the proper install directory
PREFIX= /usr/local

#CC = gcc
CC = /home/hexiongjun/SmartAuto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-fsl-linux-gnueabi-gcc

#Set this to best optimization flags for your system. Defaults are for gcc.
CFLAGS = -Wall -pedantic -O -fomit-frame-pointer -funroll-loops  -march=armv7-a -mfpu=neon -mfloat-abi=softfp

#Set this to the cpu to target
#CFLAGS += -march=i686 -malign-double

#If you would like to minimize program output uncomment this line
#CFLAGS += -DQUIET_OUT

#Set this to important linker flags
LDFLAGS = -static

# Leave items below this point unchanged!

all: pi_css5


pi_css5 : pi_fftcs.o fftsg_h.o
	$(CC) $(CFLAGS) pi_fftcs.o fftsg_h.o ${MATH_LIBS} ${LDFLAGS} -o pi_css5

pi_fftcs.o : pi_fftcs.c
	$(CC) $(CFLAGS) -c pi_fftcs.c -o pi_fftcs.o

fftsg_h.o : fftsg_h.c
	$(CC) $(CFLAGS) -c fftsg_h.c -o fftsg_h.o


install: all
	${MKDIR} ${PREFIX}/bin
	${CP} pi_css5 ${PREFIX}/bin

install-strip: strip install

strip: all
	${STRIP} pi_css5

clean:
	${RM} pi_fftcs.o fftsg_h.o pi_css5

