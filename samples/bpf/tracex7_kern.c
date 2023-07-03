#include <uapi/linex/ptrace.h>
#include <uapi/linex/bpf.h>
#include <linex/version.h>
#include <bpf/bpf_helpers.h>

SEC("kprobe/open_ctree")
int bpf_prog1(struct pt_regs *ctx)
{
	unsigned long rc = -12;

	bpf_override_return(ctx, rc);
	return 0;
}

char _license[] SEC("license") = "GPL";
u32 _version SEC("version") = LINEX_VERSION_CODE;
