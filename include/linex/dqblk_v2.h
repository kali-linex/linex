/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  Definitions for vfsv0 quota format
 */

#ifndef _LINEX_DQBLK_V2_H
#define _LINEX_DQBLK_V2_H

#include <linex/dqblk_qtree.h>

/* Numbers of blocks needed for updates */
#define V2_INIT_ALLOC QTREE_INIT_ALLOC
#define V2_INIT_REWRITE QTREE_INIT_REWRITE
#define V2_DEL_ALLOC QTREE_DEL_ALLOC
#define V2_DEL_REWRITE QTREE_DEL_REWRITE

#endif /* _LINEX_DQBLK_V2_H */
