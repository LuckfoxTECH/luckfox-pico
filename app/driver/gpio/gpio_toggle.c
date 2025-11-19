#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>

static int gpio = 33; 
static struct task_struct *toggle_thread;

static int toggle_fn(void *data)
{
    int state = 0;

    while (!kthread_should_stop()) {
        gpio_set_value(gpio, state);
        state = !state;
        msleep(500);     
    }

    return 0;
}

static int __init gpio_toggle_init(void)
{
    int ret;

    pr_info("GPIO Toggle Module Loaded\n");

    if (!gpio_is_valid(gpio)) {
        pr_err("Invalid GPIO %d\n", gpio);
        return -EINVAL;
    }

    ret = gpio_request(gpio, "gpio_toggle");
    if (ret) {
        pr_err("Failed to request GPIO %d\n", gpio);
        return ret;
    }

    gpio_direction_output(gpio, 0);

    toggle_thread = kthread_run(toggle_fn, NULL, "gpio_toggle_thread");
    if (IS_ERR(toggle_thread)) {
        gpio_free(gpio);
        pr_err("Failed to create thread\n");
        return PTR_ERR(toggle_thread);
    }

    return 0;
}

static void __exit gpio_toggle_exit(void)
{
    kthread_stop(toggle_thread);
    gpio_set_value(gpio, 0);
    gpio_free(gpio);

    pr_info("GPIO Toggle Module Unloaded\n");
}

module_init(gpio_toggle_init);
module_exit(gpio_toggle_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NHAT");
MODULE_DESCRIPTION("Luckfox Pico Zero GPIO Toggle Module");
