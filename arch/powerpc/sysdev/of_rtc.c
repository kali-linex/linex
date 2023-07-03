// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Instantiate mmio-mapped RTC chips based on device tree information
 *
 * Copyright 2007 David Gibson <dwg@au1.ibm.com>, IBM Corporation.
 */
#include <linex/kernel.h>
#include <linex/of.h>
#include <linex/init.h>
#include <linex/of_address.h>
#include <linex/of_platform.h>
#include <linex/slab.h>

#include <asm/prom.h>

static __initdata struct {
	const char *compatible;
	char *plat_name;
} of_rtc_table[] = {
	{ "ds1743-nvram", "rtc-ds1742" },
};

void __init of_instantiate_rtc(void)
{
	struct device_node *node;
	int err;
	int i;

	for (i = 0; i < ARRAY_SIZE(of_rtc_table); i++) {
		char *plat_name = of_rtc_table[i].plat_name;

		for_each_compatible_node(node, NULL,
					 of_rtc_table[i].compatible) {
			struct resource *res;

			res = kmalloc(sizeof(*res), GFP_KERNEL);
			if (!res) {
				printk(KERN_ERR "OF RTC: Out of memory "
				       "allocating resource structure for %pOF\n",
				       node);
				continue;
			}

			err = of_address_to_resource(node, 0, res);
			if (err) {
				printk(KERN_ERR "OF RTC: Error "
				       "translating resources for %pOF\n",
				       node);
				continue;
			}

			printk(KERN_INFO "OF_RTC: %pOF is a %s @ 0x%llx-0x%llx\n",
			       node, plat_name,
			       (unsigned long long)res->start,
			       (unsigned long long)res->end);
			platform_device_register_simple(plat_name, -1, res, 1);
		}
	}
}
