/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2019-09-30     Frank Wang        the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#if defined(RT_USING_USB_HOST)
#include "hal_base.h"
#include "hal_bsp.h"
#include "drv_clock.h"

#define OTG_HS_PORT 1
static struct HCD_HANDLE g_hcd;
static struct rt_completion urb_completion;

void HAL_HCD_SOF_Callback(struct HCD_HANDLE *hhcd)
{
//    rt_kprintf("sof callback\n");
}

static __IO rt_bool_t connect_status = RT_FALSE;
void HAL_HCD_Connect_Callback(struct HCD_HANDLE *hhcd)
{
    uhcd_t hcd = (uhcd_t)hhcd->pData;

    if (!connect_status)
    {
        connect_status = RT_TRUE;
        RT_DEBUG_LOG(RT_DEBUG_USB, ("connected\n"));
        rt_usbh_root_hub_connect_handler(hcd, OTG_HS_PORT, RT_TRUE);
    }
}

void HAL_HCD_Disconnect_Callback(struct HCD_HANDLE *hhcd)
{
    uhcd_t hcd = (uhcd_t)hhcd->pData;

    if (connect_status)
    {
        connect_status = RT_FALSE;
        RT_DEBUG_LOG(RT_DEBUG_USB, ("disconnnect\n"));
        rt_usbh_root_hub_disconnect_handler(hcd, OTG_HS_PORT);
    }
}

void HAL_HCD_HCNotifyURBChange_Callback(struct HCD_HANDLE *hhcd, uint8_t chnum, eUSB_OTG_urbState urb_state)
{
//    rt_kprintf("NotifyURBChange_Callback\n");
    rt_completion_done(&urb_completion);
}

static rt_err_t drv_reset_port(rt_uint8_t port)
{
    RT_DEBUG_LOG(RT_DEBUG_USB, ("reset port\n"));
    HAL_HCD_ResetPort(&g_hcd);

    return RT_EOK;
}

static int drv_pipe_xfer(upipe_t pipe, rt_uint8_t token, void *buffer, int nbytes, int timeout)
{
    rt_tick_t delay;
    eUSB_OTG_hcState hc_state;
    eUSB_OTG_urbState urb_state;

    while (1)
    {
        if (!connect_status)
        {
            return -1;
        }
        rt_completion_init(&urb_completion);
        HAL_HCD_HCSubmitRequest(&g_hcd,
                                pipe->pipe_index,
                                (pipe->ep.bEndpointAddress & 0x80) >> 7,
                                pipe->ep.bmAttributes,
                                token,
                                buffer,
                                nbytes,
                                0);
        rt_completion_wait(&urb_completion, timeout);

        hc_state = HAL_HCD_HCGetState(&g_hcd, pipe->pipe_index);
        urb_state = HAL_HCD_HCGetURBState(&g_hcd, pipe->pipe_index);

        if (hc_state == HC_NAK)
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("nak\n"));
            if (pipe->ep.bmAttributes == USB_EP_ATTR_INT)
            {
                delay = (pipe->ep.bInterval * RT_TICK_PER_SECOND / 1000) > 0 ?
                        (pipe->ep.bInterval * RT_TICK_PER_SECOND / 1000) : 1;
                rt_thread_delay(delay);
            }
            HAL_HCD_HCHalt(&g_hcd, pipe->pipe_index);
            HAL_HCD_HCInit(&g_hcd,
                           pipe->pipe_index,
                           pipe->ep.bEndpointAddress,
                           pipe->inst->address,
                           USB_OTG_SPEED_HIGH,
                           pipe->ep.bmAttributes,
                           pipe->ep.wMaxPacketSize);
            continue;
        }
        else if (hc_state == HC_STALL)
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("stall\n"));
            pipe->status = UPIPE_STATUS_STALL;
            if (pipe->callback != RT_NULL)
            {
                pipe->callback(pipe);
            }
            return -1;
        }
        else if (urb_state == URB_ERROR)
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("error\n"));
            pipe->status = UPIPE_STATUS_ERROR;
            if (pipe->callback != RT_NULL)
            {
                pipe->callback(pipe);
            }
            return -1;
        }
        else if (urb_state != URB_NOTREADY && urb_state != URB_NYET)
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("ok\n"));
            pipe->status = UPIPE_STATUS_OK;
            if (pipe->callback != RT_NULL)
            {
                pipe->callback(pipe);
            }
            if (pipe->ep.bEndpointAddress & 0x80)
            {
                return HAL_HCD_HCGetXferCount(&g_hcd, pipe->pipe_index);
            }
            return nbytes;
        }
        return -1;
    }
}

static rt_uint16_t pipe_index = 0;
static rt_uint8_t  drv_get_free_pipe_index()
{
    rt_uint8_t idx;

    for (idx = 1; idx < 16; idx++)
    {
        if (!(pipe_index & (0x01 << idx)))
        {
            pipe_index |= (0x01 << idx);
            return idx;
        }
    }

    return 0xff;
}

static void drv_free_pipe_index(rt_uint8_t index)
{
    pipe_index &= ~(0x01 << index);
}

static rt_err_t drv_open_pipe(upipe_t pipe)
{
    pipe->pipe_index = drv_get_free_pipe_index();

    HAL_HCD_HCInit(&g_hcd,
                   pipe->pipe_index,
                   pipe->ep.bEndpointAddress,
                   pipe->inst->address,
                   USB_OTG_SPEED_HIGH,
                   pipe->ep.bmAttributes,
                   pipe->ep.wMaxPacketSize);
    /* Set DATA0 PID token*/
    if (g_hcd.hc[pipe->pipe_index].epIsIn)
    {
        g_hcd.hc[pipe->pipe_index].toggleIn = 0;
    }
    else
    {
        g_hcd.hc[pipe->pipe_index].toggleOut = 0;
    }

    return RT_EOK;
}

static rt_err_t drv_close_pipe(upipe_t pipe)
{
    HAL_HCD_HCHalt(&g_hcd, pipe->pipe_index);
    drv_free_pipe_index(pipe->pipe_index);

    return RT_EOK;
}

void usb_hcd_irq_handler(void)
{
    rt_interrupt_enter();
    HAL_HCD_IRQHandler(&g_hcd);
    rt_interrupt_leave();
}

static rt_err_t usb_hcd_init(rt_device_t device)
{
    struct HCD_HANDLE *hhcd = (struct HCD_HANDLE *)device->user_data;
    struct clk_gate *hclk_ctl_gate, *utmi_clk_gate;

    hhcd->pReg = g_usbdDev.pReg;;
    hhcd->cfg.hcNum = g_usbdDev.cfg.hcNum;
    hhcd->cfg.speed = g_usbdDev.cfg.speed;
    hhcd->cfg.dmaEnable = g_usbdDev.cfg.dmaEnable;
    hhcd->cfg.phyif = g_usbdDev.cfg.phyif;;
    hhcd->cfg.sofEnable = g_usbdDev.cfg.sofEnable;;

    /* Initialize usb clocks */
    hclk_ctl_gate = get_clk_gate_from_id(g_usbdDev.hclkGateID);
    utmi_clk_gate = get_clk_gate_from_id(g_usbdDev.utmiclkGateID);
    clk_enable(hclk_ctl_gate);
    clk_enable(utmi_clk_gate);

    /* Host interrupt init */
    rt_hw_interrupt_install(g_usbdDev.irqNum, (rt_isr_handler_t)usb_hcd_irq_handler, RT_NULL, "usb_hcd_irq");
    rt_hw_interrupt_umask(g_usbdDev.irqNum);

    RT_ASSERT(HAL_HCD_Init(hhcd) == HAL_OK);
    HAL_HCD_Start(hhcd);

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops usb_hcd_ops =
{
    .init = usb_hcd_init
};
#endif

struct uhcd_ops g_uhcd_ops =
{
    drv_reset_port,
    drv_pipe_xfer,
    drv_open_pipe,
    drv_close_pipe,
};

int rt_usbh_register(void)
{
    uhcd_t uhcd = (uhcd_t)rt_malloc(sizeof(struct uhcd));

    RT_ASSERT(uhcd != RT_NULL);
    rt_memset((void *)uhcd, 0, sizeof(struct uhcd));

    uhcd->parent.type = RT_Device_Class_USBHost;
#ifdef RT_USING_DEVICE_OPS
    uhcd->parent.ops = &usb_hcd_ops;
#else
    uhcd->parent.init = usb_hcd_init;
#endif
    uhcd->parent.user_data = &g_hcd;
    uhcd->ops = &g_uhcd_ops;
    uhcd->num_ports = 1;
    g_hcd.pData = uhcd;

    rt_device_register((rt_device_t)uhcd, "usbh", 0);
    rt_usb_host_init();

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_usbh_register);
#endif
