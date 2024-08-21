/**
  * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_canfd.c
  * @version V0.1
  * @brief  canfd interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-04-29     Elaine.Zhang      first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
*  @{
*/

/** @addtogroup CANFD
 *  @{
 */

/** @defgroup CANFDn_How_To_Use How To Use
 *  @{

The CANFD driver use to configure CAN work mode and bit rate, and CAN bus transmits and receives data,
 it can be used in the following three scenarios:

- **Configure CAN**:
    - The device set CAN work mode and bit rate by rockchip_canfd_control(struct rt_can_device *can, int cmd, void *arg);
    - The device set CAN work mode and bit rate directly by rockchip_canfd_init(struct CAN_REG *pcan, rt_uint32_t baud, rt_uint32_t mode);

- **CAN Tx**:
    - The CAN bus transmit by rockchip_canfd_sendmsg(struct rt_can_device *can, const void *buf, rt_uint32_t boxno);

- **CAN Rx**:
    - The CAN bus receive In the interrupt function by HAL_CAN_Receive(CANx, RxMessage) and rockchip_canfd_recvmsg(struct rt_can_device *can, void *buf, rt_uint32_t boxno);

 @} */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <rthw.h>

#include "hal_base.h"
#include "hal_bsp.h"

#ifdef RT_USING_CAN

#define ROCKCHIP_CAN_CLK_RATE       200000000

static uint32_t tx_finsh = 0, rx_finsh = 0, rx_full = 0, err_int = 0;

struct rockchip_canfd
{
    const struct HAL_CANFD_DEV *dev;
    struct CANFD_CONFIG *configs;
    struct CANFD_MSG *RxMessage;
    struct CANFD_MSG *TxMessage;
    void *irq;
};

/********************* Public Function Definition ****************************/

/** @defgroup CANFD_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  rockchip canfd init.
 * @param can: rt_can_device.
 * @param baud: canfd bit rate
 * @param mode: canfd work mode
 */
static void rockchip_canfd_init(struct rt_can_device *can, rt_uint32_t baud, rt_uint32_t mode)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) can->parent.user_data;
    eCANFD_Bps bps;

    switch (baud)
    {
    case CAN1MBaud:
        bps = CANFD_BPS_1MBAUD;
        break;
    case CAN800kBaud:
        bps = CANFD_BPS_800KBAUD;
        break;
    case CAN500kBaud:
        bps = CANFD_BPS_500KBAUD;
        break;
    case CAN250kBaud:
        bps = CANFD_BPS_250KBAUD;
        break;
    case CAN125kBaud:
        bps = CANFD_BPS_200KBAUD;
        break;
    case CAN100kBaud:
        bps = CANFD_BPS_100KBAUD;
        break;
    case CAN50kBaud:
        bps = CANFD_BPS_50KBAUD;
        break;
    default:
        bps = CANFD_BPS_1MBAUD;
        break;
    }
    rkcan->configs->bps = bps;
    switch (mode)
    {
    case RT_CAN_MODE_NORMAL:
        rkcan->configs->canfdMode = CANFD_MODE_FD;
        break;
    case RT_CAN_MODE_LISEN:
        rkcan->configs->canfdMode = CANFD_MODE_LISTENONLY;
        break;
    case RT_CAN_MODE_LOOPBACK:
        rkcan->configs->canfdMode = CANFD_MODE_LOOPBACK;
        break;
    }
    HAL_CANFD_Init(rkcan->dev->pReg, rkcan->configs);

    HAL_CANFD_Start(rkcan->dev->pReg);
}

/**
 * @brief  rockchip canfd configure.
 * @param can: rt_can_device.
 * @param config: struct can_configure
 * @retval RT_EOK
 */
static rt_err_t rockchip_canfd_configure(struct rt_can_device *can, struct can_configure *config)
{
    rockchip_canfd_init(can, config->baud_rate, config->mode);

    return RT_EOK;
}

/**
 * @brief  rockchip canfd control.
 * @param cmd: command.
 * @param arg: void
 * @retval RT_EOK
 */
static rt_err_t rockchip_canfd_control(struct rt_can_device *can, int cmd, void *arg)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) can->parent.user_data;
    rt_uint32_t argval;

    switch (cmd)
    {
    case RT_CAN_CMD_SET_MODE:
        argval = (rt_uint32_t) arg;
        if (argval != RT_CAN_MODE_NORMAL &&
                argval != RT_CAN_MODE_LISEN &&
                argval != RT_CAN_MODE_LOOPBACK &&
                argval != RT_CAN_MODE_LOOPBACKANLISEN)
        {
            return RT_ERROR;
        }
        if (argval != can->config.mode)
        {
            can->config.mode = argval;
        }
        break;
    case RT_CAN_CMD_GET_STATUS:
    {
        rt_uint32_t errtype;

        errtype = rkcan->dev->pReg->RXERRORCNT;
        can->status.rcverrcnt = errtype ;
        errtype = rkcan->dev->pReg->TXERRORCNT;
        can->status.snderrcnt = errtype ;
        errtype = rkcan->dev->pReg->ERROR_CODE;

        can->status.errcode = errtype ;
        if (arg != &can->status)
        {
            rt_memcpy(arg, &can->status, sizeof(can->status));
        }
    }
    break;
    }

    rockchip_canfd_configure(can, &can->config);

    return RT_EOK;
}

/**
 * @brief  rockchip canfd sendmsg.
 * @param can: rt_can_device.
 * @param buf: sendmsg
 * @param boxno: sendmsg size
 * @retval RT_EOK
 */
static int rockchip_canfd_sendmsg(struct rt_can_device *can, const void *buf, rt_uint32_t boxno)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) can->parent.user_data;
    struct rt_can_msg *pmsg = (struct rt_can_msg *) buf;
    int i;

    rkcan->TxMessage->stdId = pmsg->id;
    rkcan->TxMessage->extId = pmsg->id;
    rkcan->TxMessage->rtr = pmsg->rtr;
    rkcan->TxMessage->ide = pmsg->ide;
    rkcan->TxMessage->dlc = pmsg->len;

    if (pmsg->len > 8)
    {
        rkcan->TxMessage->fdf = 1;
    }
    else
    {
        rkcan->TxMessage->fdf = 0;
    }
    for (i = 0; i < rkcan->TxMessage->dlc ; i++)
    {
        rkcan->TxMessage->data[i] = pmsg->data[i];
    }
    HAL_CANFD_Transmit(rkcan->dev->pReg, rkcan->TxMessage);

    return RT_EOK;
}

/**
 * @brief  rockchip canfd recvmsg.
 * @param can: rt_can_device.
 * @param buf: recvmsg
 * @param boxno: recvmsg size
 * @retval RT_EOK
 */
static int rockchip_canfd_recvmsg(struct rt_can_device *can, void *buf, rt_uint32_t boxno)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) can->parent.user_data;
    struct rt_can_msg *pmsg = (struct rt_can_msg *) buf;
    int i;

    if (!rkcan->RxMessage)
        return -RT_EINVAL;

    pmsg->ide = (rt_uint32_t) rkcan->RxMessage->ide;
    if (rkcan->RxMessage->ide == 1)
        pmsg->id = rkcan->RxMessage->extId;
    else
        pmsg->id = rkcan->RxMessage->stdId;

    pmsg->len = rkcan->RxMessage->dlc;
    pmsg->rtr = rkcan->RxMessage->rtr;
    pmsg->hdr = 0;

    for (i = 0; i < rkcan->RxMessage->dlc; i++)
    {
        pmsg->data[i] = rkcan->RxMessage->data[i];
    }
    return RT_EOK;
}

static const struct rt_can_ops canops =
{
    rockchip_canfd_configure,
    rockchip_canfd_control,
    rockchip_canfd_sendmsg,
    rockchip_canfd_recvmsg,
};

#ifdef RT_USING_CAN0
static struct rt_can_device rockchip_canfd0;
void rockchip_canfd0_irqhandler(int irq, void *param)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) rockchip_canfd0.parent.user_data;
    struct CAN_REG *CANx = g_can0Dev.pReg;
    unsigned status;

    rt_interrupt_enter();

    status = HAL_CANFD_GetInterrupt(CANx);
    if (status & rx_finsh)
    {
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
        rt_hw_can_isr(&rockchip_canfd0, RT_CAN_EVENT_RX_IND);
    }
    else if (status & tx_finsh)
    {
        rt_hw_can_isr(&rockchip_canfd0, RT_CAN_EVENT_TX_DONE | 0 << 8);
    }
    else if (status & rx_full)
    {
        rt_hw_can_isr(&rockchip_canfd0, RT_CAN_EVENT_RXOF_IND);
        rt_kprintf("rockchip_canfd0 int RX OF happened!\r\n");
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
    }
    else if (status & err_int)
    {
        rt_kprintf("rockchip_canfd0 int BUS ERR happened!\r\n");
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
    }
    rt_interrupt_leave();
}

static struct rockchip_canfd rockchip_canfd0data =
{
    .dev = &g_can0Dev,
    .irq = rockchip_canfd0_irqhandler,
};
#endif /*USING_CAN0*/

#ifdef RT_USING_CAN1
static struct rt_can_device rockchip_canfd1;
void rockchip_canfd1_irqhandler(int irq, void *param)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) rockchip_canfd1.parent.user_data;
    struct CAN_REG *CANx = g_can1Dev.pReg;
    unsigned status;

    rt_interrupt_enter();

    status = HAL_CANFD_GetInterrupt(CANx);
    if (status & rx_finsh)
    {
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
        rt_hw_can_isr(&rockchip_canfd1, RT_CAN_EVENT_RX_IND);
    }
    else if (status & tx_finsh)
    {
        rt_hw_can_isr(&rockchip_canfd1, RT_CAN_EVENT_TX_DONE | 0 << 8);
    }
    else if (status & rx_full)
    {
        rt_hw_can_isr(&rockchip_canfd1, RT_CAN_EVENT_RXOF_IND);
        rt_kprintf("rockchip_canfd1 int RX OF happened!\r\n");
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
    }
    else if (status & err_int)
    {
        rt_kprintf("rockchip_canfd1 int BUS ERR happened!\r\n");
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
    }
    rt_interrupt_leave();

}
static struct rockchip_canfd rockchip_canfd1data =
{
    .dev = &g_can1Dev,
    .irq = rockchip_canfd1_irqhandler,
};
#endif /*USING_CAN1*/

#ifdef RT_USING_CAN2
static struct rt_can_device rockchip_canfd2;
void rockchip_canfd2_irqhandler(int irq, void *param)
{
    struct rockchip_canfd *rkcan = (struct rockchip_canfd *) rockchip_canfd2.parent.user_data;
    struct CAN_REG *CANx = g_can2Dev.pReg;
    unsigned status;

    rt_interrupt_enter();

    status = HAL_CANFD_GetInterrupt(CANx);
    if (status & rx_finsh)
    {
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
        rt_hw_can_isr(&rockchip_canfd2, RT_CAN_EVENT_RX_IND);
    }
    else if (status & tx_finsh)
    {
        rt_hw_can_isr(&rockchip_canfd2, RT_CAN_EVENT_TX_DONE | 0 << 8);
    }
    else if (status & rx_full)
    {
        rt_hw_can_isr(&rockchip_canfd2, RT_CAN_EVENT_RXOF_IND);
        rt_kprintf("rockchip_canfd2 int RX OF happened!\r\n");
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
    }
    else if (status & err_int)
    {
        rt_kprintf("rockchip_canfd2 int BUS ERR happened!\r\n");
        HAL_CANFD_Receive(CANx, rkcan->RxMessage);
    }
    rt_interrupt_leave();
}
static struct rockchip_canfd rockchip_canfd2data =
{
    .dev = &g_can2Dev,
    .irq = rockchip_canfd2_irqhandler,
};
#endif /*USING_CAN2*/

int rockchip_canfd_dev_init(void)
{

#ifdef RT_USING_CAN0
    rockchip_canfd0.config.baud_rate = CAN500kBaud;
    rockchip_canfd0.config.msgboxsz = 1;
    rockchip_canfd0.config.sndboxnumber = 1;
    rockchip_canfd0.config.mode = RT_CAN_MODE_NORMAL;
    rockchip_canfd0.config.privmode = 0;
    rockchip_canfd0.config.ticks = 50;

    rockchip_canfd0data.configs = rt_calloc(1, sizeof(struct CANFD_CONFIG));
    rockchip_canfd0data.TxMessage = rt_calloc(1, sizeof(struct CANFD_MSG));
    rockchip_canfd0data.RxMessage = rt_calloc(1, sizeof(struct CANFD_MSG));

    rt_hw_can_register(&rockchip_canfd0, "rk_can0", &canops, &rockchip_canfd0data);
    rt_kprintf("rockchip_canfd0 register! \r\n");

    rt_hw_interrupt_install(rockchip_canfd0data.dev->irqNum, (rt_isr_handler_t)rockchip_canfd0data.irq, RT_NULL, "can0");
    rt_hw_interrupt_umask(rockchip_canfd0data.dev->irqNum);
    HAL_CRU_ClkSetFreq(rockchip_canfd1data.dev->sclkID, ROCKCHIP_CAN_CLK_RATE);

#endif

#ifdef RT_USING_CAN1
    rockchip_canfd1.config.baud_rate = CAN500kBaud;
    rockchip_canfd1.config.msgboxsz = 16;
    rockchip_canfd1.config.sndboxnumber = 1;
    rockchip_canfd1.config.mode = RT_CAN_MODE_NORMAL;
    rockchip_canfd1.config.privmode = 0;
    rockchip_canfd1.config.ticks = 50;
#ifdef RT_CAN_USING_HDR
    rockchip_canfd1.config.maxhdr = 4;
#endif

    rockchip_canfd1data.configs = rt_calloc(1, sizeof(struct CANFD_CONFIG));
    rockchip_canfd1data.TxMessage = rt_calloc(1, sizeof(struct CANFD_MSG));
    rockchip_canfd1data.RxMessage = rt_calloc(1, sizeof(struct CANFD_MSG));

    rt_hw_can_register(&rockchip_canfd1, "rk_can1", &canops, &rockchip_canfd1data);
    rt_kprintf("rockchip_canfd1 register! \r\n");

    rt_hw_interrupt_install(rockchip_canfd1data.dev->irqNum, (rt_isr_handler_t)rockchip_canfd1data.irq, RT_NULL, "can1");
    rt_hw_interrupt_umask(rockchip_canfd1data.dev->irqNum);
    HAL_CRU_ClkSetFreq(rockchip_canfd1data.dev->sclkID, ROCKCHIP_CAN_CLK_RATE);

#endif

#ifdef RT_USING_CAN2
    rockchip_canfd2.config.baud_rate = CAN500kBaud;
    rockchip_canfd2.config.msgboxsz = 1;
    rockchip_canfd2.config.sndboxnumber = 1;
    rockchip_canfd2.config.mode = RT_CAN_MODE_NORMAL;
    rockchip_canfd2.config.privmode = 0;
    rockchip_canfd2.config.ticks = 50;

    rockchip_canfd2data.configs = rt_calloc(1, sizeof(struct CANFD_CONFIG));
    rockchip_canfd2data.TxMessage = rt_calloc(1, sizeof(struct CANFD_MSG));
    rockchip_canfd2data.RxMessage = rt_calloc(1, sizeof(struct CANFD_MSG));

    rt_hw_can_register(&rockchip_canfd2, "rk_can2", &canops, &rockchip_canfd2data);
    rt_kprintf("rockchip_canfd2 register! \r\n");

    rt_hw_interrupt_install(rockchip_canfd2data.dev->irqNum, (rt_isr_handler_t)rockchip_canfd2data.irq, RT_NULL, "can2");
    rt_hw_interrupt_umask(rockchip_canfd2data.dev->irqNum);
    HAL_CRU_ClkSetFreq(rockchip_canfd1data.dev->sclkID, ROCKCHIP_CAN_CLK_RATE);

#endif

    err_int = HAL_CANFD_GetErrInterruptMaskCombin(CANFD_INT_ERR);
    rx_full = HAL_CANFD_GetErrInterruptMaskCombin(CANFD_INT_RX_OF);
    tx_finsh = HAL_CANFD_GetErrInterruptMaskCombin(CANFD_INT_TX_FINISH);
    rx_finsh = HAL_CANFD_GetErrInterruptMaskCombin(CANFD_INT_RX_FINISH);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rockchip_canfd_dev_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
#define CAN_DEV_NAME       "rk_can1"
static struct rt_semaphore rx_sem;
static rt_device_t can_dev;

static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem);
    return RT_EOK;
}

static void can_rx_thread(void *parameter)
{
    int i;
    struct rt_can_msg rxmsg = {0};

    rt_device_set_rx_indicate(can_dev, can_rx_call);

#ifdef RT_CAN_USING_HDR
    struct rt_can_filter_item items[4] =
    {
        RT_CAN_FILTER_ITEM_INIT(0x100, 0, 0, 1, 0x700, RT_NULL, RT_NULL),
        RT_CAN_FILTER_ITEM_INIT(0x300, 0, 0, 1, 0x700, RT_NULL, RT_NULL),
        RT_CAN_FILTER_ITEM_INIT(0x211, 0, 0, 1, 0x7ff, RT_NULL, RT_NULL),
        RT_CAN_FILTER_STD_INIT(0x486, RT_NULL, RT_NULL),
    };
    struct rt_can_filter_config cfg = {4, 1, items};
    rt_device_control(can_dev, RT_CAN_CMD_SET_FILTER, &cfg);
#endif

    while (1)
    {
        rxmsg.hdr = -1;
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        rt_device_read(can_dev, 0, &rxmsg, sizeof(rxmsg));
        rt_kprintf("ID:%x , LEN:%x DATA:\n", rxmsg.id, rxmsg.len);
        for (i = 0; i < 8; i++)
        {
            rt_kprintf("%2x", rxmsg.data[i]);
        }
        rt_kprintf("\n");
    }
}

int can_sample(int argc, char *argv[])
{
    struct rt_can_msg msg = {0};
    rt_err_t res = 0;
    rt_size_t  size;
    rt_thread_t thread;
    char can_name[RT_NAME_MAX];

    if (argc == 2)
    {
        rt_strncpy(can_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(can_name, CAN_DEV_NAME, RT_NAME_MAX);
    }
    can_dev = rt_device_find(can_name);
    if (!can_dev)
    {
        rt_kprintf("find %s failed!\n", can_name);
        return RT_ERROR;
    }

    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    res = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
    RT_ASSERT(res == RT_EOK);
    rt_device_control(can_dev, RT_CAN_CMD_SET_MODE, (void *)RT_CAN_MODE_NORMAL);
    thread = rt_thread_create("can_rx", can_rx_thread, RT_NULL, 1024, 25, 10);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        rt_kprintf("create can_rx thread failed!\n");
    }
    msg.id = 0x123;
    msg.ide = RT_CAN_STDID;
    msg.rtr = RT_CAN_DTR;
    msg.len = 8;
    msg.data[0] = 0x12;
    msg.data[1] = 0x34;
    msg.data[2] = 0x56;
    msg.data[3] = 0x78;
    msg.data[4] = 0x87;
    msg.data[5] = 0x65;
    msg.data[6] = 0x43;
    msg.data[7] = 0x21;
    size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }
    return res;
}
MSH_CMD_EXPORT(can_sample, can device sample);
#endif


/** @} */

#endif

/** @} */

/** @} */
