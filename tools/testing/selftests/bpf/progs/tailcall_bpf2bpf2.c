// SPDX-License-Identifier: GPL-2.0
#include <linex/bpf.h>
#include <bpf/bpf_helpers.h>
#include "bpf_legacy.h"

struct {
	__uint(type, BPF_MAP_TYPE_PROG_ARRAY);
	__uint(max_entries, 1);
	__uint(key_size, sizeof(__u32));
	__uint(value_size, sizeof(__u32));
} jmp_table SEC(".maps");

static __noinline
int subprog_tail(struct __sk_buff *skb)
{
	if (load_byte(skb, 0))
		bpf_tail_call_static(skb, &jmp_table, 1);
	else
		bpf_tail_call_static(skb, &jmp_table, 0);
	return 1;
}

int count = 0;

SEC("tc")
int classifier_0(struct __sk_buff *skb)
{
	count++;
	return subprog_tail(skb);
}

SEC("tc")
int entry(struct __sk_buff *skb)
{
	bpf_tail_call_static(skb, &jmp_table, 0);

	return 0;
}

char __license[] SEC("license") = "GPL";
