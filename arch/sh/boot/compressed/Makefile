# SPDX-License-Identifier: GPL-2.0
#
# linex/arch/sh/boot/compressed/Makefile
#
# create a compressed vmlinex image from the original vmlinex
#

OBJECTS := head_32.o misc.o cache.o piggy.o \
           ashiftrt.o ashldi3.o ashrsi3.o ashlsi3.o lshrsi3.o

targets := vmlinex vmlinex.bin vmlinex.bin.gz vmlinex.bin.bz2 \
           vmlinex.bin.lzma vmlinex.bin.xz vmlinex.bin.lzo $(OBJECTS)

GCOV_PROFILE := n

#
# IMAGE_OFFSET is the load offset of the compression loader
#
ifeq ($(CONFIG_32BIT),y)
IMAGE_OFFSET	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_MEMORY_START)  + \
			$(CONFIG_BOOT_LINK_OFFSET)]')
else
IMAGE_OFFSET	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_PAGE_OFFSET)  + \
			$(KERNEL_MEMORY) + \
			$(CONFIG_BOOT_LINK_OFFSET)]')
endif

ccflags-remove-$(CONFIG_MCOUNT) += -pg

LDFLAGS_vmlinex := --oformat $(ld-bfd) -Ttext $(IMAGE_OFFSET) -e startup \
		   -T $(obj)/../../kernel/vmlinex.lds

KBUILD_CFLAGS += -DDISABLE_BRANCH_PROFILING

$(obj)/vmlinex: $(addprefix $(obj)/, $(OBJECTS)) FORCE
	$(call if_changed,ld)

$(obj)/vmlinex.bin: vmlinex FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinex.bin.gz: $(obj)/vmlinex.bin FORCE
	$(call if_changed,gzip)
$(obj)/vmlinex.bin.bz2: $(obj)/vmlinex.bin FORCE
	$(call if_changed,bzip2_with_size)
$(obj)/vmlinex.bin.lzma: $(obj)/vmlinex.bin FORCE
	$(call if_changed,lzma_with_size)
$(obj)/vmlinex.bin.xz: $(obj)/vmlinex.bin FORCE
	$(call if_changed,xzkern_with_size)
$(obj)/vmlinex.bin.lzo: $(obj)/vmlinex.bin FORCE
	$(call if_changed,lzo_with_size)

OBJCOPYFLAGS += -R .empty_zero_page

LDFLAGS_piggy.o := -r --format binary --oformat $(ld-bfd) -T

$(obj)/piggy.o: $(obj)/vmlinex.scr $(obj)/vmlinex.bin.$(suffix_y) FORCE
	$(call if_changed,ld)
