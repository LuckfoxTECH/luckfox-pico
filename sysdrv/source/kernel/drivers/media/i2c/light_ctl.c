// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2024 Rockchip Electronics Co., Ltd.
/*
 * light_ctl driver
 *
 * V0.0X01.0X01
 * 1. first implement this driver
 */

//#define DEBUG

#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/proc_fs.h>
#include <linux/sem.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/version.h>

#include "light_ctl.h"

#define DRIVER_VERSION		KERNEL_VERSION(0, 0x01, 0x01)
#define LIGHT_NUM		(4)
#define DEVICE_NAME		"light_ctl"


static struct lightctl_device *g_lightctl_dev[LIGHT_NUM];

static int light_ctl_set(struct lightctl_device *lightctl_dev, bool on)
{
	int ret = 0;
	struct light_ctl_info *info;

	info = lightctl_dev->light_info;

	if (!info || !info->pwm) {
		dev_info(&lightctl_dev->pdev->dev, "invalid param!\n");
		return -EINVAL;
	}

	if (info->pwm) {
		if (on) {
			dev_info(&lightctl_dev->pdev->dev, "%s: pwm, on\n", __func__);
			info->pwm_state.enabled = true;
			info->pwm_state.duty_cycle = info->light_param.duty_cycle;
			info->pwm_state.period = info->light_param.period;
			info->pwm_state.polarity = info->light_param.polarity;
			ret = pwm_apply_state(info->pwm, &info->pwm_state);

			dev_dbg(&lightctl_dev->pdev->dev, "led pwm duty=%llu, period=%llu, polarity=%d\n",
				info->pwm_state.duty_cycle,
				info->pwm_state.period,
				info->pwm_state.polarity);

		} else {
			dev_info(&lightctl_dev->pdev->dev, "%s: pwm, off\n", __func__);
			info->pwm_state.enabled = false;
			ret = pwm_apply_state(info->pwm, &info->pwm_state);
		}
	}

	if (info->light_gpio && info->light_param.light_type == LIGHT_GPIO) {
		if (on) {
			dev_info(&lightctl_dev->pdev->dev, "%s: gpio, on\n", __func__);
			if (info->light_param.light_enable)
				gpiod_set_value_cansleep(info->light_gpio, 1);
		} else {
			dev_info(&lightctl_dev->pdev->dev, "%s: gpio, off\n", __func__);
			gpiod_set_value_cansleep(info->light_gpio, 0);
		}
	}

	dev_info(&lightctl_dev->pdev->dev, "%s: ret:%d\n", __func__, ret);
	return ret;
}

int light_ctl_write(int light_id, struct rk_light_param *light_param)
{
	int ret = 0;
	struct light_ctl_info *info;
	struct lightctl_device *lightctl_dev;

	if (light_id < 0 || light_id >= LIGHT_NUM)
		return -EINVAL;

	if (!light_param)
		return -EINVAL;

	lightctl_dev = g_lightctl_dev[light_id];
	if (!lightctl_dev)
		return -EINVAL;
	info = lightctl_dev->light_info;
	mutex_lock(&lightctl_dev->mutex);

	if (info->light_index != light_id) {
		dev_err(&lightctl_dev->pdev->dev,
			"%s: light_index is incorrect: light index:%d, sensor index:%d\n",
			__func__,
			info->light_index, light_id);
		mutex_unlock(&lightctl_dev->mutex);
		return -EINVAL;
	}

	memcpy(&info->light_param, light_param, sizeof(struct rk_light_param));

	if (info->pwm && info->light_param.light_type == LIGHT_PWM) {
		dev_info(&lightctl_dev->pdev->dev,
			 "%s: light type is: PWM, light enable is: %s, light duty_cycle= %lld, period= %lld, polarity= %d\n",
			 __func__,
			 (info->light_param.light_enable ? "enable" : "disable"),
			 info->light_param.duty_cycle,
			 info->light_param.period,
			 info->light_param.polarity);
		if (info->light_param.duty_cycle > 0 &&
			info->light_param.light_enable)
			ret = light_ctl_set(lightctl_dev, true);
		else
			ret = light_ctl_set(lightctl_dev, false);
		mutex_unlock(&lightctl_dev->mutex);
		return ret;
	}

	if (info->light_gpio && info->light_param.light_type == LIGHT_GPIO) {
		if (info->light_param.light_enable)
			ret = light_ctl_set(lightctl_dev, true);
		else
			ret = light_ctl_set(lightctl_dev, false);
	}

	mutex_unlock(&lightctl_dev->mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(light_ctl_write);

#ifdef CONFIG_PROC_FS
static int light_ctl_show(struct seq_file *p, void *v)
{
	struct lightctl_device *dev = p->private;

	if (dev && dev->light_info) {
		seq_puts(p, "[Header]\n");
		seq_puts(p, "version=1.0\n\n");

		seq_puts(p, "[LightCtlParam]\n");
		seq_printf(p, "light type=%d;\n", dev->light_info->light_param.light_type);
		seq_printf(p, "light enable=%d;\n", dev->light_info->light_param.light_enable);
		seq_printf(p, "duty cycle=%lld;\n", dev->light_info->light_param.duty_cycle);
		seq_printf(p, "period=%lld;\n", dev->light_info->light_param.period);
		seq_printf(p, "polarity=%d;\n", dev->light_info->light_param.polarity);
	} else {
		seq_puts(p, "light info is null!\n");
	}

	return 0;
}

DEFINE_PROC_SHOW_ATTRIBUTE(light_ctl);

/* Create the /proc/lightctl-x */
static int lightctl_proc_init(struct lightctl_device *dev)
{
	dev->procfs = proc_create_data(dev->name, 0644, NULL, &light_ctl_proc_ops, dev);
	if (!dev->procfs)
		return -EINVAL;

	return 0;
}

static void lightctl_proc_cleanup(struct lightctl_device *dev)
{
	if (dev->procfs)
		remove_proc_entry(dev->name, NULL);
	dev->procfs = NULL;
}

#endif /* CONFIG_PROC_FS */

static int lightctl_parse_dt(struct lightctl_device *lightctl_dev)
{
	struct gpio_desc *light_gpio;
	struct pwm_device *light_pwm;
	struct device *dev = &lightctl_dev->pdev->dev;
	struct pwm_state *pwm_state = &lightctl_dev->light_info->pwm_state;
	int ret = 0;

	light_gpio = devm_gpiod_get(dev, "light", GPIOD_ASIS);
	if (IS_ERR(light_gpio)) {
		ret = PTR_ERR(light_gpio);
		dev_info(dev, "Unable to claim light-gpio\n");
	} else {
		lightctl_dev->light_info->light_gpio = light_gpio;
	}

	light_pwm = devm_pwm_get(dev, NULL);
	if (IS_ERR(light_pwm)) {
		ret = PTR_ERR(light_pwm);
		dev_info(dev, "Unable to get pwm device\n");
	} else {
		lightctl_dev->light_info->pwm = light_pwm;
		pwm_state->period = light_pwm->args.period;
		pwm_state->polarity = light_pwm->args.polarity;
		dev_dbg(dev, "period %llu, polarity %d\n",
			pwm_state->period, pwm_state->polarity);
	}

	if (IS_ERR(light_pwm) && IS_ERR(light_gpio)) {
		dev_err(dev, "Neither enable-gpio nor pwm can be get, return error\n");
		return ret;
	}

	ret = device_property_read_u32(dev, "rockchip,module-index",
				       &lightctl_dev->light_info->light_index);
	if (ret < 0)
		dev_warn(dev, "light_index property missing\n");
	dev_dbg(dev, "module-index = %d", lightctl_dev->light_info->light_index);

	return ret;
}

static int lightctl_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct lightctl_device *lightctl_dev;
	int ret = 0;
	int index = 0;

	dev_info(dev, "driver version: %02x.%02x.%02x",
		 DRIVER_VERSION >> 16,
		 (DRIVER_VERSION & 0xff00) >> 8,
		 DRIVER_VERSION & 0x00ff);

	lightctl_dev = devm_kzalloc(dev, sizeof(*lightctl_dev), GFP_KERNEL);
	if (lightctl_dev == NULL)
		return -ENOMEM;

	lightctl_dev->light_info = devm_kzalloc(dev, sizeof(struct light_ctl_info), GFP_KERNEL);
	if (lightctl_dev->light_info == NULL)
		return -ENOMEM;

	lightctl_dev->pdev = pdev;

	mutex_init(&lightctl_dev->mutex);

	ret = lightctl_parse_dt(lightctl_dev);
	if (ret < 0)
		return ret;

	index = lightctl_dev->light_info->light_index;
	if (index >= LIGHT_NUM || 0 > index) {
		dev_err(dev, "light index:%d out of rage(Max: 4)\n", index);
		return -EINVAL;
	}
	if (g_lightctl_dev[index])	// have exist
		return -EEXIST;

	snprintf(lightctl_dev->name, sizeof(lightctl_dev->name), "%s-%d",
		 DEVICE_NAME, index);

	g_lightctl_dev[index] = lightctl_dev;

	lightctl_proc_init(lightctl_dev);
	dev_info(dev, "light ctrl probing successful\n");

	return 0;
}

static int lightctl_remove(struct platform_device *pdev)
{
	struct lightctl_device *light = platform_get_drvdata(pdev);

	lightctl_proc_cleanup(light);

	mutex_destroy(&light->mutex);

	return 0;
}

static const struct of_device_id lightctl_of_table[] = {
	{ .compatible = "rockchip,light-ctl" },
	{ { 0 } }
};
MODULE_DEVICE_TABLE(of, lightctl_of_table);

static struct platform_driver lightctl_driver = {
	.driver = {
		.name = DEVICE_NAME,
		.of_match_table = lightctl_of_table,
	},
	.probe = &lightctl_probe,
	.remove = &lightctl_remove,
};

module_platform_driver(lightctl_driver);

MODULE_DESCRIPTION("lightctl driver");
MODULE_AUTHOR("Chad.ma@rockchip");
MODULE_LICENSE("GPL");
