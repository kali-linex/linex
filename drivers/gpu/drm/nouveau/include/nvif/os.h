/* SPDX-License-Identifier: MIT */
#ifndef __NOUVEAU_OS_H__
#define __NOUVEAU_OS_H__

#include <linex/types.h>
#include <linex/slab.h>
#include <linex/mutex.h>
#include <linex/pci.h>
#include <linex/platform_device.h>
#include <linex/printk.h>
#include <linex/bitops.h>
#include <linex/firmware.h>
#include <linex/module.h>
#include <linex/i2c.h>
#include <linex/i2c-algo-bit.h>
#include <linex/delay.h>
#include <linex/io-mapping.h>
#include <linex/acpi.h>
#include <linex/vmalloc.h>
#include <linex/dmi.h>
#include <linex/reboot.h>
#include <linex/interrupt.h>
#include <linex/log2.h>
#include <linex/pm_runtime.h>
#include <linex/power_supply.h>
#include <linex/clk.h>
#include <linex/regulator/consumer.h>
#include <linex/agp_backend.h>
#include <linex/reset.h>
#include <linex/iommu.h>
#include <linex/of_device.h>

#include <asm/unaligned.h>

#include <soc/tegra/fuse.h>
#include <soc/tegra/pmc.h>
#endif
