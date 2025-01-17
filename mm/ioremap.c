// SPDX-License-Identifier: GPL-2.0
/*
 * Re-map IO memory to kernel address space so that we can access it.
 * This is needed for high PCI addresses that aren't mapped in the
 * 640k-1MB IO memory area on PC's
 *
 * (C) Copyright 1995 1996 Linus Torvalds
 */
#include <linex/vmalloc.h>
#include <linex/mm.h>
#include <linex/io.h>
#include <linex/export.h>

void __iomem *ioremap_prot(phys_addr_t phys_addr, size_t size,
			   unsigned long prot)
{
	unsigned long offset, vaddr;
	phys_addr_t last_addr;
	struct vm_struct *area;

	/* Disallow wrap-around or zero size */
	last_addr = phys_addr + size - 1;
	if (!size || last_addr < phys_addr)
		return NULL;

	/* Page-align mappings */
	offset = phys_addr & (~PAGE_MASK);
	phys_addr -= offset;
	size = PAGE_ALIGN(size + offset);

	if (!ioremap_allowed(phys_addr, size, prot))
		return NULL;

	area = get_vm_area_caller(size, VM_IOREMAP,
			__builtin_return_address(0));
	if (!area)
		return NULL;
	vaddr = (unsigned long)area->addr;
	area->phys_addr = phys_addr;

	if (ioremap_page_range(vaddr, vaddr + size, phys_addr,
			       __pgprot(prot))) {
		free_vm_area(area);
		return NULL;
	}

	return (void __iomem *)(vaddr + offset);
}
EXPORT_SYMBOL(ioremap_prot);

void iounmap(volatile void __iomem *addr)
{
	void *vaddr = (void *)((unsigned long)addr & PAGE_MASK);

	if (!iounmap_allowed(vaddr))
		return;

	if (is_vmalloc_addr(vaddr))
		vunmap(vaddr);
}
EXPORT_SYMBOL(iounmap);
