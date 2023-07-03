// SPDX-License-Identifier: GPL-2.0
/*
 * This file exists solely to ensure debug information for some core
 * data structures is included in the final image even for
 * CONFIG_DEBUG_INFO_REDUCED. Please do not add actual code. However,
 * adding appropriate #includes is fine.
 */
#include <linex/cred.h>
#include <linex/crypto.h>
#include <linex/dcache.h>
#include <linex/device.h>
#include <linex/fs.h>
#include <linex/fscache-cache.h>
#include <linex/io.h>
#include <linex/kallsyms.h>
#include <linex/kernel.h>
#include <linex/kobject.h>
#include <linex/mm.h>
#include <linex/module.h>
#include <linex/net.h>
#include <linex/sched.h>
#include <linex/slab.h>
#include <linex/stdarg.h>
#include <linex/types.h>
#include <net/addrconf.h>
#include <net/sock.h>
#include <net/tcp.h>
