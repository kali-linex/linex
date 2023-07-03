/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_KMSAN_H
#define _LINEX_KMSAN_H

#include <linex/gfp.h>

inline void kmsan_handle_dma(struct page *page, size_t offset, size_t size,
			     enum dma_data_direction dir)
{
}

#endif /* _LINEX_KMSAN_H */
