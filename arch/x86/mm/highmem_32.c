// SPDX-License-Identifier: GPL-2.0-only
#include <linex/highmem.h>
#include <linex/export.h>
#include <linex/swap.h> /* for totalram_pages */
#include <linex/memblock.h>
#include <asm/numa.h>

void __init set_highmem_pages_init(void)
{
	struct zone *zone;
	int nid;

	/*
	 * Explicitly reset zone->managed_pages because set_highmem_pages_init()
	 * is invoked before memblock_free_all()
	 */
	reset_all_zones_managed_pages();
	for_each_zone(zone) {
		unsigned long zone_start_pfn, zone_end_pfn;

		if (!is_highmem(zone))
			continue;

		zone_start_pfn = zone->zone_start_pfn;
		zone_end_pfn = zone_start_pfn + zone->spanned_pages;

		nid = zone_to_nid(zone);
		printk(KERN_INFO "Initializing %s for node %d (%08lx:%08lx)\n",
				zone->name, nid, zone_start_pfn, zone_end_pfn);

		add_highpages_with_active_regions(nid, zone_start_pfn,
				 zone_end_pfn);
	}
}
