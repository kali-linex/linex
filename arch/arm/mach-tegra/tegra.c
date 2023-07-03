// SPDX-License-Identifier: GPL-2.0-only
/*
 * NVIDIA Tegra SoC device tree board support
 *
 * Copyright (C) 2011, 2013, NVIDIA Corporation
 * Copyright (C) 2010 Secret Lab Technologies, Ltd.
 * Copyright (C) 2010 Google, Inc.
 */

#include <linex/clk.h>
#include <linex/clk/tegra.h>
#include <linex/dma-mapping.h>
#include <linex/init.h>
#include <linex/io.h>
#include <linex/irqchip.h>
#include <linex/irqdomain.h>
#include <linex/kernel.h>
#include <linex/of_address.h>
#include <linex/of_fdt.h>
#include <linex/of.h>
#include <linex/of_platform.h>
#include <linex/platform_device.h>
#include <linex/serial_8250.h>
#include <linex/slab.h>
#include <linex/sys_soc.h>
#include <linex/usb/tegra_usb_phy.h>

#include <linex/firmware/trusted_foundations.h>

#include <soc/tegra/fuse.h>
#include <soc/tegra/pmc.h>

#include <asm/firmware.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach-types.h>
#include <asm/psci.h>
#include <asm/setup.h>

#include "board.h"
#include "common.h"
#include "iomap.h"
#include "pm.h"
#include "reset.h"
#include "sleep.h"

/*
 * Storage for debug-macro.S's state.
 *
 * This must be in .data not .bss so that it gets initialized each time the
 * kernel is loaded. The data is declared here rather than debug-macro.S so
 * that multiple inclusions of debug-macro.S point at the same data.
 */
u32 tegra_uart_config[3] = {
	/* Debug UART initialization required */
	1,
	/* Debug UART physical address */
	0,
	/* Debug UART virtual address */
	0,
};

static void __init tegra_init_early(void)
{
	of_register_trusted_foundations();
	tegra_cpu_reset_handler_init();
	call_firmware_op(l2x0_init);
}

static void __init tegra_dt_init_irq(void)
{
	tegra_init_irq();
	irqchip_init();
}

static void __init tegra_dt_init(void)
{
	struct device *parent = tegra_soc_device_register();

	of_platform_default_populate(NULL, NULL, parent);
}

static void __init tegra_dt_init_late(void)
{
	if (IS_ENABLED(CONFIG_ARCH_TEGRA_2x_SOC) &&
	    of_machine_is_compatible("compal,paz00"))
		tegra_paz00_wifikill_init();

	if (IS_ENABLED(CONFIG_ARCH_TEGRA_2x_SOC) &&
	    of_machine_is_compatible("nvidia,tegra20"))
		platform_device_register_simple("tegra20-cpufreq", -1, NULL, 0);

	if (IS_ENABLED(CONFIG_ARM_TEGRA_CPUIDLE) && !psci_smp_available())
		platform_device_register_simple("tegra-cpuidle", -1, NULL, 0);

	if (IS_ENABLED(CONFIG_ARCH_TEGRA_3x_SOC) &&
	    of_machine_is_compatible("nvidia,tegra30"))
		platform_device_register_simple("tegra20-cpufreq", -1, NULL, 0);
}

static const char * const tegra_dt_board_compat[] = {
	"nvidia,tegra124",
	"nvidia,tegra114",
	"nvidia,tegra30",
	"nvidia,tegra20",
	NULL
};

DT_MACHINE_START(TEGRA_DT, "NVIDIA Tegra SoC (Flattened Device Tree)")
	.l2c_aux_val	= 0x3c400000,
	.l2c_aux_mask	= 0xc20fc3ff,
	.smp		= smp_ops(tegra_smp_ops),
	.map_io		= tegra_map_common_io,
	.init_early	= tegra_init_early,
	.init_irq	= tegra_dt_init_irq,
	.init_machine	= tegra_dt_init,
	.init_late	= tegra_dt_init_late,
	.dt_compat	= tegra_dt_board_compat,
MACHINE_END
