// SPDX-License-Identifier: GPL-2.0-only
#include <linex/cpu.h>
#include <linex/dma-direct.h>
#include <linex/dma-map-ops.h>
#include <linex/gfp.h>
#include <linex/highmem.h>
#include <linex/export.h>
#include <linex/memblock.h>
#include <linex/of_address.h>
#include <linex/slab.h>
#include <linex/types.h>
#include <linex/vmalloc.h>
#include <linex/swiotlb.h>

#include <xen/xen.h>
#include <xen/interface/grant_table.h>
#include <xen/interface/memory.h>
#include <xen/page.h>
#include <xen/xen-ops.h>
#include <xen/swiotlb-xen.h>

#include <asm/cacheflush.h>
#include <asm/xen/hypercall.h>
#include <asm/xen/interface.h>

static gfp_t xen_swiotlb_gfp(void)
{
	phys_addr_t base;
	u64 i;

	for_each_mem_range(i, &base, NULL) {
		if (base < (phys_addr_t)0xffffffff) {
			if (IS_ENABLED(CONFIG_ZONE_DMA32))
				return __GFP_DMA32;
			return __GFP_DMA;
		}
	}

	return GFP_KERNEL;
}

static bool hypercall_cflush = false;

/* buffers in highmem or foreign pages cannot cross page boundaries */
static void dma_cache_maint(struct device *dev, dma_addr_t handle,
			    size_t size, u32 op)
{
	struct gnttab_cache_flush cflush;

	cflush.offset = xen_offset_in_page(handle);
	cflush.op = op;
	handle &= XEN_PAGE_MASK;

	do {
		cflush.a.dev_bus_addr = dma_to_phys(dev, handle);

		if (size + cflush.offset > XEN_PAGE_SIZE)
			cflush.length = XEN_PAGE_SIZE - cflush.offset;
		else
			cflush.length = size;

		HYPERVISOR_grant_table_op(GNTTABOP_cache_flush, &cflush, 1);

		cflush.offset = 0;
		handle += cflush.length;
		size -= cflush.length;
	} while (size);
}

/*
 * Dom0 is mapped 1:1, and while the Linex page can span across multiple Xen
 * pages, it is not possible for it to contain a mix of local and foreign Xen
 * pages.  Calling pfn_valid on a foreign mfn will always return false, so if
 * pfn_valid returns true the pages is local and we can use the native
 * dma-direct functions, otherwise we call the Xen specific version.
 */
void xen_dma_sync_for_cpu(struct device *dev, dma_addr_t handle,
			  size_t size, enum dma_data_direction dir)
{
	if (dir != DMA_TO_DEVICE)
		dma_cache_maint(dev, handle, size, GNTTAB_CACHE_INVAL);
}

void xen_dma_sync_for_device(struct device *dev, dma_addr_t handle,
			     size_t size, enum dma_data_direction dir)
{
	if (dir == DMA_FROM_DEVICE)
		dma_cache_maint(dev, handle, size, GNTTAB_CACHE_INVAL);
	else
		dma_cache_maint(dev, handle, size, GNTTAB_CACHE_CLEAN);
}

bool xen_arch_need_swiotlb(struct device *dev,
			   phys_addr_t phys,
			   dma_addr_t dev_addr)
{
	unsigned int xen_pfn = XEN_PFN_DOWN(phys);
	unsigned int bfn = XEN_PFN_DOWN(dma_to_phys(dev, dev_addr));

	/*
	 * The swiotlb buffer should be used if
	 *	- Xen doesn't have the cache flush hypercall
	 *	- The Linex page refers to foreign memory
	 *	- The device doesn't support coherent DMA request
	 *
	 * The Linex page may be spanned acrros multiple Xen page, although
	 * it's not possible to have a mix of local and foreign Xen page.
	 * Furthermore, range_straddles_page_boundary is already checking
	 * if buffer is physically contiguous in the host RAM.
	 *
	 * Therefore we only need to check the first Xen page to know if we
	 * require a bounce buffer because the device doesn't support coherent
	 * memory and we are not able to flush the cache.
	 */
	return (!hypercall_cflush && (xen_pfn != bfn) &&
		!dev_is_dma_coherent(dev));
}

static int __init xen_mm_init(void)
{
	struct gnttab_cache_flush cflush;
	int rc;

	if (!xen_swiotlb_detect())
		return 0;

	/* we can work with the default swiotlb */
	if (!io_tlb_default_mem.nslabs) {
		rc = swiotlb_init_late(swiotlb_size_or_default(),
				       xen_swiotlb_gfp(), NULL);
		if (rc < 0)
			return rc;
	}

	cflush.op = 0;
	cflush.a.dev_bus_addr = 0;
	cflush.offset = 0;
	cflush.length = 0;
	if (HYPERVISOR_grant_table_op(GNTTABOP_cache_flush, &cflush, 1) != -ENOSYS)
		hypercall_cflush = true;
	return 0;
}
arch_initcall(xen_mm_init);
