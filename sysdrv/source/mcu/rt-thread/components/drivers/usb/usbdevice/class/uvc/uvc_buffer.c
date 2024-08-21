/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uvc_buffer.c
  * @version V0.1
  * @brief   uvc buffer process driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-01     Frank Wang      first implementation
  *
  ******************************************************************************
  */
#include <rtthread.h>
#include <rthw.h>
#include <rtservice.h>
#include <rtdevice.h>
#include "drivers/usb_common.h"
#include "drivers/usb_video.h"
#include "dma.h"
#include "uvc.h"
#include "uvc_buffer.h"
#include "hal_bsp.h"

//#define UVC_BUF_STS_DBG 1

static struct uvc_device *g_uvc_class[UVC_ID_MAX];

static void uvc_buffer_irq(struct uvc_device *uvc);

#define IS_BUFMGR_WCH2_BLK0_IRQ()                              \
    (g_uvc_class[UVC_ID_IR] != NULL &&                         \
     g_uvc_class[UVC_ID_IR]->video.fcc == V4L2_PIX_FMT_YUYV && \
     HAL_BUFMGR_WCHIntStsGet(BUFMGR_WCH2, BUFMGR_BLK0))

#define DEFINE_UVC_BUF_IRQ(ID)                             \
                                                           \
static void uvc_buffer_irq_##ID(int irq, void *param)      \
{                                                          \
    if (ID == UVC_ID_DEPTH && IS_BUFMGR_WCH2_BLK0_IRQ())   \
        uvc_buffer_irq(g_uvc_class[UVC_ID_IR]);            \
    else                                                   \
        uvc_buffer_irq(g_uvc_class[ID]);                   \
}                                                          \

DEFINE_UVC_BUF_IRQ(0); /* UVC_ID_DEPTH */
DEFINE_UVC_BUF_IRQ(1); /* UVC_ID_RGB */
DEFINE_UVC_BUF_IRQ(2); /* UVC_ID_IR */

/* invoke at interrupt context */
static void uvc_buffer_bufmgr_reset(struct uvc_video *video)
{
    HAL_BUFMGR_WCHPpBufEn(video->buf_chan, video->buf_blk, 0);
    HAL_BUFMGR_WCHIntAllEn(video->buf_chan, video->buf_blk, 0);
    HAL_BUFMGR_WCHIntStsAllClr(video->buf_chan, video->buf_blk);

    HAL_BUFMGR_WCHIntAllEn(video->buf_chan, video->buf_blk, 1);
    HAL_BUFMGR_WCHPpBufEn(video->buf_chan, video->buf_blk, 1);
}

static int
uvc_buffer_find_frame_end(struct uvc_video *video, uint32_t intsts)
{
    int idx;

    for (idx = 0; idx < UVC_BUFFER_NUM; idx++)
    {
        if (HAL_BUFMGR_WCHIntStsIsFull(intsts, idx) &&
            HAL_BUFMGR_WCHStsIsEOF(intsts, idx))
            return idx;
    }

    HAL_BUFMGR_WCHIntStsAllClr(video->buf_chan, video->buf_blk);
    return -RT_ERROR;
}

static int
uvc_buffer_find_frame_header(struct uvc_video *video, uint32_t intsts)
{
    int idx;

    for (idx = 0; idx < UVC_BUFFER_NUM; idx++)
    {
        if (HAL_BUFMGR_WCHIntStsIsFull(intsts, idx))
        {
            if (HAL_BUFMGR_WCHStsIsEOH(video->buf_chan, video->buf_blk, intsts, idx))
                return idx;
            else
                break;
        }
    }

    HAL_BUFMGR_WCHIntStsAllClr(video->buf_chan, video->buf_blk);
    return -RT_ERROR;
}

static int uvc_buffer_irq_route(int id, uint32_t chan, uint32_t blk)
{
    uint32_t intsts = HAL_BUFMGR_WCHIntStsGet(chan, blk);

    if (intsts)
    {
        rt_kprintf("WAR: uvc(%d) wch(%d) bro-blk(%d): 0x%08x\n",
                   id, chan, blk, intsts);
        rk_camera_inf_clear_buffer_manage_irq(chan, blk);
    }

    return RT_EOK;
}

static void uvc_buffer_status_dump(int id, struct uvc_video *video)
{
    int idx;

    rt_kprintf("uvc(%d) buffer: ", id);
    for (idx = 0; idx < UVC_BUFFER_NUM; idx++)
    {
        rt_kprintf("(%d-%d) ", idx, video->buffers[idx].state);
    }
    rt_kprintf("\n");
}

#ifdef UVC_BUF_STS_DBG
static void uvc_buffer_status_debug(int id, uint32_t intsts, uint32_t *val)
{
    int idx;

    RT_ASSERT(val != RT_NULL);

    *val = 0;

    rt_kprintf("DBG: uvc(%d) 0x%08x\n", id, intsts);

    if (HAL_BUFMGR_WCHIntStsIsOVFL(intsts))
        *val |= HAL_BUFMGR_WCH_INTSTS_OVFL_BIT;

    for (idx = 0; idx < UVC_BUFFER_NUM; idx++)
    {
        if (HAL_BUFMGR_WCHIntStsIsFull(intsts, idx))
        {
            /* intsts_full, full_flag, first_flag, last_flag */
            *val |= HAL_BUFMGR_WCH_INTSTS_FULL_BIT(idx) |
                    HAL_BUFMGR_WCH_STS_FULL_BIT(idx) |
                    HAL_BUFMGR_WCH_STS_EOH_BIT(idx) |
                    HAL_BUFMGR_WCH_STS_EOF_BIT(idx);
            break;
        }
    }
}
#endif

static void uvc_buffer_abort(int id, struct uvc_video *video)
{
    struct uvc_buffer *pbuf = video->next_buf_to_drain;

    /*
     * First, disable ping-pong buffer interrupt for write channel,
     * and then, mark error flag into next drain buffer for UVC header.
     */
    HAL_BUFMGR_WCHIntAllEn(video->buf_chan, video->buf_blk, 0);
    HAL_BUFMGR_WCHIntStsAllClr(video->buf_chan, video->buf_blk);

    /* mark error for the current and next buffer */
    pbuf->err = 1;
    pbuf->next->err = 1;

    uvc_buffer_status_dump(id, video);
}

static void uvc_buffer_irq(struct uvc_device *uvc)
{
    struct uvc_video *video = &uvc->video;
    struct uvc_buffer *pbuf;
    volatile uint32_t intsts;
    uint32_t val = 0;
    int idx, ret;

    RT_ASSERT(uvc != RT_NULL);

    rt_interrupt_enter();

    intsts = HAL_BUFMGR_WCHIntStsGet(video->buf_chan, video->buf_blk);
    if (!intsts && uvc->id == UVC_ID_DEPTH)
    {
        /*
         * The hardware irq is shared with two block in one channel of
         * the buffer manager (BM).
         *
         * For MJPEG format, the another block is used for MJPEG encoder, so
         * route the irq to another block when the current uvc block interrupt
         * status is zero, and the BM driver will handle to process it.
         *
         * For YUV format, the another block may also be used for raw data
         * output and the interrupt should handle in its own ISR, so just
         * return here (only for depth and ir in current).
         */
        if (g_uvc_class[UVC_ID_IR] != NULL &&
            g_uvc_class[UVC_ID_IR]->video.fcc == V4L2_PIX_FMT_MJPEG)
            uvc_buffer_irq_route(uvc->id, video->buf_chan, video->buf_blk ^ 1);

        goto out;
    }

    if (!uvc->video.enabled)
    {
        rt_kprintf("WAR: uvc(%d) video unopened 0x%08x\n", uvc->id, intsts);
        HAL_BUFMGR_WCHIntStsAllClr(video->buf_chan, video->buf_blk);
        goto out;
    }

#ifdef UVC_BUF_STS_DBG
    uvc_buffer_status_debug(uvc->id, intsts, &val);
    goto out;
#endif

    if (HAL_BUFMGR_WCHIntStsIsOVFL(intsts))
    {
        uvc_buffer_abort(uvc->id, video);
        rt_kprintf("ERR: uvc(%d) buffer overflow irq 0x%08x\n", uvc->id, intsts);
        goto out;
    }

    /* do reset the buffer manager channel at the end frame time */
    if (video->reset_stream)
    {
        idx = uvc_buffer_find_frame_end(video, intsts);
        if (idx >= 0)
        {
            uvc_buffer_bufmgr_reset(video);
            video->reset_stream = 0; /* clear */
        }

        goto out;
    }

    /* find the index of next to fill buffer */
    if (video->first_stream)
    {
        idx = uvc_buffer_find_frame_header(video, intsts);
        if (idx < 0)
            goto out;

        video->next_buf_to_drain = &video->buffers[idx];
        video->next_buf_to_fill  = &video->buffers[idx];
        video->first_stream = 0; /* clear */
        RT_DEBUG_LOG(UVC_BUF_DEBUG, ("INF: uvc(%d) buffer(%d) is first stream\n", uvc->id, idx));
    }
    else
    {
        idx = video->next_buf_to_fill->next->index;
        if (!HAL_BUFMGR_WCHIntStsIsFull(intsts, idx))
        {
            rt_kprintf("WAR: uvc(%d) buffer is not next to fill, next %d, status 0x%08x\n",
                       uvc->id, idx, intsts);
            goto out;
        }

        RT_DEBUG_LOG(UVC_BUF_DEBUG, ("INF: uvc(%d) buffer(%d) full irq\n", uvc->id, idx));
        video->next_buf_to_fill = video->next_buf_to_fill->next;
    }

    pbuf = video->next_buf_to_fill;
    if (pbuf->state != BUF_STATE_EMPTY)
    {
        uvc_buffer_abort(uvc->id, video);
        rt_kprintf("ERR: uvc(%d) buffer(%d) overflow 0x%08x\n",
                   uvc->id, pbuf->index, intsts);
        goto out;
    }

    /* update buffer status from hw register */
    pbuf->state = BUF_STATE_FULL;
    pbuf->eoh = HAL_BUFMGR_WCHStsIsEOH(video->buf_chan, video->buf_blk, intsts, idx);
    pbuf->eof = HAL_BUFMGR_WCHStsIsEOF(intsts, idx);
    pbuf->length = !pbuf->eof ?
                   video->buf_size : /* full buffer assert */
                   HAL_BUFMGR_WCHEndBufLenGet(video->buf_chan, video->buf_blk);

    /* get jpeg header length */
    if (pbuf->eoh && video->fcc == V4L2_PIX_FMT_MJPEG)
    {
        video->buf_jpghdr.length = rk_camera_inf_get_enc_length(uvc->id);
        video->buf_jpghdr.state = BUF_STATE_FULL;
        RT_DEBUG_LOG(UVC_BUF_DEBUG, ("uvc(%d), jpghdr len=%d\n", uvc->id, video->buf_jpghdr.length));
    }

    if (video->buf_pending)
    {
        ret = uvc_send_video_data(uvc);
        if (ret)
        {
            rt_kprintf("ERR: uvc(%d) pending buffer(%d-%d) fail to send\n",
                       uvc->id, video->next_buf_to_drain->index, video->next_buf_to_drain->state);
            rt_kprintf("INF: uvc(%d) next buffer(%d-%d) to fill\n",
                       uvc->id, video->next_buf_to_fill->index, video->next_buf_to_fill->state);
        }
        else
        {
            video->buf_pending = 0; /* clear */
        }
    }

    /* intsts_full, first_flag, last_flag */
    val = HAL_BUFMGR_WCH_INTSTS_FULL_BIT(idx) |
          HAL_BUFMGR_WCH_STS_EOH_BIT(idx) |
          HAL_BUFMGR_WCH_STS_EOF_BIT(idx);

out:
    /* clear wch status */
    if (val)
        HAL_BUFMGR_WCHIntStsClr(video->buf_chan, video->buf_blk, val);

    /* leave interrupt */
    rt_interrupt_leave();
}

static inline struct uvc_device *video_to_uvc(struct uvc_video *v)
{
    return rt_container_of(v, struct uvc_device, video);
}

static inline void
uvc_buffer_var_init(struct uvc_video *video, int wch, int blk, int irq)
{
    RT_ASSERT(video != RT_NULL);

    video->buf_chan = wch;
    video->buf_blk  = blk;
    video->buf_irq  = irq;
}

rt_err_t uvc_buffer_assign(struct uvc_device *uvc)
{
    int i;
    uint32_t addr;
    struct uvc_video *video = &uvc->video;

    /*
     * uvc buffer assign
     *
     * buffer[0] --> buffer[1] --> buffer[2] --> buffer[3]
     *   ↑                                          |
     *   |__________________________________________|
     */

    for (i = 0; i < UVC_BUFFER_NUM; i++)
    {
        addr = HAL_BUFMGR_WCHBufAddrGet(video->buf_chan, video->buf_blk, i);
        if (!addr)
        {
            rt_kprintf("ERR: uvc(%d) buffer(%d) addr is NULL\n", uvc->id, i);
            return RT_ERROR;
        }

        video->buffers[i].data = (uint8_t *)(addr - UVC_HEADER_SIZE);
        rt_kprintf("INF: uvc(%d) buffer(%d) addr 0x%08x\n",
                   uvc->id, i, (uint32_t)video->buffers[i].data);

        video->buffers[i].index = i;

        if (i != UVC_BUFFER_NUM - 1)
            video->buffers[i].next = &video->buffers[i + 1];
        else
            video->buffers[i].next = &video->buffers[0];
    }

    /* assign jpghdr buffer */
    if (video->fcc == V4L2_PIX_FMT_MJPEG)
    {
        video->buf_jpghdr.data = rk_camera_inf_get_enc_head_addr(uvc->id);
        if (!video->buf_jpghdr.data)
        {
            rt_kprintf("ERR: uvc(%d) mjpeg header addr is NULL\n", uvc->id);
            return RT_ERROR;
        }

        rt_kprintf("INF: uvc(%d) mjpeg header addr 0x%08x\n",
                   uvc->id, (uint32_t)video->buf_jpghdr.data);
    }

    video->buf_size = HAL_BUFMGR_WCHBufSizeGet(video->buf_chan, video->buf_blk);
    if (!video->buf_size)
    {
        rt_kprintf("ERR: uvc(%d) buffer size is zero\n", uvc->id);
        return RT_ERROR;
    }

    rt_kprintf("INF: uvc(%d) buffer size %d\n", uvc->id, video->buf_size);
    return RT_EOK;
}

static int
uvc_buffer_encode_header(struct uvc_video *video, uint8_t *data)
{
    int i;
    struct uvc_buffer *pbuf = video->next_buf_to_drain;
    struct uvc_device *uvc = video_to_uvc(video);

    /* Stream Header Format
     * -----------------------------------------------
     *|                 Header Length                 |
     * -----------------------------------------------
     *| EOH | ERR | STI | RES | SCR | PTS | EOF | FID |
     * -----------------------------------------------
     *|                  PTS[7:0]                     |
     * -----------------------------------------------
     *|                  PTS[15:8]                    |
     * -----------------------------------------------
     */
    data[0] = UVC_HEADER_SIZE;
    data[1] = UVC_STREAM_EOH | video->fid;

    /* encode for mjpeg header, do not tag eoh/eof/err flag */
    if (video->buf_jpghdr.state == BUF_STATE_BUSY)
        goto out;

    /* tag end of frame */
    if (pbuf && pbuf->eof)
        data[1] |= UVC_STREAM_EOF;

    /* tag error of frame */
    if (pbuf && pbuf->err)
    {
        data[1] |= UVC_STREAM_ERR;
        rt_kprintf("ERR: uvc(%d) buffer(%d) frame error\n", uvc->id, pbuf->index);
    }

out:
    /* tag reserved */
    for (i = 2; i < data[0]; i++)
        data[i] = 0x00;

    return data[0];
}

uint32_t uvc_buffer_pump_jpghdr(struct uvc_video *video)
{
    uint32_t nbytes = 0;
    struct uvc_buffer *pbuf = &video->buf_jpghdr;

    if (pbuf->state != BUF_STATE_FULL)
        return 0;

    pbuf->state = BUF_STATE_BUSY;

    /* 1st, insert uvc header */
    nbytes = uvc_buffer_encode_header(video, video->buf_jpghdr.data);

    /* 2nd, plus the payload length */
    nbytes += pbuf->length;

    /* Last, assign to ep in buffer */
    video->ep_in->buffer = pbuf->data;
    video->req_len = nbytes;

    RT_DEBUG_LOG(UVC_TXD_DEBUG, ("INF: uvc jpghdr length: %d\n", pbuf->length));

    return nbytes;
}

uint32_t uvc_buffer_pump(struct uvc_video *video)
{
    uint32_t nbytes = 0;
    rt_base_t level;
    struct uvc_buffer *pbuf = video->next_buf_to_drain;

    RT_ASSERT(pbuf != RT_NULL);

    /* 1st, mark buffer status to busy */
    level = rt_hw_interrupt_disable();
    pbuf->state = BUF_STATE_BUSY;
    rt_hw_interrupt_enable(level);

    /* 2nd, insert uvc header before data buffer */
    nbytes = uvc_buffer_encode_header(video, pbuf->data);

    /* 3rd, plus the payload length */
    nbytes += pbuf->length;

    /* Last, assign to ep in buffer */
    video->ep_in->buffer = (uint8_t *)pbuf->data;
    video->req_len = nbytes;

    if (pbuf && pbuf->eof)
    {
        video->fid ^= UVC_STREAM_FID;
//        video->req_zero = 1;
    }

    RT_DEBUG_LOG(UVC_TXD_DEBUG, ("INF: uvc buffer(%d-%d) length: %d\n",
                                 pbuf->index, pbuf->state, video->req_len));

    return nbytes;
}

void uvc_buffer_flush(struct uvc_video *video)
{
    int i;
    rt_base_t level;

    level = rt_hw_interrupt_disable();

    for (i = 0; i < UVC_BUFFER_NUM; i++)
    {
        video->buffers[i].eoh = 0;
        video->buffers[i].eof = 0;
        video->buffers[i].err = 0;
        video->buffers[i].length = 0;
        video->buffers[i].state = BUF_STATE_EMPTY;
    }

    HAL_BUFMGR_WCHIntStsAllClr(video->buf_chan, video->buf_blk);

    rt_hw_interrupt_enable(level);
}

rt_err_t
uvc_buffer_init(struct uvc_device *uvc)
{
    struct uvc_video *video = &uvc->video;
    rt_isr_handler_t handler;

    RT_ASSERT(uvc != RT_NULL);
    RT_ASSERT(uvc->id >= UVC_ID_DEPTH && uvc->id < UVC_ID_MAX);

    switch (uvc->id)
    {
    case UVC_ID_DEPTH:
        uvc_buffer_var_init(video, BUFMGR_WCH2, BUFMGR_BLK1, BUFMGR_WCH_IRQ_WCH2);
        handler = (rt_isr_handler_t)uvc_buffer_irq_0;
        break;
    case UVC_ID_RGB:
        if (video->fcc == V4L2_PIX_FMT_MJPEG)
            uvc_buffer_var_init(video, BUFMGR_WCH1, BUFMGR_BLK0, BUFMGR_WCH_IRQ_WCH1);
        else
            uvc_buffer_var_init(video, BUFMGR_WCH0, BUFMGR_BLK0, BUFMGR_WCH_IRQ_WCH0);
        handler = (rt_isr_handler_t)uvc_buffer_irq_1;
        break;
    case UVC_ID_IR:
        if (video->fcc == V4L2_PIX_FMT_MJPEG)
        {
            uvc_buffer_var_init(video, BUFMGR_WCH3, BUFMGR_BLK0, BUFMGR_WCH_IRQ_WCH3);
            handler = (rt_isr_handler_t)uvc_buffer_irq_2;
        }
        else
        {
            uvc_buffer_var_init(video, BUFMGR_WCH2, BUFMGR_BLK0, BUFMGR_WCH_IRQ_WCH2);
            handler = (rt_isr_handler_t)uvc_buffer_irq_0; /* share with depth */
        }
        break;
    default:
        rt_kprintf("ERR: %s invalid uvc id\n", __func__);
        return RT_ERROR;
    }

    /* buffer manager irq install */
    rt_hw_interrupt_install(video->buf_irq, handler, NULL, NULL);
    rt_hw_interrupt_umask(video->buf_irq);

    g_uvc_class[uvc->id] = uvc;

    return RT_EOK;
}
