
/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp.h
  * @author  ISP_TEAM
  * @version V0.0.1
  * @date
  * @brief   image signal processing (ISP) abstract device
  *
  ******************************************************************************
  */
#ifndef __ISP_H__
#define __ISP_H__

/* Includes ------------------------------------------------------------------*/
#include <rthw.h>
#include <rtthread.h>
#include "rtconfig.h"

#if defined(RT_USING_ISP)

#define ISP_WORK_QUEUE_ENABLE 0

/* Exported macro ------------------------------------------------------------*/
#define RK_ISP_CMD_G_DPCC                                       0
#define RK_ISP_CMD_S_DPCC                                       1
#define RK_ISP_CMD_G_BLS                                        2
#define RK_ISP_CMD_S_BLS                                        3
#define RK_ISP_CMD_G_SDG                                        4
#define RK_ISP_CMD_S_SDG                                        5
#define RK_ISP_CMD_G_LSC                                        6
#define RK_ISP_CMD_S_LSC                                        7
#define RK_ISP_CMD_G_AWB_MEAS                                   8
#define RK_ISP_CMD_S_AWB_MEAS                                   9
#define RK_ISP_CMD_G_FLT                                        10
#define RK_ISP_CMD_S_FLT                                        11
#define RK_ISP_CMD_G_BDM                                        12
#define RK_ISP_CMD_S_BDM                                        13
#define RK_ISP_CMD_G_CTK                                        14
#define RK_ISP_CMD_S_CTK                                        15
#define RK_ISP_CMD_G_GOC                                        16
#define RK_ISP_CMD_S_GOC                                        17
#define RK_ISP_CMD_G_HST                                        18
#define RK_ISP_CMD_S_HST                                        19
#define RK_ISP_CMD_G_AEC                                        20
#define RK_ISP_CMD_S_AEC                                        21
#define RK_ISP_CMD_G_BPL                                        22
#define RK_ISP_CMD_G_AWB_GAIN                                   23
#define RK_ISP_CMD_S_AWB_GAIN                                   24
#define RK_ISP_CMD_G_CPROC                                      25
#define RK_ISP_CMD_S_CPROC                                      26
#define RK_ISP_CMD_G_AFC                                        27
#define RK_ISP_CMD_S_AFC                                        28
#define RK_ISP_CMD_G_IE                                         29
#define RK_ISP_CMD_S_IE                                         30
#define RK_ISP_CMD_G_DPF                                        31
#define RK_ISP_CMD_S_DPF                                        32
#define RK_ISP_CMD_G_DPF_STRENGTH                               33
#define RK_ISP_CMD_S_DPF_STRENGTH                               34
#define RK_ISP_CMD_G_LAST_CONFIG                                35
#define RK_ISP_CMD_S_WDR                                        36
#define RK_ISP_CMD_G_WDR                                        37
#define RK_ISP_CMD_G_MEAS_BUF                                   38
#define RK_ISP_CMD_STREAM_ON                                    39
#define RK_ISP_CMD_STREAM_OFF                                   40
#define RK_ISP_CMD_DEVICE_INIT                                  41
#define RK_ISP_CMD_SET_FMT                                      42
#define RK_ISP_CMD_CROP_IMAGE                                   43
#define RK_ISP_CMD_IMAGE_EFFECT                                 44
#define RK_ISP_CMD_SUPER_IMPOSE                                 45
#define RK_ISP_CMD_SET_ISP_INIT_INFO                            46
#define RK_ISP_CMD_SET_OUT_ADDR                                 47
#define RK_ISP_CMD_SET_OUT_PING_PONG_ADDR                       48
#define RK_ISP_SET_MOD_EN                                       49
#define RK_ISP_GET_MOD_EN                                       50
#define RK_ISP_GET_OUT_SIZE                                     51
#define RK_ISP_SET_IMG_EFF_RGB2GREY                             52
#define RK_ISP_SET_IMG_EFF_GREY2RGB                             53
#define RK_ISP_SET_HUE                                          54
#define RK_ISP_SET_BRIGHTNESS                                   55
#define RK_ISP_SET_SAT                                          56
#define RK_ISP_SET_CONTRAST                                     57


#ifndef bool
#define bool unsigned int
#define true (1)
#define false (0)
#endif


/* Exported types ------------------------------------------------------------*/
struct rk_isp_ops;
struct rk_isp_dev
{
    struct rt_device parent;
    const struct rk_isp_ops *ops;
};

struct rk_isp_set_addr
{
    unsigned int y_addr;
    unsigned int u_addr;
    unsigned int v_addr;
    bool b_set_uv_flag;
};

struct rk_isp_ops
{
    rt_err_t (*init)(struct rk_isp_dev *dev);
    rt_err_t (*open)(struct rk_isp_dev *dev, unsigned short oflag);
    rt_err_t (*close)(struct rk_isp_dev *dev);
    rt_err_t (*control)(struct rk_isp_dev *dev, int cmd, void *arg);
    rt_err_t (*rx_indicate)(struct rk_isp_dev *dev, rt_size_t size);
};

#if ISP_WORK_QUEUE_ENABLE
struct rk_isp_work
{
    struct rt_work work;
    void (*fun)(void *parameter);
    void *parameter;
};
void rk_isp_workqueue_fun(struct rt_work *work, void *work_data);
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

rt_err_t rk_isp_controller_register(struct rk_isp_dev *isp,
                                    const char *name,
                                    void *data);

#endif

#endif

