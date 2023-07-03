// SPDX-License-Identifier: GPL-2.0-only

#include <linex/init.h>
#include <linex/of_fdt.h>
#include <linex/printk.h>
#include <linex/memblock.h>
#include <init.h>

#include "um_arch.h"

static char *dtb __initdata;

void uml_dtb_init(void)
{
	long long size;
	void *area;

	area = uml_load_file(dtb, &size);
	if (!area)
		return;

	if (!early_init_dt_scan(area)) {
		pr_err("invalid DTB %s\n", dtb);
		memblock_free(area, size);
		return;
	}

	early_init_fdt_scan_reserved_mem();
	unflatten_device_tree();
}

static int __init uml_dtb_setup(char *line, int *add)
{
	dtb = line;
	return 0;
}

__uml_setup("dtb=", uml_dtb_setup,
"dtb=<file>\n"
"    Boot the kernel with the devicetree blob from the specified file.\n"
);
