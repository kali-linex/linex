// SPDX-License-Identifier: GPL-2.0

#include <linex/ctype.h>
#include <linex/string.h>
#include <linex/types.h>

char *skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return (char *)str;
}
