/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINEX
 *		operating system.  INET is implemented using the BSD Socket
 *		interface as the means of communication with the user level.
 *
 * Authors:	Lotsa people, from code originally in tcp
 */

#ifndef _INET6_HASHTABLES_H
#define _INET6_HASHTABLES_H


#if IS_ENABLED(CONFIG_IPV6)
#include <linex/in6.h>
#include <linex/ipv6.h>
#include <linex/types.h>
#include <linex/jhash.h>

#include <net/inet_sock.h>

#include <net/ipv6.h>
#include <net/netns/hash.h>

struct inet_hashinfo;

static inline unsigned int __inet6_ehashfn(const u32 lhash,
				    const u16 lport,
				    const u32 fhash,
				    const __be16 fport,
				    const u32 initval)
{
	const u32 ports = (((u32)lport) << 16) | (__force u32)fport;
	return jhash_3words(lhash, fhash, ports, initval);
}

/*
 * Sockets in TCP_CLOSE state are _always_ taken out of the hash, so
 * we need not check it for TCP lookups anymore, thanks Alexey. -DaveM
 *
 * The sockhash lock must be held as a reader here.
 */
struct sock *__inet6_lookup_established(struct net *net,
					struct inet_hashinfo *hashinfo,
					const struct in6_addr *saddr,
					const __be16 sport,
					const struct in6_addr *daddr,
					const u16 hnum, const int dif,
					const int sdif);

struct sock *inet6_lookup_listener(struct net *net,
				   struct inet_hashinfo *hashinfo,
				   struct sk_buff *skb, int doff,
				   const struct in6_addr *saddr,
				   const __be16 sport,
				   const struct in6_addr *daddr,
				   const unsigned short hnum,
				   const int dif, const int sdif);

static inline struct sock *__inet6_lookup(struct net *net,
					  struct inet_hashinfo *hashinfo,
					  struct sk_buff *skb, int doff,
					  const struct in6_addr *saddr,
					  const __be16 sport,
					  const struct in6_addr *daddr,
					  const u16 hnum,
					  const int dif, const int sdif,
					  bool *refcounted)
{
	struct sock *sk = __inet6_lookup_established(net, hashinfo, saddr,
						     sport, daddr, hnum,
						     dif, sdif);
	*refcounted = true;
	if (sk)
		return sk;
	*refcounted = false;
	return inet6_lookup_listener(net, hashinfo, skb, doff, saddr, sport,
				     daddr, hnum, dif, sdif);
}

static inline struct sock *__inet6_lookup_skb(struct inet_hashinfo *hashinfo,
					      struct sk_buff *skb, int doff,
					      const __be16 sport,
					      const __be16 dport,
					      int iif, int sdif,
					      bool *refcounted)
{
	struct sock *sk = skb_steal_sock(skb, refcounted);

	if (sk)
		return sk;

	return __inet6_lookup(dev_net(skb_dst(skb)->dev), hashinfo, skb,
			      doff, &ipv6_hdr(skb)->saddr, sport,
			      &ipv6_hdr(skb)->daddr, ntohs(dport),
			      iif, sdif, refcounted);
}

struct sock *inet6_lookup(struct net *net, struct inet_hashinfo *hashinfo,
			  struct sk_buff *skb, int doff,
			  const struct in6_addr *saddr, const __be16 sport,
			  const struct in6_addr *daddr, const __be16 dport,
			  const int dif);

int inet6_hash(struct sock *sk);

static inline bool inet6_match(struct net *net, const struct sock *sk,
			       const struct in6_addr *saddr,
			       const struct in6_addr *daddr,
			       const __portpair ports,
			       const int dif, const int sdif)
{
	if (!net_eq(sock_net(sk), net) ||
	    sk->sk_family != AF_INET6 ||
	    sk->sk_portpair != ports ||
	    !ipv6_addr_equal(&sk->sk_v6_daddr, saddr) ||
	    !ipv6_addr_equal(&sk->sk_v6_rcv_saddr, daddr))
		return false;

	/* READ_ONCE() paired with WRITE_ONCE() in sock_bindtoindex_locked() */
	return inet_sk_bound_dev_eq(net, READ_ONCE(sk->sk_bound_dev_if), dif,
				    sdif);
}
#endif /* IS_ENABLED(CONFIG_IPV6) */

#endif /* _INET6_HASHTABLES_H */
