# Teensyduino Core Library
# http://www.pjrc.com/teensy/
# Copyright (c) 2017 PJRC.COM, LLC.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# 1. The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# 2. If the Software is incorporated into a build system that allows
# selection among a list of target devices, then similar target
# devices manufactured by PJRC.COM must be included in the list of
# target devices and selectable in the same manner.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# set your MCU type here, or make command line `make MCU=MK20DX256`
#MCU=MK20DX256
MCU=MKL26Z64
#MCU=MK64FX512
#MCU=MK66FX1M0

# make it lower case
LOWER_MCU := $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(MCU)))))))))))))))))))))))))))
MCU_LD = ../buildroot/$(LOWER_MCU).ld

# The name of your project (used to name the compiled .hex file)
TARGET = main

# configurable options
OPTIONS = -DF_CPU=48000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE

# options needed by many Arduino libraries to configure for Teensy 3.x
OPTIONS += -D__$(MCU)__ -DARDUINO=10805 -DTEENSYDUINO=144

# use "cortex-m4" for Teensy 3.x
# use "cortex-m0plus" for Teensy LC
#CPUARCH = cortex-m4
CPUARCH = cortex-m0plus

#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

COMPILERPATH = /usr/local/bin

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -Os -mcpu=$(CPUARCH) -mthumb -MMD $(OPTIONS) -I. -I../buildroot/

# compiler options for C++ only
CXXFLAGS = -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 --specs=nano.specs -mcpu=$(CPUARCH) -mthumb -T$(MCU_LD)

# additional libraries to link
LIBS = -lm 

# names for the compiler programs
CC = $(COMPILERPATH)/arm-none-eabi-gcc
CXX = $(COMPILERPATH)/arm-none-eabi-g++
OBJCOPY = $(COMPILERPATH)/arm-none-eabi-objcopy
SIZE = $(COMPILERPATH)/arm-none-eabi-size

# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
C_FILES := $(wildcard *.c) $(wildcard ../buildroot/*.c)
CPP_FILES := $(wildcard *.cpp) $(wildcard ../buildroot/*.cpp)
OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)


# the actual makefile rules (all .o files built by GNU make's default implicit rules)

all: install

$(TARGET).elf: $(OBJS) $(MCU_LD)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

install: $(TARGET).hex
	$(COMPILERPATH)/teensy_loader_cli --mcu=$(MCU) -w -v $<

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d $(TARGET).elf $(TARGET).hex
