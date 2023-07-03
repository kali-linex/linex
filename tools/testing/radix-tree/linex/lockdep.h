#ifndef _LINEX_LOCKDEP_H
#define _LINEX_LOCKDEP_H

#include <linex/spinlock.h>

struct lock_class_key {
	unsigned int a;
};

static inline void lockdep_set_class(spinlock_t *lock,
					struct lock_class_key *key)
{
}

extern int lockdep_is_held(const void *);
#endif /* _LINEX_LOCKDEP_H */
