/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_jpege.c
  * @version V0.0.1
  * @brief   jpeg enc device abstract for swallow
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-12     jpeg enc Team   first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
//#ifdef RT_USING_JPEG_ENC
#include "drv_clock.h"
#include "drv_jpege.h"


#define JPEGE_FLOW_NO_BLOCK
//#define JPEG_FLOW_BLOCK
#define JPEG_TIMER_1MS (PLL_INPUT_OSC_RATE/1000)

static rt_err_t jpeg_enc_start(struct rt_jpege_device *dev);
static rt_err_t jpeg_enc_reset(struct rt_jpege_device *dev);
static void jpeg_stop_timer(struct rt_jpege_device *dev);
static void jpeg_start_timer(struct rt_jpege_device *dev);

#define DEFINE_JPEGE(ID)                            \
static void jpege_irq##ID(int vector, void *param); \
static void jpege_timer_handle##ID(int vector, void *param);\
static struct rt_jpege_device rk_jpege_dev##ID =    \
{                                                   \
    .ctx            = RT_NULL,                      \
    .irq_num        = JPEG##ID##_IRQn,              \
    .name           = "jpge_"#ID,                   \
    .irq_handler    = jpege_irq##ID,                \
    .dev_id         = ID,                           \
    .jpege_done_sem = RT_NULL,                      \
    .status         = STATUS_IDLE,                  \
    .timer_irq_num  = TIMER##ID##_IRQn,             \
    .timer_handler  = jpege_timer_handle##ID,       \
    .timer          = TIMER##ID,                    \
};                                                  \
                                                    \
static void jpege_irq##ID(int vector, void *param)  \
{                                                   \
    rt_interrupt_enter();                           \
    jpeg_stop_timer(&rk_jpege_dev##ID);             \
    jpege_end(rk_jpege_dev##ID.ctx);                \
    jpege_clear_interrupt(rk_jpege_dev##ID.ctx);    \
    if(STATUS_CLOSE == rk_jpege_dev##ID.status)     \
    {                                               \
        rt_sem_release(rk_jpege_dev##ID.jpege_done_sem);\
    }else                                           \
    {                                               \
        jpeg_enc_start(&rk_jpege_dev##ID);          \
        jpeg_start_timer(&rk_jpege_dev##ID);        \
    }                                               \
    rt_interrupt_leave();                           \
}                                                   \
                                                    \
static void jpege_timer_handle##ID(int vector, void *param)  \
{                                                   \
    rt_interrupt_enter();                           \
    rt_hw_interrupt_mask(rk_jpege_dev##ID.timer_irq_num); \
    jpege_err("jpeg enc timeout, reset enc\n");     \
    jpeg_enc_reset(&rk_jpege_dev##ID);              \
    if(STATUS_CLOSE == rk_jpege_dev##ID.status)     \
        rt_sem_release(rk_jpege_dev##ID.jpege_done_sem);\
    rt_hw_interrupt_umask(rk_jpege_dev##ID.timer_irq_num); \
    rt_interrupt_leave();                           \
}

#ifdef RT_USING_JPEG_ENC0
DEFINE_JPEGE(0)
#endif

#ifdef RT_USING_JPEG_ENC1
DEFINE_JPEGE(1)
#endif

static struct rt_jpege_device *rk_jpege_devices[] =
{
#ifdef RT_USING_JPEG_ENC0
    &rk_jpege_dev0,
#endif
#ifdef RT_USING_JPEG_ENC1
    &rk_jpege_dev1,
#endif
    RT_NULL,
};

static rt_err_t rk_cal_buf_size(struct jpege_cfg *cfg)
{
    if (cfg->format == MPP_FMT_YUV420SP || cfg->format == MPP_FMT_YUV420P)
    {
        cfg->buffer_size = cfg->hor_stride * cfg->ver_stride * 3 / 2;
    }
    else if (cfg->format == MPP_FMT_YUV422SP || cfg->format == MPP_FMT_YUV422P)
    {
        cfg->buffer_size = cfg->hor_stride * cfg->ver_stride * 2;
    }
    else if (cfg->format == MPP_FMT_YUV400)
    {
        cfg->buffer_size = cfg->hor_stride * cfg->ver_stride;
    }
    return RT_EOK;
}

static rt_err_t update_header_and_regs(void *ctx)
{
    struct jpege_ctx *jpege_ctx = (struct jpege_ctx *)ctx;
    JpegeBits *bits = jpege_ctx->bits;
    struct jpege_cfg *cfg = jpege_ctx->cfg;

    if (cfg->cfg_change)
    {
        jpege_dbg("cfg change need update\n");
        rk_cal_buf_size(cfg);
        jpege_gen_regs(jpege_ctx);
        cfg->cfg_change = 0;
    }

    return RT_EOK;
}

static rt_err_t jpeg_enc_reset(struct rt_jpege_device *dev)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)dev->ctx;
    if (ctx == RT_NULL)
    {
        jpege_dbg("ctx is null, has been free\n");
        return RT_ERROR;
    }
    JpegeBits *bits = ctx->bits;
    struct jpege_cfg *cfg = ctx->cfg;

    jpeg_stop_timer(dev);

    jpege_reset(ctx);
    jpege_dbg("restart jpeg enc\n");
    jpeg_enc_start(dev);
    jpeg_start_timer(dev);
    return RT_EOK;
}

static void jpeg_stop_timer(struct rt_jpege_device *dev)
{
    HAL_TIMER_Stop_IT(dev->timer);
    HAL_TIMER_ClrInt(dev->timer);
    HAL_NVIC_ClearPendingIRQ(dev->timer_irq_num);
}

static void jpeg_start_timer(struct rt_jpege_device *dev)
{
    HAL_TIMER_SetCount(dev->timer, 500 * JPEG_TIMER_1MS);
    HAL_TIMER_Start_IT(dev->timer);
    rt_hw_interrupt_umask(dev->timer_irq_num);
}

static rt_err_t jpeg_enc_start(struct rt_jpege_device *dev)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)dev->ctx;
    if (ctx == RT_NULL)
        return RT_ERROR;
    struct jpege_cfg *cfg = ctx->cfg;
    JpegeBits bits = ctx->bits;

    jpege_dbg("enc start enter\n");
    jpege_bits_setup(bits, cfg->jpeg_header_addr + 4, 1024);
    jpeg_write_header(ctx);
    jpege_dbg("[%d]jpege header write %d bytes done\n",
              dev->dev_id, jpege_bits_get_bytepos(jpege_ctx->bits));
    if (cfg->cfg_change)
    {
        update_header_and_regs(ctx);
    }

    jpege_start(ctx);
    dev->status = STATUS_START;
    jpege_dbg("enc start leave\n");
    return RT_EOK;
}

static rt_err_t rk_jpege_init(rt_device_t dev)
{
    struct rt_jpege_device *jpeg_dev = (struct rt_jpege_device *)dev->user_data;
    if (jpeg_dev->jpege_done_sem == RT_NULL)
        jpeg_dev->jpege_done_sem = rt_sem_create(jpeg_dev->name, 0, RT_IPC_FLAG_FIFO);
    return RT_EOK;
}

static rt_err_t rk_jpege_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct rt_jpege_device *jpeg_dev = (struct rt_jpege_device *)dev->user_data;
    struct clk_gate *clkgate;

    if (RT_NULL == jpeg_dev->ctx)
    {
        jpege_init(&jpeg_dev->ctx);
        //clk_enable(jpeg_dev->aclk);
        clk_enable(jpeg_dev->hclk);
        clk_enable(jpeg_dev->bus_pll);
        clk_enable(jpeg_dev->bus_niu);
    }
    jpege_dbg("open %s jpeg_dev %p\n", jpeg_dev->name, jpeg_dev);

    jpeg_dev->ctx->cfg->jpeg_dev_id = jpeg_dev->dev_id;
    if (STATUS_IDLE == jpeg_dev->status)
    {
        jpeg_dev->status = STATUS_OPEN;
    }
    else
    {
        jpege_dbg("status %d not idle\n", jpeg_dev->status);
    }

    return RT_EOK;
}

static rt_err_t rk_jpege_close(rt_device_t dev)
{
    rt_err_t ret = RT_EOK;
    struct rt_jpege_device *jpeg_dev = (struct rt_jpege_device *)dev->user_data;
    struct clk_gate *clkgate;

    jpege_dbg("jpege close enter\n");
    jpeg_start_timer(jpeg_dev);
    if (jpeg_dev->status == STATUS_START)
    {
        jpeg_dev->status = STATUS_CLOSE;
        ret = rt_sem_take(jpeg_dev->jpege_done_sem, RT_WAITING_FOREVER);
        if (RT_EOK != ret)
            jpege_dbg("sem take failed\n");
    }

    jpeg_stop_timer(jpeg_dev);

    if (NULL != jpeg_dev->ctx)
    {
        clk_disable(jpeg_dev->bus_niu);
        clk_disable(jpeg_dev->bus_pll);
        clk_disable(jpeg_dev->hclk);
        clk_disable(jpeg_dev->aclk);
        jpege_deinit(jpeg_dev->ctx);
    }
    jpeg_dev->ctx = NULL;
    jpeg_dev->status = STATUS_IDLE;
    jpege_dbg("jpege close leave\n");

    return RT_EOK;
}

static rt_err_t rk_jpege_control(rt_device_t dev, int cmd, void *arg)
{
    struct rt_jpege_device *jpeg_dev = (struct rt_jpege_device *)dev->user_data;
    struct jpege_ctx *ctx = jpeg_dev->ctx;

    switch (cmd)
    {
    case CMD_SET_CFG:
    {
        struct jpeg_cfg *cfg = (struct jpeg_cfg *)arg;
        struct jpege_cfg *dst = ctx->cfg;
        rt_hw_interrupt_mask(jpeg_dev->irq_num);
        dst->width              = cfg->width;
        dst->height             = cfg->height;
        dst->hor_stride         = cfg->hor_stride;
        dst->ver_stride         = cfg->ver_stride;
        dst->q_factor           = cfg->q_factor;
        dst->yuv_y_addr         = cfg->yuv_y_addr;
        dst->yuv_u_addr         = cfg->yuv_u_addr;
        dst->yuv_v_addr         = cfg->yuv_v_addr;
        dst->jpeg_header_addr   = cfg->jpeg_header_addr;
        dst->output_addr        = cfg->output_addr;
        dst->format             = cfg->format;
        dst->cfg_change         = 1;
        rt_hw_interrupt_umask(jpeg_dev->irq_num);
        break;
    }
    case CMD_JPEG_ENC_START:
    {
        struct jpege_cfg *cfg = ctx->cfg;
        rt_err_t ret = RT_EOK;
        jpege_dbg("start enter\n");
        jpeg_enc_start(jpeg_dev);
        rt_hw_interrupt_umask(jpeg_dev->irq_num);
#ifdef JPEG_FLOW_BLOCK
        jpege_dbg("wait irq num %d sem %p\n", jpeg_dev->irq_num, jpeg_dev->jpege_done_sem);
        ret = rt_sem_take(jpeg_dev->jpege_done_sem, RT_WAITING_FOREVER);
        if (RT_EOK != ret)
            jpege_dbg("sem take failed\n");
#endif
        jpege_dbg("start leave\n");
        break;
    }
    case CMD_GET_STREAM_LEN:
    {
        *(uint32_t *)arg = ctx->cfg->stream_len;
        break;
    }
    case CMD_SET_JPEG_HEAD_TIME_STAMP:
    {
        jpeg_write_header_time_stamp((void *)ctx, arg);
        break;
    }

    case CMD_SET_JPEG_ACLK:
    {
        clk_enable(jpeg_dev->aclk);
        break;
    }
    default:
        break;
    }
    return RT_EOK;

}

const static struct rt_device_ops jpege_ops =
{
    rk_jpege_init,
    rk_jpege_open,
    rk_jpege_close,
    RT_NULL,
    RT_NULL,
    rk_jpege_control
};

rt_err_t rk_jpege_register(struct rt_jpege_device *dev,
                           const char *name,
                           rt_uint32_t flag,
                           void *data)
{
    rt_uint32_t ret;
    struct rt_device *device;

    RT_ASSERT(dev != RT_NULL);

    device = &(dev->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &jpege_ops;
#else
    device->init        = rk_jpege_init;
    device->open        = rk_jpege_open;
    device->close       = rk_jpege_close;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rk_jpege_control;
#endif
    device->user_data   = data;

    if (dev->dev_id == 0)
    {
        dev->aclk = get_clk_gate_from_id(ACLK_JPEG_ENC0_GATE);
        dev->hclk = get_clk_gate_from_id(HCLK_JPEG_ENC0_GATE);
        dev->bus_pll = get_clk_gate_from_id(ACLK_JPEG_BUS_PLL_GATE);
        dev->bus_niu = get_clk_gate_from_id(ACLK_JPEG_BUS_NIU_GATE);
    }
    else
    {
        dev->aclk = get_clk_gate_from_id(ACLK_JPEG_ENC1_GATE);
        dev->hclk = get_clk_gate_from_id(HCLK_JPEG_ENC1_GATE);
        dev->bus_pll = get_clk_gate_from_id(ACLK_JPEG_BUS_PLL_GATE);
        dev->bus_niu = get_clk_gate_from_id(ACLK_JPEG_BUS_NIU_GATE);
    }

    ret = rt_device_register(device, name, flag);
    jpege_dbg("register %s done\n", dev->name);

    return ret;
}

static rt_err_t rk_jpege_probe(struct rt_jpege_device *dev)
{
    rt_hw_interrupt_install(dev->irq_num, dev->irq_handler, dev, dev->name);
    rt_hw_interrupt_mask(dev->irq_num);

    rt_hw_interrupt_install(dev->timer_irq_num, dev->timer_handler, RT_NULL, dev->name);
    rt_hw_interrupt_mask(dev->timer_irq_num);
    HAL_TIMER_Init(dev->timer, TIMER_USER_DEFINED);

    return rk_jpege_register(dev, dev->name, RT_DEVICE_FLAG_RDWR, dev);
}

int rt_jpege_init(void)
{
    int i = 0;
    struct rt_jpege_device **dev = RT_NULL;

    for (dev = rk_jpege_devices; *dev != RT_NULL; dev++)
    {
        rk_jpege_probe(*dev);
    }
    return 0;
}

INIT_DEVICE_EXPORT(rt_jpege_init);
//#endif
