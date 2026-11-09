obj-y := core.o
CFLAGS_core.o += $(call cc-disable-warning, override-init)

obj-$(CONFIG_BPF_SYSCALL) += syscall.o verifier.o inode.o helpers.o
obj-$(CONFIG_BPF_SYSCALL) += hashtab.o arraymap.o percpu_freelist.o bpf_lru_list.o lpm_trie.o map_in_map.o
ifeq ($(CONFIG_NET),y)
obj-$(CONFIG_BPF_SYSCALL) += devmap.o
endif
# herolte: stackmap.o (BPF_MAP_TYPE_STACK_TRACE) needs perf-callchain infra
# (sysctl_perf_event_max_stack, get_callchain_buffers, get_perf_callchain with
# the modern signature) absent from this 3.18 tree. Stack-trace maps are a
# tracing feature unused by cgroup-skb traffic accounting; nothing in the built
# set references bpf_get_stackid_proto, so leave stackmap.c unbuilt.
#ifeq ($(CONFIG_PERF_EVENTS),y)
#obj-$(CONFIG_BPF_SYSCALL) += stackmap.o
#endif
obj-$(CONFIG_CGROUP_BPF) += cgroup.o
