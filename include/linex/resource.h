/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_RESOURCE_H
#define _LINEX_RESOURCE_H

#include <uapi/linex/resource.h>


struct task_struct;

void getrusage(struct task_struct *p, int who, struct rusage *ru);

#endif
