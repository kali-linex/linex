#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995, 1998, 2001, 2002 by Ralf Baechle
# Copyright (C) 2004  Maciej W. Rozycki
#

#
# Some DECstations need all possible sections of an ECOFF executable
#
ifdef CONFIG_MACH_DECSTATION
  e2eflag := -a
endif

#
# Drop some uninteresting sections in the kernel.
# This is only relevant for ELF kernels but doesn't hurt a.out
#
drop-sections := .reginfo .mdebug .comment .note .pdr .options .MIPS.options
strip-flags   := $(addprefix --remove-section=,$(drop-sections))

hostprogs := elf2ecoff

suffix-y			:= bin
suffix-$(CONFIG_KERNEL_BZIP2)	:= bz2
suffix-$(CONFIG_KERNEL_GZIP)	:= gz
suffix-$(CONFIG_KERNEL_LZMA)	:= lzma
suffix-$(CONFIG_KERNEL_LZO)	:= lzo

targets := vmlinex.ecoff
quiet_cmd_ecoff = ECOFF	  $@
      cmd_ecoff = $(obj)/elf2ecoff $(VMLINEX) $@ $(e2eflag)
$(obj)/vmlinex.ecoff: $(obj)/elf2ecoff $(VMLINEX) FORCE
	$(call if_changed,ecoff)

targets += vmlinex.bin
quiet_cmd_bin = OBJCOPY $@
      cmd_bin = $(OBJCOPY) -O binary $(strip-flags) $(VMLINEX) $@
$(obj)/vmlinex.bin: $(VMLINEX) FORCE
	$(call if_changed,bin)

targets += vmlinex.srec
quiet_cmd_srec = OBJCOPY $@
      cmd_srec = $(OBJCOPY) -S -O srec $(strip-flags) $(VMLINEX) $@
$(obj)/vmlinex.srec: $(VMLINEX) FORCE
	$(call if_changed,srec)

UIMAGE_LOADADDR  = $(VMLINEX_LOAD_ADDRESS)
UIMAGE_ENTRYADDR = $(VMLINEX_ENTRY_ADDRESS)

#
# Compressed vmlinex images
#

extra-y += vmlinex.bin.bz2
extra-y += vmlinex.bin.gz
extra-y += vmlinex.bin.lzma
extra-y += vmlinex.bin.lzo

$(obj)/vmlinex.bin.bz2: $(obj)/vmlinex.bin FORCE
	$(call if_changed,bzip2)

$(obj)/vmlinex.bin.gz: $(obj)/vmlinex.bin FORCE
	$(call if_changed,gzip)

$(obj)/vmlinex.bin.lzma: $(obj)/vmlinex.bin FORCE
	$(call if_changed,lzma)

$(obj)/vmlinex.bin.lzo: $(obj)/vmlinex.bin FORCE
	$(call if_changed,lzo)

#
# Compressed u-boot images
#

targets += uImage
targets += uImage.bin
targets += uImage.bz2
targets += uImage.gz
targets += uImage.lzma
targets += uImage.lzo

$(obj)/uImage.bin: $(obj)/vmlinex.bin FORCE
	$(call if_changed,uimage,none)

$(obj)/uImage.bz2: $(obj)/vmlinex.bin.bz2 FORCE
	$(call if_changed,uimage,bzip2)

$(obj)/uImage.gz: $(obj)/vmlinex.bin.gz FORCE
	$(call if_changed,uimage,gzip)

$(obj)/uImage.lzma: $(obj)/vmlinex.bin.lzma FORCE
	$(call if_changed,uimage,lzma)

$(obj)/uImage.lzo: $(obj)/vmlinex.bin.lzo FORCE
	$(call if_changed,uimage,lzo)

$(obj)/uImage: $(obj)/uImage.$(suffix-y)
	@ln -sf $(notdir $<) $@
	@echo '  Image $@ is ready'

#
# Flattened Image Tree (.itb) images
#

ifeq ($(ADDR_BITS),32)
itb_addr_cells = 1
endif
ifeq ($(ADDR_BITS),64)
itb_addr_cells = 2
endif

targets += vmlinex.its.S

quiet_cmd_its_cat = CAT     $@
      cmd_its_cat = cat $(real-prereqs) >$@

$(obj)/vmlinex.its.S: $(addprefix $(srctree)/arch/mips/$(PLATFORM)/,$(ITS_INPUTS)) FORCE
	$(call if_changed,its_cat)

targets += vmlinex.its
targets += vmlinex.gz.its
targets += vmlinex.bz2.its
targets += vmlinex.lzma.its
targets += vmlinex.lzo.its

quiet_cmd_cpp_its_S = ITS     $@
      cmd_cpp_its_S = $(CPP) -P -C -o $@ $< \
		        -DKERNEL_NAME="\"Linex $(KERNELRELEASE)\"" \
			-DVMLINEX_BINARY="\"$(3)\"" \
			-DVMLINEX_COMPRESSION="\"$(2)\"" \
			-DVMLINEX_LOAD_ADDRESS=$(VMLINEX_LOAD_ADDRESS) \
			-DVMLINEX_ENTRY_ADDRESS=$(VMLINEX_ENTRY_ADDRESS) \
			-DADDR_BITS=$(ADDR_BITS) \
			-DADDR_CELLS=$(itb_addr_cells)

$(obj)/vmlinex.its: $(obj)/vmlinex.its.S $(VMLINEX) FORCE
	$(call if_changed,cpp_its_S,none,vmlinex.bin)

$(obj)/vmlinex.gz.its: $(obj)/vmlinex.its.S $(VMLINEX) FORCE
	$(call if_changed,cpp_its_S,gzip,vmlinex.bin.gz)

$(obj)/vmlinex.bz2.its: $(obj)/vmlinex.its.S $(VMLINEX)  FORCE
	$(call if_changed,cpp_its_S,bzip2,vmlinex.bin.bz2)

$(obj)/vmlinex.lzma.its: $(obj)/vmlinex.its.S $(VMLINEX) FORCE
	$(call if_changed,cpp_its_S,lzma,vmlinex.bin.lzma)

$(obj)/vmlinex.lzo.its: $(obj)/vmlinex.its.S $(VMLINEX) FORCE
	$(call if_changed,cpp_its_S,lzo,vmlinex.bin.lzo)

targets += vmlinex.itb
targets += vmlinex.gz.itb
targets += vmlinex.bz2.itb
targets += vmlinex.lzma.itb
targets += vmlinex.lzo.itb

quiet_cmd_itb-image = ITB     $@
      cmd_itb-image = \
		env PATH="$(objtree)/scripts/dtc:$(PATH)" \
		$(BASH) $(MKIMAGE) \
		-D "-I dts -O dtb -p 500 \
			--include $(objtree)/arch/mips \
			--warning no-unit_address_vs_reg" \
		-f $(2) $@

$(obj)/vmlinex.itb: $(obj)/vmlinex.its $(obj)/vmlinex.bin FORCE
	$(call if_changed,itb-image,$<)

$(obj)/vmlinex.%.itb: $(obj)/vmlinex.%.its $(obj)/vmlinex.bin.% FORCE
	$(call if_changed,itb-image,$<)

# for cleaning
subdir- += compressed tools
