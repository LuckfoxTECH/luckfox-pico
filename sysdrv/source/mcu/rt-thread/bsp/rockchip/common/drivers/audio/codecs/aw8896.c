/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    aw8896.c
  * @author  Xing Zheng
  * @version v0.1
  * @date    2020.11.19
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  * Based on
  */
/*
 * aw8896.c   aw8896 codec module
 *
 * Version: v1.0.7
 *
 * Copyright (c) 2017 AWINIC Technology CO., LTD
 *
 * Author: Nick Li <liweilei@awinic.com.cn>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CODEC_AW8896

#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "aw8896_bin.h"
#include "aw8896_reg.h"
#include "aw8896.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(ar)              (sizeof(ar)/sizeof(ar[0]))
#endif

#define AW_DBG_EN               0

#if AW_DBG_EN
#define AW_DBG                  rt_kprintf
#else
#define AW_DBG(...)
#endif

#define AW_LOG                  rt_kprintf
#define AW_ERR                  rt_kprintf

#define AW_I2C_RETRIES              5
#define AW_I2C_RETRY_DELAY          5  /* 5ms */
#define AW_READ_CHIPID_RETRIES          5
#define AW_READ_CHIPID_RETRY_DELAY      5
#define AW8896_MAX_DSP_START_TRY_COUNT      10
#define DT_MAX_PROP_SIZE            80

/**
  * The range of gains.
  * To avoid using decimals, the scaled 10 times dB.
  * min: -100dB, max: 0dB, step: 0.5dB.
  */
#define AW8896_DB_MIN  (-1000)
#define AW8896_DB_MAX  (0)
#define AW8896_DB_STEP (-5)

struct aw8896_container
{
    int len;
    unsigned char *data;
};

struct aw8896_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client;

    /* AW8896 Status */
    unsigned int flags;
    unsigned int chipid;
    unsigned int init;
    unsigned int spk_rcv_mode;
    unsigned short vol;

    /* AW8896 DSP */
    int dsp_init;
    int dsp_fw_state;
    int dsp_cfg_state;
    int dsp_fw_len;
    int dsp_cfg_len;
    int dsp_fw_ver;
    int rate;
    int bits;
    int pstream;
    int cstream;
    bool agc_on;
};

rt_inline struct aw8896_priv *to_aw8896_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct aw8896_priv, codec);
}

static rt_err_t aw_rd_reg(struct rt_i2c_client *i2c_client,
                          unsigned char reg8, unsigned short *data16)
{
    struct rt_i2c_msg msgs[2];
    unsigned char data8;
    rt_err_t ret;

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg8;
    msgs[0].len = sizeof(reg8);

    msgs[1].addr = i2c_client->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = (void *)data16;
    msgs[1].len = sizeof(*data16);

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 2);
    if (ret != 2)
    {
        AW_ERR("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    /* Sequence: READ DATA_H, READ DATA_L, Swap the bytes order */
    data8 = (*data16) & 0xff;
    (*data16) = ((*data16) >> 8) | (data8 << 8);

    return RT_EOK;
}

static rt_err_t aw_wr_reg(struct rt_i2c_client *i2c_client,
                          unsigned char reg8, unsigned short data16)
{
    struct rt_i2c_msg msgs[1];
    unsigned char data_buf[3];
    rt_err_t ret;

    /* Sequence: WRITE DATA_H, WRITE DATA_L */
    data_buf[0] = reg8;
    data_buf[1] = (data16 >> 8);
    data_buf[2] = (data16 & 0xff);

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = data_buf;
    msgs[0].len = sizeof(reg8) + sizeof(data16);

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 1);
    if (ret != 1)
    {
        AW_ERR("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    return RT_EOK;
}

static rt_err_t aw_wr_regs(struct rt_i2c_client *i2c_client,
                           unsigned char reg8, unsigned short *data, unsigned int len)
{
    struct rt_i2c_msg msgs[1];
    unsigned char *data_buf;
    rt_err_t ret;

    data_buf = rt_calloc(1, sizeof(reg8) + len);
    RT_ASSERT(data_buf);
    data_buf[0] = reg8;
    rt_memcpy(&data_buf[1], (void *)data, len);

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = data_buf;
    msgs[0].len = sizeof(reg8) + len;
    ret = rt_i2c_transfer(i2c_client->bus, msgs, 1);
    if (ret != 1)
    {
        AW_ERR("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    rt_free(data_buf);

    return RT_EOK;
}

static rt_err_t aw_update_bits(struct rt_i2c_client *i2c_client,
                               unsigned char reg, unsigned short mask,
                               unsigned short val)
{
    unsigned short old, new;
    int ret;

    ret = aw_rd_reg(i2c_client, reg, &old);
    if (ret < 0)
        goto err;

    new = (old & ~mask) | (val & mask);
    ret = aw_wr_reg(i2c_client, reg, new);

err:
    return ret;
}

static void aw8896_run_mute(struct aw8896_priv *aw8896, bool mute)
{
    unsigned short reg_val = 0;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_PWMCTRL, &reg_val);
    if (mute)
        reg_val |= AW8896_BIT_PWMCTRL_HMUTE;
    else
        reg_val &= (~AW8896_BIT_PWMCTRL_HMUTE);

    aw_wr_reg(aw8896->i2c_client, AW8896_REG_PWMCTRL, reg_val);
}

static void aw8896_run_pwd(struct aw8896_priv *aw8896, bool pwd)
{
    unsigned short reg_val = 0;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, &reg_val);
    if (pwd)
        reg_val |= AW8896_BIT_SYSCTRL_PWDN;
    else
        reg_val &= (~AW8896_BIT_SYSCTRL_PWDN);

    aw_wr_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, reg_val);
}

static void aw8896_dsp_enable(struct aw8896_priv *aw8896, bool dsp)
{
    unsigned short reg_val = 0;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, &reg_val);
    if (dsp)
        reg_val &= (~AW8896_BIT_SYSCTRL_DSPBY);
    else
        reg_val |= AW8896_BIT_SYSCTRL_DSPBY;

    aw_wr_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, reg_val);
}

static int aw8896_get_iis_status(struct aw8896_priv *aw8896)
{
    unsigned short reg_val = 0;
    int ret = -1;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSST, &reg_val);
    if (reg_val & AW8896_BIT_SYSST_PLLS)
        ret = 0;

    return ret;
}

static int aw8896_get_dsp_status(struct aw8896_priv *aw8896)
{
    unsigned short reg_val = 0;
    int ret = -1;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_WDT, &reg_val);
    if (reg_val)
        ret = 0;

    return ret;
}

static void aw8896_spk_rcv_mode(struct aw8896_priv *aw8896)
{
    unsigned short reg_val = 0;

    if (aw8896->spk_rcv_mode == AW8896_SPEAKER_MODE)
    {
        /* RFB IDAC = 6V */
        aw_rd_reg(aw8896->i2c_client, AW8896_REG_AMPDBG1, &reg_val);
        reg_val |= AW8896_BIT_AMPDBG1_OPD;
        reg_val &= (~AW8896_BIT_AMPDBG1_IPWM_20UA);
        reg_val &= (~AW8896_BIT_AMPDBG1_RFB_MASK);
        reg_val |= 0x0002;
        aw_wr_reg(aw8896->i2c_client, AW8896_REG_AMPDBG1, reg_val);

        /* Speaker Mode */
        aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, &reg_val);
        reg_val &= (~AW8896_BIT_SYSCTRL_RCV_MODE);
        aw_wr_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, reg_val);

    }
    else if (aw8896->spk_rcv_mode == AW8896_RECEIVER_MODE)
    {
        /* RFB IDAC = 4V */
        aw_rd_reg(aw8896->i2c_client, AW8896_REG_AMPDBG1, &reg_val);
        reg_val |= AW8896_BIT_AMPDBG1_OPD;
        reg_val |= (AW8896_BIT_AMPDBG1_IPWM_20UA);
        reg_val &= (~AW8896_BIT_AMPDBG1_RFB_MASK);
        reg_val |= 0x000B;
        aw_wr_reg(aw8896->i2c_client, AW8896_REG_AMPDBG1, reg_val);

        /* Receiver Mode */
        aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, &reg_val);
        reg_val |= AW8896_BIT_SYSCTRL_RCV_MODE;
        aw_wr_reg(aw8896->i2c_client, AW8896_REG_SYSCTRL, reg_val);
    }
}

static void aw8896_tx_cfg(struct aw8896_priv *aw8896)
{
    unsigned short reg_val = 0;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_I2STXCFG, &reg_val);
    reg_val |= AW8896_BIT_I2STXCFG_TXEN;
    aw_wr_reg(aw8896->i2c_client, AW8896_REG_I2STXCFG, reg_val);

    if (aw8896->dsp_fw_ver == AW8896_DSP_FW_VER_D)
    {
        aw_rd_reg(aw8896->i2c_client, AW8896_REG_DBGCTRL, &reg_val);
        reg_val |= AW8896_BIT_DBGCTRL_LPBK_NEARE;
        aw_wr_reg(aw8896->i2c_client, AW8896_REG_DBGCTRL, reg_val);
    }
}

static void aw8896_start(struct aw8896_priv *aw8896)
{
    aw8896_run_pwd(aw8896, RT_FALSE);
    aw8896_run_mute(aw8896, RT_FALSE);
}

static void aw8896_stop(struct aw8896_priv *aw8896)
{
    aw8896_run_mute(aw8896, RT_TRUE);
    aw8896_run_pwd(aw8896, RT_TRUE);
}

static void aw8896_interrupt_setup(struct aw8896_priv *aw8896)
{
    unsigned short reg_val = 0;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSINTM, &reg_val);

    /* i2s status interrupt unmask */
    reg_val &= (~AW8896_BIT_SYSINTM_PLLM);
    reg_val &= (~AW8896_BIT_SYSINTM_CLKM);
    reg_val &= (~AW8896_BIT_SYSINTM_NOCLKM);

    /*  uvlo interrupt unmask */
    reg_val &= (~AW8896_BIT_SYSINTM_UVLOM);

    /* over temperature interrupt unmask */
    reg_val &= (~AW8896_BIT_SYSINTM_OTHM);

    /* dsp watchdog status interrupt unmask */
    reg_val &= (~AW8896_BIT_SYSINTM_WDM);

    aw_wr_reg(aw8896->i2c_client, AW8896_REG_SYSINTM, reg_val);
}

static void aw8896_interrupt_clear(struct aw8896_priv *aw8896)
{
    unsigned short reg_val;

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSST, &reg_val);
    AW_DBG("DBG: %s: reg SYSST=0x%x\n", __func__, reg_val);

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSINT, &reg_val);
    AW_DBG("DBG: %s: reg SYSINT=0x%x\n", __func__, reg_val);

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_SYSINTM, &reg_val);
    AW_DBG("DBG: %s: reg SYSINTM=0x%x\n", __func__, reg_val);
}

static void aw8896_dsp_container_update(struct aw8896_priv *aw8896,
                                        struct aw8896_container *aw8896_cont,
                                        int base)
{
    unsigned char tmp_val = 0;
    unsigned int tmp_len = 0;
    int i = 0;

    aw_wr_reg(aw8896->i2c_client, AW8896_REG_DSPMADD, base);
    for (i = 0; i < aw8896_cont->len; i += 2)
    {
        tmp_val = aw8896_cont->data[i + 0];
        aw8896_cont->data[i + 0] = aw8896_cont->data[i + 1];
        aw8896_cont->data[i + 1] = tmp_val;
    }

    for (i = 0; i < aw8896_cont->len; i += MAX_RAM_WRITE_BYTE_SIZE)
    {
        if ((aw8896_cont->len - i) < MAX_RAM_WRITE_BYTE_SIZE)
            tmp_len = aw8896_cont->len - i;
        else
            tmp_len = MAX_RAM_WRITE_BYTE_SIZE;

        aw_wr_regs(aw8896->i2c_client, AW8896_REG_DSPMDAT,
                   (unsigned short *)&aw8896_cont->data[i], tmp_len);
    }
}

static int aw8896_cfg_loaded(struct aw8896_priv *aw8896)
{
    const char *bin_cfg;
    struct aw8896_container aw8896_cfg;
    int ret = -1;

    if (!aw8896)
    {
        AW_ERR("ERR: %s: aw8896 is null\n", __func__);
        return -EINVAL;
    }
    aw8896->dsp_cfg_state = AW8896_DSP_CFG_FAIL;

    if (aw8896->agc_on)
        bin_cfg = aw8896_bin_cfg;
    else
        bin_cfg = aw8896_bin_cfg_agc_off;

    aw8896_cfg.len = ARRAY_SIZE(bin_cfg);
    aw8896_cfg.data = rt_calloc(1, aw8896_cfg.len);
    RT_ASSERT(aw8896_cfg.data);
    rt_memcpy(aw8896_cfg.data, bin_cfg, aw8896_cfg.len);

    aw8896_dsp_container_update(aw8896, &aw8896_cfg, AW8896_DSP_CFG_BASE);

    aw8896->dsp_cfg_len = aw8896_cfg.len;

    rt_free(aw8896_cfg.data);
    AW_DBG("DBG: %s: cfg update complete\n", __func__);

    aw8896_dsp_enable(aw8896, RT_TRUE);

    /* delay 1ms wait for dsp start run */
    rt_thread_mdelay(1);

    ret = aw8896_get_dsp_status(aw8896);
    if (ret)
    {
        aw8896_dsp_enable(aw8896, RT_FALSE);
        aw8896_run_mute(aw8896, RT_TRUE);
        AW_ERR("ERR: %s: dsp working wdt, dsp fw&cfg update failed\n",
               __func__);
    }
    else
    {
        aw8896_tx_cfg(aw8896);
        aw8896_spk_rcv_mode(aw8896);
        aw8896_start(aw8896);
        if (!(aw8896->flags & AW8896_FLAG_SKIP_INTERRUPTS))
        {
            aw8896_interrupt_clear(aw8896);
            aw8896_interrupt_setup(aw8896);
        }
        aw8896->init = 1;
        aw8896->dsp_cfg_state = AW8896_DSP_CFG_OK;
        aw8896->dsp_fw_state = AW8896_DSP_FW_OK;
        AW_DBG("DBG: %s: init ok\n", __func__);
    }

    return 0;
}

static int aw8896_load_cfg(struct aw8896_priv *aw8896)
{
    if (aw8896->dsp_cfg_state == AW8896_DSP_CFG_OK)
    {
        AW_DBG("DBG: %s: dsp cfg ok\n", __func__);
        return 0;
    }

    return aw8896_cfg_loaded(aw8896);
}

static int aw8896_fw_loaded(struct aw8896_priv *aw8896)
{
    struct aw8896_container aw8896_fw;
    int ret = -1;

    if (!aw8896)
    {
        AW_ERR("ERR: %s: aw8896 is null\n", __func__);
        return -1;
    }

    aw8896->dsp_fw_state = AW8896_DSP_FW_FAIL;

    aw8896_fw.len = ARRAY_SIZE(aw8896_bin_fw);
    aw8896_fw.data = rt_calloc(1, aw8896_fw.len);
    RT_ASSERT(aw8896_fw.data);
    rt_memcpy(aw8896_fw.data, aw8896_bin_fw, aw8896_fw.len);

    aw8896_dsp_container_update(aw8896, &aw8896_fw, AW8896_DSP_FW_BASE);

    aw8896->dsp_fw_len = aw8896_fw.len;

    rt_free(aw8896_fw.data);
    AW_DBG("DBG: %s: fw update complete\n", __func__);

    ret = aw8896_load_cfg(aw8896);
    if (ret)
    {
        AW_ERR("ERR: %s: cfg loading requested failed: %d\n", __func__, ret);
        return ret;
    }

    return 0;
}

static int aw8896_load_fw(struct aw8896_priv *aw8896)
{
    unsigned int reg_val = 0;
    unsigned int tmp_val = 0;

    if (!aw8896)
    {
        AW_ERR("ERR: %s: aw8896 is null\n", __func__);
        return -EINVAL;
    }

    if (aw8896->dsp_fw_state == AW8896_DSP_FW_OK)
    {
        AW_DBG("DBG: %s: dsp fw ok\n", __func__);
        return 0;
    }

    aw8896->dsp_fw_state = AW8896_DSP_FW_PENDING;

    aw_wr_reg(aw8896->i2c_client, AW8896_REG_DSPMADD, AW8896_DSP_FW_VER_BASE);
    aw_rd_reg(aw8896->i2c_client, AW8896_REG_DSPMDAT, (unsigned short *)&reg_val);
    tmp_val |= reg_val;
    aw_wr_reg(aw8896->i2c_client, AW8896_REG_DSPMADD,
              AW8896_DSP_FW_VER_BASE + 1);
    aw_rd_reg(aw8896->i2c_client, AW8896_REG_DSPMDAT, (unsigned short *)&reg_val);
    tmp_val |= (reg_val << 16);
    if (tmp_val == 0xdeac97d6)
    {
        aw8896->dsp_fw_ver = AW8896_DSP_FW_VER_E;
        AW_DBG("DBG: %s: dsp fw read version: AW8896_DSP_VER_FW_E: 0x%x\n",
               __func__, tmp_val);
    }
    else if (tmp_val == 0)
    {
        aw8896->dsp_fw_ver = AW8896_DSP_FW_VER_D;
        AW_DBG("DBG: %s: dsp fw read version: AW8896_DSP_FW_VER_D: 0x%x\n",
               __func__, tmp_val);
    }
    else
    {
        AW_ERR("ERR: %s: dsp fw read version err: 0x%x\n", __func__,
               tmp_val);
        return -EINVAL;
    }

    aw8896_run_mute(aw8896, RT_TRUE);
    aw8896_dsp_enable(aw8896, RT_FALSE);

    return aw8896_fw_loaded(aw8896);
}

static void aw8896_reg_container_update(struct aw8896_priv *aw8896,
                                        struct aw8896_container *aw8896_cont)
{
    int reg_addr = 0;
    int reg_val = 0;
    int i = 0;

    for (i = 0; i < aw8896_cont->len; i += 4)
    {
        reg_addr = (aw8896_cont->data[i + 1] << 8)
                   + aw8896_cont->data[i + 0];
        reg_val = (aw8896_cont->data[i + 3] << 8)
                  + aw8896_cont->data[i + 2];
        AW_DBG("DBG: %s: reg=0x%04x, val = 0x%04x\n",
               __func__, reg_addr, reg_val);
        aw_wr_reg(aw8896->i2c_client, (unsigned char)reg_addr,
                  (unsigned int)reg_val);
    }
}

static rt_err_t aw8896_hw_param(struct aw8896_priv *aw8896,
                                struct aw8896_container *aw8896_reg)
{
    unsigned short reg_rate, reg_bits, reg_val;

    if (!aw8896)
    {
        AW_ERR("ERR: %s: aw8896 is null\n", __func__);
        return -EINVAL;
    }

    if (!aw8896_reg)
    {
        AW_ERR("ERR: %s: aw8896_reg is null\n", __func__);
        return -EINVAL;
    }

    /* The data[2] is the low 8-bit of aw8896_reg->data[2] I2SCTRL */
    reg_val = aw8896_reg->data[2];
    /* Clear I2SSR and I2SFS */
    reg_val = (reg_val & (~AW8896_BIT_I2SCTRL_SR_MASK));
    reg_val = (reg_val & (~AW8896_BIT_I2SCTRL_FMS_MASK));

    /* get rate param */
    switch (aw8896->rate)
    {
    case 8000:
        reg_rate = AW8896_BIT_I2SCTRL_SR_8K;
        break;
    case 16000:
        reg_rate = AW8896_BIT_I2SCTRL_SR_16K;
        break;
    case 32000:
        reg_rate = AW8896_BIT_I2SCTRL_SR_32K;
        break;
    case 44100:
        reg_rate = AW8896_BIT_I2SCTRL_SR_44K;
        break;
    case 48000:
        reg_rate = AW8896_BIT_I2SCTRL_SR_48K;
        break;
    default:
        reg_rate = AW8896_BIT_I2SCTRL_SR_48K;
        AW_ERR("ERR: %s: rate can not support, set to default\n", __func__);
        break;
    }
    /* set chip rate */
    if (reg_rate != -1)
        reg_val = reg_val | reg_rate;

    /* get bits */
    switch (aw8896->bits)
    {
    case 16:
        reg_bits = AW8896_BIT_I2SCTRL_FMS_16BIT;
        break;
    case 20:
        reg_bits = AW8896_BIT_I2SCTRL_FMS_20BIT;
        break;
    case 24:
        reg_bits = AW8896_BIT_I2SCTRL_FMS_24BIT;
        break;
    case 32:
        reg_bits = AW8896_BIT_I2SCTRL_FMS_32BIT;
        break;
    default:
        reg_bits = AW8896_BIT_I2SCTRL_FMS_16BIT;
        break;
    }
    /* set bits */
    if (reg_bits != -1)
        reg_val = reg_val | reg_bits;

    /* Write back I2SCTRL */
    aw8896_reg->data[2] = reg_val;
    AW_DBG("DBG: %s: reg_val=0x%04x, data=0x%04x, rate=%d, bits=%d\n",
           __func__, reg_val, aw8896_reg->data[2], aw8896->rate, aw8896->bits);

    return 0;
}

static int aw8896_reg_loaded(struct aw8896_priv *aw8896)
{
    struct aw8896_container aw8896_reg;
    int ret = -1;

    if (!aw8896)
    {
        AW_ERR("ERR: %s: aw8896 is null\n", __func__);
        return -1;
    }

    aw8896_reg.len = ARRAY_SIZE(aw8896_bin_reg);
    aw8896_reg.data = rt_calloc(1, aw8896_reg.len);
    RT_ASSERT(aw8896_reg.data);
    rt_memcpy(aw8896_reg.data, aw8896_bin_reg, aw8896_reg.len);
    /**
     *  4 bytes(16 bits cmd 16 bits data) for one register
     *  Length align down to 4 for avoid overflow
     */
    for (int i = 0; i < RT_ALIGN_DOWN(aw8896_reg.len, 4); i += 4)
    {
        /* Find REG07 set default gain, bit 15:8 for gain value */
        if (aw8896_reg.data[i] == 0x07)
        {
            aw8896_reg.data[i + 3] = aw8896->vol;
            break;
        }
    }

    /* Update reg_bin according to different HW params */
    aw8896_hw_param(aw8896, &aw8896_reg);
    aw8896_reg_container_update(aw8896, &aw8896_reg);
    rt_free(aw8896_reg.data);
    ret = aw8896_get_iis_status(aw8896);
    if (ret < 0)
    {
        AW_ERR("ERR: %s: get no iis signal, ret=%d\n", __func__, ret);
    }
    else
    {
        ret = aw8896_load_fw(aw8896);
        if (ret)
            AW_ERR("ERR: %s: cfg loading requested failed: %d\n",
                   __func__, ret);
    }

    return 0;
}

static int aw8896_load_reg(struct aw8896_priv *aw8896)
{
    return aw8896_reg_loaded(aw8896);
}

static void aw8896_cold_start(struct aw8896_priv *aw8896)
{
    int ret = -1;

    ret = aw8896_load_reg(aw8896);
    if (ret)
    {
        AW_ERR("ERR: %s: cfg loading requested failed: %d\n", __func__, ret);
    }
}

static void aw8896_smartpa_cfg(struct aw8896_priv *aw8896, bool flag)
{
    if (flag == RT_TRUE)
    {
        if (aw8896->init == 0)
        {
            AW_DBG("DBG: %s, init = %d\n", __func__, aw8896->init);
            aw8896_cold_start(aw8896);
        }
        else
        {
            aw8896_spk_rcv_mode(aw8896);
            aw8896_start(aw8896);
        }
    }
    else
    {
        aw8896_stop(aw8896);
    }
}

static rt_err_t aw8896_reg_init(struct aw8896_priv *aw8896)
{
    rt_err_t ret = RT_EOK;

    AW_DBG("DBG: %s %d\n", __func__, __LINE__);
    return ret;
}

static int aw8896_codec_mute(struct audio_codec *codec, int mute, int stream)
{
    struct aw8896_priv *aw8896 = to_aw8896_priv(codec);

    if (!(aw8896->flags & AW8896_FLAG_DSP_START_ON_MUTE))
        return 0;

    if (mute)
    {
        /* stop DSP only when both playback and capture streams
         * are deactivated
         */
        if (stream == AUDIO_STREAM_PLAYBACK)
            aw8896->pstream = 0;
        else
            aw8896->cstream = 0;
        if (aw8896->pstream != 0 || aw8896->cstream != 0)
            return 0;

        /* Stop DSP */
        aw8896_smartpa_cfg(aw8896, RT_FALSE);
    }
    else
    {
        if (stream == AUDIO_STREAM_PLAYBACK)
            aw8896->pstream = 1;
        else
            aw8896->cstream = 1;

        /* Start DSP */
        aw8896_smartpa_cfg(aw8896, RT_TRUE);
    }

    return 0;
}

static rt_err_t aw8896_codec_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    return RT_EOK;
}

static rt_err_t aw8896_codec_deinit(struct audio_codec *codec)
{
    return RT_EOK;
}

static rt_err_t aw8896_codec_config(struct audio_codec *codec, eAUDIO_streamType stream,
                                    struct AUDIO_PARAMS *params)
{
    struct aw8896_priv *aw8896 = to_aw8896_priv(codec);

    aw8896->rate = params->sampleRate;
    aw8896->bits = params->sampleBits;
    return RT_EOK;
}

static rt_err_t aw8896_codec_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct aw8896_priv *aw8896 = to_aw8896_priv(codec);

    aw8896_run_pwd(aw8896, RT_FALSE);
    aw8896_codec_mute(codec, 0, stream);

    return RT_EOK;
}

static rt_err_t aw8896_codec_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct aw8896_priv *aw8896 = to_aw8896_priv(codec);

    aw8896_codec_mute(codec, 1, stream);
    aw8896_run_pwd(aw8896, RT_TRUE);

    return RT_EOK;
}


static rt_err_t aw8896_codec_set_gain(struct audio_codec *codec,
                                      eAUDIO_streamType stream,
                                      struct AUDIO_DB_CONFIG *db_config)
{
    struct aw8896_priv *aw8896 = to_aw8896_priv(codec);

    RT_ASSERT(db_config);

    aw8896->vol = db_config->dB / AW8896_DB_STEP;
    aw_update_bits(aw8896->i2c_client, AW8896_REG_DSPCFG, 0xff00, aw8896->vol << 8);

    return RT_EOK;
}

static rt_err_t aw8896_codec_get_gain(struct audio_codec *codec,
                                      eAUDIO_streamType stream,
                                      struct AUDIO_DB_CONFIG *db_config)
{
    struct aw8896_priv *aw8896 = to_aw8896_priv(codec);
    unsigned short val, volume;

    RT_ASSERT(db_config);

    aw_rd_reg(aw8896->i2c_client, AW8896_REG_DSPCFG, &val);
    volume = val >> 8;

    /* The min volume is -100dB, register value is 200. */
    if (volume > 200)
        volume = 200;

    db_config->dB = volume * AW8896_DB_STEP;

    return RT_EOK;
}

static rt_err_t aw8896_codec_get_gaininfo(struct audio_codec *codec,
        struct AUDIO_GAIN_INFO *info)
{
    RT_ASSERT(info);

    info->mindB = AW8896_DB_MIN;
    info->maxdB = AW8896_DB_MAX;
    info->step = AW8896_DB_STEP;

    return RT_EOK;
}

static const struct audio_codec_ops aw8896_ops =
{
    .init = aw8896_codec_init,
    .deinit = aw8896_codec_deinit,
    .config = aw8896_codec_config,
    .start = aw8896_codec_start,
    .stop = aw8896_codec_stop,
    .set_gain = aw8896_codec_set_gain,
    .get_gain = aw8896_codec_get_gain,
    .get_gaininfo = aw8896_codec_get_gaininfo,
};

static int aw8896_hw_reset(struct aw8896_priv *aw8896)
{
    HAL_GPIO_SetPinLevel(GPIO1, GPIO_PIN_B1, GPIO_HIGH);
    rt_thread_mdelay(1);
    HAL_GPIO_SetPinLevel(GPIO1, GPIO_PIN_B1, GPIO_LOW);
    rt_thread_mdelay(1);

    return 0;
}

static int aw8896_read_chipid(struct aw8896_priv *aw8896)
{
    unsigned int cnt = 0;
    unsigned short reg = 0;
    int ret = -1;

    while (cnt < AW_READ_CHIPID_RETRIES)
    {
        ret = aw_rd_reg(aw8896->i2c_client, AW8896_REG_ID, &reg);
        if (ret < 0)
        {
            AW_ERR("ERR: failed to read register AW8896_REG_ID: %d\n",
                   __func__, ret);
            return -EIO;
        }
        switch (reg)
        {
        case 0x0310:
            AW_DBG("DBG: %s aw8896 detected\n", __func__);
            aw8896->flags |= AW8896_FLAG_SKIP_INTERRUPTS;
            aw8896->flags |= AW8896_FLAG_DSP_START_ON_MUTE;
            aw8896->flags |= AW8896_FLAG_DSP_START_ON;
            aw8896->chipid = AW8990_ID;
            AW_DBG("DBG: %s aw8896->flags=0x%x\n", __func__,
                   aw8896->flags);
            return 0;
        default:
            AW_ERR("ERR: %s unsupported device revision (0x%x)\n",
                   __func__, reg);
            break;
        }
        cnt++;

        rt_thread_mdelay(AW_READ_CHIPID_RETRY_DELAY);
    }

    return -EINVAL;
}

static int aw8896_dump_regs(struct aw8896_priv *aw8896)
{
    unsigned char reg;
    unsigned short data16;

    for (reg = 0; reg < AW8896_REG_BSTDBG1; reg++)
    {
        aw_rd_reg(aw8896->i2c_client, reg, &data16);
        AW_DBG("DUMP: reg[0x%02x]: 0x%04x\n", reg, data16);
    }

    return 0;
}

int rt_hw_codec_aw8896_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_aw8896;
    struct aw8896_priv *aw8896;
    rt_err_t ret = RT_EOK;
    unsigned short reg = 0x0;

    aw8896 = rt_calloc(1, sizeof(struct aw8896_priv));
    RT_ASSERT(aw8896);
    aw8896->i2c_client = rt_calloc(1, sizeof(struct rt_i2c_client));
    RT_ASSERT(aw8896->i2c_client);

    aw8896->i2c_client->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
    RT_ASSERT(aw8896->i2c_client->bus);
    aw8896->i2c_client->client_addr = codec_dev->i2c_addr;
    aw8896->codec.ops = &aw8896_ops;
    aw8896->codec.id = (unsigned int)codec_dev;

    aw8896->agc_on = RT_FALSE;

    aw8896_hw_reset(aw8896);
    aw8896_read_chipid(aw8896);

    /* Read codec ID, meanwhile to check the connection of i2c bus first. */
    ret |= aw_rd_reg(aw8896->i2c_client, AW8896_REG_ID, &reg);
    if (ret != RT_EOK)
    {
        AW_ERR("ERR: %s, i2c read failed: %d\n", __func__, ret);
        goto err;
    }

    ret |= aw8896_reg_init(aw8896);
    ret |= rk_audio_register_codec(&aw8896->codec);
    if (ret != RT_EOK)
    {
        AW_ERR("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    AW_LOG("TAG: register codec aw8896 success\n");

    aw8896_dump_regs(aw8896);

    return RT_EOK;

err:
    AW_ERR("ERR: %s, register codec aw8896 failed: %d\n", __func__, ret);
    if (aw8896)
        rt_free(aw8896);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_aw8896_init);

#endif
