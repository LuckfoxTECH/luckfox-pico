/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    es8156.c
  * @author  Xing Zheng
  * @version v0.1
  * @date    2020.04.21
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CODEC_ES8156

#include "board.h"
#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "es8156.h"

#define ES8156_DB_MIN          (-95500)
#define ES8156_DB_MAX          (32000)
#define ES8156_DB_STEP         (500)
#define ES8156_0DB_BASE        (0xBF)

struct es8156_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client;
};

rt_inline struct es8156_priv *to_es8156_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct es8156_priv, codec);
}

static rt_err_t es_wr_reg(struct rt_i2c_client *i2c_client,
                          uint8_t reg8, uint8_t data8)
{
    struct rt_i2c_msg msgs[1];
    uint8_t data_buf[2];
    rt_err_t ret;

    data_buf[0] = reg8;
    data_buf[1] = data8;

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = data_buf;
    msgs[0].len = 2;

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 1);
    if (ret != 1)
    {
        rt_kprintf("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    return RT_EOK;
}

static rt_err_t es_rd_reg(struct rt_i2c_client *i2c_client,
                          uint8_t reg8, uint8_t *data8)
{
    struct rt_i2c_msg msgs[2];
    rt_err_t ret;

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg8;
    msgs[0].len = 1;

    msgs[1].addr = i2c_client->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = data8;
    msgs[1].len = 1;

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 2);
    if (ret != 2)
    {
        rt_kprintf("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    return RT_EOK;
}

static rt_err_t es_update_bits(struct rt_i2c_client *i2c_client,
                               uint8_t reg, uint8_t mask, uint8_t val)
{
    uint8_t old, new;
    int ret;

    ret = es_rd_reg(i2c_client, reg, &old);
    if (ret < 0)
        goto err;

    new = (old & ~mask) | (val & mask);
    ret = es_wr_reg(i2c_client, reg, new);

err:
    return ret;
}

static int es8156_reg_powerup(struct es8156_priv *es8156)
{
    /*
     * set clock: SCLK is as MCLK
     */
    es_update_bits(es8156->i2c_client, ES8156_SCLK_MODE_REG02, 0xEE, 0x84);
    /*
     * set analog power
     */
    es_wr_reg(es8156->i2c_client, ES8156_SCLK_MODE_REG02, 0x84);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS1_REG20, 0x2A);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS2_REG21, 0x3C);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS3_REG22, 0x02);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_LP_REG24, 0x07);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS4_REG23, 0xFA);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS3_REG22, 0x00);

    /*
     * set powerup time
     */
    es_wr_reg(es8156->i2c_client, ES8156_TIME_CONTROL1_REG0A, 0x01);
    es_wr_reg(es8156->i2c_client, ES8156_TIME_CONTROL2_REG0B, 0x01);

    /*
     * set digtal volume: ES8156_VOLUME_CONTROL_REG14: is 0xBF(0dB) by default
     */

    /*
     * set MCLK: just using sclk, mclk is not used, slave mode
     */
    es_wr_reg(es8156->i2c_client, ES8156_MAINCLOCK_CTL_REG01, 0xE0);
    es_wr_reg(es8156->i2c_client, ES8156_P2S_CONTROL_REG0D, 0x14);
    es_wr_reg(es8156->i2c_client, ES8156_MISC_CONTROL3_REG18, 0x00);
    es_wr_reg(es8156->i2c_client, ES8156_CLOCK_ON_OFF_REG08, 0x3F);
    es_wr_reg(es8156->i2c_client, ES8156_RESET_REG00, 0x02);
    es_wr_reg(es8156->i2c_client, ES8156_RESET_REG00, 0x03);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS5_REG25, 0x20);

    return RT_EOK;
}

static int es8156_reg_powerdown(struct es8156_priv *es8156)
{
    /*
     * set digtal volume: skip set ES8156_VOLUME_CONTROL_REG14
     */
    es_wr_reg(es8156->i2c_client, ES8156_EQ_CONTROL1_REG19, 0x02);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS2_REG21, 0x1F);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS3_REG22, 0x03);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS5_REG25, 0x21);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS5_REG25, 0x01);
    es_wr_reg(es8156->i2c_client, ES8156_ANALOG_SYS5_REG25, 0x87);
    es_wr_reg(es8156->i2c_client, ES8156_MISC_CONTROL3_REG18, 0x01);
    es_wr_reg(es8156->i2c_client, ES8156_MISC_CONTROL2_REG09, 0x02);
    es_wr_reg(es8156->i2c_client, ES8156_MISC_CONTROL2_REG09, 0x01);
    es_wr_reg(es8156->i2c_client, ES8156_CLOCK_ON_OFF_REG08, 0x00);

    return RT_EOK;
}

static int es8156_reg_mute(struct es8156_priv *es8156, rt_bool_t mute)
{
    if (mute)
        es_update_bits(es8156->i2c_client, ES8156_DAC_MUTE_REG13, 0x10, 0x00);
    else
        es_update_bits(es8156->i2c_client, ES8156_DAC_MUTE_REG13, 0x10, 0x10);

    return RT_EOK;
}

static rt_err_t es8156_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    struct es8156_priv *es8156 = to_es8156_priv(codec);

    switch (config->format)
    {
    case AUDIO_FMT_I2S:
        es_update_bits(es8156->i2c_client, ES8156_DAC_SDP_REG11, 0x07, 0x00);
        break;
    case AUDIO_FMT_LEFT_J:
        es_update_bits(es8156->i2c_client, ES8156_DAC_SDP_REG11, 0x07, 0x01);
        break;
    case AUDIO_FMT_PCM:
        es_update_bits(es8156->i2c_client, ES8156_DAC_SDP_REG11, 0x07, 0x03);
    case AUDIO_FMT_PCM_DELAY1:
        es_update_bits(es8156->i2c_client, ES8156_DAC_SDP_REG11, 0x07, 0x07);
        break;
    default:
        rt_kprintf("ERR: %s, Invalid format: %d\n",
                   __func__, config->format);
        return -RT_EINVAL;
    }

    if (config->clkInvert)
        es_update_bits(es8156->i2c_client, ES8156_SCLK_MODE_REG02, 0x10, 0x01);
    else
        es_update_bits(es8156->i2c_client, ES8156_SCLK_MODE_REG02, 0x10, 0x00);

    if (config->master)
        /* codec is master */
        es_update_bits(es8156->i2c_client, ES8156_SCLK_MODE_REG02, 0x01, 0x01);
    else
        /* codec is slave */
        es_update_bits(es8156->i2c_client, ES8156_SCLK_MODE_REG02, 0x01, 0x00);

    return RT_EOK;
}

static rt_err_t es8156_deinit(struct audio_codec *codec)
{
    return RT_EOK;
}

static rt_err_t es8156_set_clk(struct audio_codec *codec,
                               eAUDIO_streamType stream,
                               uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t es8156_config(struct audio_codec *codec,
                              eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct es8156_priv *es8156 = to_es8156_priv(codec);
    uint8_t val;
    rt_err_t ret;

    if (stream == AUDIO_STREAM_CAPTURE)
        return RT_EOK;

    /* Vendor suggest  that after bclk output and init codec */
    switch (params->sampleBits)
    {
    case 16:
        val = 0x30;
        break;
    case 20:
        val = 0x10;
        break;
    case 24:
        val = 0;
        break;
    case 32:
        val = 0x40;
        break;
    default:
        rt_kprintf("ERR: %s, Invalid sampleBits: %d\n",
                   __func__, params->sampleBits);
        return -RT_EINVAL;
    }

    ret = es_update_bits(es8156->i2c_client, ES8156_DAC_SDP_REG11, 0x70, val);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        return ret;
    }

    es8156_reg_mute(es8156, RT_FALSE);

    return RT_EOK;
}

static rt_err_t es8156_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct es8156_priv *es8156 = to_es8156_priv(codec);

    if (stream == AUDIO_STREAM_CAPTURE)
        return RT_EOK;

    es8156_reg_powerup(es8156);

    return RT_EOK;
}

static rt_err_t es8156_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct es8156_priv *es8156 = to_es8156_priv(codec);

    if (stream == AUDIO_STREAM_CAPTURE)
        return RT_EOK;

    es8156_reg_mute(es8156, RT_TRUE);
    es8156_reg_powerdown(es8156);

    return RT_EOK;
}

static rt_err_t es8156_set_gain(struct audio_codec *codec,
                                eAUDIO_streamType stream,
                                struct AUDIO_DB_CONFIG *db_config)
{
    struct es8156_priv *es8156 = to_es8156_priv(codec);
    int dB = db_config->dB;

    if (stream == AUDIO_STREAM_CAPTURE)
        return RT_EOK;

    if (dB >= 0)
    {
        if (dB > ES8156_DB_MAX)
            dB = ES8156_DB_MAX;

        dB = dB / ES8156_DB_STEP + ES8156_0DB_BASE;
    }
    else
    {
        if (dB < ES8156_DB_MIN)
            dB = ES8156_DB_MIN;

        dB = -(dB / ES8156_DB_STEP);
        dB = (ES8156_0DB_BASE - dB) % ES8156_0DB_BASE;
    }

    es_wr_reg(es8156->i2c_client, ES8156_VOLUME_CONTROL_REG14, dB);

    return RT_EOK;
}

static rt_err_t es8156_get_gain(struct audio_codec *codec,
                                eAUDIO_streamType stream,
                                struct AUDIO_DB_CONFIG *db_config)
{
    struct es8156_priv *es8156 = to_es8156_priv(codec);
    uint8_t dB;

    if (stream == AUDIO_STREAM_CAPTURE)
        return RT_EOK;

    es_rd_reg(es8156->i2c_client, ES8156_VOLUME_CONTROL_REG14, &dB);
    if (dB >= ES8156_0DB_BASE)
    {
        /* Positive Gain */
        db_config->dB = (dB - ES8156_0DB_BASE) * ES8156_DB_STEP;
    }
    else
    {
        /* Negative Gain */
        dB = (ES8156_0DB_BASE - dB) % ES8156_0DB_BASE;
        db_config->dB = -(dB * ES8156_DB_STEP);
    }

    return RT_EOK;
}

static rt_err_t es8156_get_gaininfo(struct audio_codec *codec,
                                    struct AUDIO_GAIN_INFO *info)
{
    if (!info)
        return -RT_EINVAL;

    info->mindB = ES8156_DB_MIN;
    info->maxdB = ES8156_DB_MAX;
    info->step = ES8156_DB_STEP;

    return RT_EOK;
}

static const struct audio_codec_ops es8156_ops =
{
    .init = es8156_init,
    .deinit = es8156_deinit,
    .set_clk = es8156_set_clk,
    .config = es8156_config,
    .start = es8156_start,
    .stop = es8156_stop,
    .set_gain = es8156_set_gain,
    .get_gain = es8156_get_gain,
    .get_gaininfo = es8156_get_gaininfo,
};

int rt_hw_codec_es8156_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_es8156;
    struct es8156_priv *es8156;
    uint8_t chip_id0, chip_id1;
    rt_err_t ret = RT_EOK;

    es8156 = rt_calloc(1, sizeof(struct es8156_priv));
    RT_ASSERT(es8156);
    es8156->i2c_client = rt_calloc(1, sizeof(struct rt_i2c_client));
    RT_ASSERT(es8156->i2c_client);

    es8156->i2c_client->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
    RT_ASSERT(es8156->i2c_client->bus);
    es8156->i2c_client->client_addr = codec_dev->i2c_addr;
    es8156->codec.ops = &es8156_ops;
    es8156->codec.id = (uint32_t)codec_dev;

    ret |= es_rd_reg(es8156->i2c_client, ES8156_CHIPID1_REGFD, &chip_id1);
    ret |= es_rd_reg(es8156->i2c_client, ES8156_CHIPID0_REGFE, &chip_id0);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, read codec id failed: %d\n", __func__, ret);
        goto err;
    }
    if (chip_id1 != 0x81 && chip_id0 != 0x55)
    {
        rt_kprintf("ERR: %s, ID[%002x %02x]not es8156\n",
                   __func__, chip_id1, chip_id0);
        goto err;
    }

    ret = rk_audio_register_codec(&es8156->codec);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    rt_kprintf("TAG: register codec es8156 success\n");
    return RT_EOK;

err:
    rt_kprintf("ERR: %s, register codec es8156 failed: %d\n", __func__, ret);
    if (es8156)
        rt_free(es8156);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_es8156_init);

#endif
