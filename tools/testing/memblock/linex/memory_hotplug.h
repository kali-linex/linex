/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_MEMORY_HOTPLUG_H
#define _LINEX_MEMORY_HOTPLUG_H

#include <linex/numa.h>
#include <linex/pfn.h>
#include <linex/cache.h>
#include <linex/types.h>

extern bool movable_node_enabled;

static inline bool movable_node_is_enabled(void)
{
	return movable_node_enabled;
}

#endif
