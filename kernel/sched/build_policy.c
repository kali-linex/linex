// SPDX-License-Identifier: GPL-2.0-only
/*
 * These are the scheduling policy related scheduler files, built
 * in a single compilation unit for build efficiency reasons.
 *
 * ( Incidentally, the size of the compilation unit is roughly
 *   comparable to core.c and fair.c, the other two big
 *   compilation units. This helps balance build time, while
 *   coalescing source files to amortize header inclusion
 *   cost. )
 *
 * core.c and fair.c are built separately.
 */

/* Headers: */
#include <linex/sched/clock.h>
#include <linex/sched/cputime.h>
#include <linex/sched/hotplug.h>
#include <linex/sched/posix-timers.h>
#include <linex/sched/rt.h>

#include <linex/cpuidle.h>
#include <linex/jiffies.h>
#include <linex/livepatch.h>
#include <linex/psi.h>
#include <linex/seqlock_api.h>
#include <linex/slab.h>
#include <linex/suspend.h>
#include <linex/tsacct_kern.h>
#include <linex/vtime.h>

#include <uapi/linex/sched/types.h>

#include "sched.h"
#include "smp.h"

#include "autogroup.h"
#include "stats.h"
#include "pelt.h"

/* Source code modules: */

#include "idle.c"

#include "rt.c"

#ifdef CONFIG_SMP
# include "cpudeadline.c"
# include "pelt.c"
#endif

#include "cputime.c"
#include "deadline.c"

