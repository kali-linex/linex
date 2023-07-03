/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_BUILDID_H
#define _LINEX_BUILDID_H

#include <linex/mm_types.h>

#define BUILD_ID_SIZE_MAX 20

int build_id_parse(struct vm_area_struct *vma, unsigned char *build_id,
		   __u32 *size);
int build_id_parse_buf(const void *buf, unsigned char *build_id, u32 buf_size);

#if IS_ENABLED(CONFIG_STACKTRACE_BUILD_ID) || IS_ENABLED(CONFIG_CRASH_CORE)
extern unsigned char vmlinex_build_id[BUILD_ID_SIZE_MAX];
void init_vmlinex_build_id(void);
#else
static inline void init_vmlinex_build_id(void) { }
#endif

#endif
