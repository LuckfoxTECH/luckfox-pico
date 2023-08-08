// SPDX-License-Identifier:     GPL-2.0+
/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <power/pmic.h>
#include <power/rk8xx_pmic.h>
#include <sound.h>
#include "rk817_codec.h"

#define DBG(format, ...) \
		printf("RK817: " format, ## __VA_ARGS__)

/* For route */
#define RK817_CODEC_PLAYBACK	1
#define RK817_CODEC_CAPTURE	2
#define RK817_CODEC_INCALL	4
#define RK817_CODEC_ALL	(RK817_CODEC_PLAYBACK |\
	RK817_CODEC_CAPTURE | RK817_CODEC_INCALL)

/*
 * DDAC L/R volume setting
 * 0db~-95db,0.375db/step,for example:
 * 0: 0dB
 * 0x0a: -3.75dB
 * 0x7d: -46dB
 * 0xff: -95dB
 */
#define OUT_VOLUME	(0x03)

#define CODEC_SET_SPK 1
#define CODEC_SET_HP 2
#define INITIAL_VOLUME	3

struct rk817_codec_priv {
	struct udevice *dev;
	struct rk8xx_priv *rk817;
	unsigned int stereo_sysclk;
	unsigned int rate;
	unsigned int spk_volume;
	unsigned int hp_volume;
	bool use_ext_amplifier;
	long int playback_path;
	int spk_mute_delay;
	int hp_mute_delay;
};

static int snd_soc_write(struct udevice *dev, unsigned int reg,
			 unsigned int val)
{
	return pmic_reg_write(dev, reg, val);
}

static int snd_soc_update_bits(struct udevice *dev, unsigned int reg,
			       unsigned int mask, unsigned int value)
{
	return pmic_clrsetbits(dev, reg, mask, value);
}

static int rk817_reset(struct rk817_codec_priv *priv)
{
	struct udevice *codec = priv->dev->parent;

	snd_soc_write(codec, RK817_CODEC_DTOP_LPT_SRST, 0x40);
	snd_soc_write(codec, RK817_CODEC_DDAC_POPD_DACST, 0x02);

	return 0;
}

static struct rk817_reg_val_typ playback_power_up_list[] = {
	{RK817_CODEC_AREF_RTCFG1, 0x40},
	{RK817_CODEC_DDAC_POPD_DACST, 0x02},
	{RK817_CODEC_DDAC_SR_LMT0, 0x02},
	/* {RK817_CODEC_DTOP_DIGEN_CLKE, 0x0f}, */
	/* APLL */
	{RK817_CODEC_APLL_CFG0, 0x04},
	{RK817_CODEC_APLL_CFG1, 0x58},
	{RK817_CODEC_APLL_CFG2, 0x2d},
	{RK817_CODEC_APLL_CFG3, 0x0c},
	{RK817_CODEC_APLL_CFG4, 0xa5},
	{RK817_CODEC_APLL_CFG5, 0x00},

	{RK817_CODEC_DI2S_RXCMD_TSD, 0x00},
	{RK817_CODEC_DI2S_RSD, 0x00},
	/* {RK817_CODEC_DI2S_CKM, 0x00}, */
	{RK817_CODEC_DI2S_RXCR1, 0x00},
	{RK817_CODEC_DI2S_RXCMD_TSD, 0x20},
	{RK817_CODEC_DTOP_VUCTIME, 0xf4},
	{RK817_CODEC_DDAC_MUTE_MIXCTL, 0x00},

	{RK817_CODEC_DDAC_VOLL, 0x0a},
	{RK817_CODEC_DDAC_VOLR, 0x0a},
};

#define RK817_CODEC_PLAYBACK_POWER_UP_LIST_LEN \
	ARRAY_SIZE(playback_power_up_list)

static struct rk817_reg_val_typ playback_power_down_list[] = {
	{RK817_CODEC_DDAC_MUTE_MIXCTL, 0x01},
	{RK817_CODEC_ADAC_CFG1, 0x0f},
	/* HP */
	{RK817_CODEC_AHP_CFG0, 0xe0},
	{RK817_CODEC_AHP_CP, 0x09},
	/* SPK */
	{RK817_CODEC_ACLASSD_CFG1, 0x69},
};

#define RK817_CODEC_PLAYBACK_POWER_DOWN_LIST_LEN \
	ARRAY_SIZE(playback_power_down_list)

static int rk817_codec_power_up(struct rk817_codec_priv *rk817, int type)
{
	struct udevice *codec = rk817->dev->parent;
	int i;

	DBG("%s : power up %s %s %s\n", __func__,
	    type & RK817_CODEC_PLAYBACK ? "playback" : "",
	    type & RK817_CODEC_CAPTURE ? "capture" : "",
	    type & RK817_CODEC_INCALL ? "incall" : "");

	if (type & RK817_CODEC_PLAYBACK) {
		snd_soc_update_bits(codec, RK817_CODEC_DTOP_DIGEN_CLKE,
				    DAC_DIG_CLK_MASK, DAC_DIG_CLK_EN);
		for (i = 0; i < RK817_CODEC_PLAYBACK_POWER_UP_LIST_LEN; i++) {
			snd_soc_write(codec, playback_power_up_list[i].reg,
				      playback_power_up_list[i].value);
		}
	}

	return 0;
}

static int rk817_codec_power_down(struct rk817_codec_priv *rk817, int type)
{
	struct udevice *codec = rk817->dev->parent;
	int i;

	DBG("%s : power down %s %s %s\n", __func__,
	    type & RK817_CODEC_PLAYBACK ? "playback" : "",
	    type & RK817_CODEC_CAPTURE ? "capture" : "",
	    type & RK817_CODEC_INCALL ? "incall" : "");

	/* mute output for pop noise */
	if ((type & RK817_CODEC_PLAYBACK) ||
	    (type & RK817_CODEC_INCALL)) {
		snd_soc_update_bits(codec, RK817_CODEC_DDAC_MUTE_MIXCTL,
				    DACMT_ENABLE, DACMT_ENABLE);
	}

	if (type & RK817_CODEC_PLAYBACK) {
		for (i = 0; i < RK817_CODEC_PLAYBACK_POWER_DOWN_LIST_LEN; i++) {
			snd_soc_write(codec, playback_power_down_list[i].reg,
				      playback_power_down_list[i].value);
		}
		snd_soc_update_bits(codec, RK817_CODEC_DTOP_DIGEN_CLKE,
				    DAC_DIG_CLK_MASK, DAC_DIG_CLK_DIS);
	}

	if (type == RK817_CODEC_ALL) {
		for (i = 0; i < RK817_CODEC_PLAYBACK_POWER_DOWN_LIST_LEN; i++) {
			snd_soc_write(codec, playback_power_down_list[i].reg,
				      playback_power_down_list[i].value);
		}
		snd_soc_write(codec, RK817_CODEC_DTOP_DIGEN_CLKE, 0x00);
		snd_soc_write(codec, RK817_CODEC_APLL_CFG5, 0x01);
		snd_soc_write(codec, RK817_CODEC_AREF_RTCFG1, 0x06);
	}

	return 0;
}

static int rk817_playback_path_put(struct rk817_codec_priv *rk817, int path)
{
	struct udevice *codec = rk817->dev->parent;
	long int pre_path;

	if (rk817->playback_path == path) {
		DBG("%s : playback_path is not changed!\n", __func__);
		return 0;
	}

	pre_path = rk817->playback_path;
	rk817->playback_path = path;

	DBG("%s : set playback_path %ld, pre_path %ld\n",
	    __func__, rk817->playback_path, pre_path);

	switch (rk817->playback_path) {
	case OFF:
		rk817_codec_power_down(rk817, RK817_CODEC_PLAYBACK);
		break;
	case RCV:
	case SPK_PATH:
	case RING_SPK:
		if (pre_path == OFF)
			rk817_codec_power_up(rk817, RK817_CODEC_PLAYBACK);
		if (!rk817->use_ext_amplifier) {
			/* power on dac ibias/l/r */
			snd_soc_write(codec, RK817_CODEC_ADAC_CFG1,
				      PWD_DACBIAS_ON | PWD_DACD_ON |
				      PWD_DACL_ON | PWD_DACR_ON);
			/* CLASS D mode */
			snd_soc_write(codec, RK817_CODEC_DDAC_MUTE_MIXCTL, 0x10);
			/* CLASS D enable */
			snd_soc_write(codec, RK817_CODEC_ACLASSD_CFG1, 0xa5);
			/* restart CLASS D, OCPP/N */
			snd_soc_write(codec, RK817_CODEC_ACLASSD_CFG2, 0xc4);
		} else {
			/* HP_CP_EN , CP 2.3V */
			snd_soc_write(codec, RK817_CODEC_AHP_CP, 0x11);
			/* power on HP two stage opamp ,HP amplitude 0db */
			snd_soc_write(codec, RK817_CODEC_AHP_CFG0, 0x80);
			/* power on dac ibias/l/r */
			snd_soc_write(codec, RK817_CODEC_ADAC_CFG1,
				      PWD_DACBIAS_ON | PWD_DACD_DOWN |
				      PWD_DACL_ON | PWD_DACR_ON);
			snd_soc_update_bits(codec, RK817_CODEC_DDAC_MUTE_MIXCTL,
					    DACMT_ENABLE, DACMT_DISABLE);
		}
		snd_soc_write(codec, RK817_CODEC_DDAC_VOLL, rk817->spk_volume);
		snd_soc_write(codec, RK817_CODEC_DDAC_VOLR, rk817->spk_volume);
		break;
	case HP_PATH:
	case HP_NO_MIC:
	case RING_HP:
	case RING_HP_NO_MIC:
		if (pre_path == OFF)
			rk817_codec_power_up(rk817, RK817_CODEC_PLAYBACK);
		/* HP_CP_EN , CP 2.3V */
		snd_soc_write(codec, RK817_CODEC_AHP_CP, 0x11);
		/* power on HP two stage opamp ,HP amplitude 0db */
		snd_soc_write(codec, RK817_CODEC_AHP_CFG0, 0x80);
		/* power on dac ibias/l/r */
		snd_soc_write(codec, RK817_CODEC_ADAC_CFG1,
			      PWD_DACBIAS_ON | PWD_DACD_DOWN |
			      PWD_DACL_ON | PWD_DACR_ON);
		snd_soc_update_bits(codec, RK817_CODEC_DDAC_MUTE_MIXCTL,
				    DACMT_ENABLE, DACMT_DISABLE);

		snd_soc_write(codec, RK817_CODEC_DDAC_VOLL, rk817->hp_volume);
		snd_soc_write(codec, RK817_CODEC_DDAC_VOLR, rk817->hp_volume);
		break;
	case BT:
		break;
	case SPK_HP:
	case RING_SPK_HP:
		if (pre_path == OFF)
			rk817_codec_power_up(rk817, RK817_CODEC_PLAYBACK);

		/* HP_CP_EN , CP 2.3V  */
		snd_soc_write(codec, RK817_CODEC_AHP_CP, 0x11);
		/* power on HP two stage opamp ,HP amplitude 0db */
		snd_soc_write(codec, RK817_CODEC_AHP_CFG0, 0x80);

		/* power on dac ibias/l/r */
		snd_soc_write(codec, RK817_CODEC_ADAC_CFG1,
			      PWD_DACBIAS_ON | PWD_DACD_ON |
			      PWD_DACL_ON | PWD_DACR_ON);

		if (!rk817->use_ext_amplifier) {
			/* CLASS D mode */
			snd_soc_write(codec, RK817_CODEC_DDAC_MUTE_MIXCTL, 0x10);
			/* CLASS D enable */
			snd_soc_write(codec, RK817_CODEC_ACLASSD_CFG1, 0xa5);
			/* restart CLASS D, OCPP/N */
			snd_soc_write(codec, RK817_CODEC_ACLASSD_CFG2, 0xc4);
		}

		snd_soc_write(codec, RK817_CODEC_DDAC_VOLL, rk817->hp_volume);
		snd_soc_write(codec, RK817_CODEC_DDAC_VOLR, rk817->hp_volume);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int rk817_hw_params(struct udevice *dev, unsigned int samplerate,
			   unsigned int fmt, unsigned int channels)
{
	struct rk817_codec_priv *rk817 = dev_get_priv(dev);
	struct udevice *codec = rk817->dev->parent;

	snd_soc_update_bits(codec, RK817_CODEC_DI2S_CKM,
			    RK817_I2S_MODE_MASK, RK817_I2S_MODE_SLV);
	snd_soc_write(codec, RK817_CODEC_DI2S_RXCR2, VDW_RX_16BITS);
	snd_soc_write(codec, RK817_CODEC_DI2S_TXCR2, VDW_TX_16BITS);

	return 0;
}

static int rk817_digital_mute(struct rk817_codec_priv *rk817, int mute)
{
	struct udevice *codec = rk817->dev->parent;

	if (mute)
		snd_soc_update_bits(codec, RK817_CODEC_DDAC_MUTE_MIXCTL,
				    DACMT_ENABLE, DACMT_ENABLE);
	else
		snd_soc_update_bits(codec, RK817_CODEC_DDAC_MUTE_MIXCTL,
				    DACMT_ENABLE, DACMT_DISABLE);

	return 0;
}

static int rk817_startup(struct udevice *dev)
{
	struct rk817_codec_priv *rk817 = dev_get_priv(dev);

	rk817_playback_path_put(rk817, SPK_HP);
	rk817_digital_mute(rk817, 0);

	return 0;
}

static const struct snd_soc_dai_ops rk817_codec_ops = {
	.hw_params = rk817_hw_params,
	.startup = rk817_startup,
};

static int rk817_codec_probe(struct udevice *dev)
{
	struct rk8xx_priv *rk817 = dev_get_priv(dev->parent);
	struct rk817_codec_priv *rk817_codec = dev_get_priv(dev);

	if (!rk817) {
		printf("%s : rk817 is null\n", __func__);
		return -EINVAL;
	}

	switch (rk817->variant) {
	case RK809_ID:
	case RK817_ID:
		break;
	default:
		return -EINVAL;
	}

	rk817_codec->dev = dev;
	rk817_codec->hp_volume = INITIAL_VOLUME;
	rk817_codec->spk_volume = INITIAL_VOLUME;
	rk817_codec->playback_path = OFF;
	rk817_reset(rk817_codec);

	return 0;
}

static const struct udevice_id rk817_codec_ids[] = {
	{ .compatible = "rockchip,rk817-codec" },
	{ }
};

U_BOOT_DRIVER(rk817) = {
	.name = "rk817_codec",
	.id = UCLASS_CODEC,
	.of_match = rk817_codec_ids,
	.probe = rk817_codec_probe,
	.priv_auto_alloc_size = sizeof(struct rk817_codec_priv),
	.ops = &rk817_codec_ops,
};

UCLASS_DRIVER(codec) = {
	.id = UCLASS_CODEC,
	.name = "codec",
};
