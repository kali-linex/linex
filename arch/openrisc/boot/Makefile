# SPDX-License-Identifier: GPL-2.0
#
# Makefile for bootable kernel images
#

targets += vmlinex.bin

OBJCOPYFLAGS_vmlinex.bin := -O binary
$(obj)/vmlinex.bin: vmlinex FORCE
	$(call if_changed,objcopy)
