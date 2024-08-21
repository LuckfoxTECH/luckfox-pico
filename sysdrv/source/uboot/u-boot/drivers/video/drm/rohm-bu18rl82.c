// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <video_bridge.h>
#include <asm/unaligned.h>
#include <linux/media-bus-format.h>
#include <power/regulator.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

#define BU18RL82_REG_RESET 0X000E

#define BU18RL82_SWRST_REG BIT(0)
#define BU18RL82_SWRST_EXCREG BIT(1)
#define BU18RL82_SWRST_ALL BIT(7)

struct des_reg_sequence {
	uint reg;
	uint def;
};

struct serdes_init_seq {
	struct des_reg_sequence *reg_sequence;
	uint reg_seq_cnt;
};

struct bu18rl82_priv {
	struct udevice *dev;
	struct serdes_init_seq *serdes_init_seq;
};

static void bu18rl82_bridge_reset(struct rockchip_bridge *bridge)
{
	int ret = 0;
	struct udevice *dev = bridge->dev;
	struct udevice *bus = dev_get_parent(dev);

	ret = dm_i2c_reg_write(dev, BU18RL82_REG_RESET,
			       (BU18RL82_SWRST_REG | BU18RL82_SWRST_EXCREG | BU18RL82_SWRST_ALL));
	if (ret < 0)
		printf("failed to reset bu18rl82(%s) ret=%d\n", bus->name, ret);
	mdelay(5);
}

static int bu18rl82_serdes_init_sequence_write(struct bu18rl82_priv *priv)
{
	struct serdes_init_seq *serdes_init_seq = priv->serdes_init_seq;
	struct des_reg_sequence *reg_sequence =  serdes_init_seq->reg_sequence;
	uint cnt = serdes_init_seq->reg_seq_cnt;
	struct udevice *dev = priv->dev;
	uint i;
	int ret = 0;

	for (i = 0; i < cnt; i++) {
		ret = dm_i2c_reg_write(dev, reg_sequence[i].reg, reg_sequence[i].def);
		if (ret < 0) {
			dev_err(priv->dev, "failed write reg: 0x%04x value: 0x%04x\n",
				reg_sequence[i].reg, reg_sequence[i].def);
			break;
		}
	}

	return ret;
}

static void bu18rl82_bridge_enable(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	struct bu18rl82_priv *priv = dev_get_priv(dev);
	struct udevice *bus = dev_get_parent(dev);
	int i;
	int ret;

	for (i = 0; i < 10; i++) {
		ret = bu18rl82_serdes_init_sequence_write(priv);
		if (ret < 0) {
			dev_err(priv->dev, "%s ret=%d\n", bus->name, ret);
			continue;
		}

		break;
	}
}

static const struct rockchip_bridge_funcs bu18rl82_bridge_funcs = {
	.enable = bu18rl82_bridge_enable,
};

static int bu18rl82_parse_init_seq(struct udevice *dev, const u16 *data,
				   int length, struct serdes_init_seq *seq)
{
	struct des_reg_sequence *reg_sequence;
	u16 *buf, *d;
	unsigned int i, cnt;
	int ret;

	if (!seq)
		return -EINVAL;

	buf = calloc(1, length);
	if (!buf)
		return -ENOMEM;

	memcpy(buf, data, length);

	d = buf;
	cnt = length / 4;
	seq->reg_seq_cnt = cnt;

	seq->reg_sequence = calloc(cnt, sizeof(struct des_reg_sequence));
	if (!seq->reg_sequence) {
		ret = -ENOMEM;
		goto free_buf;
	}

	for (i = 0; i < cnt; i++) {
		reg_sequence = &seq->reg_sequence[i];
		reg_sequence->reg = get_unaligned_be16(&d[0]);
		reg_sequence->def = get_unaligned_be16(&d[1]);
		d += 2;
	}

	return 0;

free_buf:
	free(buf);

	return ret;
}

static int bu18rl82_get_init_seq(struct bu18rl82_priv *priv)
{
	const void *data = NULL;
	int len, err;

	data = dev_read_prop(priv->dev, "serdes-init-sequence", &len);
	if (!data) {
		printf("failed to get serdes-init-sequence\n");
		return -EINVAL;
	}

	priv->serdes_init_seq = calloc(1, sizeof(*priv->serdes_init_seq));
	if (!priv->serdes_init_seq)
		return -ENOMEM;

	err = bu18rl82_parse_init_seq(priv->dev, data, len, priv->serdes_init_seq);
	if (err) {
		printf("failed to parse serdes-init-sequence\n");
		goto free_init_seq;
	}

	return 0;

free_init_seq:
	free(priv->serdes_init_seq);

	return err;
}

static int bu18rl82_probe(struct udevice *dev)
{
	struct bu18rl82_priv *priv = dev_get_priv(dev);
	struct rockchip_bridge *bridge;
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	priv->dev = dev;

	bridge = calloc(1, sizeof(*bridge));
	if (!bridge)
		return -ENOMEM;

	ret = bu18rl82_get_init_seq(priv);
	if (ret)
		goto free_bridge;

	dev->driver_data = (ulong)bridge;
	bridge->dev = dev;
	bridge->funcs = &bu18rl82_bridge_funcs;

	bu18rl82_bridge_reset(bridge);

	return 0;

free_bridge:
	free(bridge);

	return ret;
}

static const struct udevice_id bu18rl82_of_match[] = {
	{ .compatible = "rohm,bu18rl82", },
	{}
};

U_BOOT_DRIVER(bu18rl82) = {
	.name = "bu18rl82",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = bu18rl82_of_match,
	.probe = bu18rl82_probe,
	.priv_auto_alloc_size = sizeof(struct bu18rl82_priv),
};
