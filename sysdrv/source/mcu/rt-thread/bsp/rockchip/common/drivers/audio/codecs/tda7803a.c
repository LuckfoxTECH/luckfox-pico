/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    tda7803a.c
  * @author  Jun Zeng
  * @version v0.1
  * @date    2022.11.25
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CODEC_TDA7803

#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "tda7803a.h"

/* Vendor suggest the startup delay should >100ms */
#define TDA7803_PA_CTL_DELAY_MS      120
#define TDA7803_PA_GPIO_BANK         GPIO_BANK0
#define TDA7803_PA_GPIO              GPIO0
#define TDA7803_PA_PIN               GPIO_PIN_A6
#define TDA7803_PA_PIN_FUNC_GPIO     PIN_CONFIG_MUX_FUNC0

struct tda7803_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client[MAX_DEVICE_NUM];

    uint32_t device_num;
    uint32_t samplerate;
};

static inline struct tda7803_priv *to_tda7803_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct tda7803_priv, codec);
}

static rt_err_t tda_wr_reg(struct rt_i2c_client *i2c_client, rt_uint8_t reg_addr, rt_uint8_t reg_data)
{
    struct rt_i2c_msg msgs[1];
    rt_uint8_t reg_info[2];
    int ret;

    reg_info[0] = reg_addr;
    reg_info[1] = reg_data;

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg_info[0];
    msgs[0].len = 2;

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 1);
    if (ret != 1)
    {
        rt_kprintf("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    return RT_EOK;
}

static rt_err_t tda_rd_reg(struct rt_i2c_client *i2c_client, rt_uint8_t reg_addr, rt_uint8_t *reg_data)
{
    struct rt_i2c_msg msgs[2];
    int ret;

    msgs[0].addr  = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg_addr;
    msgs[0].len   = 1;

    msgs[1].addr  = i2c_client->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = reg_data;
    msgs[1].len   = 1;

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 2);
    if (ret != 2)
    {
        rt_kprintf("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    return RT_EOK;
}

static rt_err_t tda_pa_ctl(bool on)
{
    if (on)
    {
        HAL_GPIO_SetPinLevel(TDA7803_PA_GPIO, TDA7803_PA_PIN, GPIO_HIGH);
        rt_thread_mdelay(TDA7803_PA_CTL_DELAY_MS);
    }
    else
    {
        HAL_GPIO_SetPinLevel(TDA7803_PA_GPIO, TDA7803_PA_PIN, GPIO_LOW);
    }

    return RT_EOK;
}

static rt_err_t tda7803_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    tda_pa_ctl(1);

    return RT_EOK;
}

static rt_err_t tda7803_deinit(struct audio_codec *codec)
{
    tda_pa_ctl(0);

    return RT_EOK;
}

static rt_err_t tda7803_config(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params)
{
    struct tda7803_priv *tda7803 = to_tda7803_priv(codec);
    rt_err_t ret = RT_EOK;

    tda7803->samplerate = params->sampleRate / 4;

    for (int i = 0; i < tda7803->device_num; i++)
    {
        switch (tda7803->samplerate)
        {
        case 44100:
            ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER3, SAMPLE_FREQUENCY_RANGE_44100HZ |
                              DIGITAL_INPUT_FORMAT_TDM_8CH_DEVICE_2);
            break;
        case 48000:
            ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER3, SAMPLE_FREQUENCY_RANGE_48000HZ |
                              DIGITAL_INPUT_FORMAT_TDM_8CH_DEVICE_2);
            break;
        case 96000:
            ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER3, SAMPLE_FREQUENCY_RANGE_96000HZ |
                              DIGITAL_INPUT_FORMAT_TDM_8CH_DEVICE_2);
            break;
        case 192000:
            ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER3, SAMPLE_FREQUENCY_RANGE_192000HZ |
                              DIGITAL_INPUT_FORMAT_TDM_8CH_DEVICE_2);
            break;
        default:
            ret = RT_ERROR;
            break;
        }

        if (ret != RT_EOK)
            rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

        ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER2, DISABLE_DIGITAL_MUTE_OFF |
                          CHANNEL_2_AND_4_UMUTE | CHANNEL_1_AND_3_UMUTE | MUTE_TIME_SETTING_1_45MS);
        if (ret != RT_EOK)
            rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
    }

    return ret;

}

static rt_err_t tda7803_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    /* ECO Mode ---> Amplifier Mode */
    struct tda7803_priv *tda7803 = to_tda7803_priv(codec);
    rt_err_t ret = RT_EOK;

    for (int i = 0; i < tda7803->device_num; i++)
    {
        ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER7, AMPLIEFIR_SWITCH_ON);
    }

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t tda7803_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    /* ECO Mode ---> Amplifier Mode */
    struct tda7803_priv *tda7803 = to_tda7803_priv(codec);
    rt_err_t ret = RT_EOK;

    for (int i = 0; i < tda7803->device_num; i++)
    {
        ret |= tda_wr_reg(tda7803->i2c_client[i], TDA7803A_REGISTER7, AMPLIEFIR_SWITCH_OFF);
    }

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static const struct audio_codec_ops tda7803_ops =
{
    .init = tda7803_init,
    .deinit = tda7803_deinit,
    .config = tda7803_config,
    .start = tda7803_start,
    .stop = tda7803_stop,
};

static int spk_mute_prepare(void)
{
    HAL_PINCTRL_SetIOMUX(TDA7803_PA_GPIO_BANK,
                         TDA7803_PA_PIN,
                         TDA7803_PA_PIN_FUNC_GPIO);
    HAL_GPIO_SetPinDirection(TDA7803_PA_GPIO, TDA7803_PA_PIN, GPIO_OUT);

    HAL_GPIO_SetPinLevel(TDA7803_PA_GPIO, TDA7803_PA_PIN, GPIO_LOW);
}

int rt_hw_codec_tda7803_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_tda7803;
    struct tda7803_priv *tda7803;
    rt_err_t ret = RT_EOK;

    spk_mute_prepare();

    tda7803 = rt_calloc(1, sizeof(struct tda7803_priv));
    RT_ASSERT(tda7803);

    for (int i = 0; i < codec_dev->device_num; i++)
    {
        tda7803->i2c_client[i] = rt_calloc(1, sizeof(struct rt_i2c_client));
        RT_ASSERT(tda7803->i2c_client[i]);
        tda7803->i2c_client[i]->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
        RT_ASSERT(tda7803->i2c_client[i]->bus);
        tda7803->i2c_client[i]->client_addr = codec_dev->i2c_addr[i];
    }

    tda7803->codec.ops = &tda7803_ops;
    tda7803->codec.id = (uint32_t)codec_dev;
    tda7803->device_num = codec_dev->device_num;

    ret |= rk_audio_register_codec(&tda7803->codec);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    rt_kprintf("TAG: register codec tda7803a success\n");
    return RT_EOK;

err:
    rt_kprintf("ERR: %s, register codec tda7803a failed: %d\n", __func__, ret);
    for (int i = 0; i < codec_dev->device_num; i++)
    {
        if (tda7803->i2c_client[i])
            rt_free(tda7803->i2c_client[i]);
    }
    if (tda7803)
        rt_free(tda7803);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_tda7803_init);

#endif

