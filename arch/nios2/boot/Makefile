#
# arch/nios2/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

UIMAGE_LOADADDR = $(shell $(NM) vmlinex | awk '$$NF == "_stext" {print $$1}')
UIMAGE_ENTRYADDR = $(shell $(NM) vmlinex | awk '$$NF == "_start" {print $$1}')
UIMAGE_COMPRESSION = gzip

OBJCOPYFLAGS_vmlinex.bin := -O binary

targets += vmlinex.bin vmlinex.gz vmImage

$(obj)/vmlinex.bin: vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinex.gz: $(obj)/vmlinex.bin FORCE
	$(call if_changed,gzip)

$(obj)/vmImage: $(obj)/vmlinex.gz FORCE
	$(call if_changed,uimage)
	@$(kecho) 'Kernel: $@ is ready'

$(obj)/zImage: $(obj)/compressed/vmlinex FORCE
	$(call if_changed,objcopy)
	@$(kecho) 'Kernel: $@ is ready'

$(obj)/compressed/vmlinex: $(obj)/vmlinex.gz FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@
