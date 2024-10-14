/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-20     mark.huang  the first version
 */

#include "btKoalaControl.h"


#ifdef BT_CHIP_KOALA
#include <rtthread.h>

_koalaPcb_ koalapcb;
uint32 KoalaHwPowerOn(void *config)
{
    RK_LOGD("Koala", "KoalaHwPowerOn start\n");
    int ret = 0;
    koalapcb.PowerStatus = POWER_STATUS_ON;
    return ret;

}

uint32 KoalaHwPowerOff(void *config)
{
    koalapcb.PowerStatus = POWER_STATUS_OFF;
    return 0;

}

uint32 KoalaHwInit(void)
{
    memset(&koalapcb, 0, sizeof(_koalaPcb_));
    return 0;

}

uint32 KoalaHciDataRead(uint8 *data, uint32 len)
{
    /* RX */
    //RK_LOGD("Koala","host_recv_packet now!\n");
    return hci_hread(data, len);
}

uint32 KoalaHciDataWrite(uint8 *data , uint32 len)
{
    /* TX */
    //RK_LOGD("Koala","host_send_packet now!\n");
    hci_hwrite(data, len);
    return 0;
}

/******************************************************************************
**  HCI H4 Services interface table
******************************************************************************/

const bt_hw_control_t koala_hw_control =
{
    KoalaHwInit,
    KoalaHwPowerOn,
    KoalaHwPowerOff,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    KoalaHciDataWrite,
    KoalaHciDataRead,
};

#endif    //(BT_CHIP_CONFIG == BT_CHIP_Koala)


