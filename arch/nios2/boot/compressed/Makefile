# SPDX-License-Identifier: GPL-2.0
#
# create a compressed vmlinex image from the original vmlinex
#

targets		:= vmlinex head.o misc.o piggy.o vmlinex.lds
asflags-y	:=

OBJECTS = $(obj)/head.o $(obj)/misc.o

LDFLAGS_vmlinex := -T

$(obj)/vmlinex: $(obj)/vmlinex.lds $(OBJECTS) $(obj)/piggy.o FORCE
	$(call if_changed,ld)

LDFLAGS_piggy.o := -r --format binary --oformat elf32-littlenios2 -T

$(obj)/piggy.o: $(obj)/vmlinex.scr $(obj)/../vmlinex.gz FORCE
	$(call if_changed,ld)
