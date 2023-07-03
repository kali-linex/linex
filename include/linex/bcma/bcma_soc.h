/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINEX_BCMA_SOC_H_
#define LINEX_BCMA_SOC_H_

#include <linex/bcma/bcma.h>

struct bcma_soc {
	struct bcma_bus bus;
	struct device *dev;
};

int __init bcma_host_soc_register(struct bcma_soc *soc);
int __init bcma_host_soc_init(struct bcma_soc *soc);

int bcma_bus_register(struct bcma_bus *bus);

#endif /* LINEX_BCMA_SOC_H_ */
