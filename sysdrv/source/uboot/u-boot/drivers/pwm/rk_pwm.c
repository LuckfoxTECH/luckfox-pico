/*
 * Copyright (c) 2016 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <clk.h>
#include <div64.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <pwm.h>
#include <regmap.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/arch/pwm.h>
#include <power/regulator.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * regs for pwm v4
 */
#define HIWORD_UPDATE(v, l, h)	(((v) << (l)) | (GENMASK(h, l) << 16))

/* VERSION_ID */
#define VERSION_ID			0x0
#define CHANNEL_NUM_SUPPORT_SHIFT	0
#define CHANNEL_NUM_SUPPORT_MASK	(0xf << CHANNEL_NUM_SUPPORT_SHIFT)
#define CHANNLE_INDEX_SHIFT		4
#define CHANNLE_INDEX_MASK		(0xf << CHANNLE_INDEX_SHIFT)
/* ENABLE */
#define ENABLE				0x4
#define PWM_ENABLE_V4			(0x3 << 0)
#define PWM_CLK_EN(v)			HIWORD_UPDATE(v, 0, 0)
#define PWM_EN(v)			HIWORD_UPDATE(v, 1, 1)
#define PWM_CTRL_UPDATE_EN(v)		HIWORD_UPDATE(v, 2, 2)
#define PWM_GLOBAL_JOIN_EN(v)		HIWORD_UPDATE(v, 4, 4)
/* CLK_CTRL */
#define CLK_CTRL			0x8
#define CLK_PRESCALE(v)			HIWORD_UPDATE(v, 0, 2)
#define CLK_SCALE(v)			HIWORD_UPDATE(v, 4, 12)
#define CLK_SRC_SEL(v)			HIWORD_UPDATE(v, 13, 14)
#define CLK_GLOBAL_SEL(v)		HIWORD_UPDATE(v, 15, 15)
/* CTRL */
#define CTRL_V4				0xc
#define PWM_MODE(v)			HIWORD_UPDATE(v, 0, 1)
#define ONESHOT_MODE			0
#define CONTINUOUS_MODE			1
#define CAPTURE_MODE			2
#define PWM_POLARITY(v)			HIWORD_UPDATE(v, 2, 3)
#define DUTY_NEGATIVE			(0 << 0)
#define DUTY_POSITIVE			(1 << 0)
#define INACTIVE_NEGATIVE		(0 << 1)
#define INACTIVE_POSITIVE		(1 << 1)
#define PWM_ALIGNED_INVALID(v)		HIWORD_UPDATE(v, 5, 5)
#define PWM_IN_SEL(v)			HIWORD_UPDATE(v, 6, 8)
/* PERIOD */
#define PERIOD				0x10
/* DUTY */
#define DUTY				0x14

struct rk_pwm_priv {
	fdt_addr_t base;
	ulong freq;
	u32 conf_polarity;
	bool vop_pwm_en; /* indicate voppwm mirror register state */
	const struct rockchip_pwm_data *data;
};

struct rockchip_pwm_funcs {
	int (*set_invert)(struct udevice *dev, uint channel, bool polarity);
	int (*set_config)(struct udevice *dev, uint channel,
			   uint period_ns, uint duty_ns);
	int (*set_enable)(struct udevice *dev, uint channel, bool enable);
};

struct rockchip_pwm_data {
	struct rockchip_pwm_regs regs;
	struct rockchip_pwm_funcs funcs;
	unsigned int prescaler;
	bool supports_polarity;
	bool supports_lock;
	bool vop_pwm;
	u8 main_version;
	u32 enable_conf;
	u32 enable_conf_mask;
};

static int rk_pwm_set_invert_v4(struct udevice *dev, uint channel, bool polarity)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	debug("%s: polarity=%u\n", __func__, polarity);
	if (polarity)
		priv->conf_polarity = PWM_DUTY_NEGATIVE | PWM_INACTIVE_POSTIVE;
	else
		priv->conf_polarity = PWM_DUTY_POSTIVE | PWM_INACTIVE_NEGATIVE;

	return 0;
}

static int rk_pwm_set_invert_v1(struct udevice *dev, uint channel, bool polarity)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	debug("%s: polarity=%u\n", __func__, polarity);
	if (polarity)
		priv->conf_polarity = PWM_DUTY_NEGATIVE | PWM_INACTIVE_POSTIVE;
	else
		priv->conf_polarity = PWM_DUTY_POSTIVE | PWM_INACTIVE_NEGATIVE;

	return 0;
}

static int rk_pwm_set_invert(struct udevice *dev, uint channel, bool polarity)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	if (!priv->data->supports_polarity) {
		debug("%s: Do not support polarity\n", __func__);
		return 0;
	}

	return priv->data->funcs.set_invert(dev, channel, polarity);
}

static int rk_pwm_set_config_v4(struct udevice *dev, uint channel,
				uint period_ns, uint duty_ns)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);
	unsigned long period, duty;

	period = lldiv((uint64_t)(priv->freq / 1000) * period_ns,
		       priv->data->prescaler * 1000000);
	duty = lldiv((uint64_t)(priv->freq / 1000) * duty_ns,
		     priv->data->prescaler * 1000000);

	writel(period, priv->base + PERIOD);
	writel(duty, priv->base + DUTY);

	if (priv->data->supports_polarity)
		writel(PWM_POLARITY(priv->conf_polarity), priv->base + CTRL_V4);

	writel(PWM_MODE(CONTINUOUS_MODE) | PWM_ALIGNED_INVALID(false),
	       priv->base + CTRL_V4);

	writel(PWM_CTRL_UPDATE_EN(true), priv->base + ENABLE);

	debug("%s: period=%lu, duty=%lu\n", __func__, period, duty);

	return 0;
}

static int rk_pwm_set_config_v1(struct udevice *dev, uint channel,
				uint period_ns, uint duty_ns)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);
	const struct rockchip_pwm_regs *regs = &priv->data->regs;
	unsigned long period, duty;
	u32 ctrl;

	debug("%s: period_ns=%u, duty_ns=%u\n", __func__, period_ns, duty_ns);

	ctrl = readl(priv->base + regs->ctrl);
	if (priv->data->vop_pwm) {
		if (priv->vop_pwm_en)
			ctrl |= RK_PWM_ENABLE;
		else
			ctrl &= ~RK_PWM_ENABLE;
	}

	/*
	 * Lock the period and duty of previous configuration, then
	 * change the duty and period, that would not be effective.
	 */
	if (priv->data->supports_lock) {
		ctrl |= PWM_LOCK;
		writel(ctrl, priv->base + regs->ctrl);
	}

	period = lldiv((uint64_t)(priv->freq / 1000) * period_ns,
		       priv->data->prescaler * 1000000);
	duty = lldiv((uint64_t)(priv->freq / 1000) * duty_ns,
		     priv->data->prescaler * 1000000);

	writel(period, priv->base + regs->period);
	writel(duty, priv->base + regs->duty);

	if (priv->data->supports_polarity) {
		ctrl &= ~(PWM_DUTY_MASK | PWM_INACTIVE_MASK);
		ctrl |= priv->conf_polarity;
	}

	/*
	 * Unlock and set polarity at the same time,
	 * the configuration of duty, period and polarity
	 * would be effective together at next period.
	 */
	if (priv->data->supports_lock)
		ctrl &= ~PWM_LOCK;
	writel(ctrl, priv->base + regs->ctrl);

	debug("%s: period=%lu, duty=%lu\n", __func__, period, duty);

	return 0;
}

static int rk_pwm_set_config(struct udevice *dev, uint channel,
			     uint period_ns, uint duty_ns)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	return priv->data->funcs.set_config(dev, channel, period_ns, duty_ns);
}

static int rk_pwm_set_enable_v4(struct udevice *dev, uint channel, bool enable)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	debug("%s: Enable '%s'\n", __func__, dev->name);

	writel(PWM_EN(enable) | PWM_CLK_EN(enable), priv->base + ENABLE);

	if (enable)
		pinctrl_select_state(dev, "active");

	return 0;
}

static int rk_pwm_set_enable_v1(struct udevice *dev, uint channel, bool enable)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);
	const struct rockchip_pwm_regs *regs = &priv->data->regs;
	u32 ctrl;

	debug("%s: Enable '%s'\n", __func__, dev->name);

	ctrl = readl(priv->base + regs->ctrl);
	ctrl &= ~priv->data->enable_conf_mask;

	if (enable)
		ctrl |= priv->data->enable_conf;
	else
		ctrl &= ~priv->data->enable_conf;

	writel(ctrl, priv->base + regs->ctrl);
	if (priv->data->vop_pwm)
		priv->vop_pwm_en = enable;

	if (enable)
		pinctrl_select_state(dev, "active");

	return 0;
}

static int rk_pwm_set_enable(struct udevice *dev, uint channel, bool enable)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	return priv->data->funcs.set_enable(dev, channel, enable);
}

static int rk_pwm_ofdata_to_platdata(struct udevice *dev)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr(dev);

	return 0;
}

static int rk_pwm_probe(struct udevice *dev)
{
	struct rk_pwm_priv *priv = dev_get_priv(dev);
	struct clk clk;
	int ret = 0;

	ret = clk_get_by_index(dev, 0, &clk);
	if (ret < 0) {
		debug("%s get clock fail!\n", __func__);
		return -EINVAL;
	}

	ret = clk_get_rate(&clk);
	if (ret < 0) {
		debug("%s pwm get clock rate fail!\n", __func__);
		return -EINVAL;
	}
	priv->freq = ret;
	priv->data = (struct rockchip_pwm_data *)dev_get_driver_data(dev);

	if (priv->data->supports_polarity)
		priv->conf_polarity = PWM_DUTY_POSTIVE | PWM_INACTIVE_POSTIVE;

	return 0;
}

static const struct pwm_ops rk_pwm_ops = {
	.set_invert	= rk_pwm_set_invert,
	.set_config	= rk_pwm_set_config,
	.set_enable	= rk_pwm_set_enable,
};

static const struct rockchip_pwm_data pwm_data_v1 = {
	.main_version = 0x01,
	.regs = {
		.version = 0x5c,
		.duty = 0x04,
		.period = 0x08,
		.ctrl = 0x0c,
	},
	.prescaler = 2,
	.supports_polarity = false,
	.supports_lock = false,
	.vop_pwm = false,
	.enable_conf = PWM_CTRL_OUTPUT_EN | PWM_CTRL_TIMER_EN,
	.enable_conf_mask = BIT(1) | BIT(3),
	.funcs = {
		.set_invert = rk_pwm_set_invert_v1,
		.set_config = rk_pwm_set_config_v1,
		.set_enable = rk_pwm_set_enable_v1,
	},
};

static const struct rockchip_pwm_data pwm_data_v2 = {
	.main_version = 0x02,
	.regs = {
		.version = 0x5c,
		.duty = 0x08,
		.period = 0x04,
		.ctrl = 0x0c,
	},
	.prescaler = 1,
	.supports_polarity = true,
	.supports_lock = false,
	.vop_pwm = false,
	.enable_conf = PWM_OUTPUT_LEFT | PWM_LP_DISABLE | RK_PWM_ENABLE |
		       PWM_CONTINUOUS,
	.enable_conf_mask = GENMASK(2, 0) | BIT(5) | BIT(8),
	.funcs = {
		.set_invert = rk_pwm_set_invert_v1,
		.set_config = rk_pwm_set_config_v1,
		.set_enable = rk_pwm_set_enable_v1,
	},
};

static const struct rockchip_pwm_data pwm_data_vop = {
	.main_version = 0x02,
	.regs = {
		.version = 0x5c,
		.duty = 0x08,
		.period = 0x04,
		.ctrl = 0x00,
	},
	.prescaler = 1,
	.supports_polarity = true,
	.supports_lock = false,
	.vop_pwm = true,
	.enable_conf = PWM_OUTPUT_LEFT | PWM_LP_DISABLE | RK_PWM_ENABLE |
		       PWM_CONTINUOUS,
	.enable_conf_mask = GENMASK(2, 0) | BIT(5) | BIT(8),
	.funcs = {
		.set_invert = rk_pwm_set_invert_v1,
		.set_config = rk_pwm_set_config_v1,
		.set_enable = rk_pwm_set_enable_v1,
	},
};

static const struct rockchip_pwm_data pwm_data_v3 = {
	.main_version = 0x03,
	.regs = {
		.version = 0x5c,
		.duty = 0x08,
		.period = 0x04,
		.ctrl = 0x0c,
	},
	.prescaler = 1,
	.supports_polarity = true,
	.supports_lock = true,
	.vop_pwm = false,
	.enable_conf = PWM_OUTPUT_LEFT | PWM_LP_DISABLE | RK_PWM_ENABLE |
		       PWM_CONTINUOUS,
	.enable_conf_mask = GENMASK(2, 0) | BIT(5) | BIT(8),
	.funcs = {
		.set_invert = rk_pwm_set_invert_v1,
		.set_config = rk_pwm_set_config_v1,
		.set_enable = rk_pwm_set_enable_v1,
	},
};

static const struct rockchip_pwm_data pwm_data_v4 = {
	.main_version = 0x04,
	.regs = {
		.version = 0x0,
		.enable = 0x4,
		.ctrl = 0xc,
		.period = 0x10,
		.duty = 0x14,
	},
	.prescaler = 1,
	.supports_polarity = true,
	.supports_lock = true,
	.vop_pwm = false,
	.enable_conf = PWM_ENABLE_V4,
	.funcs = {
		.set_invert = rk_pwm_set_invert_v4,
		.set_config = rk_pwm_set_config_v4,
		.set_enable = rk_pwm_set_enable_v4,
	},
};

static const struct udevice_id rk_pwm_ids[] = {
	{ .compatible = "rockchip,rk2928-pwm", .data = (ulong)&pwm_data_v1},
	{ .compatible = "rockchip,rk3288-pwm", .data = (ulong)&pwm_data_v2},
	{ .compatible = "rockchip,rk3328-pwm", .data = (ulong)&pwm_data_v3},
	{ .compatible = "rockchip,vop-pwm", .data = (ulong)&pwm_data_vop},
	{ .compatible = "rockchip,rk3399-pwm", .data = (ulong)&pwm_data_v2},
	{ .compatible = "rockchip,rk3576-pwm", .data = (ulong)&pwm_data_v4},
	{ }
};

U_BOOT_DRIVER(rk_pwm) = {
	.name	= "rk_pwm",
	.id	= UCLASS_PWM,
	.of_match = rk_pwm_ids,
	.ops	= &rk_pwm_ops,
	.ofdata_to_platdata	= rk_pwm_ofdata_to_platdata,
	.probe		= rk_pwm_probe,
	.priv_auto_alloc_size	= sizeof(struct rk_pwm_priv),
};
