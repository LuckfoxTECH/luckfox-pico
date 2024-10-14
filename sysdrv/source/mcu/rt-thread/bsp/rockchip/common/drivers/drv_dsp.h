/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_dsp.h
  * @author  Huaping Liao
  * @version V0.1
  * @date    24-Jan-2019
  * @brief   dsp driver
  *
  ******************************************************************************
  */

#ifndef _DRV_DSP_H_
#define _DRV_DSP_H_

#include <rtthread.h>

#include <soc.h>

/* RKDSP control command */
#define RKDSP_CTL_QUEUE_WORK          0x11
#define RKDSP_CTL_DEQUEUE_WORK        0x12
#define RKDSP_CTL_GET_FREQ            0x13
#define RKDSP_CTL_SET_FREQ            0x14
#define RKDSP_CTL_SEND_VENDOR_KEY     0x15

#define DSP_CMD_ARM2DSP               0x01
#define DSP_CMD_DSP2ARM               0x02

#define DSP_AUTH_WAIT_FLAG            0x5A5A5A5A
#define DSP_AUTH_RESP_FLAG            0xA5A5A5A5

#define DSP_CMD_DEFINE(chan, dir, id)  \
        ((chan & 0xff) << 16 | (dir & 0xff) << 8 | (id & 0xff))

#define DSP_CMD_WORK        \
        DSP_CMD_DEFINE(MBOX_CH_0, DSP_CMD_ARM2DSP, 0x01)
#define DSP_CMD_WORK_DONE   \
        DSP_CMD_DEFINE(MBOX_CH_0, DSP_CMD_DSP2ARM, 0x01)

#define DSP_CMD_READY       \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_DSP2ARM, 0x01)
#define DSP_CMD_CONFIG      \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_ARM2DSP, 0x02)
#define DSP_CMD_CONFIG_DONE \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_DSP2ARM, 0x02)
#define DSP_CMD_SUSPEND      \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_ARM2DSP, 0x03)
#define DSP_CMD_SHUTDOWN      \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_ARM2DSP, 0x04)
#define DSP_CMD_SEND_VENDOR_KEY      \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_ARM2DSP, 0x08)
#define DSP_CMD_ASSERT        \
        DSP_CMD_DEFINE(MBOX_CH_1, DSP_CMD_DSP2ARM, 0x09)

#define DSP_CMD_STARTUP             \
        DSP_CMD_DEFINE(MBOX_CH_2, DSP_CMD_DSP2ARM, 0x01)
#define DSP_CMD_PRE_FILE_INFO   \
        DSP_CMD_DEFINE(MBOX_CH_2, DSP_CMD_ARM2DSP, 0x02)
#define DSP_CMD_PRE_FILE_DONE           \
        DSP_CMD_DEFINE(MBOX_CH_2, DSP_CMD_DSP2ARM, 0x03)

#define DSP_CMD_TRACE       \
        DSP_CMD_DEFINE(MBOX_CH_3, DSP_CMD_DSP2ARM, 0x01)
#define DSP_CMD_TRACE_DONE  \
        DSP_CMD_DEFINE(MBOX_CH_3, DSP_CMD_ARM2DSP, 0x01)

#define RKDSP_CACHE_LINE_SIZE      (128)

#if defined(XIP_MAP0_BASE0) && defined(XIP_MAP0_BASE1)
#define RKDSP_XIP_ADDR_FOR_DSP          XIP_MAP0_BASE1
#define RKDSP_XIP_ADDR_FOR_ARM          XIP_MAP0_BASE0
#define RKDSP_XIP_ADDR_ALIAS(alias, addr)                                                       \
    if (((uint32_t)addr & 0xFF000000) == RKDSP_XIP_ADDR_FOR_ARM)                                \
        alias = (void *)(((uint32_t)addr & ~RKDSP_XIP_ADDR_FOR_ARM) | RKDSP_XIP_ADDR_FOR_DSP);  \
    else                                                                                        \
        alias = addr;
#else
#define RKDSP_XIP_ADDR_ALIAS(alias, addr) { alias = addr; }
#endif

enum rkdsp_work_type
{
    RKDSP_ALGO_WORK = 0,
    RKDSP_CFG_WORK,
    RKDSP_CODEC_WORK
};

struct dsp_work
{
    uint32_t id;
    uint32_t algo_type;
    uint32_t param;
    uint32_t param_size;
    uint32_t result;
    uint32_t work_type;
    uint32_t rate;
    uint32_t cycles;
};

struct dsp_auth
{
    uint32_t flag;
    uint32_t key;
    uint32_t len;
    uint32_t result;
};

#ifndef rkdsp_malloc
#define rkdsp_malloc(size)            \
            rt_malloc_align(RT_ALIGN(size, RKDSP_CACHE_LINE_SIZE), \
                            RKDSP_CACHE_LINE_SIZE)
#endif

#ifndef rkdsp_free
#define rkdsp_free(buf)            \
                rt_free_align(buf)
#endif

struct dsp_work *rk_dsp_work_create(enum rkdsp_work_type work_type);
rt_err_t rk_dsp_work_destroy(struct dsp_work *work);

#endif // _DRV_DSP_H_
