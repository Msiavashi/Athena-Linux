#ifndef ATHENA_H
#define ATHENA_H
#include <linux/types.h>
#include <linux/sched.h>

struct athena_pmc_callbacks {
	int (*pmcs_alloc_per_thread_data)(unsigned long, struct task_struct *);
	void (*pmcs_save_callback)(struct task_struct *, int);
	void (*pmcs_restore_callback)(struct task_struct *, int);
	void (*pmcs_tbs_tick)(struct task_struct *, int);
	void (*pmcs_exec_thread)(struct task_struct *);
	void (*pmcs_free_per_thread_data)(struct task_struct *);
	void (*pmcs_exit_thread)(struct task_struct *);
	int (*get_pmc_val)(struct task_struct *task, uint64_t pmc_event,
			   uint64_t *value);
};

int get_pmc_val(struct task_struct *task, uint64_t pmc_event, uint64_t *value);
int pmcs_alloc_per_thread_data(unsigned long clone_flags,
			       struct task_struct *p);
void pmcs_save_callback(struct task_struct *task, int cpu);
void pmcs_restore_callback(struct task_struct *task, int cpu);
void pmcs_tbs_tick(struct task_struct *task, int cpu);
void pmcs_exec_thread(struct task_struct *task);
void pmcs_free_per_thread_data(struct task_struct *task);
void pmcs_exit_thread(struct task_struct *task);
#endif