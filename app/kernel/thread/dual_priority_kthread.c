#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

static struct task_struct *thread1;
static struct task_struct *thread2;

static int thread1_func(void *data)
{
    while (!kthread_should_stop()) {
        pr_info("Thread 1 (high prio) running\n");
        msleep(500);
    }
    return 0;
}

static int thread2_func(void *data)
{
    while (!kthread_should_stop()) {
        pr_info("Thread 2 (low prio) running\n");
        msleep(1000);
    }
    return 0;
}

static int __init dual_priority_init(void)
{
    // tạo thread 1
    thread1 = kthread_run(thread1_func, NULL, "high_prio_thread");
    if (IS_ERR(thread1))
        return PTR_ERR(thread1);

    // tạo thread 2
    thread2 = kthread_run(thread2_func, NULL, "low_prio_thread");
    if (IS_ERR(thread2)) {
        kthread_stop(thread1);
        return PTR_ERR(thread2);
    }

    set_user_nice(thread1, -20); // -20 là cao nhất, 19 là thấp nhất
    set_user_nice(thread2, 10);  // ưu tiên thấp hơn

    pr_info("Dual priority kthreads started\n");
    return 0;
}

static void __exit dual_priority_exit(void)
{
    if (thread1)
        kthread_stop(thread1);
    if (thread2)
        kthread_stop(thread2);

    pr_info("Dual priority kthreads stopped\n");
}

module_init(dual_priority_init);
module_exit(dual_priority_exit);

MODULE_LICENSE("GPL");
