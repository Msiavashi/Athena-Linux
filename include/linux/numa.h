/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NUMA_H
#define _LINUX_NUMA_H


#ifdef CONFIG_NODES_SHIFT
#define NODES_SHIFT     CONFIG_NODES_SHIFT
#else
#define NODES_SHIFT     0
#endif

#define MAX_NUMNODES    (1 << NODES_SHIFT)

#define	NUMA_NO_NODE	(-1)

#ifdef CONFIG_NUMA
#ifdef CONFIG_SCHED_ATHENA

/**
 * TODO: This should be dynamically get number of sockets
 * Currently this value only works for 2 socket machine.
 * For more socket machines, this should be increased.
 */
#define NR_NODES 2

#endif /* CONFIG_SCHED_ATHENA */
#endif

#endif /* _LINUX_NUMA_H */
