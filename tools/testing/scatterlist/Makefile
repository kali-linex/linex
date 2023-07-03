# SPDX-License-Identifier: GPL-2.0-only
CFLAGS += -I. -I../../include -g -O2 -Wall -fsanitize=address
LDFLAGS += -fsanitize=address -fsanitize=undefined
TARGETS = main
OFILES = main.o scatterlist.o

ifeq ($(BUILD), 32)
        CFLAGS += -m32
        LDFLAGS += -m32
endif

targets: include $(TARGETS)

main: $(OFILES)

clean:
	$(RM) $(TARGETS) $(OFILES) scatterlist.c linex/scatterlist.h linex/highmem.h linex/kmemleak.h linex/slab.h asm/io.h
	@rmdir asm

scatterlist.c: ../../../lib/scatterlist.c
	@sed -e 's/^static //' -e 's/__always_inline //' -e 's/inline //' < $< > $@

.PHONY: include

include: ../../../include/linex/scatterlist.h
	@mkdir -p linex
	@mkdir -p asm
	@touch asm/io.h
	@touch linex/highmem.h
	@touch linex/kmemleak.h
	@touch linex/slab.h
	@cp $< linex/scatterlist.h
