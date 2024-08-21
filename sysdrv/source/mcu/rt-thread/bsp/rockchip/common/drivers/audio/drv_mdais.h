/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_mdais.h
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   mdais driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#if defined(RT_USING_MULTI_DAIS)

#include "rk_audio.h"

struct rk_dai
{
    struct audio_dai *dai;
    unsigned int fmt;
    unsigned int fmt_msk;
};

struct rk_mdais_dev
{
    struct audio_dai dai;
    struct rk_dai *dais;
    uint8_t num_dais;
    const struct audio_mdais_desc *desc;
};

static inline struct rk_mdais_dev *to_mdais(struct audio_dai *dai)
{
    return HAL_CONTAINER_OF(dai, struct rk_mdais_dev, dai);
}

#endif
