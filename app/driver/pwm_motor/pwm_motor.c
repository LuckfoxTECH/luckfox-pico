/* pwm_servo.c - Platform PWM servo driver (custom 400Hz servo)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/pwm.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/err.h>

#define DRIVER_NAME "pwm_servo"

/* Servo thực tế: góc 0–120 độ */
#define SERVO_MAX_DEG   120U

/* Thông số đo thực tế (duty %) */
#define DUTY_RIGHT_PCT  47.02f    /* 0 deg */
#define DUTY_MID_PCT    58.54f    /* 60 deg (không dùng trực tiếp) */
#define DUTY_LEFT_PCT   70.76f    /* 120 deg */

/* Period thực (≈ 397.66 Hz) */
#define SERVO_PERIOD_NS 2515000U  /* 2.515 ms */

struct pwm_servo_data {
    struct device *dev;
    struct pwm_device *pwm;
    struct mutex lock;
    unsigned int cur_angle;
    unsigned int period_ns;
};

/* Map góc -> duty ns */
static inline u32 pwm_servo_deg_to_pulse_ns(struct pwm_servo_data *s, unsigned int deg)
{
    u32 duty_min = 4702;   /* 47.02% x100 */
    u32 duty_max = 7076;   /* 70.76% x100 */
    u32 duty_x100;
    u32 period = s->period_ns;
    u32 pulse_ns;

    if (deg > SERVO_MAX_DEG)
        deg = SERVO_MAX_DEG;

    /* Linear interpolation (32-bit only) */
    duty_x100 = duty_min +
                ((duty_max - duty_min) * deg) / SERVO_MAX_DEG;

    /*
     * pulse = period * duty_x100 / 10000
     *
     * → Thay bằng:
     *     t = period / 10000
     *     pulse = t * duty_x100
     *
     * → Không tạo phép chia 64bit
     */

    pulse_ns = (period / 10000U) * duty_x100;

    return pulse_ns;
}



static ssize_t angle_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct pwm_servo_data *s = dev_get_drvdata(dev);
    if (!s) return -ENODEV;
    return scnprintf(buf, PAGE_SIZE, "%u\n", s->cur_angle);
}

static ssize_t angle_store(struct device *dev, struct device_attribute *attr,
                           const char *buf, size_t count)
{
    struct pwm_servo_data *s = dev_get_drvdata(dev);
    unsigned int deg;
    int ret;
    u32 pulse_ns;

    ret = kstrtouint(buf, 10, &deg);
    if (ret) return ret;

    if (deg > SERVO_MAX_DEG)
        deg = SERVO_MAX_DEG;

    mutex_lock(&s->lock);

    pulse_ns = pwm_servo_deg_to_pulse_ns(s, deg);

    ret = pwm_config(s->pwm, pulse_ns, s->period_ns);
    if (ret) {
        dev_err(dev, "pwm_config failed: %d\n", ret);
        mutex_unlock(&s->lock);
        return ret;
    }

    ret = pwm_enable(s->pwm);
    if (ret) {
        dev_err(dev, "pwm_enable failed: %d\n", ret);
        mutex_unlock(&s->lock);
        return ret;
    }

    s->cur_angle = deg;
    dev_info(dev, "angle=%u → duty=%u ns\n", deg, pulse_ns);

    mutex_unlock(&s->lock);
    return count;
}

static DEVICE_ATTR_RW(angle);

static int pwm_servo_probe(struct platform_device *pdev)
{
    struct pwm_servo_data *s;
    int ret;

    s = devm_kzalloc(&pdev->dev, sizeof(*s), GFP_KERNEL);
    if (!s) return -ENOMEM;

    mutex_init(&s->lock);

    s->pwm = devm_pwm_get(&pdev->dev, NULL);
    if (IS_ERR(s->pwm)) {
        dev_err(&pdev->dev, "PWM get failed\n");
        return PTR_ERR(s->pwm);
    }

    /* Gán chu kỳ thực đo */
    s->period_ns = SERVO_PERIOD_NS;

    ret = device_create_file(&pdev->dev, &dev_attr_angle);
    if (ret) return ret;

    /* servo ở 0 độ mặc định */
    ret = pwm_config(s->pwm, pwm_servo_deg_to_pulse_ns(s, 0), s->period_ns);
    if (!ret)
        ret = pwm_enable(s->pwm);

    if (ret) {
        device_remove_file(&pdev->dev, &dev_attr_angle);
        return ret;
    }

    platform_set_drvdata(pdev, s);

    dev_info(&pdev->dev,
             "PWM servo loaded: period=%u ns (≈%u Hz)\n",
             s->period_ns,
             (unsigned int)(1000000000ULL / s->period_ns));

    return 0;
}

static int pwm_servo_remove(struct platform_device *pdev)
{
    struct pwm_servo_data *s = platform_get_drvdata(pdev);
    if (!s) return 0;

    pwm_disable(s->pwm);
    device_remove_file(&pdev->dev, &dev_attr_angle);

    return 0;
}

static const struct of_device_id pwm_servo_of_match[] = {
    { .compatible = "mg90s-servo" },
    { .compatible = "pwm-servo" },
    { }
};
MODULE_DEVICE_TABLE(of, pwm_servo_of_match);

static struct platform_driver pwm_servo_driver = {
    .probe = pwm_servo_probe,
    .remove = pwm_servo_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = pwm_servo_of_match,
    },
};

module_platform_driver(pwm_servo_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Custom PWM servo driver (400Hz duty mapping)");
