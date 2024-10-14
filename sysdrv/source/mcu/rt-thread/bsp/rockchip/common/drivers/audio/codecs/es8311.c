/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    es8311.c
  * @author  Xiaotan Luo
  * @version v0.1
  * @date    2019.10.16
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CODEC_ES8311

#include "board.h"
#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "es8311.h"

/* Vendor suggest the startup delay should >100ms */
#define PA_CTL_DELAY_MS        120
#define ES8311_PA_GPIO_BANK PA_GPIO_BANK
#define ES8311_PA_GPIO      PA_GPIO
#define ES8311_PA_PIN       PA_PIN
#define ES8311_PA_PIN_FUNC_GPIO PA_PIN_FUNC_GPIO

struct es8311_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client;
    uint32_t work_cnt;
};

static struct rt_mutex lock;

rt_inline struct es8311_priv *to_es8311_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct es8311_priv, codec);
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

static rt_err_t es_pa_ctl(bool on)
{
    if (on)
    {
        HAL_GPIO_SetPinLevel(ES8311_PA_GPIO, ES8311_PA_PIN, GPIO_HIGH);
        rt_thread_mdelay(PA_CTL_DELAY_MS);
    }
    else
    {
        HAL_GPIO_SetPinLevel(ES8311_PA_GPIO, ES8311_PA_PIN, GPIO_LOW);
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

static rt_err_t es8311_reg_init(struct es8311_priv *es8311)
{
    rt_err_t ret = RT_EOK;

    /* reset codec */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_RESET_REG00, 0x1F);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_GP_REG45, 0x00);

    rt_thread_mdelay(10);

    es_wr_reg(es8311->i2c_client, ES8311_GPIO_REG44, 0x08);
    rt_thread_mdelay(1);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_GPIO_REG44, 0x08);
    /* set ADC/DAC CLK */
    /* MCLK from BCLK  */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG01, 0xB0);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG02, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG02, 0x10);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG03, 0x10);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG16, 0x24);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG04, 0x10);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG05, 0x00);

    /* set system power up */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0B, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0C, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG10, 0x1F);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG11, 0x7F);
    /* chip powerup. slave mode */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_RESET_REG00, 0x80);
    rt_thread_mdelay(50);

    /* power up analog */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0D, 0x01);
    /* power up digital */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG01, 0xBF);

    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG14, 0x10);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG12, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG13, 0x10);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SDPIN_REG09, 0x0c);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SDPOUT_REG0A, 0x0c);

    /* set normal power mode */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0E, 0x02);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0F, 0x44);

    /* set adc softramp */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG15, 0x40);
    /* set adc hpf */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG1B, 0x0A);
    /* set adc hpf,ADC_EQ bypass */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG1C, 0x6A);
    /* set adc digtal vol */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG17, 0xBF);

    /* set dac softramp,disable DAC_EQ */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_DAC_REG37, 0x18);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_DAC_REG32, 0xB0);

    /* set adc gain scale up */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG16, 0x24);
    /* set adc alc maxgain */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG17, 0xBF);
    /* adc alc disable,alc_winsize */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG18, 0x07);
    /* set alc target level */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG19, 0xFB);
    /* set adc_automute noise gate */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG1A, 0x03);
    /* set adc_automute vol */
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG1B, 0xEA);
    return ret;
}

static rt_err_t es8311_codec_standby(struct es8311_priv *es8311)
{
    rt_err_t ret = RT_EOK;

    ret |= es_wr_reg(es8311->i2c_client, ES8311_DAC_REG32, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_ADC_REG17, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0E, 0xFF);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG12, 0x02);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG14, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0D, 0xFA);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_RESET_REG00, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_RESET_REG00, 0x1F);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG01, 0x30);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_CLK_MANAGER_REG01, 0x00);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_GP_REG45, 0x01);
    ret |= es_wr_reg(es8311->i2c_client, ES8311_SYSTEM_REG0D, 0xFC);

    return ret;
}

static rt_err_t es8311_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    return RT_EOK;
}

static rt_err_t es8311_deinit(struct audio_codec *codec)
{
    return RT_EOK;
}

static rt_err_t es8311_set_clk(struct audio_codec *codec, eAUDIO_streamType stream, uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t es8311_config(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params)
{
    struct es8311_priv *es8311 = to_es8311_priv(codec);
    int wl;
    rt_err_t ret = RT_EOK;

    /* Vendor suggest  that after bclk output and init codec */
    if (es8311->work_cnt <= 0)
        ret |= es8311_reg_init(es8311);

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

    ret |= es_update_bits(es8311->i2c_client, ES8311_SDPIN_REG09,
                          ES8311_SDPIN_REG09_DACWL_MASK,
                          wl << ES8311_SDPIN_REG09_DACWL_SHIFT);

    ret |= es_update_bits(es8311->i2c_client, ES8311_SDPOUT_REG0A,
                          ES8311_SDPOUT_REG0A_ADCWL_MASK,
                          wl << ES8311_SDPOUT_REG0A_ADCWL_SHIFT);

    if (stream == AUDIO_STREAM_PLAYBACK)
        es_pa_ctl(1);

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es8311_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct es8311_priv *es8311 = to_es8311_priv(codec);
    rt_mutex_take(&lock, RT_WAITING_FOREVER);
    es8311->work_cnt++;
    rt_mutex_release(&lock);
    return RT_EOK;
}

static rt_err_t es8311_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct es8311_priv *es8311 = to_es8311_priv(codec);
    rt_err_t ret = RT_EOK;

    rt_mutex_take(&lock, RT_WAITING_FOREVER);
    if (es8311->work_cnt)
        es8311->work_cnt--;
    rt_mutex_release(&lock);
    if (es8311->work_cnt <= 0)
        es8311_codec_standby(es8311);
    if (stream == AUDIO_STREAM_PLAYBACK)
        es_pa_ctl(0);


    return ret;
}

static const struct audio_codec_ops es8311_ops =
{
    .init = es8311_init,
    .deinit = es8311_deinit,
    .set_clk = es8311_set_clk,
    .config = es8311_config,
    .start = es8311_start,
    .stop = es8311_stop,
};

static int misc_prepare(void)
{
    HAL_PINCTRL_SetIOMUX(ES8311_PA_GPIO_BANK, ES8311_PA_PIN, ES8311_PA_PIN_FUNC_GPIO);
    HAL_GPIO_SetPinDirection(ES8311_PA_GPIO, ES8311_PA_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(ES8311_PA_GPIO, ES8311_PA_PIN, GPIO_LOW);

    return RT_EOK;
}

int rt_hw_codec_es8311_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_es8311;
    struct es8311_priv *es8311;
    rt_err_t ret = RT_EOK;
    uint8_t chip_id1;
    uint8_t chip_id2;

    misc_prepare();
    es_pa_ctl(0);

    es8311 = rt_calloc(1, sizeof(struct es8311_priv));
    RT_ASSERT(es8311);
    es8311->i2c_client = rt_calloc(1, sizeof(struct rt_i2c_client));
    RT_ASSERT(es8311->i2c_client);
    rt_mutex_init(&lock, "lock", RT_IPC_FLAG_FIFO);

    es8311->i2c_client->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
    RT_ASSERT(es8311->i2c_client->bus);
    es8311->i2c_client->client_addr = codec_dev->i2c_addr;
    es8311->codec.ops = &es8311_ops;
    es8311->codec.id = (uint32_t)codec_dev;
    es8311->work_cnt = 0;

    ret |= es_rd_reg(es8311->i2c_client, ES8311_CHD1_REGFD, &chip_id1);
    ret |= es_rd_reg(es8311->i2c_client, ES8311_CHD2_REGFE, &chip_id2);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, read codec id failed: %d\n", __func__, ret);
        goto err;
    }
    if (chip_id1 != 0x83 && chip_id2 != 0x11)
    {
        rt_kprintf("ERR: %s, ID[%0x%0x]not es8311\n", __func__, chip_id1, chip_id2);
        goto err;
    }
    ret |= rk_audio_register_codec(&es8311->codec);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    rt_kprintf("TAG: register codec es8311 success\n");
    return RT_EOK;

err:
    rt_kprintf("ERR: %s, register codec es8311 failed: %d\n", __func__, ret);
    if (es8311)
        rt_free(es8311);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_es8311_init);

#endif
