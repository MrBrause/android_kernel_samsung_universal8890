#ifndef _LINUX_CGROUP_DEFS_H
#define _LINUX_CGROUP_DEFS_H

#include <linux/types.h>

/*
 * herolte eBPF backport (Phase B2) — minimal sock_cgroup_data.
 *
 * On mainline this struct packs NET_PRIO prioidx and NET_CLASSID classid
 * alongside the cgroup pointer.  Both CONFIG_CGROUP_NET_PRIO and
 * CONFIG_CGROUP_NET_CLASSID are disabled in the herolte defconfig, so no
 * code ever reads those fields.  The only consumer here is
 * sock_cgroup_ptr() (see <linux/cgroup.h>), which in this backport always
 * resolves to the default unified-hierarchy root regardless of val.  We
 * therefore keep a single opaque u64 so the field has a stable size and
 * sk_cgrp_data can be zero-initialised like any other sock member.
 */
struct sock_cgroup_data {
	u64	val;
};

#endif /* _LINUX_CGROUP_DEFS_H */
