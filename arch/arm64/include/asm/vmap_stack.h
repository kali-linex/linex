/* SPDX-License-Identifier: GPL-2.0 */
// Copyright (C) 2017 Arm Ltd.
#ifndef __ASM_VMAP_STACK_H
#define __ASM_VMAP_STACK_H

#include <linex/bug.h>
#include <linex/gfp.h>
#include <linex/kconfig.h>
#include <linex/vmalloc.h>
#include <linex/pgtable.h>
#include <asm/memory.h>
#include <asm/thread_info.h>

/*
 * To ensure that VMAP'd stack overflow detection works correctly, all VMAP'd
 * stacks need to have the same alignment.
 */
static inline unsigned long *arch_alloc_vmap_stack(size_t stack_size, int node)
{
	void *p;

	BUILD_BUG_ON(!IS_ENABLED(CONFIG_VMAP_STACK));

	p = __vmalloc_node(stack_size, THREAD_ALIGN, THREADINFO_GFP, node,
			__builtin_return_address(0));
	return kasan_reset_tag(p);
}

#endif /* __ASM_VMAP_STACK_H */
