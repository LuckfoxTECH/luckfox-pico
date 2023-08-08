#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <asm/ioctl.h>

typedef unsigned int u32;

struct atbm_info {
	dev_t devid;
	struct cdev *my_cdev;
	struct class *my_class;
	struct device *my_device;
	int power_gpio;
	int power_irq_num;
	int insmod_gpio;
	int insmod_irq_num;
	char inited;
	char reserved[3];
};

static struct atbm_info s_atbm_info;
static int new_event = 0;

static wait_queue_head_t wait_queue;

extern void wifi_pm_power(int on);

static int atbm_open(struct inode *inode, struct file *filp)
{
	int cnt = 10;

	while (!s_atbm_info.inited){
		msleep(10);
		cnt--;
		if (cnt <= 0){
			printk("%s timeout.\n", __func__);
			return -1;
		}
	}

	filp->private_data = &s_atbm_info;
	return 0;
}

static int atbm_release(struct inode *inode, struct file *filp)
{
	filp->private_data = NULL;
	return 0;
}

static ssize_t atbm_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
	int ret = 0;
	char insmod_module = 0;

	ret = wait_event_interruptible_timeout(wait_queue, new_event, msecs_to_jiffies(3000));
	if (ret < 0 || !new_event)
	{
		return -1;
	}

	new_event = 0;

	if (gpio_get_value(s_atbm_info.power_gpio)){
		insmod_module = 1;
	}
	else if (!gpio_get_value(s_atbm_info.power_gpio)) {
		insmod_module = 0;
	}
	else {
		return -1;
	}

	if (0 == copy_to_user(buff, &insmod_module, sizeof(insmod_module)))
	{
		return sizeof(insmod_module);
	}

	return -1;
}

static struct file_operations atbm_fops = {
    .owner = THIS_MODULE,
    .open = atbm_open,
    .release = atbm_release,
    .read = atbm_read,
};

static u32 power_gpio_irq(void *arg)
{
	new_event = 1;
	wake_up_interruptible(&wait_queue);
	return 0;
}

static u32 insmod_gpio_irq(void *arg)
{
	new_event = 1;
	wake_up_interruptible(&wait_queue);
	return 0;
}

static int __init atbm_module_init(void)
{
	memset(&s_atbm_info, 0, sizeof(s_atbm_info));

	alloc_chrdev_region(&s_atbm_info.devid, 0, 1, "atbm_power_ioctl");

	s_atbm_info.my_cdev = cdev_alloc();
	cdev_init(s_atbm_info.my_cdev, &atbm_fops);

	s_atbm_info.my_cdev->owner = THIS_MODULE;
	cdev_add(s_atbm_info.my_cdev, s_atbm_info.devid, 1);

	s_atbm_info.my_class = class_create(THIS_MODULE, "atbm_power_ioctl_class");
	s_atbm_info.my_device = device_create(s_atbm_info.my_class, NULL, s_atbm_info.devid, NULL, "atbm_power_ioctl");

	wifi_pm_power(1);

	init_waitqueue_head(&wait_queue);

	s_atbm_info.power_gpio = GPIOG(17);

	if (0 != gpio_request(s_atbm_info.power_gpio, NULL)){
		printk("gpio_request host err. %d\n", s_atbm_info.power_gpio);
		goto err_power_gpio;
	}

	gpio_direction_input(s_atbm_info.power_gpio);
	sw_gpio_setcfg(s_atbm_info.power_gpio, GPIO_CFG_INPUT);

	s_atbm_info.power_irq_num = sw_gpio_irq_request(s_atbm_info.power_gpio, TRIG_EDGE_DOUBLE, power_gpio_irq, NULL);
	if (!s_atbm_info.power_irq_num)
	{
		printk("sw_gpio_irq_request host err.\n");
		goto err_power_irq;
	}

	s_atbm_info.insmod_gpio = GPIOG(18);

	if (0 != gpio_request(s_atbm_info.insmod_gpio, NULL)){
		printk("gpio_request wakeup err. %d\n", s_atbm_info.insmod_gpio);
		goto err_insmod_gpio;
	}

	gpio_direction_input(s_atbm_info.insmod_gpio);
	sw_gpio_setcfg(s_atbm_info.insmod_gpio, GPIO_CFG_INPUT);

	s_atbm_info.insmod_irq_num = sw_gpio_irq_request(s_atbm_info.insmod_gpio, TRIG_EDGE_DOUBLE, insmod_gpio_irq, NULL);
	if (!s_atbm_info.insmod_irq_num)
	{
		printk("sw_gpio_irq_request wakeup err.\n");
		goto err_insmod_irq;
	}

	s_atbm_info.inited = 1;
	return 0;

err_insmod_irq:
	gpio_free(s_atbm_info.insmod_gpio);
err_insmod_gpio:
	sw_gpio_irq_free(s_atbm_info.power_irq_num);
err_power_irq:
	gpio_free(s_atbm_info.power_gpio);
err_power_gpio:
	device_destroy(s_atbm_info.my_class, s_atbm_info.devid);
	class_destroy(s_atbm_info.my_class);
	cdev_del(s_atbm_info.my_cdev);
	unregister_chrdev_region(s_atbm_info.devid, 1);
	memset(&s_atbm_info, 0, sizeof(s_atbm_info));
	return -1;
}

static void __exit atbm_module_exit(void)
{
	sw_gpio_irq_free(s_atbm_info.insmod_irq_num);
	gpio_free(s_atbm_info.insmod_gpio);
	sw_gpio_irq_free(s_atbm_info.power_irq_num);
	gpio_free(s_atbm_info.power_gpio);

	device_destroy(s_atbm_info.my_class, s_atbm_info.devid);
	class_destroy(s_atbm_info.my_class);
	cdev_del(s_atbm_info.my_cdev);
	unregister_chrdev_region(s_atbm_info.devid, 1);
	memset(&s_atbm_info, 0, sizeof(s_atbm_info));
}

module_init(atbm_module_init);
module_exit(atbm_module_exit);

MODULE_LICENSE("GPL");




