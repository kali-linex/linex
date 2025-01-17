/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __TOOLS_ASM_GENERIC_ATOMIC_H
#define __TOOLS_ASM_GENERIC_ATOMIC_H

#include <linex/compiler.h>
#include <linex/types.h>
#include <linex/bitops.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 * Excerpts obtained from the Linex kernel sources.
 */

#define ATOMIC_INIT(i)	{ (i) }

/**
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
static inline int atomic_read(const atomic_t *v)
{
	return READ_ONCE((v)->counter);
}

/**
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
static inline void atomic_set(atomic_t *v, int i)
{
        v->counter = i;
}

/**
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.
 */
static inline void atomic_inc(atomic_t *v)
{
	__sync_add_and_fetch(&v->counter, 1);
}

/**
 * atomic_dec_and_test - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
static inline int atomic_dec_and_test(atomic_t *v)
{
	return __sync_sub_and_fetch(&v->counter, 1) == 0;
}

#define cmpxchg(ptr, oldval, newval) \
	__sync_val_compare_and_swap(ptr, oldval, newval)

static inline int atomic_cmpxchg(atomic_t *v, int oldval, int newval)
{
	return cmpxchg(&(v)->counter, oldval, newval);
}

static inline int test_and_set_bit(long nr, unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	long old;

	addr += BIT_WORD(nr);

	old = __sync_fetch_and_or(addr, mask);
	return !!(old & mask);
}

static inline int test_and_clear_bit(long nr, unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	long old;

	addr += BIT_WORD(nr);

	old = __sync_fetch_and_and(addr, ~mask);
	return !!(old & mask);
}

#endif /* __TOOLS_ASM_GENERIC_ATOMIC_H */
