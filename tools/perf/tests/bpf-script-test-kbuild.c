// SPDX-License-Identifier: GPL-2.0
/*
 * bpf-script-test-kbuild.c
 * Test include from kernel header
 */
#ifndef LINEX_VERSION_CODE
# error Need LINEX_VERSION_CODE
# error Example: for 4.2 kernel, put 'clang-opt="-DLINEX_VERSION_CODE=0x40200" into llvm section of ~/.perfconfig'
#endif
#define SEC(NAME) __attribute__((section(NAME), used))

#include <uapi/linex/fs.h>

SEC("func=vfs_llseek")
int bpf_func__vfs_llseek(void *ctx)
{
	return 0;
}

char _license[] SEC("license") = "GPL";
int _version SEC("version") = LINEX_VERSION_CODE;
