// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2020 Facebook
#include <linex/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

struct {
	__uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
	__uint(max_entries, 1);
	__type(key, int);
	__type(value, int);
} array_1 SEC(".maps");

struct {
	__uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
	__uint(max_entries, 1);
	__type(key, int);
	__type(value, int);
	__uint(map_flags, BPF_F_PRESERVE_ELEMS);
} array_2 SEC(".maps");

SEC("raw_tp/sched_switch")
int BPF_PROG(read_array_1)
{
	struct bpf_perf_event_value val;

	return bpf_perf_event_read_value(&array_1, 0, &val, sizeof(val));
}

SEC("raw_tp/task_rename")
int BPF_PROG(read_array_2)
{
	struct bpf_perf_event_value val;

	return bpf_perf_event_read_value(&array_2, 0, &val, sizeof(val));
}

char LICENSE[] SEC("license") = "GPL";
