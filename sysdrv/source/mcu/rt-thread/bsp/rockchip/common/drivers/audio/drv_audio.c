/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_audio.c
  * @author  Sugar Zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   audio driver for rksoc
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup Audio
 *  @{
 */

/** @defgroup Audio_How_To_Use How To Use
 *  @{

 See more information, click [here](https://www.rt-thread.org/document/site/programming-manual/device/audio/audio/)

 @} */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_AUDIO

#include "rk_audio.h"

RT_WEAK const struct audio_card_desc rk_board_audio_cards[] = {0};

int rk_snd_card_init(void)
{
    const struct audio_card_desc *desc;
    rt_err_t ret;

    for (desc = rk_board_audio_cards; desc->dai != NULL; desc++)
    {
        ret = rk_audio_register_by_card_desc(desc);
        if (ret != RT_EOK)
            rt_kprintf("%s register failed!\n", desc->name);
    }

    return 0;
}

INIT_ENV_EXPORT(rk_snd_card_init);
#endif

/** @} */  // Audio

/** @} */  // RKBSP_Driver_Reference
