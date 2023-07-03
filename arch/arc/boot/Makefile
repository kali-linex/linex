# SPDX-License-Identifier: GPL-2.0

# uImage build relies on mkimage being availble on your host for ARC target
# You will need to build u-boot for ARC, rename mkimage to arc-elf32-mkimage
# and make sure it's reacable from your PATH

OBJCOPYFLAGS= -O binary -R .note -R .note.gnu.build-id -R .comment -S

LINEX_START_TEXT = $$($(READELF) -h vmlinex | \
			grep "Entry point address" | grep -o 0x.*)

UIMAGE_LOADADDR    = $(CONFIG_LINEX_LINK_BASE)
UIMAGE_ENTRYADDR   = $(LINEX_START_TEXT)

targets += vmlinex.bin
targets += vmlinex.bin.gz
targets += vmlinex.bin.lzma
targets += uImage.bin
targets += uImage.gz
targets += uImage.lzma

$(obj)/vmlinex.bin: vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinex.bin.gz: $(obj)/vmlinex.bin FORCE
	$(call if_changed,gzip)

$(obj)/vmlinex.bin.lzma: $(obj)/vmlinex.bin FORCE
	$(call if_changed,lzma)

$(obj)/uImage.bin: $(obj)/vmlinex.bin FORCE
	$(call if_changed,uimage,none)

$(obj)/uImage.gz: $(obj)/vmlinex.bin.gz FORCE
	$(call if_changed,uimage,gzip)

$(obj)/uImage.lzma: $(obj)/vmlinex.bin.lzma FORCE
	$(call if_changed,uimage,lzma)
