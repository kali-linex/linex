// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * arch/powerpc/platforms/embedded6xx/wii.c
 *
 * Nintendo Wii board-specific support
 * Copyright (C) 2008-2009 The GameCube Linex Team
 * Copyright (C) 2008,2009 Albert Herranz
 */
#define DRV_MODULE_NAME "wii"
#define pr_fmt(fmt) DRV_MODULE_NAME ": " fmt

#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/irq.h>
#include <linex/seq_file.h>
#include <linex/of_address.h>
#include <linex/of_platform.h>

#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/time.h>
#include <asm/udbg.h>

#include "flipper-pic.h"
#include "hlwd-pic.h"
#include "usbgecko_udbg.h"

/* control block */
#define HW_CTRL_COMPATIBLE	"nintendo,hollywood-control"

#define HW_CTRL_RESETS		0x94
#define HW_CTRL_RESETS_SYS	(1<<0)

/* gpio */
#define HW_GPIO_COMPATIBLE	"nintendo,hollywood-gpio"

#define HW_GPIO_BASE(idx)	(idx * 0x20)
#define HW_GPIO_OUT(idx)	(HW_GPIO_BASE(idx) + 0)
#define HW_GPIO_DIR(idx)	(HW_GPIO_BASE(idx) + 4)
#define HW_GPIO_OWNER		(HW_GPIO_BASE(1) + 0x1c)

#define HW_GPIO_SHUTDOWN	(1<<1)
#define HW_GPIO_SLOT_LED	(1<<5)
#define HW_GPIO_SENSOR_BAR	(1<<8)


static void __iomem *hw_ctrl;
static void __iomem *hw_gpio;

static void __noreturn wii_spin(void)
{
	local_irq_disable();
	for (;;)
		cpu_relax();
}

static void __iomem *__init wii_ioremap_hw_regs(char *name, char *compatible)
{
	void __iomem *hw_regs = NULL;
	struct device_node *np;
	struct resource res;
	int error = -ENODEV;

	np = of_find_compatible_node(NULL, NULL, compatible);
	if (!np) {
		pr_err("no compatible node found for %s\n", compatible);
		goto out;
	}
	error = of_address_to_resource(np, 0, &res);
	if (error) {
		pr_err("no valid reg found for %pOFn\n", np);
		goto out_put;
	}

	hw_regs = ioremap(res.start, resource_size(&res));
	if (hw_regs) {
		pr_info("%s at 0x%pa mapped to 0x%p\n", name,
			&res.start, hw_regs);
	}

out_put:
	of_node_put(np);
out:
	return hw_regs;
}

static void __init wii_setup_arch(void)
{
	hw_ctrl = wii_ioremap_hw_regs("hw_ctrl", HW_CTRL_COMPATIBLE);
	hw_gpio = wii_ioremap_hw_regs("hw_gpio", HW_GPIO_COMPATIBLE);
	if (hw_gpio) {
		/* turn off the front blue led and IR light */
		clrbits32(hw_gpio + HW_GPIO_OUT(0),
			  HW_GPIO_SLOT_LED | HW_GPIO_SENSOR_BAR);
	}
}

static void __noreturn wii_restart(char *cmd)
{
	local_irq_disable();

	if (hw_ctrl) {
		/* clear the system reset pin to cause a reset */
		clrbits32(hw_ctrl + HW_CTRL_RESETS, HW_CTRL_RESETS_SYS);
	}
	wii_spin();
}

static void wii_power_off(void)
{
	local_irq_disable();

	if (hw_gpio) {
		/*
		 * set the owner of the shutdown pin to ARM, because it is
		 * accessed through the registers for the ARM, below
		 */
		clrbits32(hw_gpio + HW_GPIO_OWNER, HW_GPIO_SHUTDOWN);

		/* make sure that the poweroff GPIO is configured as output */
		setbits32(hw_gpio + HW_GPIO_DIR(1), HW_GPIO_SHUTDOWN);

		/* drive the poweroff GPIO high */
		setbits32(hw_gpio + HW_GPIO_OUT(1), HW_GPIO_SHUTDOWN);
	}
	wii_spin();
}

static void __noreturn wii_halt(void)
{
	if (ppc_md.restart)
		ppc_md.restart(NULL);
	wii_spin();
}

static void __init wii_pic_probe(void)
{
	flipper_pic_probe();
	hlwd_pic_probe();
}

static int __init wii_probe(void)
{
	pm_power_off = wii_power_off;

	ug_udbg_init();

	return 1;
}

static void wii_shutdown(void)
{
	hlwd_quiesce();
	flipper_quiesce();
}

static const struct of_device_id wii_of_bus[] = {
	{ .compatible = "nintendo,hollywood", },
	{ },
};

static int __init wii_device_probe(void)
{
	of_platform_populate(NULL, wii_of_bus, NULL, NULL);
	return 0;
}
machine_device_initcall(wii, wii_device_probe);

define_machine(wii) {
	.name			= "wii",
	.compatible		= "nintendo,wii",
	.probe			= wii_probe,
	.setup_arch		= wii_setup_arch,
	.restart		= wii_restart,
	.halt			= wii_halt,
	.init_IRQ		= wii_pic_probe,
	.get_irq		= flipper_pic_get_irq,
	.progress		= udbg_progress,
	.machine_shutdown	= wii_shutdown,
};
