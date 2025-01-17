// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *	WAX Device Driver
 *
 *	(c) Copyright 2000 The Puffin Group Inc.
 *
 *	by Helge Deller <deller@gmx.de>
 */

#include <linex/errno.h>
#include <linex/init.h>
#include <linex/interrupt.h>
#include <linex/ioport.h>
#include <linex/slab.h>
#include <linex/module.h>
#include <linex/types.h>

#include <asm/io.h>
#include <asm/hardware.h>

#include "gsc.h"

#define WAX_GSC_IRQ	7	/* Hardcoded Interrupt for GSC */

static void wax_choose_irq(struct parisc_device *dev, void *ctrl)
{
	int irq;

	switch (dev->id.sversion) {
		case 0x73:	irq =  1; break; /* i8042 General */
		case 0x8c:	irq =  6; break; /* Serial */
		case 0x90:	irq = 10; break; /* EISA */
		default:	return;		 /* Unknown */
	}

	gsc_asic_assign_irq(ctrl, irq, &dev->irq);

	switch (dev->id.sversion) {
		case 0x73:	irq =  2; break; /* i8042 High-priority */
		case 0x90:	irq =  0; break; /* EISA NMI */
		default:	return;		 /* No secondary IRQ */
	}

	gsc_asic_assign_irq(ctrl, irq, &dev->aux_irq);
}

static void __init
wax_init_irq(struct gsc_asic *wax)
{
	unsigned long base = wax->hpa;

	/* Wax-off */
	gsc_writel(0x00000000, base+OFFSET_IMR);

	/* clear pending interrupts */
	gsc_readl(base+OFFSET_IRR);

	/* We're not really convinced we want to reset the onboard
         * devices. Firmware does it for us...
	 */

	/* Resets */
//	gsc_writel(0xFFFFFFFF, base+0x1000); /* HIL */
//	gsc_writel(0xFFFFFFFF, base+0x2000); /* RS232-B on Wax */
}

static int __init wax_init_chip(struct parisc_device *dev)
{
	struct gsc_asic *wax;
	struct parisc_device *parent;
	int ret;

	wax = kzalloc(sizeof(*wax), GFP_KERNEL);
	if (!wax)
		return -ENOMEM;

	wax->name = "wax";
	wax->hpa = dev->hpa.start;

	wax->version = 0;   /* gsc_readb(wax->hpa+WAX_VER); */
	printk(KERN_INFO "%s at 0x%lx found.\n", wax->name, wax->hpa);

	/* Stop wax hissing for a bit */
	wax_init_irq(wax);

	/* the IRQ wax should use */
	dev->irq = gsc_claim_irq(&wax->gsc_irq, WAX_GSC_IRQ);
	if (dev->irq < 0) {
		printk(KERN_ERR "%s(): cannot get GSC irq\n",
				__func__);
		kfree(wax);
		return -EBUSY;
	}

	wax->eim = ((u32) wax->gsc_irq.txn_addr) | wax->gsc_irq.txn_data;

	ret = request_irq(wax->gsc_irq.irq, gsc_asic_intr, 0, "wax", wax);
	if (ret < 0) {
		kfree(wax);
		return ret;
	}

	/* enable IRQ's for devices below WAX */
	gsc_writel(wax->eim, wax->hpa + OFFSET_IAR);

	/* Done init'ing, register this driver */
	ret = gsc_common_setup(dev, wax);
	if (ret) {
		kfree(wax);
		return ret;
	}

	gsc_fixup_irqs(dev, wax, wax_choose_irq);
	/* On 715-class machines, Wax EISA is a sibling of Wax, not a child. */
	parent = parisc_parent(dev);
	if (parent->id.hw_type != HPHW_IOA) {
		gsc_fixup_irqs(parent, wax, wax_choose_irq);
	}

	return ret;
}

static const struct parisc_device_id wax_tbl[] __initconst = {
  	{ HPHW_BA, HVERSION_REV_ANY_ID, HVERSION_ANY_ID, 0x0008e },
	{ 0, }
};

MODULE_DEVICE_TABLE(parisc, wax_tbl);

struct parisc_driver wax_driver __refdata = {
	.name =		"wax",
	.id_table =	wax_tbl,
	.probe =	wax_init_chip,
};
