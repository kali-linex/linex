// SPDX-License-Identifier: GPL-2.0
#include <linex/kernel.h>
#include <linex/init.h>
#include <linex/types.h>
#include <linex/pci.h>
#include <linex/sh_intc.h>

int pcibios_map_platform_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	int irq;

	if (dev->bus->number == 0) {
		switch (slot) {
		case 4: return evt2irq(0x2a0);	/* eth0       */
		case 8: return evt2irq(0x2a0);	/* eth1       */
		case 6: return evt2irq(0x240);	/* PCI bridge */
		default:
			printk(KERN_ERR "PCI: Bad IRQ mapping request "
					"for slot %d\n", slot);
			return evt2irq(0x240);
		}
	} else {
		switch (pin) {
		case 0:   irq =  evt2irq(0x240); break;
		case 1:   irq =  evt2irq(0x240); break;
		case 2:   irq =  evt2irq(0x240); break;
		case 3:   irq =  evt2irq(0x240); break;
		case 4:   irq =  evt2irq(0x240); break;
		default:  irq = -1; break;
		}
	}
	return irq;
}
