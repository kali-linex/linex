/*
 * mmio_warning_test
 *
 * Copyright (C) 2019, Google LLC.
 *
 * This work is licensed under the terms of the GNU GPL, version 2.
 *
 * Test that we don't get a kernel warning when we call KVM_RUN after a
 * triple fault occurs.  To get the triple fault to occur we call KVM_RUN
 * on a VCPU that hasn't been properly setup.
 *
 */

#define _GNU_SOURCE
#include <fcntl.h>
#include <kvm_util.h>
#include <linex/kvm.h>
#include <processor.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <test_util.h>
#include <unistd.h>

#define NTHREAD 4
#define NPROCESS 5

struct thread_context {
	int kvmcpu;
	struct kvm_run *run;
};

void *thr(void *arg)
{
	struct thread_context *tc = (struct thread_context *)arg;
	int res;
	int kvmcpu = tc->kvmcpu;
	struct kvm_run *run = tc->run;

	res = ioctl(kvmcpu, KVM_RUN, 0);
	pr_info("ret1=%d exit_reason=%d suberror=%d\n",
		res, run->exit_reason, run->internal.suberror);

	return 0;
}

void test(void)
{
	int i, kvm, kvmvm, kvmcpu;
	pthread_t th[NTHREAD];
	struct kvm_run *run;
	struct thread_context tc;

	kvm = open("/dev/kvm", O_RDWR);
	TEST_ASSERT(kvm != -1, "failed to open /dev/kvm");
	kvmvm = __kvm_ioctl(kvm, KVM_CREATE_VM, NULL);
	TEST_ASSERT(kvmvm > 0, KVM_IOCTL_ERROR(KVM_CREATE_VM, kvmvm));
	kvmcpu = ioctl(kvmvm, KVM_CREATE_VCPU, 0);
	TEST_ASSERT(kvmcpu != -1, KVM_IOCTL_ERROR(KVM_CREATE_VCPU, kvmcpu));
	run = (struct kvm_run *)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED,
				    kvmcpu, 0);
	tc.kvmcpu = kvmcpu;
	tc.run = run;
	srand(getpid());
	for (i = 0; i < NTHREAD; i++) {
		pthread_create(&th[i], NULL, thr, (void *)(uintptr_t)&tc);
		usleep(rand() % 10000);
	}
	for (i = 0; i < NTHREAD; i++)
		pthread_join(th[i], NULL);
}

int get_warnings_count(void)
{
	int warnings;
	FILE *f;

	f = popen("dmesg | grep \"WARNING:\" | wc -l", "r");
	if (fscanf(f, "%d", &warnings) < 1)
		warnings = 0;
	pclose(f);

	return warnings;
}

int main(void)
{
	int warnings_before, warnings_after;

	TEST_REQUIRE(host_cpu_is_intel);

	TEST_REQUIRE(!vm_is_unrestricted_guest(NULL));

	warnings_before = get_warnings_count();

	for (int i = 0; i < NPROCESS; ++i) {
		int status;
		int pid = fork();

		if (pid < 0)
			exit(1);
		if (pid == 0) {
			test();
			exit(0);
		}
		while (waitpid(pid, &status, __WALL) != pid)
			;
	}

	warnings_after = get_warnings_count();
	TEST_ASSERT(warnings_before == warnings_after,
		   "Warnings found in kernel.  Run 'dmesg' to inspect them.");

	return 0;
}
