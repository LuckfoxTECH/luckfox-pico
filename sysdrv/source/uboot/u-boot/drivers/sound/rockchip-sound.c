// SPDX-License-Identifier:     GPL-2.0+
/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 */

#include <dm.h>
#include <malloc.h>
#include <common.h>
#include <asm/io.h>
#include <linux/libfdt.h>
#include <fdtdec.h>
#include <i2s.h>
#include <sound.h>
#include <asm/arch-rockchip/resource_img.h>

#define WAV_SIZE		(5 * 1024 * 1024) /* BYTE */
#define SAMPLERATE		44100

static struct udevice *i2s_dev, *codec_dev;

static int load_audio_wav(void *buf, const char *wav_name, int size)
{
	int ret = 0;
#ifdef CONFIG_ROCKCHIP_RESOURCE_IMAGE
	ret = rockchip_read_resource_file(buf, wav_name, 0, size);
#endif

	return ret;
}

static int sound_hw_params(struct udevice *dev, unsigned int samplerate,
			   unsigned int fmt, unsigned int channels)
{
	const struct snd_soc_dai_ops *ops = dev_get_driver_ops(dev);

	if (!ops || !ops->hw_params)
		return -ENOTSUPP;

	return ops->hw_params(dev, samplerate, fmt, channels);
}

static int sound_startup(struct udevice *dev)
{
	const struct snd_soc_dai_ops *ops = dev_get_driver_ops(dev);

	if (!ops || !ops->startup)
		return -ENOTSUPP;

	return ops->startup(dev);
}

static int sound_set_sysclk(struct udevice *dev, unsigned int freq)
{
	const struct snd_soc_dai_ops *ops = dev_get_driver_ops(dev);

	if (!ops || !ops->set_sysclk)
		return -ENOTSUPP;

	return ops->set_sysclk(dev, freq);
}

int sound_init(const void *blob)
{
	int ret;

	ret = uclass_get_device(UCLASS_I2S, 0, &i2s_dev);
	if (ret) {
		if (ret != -ENODEV) {
			printf("Get i2s device failed: %d\n", ret);
			return ret;
		}
		return 0;
	}

	ret = uclass_get_device(UCLASS_CODEC, 0, &codec_dev);
	if (ret) {
		if (ret != -ENODEV) {
			printf("Get codec device failed: %d\n", ret);
			return ret;
		}
		return 0;
	}

	sound_set_sysclk(i2s_dev, SAMPLERATE * 256);
	sound_hw_params(i2s_dev, SAMPLERATE, 16, 2);
	sound_hw_params(codec_dev, SAMPLERATE, 16, 2);
	sound_startup(i2s_dev);
	sound_startup(codec_dev);

	return ret;
}

static int _sound_play(struct udevice *dev, unsigned int *data,
		       unsigned long data_size)
{
	const struct snd_soc_dai_ops *ops = dev_get_driver_ops(dev);

	if (!ops || !ops->transfer)
		return -ENOTSUPP;

	return ops->transfer(dev, data, data_size);
}

int sound_play(u32 msec, u32 frequency)
{
	unsigned int *buf;
	unsigned long buf_size;
	unsigned int ret = 0;

	buf_size = WAV_SIZE;

	buf = malloc(buf_size);
	if (!buf) {
		debug("%s: buf malloc failed\n", __func__);
		return -ENOMEM;
	}
	ret = load_audio_wav(buf, "boot.wav", buf_size);
	/* if boot.wav not find, use sound_create_square_wave */
	if (ret <= 0)
		sound_create_square_wave((unsigned short *)buf,
					 buf_size / sizeof(unsigned short),
					 frequency);

	ret = _sound_play(i2s_dev, buf, (buf_size / sizeof(int)));
	free(buf);

	return ret;
}
