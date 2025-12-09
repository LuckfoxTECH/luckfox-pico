/* pwm_servo.c - Platform PWM servo driver (mg90s / pdi-1109mg)
 *
 * Bind with DT node compatible = "pwm-servo" or "mg90s-servo".
 *
 * Exposes sysfs attribute "angle" on the platform device.
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
#define DEFAULT_MAX_DEG 120U
#define DEFAULT_MIN_US 1000U
#define DEFAULT_MAX_US 2000U

struct pwm_servo_data {
	struct device *dev;
	struct pwm_device *pwm;
	struct mutex lock;
	unsigned int cur_angle;
	unsigned int max_deg;
	unsigned int min_pulse_us;
	unsigned int max_pulse_us;
	unsigned int period_ns;
};

static inline u32 pwm_servo_deg_to_pulse_ns(struct pwm_servo_data *s, unsigned int deg)
{
	u32 us;
	if (deg > s->max_deg)
		deg = s->max_deg;

	us = s->min_pulse_us +
	     (u32)(s->max_pulse_us - s->min_pulse_us) * (u32)deg / (u32)s->max_deg;

	return us * 1000U;
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
	int ret;
	unsigned int deg;
	u32 pulse_ns;

	if (!s) return -ENODEV;

	ret = kstrtouint(buf, 10, &deg);
	if (ret) return ret;

	if (deg > s->max_deg) deg = s->max_deg;

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
	dev_info(dev, "angle=%u -> pulse=%u ns\n", deg, pulse_ns);

	mutex_unlock(&s->lock);
	return count;
}

static DEVICE_ATTR_RW(angle);

static int pwm_servo_probe(struct platform_device *pdev)
{
	struct pwm_servo_data *s;
	int ret;
	unsigned int period_ns = 0;

	s = devm_kzalloc(&pdev->dev, sizeof(*s), GFP_KERNEL);
	if (!s) return -ENOMEM;

	s->dev = &pdev->dev;
	mutex_init(&s->lock);
	s->cur_angle = 0;
	s->max_deg = DEFAULT_MAX_DEG;
	s->min_pulse_us = DEFAULT_MIN_US;
	s->max_pulse_us = DEFAULT_MAX_US;

	/* get pwm from DT (pwm-names / pwms) */
	s->pwm = devm_pwm_get(&pdev->dev, "mg90s");
	if (IS_ERR(s->pwm)) {
		dev_warn(&pdev->dev, "devm_pwm_get(dev,\"mg90s\") failed: %ld, trying generic\n",
			 PTR_ERR(s->pwm));
		s->pwm = devm_pwm_get(&pdev->dev, NULL);
		if (IS_ERR(s->pwm)) {
			dev_err(&pdev->dev, "failed to obtain PWM: %ld\n", PTR_ERR(s->pwm));
			return PTR_ERR(s->pwm);
		}
	}

	period_ns = pwm_get_period(s->pwm);
	if (period_ns == 0)
		period_ns = 20U * 1000U * 1000U;
	s->period_ns = period_ns;

	ret = device_create_file(&pdev->dev, &dev_attr_angle);
	if (ret) {
		dev_err(&pdev->dev, "device_create_file failed: %d\n", ret);
		return ret;
	}

	ret = pwm_config(s->pwm, pwm_servo_deg_to_pulse_ns(s, 0), s->period_ns);
	if (ret) {
		dev_err(&pdev->dev, "initial pwm_config failed: %d\n", ret);
		device_remove_file(&pdev->dev, &dev_attr_angle);
		return ret;
	}

	ret = pwm_enable(s->pwm);
	if (ret) {
		dev_err(&pdev->dev, "initial pwm_enable failed: %d\n", ret);
		device_remove_file(&pdev->dev, &dev_attr_angle);
		return ret;
	}

	platform_set_drvdata(pdev, s);
	dev_info(&pdev->dev, "pwm_servo probed: period=%u ns, max_deg=%u\n",
		 s->period_ns, s->max_deg);

	return 0;
}

static int pwm_servo_remove(struct platform_device *pdev)
{
	struct pwm_servo_data *s = platform_get_drvdata(pdev);

	if (!s) return 0;

	device_remove_file(&pdev->dev, &dev_attr_angle);

	if (s->pwm)
		pwm_disable(s->pwm);

	platform_set_drvdata(pdev, NULL);
	dev_info(&pdev->dev, "pwm_servo removed\n");
	return 0;
}

static const struct of_device_id pwm_servo_of_match[] = {
	{ .compatible = "pwm-servo" },
	{ .compatible = "mg90s-servo" },
	{ /* sentinel */ }
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
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("Platform PWM servo driver (sysfs angle)");
