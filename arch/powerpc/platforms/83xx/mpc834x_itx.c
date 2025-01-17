// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * arch/powerpc/platforms/83xx/mpc834x_itx.c
 *
 * MPC834x ITX board specific routines
 *
 * Maintainer: Kumar Gala <galak@kernel.crashing.org>
 */

#include <linex/stddef.h>
#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/errno.h>
#include <linex/reboot.h>
#include <linex/pci.h>
#include <linex/kdev_t.h>
#include <linex/major.h>
#include <linex/console.h>
#include <linex/delay.h>
#include <linex/seq_file.h>
#include <linex/root_dev.h>
#include <linex/of_platform.h>

#include <linex/atomic.h>
#include <asm/time.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ipic.h>
#include <asm/irq.h>
#include <asm/udbg.h>
#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include "mpc83xx.h"

static const struct of_device_id mpc834x_itx_ids[] __initconst = {
	{ .compatible = "fsl,pq2pro-localbus", },
	{},
};

static int __init mpc834x_itx_declare_of_platform_devices(void)
{
	mpc83xx_declare_of_platform_devices();
	return of_platform_bus_probe(NULL, mpc834x_itx_ids, NULL);
}
machine_device_initcall(mpc834x_itx, mpc834x_itx_declare_of_platform_devices);

/* ************************************************************************
 *
 * Setup the architecture
 *
 */
static void __init mpc834x_itx_setup_arch(void)
{
	mpc83xx_setup_arch();

	mpc834x_usb_cfg();
}

define_machine(mpc834x_itx) {
	.name			= "MPC834x ITX",
	.compatible		= "MPC834xMITX",
	.setup_arch		= mpc834x_itx_setup_arch,
	.discover_phbs  	= mpc83xx_setup_pci,
	.init_IRQ		= mpc83xx_ipic_init_IRQ,
	.get_irq		= ipic_get_irq,
	.restart		= mpc83xx_restart,
	.time_init		= mpc83xx_time_init,
	.progress		= udbg_progress,
};
