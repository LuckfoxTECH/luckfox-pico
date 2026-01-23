/*
 * luckfox_ctrl.c
 * Servo MG90S + DC Motor MX1919 (PWM MODE A)
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
#define DUTY_DEADZONE       20

/* Motor physical tuning */
#define DUTY_DEADZONE       20   /* joystick deadzone */
#define MOTOR_DUTY_MIN      60   /* % duty to overcome static friction */
#define MOTOR_DUTY_MAX      100

/* ================= PRIVATE ================= */
struct luckfox_priv {
    struct pwm_device *servo_pwm;

    struct pwm_device *motor_pwm_ina;
    struct pwm_device *motor_pwm_inb;

    int motor_speed;
    int motor_brake;
    int motor_mode;

    struct mutex lock;
    dev_t devno;
    struct cdev cdev;
    struct class *class;
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

static int servo_write_angle(struct luckfox_priv *p, int angle)
{
    struct pwm_state st;
    u32 pulse;

    if (!p || IS_ERR(p->servo_pwm))
        return -ENODEV;

    servo_angle_to_pulse_ns(angle, &pulse);

    pwm_init_state(p->servo_pwm, &st);
    st.period = SERVO_PERIOD_NS;
    st.duty_cycle = pulse;
    st.enabled = true;
    st.polarity = PWM_POLARITY_NORMAL;

    dbg("servo angle=%d pulse=%u\n", angle, pulse);
    return pwm_apply_state(p->servo_pwm, &st);
}

/* =========================================================
 * MOTOR – MX1919 PWM MODE A
 * ========================================================= */
static int motor_apply(struct luckfox_priv *p)
{
    struct pwm_state ina, inb;
    u64 duty_ns;
    int effective;
    int duty_percent;

    if (!p ||
        IS_ERR(p->motor_pwm_ina) ||
        IS_ERR(p->motor_pwm_inb))
        return -ENODEV;

    pwm_init_state(p->motor_pwm_ina, &ina);
    pwm_init_state(p->motor_pwm_inb, &inb);

    ina.period = MOTOR_PWM_PERIOD_NS;
    inb.period = MOTOR_PWM_PERIOD_NS;

    dbg("motor mode=%d speed=%d brake=%d\n",
        p->motor_mode, p->motor_speed, p->motor_brake);

    /* ================= STOP ================= */
    if (p->motor_mode == MOTOR_STOP) {
        ina.enabled = false;
        inb.enabled = false;
        ina.duty_cycle = 0;
        inb.duty_cycle = 0;

        pwm_apply_state(p->motor_pwm_ina, &ina);
        pwm_apply_state(p->motor_pwm_inb, &inb);

        dbg("motor STOP\n");
        return 0;
    }

    /* ========== speed - brake ========== */
    effective = p->motor_speed - p->motor_brake;
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
    if (p->motor_mode == MOTOR_FWD) {
        ina.duty_cycle = duty_ns;
        ina.enabled = duty_ns > 0;

        inb.duty_cycle = 0;
        inb.enabled = false;

        dbg("motor FWD duty=%llu\n", duty_ns);
    } else { /* MOTOR_REV */
        inb.duty_cycle = duty_ns;
        inb.enabled = duty_ns > 0;

        ina.duty_cycle = 0;
        ina.enabled = false;

        dbg("motor REV duty=%llu\n", duty_ns);
    }

    pwm_apply_state(p->motor_pwm_ina, &ina);
    pwm_apply_state(p->motor_pwm_inb, &inb);

    return 0;
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

    mutex_lock(&p->lock);

    switch (cmd) {

    case SERVO_SET_ANGLE:
        val = clamp(val, -20, 20);
        servo_write_angle(p, 90 + val);
        break;

    case MOTOR_SET_SPEED:
        p->motor_speed = clamp(val, 0, 100);
        motor_apply(p);
        break;

    case MOTOR_SET_BRAKE:
        p->motor_brake = clamp(val, 0, 100);
        motor_apply(p);
        break;

    case MOTOR_SET_MODE:
        p->motor_mode = clamp(val, MOTOR_STOP, MOTOR_REV);
        motor_apply(p);
        break;

    default:
        mutex_unlock(&p->lock);
        return -EINVAL;
    }

    mutex_unlock(&p->lock);
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

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = luckfox_open,
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

    /* PWM from Device Tree */
    p->servo_pwm =
        devm_of_pwm_get(dev, dev->of_node, "mg90s");

    p->motor_pwm_ina =
        devm_of_pwm_get(dev, dev->of_node, "mx1919_pwm_fwd");

    p->motor_pwm_inb =
        devm_of_pwm_get(dev, dev->of_node, "mx1919_pwm_rev");

    if (IS_ERR(p->servo_pwm) ||
        IS_ERR(p->motor_pwm_ina) ||
        IS_ERR(p->motor_pwm_inb)) {
        dev_err(dev, "Failed to get PWM from DT\n");
        return -ENODEV;
    }

    p->motor_mode  = MOTOR_STOP;
    p->motor_speed = 0;
    p->motor_brake = 0;

    ret = alloc_chrdev_region(&p->devno, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&p->cdev, &fops);
    cdev_add(&p->cdev, p->devno, 1);

    p->class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(p->class, NULL, p->devno, NULL, DEVICE_NAME);

    platform_set_drvdata(pdev, p);

    servo_write_angle(p, 90);

    dev_info(dev, "luckfox_ctrl loaded (MX1919 PWM MODE A)\n");
    return 0;
}

static int luckfox_ctrl_remove(struct platform_device *pdev)
{
    struct luckfox_priv *p = platform_get_drvdata(pdev);

    device_destroy(p->class, p->devno);
    class_destroy(p->class);
    cdev_del(&p->cdev);
    unregister_chrdev_region(p->devno, 1);

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
MODULE_DESCRIPTION("Luckfox Servo + DC Motor MX1919 (PWM MODE A)");
