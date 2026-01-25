/*
 * luckfox_ctrl.c
 * Servo MG90S + DC Motor MX1919 (PWM MODE A)
 * Fixed version: Realtime, Safe, Stable
 *
 * Motor:
 *   mode  : 0=STOP, 1=FWD, 2=REV
 *   speed : 0..100
 *   brake : 0..100
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/pwm.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/atomic.h>

#define DEVICE_NAME "luckfox_ctrl"

/* ================= DEBUG ================= */
#define LUCKFOX_DBG 1
#if LUCKFOX_DBG
#define dbg(fmt, ...) pr_info("[luckfox][DBG] " fmt, ##__VA_ARGS__)
#else
#define dbg(fmt, ...)
#endif

/* ================= IOCTL ================= */
#define SERVO_SET_ANGLE     _IOW('p', 1, int)
#define MOTOR_SET_SPEED     _IOW('p', 4, int)
#define MOTOR_SET_MODE      _IOW('p', 5, int)
#define MOTOR_SET_BRAKE     _IOW('p', 6, int)

/* ================= SERVO ================= */
#define SERVO_PERIOD_NS     2515000U
#define DUTY_RIGHT_X100     4974U
#define DUTY_LEFT_X100      7277U

/* ================= MOTOR ================= */
#define MOTOR_STOP  0
#define MOTOR_FWD   1
#define MOTOR_REV   2

#define MOTOR_PWM_PERIOD_NS 50000U   /* 20 kHz */

/* Motor physical tuning */
#define DUTY_DEADZONE       20   /* joystick deadzone */
#define MOTOR_DUTY_MIN      60   /* % duty to overcome static friction */
#define MOTOR_DUTY_MAX      100

/* ================= PRIVATE ================= */
struct luckfox_priv {
    struct pwm_device *servo_pwm;

    struct pwm_device *motor_pwm_ina;
    struct pwm_device *motor_pwm_inb;

    /* Motor state - protected by lock */
    int motor_speed;
    int motor_brake;
    int motor_mode;

    /* Servo state - protected by lock */
    int servo_angle;
    bool servo_pending;

    /* Workqueues for async PWM operations */
    struct work_struct motor_work;
    struct work_struct servo_work;
    atomic_t motor_pending;

    struct mutex lock;
    dev_t devno;
    struct cdev cdev;
    struct class *class;
    struct device *device;
};

/* =========================================================
 * SERVO
 * ========================================================= */
static int servo_angle_to_pulse_ns(int angle, u32 *pulse)
{
    u32 duty;

    angle = clamp(angle, 70, 110);

    duty = DUTY_RIGHT_X100 +
        ((DUTY_LEFT_X100 - DUTY_RIGHT_X100) *
         (110 - angle)) / 40;

    *pulse = (SERVO_PERIOD_NS / 10000U) * duty;
    return 0;
}

static void servo_work_handler(struct work_struct *work)
{
    struct luckfox_priv *p = container_of(work, struct luckfox_priv, servo_work);
    struct pwm_state st;
    u32 pulse;
    int angle;
    int ret;

    /* Read angle with lock */
    mutex_lock(&p->lock);
    if (!p->servo_pending) {
        mutex_unlock(&p->lock);
        return;
    }
    angle = p->servo_angle;
    p->servo_pending = false;
    mutex_unlock(&p->lock);

    /* Apply PWM without holding lock */
    if (IS_ERR_OR_NULL(p->servo_pwm)) {
        pr_err("[luckfox] servo_pwm is invalid\n");
        return;
    }

    servo_angle_to_pulse_ns(angle, &pulse);

    pwm_init_state(p->servo_pwm, &st);
    st.period = SERVO_PERIOD_NS;
    st.duty_cycle = pulse;
    st.enabled = true;
    st.polarity = PWM_POLARITY_NORMAL;

    ret = pwm_apply_state(p->servo_pwm, &st);
    if (ret) {
        pr_err("[luckfox] Failed to apply servo PWM: %d\n", ret);
        return;
    }

    dbg("servo angle=%d pulse=%u\n", angle, pulse);
}

/* =========================================================
 * MOTOR – MX1919 PWM MODE A
 * ========================================================= */
static void motor_work_handler(struct work_struct *work)
{
    struct luckfox_priv *p = container_of(work, struct luckfox_priv, motor_work);
    struct pwm_state ina, inb;
    u64 duty_ns;
    int effective;
    int duty_percent;
    int mode, speed, brake;
    int ret;

    /* Check if update is pending */
    if (!atomic_xchg(&p->motor_pending, 0))
        return;

    /* Read motor state with lock */
    mutex_lock(&p->lock);
    mode = p->motor_mode;
    speed = p->motor_speed;
    brake = p->motor_brake;
    mutex_unlock(&p->lock);

    /* Validate PWM devices */
    if (IS_ERR_OR_NULL(p->motor_pwm_ina) ||
        IS_ERR_OR_NULL(p->motor_pwm_inb)) {
        pr_err("[luckfox] motor PWM devices invalid\n");
        return;
    }

    pwm_init_state(p->motor_pwm_ina, &ina);
    pwm_init_state(p->motor_pwm_inb, &inb);

    ina.period = MOTOR_PWM_PERIOD_NS;
    inb.period = MOTOR_PWM_PERIOD_NS;
    ina.polarity = PWM_POLARITY_NORMAL;
    inb.polarity = PWM_POLARITY_NORMAL;

    dbg("motor mode=%d speed=%d brake=%d\n", mode, speed, brake);

    /* ================= STOP ================= */
    if (mode == MOTOR_STOP) {
        ina.enabled = false;
        inb.enabled = false;
        ina.duty_cycle = 0;
        inb.duty_cycle = 0;

        ret = pwm_apply_state(p->motor_pwm_ina, &ina);
        if (ret)
            pr_err("[luckfox] Failed to stop motor INA: %d\n", ret);

        ret = pwm_apply_state(p->motor_pwm_inb, &inb);
        if (ret)
            pr_err("[luckfox] Failed to stop motor INB: %d\n", ret);

        dbg("motor STOP\n");
        return;
    }

    /* ========== speed - brake ========== */
    effective = speed - brake;
    if (effective < DUTY_DEADZONE)
        effective = 0;

    if (effective == 0) {
        duty_ns = 0;
    } else {
        /*
         * Map speed (0..100) -> duty (MOTOR_DUTY_MIN..100)
         * This overcomes static friction
         */
        duty_percent = MOTOR_DUTY_MIN +
            (MOTOR_DUTY_MAX - MOTOR_DUTY_MIN) *
            effective / 100;

        duty_ns = (u64)ina.period * duty_percent;
        do_div(duty_ns, 100);
    }

    /* ================= DIRECTION ================= */
    if (mode == MOTOR_FWD) {
        ina.duty_cycle = duty_ns;
        ina.enabled = (duty_ns > 0);

        inb.duty_cycle = 0;
        inb.enabled = false;

        dbg("motor FWD duty=%llu\n", duty_ns);
    } else { /* MOTOR_REV */
        inb.duty_cycle = duty_ns;
        inb.enabled = (duty_ns > 0);

        ina.duty_cycle = 0;
        ina.enabled = false;

        dbg("motor REV duty=%llu\n", duty_ns);
    }

    ret = pwm_apply_state(p->motor_pwm_ina, &ina);
    if (ret)
        pr_err("[luckfox] Failed to apply motor INA: %d\n", ret);

    ret = pwm_apply_state(p->motor_pwm_inb, &inb);
    if (ret)
        pr_err("[luckfox] Failed to apply motor INB: %d\n", ret);
}

/* =========================================================
 * IOCTL
 * ========================================================= */
static long luckfox_ioctl(struct file *f,
                          unsigned int cmd,
                          unsigned long arg)
{
    struct luckfox_priv *p = f->private_data;
    int val;

    if (!p)
        return -ENODEV;

    if (copy_from_user(&val, (void __user *)arg, sizeof(val)))
        return -EFAULT;

    switch (cmd) {

    case SERVO_SET_ANGLE:
        val = clamp(val, -20, 20);
        mutex_lock(&p->lock);
        p->servo_angle = 90 + val;
        p->servo_pending = true;
        mutex_unlock(&p->lock);
        schedule_work(&p->servo_work);
        break;

    case MOTOR_SET_SPEED:
        mutex_lock(&p->lock);
        p->motor_speed = clamp(val, 0, 100);
        mutex_unlock(&p->lock);
        atomic_set(&p->motor_pending, 1);
        schedule_work(&p->motor_work);
        break;

    case MOTOR_SET_BRAKE:
        mutex_lock(&p->lock);
        p->motor_brake = clamp(val, 0, 100);
        mutex_unlock(&p->lock);
        atomic_set(&p->motor_pending, 1);
        schedule_work(&p->motor_work);
        break;

    case MOTOR_SET_MODE:
        val = clamp(val, MOTOR_STOP, MOTOR_REV);
        mutex_lock(&p->lock);
        p->motor_mode = val;
        mutex_unlock(&p->lock);
        atomic_set(&p->motor_pending, 1);
        schedule_work(&p->motor_work);
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

/* =========================================================
 * CHAR DEVICE
 * ========================================================= */
static int luckfox_open(struct inode *i, struct file *f)
{
    f->private_data =
        container_of(i->i_cdev, struct luckfox_priv, cdev);
    return 0;
}

static int luckfox_release(struct inode *i, struct file *f)
{
    /* Optional: Add any cleanup needed on close */
    return 0;
}

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = luckfox_open,
    .release        = luckfox_release,
    .unlocked_ioctl = luckfox_ioctl,
};

/* =========================================================
 * PROBE / REMOVE
 * ========================================================= */
static int luckfox_ctrl_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct luckfox_priv *p;
    int ret;

    p = devm_kzalloc(dev, sizeof(*p), GFP_KERNEL);
    if (!p)
        return -ENOMEM;

    mutex_init(&p->lock);
    atomic_set(&p->motor_pending, 0);

    /* Initialize workqueues */
    INIT_WORK(&p->motor_work, motor_work_handler);
    INIT_WORK(&p->servo_work, servo_work_handler);

    /* PWM from Device Tree */
    p->servo_pwm = devm_of_pwm_get(dev, dev->of_node, "mg90s");
    if (IS_ERR(p->servo_pwm)) {
        dev_err(dev, "Failed to get servo PWM: %ld\n", PTR_ERR(p->servo_pwm));
        return PTR_ERR(p->servo_pwm);
    }

    p->motor_pwm_ina = devm_of_pwm_get(dev, dev->of_node, "mx1919_pwm_fwd");
    if (IS_ERR(p->motor_pwm_ina)) {
        dev_err(dev, "Failed to get motor INA PWM: %ld\n", PTR_ERR(p->motor_pwm_ina));
        return PTR_ERR(p->motor_pwm_ina);
    }

    p->motor_pwm_inb = devm_of_pwm_get(dev, dev->of_node, "mx1919_pwm_rev");
    if (IS_ERR(p->motor_pwm_inb)) {
        dev_err(dev, "Failed to get motor INB PWM: %ld\n", PTR_ERR(p->motor_pwm_inb));
        return PTR_ERR(p->motor_pwm_inb);
    }

    /* Initialize motor state */
    p->motor_mode  = MOTOR_STOP;
    p->motor_speed = 0;
    p->motor_brake = 0;
    p->servo_angle = 90;
    p->servo_pending = false;

    /* Allocate character device */
    ret = alloc_chrdev_region(&p->devno, 0, 1, DEVICE_NAME);
    if (ret) {
        dev_err(dev, "Failed to allocate chrdev region: %d\n", ret);
        return ret;
    }

    cdev_init(&p->cdev, &fops);
    p->cdev.owner = THIS_MODULE;

    ret = cdev_add(&p->cdev, p->devno, 1);
    if (ret) {
        dev_err(dev, "Failed to add cdev: %d\n", ret);
        goto err_chrdev;
    }

    /* Create device class */
    p->class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(p->class)) {
        ret = PTR_ERR(p->class);
        dev_err(dev, "Failed to create class: %d\n", ret);
        goto err_cdev;
    }

    /* Create device node */
    p->device = device_create(p->class, NULL, p->devno, NULL, DEVICE_NAME);
    if (IS_ERR(p->device)) {
        ret = PTR_ERR(p->device);
        dev_err(dev, "Failed to create device: %d\n", ret);
        goto err_class;
    }

    platform_set_drvdata(pdev, p);

    /* Initialize servo to center position */
    p->servo_angle = 90;
    p->servo_pending = true;
    schedule_work(&p->servo_work);

    dev_info(dev, "luckfox_ctrl loaded (MX1919 PWM MODE A) - Fixed version\n");
    return 0;

err_class:
    class_destroy(p->class);
err_cdev:
    cdev_del(&p->cdev);
err_chrdev:
    unregister_chrdev_region(p->devno, 1);
    return ret;
}

static int luckfox_ctrl_remove(struct platform_device *pdev)
{
    struct luckfox_priv *p = platform_get_drvdata(pdev);
    struct pwm_state st;

    if (!p)
        return 0;

    /* Cancel all pending work */
    cancel_work_sync(&p->motor_work);
    cancel_work_sync(&p->servo_work);

    /* Stop motor */
    mutex_lock(&p->lock);
    p->motor_mode = MOTOR_STOP;
    mutex_unlock(&p->lock);

    /* Disable all PWM outputs */
    if (!IS_ERR_OR_NULL(p->motor_pwm_ina)) {
        pwm_init_state(p->motor_pwm_ina, &st);
        st.enabled = false;
        st.duty_cycle = 0;
        pwm_apply_state(p->motor_pwm_ina, &st);
    }

    if (!IS_ERR_OR_NULL(p->motor_pwm_inb)) {
        pwm_init_state(p->motor_pwm_inb, &st);
        st.enabled = false;
        st.duty_cycle = 0;
        pwm_apply_state(p->motor_pwm_inb, &st);
    }

    if (!IS_ERR_OR_NULL(p->servo_pwm)) {
        pwm_disable(p->servo_pwm);
    }

    /* Cleanup device nodes */
    device_destroy(p->class, p->devno);
    class_destroy(p->class);
    cdev_del(&p->cdev);
    unregister_chrdev_region(p->devno, 1);

    dev_info(&pdev->dev, "luckfox_ctrl removed\n");
    return 0;
}

/* =========================================================
 * DRIVER
 * ========================================================= */
static const struct of_device_id luckfox_dt_ids[] = {
    { .compatible = "luckfox,ctrl" },
    { }
};
MODULE_DEVICE_TABLE(of, luckfox_dt_ids);

static struct platform_driver luckfox_ctrl_driver = {
    .probe  = luckfox_ctrl_probe,
    .remove = luckfox_ctrl_remove,
    .driver = {
        .name = "luckfox_ctrl",
        .of_match_table = luckfox_dt_ids,
    },
};

module_platform_driver(luckfox_ctrl_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Trang");
MODULE_DESCRIPTION("Luckfox Servo + DC Motor MX1919 (PWM MODE A) - Fixed");
MODULE_VERSION("2.0");