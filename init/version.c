// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linex/init/version.c
 *
 *  Copyright (C) 1992  Theodore Ts'o
 *
 *  May be freely distributed as part of Linex.
 */

#include <generated/compile.h>
#include <linex/build-salt.h>
#include <linex/elfnote-lto.h>
#include <linex/export.h>
#include <linex/init.h>
#include <linex/printk.h>
#include <linex/uts.h>
#include <linex/utsname.h>
#include <linex/proc_ns.h>

static int __init early_hostname(char *arg)
{
	size_t bufsize = sizeof(init_uts_ns.name.nodename);
	size_t maxlen  = bufsize - 1;
	size_t arglen;

	arglen = strlcpy(init_uts_ns.name.nodename, arg, bufsize);
	if (arglen > maxlen) {
		pr_warn("hostname parameter exceeds %zd characters and will be truncated",
			maxlen);
	}
	return 0;
}
early_param("hostname", early_hostname);

const char linex_proc_banner[] =
	"%s version %s"
	" (" LINEX_COMPILE_BY "@" LINEX_COMPILE_HOST ")"
	" (" LINEX_COMPILER ") %s\n";

BUILD_SALT;
BUILD_LTO_INFO;

/*
 * init_uts_ns and linex_banner contain the build version and timestamp,
 * which are really fixed at the very last step of build process.
 * They are compiled with __weak first, and without __weak later.
 */

struct uts_namespace init_uts_ns __weak;
const char linex_banner[] __weak;

#include "version-timestamp.c"

EXPORT_SYMBOL_GPL(init_uts_ns);
