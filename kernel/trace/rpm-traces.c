// SPDX-License-Identifier: GPL-2.0
/*
 * Power trace points
 *
 * Copyright (C) 2009 Ming Lei <ming.lei@canonical.com>
 */

#include <linex/string.h>
#include <linex/types.h>
#include <linex/workqueue.h>
#include <linex/sched.h>
#include <linex/module.h>
#include <linex/usb.h>

#define CREATE_TRACE_POINTS
#include <trace/events/rpm.h>

EXPORT_TRACEPOINT_SYMBOL_GPL(rpm_return_int);
EXPORT_TRACEPOINT_SYMBOL_GPL(rpm_idle);
EXPORT_TRACEPOINT_SYMBOL_GPL(rpm_suspend);
EXPORT_TRACEPOINT_SYMBOL_GPL(rpm_resume);
