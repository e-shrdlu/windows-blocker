#define BPF_NO_GLOBAL_DATA // --> compatability, 
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
// #include <bpf/bpf_tracing.h> // don't need in XDP

const int pid_filter = 0;

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// sudo ls /sys/kernel/debug/tracing/events/syscalls
// SEC("tp/syscalls/sys_enter_write")
SEC("xdp")
int windows_blocker(struct xdp_md *ctx)
{
	// fuck em
	return XDP_DROP;

	// int pid = bpf_get_current_pid_tgid() >> 32;
	// if (pid_filter && pid != pid_filter) {
	// 	return 0;
	// }
	//
	// bpf_printk("BPF time fuck yeah, btw PID is %d\n", pid);
	// return 0;
}
