/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */
#include "wiced_result.h"
#include "wiced_platform.h"
#include "wiced_rtos.h"
#include "wiced_audio.h"
#include "platform_i2s.h"

#ifndef BT_AUDIO_DEVICE
#define BT_AUDIO_DEVICE "es8388p"
#endif

#ifndef BT_MIC_DEVICE
#define BT_MIC_DEVICE "pdmc"
#endif

typedef struct
{
    const char *name;
} brcm_dac_data_t;

brcm_dac_data_t dac_data =
{
    .name = "brcm dac"
};

brcm_dac_data_t rk_recorder_data =
{
    .name = "rk recorder"
};

#include <rtdevice.h>
#include <rtthread.h>
#include "rk_audio.h"
#include "hal_base.h"

#include "drv_heap.h"

static rt_device_t audio_device = NULL;
static rt_device_t r_audio_device = NULL;

static struct AUDIO_PARAMS param = {0, 0, 0};
static struct audio_buf abuf = {NULL, 0, 0};

static struct AUDIO_PARAMS r_param = {0, 0, 0};
static struct audio_buf r_abuf = {NULL, 0, 0};

static uint32_t frame_size = 0;
static uint32_t r_frame_size = 0;

void a2d_audio_play(uint8_t *data, int len)
{
    int count, remain = len, wlen, wframe;
    char *pdata = data;

    //printf("[0x%08lx] %d\n", HAL_GetTick(), len);
    if (remain > 0)
    {
        if (remain > frame_size)
            count = (remain / frame_size) + 1;
        else
            count = 1;


        for (; count; count--)
        {
            if (remain < frame_size)
            {
                rt_kprintf("period_size change ,new period_size:%d,old:%d\n", remain, frame_size);

                frame_size = remain;
                //a2d_reset_audio_cfg(0, frame_size);
            }
            //rt_kprintf("audio_play data_size:%d \n", len);
            wlen = (remain > frame_size) ? frame_size : remain;
            wframe = wlen / param.channels / (param.sampleBits >> 3);
            if (!rt_device_write(audio_device, 0, pdata, wframe))
            {
                rt_kprintf("Error rt_device_write fail,data_size = %d\n", wlen);
            }
            remain -= wlen;
            pdata = pdata + wlen;
            if (remain == 0)
                break;
        }
    }
}

/******************************************************
 *               Function Declarations
 ******************************************************/
static wiced_result_t brcm_dac_configure(void *driver_data, wiced_audio_config_t *config, uint32_t *mclk);
static wiced_result_t brcm_dac_init(void *driver_data, wiced_audio_data_port_t *data_port);
static wiced_result_t brcm_dac_play(void *driver_data);
static wiced_result_t brcm_dac_stop_play(void *driver_data);
wiced_result_t brcm_dac_deinit(void *driver_data);
static wiced_result_t brcm_dac_volume(void *driver_data, double decibels);
static wiced_result_t brcm_dac_get_volume_range(void *driver_data, double *min_volume_in_decibels, double *max_volume_in_decibels);
static wiced_result_t brcm_dac_ioctl(void *driver_data, wiced_audio_device_ioctl_t cmd, wiced_audio_device_ioctl_data_t *cmd_data);

const wiced_audio_device_interface_t brcm_dac_interface =
{
    .audio_device_init             = brcm_dac_init,
    .audio_device_deinit           = brcm_dac_deinit,
    .audio_device_configure        = brcm_dac_configure,
    .audio_device_start_streaming  = brcm_dac_play,
    .audio_device_stop_streaming   = brcm_dac_stop_play,
    .audio_device_set_volume       = brcm_dac_volume,
    .audio_device_set_treble       = NULL,
    .audio_device_set_bass         = NULL,
    .audio_device_get_volume_range = brcm_dac_get_volume_range,
    .audio_device_ioctl            = brcm_dac_ioctl,
    .audio_device_driver_specific  = &dac_data,
    .device_id                     = 0xab00
};

static wiced_result_t brcm_dac_init(void *driver_data, wiced_audio_data_port_t *data_port)
{
    int ret;

    printf("==%s==\n", __func__);

    if (audio_device == NULL)
    {
        audio_device = rt_device_find(BT_AUDIO_DEVICE);
        printf("==audio_device %p==\n", audio_device);
        ret = rt_device_open(audio_device, RT_DEVICE_OFLAG_WRONLY);
        RT_ASSERT(ret == RT_EOK);
    }

    return WICED_SUCCESS;
}

static wiced_result_t brcm_dac_configure(void *driver_data, wiced_audio_config_t *config, uint32_t *mclk)
{
    uint32_t size;
    int ret;

    if (audio_device != NULL)
    {
        abuf.period_size = 128;
        abuf.buf_size = abuf.period_size * 16;
        size = abuf.buf_size * 2 * (16 >> 3); /* frames to bytes */
        frame_size = abuf.period_size * 2 * (16 >> 3);

        abuf.buf = rt_malloc_uncache(size);
        RT_ASSERT(abuf.buf != NULL);

        param.channels = config->channels;
        param.sampleRate = config->sample_rate;
        param.sampleBits = config->bits_per_sample;

        ret = rt_device_control(audio_device, RK_AUDIO_CTL_PCM_PREPARE, &abuf);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(audio_device, RK_AUDIO_CTL_HW_PARAMS, &param);
        RT_ASSERT(ret == RT_EOK);
    }
    printf("### brcm_dac_configure rate: %d, channels: %d, bits: %d ###\n",
           param.sampleRate, param.channels, param.sampleBits);

    return WICED_SUCCESS;
}

static wiced_result_t brcm_dac_play(void *driver_data)
{
    printf("==%s==\n", __func__);
    return WICED_SUCCESS;
}

static wiced_result_t brcm_dac_stop_play(void *driver_data)
{
    //if (play_dev)
    //  pcm_stop(play_dev);
    printf("==%s==\n", __func__);
    return WICED_SUCCESS;
}


wiced_result_t brcm_dac_deinit(void *driver_data)
{
    int ret = 0;
    printf("==%s audio_device %p==\n", __func__, audio_device);

    if (audio_device != NULL)
    {
        ret = rt_device_control(audio_device, RK_AUDIO_CTL_STOP, NULL);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(audio_device, RK_AUDIO_CTL_PCM_RELEASE, NULL);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_close(audio_device);
        RT_ASSERT(ret == RT_EOK);

        if (abuf.buf != NULL)
        {
            rt_free_uncache(abuf.buf);
            abuf.buf = NULL;
        }

        audio_device = NULL;
    }

    return WICED_SUCCESS;
}

static wiced_result_t brcm_dac_volume(void *driver_data, double decibels)
{
    printf("==%s==\n", __func__);
    return WICED_SUCCESS;
}

static wiced_result_t brcm_dac_get_volume_range(void *driver_data, double *min_volume_in_decibels, double *max_volume_in_decibels)
{
    UNUSED_PARAMETER(driver_data);

    *min_volume_in_decibels = 12;
    *max_volume_in_decibels = 100;

    return WICED_SUCCESS;
}

static wiced_result_t brcm_dac_ioctl(void *driver_data, wiced_audio_device_ioctl_t cmd, wiced_audio_device_ioctl_data_t *cmd_data)
{
    UNUSED_PARAMETER(driver_data);
    UNUSED_PARAMETER(cmd);
    UNUSED_PARAMETER(cmd_data);
    printf("==%s==\n", __func__);

    return WICED_UNSUPPORTED;
}

// recorder

uint32_t rk_recorder_capture_sample(char *ptr, int len)
{
    int rlen = 0;

    RT_ASSERT(r_abuf.period_size == len);

    rlen = rt_device_read(r_audio_device, 0, ptr, r_abuf.period_size);

    if (rlen != r_abuf.period_size)
    {
        rt_kprintf("Capturing sample: rlen %d, period_size: %d\n", rlen, r_abuf.period_size);
    }

    return r_abuf.period_size;
}

static wiced_result_t rk_recorder_configure(void *driver_data, wiced_audio_config_t *config, uint32_t *mclk)
{
    uint32_t size;
    int ret;

    if (r_audio_device != NULL)
    {
        r_abuf.period_size = 256;
        r_abuf.buf_size = r_abuf.period_size * 8;
        size = r_abuf.buf_size * 2 * (16 >> 3); /* frames to bytes */
        r_frame_size = r_abuf.period_size * 2 * (16 >> 3);

        r_abuf.buf = rt_malloc_uncache(size);
        RT_ASSERT(r_abuf.buf != NULL);

        r_param.channels = config->channels;
        r_param.sampleRate = config->sample_rate;
        r_param.sampleBits = config->bits_per_sample;

        ret = rt_device_control(r_audio_device, RK_AUDIO_CTL_PCM_PREPARE, &r_abuf);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(r_audio_device, RK_AUDIO_CTL_HW_PARAMS, &r_param);
        RT_ASSERT(ret == RT_EOK);
    }
    printf("### brcm_dac_configure rate: %d, channels: %d, bits: %d ###\n",
           config->sample_rate, config->channels, config->bits_per_sample);

    return WICED_SUCCESS;
}
wiced_result_t rk_recorder_deinit(void *driver_data)
{
    int ret = 0;
    printf("==%s audio_device %p==\n", __func__, r_audio_device);

    if (r_audio_device != NULL)
    {
        ret = rt_device_control(r_audio_device, RK_AUDIO_CTL_STOP, NULL);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(r_audio_device, RK_AUDIO_CTL_PCM_RELEASE, NULL);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_close(r_audio_device);
        RT_ASSERT(ret == RT_EOK);

        if (r_abuf.buf != NULL)
        {
            rt_free_uncache(r_abuf.buf);
            r_abuf.buf = NULL;
        }

        r_audio_device = NULL;
    }

    return WICED_SUCCESS;
}

static wiced_result_t rk_recorder_init(void *driver_data, wiced_audio_data_port_t *data_port)
{
    int ret;

    printf("==%s==\n", __func__);

    if (r_audio_device == NULL)
    {
        //r_audio_device = rt_device_find("adcc");
        r_audio_device = rt_device_find(BT_MIC_DEVICE);
        printf("==audio_device %p==\n", r_audio_device);
        ret = rt_device_open(r_audio_device, RT_DEVICE_OFLAG_RDONLY);
        RT_ASSERT(ret == RT_EOK);
    }

    return WICED_SUCCESS;
}

const wiced_audio_device_interface_t rk_recorder_interface =
{
    .audio_device_init             = rk_recorder_init,
    .audio_device_deinit           = rk_recorder_deinit,
    .audio_device_configure        = rk_recorder_configure,
    .audio_device_start_streaming  = NULL,
    .audio_device_stop_streaming   = NULL,
    .audio_device_set_volume       = NULL,
    .audio_device_set_treble       = NULL,
    .audio_device_set_bass         = NULL,
    .audio_device_get_volume_range = NULL,
    .audio_device_ioctl            = NULL,
    .audio_device_driver_specific  = &rk_recorder_data,
    .device_id                     = 0xaaaa
};

/* This function can only be called from the platform initialization routine */
wiced_result_t brcm_dac_device_register()
{
    wiced_register_audio_device(rk_recorder_interface.device_id, &rk_recorder_interface);
    return wiced_register_audio_device(brcm_dac_interface.device_id, &brcm_dac_interface);
}
