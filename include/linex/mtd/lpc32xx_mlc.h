/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Platform data for LPC32xx SoC MLC NAND controller
 *
 * Copyright © 2012 Roland Stigge
 */

#ifndef __LINEX_MTD_LPC32XX_MLC_H
#define __LINEX_MTD_LPC32XX_MLC_H

#include <linex/dmaengine.h>

struct lpc32xx_mlc_platform_data {
	bool (*dma_filter)(struct dma_chan *chan, void *filter_param);
};

#endif  /* __LINEX_MTD_LPC32XX_MLC_H */
