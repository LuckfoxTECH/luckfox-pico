/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    es8388.c
  * @author  Xing Zheng
  * @version v0.1
  * @date    2019.07.01
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CODEC_ES8388

#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "es8388.h"

#define ARRAY_SIZE(ar) (sizeof(ar) / sizeof(ar[0]))

/* Vendor suggest the startup delay should >130ms */
#define PA_CTL_DELAY_MS        150
#define ES8388_DB_MIN          (-96000)
#define ES8388_DB_MAX          (0)
#define ES8388_DB_STEP         (500)

struct es8388_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client;
    uint32_t playback_fs;
    bool deemph;
};

static const struct
{
    int rate;
    unsigned int val;
} deemph_settings[] =
{
    { 0, ES8388_DACCONTROL6_DEEMPH_OFF },
    { 32000, ES8388_DACCONTROL6_DEEMPH_32k },
    { 44100, ES8388_DACCONTROL6_DEEMPH_44_1k },
    { 48000, ES8388_DACCONTROL6_DEEMPH_48k },
};

static inline struct es8388_priv *to_es8388_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct es8388_priv, codec);
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
        HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_B1, GPIO_HIGH);
        rt_thread_mdelay(PA_CTL_DELAY_MS);
    }
    else
    {
        HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_B1, GPIO_LOW);
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

static rt_err_t es8388_set_fmt(struct es8388_priv *es8388)
{
    uint8_t dac_mode = 0;
    uint8_t adc_mode = 0;
    rt_err_t ret = RT_EOK;

    /* Slave serial port mode, we will support other formats in the future. */
    ret |= es_update_bits(es8388->i2c_client, ES8388_MASTERMODE,
                          ES8388_MASTERMODE_MSC, 0);

    /* Just using i2s format here. */
    dac_mode |= ES8388_DACCONTROL1_DACFORMAT_I2S;
    adc_mode |= ES8388_ADCCONTROL4_ADCFORMAT_I2S;

    ret |= es_update_bits(es8388->i2c_client, ES8388_DACCONTROL1,
                          ES8388_DACCONTROL1_DACFORMAT_MASK, dac_mode);
    ret |= es_update_bits(es8388->i2c_client, ES8388_ADCCONTROL4,
                          ES8388_ADCCONTROL4_ADCFORMAT_MASK, adc_mode);

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es8388_reg_init(struct es8388_priv *es8388)
{
    rt_err_t ret = RT_EOK;

    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL3, ES8388_DACCONTROL3_DACMUTE);
    /* Chip Control and Power Management */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_CONTROL2, 0x50);
    ret |= es_wr_reg(es8388->i2c_client, ES8388_CHIPPOWER, 0x00); // normal all and power up all

    /* DAC */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACPOWER, 0xc0);  // Disable DAC and disable Lout/Rout/1/2
    ret |= es_wr_reg(es8388->i2c_client, ES8388_CONTROL1, 0x12);  // Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL1, 0x18);  // 1a 0x18:16bit iis , 0x00:24
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL2, 0x02);  // DACFsMode,SINGLE SPEED; DACFsRatio,256
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL16, 0x00); // 0x00 audio on LIN1&RIN1, 0x09 LIN2&RIN2
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL17, 0x80); // only left DAC to left mixer enable 0db
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL20, 0x80); // only right DAC to right mixer enable 0db
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL21, 0x80); // set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL23, 0x00); // vroi=0
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACPOWER, 0x0c);  // Just enable DAC LOUT1 and ROUT1
    /* ADC */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCPOWER, 0xff);
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL1, 0xbb); // MIC Left and Right channel PGA gain
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL2, 0xf0); // 0xf0 are differential for LINSEL & RINSEL. 0x00 LINSEL & RINSEL, LIN1/RIN1 as ADC Input; DSSEL,use one DS Reg11; DSR, LINPUT1-RINPUT1
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL3, 0x02);
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL5, 0x02); //ADCFsMode,singel SPEED,RATIO=256
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCPOWER, 0x09); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode
    /* enable es8388 PA */

    /* set LADCVOL and LADCVOR 0dB */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL8, 0x00);
    ret |= es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL9, 0x00);

    /* set LDACVOL and LDACVOR 0dB */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL4, 0x00);
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL5, 0x00);
    /* set the LOUT2VOL and ROUT2VOL -24dB */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL26, 0x1e);
    ret |= es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL27, 0x1e);

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es8388_set_deemph(struct es8388_priv *es8388)
{
    uint8_t val;
    int i, best;

    /*
     * If we're using deemphasis select the nearest available sample
     * rate.
     */
    if (es8388->deemph)
    {
        best = 0;
        for (i = 1; i < ARRAY_SIZE(deemph_settings); i++)
        {
            if (abs(deemph_settings[i].rate - es8388->playback_fs) <
                    abs(deemph_settings[best].rate - es8388->playback_fs))
                best = i;
        }

        val = deemph_settings[best].val;
    }
    else
    {
        val = ES8388_DACCONTROL6_DEEMPH_OFF;
    }

    return es_update_bits(es8388->i2c_client, ES8388_DACCONTROL6,
                          ES8388_DACCONTROL6_DEEMPH_MASK, val);
}

static rt_err_t es8388_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    struct es8388_priv *es8388 = to_es8388_priv(codec);
    rt_err_t ret = RT_EOK;

    /* prepare process */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_CHIPPOWER, 0);
    ret |= es_update_bits(es8388->i2c_client, ES8388_CONTROL1,
                          ES8388_CONTROL1_VMIDSEL_MASK |
                          ES8388_CONTROL1_ENREF,
                          ES8388_CONTROL1_VMIDSEL_50k |
                          ES8388_CONTROL1_ENREF);

    /* If REF is off, we need to restart it. */
    ret |= es_update_bits(es8388->i2c_client, ES8388_CONTROL1,
                          ES8388_CONTROL1_VMIDSEL_MASK |
                          ES8388_CONTROL1_ENREF,
                          ES8388_CONTROL1_VMIDSEL_5k |
                          ES8388_CONTROL1_ENREF);

    /* Charge caps */
    rt_thread_mdelay(100);

    /* start process */
    ret |= es_wr_reg(es8388->i2c_client, ES8388_CONTROL2,
                     ES8388_CONTROL2_OVERCURRENT_ON |
                     ES8388_CONTROL2_THERMAL_SHUTDOWN_ON);

    /* VREF, VMID=2*500k, digital stopped */
    ret |= es_update_bits(es8388->i2c_client, ES8388_CONTROL1,
                          ES8388_CONTROL1_VMIDSEL_MASK |
                          ES8388_CONTROL1_ENREF,
                          ES8388_CONTROL1_VMIDSEL_500k |
                          ES8388_CONTROL1_ENREF);

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es8388_deinit(struct audio_codec *codec)
{
    return RT_EOK;
}

static rt_err_t es8388_set_clk(struct audio_codec *codec, eAUDIO_streamType stream, uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t es8388_config(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params)
{
    struct es8388_priv *es8388 = to_es8388_priv(codec);
    int ratio = 0, mclkdiv2 = 0;  /* used for slave mode */
    int wl, reg;
    rt_err_t ret = RT_EOK;

    if (stream == AUDIO_STREAM_PLAYBACK)
        reg = ES8388_DACCONTROL2;
    else
        reg = ES8388_ADCCONTROL5;

    ret |= es_update_bits(es8388->i2c_client, ES8388_MASTERMODE,
                          ES8388_MASTERMODE_MCLKDIV2,
                          mclkdiv2 ? ES8388_MASTERMODE_MCLKDIV2 : 0);

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

    if (stream == AUDIO_STREAM_PLAYBACK)
    {
        ret |= es_update_bits(es8388->i2c_client, ES8388_DACCONTROL1,
                              ES8388_DACCONTROL1_DACWL_MASK,
                              wl << ES8388_DACCONTROL1_DACWL_SHIFT);

        es8388->playback_fs = params->sampleRate;
        ret |= es8388_set_deemph(es8388);
    }
    else
    {
        ret |= es_update_bits(es8388->i2c_client, ES8388_ADCCONTROL4,
                              ES8388_ADCCONTROL4_ADCWL_MASK,
                              wl << ES8388_ADCCONTROL4_ADCWL_SHIFT);
    }

    ratio = 2;  /* FIXME: force 256FS for mclk */
    ret |= es_update_bits(es8388->i2c_client, reg, ES8388_RATEMASK, ratio);

    /*
     * The es8388_start() is before es8388_config(), therefore,
     * put the unmute here.
     */
    ret |= es_update_bits(es8388->i2c_client, ES8388_DACCONTROL3,
                          ES8388_DACCONTROL3_DACMUTE, 0);
    es_pa_ctl(1);

    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t es8388_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    return RT_EOK;
}

static rt_err_t es8388_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct es8388_priv *es8388 = to_es8388_priv(codec);
    rt_err_t ret = RT_EOK;

    ret |= es_update_bits(es8388->i2c_client, ES8388_DACCONTROL3,
                          ES8388_DACCONTROL3_DACMUTE, ES8388_DACCONTROL3_DACMUTE);

    ret |= es_update_bits(es8388->i2c_client, ES8388_CONTROL1,
                          ES8388_CONTROL1_VMIDSEL_MASK |
                          ES8388_CONTROL1_ENREF,
                          0);
    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    es_pa_ctl(0);

    return ret;
}

static rt_err_t es8388_set_gain(struct audio_codec *codec,
                                eAUDIO_streamType stream,
                                struct AUDIO_DB_CONFIG *db_config)
{
    struct es8388_priv *es8388 = to_es8388_priv(codec);
    int db = db_config->dB;

    if (stream == AUDIO_STREAM_PLAYBACK)
    {
        if (db > 0)
            return RT_EOK;

        db = -(db / ES8388_DB_STEP);
        es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL4, db);
        es_wr_reg(es8388->i2c_client, ES8388_DACCONTROL5, db);
    }
    else
    {
        if (db > 0)
            return RT_EOK;

        db = -(db / ES8388_DB_STEP);
        es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL8, db);
        es_wr_reg(es8388->i2c_client, ES8388_ADCCONTROL9, db);
    }

    return RT_EOK;
}

static rt_err_t es8388_get_gain(struct audio_codec *codec,
                                eAUDIO_streamType stream,
                                struct AUDIO_DB_CONFIG *db_config)
{
    struct es8388_priv *es8388 = to_es8388_priv(codec);
    uint8_t vol;

    if (stream == AUDIO_STREAM_PLAYBACK)
        es_rd_reg(es8388->i2c_client, ES8388_DACCONTROL4, &vol);
    else
        es_rd_reg(es8388->i2c_client, ES8388_ADCCONTROL8, &vol);

    db_config->dB = -(vol * ES8388_DB_STEP);

    return RT_EOK;
}

static rt_err_t es8388_get_gaininfo(struct audio_codec *codec,
                                    struct AUDIO_GAIN_INFO *info)
{
    if (!info)
        return -RT_EINVAL;

    info->mindB = ES8388_DB_MIN;
    info->maxdB = ES8388_DB_MAX;
    info->step = ES8388_DB_STEP;

    return RT_EOK;
}

static const struct audio_codec_ops es8388_ops =
{
    .init = es8388_init,
    .deinit = es8388_deinit,
    .set_clk = es8388_set_clk,
    .config = es8388_config,
    .start = es8388_start,
    .stop = es8388_stop,
    .set_gain = es8388_set_gain,
    .get_gain = es8388_get_gain,
    .get_gaininfo = es8388_get_gaininfo,
};

static int misc_prepare(void)
{
    /* set SPK_MUTE(GPIO0_B1) to high */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0, GPIO_PIN_B1, PIN_CONFIG_MUX_FUNC0);
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_B1, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_B1, GPIO_LOW);

    /* set iomux to sda and scl for rk2108 i2c0 from GPIO0D3 and GPIO0D4 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0, GPIO_PIN_D3, PIN_CONFIG_MUX_FUNC2);
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0, GPIO_PIN_D4, PIN_CONFIG_MUX_FUNC2);
    /* Using i2c0 mux0 via GPIO0D3 GPIO0D4*/
    WRITE_REG(GRF->SOC_CON5, 0x00030001);

    return RT_EOK;
}

int rt_hw_codec_es8388_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_es8388;
    struct es8388_priv *es8388;
    rt_err_t ret;

    misc_prepare();

    es8388 = rt_calloc(1, sizeof(struct es8388_priv));
    RT_ASSERT(es8388);
    es8388->i2c_client = rt_calloc(1, sizeof(struct rt_i2c_client));
    RT_ASSERT(es8388->i2c_client);

    es8388->i2c_client->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
    RT_ASSERT(es8388->i2c_client->bus);
    es8388->i2c_client->client_addr = codec_dev->i2c_addr;
    es8388->codec.ops = &es8388_ops;
    es8388->codec.id = (uint32_t)codec_dev;

    /* Reset codec, meanwhile to check the connection of i2c bus first. */
    ret = es_wr_reg(es8388->i2c_client, ES8388_CONTROL1, 0);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, reset codec failed: %d\n", __func__, ret);
        goto err;
    }
    rt_thread_mdelay(20);

    ret |= es8388_reg_init(es8388);
    ret |= es8388_set_fmt(es8388);
    ret |= rk_audio_register_codec(&es8388->codec);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    rt_kprintf("TAG: register codec es8388 success\n");
    return RT_EOK;

err:
    rt_kprintf("ERR: %s, register codec es8388 failed: %d\n", __func__, ret);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_es8388_init);

#endif
