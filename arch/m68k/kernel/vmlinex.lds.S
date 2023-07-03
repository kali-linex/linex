/* SPDX-License-Identifier: GPL-2.0 */
#if defined(CONFIG_MMU) && !defined(CONFIG_COLDFIRE)
PHDRS
{
  text PT_LOAD FILEHDR PHDRS FLAGS (7);
  data PT_LOAD FLAGS (7);
}
#ifdef CONFIG_SUN3
#include "vmlinex-sun3.lds"
#else
#include "vmlinex-std.lds"
#endif
#else
#include "vmlinex-nommu.lds"
#endif
