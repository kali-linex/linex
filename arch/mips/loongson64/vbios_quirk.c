// SPDX-License-Identifier: GPL-2.0+

#include <linex/pci.h>
#include <loongson.h>

static void pci_fixup_video(struct pci_dev *pdev)
{
	struct resource *res = &pdev->resource[PCI_ROM_RESOURCE];

	if (res->start)
		return;

	if (!loongson_sysconf.vgabios_addr)
		return;

	pci_disable_rom(pdev);
	if (res->parent)
		release_resource(res);

	res->start = virt_to_phys((void *) loongson_sysconf.vgabios_addr);
	res->end   = res->start + 256*1024 - 1;
	res->flags = IORESOURCE_MEM | IORESOURCE_ROM_SHADOW |
		     IORESOURCE_PCI_FIXED;

	dev_info(&pdev->dev, "Video device with shadowed ROM at %pR\n", res);
}
DECLARE_PCI_FIXUP_CLASS_HEADER(PCI_VENDOR_ID_ATI, 0x9615,
			       PCI_CLASS_DISPLAY_VGA, 8, pci_fixup_video);
