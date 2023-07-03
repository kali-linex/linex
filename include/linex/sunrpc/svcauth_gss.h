/* SPDX-License-Identifier: GPL-2.0 */
/*
 * linex/include/linex/sunrpc/svcauth_gss.h
 *
 * Bruce Fields <bfields@umich.edu>
 * Copyright (c) 2002 The Regents of the University of Michigan
 */

#ifndef _LINEX_SUNRPC_SVCAUTH_GSS_H
#define _LINEX_SUNRPC_SVCAUTH_GSS_H

#include <linex/sched.h>
#include <linex/sunrpc/types.h>
#include <linex/sunrpc/xdr.h>
#include <linex/sunrpc/svcauth.h>
#include <linex/sunrpc/svcsock.h>
#include <linex/sunrpc/auth_gss.h>

int gss_svc_init(void);
void gss_svc_shutdown(void);
int gss_svc_init_net(struct net *net);
void gss_svc_shutdown_net(struct net *net);
struct auth_domain *svcauth_gss_register_pseudoflavor(u32 pseudoflavor,
						      char *name);
u32 svcauth_gss_flavor(struct auth_domain *dom);

#endif /* _LINEX_SUNRPC_SVCAUTH_GSS_H */
