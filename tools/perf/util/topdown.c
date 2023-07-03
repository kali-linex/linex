// SPDX-License-Identifier: GPL-2.0
#include "topdown.h"
#include <linex/kernel.h>

__weak bool arch_topdown_sample_read(struct evsel *leader __maybe_unused)
{
	return false;
}
