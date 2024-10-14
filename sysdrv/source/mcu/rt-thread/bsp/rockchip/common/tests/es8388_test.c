/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    es8388_test.c
  * @author  Xing Zheng
  * @version v0.1
  * @date    2019.07.01
  * @brief   The rt-thread codec test for Rockchip
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_CODEC_ES8388

#include <stdint.h>
#include "rk_audio.h"
#include "hal_base.h"
#include "drv_heap.h"

#define AUDIO_PERIOD_BYTES (256 * 2 * 2)  /* 1 KB */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* sine 1k, 16k/16-bit/2ch */
const uint16_t sine_1000_0_3db_16k_2ch_16bit_1period[] =
{
    0x0000, 0x0000,
    0x0eb2, 0x0eb2,
    0x1b27, 0x1b27,
    0x237a, 0x237a,
    0x2667, 0x2667,
    0x237a, 0x237a,
    0x1b26, 0x1b26,
    0x0eb4, 0x0eb4,
    0xfffd, 0xfffd,
    0xf152, 0xf152,
    0xe4d5, 0xe4d5,
    0xdc89, 0xdc89,
    0xd998, 0xd998,
    0xdc86, 0xdc86,
    0xe4d9, 0xe4d9,
    0xf14e, 0xf14e,
};

/* To be cleanup */
static int prepare_misc_env(void)
{
    /* set i2s1 tx bus via m0 */
    GRF->GPIO0A_IOMUX_H = 0x77774444;
    GRF->GPIO0B_IOMUX_L = 0x00070004;

    /* set burst request for i2s1 tx and rx */
    // GRF->DMAC_CON[6] = 0x000f0005;
    /*
     * refer to '11.6.7 Interconnection of I2SIN, I2S1, I2S1OUT and Audio Codec',
     * set i2s1 from I2SIN IO interface
     */
    GRF->SOC_CON[4] = 0x06000600;

    return RT_EOK;
}
void es8388_test(int argc, char * *argv)
{
    struct AUDIO_PARAMS aparams;
    struct audio_buf abuf;
    struct rt_device *audio_dev;
    uint32_t i, j, play_secs = 4; /* 1s=62.5 period_size, 250/62.5=4s */
    uint32_t trans_len = AUDIO_PERIOD_BYTES * sizeof(uint16_t);
    uint32_t sine_bytes = ARRAY_SIZE(sine_1000_0_3db_16k_2ch_16bit_1period) * sizeof(uint16_t);
    uint16_t *sine_buf = (uint16_t *)sine_1000_0_3db_16k_2ch_16bit_1period;
    void *period_buf = NULL;
    rt_err_t ret = RT_EOK;

    prepare_misc_env();

    if (argc == 2)
        play_secs = strtol(argv[1], NULL, 10);

    rt_kprintf("%s playback %d secs\n", __func__, play_secs);

    audio_dev = rt_device_find("sound1p");
    RT_ASSERT(audio_dev != RT_NULL);

    period_buf = (void *)rt_malloc_align(trans_len, 32);
    RT_ASSERT(period_buf != RT_NULL);

    rt_device_open(audio_dev, RT_DEVICE_OFLAG_WRONLY);

    for (i = 0; i < trans_len / sine_bytes; i++)
    {
        rt_memcpy((void *)(period_buf + sine_bytes * i), (void *)sine_buf, sine_bytes);
    }

#if 0  /* dump period_buf */
    {
        uint16_t *period_buf16 = (uint16_t *)period_buf;

        for (i = 0; i < trans_len / 2; i++)
        {
            rt_kprintf("period_buf[%d]: 0x%x\n", i, period_buf16[i]);
        }
    }
#endif

    /* config stream */
    rt_memset(&aparams, 0x0, sizeof(aparams));
    aparams.channels = AUDIO_CHANNELS_2;
    aparams.sampleRate = AUDIO_SAMPLERATE_16000;
    aparams.sampleBits = AUDIO_SAMPLEBITS_16;

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_HW_PARAMS, &aparams);
    RT_ASSERT(ret == RT_EOK);

    rt_memset(&abuf, 0x0, sizeof(abuf));
    abuf.buf = period_buf;
    abuf.buf_size = trans_len;
    abuf.period_size = trans_len;

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_PREPARE, &abuf);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_START, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_START, NULL);
    RT_ASSERT(ret == RT_EOK);

    /* wait a moments */
    while (play_secs--)
    {
        rt_thread_mdelay(1000);
    }

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_STOP, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_STOP, NULL);
    RT_ASSERT(ret == RT_EOK);

    rt_free_align(period_buf);

    rt_device_close(audio_dev);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(es8388_test, codec es8388 test. e.g: es8388_test <play_secs>);
#endif

#endif
