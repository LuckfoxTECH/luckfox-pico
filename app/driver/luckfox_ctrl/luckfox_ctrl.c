/* luckfox_ctrl.c - ioctl servo (MG90S mapping) supporting 4-cell DT pwms
 *
 * - Uses devm_of_pwm_get() so DT entries like:
 *     pwms = <&pwm0 0 20000000 0>;
 *   are supported.
 *
 * - Uses same mapping as original pwm_servo.c:
 *     DUTY_RIGHT_X100 = 47.02% (x100)
 *     DUTY_LEFT_X100  = 70.76% (x100)
 *     SERVO_MAX_DEG   = 120
 *     SERVO_PERIOD_NS = 2515000 (kept from original mapping)
 *
 * - Exposes /dev/luckfox_ctrl and an ioctl:
 *     SERVO_SET_ANGLE (int)
 *
 * Build as kernel module (obj-m += luckfox_ctrl.o)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pwm.h>
#include <linux/gpio/consumer.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>

#define DEVICE_NAME "luckfox_ctrl"

/* IOCTLs */
#define SERVO_SET_ANGLE   _IOW('p', 1, int)
#define GPIO_SET_LEVEL    _IOW('p', 2, int)
#define GPIO_SET_DIR      _IOW('p', 3, int)

/* MG90S mapping (from your pwm_servo.c) */
#define SERVO_MAX_DEG   120U
#define SERVO_PERIOD_NS 2515000U   /* 2.515 ms */

#define DUTY_RIGHT_X100  4702U    /* 47.02% x100 */
#define DUTY_LEFT_X100   7076U    /* 70.76% x100 */

struct luckfox_priv {
    struct pwm_device *servo_pwm;
    struct gpio_desc *gpio_test;
    struct mutex lock;
    dev_t devno;
    struct cdev cdev;
    struct class *class;
};

static int servo_angle_to_pulse_ns(unsigned int deg, u32 *out_pulse_ns)
{
    u32 duty_x100;
    u32 pulse_ns;

    if (deg > SERVO_MAX_DEG)
        deg = SERVO_MAX_DEG;

    duty_x100 = DUTY_RIGHT_X100 +
                 ((DUTY_LEFT_X100 - DUTY_RIGHT_X100) * deg) / SERVO_MAX_DEG;

    /* pulse = period * duty_x100 / 10000 */
    pulse_ns = (SERVO_PERIOD_NS / 10000U) * duty_x100;

    *out_pulse_ns = pulse_ns;
    return 0;
}

static int servo_write_angle(struct luckfox_priv *p, unsigned int angle)
{
    u32 pulse_ns;

    if (!p || !p->servo_pwm)
        return -ENODEV;

    servo_angle_to_pulse_ns(angle, &pulse_ns);

    /* configure pwm (use same period as original mapping) */
    pwm_config(p->servo_pwm, pulse_ns, SERVO_PERIOD_NS);
    pwm_enable(p->servo_pwm);

    return 0;
}

/* ioctl handler */
static long luckfox_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct luckfox_priv *p = file->private_data;
    int val;

    if (!p)
        return -ENODEV;

    switch (cmd) {
    case SERVO_SET_ANGLE:
        if (copy_from_user(&val, (void __user *)arg, sizeof(int)))
            return -EFAULT;
        if (val < 0) val = 0;
        if (val > (int)SERVO_MAX_DEG) val = SERVO_MAX_DEG;
        mutex_lock(&p->lock);
        servo_write_angle(p, (unsigned int)val);
        mutex_unlock(&p->lock);
        pr_info("luckfox_ctrl: set angle=%d\n", val);
        break;

    case GPIO_SET_LEVEL:
        if (!p->gpio_test) return -ENODEV;
        if (copy_from_user(&val, (void __user *)arg, sizeof(int)))
            return -EFAULT;
        gpiod_set_value(p->gpio_test, val ? 1 : 0);
        pr_info("luckfox_ctrl: gpio set level=%d\n", val);
        break;

    case GPIO_SET_DIR:
        if (!p->gpio_test) return -ENODEV;
        if (copy_from_user(&val, (void __user *)arg, sizeof(int)))
            return -EFAULT;
        gpiod_direction_output(p->gpio_test, val ? 1 : 0);
        pr_info("luckfox_ctrl: gpio set dir out (init=%d)\n", val);
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

static int luckfox_open(struct inode *inode, struct file *filp)
{
    struct luckfox_priv *p = container_of(inode->i_cdev, struct luckfox_priv, cdev);
    filp->private_data = p;
    return 0;
}

static int luckfox_release(struct inode *inode, struct file *filp)
{
    filp->private_data = NULL;
    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = luckfox_ioctl,
    .open = luckfox_open,
    .release = luckfox_release,
};

/* probe */
static int luckfox_ctrl_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct luckfox_priv *p;
    int ret;

    pr_info("luckfox_ctrl: probe\n");

    p = devm_kzalloc(dev, sizeof(*p), GFP_KERNEL);
    if (!p)
        return -ENOMEM;

    mutex_init(&p->lock);

    /* Get PWM from DT by name "mg90s" (supports 4-cell pwms in DT) */
    p->servo_pwm = devm_of_pwm_get(dev, dev->of_node, "mg90s");
    if (IS_ERR(p->servo_pwm)) {
        dev_err(dev, "cannot get servo PWM (mg90s): %ld\n", PTR_ERR(p->servo_pwm));
        return PTR_ERR(p->servo_pwm);
    }

    /* Optional GPIO named "test" in DT (optional) */
    p->gpio_test = devm_gpiod_get_optional(dev, "test", GPIOD_OUT_LOW);
    if (IS_ERR(p->gpio_test)) {
        dev_err(dev, "failed to get optional test gpio\n");
        /* not fatal: continue without gpio */
        p->gpio_test = NULL;
    }

    /* Allocate char device number */
    ret = alloc_chrdev_region(&p->devno, 0, 1, DEVICE_NAME);
    if (ret) {
        dev_err(dev, "alloc_chrdev_region failed: %d\n", ret);
        return ret;
    }

    /* init cdev as part of priv so we can find priv from inode */
    cdev_init(&p->cdev, &fops);
    p->cdev.owner = THIS_MODULE;

    ret = cdev_add(&p->cdev, p->devno, 1);
    if (ret) {
        dev_err(dev, "cdev_add failed: %d\n", ret);
        unregister_chrdev_region(p->devno, 1);
        return ret;
    }

    p->class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(p->class)) {
        dev_err(dev, "class_create failed\n");
        cdev_del(&p->cdev);
        unregister_chrdev_region(p->devno, 1);
        return PTR_ERR(p->class);
    }

    if (!device_create(p->class, NULL, p->devno, NULL, DEVICE_NAME)) {
        dev_err(dev, "device_create failed\n");
        class_destroy(p->class);
        cdev_del(&p->cdev);
        unregister_chrdev_region(p->devno, 1);
        return -ENOMEM;
    }

    /* store priv in platform device */
    platform_set_drvdata(pdev, p);

    pr_info("luckfox_ctrl: ready at /dev/%s (major=%d minor=%d)\n",
            DEVICE_NAME, MAJOR(p->devno), MINOR(p->devno));

    /* set default 0 angle to be safe */
    mutex_lock(&p->lock);
    servo_write_angle(p, 0);
    mutex_unlock(&p->lock);

    return 0;
}

static int luckfox_ctrl_remove(struct platform_device *pdev)
{
    struct luckfox_priv *p = platform_get_drvdata(pdev);

    if (!p) return 0;

    pwm_disable(p->servo_pwm);

    device_destroy(p->class, p->devno);
    class_destroy(p->class);
    cdev_del(&p->cdev);
    unregister_chrdev_region(p->devno, 1);

    pr_info("luckfox_ctrl removed\n");
    return 0;
}

static const struct of_device_id luckfox_dt_ids[] = {
    { .compatible = "luckfox,ctrl" },
    { }
};
MODULE_DEVICE_TABLE(of, luckfox_dt_ids);

static struct platform_driver luckfox_ctrl_driver = {
    .probe = luckfox_ctrl_probe,
    .remove = luckfox_ctrl_remove,
    .driver = {
        .name = "luckfox_ctrl",
        .of_match_table = luckfox_dt_ids,
    },
};

module_platform_driver(luckfox_ctrl_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("Luckfox control driver (MG90S ioctl) - supports 4-cell DT pwms");
