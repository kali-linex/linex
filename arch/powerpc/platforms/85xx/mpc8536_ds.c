// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * MPC8536 DS Board Setup
 *
 * Copyright 2008 Freescale Semiconductor, Inc.
 */

#include <linex/stddef.h>
#include <linex/kernel.h>
#include <linex/pci.h>
#include <linex/kdev_t.h>
#include <linex/delay.h>
#include <linex/seq_file.h>
#include <linex/interrupt.h>
#include <linex/of_platform.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
#include <asm/swiotlb.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include "mpc85xx.h"

void __init mpc8536_ds_pic_init(void)
{
	struct mpic *mpic = mpic_alloc(NULL, 0, MPIC_BIG_ENDIAN,
			0, 256, " OpenPIC  ");
	BUG_ON(mpic == NULL);
	mpic_init(mpic);
}

/*
 * Setup the architecture
 */
static void __init mpc8536_ds_setup_arch(void)
{
	if (ppc_md.progress)
		ppc_md.progress("mpc8536_ds_setup_arch()", 0);

	fsl_pci_assign_primary();

	swiotlb_detect_4g();

	printk("MPC8536 DS board from Freescale Semiconductor\n");
}

machine_arch_initcall(mpc8536_ds, mpc85xx_common_publish_devices);

define_machine(mpc8536_ds) {
	.name			= "MPC8536 DS",
	.compatible		= "fsl,mpc8536ds",
	.setup_arch		= mpc8536_ds_setup_arch,
	.init_IRQ		= mpc8536_ds_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
	.pcibios_fixup_phb      = fsl_pcibios_fixup_phb,
#endif
	.get_irq		= mpic_get_irq,
	.progress		= udbg_progress,
};
