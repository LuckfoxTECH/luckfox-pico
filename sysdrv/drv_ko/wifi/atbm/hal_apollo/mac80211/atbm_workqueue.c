/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifdef CONFIG_ATBM_SELF_WORKQUEUE
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/gfp.h>
#include <asm/unaligned.h>
#include <net/atbm_mac80211.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <net/atbm_mac80211.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/freezer.h>
#include "atbm_workqueue.h"
#include "ieee80211_atbm_mem.h"
enum{
	WQ_THREAD_WAKEUP,
	WQ_THREAD_SHOULD_SUSPEND,
	WQ_THREAD_SUSPENED,
	WQ_THREAD_SHOULD_STOP,
};
struct work_wait{
	struct atbm_work_struct work;
};

#define WQ_THREAD_PR	0

static struct atbm_workqueue_struct  *common_wq = NULL;
/*
*test wk bit
*/
#define wk_is_pending(wk)		test_bit(ATBM_WORK_STRUCT_PENDING_BIT,atbm_work_data_bits(wk))
#define wk_is_delayed(wk)		test_bit(ATBM_WORK_STRUCT_DELAYED_BIT,atbm_work_data_bits(wk))
#define wk_is_running(wk)		test_bit(ATBM_WORK_STRUCT_RUNNING_BIT,atbm_work_data_bits(wk))
#define wk_is_waiting(wk)		test_bit(ATBM_WORK_STRUCT_WAIT_BIT,atbm_work_data_bits(wk))
#define wk_is_barrier(wk)		test_bit(ATBM_WORK_STRUCT_BARRIER_BIT,atbm_work_data_bits(wk))

/*
*clear wk bit
*/
#define wk_clear_pending(wk)	clear_bit(ATBM_WORK_STRUCT_PENDING_BIT,atbm_work_data_bits(wk))
#define wk_clear_delayed(wk)	clear_bit(ATBM_WORK_STRUCT_DELAYED_BIT,atbm_work_data_bits(wk))
#define wk_clear_running(wk)	clear_bit(ATBM_WORK_STRUCT_RUNNING_BIT,atbm_work_data_bits(wk))
#define wk_clear_waiting(wk)	clear_bit(ATBM_WORK_STRUCT_WAIT_BIT,atbm_work_data_bits(wk))
#define wk_clear_barrier(wk)	clear_bit(ATBM_WORK_STRUCT_BARRIER_BIT,atbm_work_data_bits(wk))

/*
*set wk bit
*/
#define wk_set_pending(wk)		set_bit(ATBM_WORK_STRUCT_PENDING_BIT,atbm_work_data_bits(wk))
#define wk_set_delayed(wk)		set_bit(ATBM_WORK_STRUCT_DELAYED_BIT,atbm_work_data_bits(wk))
#define wk_set_running(wk)		set_bit(ATBM_WORK_STRUCT_RUNNING_BIT,atbm_work_data_bits(wk))
#define wk_set_waiting(wk)		set_bit(ATBM_WORK_STRUCT_WAIT_BIT,atbm_work_data_bits(wk))
#define wk_set_barrier(wk)		set_bit(ATBM_WORK_STRUCT_BARRIER_BIT,atbm_work_data_bits(wk))


#define wk_is_busy(_wk)	(wk_is_pending(_wk) ||  wk_is_delayed(_wk) || wk_is_running(_wk))

#ifdef CONFIG_ATBM_WK_TEST
static struct task_struct *test_thread = NULL;
#define WQ_THREAD_TEST_PR WQ_THREAD_PR+1

#define wk_is_testpending(wk)	test_bit(ATBM_WORK_STRUCT_TEST_PENDING_BIT,atbm_work_data_bits(wk))
#define wk_is_testdelayed(wk)	test_bit(ATBM_WORK_STRUCT_TEST_DELAYED_BIT,atbm_work_data_bits(wk))
#define wk_is_testrunning(wk)	test_bit(ATBM_WORK_STRUCT_TEST_RUNNING_BIT,atbm_work_data_bits(wk))
#define wk_is_testwaiting(wk)	test_bit(ATBM_WORK_STRUCT_TEST_WAIT_BIT,atbm_work_data_bits(wk))

#define wk_clear_testpending(wk)	clear_bit(ATBM_WORK_STRUCT_TEST_PENDING_BIT,atbm_work_data_bits(wk))
#define wk_clear_testdelayed(wk)	clear_bit(ATBM_WORK_STRUCT_TEST_DELAYED_BIT,atbm_work_data_bits(wk))
#define wk_clear_testrunning(wk)	clear_bit(ATBM_WORK_STRUCT_TEST_RUNNING_BIT,atbm_work_data_bits(wk))
#define wk_clear_testwaiting(wk)	clear_bit(ATBM_WORK_STRUCT_TEST_WAIT_BIT,atbm_work_data_bits(wk))

#define wk_set_testpending(wk)	set_bit(ATBM_WORK_STRUCT_TEST_PENDING_BIT,atbm_work_data_bits(wk))
#define wk_set_testdelayed(wk)	set_bit(ATBM_WORK_STRUCT_TEST_DELAYED_BIT,atbm_work_data_bits(wk))
#define wk_set_testrunning(wk)	set_bit(ATBM_WORK_STRUCT_TEST_RUNNING_BIT,atbm_work_data_bits(wk))
#define wk_set_testwaiting(wk)	set_bit(ATBM_WORK_STRUCT_TEST_WAIT_BIT,atbm_work_data_bits(wk))

static void atbm_workqueue_test_work(struct atbm_work_struct *work)
{
	
}
static void atbm_workqueue_test_delay_work(struct atbm_work_struct *work)
{
	
}
static void atbm_wk_test_init(struct atbm_work_struct *work)
{
	init_waitqueue_head(&work->running);
	init_waitqueue_head(&work->pending);
	init_waitqueue_head(&work->delayed);
}
static int atbm_workqueue_test_thread(void *priv)
{
	struct sched_param param = { .sched_priority = WQ_THREAD_TEST_PR };
	struct atbm_work_struct work;
	struct atbm_delayed_work delay_work;
	struct atbm_workqueue_struct *wq = NULL;
	bool ret = false;
	
	ieee80211_atbm_init_work(&work,atbm_workqueue_test_work,"test_work");
	ieee80211_atbm_init_delay_work(&delay_work,atbm_workqueue_test_delay_work,"test_delay_work");
	/*
	*the policy of the sheduler is same with the sdio irq thread
	*/
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 9, 0))
	sched_set_fifo(current);
#else
	sched_setscheduler(current, SCHED_FIFO, &param);
#endif	
	set_current_state(TASK_INTERRUPTIBLE);	
	while (!kthread_should_stop()) {

		set_current_state(TASK_INTERRUPTIBLE);
		if (!kthread_should_stop())
			schedule_timeout(2*HZ);
		__set_current_state(TASK_RUNNING);
		
		wq = ieee80211_atbm_alloc_workqueue("wq_test",0);

		if(wq == NULL){
			atbm_printk_always("%s err\n",__func__);
			break;
		}
		/*
		*[TEST] flush pending work function
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WK] flush pending work\n");
		ieee80211_atbm_queue_work(wq,&work);
		ret = ieee80211_atbm_flush_work(&work);
		atbm_printk_always("[TEST WK] flush pending work finish(%d)\n",ret);
		/*
		*[TEST] flush running work function
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WK] flush running work\n");
		wk_set_testrunning(&work);
		ieee80211_atbm_queue_work(wq,&work);
		wait_event_interruptible(work.running,wk_is_testrunning(&work) == 0);
		ret = ieee80211_atbm_flush_work(&work);
		atbm_printk_always("[TEST WK] flush running work finish(%d)\n",ret);
		/*
		*[TEST] concle work pending
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WK] concle work pending\n");
		ieee80211_atbm_queue_work(wq,&work);
		ret = ieee80211_atbm_cancel_work_sync(&work);
		atbm_printk_always("[TEST WK] concle work pending finish(%d)\n",ret);
		/*
		*[TEST] concle work running
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WK] concle work running\n");
		wk_set_testrunning(&work);
		ieee80211_atbm_queue_work(wq,&work);
		wait_event_interruptible(work.running,wk_is_testrunning(&work) == 0);
		ret = ieee80211_atbm_cancel_work_sync(&work);
		atbm_printk_always("[TEST WK] concle work running(%d)\n",ret);
		/*
		*[TEST] concle dup work
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WK] concle dup work\n");
		wk_set_testrunning(&work);
		ieee80211_atbm_queue_work(wq,&work);
		wait_event_interruptible(work.running,wk_is_testrunning(&work) == 0);
		ieee80211_atbm_queue_work(wq,&work);
		ret = ieee80211_atbm_cancel_work_sync(&work);
		atbm_printk_always("[TEST WK] concle dup work (%d)\n",ret);
		/*
		*[TEST] queue dup work
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WK] que dup work\n");
		ieee80211_atbm_queue_work(wq,&work);
		ret = ieee80211_atbm_queue_work(wq,&work);
		atbm_printk_always("[TEST WK] que dup work (%d)\n",ret);
		////////////////////////////////////////////////////////////
		/*
		*[TEST] flush delayed work
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] flush delayed work\n");
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		ret = ieee80211_atbm_flush_delayed_work(&delay_work);
		atbm_printk_always("[TEST DWK] flush delayed work (%d)\n",ret);
		/*
		*[TEST] flush delayed work already in pending
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] flush pending work\n");
		wk_set_testpending(&delay_work.work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		wait_event_interruptible(delay_work.work.pending,wk_is_testpending(&delay_work.work) == 0);
		ret = ieee80211_atbm_flush_delayed_work(&delay_work);
		atbm_printk_always("[TEST DWK] flush pending work (%d)\n",ret);
		/*
		*[TEST] flush delayed work already in running
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] flush running work\n");
		wk_set_testrunning(&delay_work.work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		wait_event_interruptible(delay_work.work.running,wk_is_testrunning(&delay_work.work) == 0);
		ret = ieee80211_atbm_flush_delayed_work(&delay_work);
		atbm_printk_always("[TEST DWK] flush running work(%d)\n",ret);

		///////////////////////////////////////////////////////////////
		/*
		*[TEST] concle delayed work
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] concle delayed work\n");
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		ret = ieee80211_atbm_cancel_delayed_work(&delay_work);
		atbm_printk_always("[TEST DWK] concle delayed work(%d)\n",ret);
		/*
		*[TEST] concle pending work
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] concle pending work\n");
		wk_set_testpending(&delay_work.work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		wait_event_interruptible(delay_work.work.pending,wk_is_testpending(&delay_work.work) == 0);
		ret = ieee80211_atbm_cancel_delayed_work(&delay_work);
		atbm_printk_always("[TEST DWK] concle pending work(%d)\n",ret);
		/*
		*[TEST] concle running work
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] concle running work\n");
		wk_set_testrunning(&delay_work.work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		wait_event_interruptible(delay_work.work.running,wk_is_testrunning(&delay_work.work) == 0);
		ret = ieee80211_atbm_cancel_delayed_work(&delay_work);
		atbm_printk_always("[TEST DWK] concle running work(%d)\n",ret);

		//////////////////////////////////////////////////////////////
		/*
		*[TEST] concle delayed work sync
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] concle delayed work sync\n");
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		ret = ieee80211_atbm_cancel_delayed_work_sync(&delay_work);
		atbm_printk_always("[TEST DWK] concle delayed work sync(%d)\n",ret);
		/*
		*[TEST] concle pending work sync
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] concle pending work sync\n");
		wk_set_testpending(&delay_work.work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		wait_event_interruptible(delay_work.work.pending,wk_is_testpending(&delay_work.work) == 0);
		ret = ieee80211_atbm_cancel_delayed_work_sync(&delay_work);
		atbm_printk_always("[TEST DWK] concle pending work sync(%d)\n",ret);
		/*
		*[TEST] concle running work sync
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST DWK] concle running work sync\n");
		wk_set_testrunning(&delay_work.work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		wait_event_interruptible(delay_work.work.running,wk_is_testrunning(&delay_work.work) == 0);
		ret = ieee80211_atbm_cancel_delayed_work_sync(&delay_work);
		atbm_printk_always("[TEST DWK] concle running work sync(%d)\n",ret);
		/*
		*[TEST] concle delayed work sync
		*/
		/*
		*destroy wq
		*/
		atbm_printk_always("############################################################");
		atbm_printk_always("############################################################");
		atbm_printk_always("[TEST WQ] destroy wq\n");
		ieee80211_atbm_queue_work(wq,&work);
		ieee80211_atbm_queue_delayed_work(wq,&delay_work,HZ/10);
		ieee80211_atbm_destroy_workqueue(wq);
		wq = NULL;
		atbm_printk_always("############################################################");
		atbm_printk_always("############################################################");
	}
	__set_current_state(TASK_RUNNING);
	if(wq){	
		ieee80211_atbm_flush_work(&work);
		ieee80211_atbm_flush_delayed_work(&delay_work);
		ieee80211_atbm_destroy_workqueue(wq);
	}
	
	return 0;
}
static void atbm_workqueue_test_thread_int(void)
{
	test_thread = kthread_create(atbm_workqueue_test_thread,NULL, "test_thread");

	if (IS_ERR(test_thread)){
		atbm_printk_always("workqueue test thread err\n");
	}else {
		wake_up_process(test_thread);
	}
	
}

static void atbm_workqueue_test_thread_exit(void)
{
	if(test_thread == NULL)
		return;

	kthread_stop(test_thread);
}
#endif
static void atbm_wk_go_running(struct atbm_work_struct *work)
{
	wk_set_running(work);
#ifdef CONFIG_ATBM_WK_TEST
	if(wk_is_testrunning(work)){
		wk_clear_testrunning(work);
		wake_up(&work->running);
	}
#endif
}

static void atbm_wk_out_running(struct atbm_work_struct *work)
{
	wk_clear_running(work);
}

static void atbm_wk_go_pending(struct atbm_work_struct *work)
{
	wk_set_pending(work);
#ifdef CONFIG_ATBM_WK_TEST
	if(wk_is_testpending(work)){
		wk_clear_testpending(work);
		wake_up(&work->pending);
	}
#endif

}

static void atbm_wk_out_pending(struct atbm_work_struct *work)
{
	wk_clear_pending(work);
}

static void atbm_wk_go_delayed(struct atbm_work_struct *work)
{
	wk_set_delayed(work);
}

static void atbm_wk_out_delayed(struct atbm_work_struct *work)
{
	wk_clear_delayed(work);
}
static void atbm_wk_go_waiting(struct atbm_work_struct *work)
{
	wk_set_waiting(work);
}

static void atbm_wk_out_waiting(struct atbm_work_struct *work)
{
	wk_clear_waiting(work);
} 

static void atbm_wk_go_barrier(struct atbm_work_struct *work)
{
	wk_set_barrier(work);
}

static void atbm_wk_out_barrier(struct atbm_work_struct *work)
{
	if(wk_is_barrier(work)){
		wk_clear_barrier(work);
		wake_up(&work->barrier);
	}
} 

static int atbm_wk_in_runing(struct atbm_work_struct *work)
{
	return !!wk_is_running(work);
}

static int atbm_wk_in_delayed(struct atbm_work_struct *work)
{
	return !!wk_is_delayed(work);
}

static int atbm_wk_in_pending(struct atbm_work_struct *work)
{
	return !!wk_is_pending(work);
}

static int atbm_wk_in_waiting(struct atbm_work_struct *work)
{
	return !!wk_is_waiting(work);
}
static int atbm_wk_in_barrier(struct atbm_work_struct *work)
{
	return !!wk_is_barrier(work);
}
static int atbm_workqueue_wait_action(struct atbm_workqueue_struct *wq)
{
	set_current_state(TASK_INTERRUPTIBLE);	
	while (!kthread_should_stop()) {

		if (test_and_clear_bit(WQ_THREAD_WAKEUP,
				       &wq->flags)) {
			__set_current_state(TASK_RUNNING);
			return 0;
		}
		set_current_state(TASK_INTERRUPTIBLE);
		if (!kthread_should_stop())
			schedule_timeout(20*HZ);
		set_current_state(TASK_INTERRUPTIBLE);
		
	}
	__set_current_state(TASK_RUNNING);
	return -1;
}

static void atbm_workqueue_process_works(struct atbm_workqueue_struct *wq)
{
	unsigned long flags;
	struct atbm_work_struct *work = NULL;
	atbm_printk_debug("%s in\n",wq->name);
	spin_lock_irqsave(&wq->lock, flags);
	while(!list_empty(&wq->works)){
		work = list_first_entry(&wq->works, struct atbm_work_struct,entry);
		WARN_ON(work == NULL);		
		list_del(&work->entry);
		atbm_wk_out_pending(work);
		atbm_wk_go_running(work);
		spin_unlock_irqrestore(&wq->lock, flags);
		atbm_printk_debug("%s in\n",work->name);
		//BUG_ON(work->func == NULL);
		
		if(work->func == NULL){
			atbm_printk_err("%s %d ,ERROR !!! work->func is NULL\n",__func__,__LINE__);
			//return;
		}else
			work->func(work);
		atbm_printk_debug("%s out\n",work->name);
		spin_lock_irqsave(&wq->lock, flags);
		atbm_wk_out_running(work);
		/*
		*be carefully add any code which will use work after atbm_wk_out_barrier.
		*/
		atbm_wk_out_barrier(work);
	}
	spin_unlock_irqrestore(&wq->lock, flags);
	atbm_printk_debug("%s out\n",wq->name);
}
static int atbm_workqueue_thread(void *priv)
{
	struct atbm_workqueue_struct *wq = (struct atbm_workqueue_struct *)priv;
	struct sched_param param = { .sched_priority = WQ_THREAD_PR };
	
	atbm_printk_init("%s\n",wq->name);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 9, 0))
        sched_set_fifo(current);
#else
        sched_setscheduler(current, SCHED_FIFO, &param);
#endif
	
	while(!atbm_workqueue_wait_action(wq)){
		atbm_workqueue_process_works(wq);
	};
	atbm_printk_init("%s:exit\n",wq->name);
	atbm_workqueue_process_works(wq);
	return 0;
}
static void ieee80211_atbm_wait_work(struct atbm_work_struct *work)
{
	struct work_wait *wait = container_of(work, struct work_wait, work);
	
	atbm_printk_debug("%s:wait done\n",work->name);
}
static void atbm_workqueue_wakeup(struct atbm_workqueue_struct *wq,struct task_struct	*work_thread)
{
	if(test_and_set_bit(WQ_THREAD_WAKEUP, &wq->flags) == 0){
		wake_up_process(work_thread);
	}
}
static bool _ieee80211_atbm_queue_work(struct atbm_workqueue_struct *wq,struct atbm_work_struct *work)
{
	unsigned long flags=0;
	bool ret = false;
	struct task_struct *thread = NULL;
	
	work->wq = wq;
	
	spin_lock_irqsave(&wq->lock, flags);
	thread = wq->work_thread;
	if(thread){
		list_add_tail(&work->entry, &wq->works);
		atbm_printk_debug("[queue work] [%s]->[%s]\n",work->name,wq->name);
		atbm_workqueue_wakeup(wq,thread);
	    ret = true;
	}else {
		atbm_wk_out_pending(work);
	}
	spin_unlock_irqrestore(&wq->lock, flags);

	return ret;
}
static void _ieee80211_atbm_clear_pending_work(struct atbm_workqueue_struct *wq,struct atbm_work_struct *work)
{
	//BUG_ON(wq == NULL);
	if(wq == NULL || work == NULL){
		atbm_printk_err("%s %d ,ERROR !!! wq is NULL\n",__func__,__LINE__);
		return;
	}
	list_del(&work->entry);
	atbm_wk_out_pending(work);
}
static void _ieee80211_atbm_clear_delay_work(struct atbm_workqueue_struct *wq,struct atbm_work_struct *work)
{
	unsigned long flags=0;
	spin_lock_irqsave(&wq->lock, flags);
	list_del(&work->entry);
	atbm_wk_out_delayed(work);
	if(atbm_wk_in_waiting(work)){
		atbm_wk_out_waiting(work);
		wake_up(&work->timeout);
	}
	atbm_printk_debug("[clear delay work] [%s]->[%s]\n",work->name,wq->name);
	spin_unlock_irqrestore(&wq->lock, flags);
}
static void atbm_delayed_work_timer_fn(unsigned long __data)
{
	struct atbm_delayed_work *dwork = (struct atbm_delayed_work *)__data;
	unsigned long flags=0;
	/* should have been called from irqsafe timer with irq already off */
	atbm_printk_debug("[%s] time out\n",dwork->work.name);
	spin_lock_irqsave(&dwork->work.lock, flags);
	dwork->n_timeout ++;
	if(atbm_wk_in_delayed(&dwork->work)){
		_ieee80211_atbm_clear_delay_work(dwork->wq,&dwork->work);
		if (!atbm_wk_in_pending(&dwork->work)) {
			atbm_wk_go_pending(&dwork->work);
			_ieee80211_atbm_queue_work(dwork->wq,&dwork->work);
		}else {
			WARN_ON(1);
		}
	}else {
		atbm_printk_debug("[%s] cancled\n",dwork->work.name);
	}
	spin_unlock_irqrestore(&dwork->work.lock, flags);
}
static void atbm_waitwk_init(struct work_wait *wait,const char *name)
{
	ieee80211_atbm_init_work(&wait->work,ieee80211_atbm_wait_work,name);
}
static void atbm_waitwk_work_for_done(struct work_wait *wait)
{
	wait_event_interruptible(wait->work.barrier,atbm_wk_in_barrier(&wait->work) == 0);
}
static void atbm_waitwk_insert(struct work_wait *wait,struct list_head *head,bool tail)
{
	atbm_wk_go_barrier(&wait->work);
	atbm_wk_go_pending(&wait->work);
	
	if(tail == true){
		list_add_tail(&wait->work.entry, head);
	}else {
		list_add(&wait->work.entry,head);
	}
}
void ieee80211_atbm_init_work(struct atbm_work_struct *work,atbm_work_func_t func,const char *name)
{
	INIT_LIST_HEAD(&work->entry);
	work->data = 0;
	work->func = func;
	work->name = name;
	spin_lock_init(&work->lock);
	init_waitqueue_head(&work->timeout);
	init_waitqueue_head(&work->barrier);
#ifdef CONFIG_ATBM_WK_TEST
	atbm_wk_test_init(work);
#endif
}
void ieee80211_atbm_init_delay_work(struct atbm_delayed_work *work,atbm_work_func_t func,const char *name)
{
	ieee80211_atbm_init_work(&work->work,func,name);
	work->n_started = 0;
	work->n_timeout = 0;
	atbm_setup_timer(&work->timer, atbm_delayed_work_timer_fn,(unsigned long)(work));
}

struct atbm_workqueue_struct  *ieee80211_atbm_alloc_workqueue(const char *name,long flags)
{
	struct atbm_workqueue_struct  *wq;
	void *bh;
	atbm_printk_always("%s:[%s] want alloc wq\n",__func__,name);
	wq = atbm_kzalloc(sizeof(struct atbm_workqueue_struct), GFP_KERNEL);

	if(wq == NULL)
		goto err;
	
	INIT_LIST_HEAD(&wq->works);
	INIT_LIST_HEAD(&wq->flush_works);
	INIT_LIST_HEAD(&wq->delay_works);
	spin_lock_init(&wq->lock);
	init_waitqueue_head(&wq->wait_flushed);
	mutex_init(&wq->mutex);
	wq->name = name;
	wq->flush_empty = true;
	bh = kthread_create(atbm_workqueue_thread,wq, name);
	if (IS_ERR(bh)){
		atbm_printk_err("workqueue %s err\n",name);
		goto err;
	}else {
		spin_lock_bh(&wq->lock);
		rcu_assign_pointer(wq->work_thread,bh);
		spin_unlock_bh(&wq->lock);
		wake_up_process(wq->work_thread);
	}
	atbm_printk_always("%s:alloc[%s]\n",__func__,wq->name);
	return wq;
err:
	if(wq)
		atbm_kfree(wq);
	return NULL;
}
void ieee80211_atbm_workqueue_init(void)
{
	common_wq = ieee80211_atbm_alloc_workqueue("atbm_cowq",0);
	WARN_ON(common_wq == NULL);
#ifdef CONFIG_ATBM_WK_TEST
	atbm_workqueue_test_thread_int();
#endif
}
void ieee80211_atbm_workqueue_exit(void)
{
	if(common_wq)
		ieee80211_atbm_destroy_workqueue(common_wq);
#ifdef CONFIG_ATBM_WK_TEST
	atbm_workqueue_test_thread_exit();
#endif
}
bool ieee80211_atbm_queue_work(struct atbm_workqueue_struct *wq,struct atbm_work_struct *work)
{
	
	unsigned long flags=0;
	bool ret = false;
	
	spin_lock_irqsave(&work->lock, flags);
	if (!atbm_wk_in_pending(work)) {
		atbm_wk_go_pending(work);
		ret = _ieee80211_atbm_queue_work(wq,work);
	}
	spin_unlock_irqrestore(&work->lock, flags);
	
	return ret;
}
static bool _ieee80211_atbm_queue_delayed_work(struct atbm_workqueue_struct *wq,struct atbm_delayed_work *dwork, unsigned long delay)
{
	unsigned long flags=0;
	bool ret = false;
	struct task_struct *thread = NULL;
	struct atbm_timer_list *timer = &dwork->timer;
	
	spin_lock_irqsave(&wq->lock, flags);
	thread = wq->work_thread;
	if(thread){
		list_add_tail(&dwork->work.entry, &wq->delay_works);
		atbm_wk_go_delayed(&dwork->work);
		dwork->wq = wq;
		timer->expires = jiffies + delay;
		dwork->n_started ++;
		atbm_add_timer(timer);
		atbm_printk_debug("[queue delay work]->[%s]\n",dwork->work.name);
	    ret = true;
	}else {
		atbm_wk_out_pending(&dwork->work);
	}
	spin_unlock_irqrestore(&wq->lock, flags);

	return ret;
}
bool ieee80211_atbm_queue_delayed_work(struct atbm_workqueue_struct *wq,struct atbm_delayed_work *dwork, unsigned long delay)
{
	unsigned long flags=0;
	bool ret = false;
	
	spin_lock_irqsave(&dwork->work.lock, flags);
	if (!atbm_wk_in_pending(&dwork->work)) {
		if (!delay) {
			atbm_wk_go_pending(&dwork->work);
			ret = _ieee80211_atbm_queue_work(wq, &dwork->work);
			spin_unlock_irqrestore(&dwork->work.lock, flags);
			return ret;
		}
		if(atbm_wk_in_delayed(&dwork->work)){
			WARN_ON_ONCE(atbm_wk_in_delayed(&dwork->work));
			if(atbm_del_timer(&dwork->timer)){
				dwork->n_timeout ++;
				_ieee80211_atbm_clear_delay_work(wq,&dwork->work);
			}else {
				spin_unlock_irqrestore(&dwork->work.lock, flags);
				atbm_printk_always("%s,delay work is runing,check code\n",dwork->work.name);
				return false;
			}
		}
		ret = _ieee80211_atbm_queue_delayed_work(wq,dwork,delay);
	}else {
		atbm_printk_debug("[%s] is pending\n",dwork->work.name);
	}
	spin_unlock_irqrestore(&dwork->work.lock, flags);

	return ret;
}

void ieee80211_atbm_flush_workqueue(struct atbm_workqueue_struct *wq)
{
	struct work_wait wait;
	unsigned long flags;
	
	spin_lock_irqsave(&wq->lock, flags);
	atbm_printk_debug("[flush wq]->[%s] in\n",wq->name);
	atbm_waitwk_init(&wait,"flush");
	
	if(wq->work_thread == NULL){
		spin_unlock_irqrestore(&wq->lock, flags);
		return;
	}
	atbm_waitwk_insert(&wait,&wq->works,true);
	atbm_workqueue_wakeup(wq,wq->work_thread);
	spin_unlock_irqrestore(&wq->lock, flags);
	atbm_waitwk_work_for_done(&wait);
	
	atbm_printk_debug("[flush wq]->[%s] out\n",wq->name);
}

void ieee80211_atbm_destroy_workqueue(struct atbm_workqueue_struct *wq)
{
	unsigned long flags = 0;
	struct work_wait wait;
	struct task_struct	*work_thread = NULL;

	atbm_waitwk_init(&wait,"destroy");
	
	if(wq == NULL)
		return;
	
	atbm_printk_debug("[destroy wq]->[%s] in\n",wq->name);
	spin_lock_irqsave(&wq->lock, flags);
	do{
		work_thread = wq->work_thread;
		wq->work_thread = NULL;
		
		if(work_thread == NULL)
			break;
		while(!list_empty(&wq->delay_works)){
			struct atbm_work_struct *work = list_first_entry(&wq->delay_works, struct atbm_work_struct,entry);
			struct atbm_delayed_work *dwork = atbm_to_delayed_work(work);
			atbm_printk_always("%s:flush start\n",work->name);
			spin_unlock_irqrestore(&wq->lock, flags);
			if(atbm_del_timer_sync(&dwork->timer)){
				atbm_delayed_work_timer_fn((unsigned long)dwork);
			}
			spin_lock_irqsave(&wq->lock, flags);
			atbm_printk_always("%s:flush end\n",work->name);
		}
		atbm_waitwk_insert(&wait,&wq->works,true);
		atbm_workqueue_wakeup(wq,work_thread);
		spin_unlock_irqrestore(&wq->lock, flags);
		
		atbm_waitwk_work_for_done(&wait);
		
		spin_lock_irqsave(&wq->lock, flags);
		
	}while(0);
	spin_unlock_irqrestore(&wq->lock, flags);

	if(work_thread){
		kthread_stop(work_thread);
	}

	if(wq){
		atbm_kfree(wq);
	}
	atbm_printk_debug("[destroy wq]->[%s] out\n",wq->name);
	return;
}

bool ieee80211_atbm_flush_delayed_work(struct atbm_delayed_work *dwork)
{
	struct atbm_work_struct *work = &dwork->work;
	unsigned long flags1 = 0;
	unsigned long flags2 = 0;
	struct atbm_workqueue_struct *wq;
	bool ret = false;
	struct work_wait wait;
	struct list_head *head;
	bool tail = false;

	atbm_waitwk_init(&wait,dwork->work.name);
	
	spin_lock_irqsave(&work->lock, flags1);
	
	if (likely(atbm_del_timer(&dwork->timer))){
	//	BUG_ON(dwork->wq == NULL);
		
		if(dwork->wq == NULL){
			atbm_printk_err("%s %d ,ERROR !!! dwork->wq == NULL\n",__func__,__LINE__);
			//spin_unlock_irqrestore(&work->lock, flags1);
			ret = false;
			goto w_lock;
		}
	
		dwork->n_timeout ++;
		_ieee80211_atbm_clear_delay_work(dwork->wq,&dwork->work);
		if(!atbm_wk_in_pending(work)){
			atbm_wk_go_pending(work);
			ret = _ieee80211_atbm_queue_work(dwork->wq,work);
			if(ret == false){
				goto w_lock;
			}
		}
	}else if(atbm_wk_in_delayed(work)){
		/*
		*timer is time out ,wait runing timer;
		*/
		atbm_wk_go_waiting(work);
		atbm_printk_debug("[flush_delayed_work]->[%s] wait timeout\n",work->name);
		spin_unlock_irqrestore(&work->lock, flags1);
		wait_event_interruptible(work->timeout,atbm_wk_in_waiting(work) == 0);
		spin_lock_irqsave(&work->lock, flags1);	
	}
	
	if((wq = dwork->wq )== NULL){
		ret = false;
		goto w_lock;
	}
	
	spin_lock_irqsave(&wq->lock, flags2);

	if(!wk_is_busy(work)){
		ret = false;
		goto wq_lock;
	}

	if(atbm_wk_in_pending(work)){
		head = work->entry.next;
	    tail = true;
	}else if(atbm_wk_in_runing(work)){
		head = &wq->works;
	    tail = false;
	}else{ 
		//BUG_ON(1);
		atbm_printk_err("%s %d ,ERROR !!! no work pending & runing \n",__func__,__LINE__);
	
		ret = false;
		goto wq_lock;
	}
	if(wq->work_thread){
		atbm_waitwk_insert(&wait,head,tail);
		atbm_workqueue_wakeup(wq,wq->work_thread);
	}else {
		goto wq_lock;
	}
	ret = true;
	spin_unlock_irqrestore(&wq->lock, flags2);
	spin_unlock_irqrestore(&work->lock, flags1);

	atbm_waitwk_work_for_done(&wait);
	return ret;
wq_lock:
	spin_unlock_irqrestore(&wq->lock, flags2);
w_lock:
	spin_unlock_irqrestore(&work->lock, flags1);
	return ret;
}
int ieee80211_atbm_schedule_work(struct atbm_work_struct *work)
{
	if(!common_wq)
		return 0;
	return ieee80211_atbm_queue_work(common_wq,work);
}

bool ieee80211_atbm_cancel_delayed_work_sync(struct atbm_delayed_work *dwork)
{
	unsigned long flags1 = 0;
	unsigned long flags2 = 0;
	struct atbm_work_struct *work = &dwork->work;
	struct atbm_workqueue_struct *wq;
	bool ret = false;
	struct work_wait wait;
	struct list_head *head = NULL;

	atbm_waitwk_init(&wait,work->name);
	
	atbm_printk_debug("[cancel_delayed sync]->[%s] in\n",work->name);

	spin_lock_irqsave(&work->lock, flags1);	
	
	if (likely(atbm_del_timer(&dwork->timer))){
		atbm_printk_debug("[cancel_delayed sync]->[%s] success\n",work->name);
		ret = true;
		dwork->n_timeout ++;
		//BUG_ON(dwork->wq == NULL);
		
		if(dwork->wq == NULL){
			atbm_printk_err("%s %d ,ERROR !!! dwork->wq == NULL\n",__func__,__LINE__);
			ret = false;
			goto w_lock;
		}
		
		_ieee80211_atbm_clear_delay_work(dwork->wq,&dwork->work);	
	}else if(atbm_wk_in_delayed(work)){
		/*
		*timer is time out ,wait runing timer;
		*/
		atbm_wk_go_waiting(work);
		atbm_printk_always("[cancel_delayed sync]->[%s] wait timeout\n",work->name);
		spin_unlock_irqrestore(&work->lock, flags1);
		wait_event_interruptible(work->timeout,atbm_wk_in_waiting(work) == 0);
		spin_lock_irqsave(&work->lock, flags1);	
		ret = true;
	}
	
	if((wq = dwork->wq) == NULL){
		goto w_lock;
	}
	
	spin_lock_irqsave(&wq->lock, flags2);

	if(atbm_wk_in_pending(work)){
		_ieee80211_atbm_clear_pending_work(wq,work);
		atbm_printk_debug("[cancel_delayed sync]->[%s] pendding\n",work->name);
		ret = true;
	}
	if(!wk_is_busy(work)){
		goto wq_lock;
	}
	if(!atbm_wk_in_runing(work)){
		//BUG_ON(1);
		atbm_printk_err("%s %d ,ERROR !!! wk is no running\n",__func__,__LINE__);
		ret = false;
		goto wq_lock;
	}
	if(wq->work_thread){
		atbm_waitwk_insert(&wait,&wq->works,false);
		atbm_workqueue_wakeup(wq,wq->work_thread);
	}else {
		goto wq_lock;
	}
	ret =  false;
	spin_unlock_irqrestore(&wq->lock, flags2);
	spin_unlock_irqrestore(&work->lock, flags1);

	atbm_waitwk_work_for_done(&wait);
	return ret;
wq_lock:
	spin_unlock_irqrestore(&wq->lock, flags2);
w_lock:
	spin_unlock_irqrestore(&work->lock, flags1);
	return ret;
}

bool ieee80211_atbm_cancel_delayed_work(struct atbm_delayed_work *dwork)
{
	unsigned long flags1=0;
	unsigned long flags2=0;
	struct atbm_workqueue_struct *wq;
	struct atbm_work_struct *work = &dwork->work;
	bool ret = false;
	
	spin_lock_irqsave(&work->lock, flags1);
	
	atbm_printk_debug("[cancel_delayed]->[%s] in\n",work->name);
	
	if (likely(atbm_del_timer(&dwork->timer))){
	//	BUG_ON(dwork->wq == NULL);
		if(dwork->wq == NULL){
			atbm_printk_err("%s %d ,ERROR !!! dwork->wq == NULL\n",__func__,__LINE__);
			ret = false;
			goto exit;
		}
		dwork->n_timeout ++;
		_ieee80211_atbm_clear_delay_work(dwork->wq,work);
		atbm_printk_debug("[cancel_delayed]->[%s] sucess\n",work->name);
		ret = true;
	}
	
	if((wq = dwork->wq) == NULL){
		atbm_printk_debug("[cancel_delayed]->[%s] wq err\n",work->name);
		goto exit;
	}
	
	spin_lock_irqsave(&wq->lock, flags2);
	if(atbm_wk_in_pending(work)){
		_ieee80211_atbm_clear_pending_work(wq,work);
		atbm_printk_debug("[cancel_delayed]->[%s] pendding\n",work->name);
		ret = true;
	}
	
	if(wk_is_busy(work)){
		ret = false;
	}
	spin_unlock_irqrestore(&wq->lock, flags2);
exit:
	spin_unlock_irqrestore(&work->lock, flags1);
	return ret;
}

bool ieee80211_atbm_cancel_work_sync(struct atbm_work_struct *work)
{
	unsigned long flags1 = 0;
	unsigned long flags2 = 0;
	struct atbm_workqueue_struct *wq;
	bool ret = false;
	struct work_wait wait;

	atbm_waitwk_init(&wait,work->name);	
	atbm_printk_debug("[cancel_work]->[%s] in\n",work->name);
	
	spin_lock_irqsave(&work->lock, flags1);
	
	if((wq = work->wq) == NULL){
		atbm_printk_debug("[cancel_work]->[%s] out 1\n",work->name);
		ret = false;
		goto w_lock;
	}
	
	spin_lock_irqsave(&wq->lock, flags2);

	if(atbm_wk_in_pending(work)){
		_ieee80211_atbm_clear_pending_work(wq,work);
		atbm_printk_debug("[cancel_work]->[%s] pending\n",work->name);
		ret = true;
	}
	
	if(!wk_is_busy(work)){
		atbm_printk_debug("[cancel_work]->[%s] out 1\n",work->name);
		goto wq_lock;
	}
	
	if(!atbm_wk_in_runing(work)){
		//BUG_ON(1);
		
		atbm_printk_err("%s %d ,ERROR !!! work is not runing\n",__func__,__LINE__);
		goto wq_lock;
	}
	
	if(wq->work_thread){
		atbm_waitwk_insert(&wait,&wq->works,false);
		atbm_workqueue_wakeup(wq,wq->work_thread);
	}else
		goto wq_lock;
	
	ret = false;
	spin_unlock_irqrestore(&wq->lock, flags2);
	spin_unlock_irqrestore(&work->lock, flags1);

	atbm_waitwk_work_for_done(&wait);
	
	atbm_printk_debug("[cancel_work]->[%s] finished\n",work->name);
	return ret;
wq_lock:
	spin_unlock_irqrestore(&wq->lock, flags2);
w_lock:
	spin_unlock_irqrestore(&work->lock, flags1);
	return ret;
}
bool ieee80211_atbm_flush_work(struct atbm_work_struct *work)
{
	unsigned long flags1 = 0;
	unsigned long flags2 = 0;
	struct atbm_workqueue_struct *wq;
	bool ret = false;
	struct work_wait wait;
	struct list_head *head = NULL;
	bool tail = false;

	atbm_waitwk_init(&wait,work->name);
	
	atbm_printk_debug("[flush_work]->[%s] in\n",work->name);
	spin_lock_irqsave(&work->lock, flags1);
	
	if((wq = work->wq)== NULL){
		ret = false;
		atbm_printk_debug("[flush_work]->[%s] out 1\n",work->name);
		goto w_lock;
	}
	
	spin_lock_irqsave(&wq->lock, flags2);
	
	if(!wk_is_busy(work)){
		atbm_printk_debug("[cancel_work]->[%s] out 1\n",work->name);
		goto wq_lock;
	}
	
	if(atbm_wk_in_runing(work)){
		head = &wq->works;
		tail = false;
	}else if(atbm_wk_in_pending(work)){
		head = work->entry.next;
		tail = true;
	}else{
		//BUG_ON(1);
		atbm_printk_err("%s %d ,ERROR !!! work not pending & runing\n",__func__,__LINE__);
		ret = false;
		goto wq_lock;
	}
	
	if(wq->work_thread){
		atbm_waitwk_insert(&wait,head,true);
		atbm_workqueue_wakeup(wq,wq->work_thread);
	}else {
		goto wq_lock;
	}
	
	spin_unlock_irqrestore(&wq->lock, flags2);
	spin_unlock_irqrestore(&work->lock, flags1);
	ret = true;
	atbm_waitwk_work_for_done(&wait);
	atbm_printk_debug("[flush_work]->[%s] finished\n",work->name);
	return ret;
wq_lock:
	spin_unlock_irqrestore(&wq->lock, flags2);
w_lock:
	spin_unlock_irqrestore(&work->lock, flags1);
	return ret;
}
#endif
