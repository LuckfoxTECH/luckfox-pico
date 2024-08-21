/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <stdio.h>
#include <string.h>

#include "drv_dsp.h"
#include "drv_heap.h"
#include "drv_pm.h"
#include "hal_base.h"

#include "lib_imageprocess.h"

#define PRINTF_TIME     0
#define DO_ROTATE_TEST  0

static struct rt_device *g_dsp_dev = NULL;
static rt_mutex_t work_lock = NULL;

#if !RK_SCALE_USING_DSP
typedef void (*scale_fun)(image_st *src, image_st *dst);
static scale_fun fun_table[] =
{
    imgScale_565RGB2RGB,
    imgScale_RGB2RGB,
    imgScale_ARGB2RGB,
    imgScale_565RGB2RGB565,
    imgScale_RGB2RGB565,
    imgScale_ARGB2RGB565,
};
#endif

void *rk_imagelib_dsp_dev(void)
{
    return (void *)g_dsp_dev;
}

void rk_imagelib_dsp_lock(void)
{
    if (work_lock)
        rt_mutex_take(work_lock, RT_WAITING_FOREVER);
}

void rk_imagelib_dsp_unlock(void)
{
    if (work_lock)
        rt_mutex_release(work_lock);
}

#ifdef RT_USING_FINSH
extern long list_device(void);
#endif
int rk_imagelib_init(void)
{
#if RK_ROTATE_USING_DSP || RK_SCALE_USING_DSP || RK_LZW_USING_DSP
    uint32_t rate;

    if (g_dsp_dev != NULL)
        return RT_EOK;

    g_dsp_dev = rt_device_find("dsp0");
    if (g_dsp_dev == NULL)
    {
        rt_kprintf("Cannot find dsp0\n");
#ifdef RT_USING_FINSH
        list_device();
#endif
        return RT_ERROR;
    }
    rt_device_open(g_dsp_dev, RT_DEVICE_OFLAG_RDWR);

    rt_device_control(g_dsp_dev, RKDSP_CTL_SET_FREQ, (void *)(99000000));
    rt_device_control(g_dsp_dev, RKDSP_CTL_GET_FREQ, (void *)&rate);
    rt_kprintf("Current dsp freq: %d MHz\n", rate / 1000000);
#endif
    work_lock = rt_mutex_create("work_lock", RT_IPC_FLAG_PRIO);
    RT_ASSERT(work_lock != NULL);

    return RT_EOK;
}

void rk_rotate_process(struct rotateimage_st *ps, struct rotateimage_st *pd, int32_t angle)
{
#if RK_ROTATE_USING_DSP
    struct dsp_work *work;
    struct rk_rotate_cfg *param;
    int ret;

    if (g_dsp_dev == NULL)
        return;

    param = (struct rk_rotate_cfg *)rkdsp_malloc(sizeof(struct rk_rotate_cfg));
    param->ps.width = ps->width;
    param->ps.height = ps->height;
    param->ps.stride = ps->stride;
    param->ps.cx = ps->cx;
    param->ps.cy = ps->cy;
    RKDSP_XIP_ADDR_ALIAS(param->ps.pdata, ps->pdata);

    param->pd.width = pd->width;
    param->pd.height = pd->height;
    param->pd.stride = pd->stride;
    param->pd.cx = pd->cx;
    param->pd.cy = pd->cy;
    param->pd.pdata = pd->pdata;

    param->angle = angle % 360;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    RT_ASSERT(work);
    work->id = IMG_PROCESS_ID;
    work->algo_type = DSP_ALGO_ROTATE_24BIT;
    work->param = (uint32_t)param;
    work->param_size = sizeof(struct rk_rotate_cfg);

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->ps.pdata, param->ps.stride * param->ps.height);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rk_imagelib_dsp_lock();
    pm_runtime_request(PM_RUNTIME_ID_USB);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    pm_runtime_release(PM_RUNTIME_ID_USB);
    rk_imagelib_dsp_unlock();
#if PRINTF_TIME
    rt_kprintf("[DSP]Rotate %ld cast %lu us(%lu cycles)\n", param->angle, (uint32_t)(work->cycles / 396), work->cycles);
#endif
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rkdsp_free(param);
    rk_dsp_work_destroy(work);
#else
#if PRINTF_TIME
    uint32_t st, et;
    st = HAL_GetTick();
#endif
    imagerotate_ARGB2RGB(ps, pd, (float)(angle % 360));
#if PRINTF_TIME
    et = HAL_GetTick();
    rt_kprintf("[ARM]Rotate %ld cast %lu ms\n", angle % 360, et - st);
#endif
#endif
}

void rk_rotate_process_16bit(struct rotateimage_st *ps, struct rotateimage_st *pd, int32_t angle)
{
#if RK_ROTATE_USING_DSP
    struct dsp_work *work;
    struct rk_rotate_cfg *param;
    int ret;

    if (g_dsp_dev == NULL)
        return;

    param = (struct rk_rotate_cfg *)rkdsp_malloc(sizeof(struct rk_rotate_cfg));
    param->ps.width = ps->width;
    param->ps.height = ps->height;
    param->ps.stride = ps->stride;
    param->ps.cx = ps->cx;
    param->ps.cy = ps->cy;
    RKDSP_XIP_ADDR_ALIAS(param->ps.pdata, ps->pdata);

    param->pd.width = pd->width;
    param->pd.height = pd->height;
    param->pd.stride = pd->stride;
    param->pd.cx = pd->cx;
    param->pd.cy = pd->cy;
    param->pd.pdata = pd->pdata;

    param->angle = angle % 360;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    RT_ASSERT(work);
    work->id = IMG_PROCESS_ID;
    work->algo_type = DSP_ALGO_ROTATE_16BIT;
    work->param = (uint32_t)param;
    work->param_size = sizeof(struct rk_rotate_cfg);

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->ps.pdata, param->ps.stride * param->ps.height);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rk_imagelib_dsp_lock();
    pm_runtime_request(PM_RUNTIME_ID_USB);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    pm_runtime_release(PM_RUNTIME_ID_USB);
    rk_imagelib_dsp_unlock();
#if PRINTF_TIME
    rt_kprintf("[DSP]Rotate %ld cast %lu us(%lu cycles)\n", param->angle, (uint32_t)(work->cycles / 396), work->cycles);
#endif
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rkdsp_free(param);
    rk_dsp_work_destroy(work);
#else
#if PRINTF_TIME
    uint32_t st, et;
    st = HAL_GetTick();
#endif
    imagerotate_ARGB2RGB565(ps, pd, (float)(angle % 360));
#if PRINTF_TIME
    et = HAL_GetTick();
    rt_kprintf("[ARM]Rotate %ld cast %lu ms\n", angle % 360, et - st);
#endif
#endif
}

void rk_rotate_process_16bitfast(struct rotateimage_st *ps, struct rotateimage_st *pbg, struct rotateimage_st *pd, short *preIndex, int32_t angle)
{
#if RK_ROTATE_USING_DSP
    struct dsp_work *work;
    struct rk_rotate_cfg *param;
    int ret;

    if (g_dsp_dev == NULL)
        return;

    param = (struct rk_rotate_cfg *)rkdsp_malloc(sizeof(struct rk_rotate_cfg));
    param->ps.width = ps->width;
    param->ps.height = ps->height;
    param->ps.stride = ps->stride;
    param->ps.cx = ps->cx;
    param->ps.cy = ps->cy;
    RKDSP_XIP_ADDR_ALIAS(param->ps.pdata, ps->pdata);

    param->pd.width = pd->width;
    param->pd.height = pd->height;
    param->pd.stride = pd->stride;
    param->pd.cx = pd->cx;
    param->pd.cy = pd->cy;
    param->pd.pdata = pd->pdata;

    param->pbg.width = pbg->width;
    param->pbg.height = pbg->height;
    param->pbg.stride = pbg->stride;
    param->pbg.cx = pbg->cx;
    param->pbg.cy = pbg->cy;
    param->pbg.pdata = pbg->pdata;

    param->angle = angle % 360;
    param->preinfo = preIndex;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    RT_ASSERT(work);
    work->id = IMG_PROCESS_ID;
    work->algo_type = DSP_ALGO_ROTATE_16BIT_FAST;
    work->param = (uint32_t)param;
    work->param_size = sizeof(struct rk_rotate_cfg);

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->ps.pdata, param->ps.stride * param->ps.height);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->pd.pdata, param->pd.stride * param->pd.height);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->pbg.pdata, param->pbg.stride * param->pbg.height);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->preinfo, param->pbg.height * 2 * sizeof(short));

    rk_imagelib_dsp_lock();
    pm_runtime_request(PM_RUNTIME_ID_USB);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    pm_runtime_release(PM_RUNTIME_ID_USB);
    rk_imagelib_dsp_unlock();
#if PRINTF_TIME
    rt_kprintf("[DSP]Rotate %ld cast %lu us(%lu cycles)\n", param->angle, (uint32_t)(work->cycles / 396), work->cycles);
#endif
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rkdsp_free(param);
    rk_dsp_work_destroy(work);
#else
#if PRINTF_TIME
    uint32_t st, et;
    st = HAL_GetTick();
#endif
    imagerotate_ARGB2RGB565Fast(ps, pbg, pd, preIndex, (float)(angle % 360));
#if PRINTF_TIME
    et = HAL_GetTick();
    rt_kprintf("[ARM]Rotate %ld cast %lu ms\n", angle % 360, et - st);
#endif
#endif
}

void rk_scale_process(struct image_st *ps, struct image_st *pd, int32_t type)
{
#if RK_SCALE_USING_DSP
    struct dsp_work *work;
    struct rk_scale_cfg *param;
    int ret;

    if (g_dsp_dev == NULL)
        return;

    param = (struct rk_scale_cfg *)rkdsp_malloc(sizeof(struct rk_scale_cfg));
    param->ps = *ps;
    RKDSP_XIP_ADDR_ALIAS(param->ps.pdata, ps->pdata);
    param->pd = *pd;
    param->src_type = type;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    RT_ASSERT(work);
    work->id = IMG_PROCESS_ID;
    work->algo_type = DSP_ALGO_SCALE_24BIT;
    work->param = (uint32_t)param;
    work->param_size = sizeof(struct rk_scale_cfg);

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->ps.pdata, param->ps.stride * param->ps.height);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rk_imagelib_dsp_lock();
    pm_runtime_request(PM_RUNTIME_ID_USB);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    pm_runtime_release(PM_RUNTIME_ID_USB);
    rk_imagelib_dsp_unlock();

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)param->pd.pdata, param->pd.stride * param->pd.height);

    rkdsp_free(param);
    rk_dsp_work_destroy(work);
#else
    fun_table[type % (sizeof(fun_table) / sizeof(scale_fun))](ps, pd);
#endif
}

void rk_lzw_decompress_dsp(uint8_t *src, uint32_t src_len, uint8_t *dst, uint32_t dst_len)
{
#if RK_LZW_USING_DSP
    struct dsp_work *work;
    struct rk_lzw_cfg *param;
    int ret;

    if (g_dsp_dev == NULL)
        return;

    param = (struct rk_lzw_cfg *)rkdsp_malloc(sizeof(struct rk_lzw_cfg));
    param->src = src;
    param->src_len = src_len;
    param->dst = dst;
    param->dst_len = dst_len;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    RT_ASSERT(work);
    work->id = 0x50000002;
    work->algo_type = DSP_ALGO_DSP_ALGO_LZW_DECOMPRESS;
    work->param = (uint32_t)param;
    work->param_size = sizeof(struct rk_lzw_cfg);

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)param->src, param->src_len);

    rk_imagelib_dsp_lock();
    pm_runtime_request(PM_RUNTIME_ID_USB);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(g_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);
    pm_runtime_release(PM_RUNTIME_ID_USB);
    rk_imagelib_dsp_unlock();

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)param->dst, param->dst_len);

    rkdsp_free(param);
    rk_dsp_work_destroy(work);
#endif
}

void rk_image_reset(struct image_st *ps, uint8_t dotbyte)
{
    if (ps->pdata == NULL)
    {
        rt_kprintf("Buffer is null\n");
        return;
    }

    for (int i = 0; i < ps->height; i++)
        memset(ps->pdata + i * ps->stride, 0x0, ps->width * dotbyte);
}

void rk_image_copy(struct image_st *ps, struct image_st *pd, uint8_t dotbyte)
{
    uint32_t h = MIN(ps->height, pd->height);
    uint32_t w = MIN(ps->width, pd->width);
    if (pd->pdata == NULL || ps->pdata == NULL)
    {
        rt_kprintf("Buffer is null\n");
        return;
    }

    for (int i = 0; i < h; i++)
        memcpy(pd->pdata + i * pd->stride, ps->pdata + i * ps->stride, w * dotbyte);
}

static inline void rgb565_to_rgb888(uint8_t *src, uint8_t *dst)
{
    uint8_t tmp[2];
    *(uint16_t *)(tmp) = *(uint16_t *)(src);

    dst[0] = tmp[0] << 3;
    dst[1] = ((tmp[1] << 5) | (tmp[0] >> 3)) & 0xfc;
    dst[2] = tmp[1] & 0xf8;
}

static inline void rgb888_to_rgb565(uint8_t *src, uint8_t *dst)
{
    uint8_t tmp[3];
    tmp[0] = src[0];
    tmp[1] = src[1];
    tmp[2] = src[2];

    dst[0] = ((tmp[1] << 3) & 0xe0) | (tmp[0] >> 3);
    dst[1] = (tmp[2] & 0xf8) | (tmp[1] >> 5);
}

void rk_image_copy_to565(struct image_st *ps, struct image_st *pd, uint8_t dotbyte)
{
    uint32_t h, w;

    if (dotbyte == 2)
    {
        rk_image_copy(ps, pd, dotbyte);
        return;
    }

    h = MIN(ps->height, pd->height);
    w = MIN(ps->width, pd->width);
    if (pd->pdata == NULL || ps->pdata == NULL)
    {
        rt_kprintf("Buffer is null\n");
        return;
    }

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            rgb888_to_rgb565(ps->pdata + i * ps->stride + j * dotbyte,
                             pd->pdata + i * pd->stride + j * 2);
        }
    }
}

void rk_image_copy_to888(struct image_st *ps, struct image_st *pd, uint8_t dotbyte)
{
    uint32_t h, w;

    if (dotbyte == 3)
    {
        rk_image_copy(ps, pd, dotbyte);
        return;
    }

    h = MIN(ps->height, pd->height);
    w = MIN(ps->width, pd->width);
    if (pd->pdata == NULL || ps->pdata == NULL)
    {
        rt_kprintf("Buffer is null\n");
        return;
    }

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            rgb565_to_rgb888(ps->pdata + i * ps->stride + j * dotbyte,
                             pd->pdata + i * pd->stride + j * 3);
        }
    }
}

void rk_imagelib_deinit(void)
{
#if RK_ROTATE_USING_DSP || RK_SCALE_USING_DSP
    if (g_dsp_dev == NULL)
        return;
    rt_device_close(g_dsp_dev);
    g_dsp_dev = NULL;
#endif
    rt_mutex_delete(work_lock);
    work_lock = NULL;
}

#if DO_ROTATE_TEST
static void do_rotate(void *arg)
{
    struct rotateimage_st ps, pd;
    rk_imagelib_init();
    int screen_w = 452;
    int screen_h = 452;
    unsigned char *p_dbuf, *p_sbuf;
    unsigned char *s_dbuf, *s_sbuf;
    unsigned char *sdata;
    uint32_t size;

    ps.width = 250;
    ps.height = 20;
    ps.stride = ps.width * 4;
    ps.cx = 35.0;
    ps.cy = 10.0;

    size = RT_ALIGN(ps.stride * ps.height, RKDSP_CACHE_LINE_SIZE) + RKDSP_CACHE_LINE_SIZE;
    p_sbuf = rt_malloc_psram(size);
    RT_ASSERT(p_sbuf != RT_NULL);
    s_sbuf = rt_malloc(size);
    RT_ASSERT(s_sbuf != RT_NULL);
    ps.pdata = (unsigned char *)RT_ALIGN((uint32_t)p_sbuf, RKDSP_CACHE_LINE_SIZE);
    sdata = (unsigned char *)RT_ALIGN((uint32_t)s_sbuf, RKDSP_CACHE_LINE_SIZE);
    // ps.pdata = (unsigned char *)rt_malloc_psram(ps.stride * ps.height);
    memset(ps.pdata, 0, ps.stride * ps.height);
    for (int y = 0; y < ps.height; y++)
    {
        for (int x = 0; x < ps.width; x++)
        {
            ps.pdata[y * ps.stride + x * 4] = 255;
            ps.pdata[y * ps.stride + x * 4 + 1] = 255;
            ps.pdata[y * ps.stride + x * 4 + 2] = 255;
        }
    }
    for (int y = 1; y < (ps.height - 1); y++)
    {
        for (int x = 1; x < (ps.width - 1); x++)
        {
            ps.pdata[y * ps.stride + x * 4 + 3] = 255;
        }
    }
    memset(sdata, 0, ps.stride * ps.height);
    for (int y = 0; y < ps.height; y++)
    {
        for (int x = 0; x < ps.width; x++)
        {
            sdata[y * ps.stride + x * 4] = 255;
            sdata[y * ps.stride + x * 4 + 1] = 255;
            sdata[y * ps.stride + x * 4 + 2] = 255;
        }
    }
    for (int y = 1; y < (ps.height - 1); y++)
    {
        for (int x = 1; x < (ps.width - 1); x++)
        {
            sdata[y * ps.stride + x * 4 + 3] = 255;
        }
    }
    pd.width = screen_w;
    pd.height = screen_h;
    pd.stride = RT_ALIGN(pd.width * 3, 4);
    size = RT_ALIGN(pd.stride * pd.height, RKDSP_CACHE_LINE_SIZE) + RKDSP_CACHE_LINE_SIZE;
    p_dbuf = rt_malloc_psram(size);
    RT_ASSERT(p_dbuf != RT_NULL);
    s_dbuf = rt_malloc(size);
    RT_ASSERT(s_dbuf != RT_NULL);
    pd.pdata = (unsigned char *)RT_ALIGN((uint32_t)p_dbuf, RKDSP_CACHE_LINE_SIZE);
    pd.cx = pd.width / 2.0;
    pd.cy = pd.height / 2.0;
    char file_name[128];

#if !RK_SCALE_USING_DSP
    uint32_t st, et;
#endif

    for (int angle = 0; angle < 360; angle += 90)
    {
        rt_kprintf("Rotate out RGB888 test\n");
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        pd.pdata = (unsigned char *)RT_ALIGN((uint32_t)s_dbuf, RKDSP_CACHE_LINE_SIZE);
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        ps.pdata = sdata;
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        pd.pdata = (unsigned char *)RT_ALIGN((uint32_t)p_dbuf, RKDSP_CACHE_LINE_SIZE);
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif

        rt_kprintf("Rotate out RGB565 test\n");
        ps.pdata = (unsigned char *)RT_ALIGN((uint32_t)p_sbuf, RKDSP_CACHE_LINE_SIZE);
        pd.pdata = (unsigned char *)RT_ALIGN((uint32_t)p_dbuf, RKDSP_CACHE_LINE_SIZE);
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process_16bit(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        pd.pdata = (unsigned char *)RT_ALIGN((uint32_t)s_dbuf, RKDSP_CACHE_LINE_SIZE);
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process_16bit(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        ps.pdata = sdata;
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process_16bit(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        pd.pdata = (unsigned char *)RT_ALIGN((uint32_t)p_dbuf, RKDSP_CACHE_LINE_SIZE);
        rt_kprintf("Rotate %d from %p to %p\n", angle, ps.pdata, pd.pdata);
        memset(pd.pdata, 0, pd.stride * pd.height);
#if !RK_SCALE_USING_DSP
        st = HAL_GetTick();
        for (int i = 0; i < 1000; i++)
#endif
            rk_rotate_process_16bit(&ps, &pd, (angle % 360));
#if !RK_SCALE_USING_DSP
        et = HAL_GetTick();
        rt_kprintf("%lu us\n", et - st);
#endif
        snprintf(file_name, sizeof(file_name), "dsp-%d.bmp%c", angle, '\0');
        SaveBmp(file_name, pd.pdata, pd.width, pd.height, 3, pd.stride);
    }
    rk_imagelib_deinit();
    // rt_free_psram(p_sbuf);
    // rt_free_psram(p_dbuf);
}

void rotate(int argc, char *argv[])
{
    rt_thread_t thread;

    thread = rt_thread_create("rotate",
                              do_rotate, NULL,
                              4096, 10, 10);
    if (thread != RT_NULL)
        rt_thread_startup(thread);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(rotate, rotate test case);
#endif
#endif
