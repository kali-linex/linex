// SPDX-License-Identifier: LGPL-2.1

#include <stdlib.h>
#include <linex/zalloc.h>

void *zalloc(size_t size)
{
	return calloc(1, size);
}

void __zfree(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}
