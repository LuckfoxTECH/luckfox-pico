/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    es7243.c
  * @author  Xiaotan Luo
  * @version v0.1
  * @date    2019.10.16
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CODEC_ES7243

#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "es7243.h"

struct es7243_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client;
};

rt_inline struct es7243_priv *to_es7243_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct es7243_priv, codec);
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

static rt_err_t es7243_reg_init(struct es7243_priv *es7243)
{
    rt_err_t ret = RT_EOK;

    ret |= es_wr_reg(es7243->i2c_client, ES7243_MODECFG_REG00, 0x05);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_STATECTL_REG06, 0x18);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_ANACTL0_REG07, 0x3F);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_ANACTL2_REG09, 0x80);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_SDPFMT_REG01, 0x0C);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_LRCDIV_REG02, 0x10);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_BCKDIV_REG03, 0x04);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_CLKDIV_REG04, 0x01);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_MUTECTL_REG05, 0x1A);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_ANACTL1_REG08, 0x4B);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_ANACTL0_REG07, 0x80);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_ANACTL2_REG09, 0x00);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_STATECTL_REG06, 0x00);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_MUTECTL_REG05, 0x13);
    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es7243_reg_standby(struct es7243_priv *es7243)
{
    rt_err_t ret = RT_EOK;


    ret |= es_wr_reg(es7243->i2c_client, ES7243_STATECTL_REG06, 0x05);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_MUTECTL_REG05, 0x1B);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_STATECTL_REG06, 0x5C);
    ret |= es_wr_reg(es7243->i2c_client, ES7243_ANACTL2_REG09, 0x9F);
    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}
static rt_err_t es7243_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    return RT_EOK;
}

static rt_err_t es7243_deinit(struct audio_codec *codec)
{
    struct es7243_priv *es7243 = to_es7243_priv(codec);
    return es7243_reg_standby(es7243);
}

static rt_err_t es7243_set_clk(struct audio_codec *codec, eAUDIO_streamType stream, uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t es7243_config(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params)
{
    struct es7243_priv *es7243 = to_es7243_priv(codec);
    int wl;
    rt_err_t ret = RT_EOK;

    ret |= es7243_reg_init(es7243);
    switch (params->sampleBits)
    {
    case 16:
        wl = 3;
        break;
    case 18:
        wl = 2;
        break;
    case 20:
        wl = 1;
        break;
    case 24:
        wl = 0;
        break;
    case 32:
        wl = 4;
        break;
    default:
        return -RT_EINVAL;
    }

    ret |= es_update_bits(es7243->i2c_client, ES7243_SDPFMT_REG01,
                          ES7243_SDPFMT_REG01_ADCWL_MASK,
                          wl << ES7243_SDPFMT_REG01_ADCWL_SHIFT);
    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es7243_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    return RT_EOK;
}

static rt_err_t es7243_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    return RT_EOK;
}

static const struct audio_codec_ops es7243_ops =
{
    .init = es7243_init,
    .deinit = es7243_deinit,
    .set_clk = es7243_set_clk,
    .config = es7243_config,
    .start = es7243_start,
    .stop = es7243_stop,
};

int rt_hw_codec_es7243_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_es7243;
    struct es7243_priv *es7243;
    rt_err_t ret = RT_EOK;
    uint8_t reg = 0x0;

    es7243 = rt_calloc(1, sizeof(struct es7243_priv));
    RT_ASSERT(es7243);
    es7243->i2c_client = rt_calloc(1, sizeof(struct rt_i2c_client));
    RT_ASSERT(es7243->i2c_client);

    es7243->i2c_client->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
    RT_ASSERT(es7243->i2c_client->bus);
    es7243->i2c_client->client_addr = codec_dev->i2c_addr;
    es7243->codec.ops = &es7243_ops;
    es7243->codec.id = (uint32_t)codec_dev;

    /* Read codec ID, meanwhile to check the connection of i2c bus first. */
    ret |= es_rd_reg(es7243->i2c_client, ES7243_CHIPID_REG0E, &reg);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, i2c read failed: %d\n", __func__, ret);
        goto err;
    }
    ret |= es7243_reg_init(es7243);
    ret |= rk_audio_register_codec(&es7243->codec);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    rt_kprintf("TAG: register codec es7243 success\n");
    return RT_EOK;

err:
    rt_kprintf("ERR: %s, register codec es7243 failed: %d\n", __func__, ret);
    if (es7243)
        rt_free(es7243);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_es7243_init);

#endif
