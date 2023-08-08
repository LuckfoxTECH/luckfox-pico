/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ATBM_WORKQUEUE_H
#define ATBM_WORKQUEUE_H
#ifdef CONFIG_ATBM_SELF_WORKQUEUE
//#define CONFIG_ATBM_WK_TEST
#define ATBM_WORK_STRUCT_PENDING_BIT	 0	/* work item is pending execution */
#define ATBM_WORK_STRUCT_DELAYED_BIT	 1	/* work item is delayed */
#define ATBM_WORK_STRUCT_WAIT_BIT		 2
#define ATBM_WORK_STRUCT_RUNNING_BIT	 3
#define ATBM_WORK_STRUCT_BARRIER_BIT	 4

#ifdef CONFIG_ATBM_WK_TEST
#define ATBM_WORK_STRUCT_TEST_BIT		 5
#define ATBM_WORK_STRUCT_TEST_PENDING_BIT	(ATBM_WORK_STRUCT_TEST_BIT+ATBM_WORK_STRUCT_PENDING_BIT)
#define ATBM_WORK_STRUCT_TEST_DELAYED_BIT	(ATBM_WORK_STRUCT_TEST_BIT+ATBM_WORK_STRUCT_DELAYED_BIT)
#define ATBM_WORK_STRUCT_TEST_WAIT_BIT		(ATBM_WORK_STRUCT_TEST_BIT+ATBM_WORK_STRUCT_WAIT_BIT)
#define ATBM_WORK_STRUCT_TEST_RUNNING_BIT	(ATBM_WORK_STRUCT_TEST_BIT+ATBM_WORK_STRUCT_RUNNING_BIT)
#endif
#define atbm_work_data_bits(work) ((unsigned long *)(&(work)->data))

struct atbm_workqueue_struct;
struct atbm_work_struct;
struct atbm_workqueue_struct{
	struct list_head	works;
	struct list_head    flush_works;
	struct list_head    delay_works;
	struct task_struct		__rcu *work_thread;
	unsigned long		flags;
	spinlock_t			lock;
	bool 				flush_empty;
	wait_queue_head_t	wait_flushed;
	const char *name;
	struct mutex		mutex;
};
typedef void (*atbm_work_func_t)(struct atbm_work_struct *work);

struct atbm_work_struct{
	unsigned long data;
	struct list_head entry;
	atbm_work_func_t func;
	spinlock_t	  lock;
	wait_queue_head_t	timeout;
	wait_queue_head_t	barrier;
	struct atbm_workqueue_struct *wq;
	const char *name;
#ifdef CONFIG_ATBM_WK_TEST
	wait_queue_head_t	running;
	wait_queue_head_t	pending;
	wait_queue_head_t	delayed;
#endif	
};
struct atbm_delayed_work{
	struct atbm_work_struct work;
	struct atbm_timer_list timer;
	struct atbm_workqueue_struct __rcu *wq;
	int n_started;
	int n_timeout;
};
void ieee80211_atbm_init_work(struct atbm_work_struct *work,atbm_work_func_t func,const char *name);
void ieee80211_atbm_init_delay_work(struct atbm_delayed_work *work,atbm_work_func_t func,const char *name);
struct atbm_workqueue_struct  *ieee80211_atbm_alloc_workqueue(const char *name,long flags);
bool ieee80211_atbm_queue_work(struct atbm_workqueue_struct *wq,struct atbm_work_struct *work);
bool ieee80211_atbm_queue_delayed_work(struct atbm_workqueue_struct *wq,struct atbm_delayed_work *dwork, unsigned long delay);
void ieee80211_atbm_flush_workqueue(struct atbm_workqueue_struct *wq);
void ieee80211_atbm_destroy_workqueue(struct atbm_workqueue_struct *wq);
bool ieee80211_atbm_flush_delayed_work(struct atbm_delayed_work *dwork);
int ieee80211_atbm_schedule_work(struct atbm_work_struct *work);
bool ieee80211_atbm_cancel_delayed_work_sync(struct atbm_delayed_work *dwork);
bool ieee80211_atbm_cancel_delayed_work(struct atbm_delayed_work *dwork);
bool ieee80211_atbm_cancel_work_sync(struct atbm_work_struct *work);
bool ieee80211_atbm_flush_work(struct atbm_work_struct *work);
void ieee80211_atbm_workqueue_init(void);
void ieee80211_atbm_workqueue_exit(void);

#define atbm_cancel_delayed_work 		ieee80211_atbm_cancel_delayed_work	
#define atbm_cancel_delayed_work_sync	ieee80211_atbm_cancel_delayed_work_sync
#define atbm_cancel_work_sync			ieee80211_atbm_cancel_work_sync
#define atbm_queue_work					ieee80211_atbm_queue_work
#define atbm_queue_delayed_work			ieee80211_atbm_queue_delayed_work
#define atbm_flush_workqueue			ieee80211_atbm_flush_workqueue
#define ATBM_INIT_WORK(_wk,_f)			ieee80211_atbm_init_work(_wk,_f,#_f);
#define atbm_create_singlethread_workqueue(_name)	ieee80211_atbm_alloc_workqueue(_name,0);
#define atbm_destroy_workqueue			ieee80211_atbm_destroy_workqueue
#define ATBM_INIT_DELAYED_WORK(_dwk,_f)	ieee80211_atbm_init_delay_work(_dwk,_f,#_f)
#define atbm_flush_delayed_work			ieee80211_atbm_flush_delayed_work
#define atbm_schedule_work				ieee80211_atbm_schedule_work
#define atbm_alloc_ordered_workqueue	ieee80211_atbm_alloc_workqueue
#define atbm_work_pending(wk)			test_bit(ATBM_WORK_STRUCT_PENDING_BIT, atbm_work_data_bits(wk))
#define atbm_flush_work					ieee80211_atbm_flush_work
#define atbm_wq_init()					ieee80211_atbm_workqueue_init()
#define atbm_wq_exit()					ieee80211_atbm_workqueue_exit()

static inline struct atbm_delayed_work *atbm_to_delayed_work(struct atbm_work_struct *work)
{
	return container_of(work, struct atbm_delayed_work, work);
}

#else
#define atbm_cancel_delayed_work		cancel_delayed_work
#define atbm_cancel_delayed_work_sync	cancel_delayed_work_sync
#define atbm_cancel_work_sync			cancel_work_sync
#define atbm_queue_work					queue_work
#define atbm_queue_delayed_work			queue_delayed_work
#define atbm_flush_workqueue			flush_workqueue
#define ATBM_INIT_WORK					INIT_WORK
#define atbm_create_singlethread_workqueue	create_singlethread_workqueue
#define atbm_destroy_workqueue			destroy_workqueue
#define ATBM_INIT_DELAYED_WORK			INIT_DELAYED_WORK
#define atbm_flush_delayed_work			flush_delayed_work
#define atbm_schedule_work				schedule_work
#define atbm_alloc_ordered_workqueue	alloc_ordered_workqueue
#define atbm_work_pending				work_pending
#define atbm_to_delayed_work			to_delayed_work
#define atbm_flush_work					flush_work
#define atbm_wq_init()
#define atbm_wq_exit()
#endif
#endif /* ATBM_WORKQUEUE_H */
