/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef LINEX_KALLSYMS_SELFTEST_H_
#define LINEX_KALLSYMS_SELFTEST_H_

#include <linex/types.h>

extern int kallsyms_test_var_bss;
extern int kallsyms_test_var_data;

extern int kallsyms_test_func(void);
extern int kallsyms_test_func_weak(void);

#endif // LINEX_KALLSYMS_SELFTEST_H_
