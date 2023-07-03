# SPDX-License-Identifier: GPL-2.0
OUTPUT := .output
abs_out := $(abspath $(OUTPUT))

CLANG ?= clang
LLC ?= llc
LLVM_STRIP ?= llvm-strip

TOOLS_PATH := $(abspath ../../../../tools)
BPFTOOL_SRC := $(TOOLS_PATH)/bpf/bpftool
BPFTOOL_OUTPUT := $(abs_out)/bpftool
DEFAULT_BPFTOOL := $(BPFTOOL_OUTPUT)/bootstrap/bpftool
BPFTOOL ?= $(DEFAULT_BPFTOOL)

LIBBPF_SRC := $(TOOLS_PATH)/lib/bpf
LIBBPF_OUTPUT := $(abs_out)/libbpf
LIBBPF_DESTDIR := $(LIBBPF_OUTPUT)
LIBBPF_INCLUDE := $(LIBBPF_DESTDIR)/include
BPFOBJ := $(LIBBPF_OUTPUT)/libbpf.a

INCLUDES := -I$(OUTPUT) -I$(LIBBPF_INCLUDE) -I$(TOOLS_PATH)/include/uapi
CFLAGS := -g -Wall

VMLINEX_BTF_PATHS ?= $(if $(O),$(O)/vmlinex)				\
		     $(if $(KBUILD_OUTPUT),$(KBUILD_OUTPUT)/vmlinex)	\
		     ../../../../vmlinex				\
		     /sys/kernel/btf/vmlinex				\
		     /boot/vmlinex-$(shell uname -r)
VMLINEX_BTF ?= $(abspath $(firstword $(wildcard $(VMLINEX_BTF_PATHS))))
ifeq ($(VMLINEX_BTF),)
$(error Cannot find a vmlinex for VMLINEX_BTF at any of "$(VMLINEX_BTF_PATHS)")
endif

ifeq ($(V),1)
Q =
msg =
else
Q = @
msg = @printf '  %-8s %s%s\n' "$(1)" "$(notdir $(2))" "$(if $(3), $(3))";
MAKEFLAGS += --no-print-directory
submake_extras := feature_display=0
endif

.DELETE_ON_ERROR:

.PHONY: all clean

all: entrypoints.lskel.h

clean:
	$(call msg,CLEAN)
	$(Q)rm -rf $(OUTPUT) entrypoints

entrypoints.lskel.h: $(OUTPUT)/entrypoints.bpf.o | $(BPFTOOL)
	$(call msg,GEN-SKEL,$@)
	$(Q)$(BPFTOOL) gen skeleton -L $< > $@


$(OUTPUT)/entrypoints.bpf.o: entrypoints.bpf.c $(OUTPUT)/vmlinex.h $(BPFOBJ) | $(OUTPUT)
	$(call msg,BPF,$@)
	$(Q)$(CLANG) -g -O2 -target bpf $(INCLUDES)			      \
		 -c $(filter %.c,$^) -o $@ &&				      \
	$(LLVM_STRIP) -g $@

$(OUTPUT)/vmlinex.h: $(VMLINEX_BTF) $(BPFTOOL) | $(INCLUDE_DIR)
ifeq ($(VMLINEX_H),)
	$(call msg,GEN,,$@)
	$(Q)$(BPFTOOL) btf dump file $(VMLINEX_BTF) format c > $@
else
	$(call msg,CP,,$@)
	$(Q)cp "$(VMLINEX_H)" $@
endif

$(OUTPUT) $(LIBBPF_OUTPUT) $(BPFTOOL_OUTPUT):
	$(call msg,MKDIR,$@)
	$(Q)mkdir -p $@

$(BPFOBJ): $(wildcard $(LIBBPF_SRC)/*.[ch] $(LIBBPF_SRC)/Makefile) | $(LIBBPF_OUTPUT)
	$(Q)$(MAKE) $(submake_extras) -C $(LIBBPF_SRC)			       \
		    OUTPUT=$(abspath $(dir $@))/ prefix=		       \
		    DESTDIR=$(LIBBPF_DESTDIR) $(abspath $@) install_headers

ifeq ($(CROSS_COMPILE),)
$(DEFAULT_BPFTOOL): $(BPFOBJ) | $(BPFTOOL_OUTPUT)
	$(Q)$(MAKE) $(submake_extras) -C $(BPFTOOL_SRC)			       \
		    OUTPUT=$(BPFTOOL_OUTPUT)/				       \
		    LIBBPF_BOOTSTRAP_OUTPUT=$(LIBBPF_OUTPUT)/		       \
		    LIBBPF_BOOTSTRAP_DESTDIR=$(LIBBPF_DESTDIR)/ bootstrap
else
$(DEFAULT_BPFTOOL): | $(BPFTOOL_OUTPUT)
	$(Q)$(MAKE) $(submake_extras) -C $(BPFTOOL_SRC)			       \
		    OUTPUT=$(BPFTOOL_OUTPUT)/ bootstrap
endif
