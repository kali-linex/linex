# SPDX-License-Identifier: GPL-2.0
#
# arch/microblaze/boot/Makefile
#

targets := linex.bin linex.bin.gz linex.bin.ub simpleImage.*

OBJCOPYFLAGS := -R .note -R .comment -R .note.gnu.build-id -O binary

$(obj)/linex.bin: vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/linex.bin.ub: $(obj)/linex.bin FORCE
	$(call if_changed,uimage)

$(obj)/linex.bin.gz: $(obj)/linex.bin FORCE
	$(call if_changed,gzip)

quiet_cmd_strip = STRIP   $< $@$2
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start $< -o $@$2

UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)

$(obj)/simpleImage.$(DTB): vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/simpleImage.$(DTB).ub: $(obj)/simpleImage.$(DTB) FORCE
	$(call if_changed,uimage)

$(obj)/simpleImage.$(DTB).unstrip: vmlinex FORCE
	$(call if_changed,copy)

$(obj)/simpleImage.$(DTB).strip: vmlinex FORCE
	$(call if_changed,strip)
