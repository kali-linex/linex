// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2018, 2019 Oracle. All rights reserved.
 */

#include <linex/sunrpc/clnt.h>
#include <linex/sunrpc/sched.h>
#include <linex/sunrpc/svc.h>
#include <linex/sunrpc/svc_xprt.h>
#include <linex/sunrpc/auth_gss.h>
#include <linex/sunrpc/gss_err.h>

#define CREATE_TRACE_POINTS
#include <trace/events/rpcgss.h>
