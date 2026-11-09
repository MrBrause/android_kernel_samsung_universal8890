#ifndef _LINUX_ONCE_H
#define _LINUX_ONCE_H

#include <linux/types.h>
#include <linux/compiler.h>

/*
 * Minimal DO_ONCE() backport for the 3.18 tree (herolte).
 *
 * Upstream's DO_ONCE() uses static-key patching plus lib/once.c
 * (__do_once_start()/__do_once_done()), neither of which exists in this tree.
 * The only in-tree users are prandom_init_once() (percpu PRNG seeding) and the
 * modern eBPF core's bpf_user_rnd_init_once(). For those, a benign double-run
 * on a rare race simply reseeds, so a plain one-shot guard is sufficient and
 * needs no new .c file or static-key infrastructure.
 */
#define DO_ONCE(func, ...)						\
	({								\
		static bool ___done __read_mostly;			\
		bool ___ret = false;					\
		if (unlikely(!READ_ONCE(___done))) {			\
			WRITE_ONCE(___done, true);			\
			func(__VA_ARGS__);				\
			___ret = true;					\
		}							\
		unlikely(___ret);					\
	})

#define get_random_once(buf, nbytes)					\
	DO_ONCE(get_random_bytes, (buf), (nbytes))

#endif /* _LINUX_ONCE_H */
