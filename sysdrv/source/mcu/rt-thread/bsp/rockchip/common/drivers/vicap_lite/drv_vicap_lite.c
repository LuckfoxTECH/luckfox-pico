/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vicap.c
  * @version V0.0.1
  * @brief   video capture device abstract for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-22     ISP Team      first implementation
  *
  ******************************************************************************
  */

#if defined(__RT_THREAD__)
#include "drv_vicap_lite.h"
#include "swallow.h"
#include "drv_clock.h"
#elif defined(__RK_OS__)
#include "driver/drv_vicap_lite.h"
#endif

#ifdef VICAP_LITE_MODULE_COMPILED
#define VICAP_LITE_USE_IRQ 0

#define VIP_DEBUG 0
#if VIP_DEBUG
#include <stdio.h>
#define VIP_DBG(...) rt_kprintf("[VICAP_LITE]:");rt_kprintf(__VA_ARGS__)
#else
#define VIP_DBG(...)
#endif

#define VICAP_LITE_DT(x) ((x & 0x3f) << 10)
#define VICAP_LITE_VC(x) ((x & 0x3) << 8)

#if defined(__RT_THREAD__)

struct rk_vicap_lite_device vicap_lite_instance;

static void rk_vicap_lite_irq(int irq, void *param)
{

    struct rk_vicap_lite_device *vicap_lite_dev = &vicap_lite_instance;
    rt_interrupt_enter();
#if VICAP_LITE_USE_IRQ
    rt_kprintf("(%s) enter %x \n", __FUNCTION__, *(uint32_t *)(VICAP_LITE_BASE + VICAP_LITE_MIPI_INTSTAT_OFFSET));
#endif
    /* clr all irq bits */
    *(uint32_t *)(VICAP_LITE_BASE + VICAP_LITE_MIPI_INTSTAT_OFFSET) = 0x1fffff;

    rt_interrupt_leave();
}

rt_err_t rk_rtthread_vicap_lite_control(struct rt_device *dev,
                                        int cmd,
                                        void *args)
{
    struct rk_vicap_lite_cfg *cfg = (struct rk_vicap_lite_cfg *)args;
    struct rk_vicap_lite_device *vip = (struct rk_vicap_lite_device *)dev;
    struct VICAP_LITE_REG *pReg = vip->p_reg;
    rt_err_t ret;

    VIP_DBG("(%s) enter \n", __FUNCTION__);

    uint32_t ID_CTRL0 = VICAP_LITE_VC(cfg->vc) | VICAP_LITE_DT(cfg->dt) | 1;
    uint32_t ID_CTRL1 = (cfg->height << 16) | cfg->width;

    if (cmd == VICAP_LITE_ENABLE)
    {
        if (cfg->mipi_id == VICAP_LITE_MIPI_ID0)
        {
            pReg->MIPI_ID0_CTRL0 = ID_CTRL0;
            pReg->MIPI_ID0_CTRL1 = ID_CTRL1;
            pReg->MIPI_FRAME0_ADDR_Y_ID0 = cfg->addr_y[0];
            pReg->MIPI_FRAME1_ADDR_Y_ID0 = cfg->addr_y[1];
            pReg->MIPI_FRAME0_ADDR_UV_ID0 = cfg->addr_uv[0];
            pReg->MIPI_FRAME1_ADDR_UV_ID0 = cfg->addr_uv[1];
            pReg->MIPI_FRAME0_VLW_Y_ID0 = cfg->stride_y;
            pReg->MIPI_FRAME1_VLW_Y_ID0 = cfg->stride_y;
            pReg->MIPI_FRAME0_VLW_UV_ID0 = cfg->stride_uv;
            pReg->MIPI_FRAME1_VLW_UV_ID0 = cfg->stride_uv;
#if VICAP_LITE_USE_IRQ
            pReg->MIPI_INTEN |= VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID0_INTST_MASK |
                                VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID0_INTST_MASK;
#endif
        }
        else if (cfg->mipi_id == VICAP_LITE_MIPI_ID1)
        {
            pReg->MIPI_ID1_CTRL0 = ID_CTRL0;
            pReg->MIPI_ID1_CTRL1 = ID_CTRL1;
            pReg->MIPI_FRAME0_ADDR_Y_ID1 = cfg->addr_y[0];
            pReg->MIPI_FRAME1_ADDR_Y_ID1 = cfg->addr_y[1];
            pReg->MIPI_FRAME0_ADDR_UV_ID1 = cfg->addr_uv[0];
            pReg->MIPI_FRAME1_ADDR_UV_ID1 = cfg->addr_uv[1];
            pReg->MIPI_FRAME0_VLW_Y_ID1 = cfg->stride_y;
            pReg->MIPI_FRAME1_VLW_Y_ID1 = cfg->stride_y;
            pReg->MIPI_FRAME0_VLW_UV_ID1 = cfg->stride_uv;
            pReg->MIPI_FRAME1_VLW_UV_ID1 = cfg->stride_uv;
#if VICAP_LITE_USE_IRQ
            pReg->MIPI_INTEN |= VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID1_INTST_MASK |
                                VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID1_INTST_MASK;
#endif
        }
        else if (cfg->mipi_id == VICAP_LITE_MIPI_ID2)
        {
            pReg->MIPI_ID2_CTRL0 = ID_CTRL0;
            pReg->MIPI_ID2_CTRL1 = ID_CTRL1;
            pReg->MIPI_FRAME0_ADDR_Y_ID2 = cfg->addr_y[0];
            pReg->MIPI_FRAME1_ADDR_Y_ID2 = cfg->addr_y[1];
            pReg->MIPI_FRAME0_ADDR_UV_ID2 = cfg->addr_uv[0];
            pReg->MIPI_FRAME1_ADDR_UV_ID2 = cfg->addr_uv[1];
            pReg->MIPI_FRAME0_VLW_Y_ID2 = cfg->stride_y;
            pReg->MIPI_FRAME1_VLW_Y_ID2 = cfg->stride_y;
            pReg->MIPI_FRAME0_VLW_UV_ID2 = cfg->stride_uv;
            pReg->MIPI_FRAME1_VLW_UV_ID2 = cfg->stride_uv;
#if VICAP_LITE_USE_IRQ
            pReg->MIPI_INTEN |= VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID2_INTST_MASK |
                                VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID2_INTST_MASK;
#endif
        }
        else
        {
            pReg->MIPI_ID3_CTRL1 = ID_CTRL0;
            pReg->MIPI_ID3_CTRL1 = ID_CTRL1;
            pReg->MIPI_FRAME0_ADDR_Y_ID3 = cfg->addr_y[0];
            pReg->MIPI_FRAME1_ADDR_Y_ID3 = cfg->addr_y[1];
            pReg->MIPI_FRAME0_ADDR_UV_ID3 = cfg->addr_uv[0];
            pReg->MIPI_FRAME1_ADDR_UV_ID3 = cfg->addr_uv[1];
            pReg->MIPI_FRAME0_VLW_Y_ID3 = cfg->stride_y;
            pReg->MIPI_FRAME1_VLW_Y_ID3 = cfg->stride_y;
            pReg->MIPI_FRAME0_VLW_UV_ID3 = cfg->stride_uv;
            pReg->MIPI_FRAME1_VLW_UV_ID3 = cfg->stride_uv;
#if VICAP_LITE_USE_IRQ
            pReg->MIPI_INTEN |= VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID3_INTST_MASK |
                                VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID3_INTST_MASK;
#endif
        }
    }
    else if (cmd == VICAP_LITE_DISABLE)
    {
        if (cfg->mipi_id == VICAP_LITE_MIPI_ID0)
        {
            pReg->MIPI_ID0_CTRL0 = 0;
        }
        else if (cfg->mipi_id == VICAP_LITE_MIPI_ID1)
        {
            pReg->MIPI_ID1_CTRL0 = 0;
        }
        else if (cfg->mipi_id == VICAP_LITE_MIPI_ID2)
        {
            pReg->MIPI_ID2_CTRL0 = 0;
        }
        else if (cfg->mipi_id == VICAP_LITE_MIPI_ID3)
        {
            pReg->MIPI_ID3_CTRL0 = 0;
        }
    }

    VIP_DBG("(%s) exit \n", __FUNCTION__);
    return ret;
}


static rt_err_t rk_rtthread_vicap_lite_init(struct rt_device *dev)
{

    struct rk_vicap_lite_device *vip;
    struct clk_gate *clkgate;

    VIP_DBG("(%s) enter \n", __FUNCTION__);
    RT_ASSERT(dev != RT_NULL);

    vip = (struct rk_vicap_lite_device *)dev;
    if (vip->ops->init)
    {
        return (vip->ops->init(vip));
    }

    VIP_DBG("(%s) exit \n", __FUNCTION__);
    return RT_ENOSYS;
}

static rt_err_t rk_rtthread_vicap_lite_open(struct rt_device *dev, rt_uint16_t oflag)
{
    struct rk_vicap_lite_device *vip;

    VIP_DBG("(%s) enter \n", __FUNCTION__);
    RT_ASSERT(dev != RT_NULL)

    vip = (struct rk_vicap_lite_device *)&vicap_lite_instance;

    clk_enable(vip->clk.vicap_lite_hclk);
    clk_enable(vip->clk.vicap_lite_aclk);
    clk_enable(vip->clk.vicap_lite_dclk);
    HAL_DelayMs(1);

    VIP_DBG("(%s) exit \n", __FUNCTION__);
    return RT_ENOSYS;
}


static rt_err_t rk_rtthread_vicap_lite_close(struct rt_device *dev)
{
    struct rk_vicap_lite_device *vip;

    VIP_DBG("(%s) enter \n", __FUNCTION__);

    RT_ASSERT(dev != RT_NULL);

    vip = (struct rk_vicap_lite_device *)&vicap_lite_instance;

    clk_disable(vip->clk.vicap_lite_dclk);
    clk_disable(vip->clk.vicap_lite_aclk);
    clk_disable(vip->clk.vicap_lite_hclk);
    HAL_DelayMs(1);

    VIP_DBG("(%s) exit \n", __FUNCTION__);
    return RT_ENOSYS;
}


const static struct rt_device_ops rk_vicap_lite_dev_ops =
{
    rk_rtthread_vicap_lite_init,
    rk_rtthread_vicap_lite_open,
    rk_rtthread_vicap_lite_close,
    RT_NULL,
    RT_NULL,
    rk_rtthread_vicap_lite_control
};


int rk_rtthread_vicap_lite_register(void)
{
    int ret;
    struct rt_device *device;
    struct rk_vicap_lite_device *vip;
    struct clk_gate *clkgate;

    VIP_DBG("(%s) enter \n", __FUNCTION__);

    vip = (struct rk_vicap_lite_device *)&vicap_lite_instance;
    vip->p_reg = (struct VICAP_LITE_REG *)VICAP_LITE_BASE;

    device = &(vip->dev);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    vip->clk.vicap_lite_hclk = get_clk_gate_from_id(HCLK_VIPCAP_GATE);
    vip->clk.vicap_lite_aclk = get_clk_gate_from_id(ACLK_VIPCAP_GATE);
    vip->clk.vicap_lite_dclk = get_clk_gate_from_id(DCLK_VIPCAP_PLL_GATE);

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rk_vicap_lite_dev_ops;
#else
    device->init        = RT_NULL;
    device->open        = rk_rtthread_vicap_lite_open;
    device->close       = rk_rtthread_vicap_lite_close;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rk_rtthread_vicap_lite_control;
#endif
    vip->vicap_lite_isr = (rt_isr_handler_t)&rk_vicap_lite_irq;
    vip->irq = VICAP_IRQn;
    rt_hw_interrupt_install(vip->irq, vip->vicap_lite_isr, RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(vip->irq);
    /* frame start irq */
    /* y_fifo_overflow irq */
    /* uv_fifo_overflow irq */
    /* bus error irq */
    /* csi2rx_fifo_overflow irq */
    vip->p_reg->MIPI_INTEN = VICAP_LITE_MIPI_INTSTAT_BUS_ERR_INTST_MASK |
                             VICAP_LITE_MIPI_INTSTAT_CSI2RX_FIFO_OVERFLOW_INTST_MASK |
                             VICAP_LITE_MIPI_INTSTAT_BANDWIDTH_LACK_INTST_MASK |
                             VICAP_LITE_MIPI_INTSTAT_CONFIG_FIFO_OVERFLOW_INTST_MASK |
                             VICAP_LITE_MIPI_INTSTAT_DMA_UV_FIFO_OVERFLOW_INTST_MASK |
                             VICAP_LITE_MIPI_INTSTAT_DMA_Y_FIFO_OVERFLOW_INTST_MASK;

    ret = rt_device_register(device, "vicap_lite", RT_DEVICE_FLAG_RDWR);
    VIP_DBG("(%s) exit \n", __FUNCTION__);
    return ret;

}
#endif

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_rtthread_vicap_lite_register);
#endif

#endif
