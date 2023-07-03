/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2021 Hengqi Chen */

#include <test_progs.h>
#include <bpf/btf.h>

static const char *module_name = "bpf_testmod";
static const char *symbol_name = "bpf_testmod_test_read";

void test_btf_module()
{
	struct btf *vmlinex_btf, *module_btf;
	__s32 type_id;

	if (!env.has_testmod) {
		test__skip();
		return;
	}

	vmlinex_btf = btf__load_vmlinex_btf();
	if (!ASSERT_OK_PTR(vmlinex_btf, "could not load vmlinex BTF"))
		return;

	module_btf = btf__load_module_btf(module_name, vmlinex_btf);
	if (!ASSERT_OK_PTR(module_btf, "could not load module BTF"))
		goto cleanup;

	type_id = btf__find_by_name(module_btf, symbol_name);
	ASSERT_GT(type_id, 0, "func not found");

cleanup:
	btf__free(module_btf);
	btf__free(vmlinex_btf);
}
