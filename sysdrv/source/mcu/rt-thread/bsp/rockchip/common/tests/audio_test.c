/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    audio_test.c
  * @author  sugar zhang
  * @version V0.1
  * @date    10-Feb-2019
  * @brief   audio test for pisces
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_AUDIO

#include <stdint.h>
#include "rk_audio.h"
#include "hal_base.h"
#include "drv_heap.h"
#ifdef RT_USING_DSP_WAKEUP
#include "dsp_test.h"
#endif

/**
 * config vad buffer size which must be multiple of period size
 */
#define AUDIO_VAD_BUFFER_SIZE   (128 * 1024) /* 128 KB */
#define AUDIO_VAD_PERIOD_KSIZE  (8) /* size in units of kbytes */
/**
 * config audio sram buffer size which must be multiple of period size
 */
#define AUDIO_SRAM_BUFFER_SIZE  (128 * 1024) /* 256 KB */
#define AUDIO_SRAM_PERIOD_SIZE  (8 * 1024) /* 8 KB */

#ifdef RT_USING_DSP_WAKEUP
static void dsp_audio_callback(uint32_t status)
{
    struct rt_device *audio_dev;
    rt_err_t ret;

    audio_dev = rt_device_find("sound0c");
    RT_ASSERT(audio_dev != RT_NULL);

    /* TBD: handle dsp status */

    /*
     * dsp had detected the key words voice, so switch audio
     * path from 'dai-->vad-->dtcm(dsp)' to 'dai-->dma-->sram'.
     */
    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PATH_DMA, NULL);
    RT_ASSERT(ret == RT_EOK);
}
#endif

static uint8_t *capturebuf = NULL;

void audio_playback();
void audio_capture(void)
{
    struct audio_buf vadbuf;
    struct AUDIO_PARAMS aparams;
    struct audio_buf abuf;
    struct rt_device *audio_dev;
    struct rt_device *vad_dev;
    uint32_t periodsz = 0;
    uint8_t en = 1;
    rt_err_t ret = RT_EOK;

    rt_kprintf("%s\n", __func__);

    audio_dev = rt_device_find("sound0c");
    RT_ASSERT(audio_dev);
    vad_dev = rt_device_find("vad");
    RT_ASSERT(vad_dev);

    rt_device_open(audio_dev, RT_DEVICE_OFLAG_RDONLY);
    rt_device_open(vad_dev, RT_DEVICE_OFLAG_RDONLY);

    vadbuf.buf_size = AUDIO_VAD_BUFFER_SIZE;

#ifdef RT_USING_DSP_WAKEUP
    /* test with dsp audio recognize routine, buffer is placed in dsp dtcm */
    vadbuf.buf = (uint8_t *)dsp_malloc(AUDIO_VAD_BUFFER_SIZE);
    RT_ASSERT(vadbuf.buf != RT_NULL);
    periodsz = dsp_get_period_size() / 1024;
    RT_ASSERT(periodsz > 0);
    dsp_wakeup_register_call((dsp_wakeup_call)&dsp_audio_callback);
#else
    /* cm4 standalone test, buffer is placed in sram */
    vadbuf.buf = (uint8_t *)rt_malloc_large(AUDIO_VAD_BUFFER_SIZE);
    RT_ASSERT(vadbuf.buf != RT_NULL);
    periodsz = AUDIO_VAD_PERIOD_KSIZE;
    capturebuf = vadbuf.buf;
#endif

    rt_kprintf("vad buf: 0x%x, size: 0x%x bytes\n", vadbuf.buf, vadbuf.buf_size);
    rt_kprintf("vad periodsize: 0x%x kbytes\n", periodsz);

    /* config vad */
    ret = rt_device_control(vad_dev, RK_AUDIO_CTL_VAD_SET_BUFFER, &vadbuf);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(vad_dev, RK_AUDIO_CTL_VAD_SET_PERIOD, &periodsz);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(vad_dev, RK_AUDIO_CTL_VAD_EN_BUSMODE, &en);
    RT_ASSERT(ret == RT_EOK);

    /* config stream */
    rt_memset(&aparams, 0x0, sizeof(aparams));
    aparams.channels = AUDIO_CHANNELS_2;
    aparams.sampleRate = AUDIO_SAMPLERATE_16000;
    aparams.sampleBits = AUDIO_SAMPLEBITS_16;

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_HW_PARAMS, &aparams);
    RT_ASSERT(ret == RT_EOK);

    rt_memset(&abuf, 0x0, sizeof(abuf));
    abuf.buf = (uint8_t *)rt_malloc_large(AUDIO_SRAM_BUFFER_SIZE);
    RT_ASSERT(abuf.buf != RT_NULL);
    abuf.buf_size = AUDIO_SRAM_BUFFER_SIZE;
    abuf.period_size = AUDIO_SRAM_PERIOD_SIZE;

//    ret = rt_device_control(audio_dev, AUDIO_IOCTL_PCM_PREPARE, &abuf);
//    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_START, NULL);
    RT_ASSERT(ret == RT_EOK);
}

void audio_playback()
{
    struct AUDIO_PARAMS aparams;
    struct audio_buf abuf;
    struct rt_device *audio_dev;
    rt_err_t ret = RT_EOK;

    rt_kprintf("%s\n", __func__);
    audio_dev = rt_device_find("sound1p");
    RT_ASSERT(audio_dev);

    rt_device_open(audio_dev, RT_DEVICE_OFLAG_WRONLY);

    /* config stream */
    rt_memset(&aparams, 0x0, sizeof(aparams));
    aparams.channels = AUDIO_CHANNELS_2;
    aparams.sampleRate = AUDIO_SAMPLERATE_16000;
    aparams.sampleBits = AUDIO_SAMPLEBITS_16;

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_HW_PARAMS, &aparams);
    RT_ASSERT(ret == RT_EOK);

    rt_memset(&abuf, 0x0, sizeof(abuf));
    abuf.buf = capturebuf;//(uint8_t *)rt_malloc_large(AUDIO_SRAM_BUFFER_SIZE);
    if (!abuf.buf)
    {
        abuf.buf = (uint8_t *)rt_malloc_large(AUDIO_SRAM_BUFFER_SIZE);
        rt_kprintf("new pcm buf: 0x%x, clean buf\n", abuf.buf);
        memset(abuf.buf, 0x0, AUDIO_SRAM_BUFFER_SIZE);
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, abuf.buf, AUDIO_SRAM_BUFFER_SIZE);
    }
    RT_ASSERT(abuf.buf != RT_NULL);
    abuf.buf_size = AUDIO_SRAM_BUFFER_SIZE;
    abuf.period_size = AUDIO_SRAM_PERIOD_SIZE;
    rt_kprintf("pcm buf: 0x%x, size: 0x%x bytes\n", abuf.buf, abuf.buf_size);
    rt_kprintf("pcm periodsize: 0x%x kbytes\n", AUDIO_SRAM_PERIOD_SIZE);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_PREPARE, &abuf);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_PCM_START, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(audio_dev, RK_AUDIO_CTL_START, NULL);
    RT_ASSERT(ret == RT_EOK);
}

void audio_loopback()
{
    audio_capture();
    audio_playback();
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(audio_capture, audio drive test. e.g: audio_capture);
MSH_CMD_EXPORT(audio_playback, audio drive test. e.g: audio_playback);
MSH_CMD_EXPORT(audio_loopback, audio drive test. e.g: audio_loopback);
#endif

#endif
