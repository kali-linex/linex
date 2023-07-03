#include <linex/stddef.h>
#include <linex/ipv6.h>
#include <linex/bpf.h>
#include <linex/in.h>
#include <sys/socket.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

SEC("freplace/do_bind")
int new_do_bind(struct bpf_sock_addr *ctx)
{
  struct sockaddr_in sa = {};

  bpf_bind(ctx, (struct sockaddr *)&sa, sizeof(sa));
  return 0;
}

char _license[] SEC("license") = "GPL";
