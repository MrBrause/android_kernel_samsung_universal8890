# Out-of-tree patches

Standalone patches kept with this kernel tree for reference and for
re-applying to a clean checkout.

## android14-ebpf-data-usage.patch

Backports the Android 14 eBPF network-traffic-accounting stack onto the
stock herolte 3.18 kernel, restoring **Settings → Network & internet →
Data usage** (per-app and totals) for mobile, Wi-Fi and tethering.

Android 14 dropped `xt_qtaguid` and accounts all traffic through the
modern eBPF path (netbpfload → cgroup-skb programs attached to the
cgroup v2 root → per-uid maps → NetworkStatsService). The stock 3.18
kernel had none of that, so data usage read zero. This cumulative patch
brings up the whole stack:

- Modern BPF subsystem (verifier, maps, syscall, JIT glue)
- Mountable cgroup2 filesystem (additive; stock cgroup v1 path untouched)
- cgroup-BPF attach — `CONFIG_CGROUP_BPF`, `struct sock.sk_cgrp_data`,
  and the attach/run machinery; this is what turns per-uid accounting on
- Socket-cookie infrastructure — `SO_COOKIE`, `sock_gen_cookie()`,
  `bpf_get_socket_cookie()` — for socket tagging
- `xt_bpf` revision 1 / pinned-program mode, so netd's
  BandwidthController can build its `bw_*` iptables chains

The `CONFIG_CGROUP_BPF=y` defconfig change is included in the patch, so
no manual config edit is needed.

**Source:** the net/, bpf and netfilter leaf helpers were forward-ported
from the sibling Samsung Exynos 3.18.140 tree
<https://github.com/Exynos7870-Revived-Beta/android_kernel_samsung_exynos7870/tree/lineage-19.1-bpf>
(same kernel base as universal8890), with the remainder backported from
mainline.

### Applies to

This patch applies cleanly to the **pristine** LineageOS 21 herolte
kernel at:

    abf3431fdee1b5a1c06e4d0766b58966cea2449c        # the commit immediately before the backport

It is the exact diff that was squashed into the backport commit, so it
can also be regenerated with `git format-patch -1 <backport commit>`.

### Apply

Reset the kernel source to the pristine base, then:

    git apply patches/android14-ebpf-data-usage.patch
    # or, without a git worktree:
    patch -p1 < patches/android14-ebpf-data-usage.patch

### Build note

Build **out of tree** (`O=…`, as the LineageOS build system does). A
stray in-source `.config` or `include/config/` trips the kernel's
`prepare3` "source tree is not clean" guard. If you hit it, clean the
source dir — this does **not** touch the applied patch's source files,
so there is no need to re-apply:

    rm -f  .config .config.old
    rm -rf include/config include/generated
    # or: make mrproper

### Tested

herolte (SM-G930F), Android 14 / LineageOS 21: mobile, Wi-Fi and
USB/Wi-Fi tethering all accounted, with the per-app breakdown, in
Settings and `dumpsys netstats --uid`.
