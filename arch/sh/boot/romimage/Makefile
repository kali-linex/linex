# SPDX-License-Identifier: GPL-2.0
#
# linex/arch/sh/boot/romimage/Makefile
#
# create an romImage file suitable for burning to flash/mmc from zImage
#

targets		:= vmlinex head.o zeropage.bin piggy.o
load-y		:= 0

mmcif-load-$(CONFIG_CPU_SUBTYPE_SH7724)	:= 0xe5200000 # ILRAM
mmcif-obj-$(CONFIG_CPU_SUBTYPE_SH7724)	:= $(obj)/mmcif-sh7724.o
load-$(CONFIG_ROMIMAGE_MMCIF)		:= $(mmcif-load-y)
obj-$(CONFIG_ROMIMAGE_MMCIF)		:= $(mmcif-obj-y)

LDFLAGS_vmlinex := --oformat $(ld-bfd) -Ttext $(load-y) -e romstart \
		   -T $(obj)/../../kernel/vmlinex.lds

$(obj)/vmlinex: $(obj)/head.o $(obj-y) $(obj)/piggy.o FORCE
	$(call if_changed,ld)

OBJCOPYFLAGS += -j .empty_zero_page

$(obj)/zeropage.bin: vmlinex FORCE
	$(call if_changed,objcopy)

LDFLAGS_piggy.o := -r --format binary --oformat $(ld-bfd) -T

$(obj)/piggy.o: $(obj)/vmlinex.scr $(obj)/zeropage.bin arch/sh/boot/zImage FORCE
	$(call if_changed,ld)
