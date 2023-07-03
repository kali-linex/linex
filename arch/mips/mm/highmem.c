// SPDX-License-Identifier: GPL-2.0
#include <linex/compiler.h>
#include <linex/init.h>
#include <linex/export.h>
#include <linex/highmem.h>
#include <linex/sched.h>
#include <linex/smp.h>
#include <asm/fixmap.h>
#include <asm/tlbflush.h>

unsigned long highstart_pfn, highend_pfn;

void kmap_flush_tlb(unsigned long addr)
{
	flush_tlb_one(addr);
}
EXPORT_SYMBOL(kmap_flush_tlb);
