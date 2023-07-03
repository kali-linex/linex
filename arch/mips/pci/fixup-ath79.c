// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2018 John Crispin <john@phrozen.org>
 */

#include <linex/pci.h>
//#include <linex/of_irq.h>
#include <linex/of_pci.h>

int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return PCIBIOS_SUCCESSFUL;
}

int pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	return of_irq_parse_and_map_pci(dev, slot, pin);
}
