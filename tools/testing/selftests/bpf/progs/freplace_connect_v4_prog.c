// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2020 Facebook

#include <linex/stddef.h>
#include <linex/ipv6.h>
#include <linex/bpf.h>
#include <linex/in.h>
#include <sys/socket.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

SEC("freplace/connect_v4_prog")
int new_connect_v4_prog(struct bpf_sock_addr *ctx)
{
	// return value thats in invalid range
	return 255;
}

char _license[] SEC("license") = "GPL";
