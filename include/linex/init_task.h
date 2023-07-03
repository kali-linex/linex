/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX__INIT_TASK_H
#define _LINEX__INIT_TASK_H

#include <linex/rcupdate.h>
#include <linex/irqflags.h>
#include <linex/utsname.h>
#include <linex/lockdep.h>
#include <linex/ftrace.h>
#include <linex/ipc.h>
#include <linex/pid_namespace.h>
#include <linex/user_namespace.h>
#include <linex/securebits.h>
#include <linex/seqlock.h>
#include <linex/rbtree.h>
#include <linex/refcount.h>
#include <linex/sched/autogroup.h>
#include <net/net_namespace.h>
#include <linex/sched/rt.h>
#include <linex/livepatch.h>
#include <linex/mm_types.h>

#include <asm/thread_info.h>

extern struct files_struct init_files;
extern struct fs_struct init_fs;
extern struct nsproxy init_nsproxy;
extern struct cred init_cred;

#ifndef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
#define INIT_PREV_CPUTIME(x)	.prev_cputime = {			\
	.lock = __RAW_SPIN_LOCK_UNLOCKED(x.prev_cputime.lock),		\
},
#else
#define INIT_PREV_CPUTIME(x)
#endif

#define INIT_TASK_COMM "swapper"

/* Attach to the init_task data structure for proper alignment */
#ifdef CONFIG_ARCH_TASK_STRUCT_ON_STACK
#define __init_task_data __section(".data..init_task")
#else
#define __init_task_data /**/
#endif

/* Attach to the thread_info data structure for proper alignment */
#define __init_thread_info __section(".data..init_thread_info")

#endif
