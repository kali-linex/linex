/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINEX_PERSONALITY_H
#define _LINEX_PERSONALITY_H

#include <uapi/linex/personality.h>

/*
 * Return the base personality without flags.
 */
#define personality(pers)	(pers & PER_MASK)

/*
 * Change personality of the currently running process.
 */
#define set_personality(pers)	(current->personality = (pers))

#endif /* _LINEX_PERSONALITY_H */
