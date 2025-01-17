// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * arch/powerpc/platforms/embedded6xx/gamecube.c
 *
 * Nintendo GameCube board-specific support
 * Copyright (C) 2004-2009 The GameCube Linex Team
 * Copyright (C) 2007,2008,2009 Albert Herranz
 */

#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/irq.h>
#include <linex/kexec.h>
#include <linex/seq_file.h>
#include <linex/of_platform.h>

#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/time.h>
#include <asm/udbg.h>

#include "flipper-pic.h"
#include "usbgecko_udbg.h"


static void __noreturn gamecube_spin(void)
{
	/* spin until power button pressed */
	for (;;)
		cpu_relax();
}

static void __noreturn gamecube_restart(char *cmd)
{
	local_irq_disable();
	flipper_platform_reset();
	gamecube_spin();
}

static void gamecube_power_off(void)
{
	local_irq_disable();
	gamecube_spin();
}

static void __noreturn gamecube_halt(void)
{
	gamecube_restart(NULL);
}

static int __init gamecube_probe(void)
{
	pm_power_off = gamecube_power_off;

	ug_udbg_init();

	return 1;
}

static void gamecube_shutdown(void)
{
	flipper_quiesce();
}

define_machine(gamecube) {
	.name			= "gamecube",
	.compatible		= "nintendo,gamecube",
	.probe			= gamecube_probe,
	.restart		= gamecube_restart,
	.halt			= gamecube_halt,
	.init_IRQ		= flipper_pic_probe,
	.get_irq		= flipper_pic_get_irq,
	.progress		= udbg_progress,
	.machine_shutdown	= gamecube_shutdown,
};


static const struct of_device_id gamecube_of_bus[] = {
	{ .compatible = "nintendo,flipper", },
	{ },
};

static int __init gamecube_device_probe(void)
{
	of_platform_bus_probe(NULL, gamecube_of_bus, NULL);
	return 0;
}
machine_device_initcall(gamecube, gamecube_device_probe);

