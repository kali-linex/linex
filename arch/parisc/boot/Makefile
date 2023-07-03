# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for the linex parisc-specific parts of the boot image creator.
#

targets := image
targets += bzImage
subdir- := compressed

$(obj)/image: vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/bzImage: $(obj)/compressed/vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/compressed/vmlinex: FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@
