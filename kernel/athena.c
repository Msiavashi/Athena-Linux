#include <linux/athena.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/rwlock.h>
#include <linux/rcupdate.h>

extern unsigned int sysctl_athena_enable;
DEFINE_RWLOCK(pmc_ops_lock);

struct athena_pmc_callbacks pmc_callbacks;

EXPORT_SYMBOL(pmc_callbacks);

/* Invoked when forking a process/thread */
int pmcs_alloc_per_thread_data(unsigned long clone_flags, struct task_struct *p)
{
	int ret = 0;
	unsigned long flags;

	if (!sysctl_athena_enable)
		return 0;

	read_lock_irqsave(&pmc_ops_lock, flags);

	if (!pmc_callbacks.pmcs_alloc_per_thread_data) {
		read_unlock_irqrestore(&pmc_ops_lock, flags);
		return 0;
	}

	read_unlock_irqrestore(&pmc_ops_lock, flags);

	/* Invoke the allocation operation (may block) */
	ret = pmc_callbacks.pmcs_alloc_per_thread_data(clone_flags, p);

	return ret;
}

/* Invoked when a context switch out takes place */
void pmcs_save_callback(struct task_struct *task, int cpu)
{
	if (!sysctl_athena_enable)
		return;

	if (!pmc_callbacks.pmcs_save_callback)
		return;

	rcu_read_lock();

	pmc_callbacks.pmcs_save_callback(task, cpu);

	rcu_read_unlock();
}

/* Invoked when a context switch in takes place */
void pmcs_restore_callback(struct task_struct *task, int cpu)
{
	if (!sysctl_athena_enable)
		return;

	if (!pmc_callbacks.pmcs_restore_callback)
		return;

	rcu_read_lock();

	pmc_callbacks.pmcs_restore_callback(task, cpu);

	rcu_read_unlock();
}

/* Invoked from scheduler_tick() */
void pmcs_tbs_tick(struct task_struct *task, int cpu)
{
	if (!sysctl_athena_enable)
		return;

	if (!pmc_callbacks.pmcs_tbs_tick)
		return;

	rcu_read_lock();

	pmc_callbacks.pmcs_tbs_tick(task, cpu);

	rcu_read_unlock();
}

/* Invoked when a process calls exec() */
void pmcs_exec_thread(struct task_struct *task)
{
	if (!sysctl_athena_enable)
		return;

	if (!pmc_callbacks.pmcs_exec_thread)
		return;

	rcu_read_lock();

	pmc_callbacks.pmcs_exec_thread(task);

	rcu_read_unlock();
}

/* Invoked when the kernel frees up the process descriptor */
void pmcs_free_per_thread_data(struct task_struct *task)
{
	unsigned long flags;

	if (!sysctl_athena_enable)
		return;

	read_lock_irqsave(&pmc_ops_lock, flags);

	if (!pmc_callbacks.pmcs_free_per_thread_data) {
		read_unlock_irqrestore(&pmc_ops_lock, flags);
		return;
	}

	read_unlock_irqrestore(&pmc_ops_lock, flags);

	pmc_callbacks.pmcs_free_per_thread_data(task);
}

/* Invoked when a process exits */
void pmcs_exit_thread(struct task_struct *task)
{
	unsigned long flags;

	if (!sysctl_athena_enable)
		return;

	read_lock_irqsave(&pmc_ops_lock, flags);

	if (!pmc_callbacks.pmcs_exit_thread) {
		read_unlock_irqrestore(&pmc_ops_lock, flags);
		return;
	}

	read_unlock_irqrestore(&pmc_ops_lock, flags);

	pmc_callbacks.pmcs_exit_thread(task);
}

/* Call this function to get value of a counter */
int get_pmc_val(struct task_struct *task, uint64_t pmc_event, uint64_t *value)
{
	int ret = -1;

	if (!sysctl_athena_enable)
		return ret;

	if (!pmc_callbacks.get_pmc_val)
		return ret;

	rcu_read_lock();

	ret = pmc_callbacks.get_pmc_val(task, pmc_event, value);

	rcu_read_unlock();

	return ret;
}
