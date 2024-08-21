/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GLOBE_AIRKISS_H_
#define __GLOBE_AIRKISS_H_
#include <rtthread.h>
#include <rtdef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "sysinfo.h"
#include "net_ctrl.h"
#include "sys_ctrl.h"
#include "platform_init.h"
#include "sysinfo.h"
#include "wlan_api.h"

#define ALL_SUBTYPE (0xFFFF)

#define AK_TIME_OUT_MS 120000

typedef enum
{
    FDCM_SUCCESS     = 0,    /* success */
    FDCM_OPEN_ERR    = 1,  /*  */
    FDCM_READ_ERR   = 2,
    FDCM_WRITE_ERR  = 3,

} fdcm_status_t;

typedef enum network_msg_subtype
{
    CTRL_MSG_SUB_TYPE_NETWORK_CONNECT = 0,
    CTRL_MSG_SUB_TYPE_NETWORK_DISCONNECT,
    CTRL_MSG_SUB_TYPE_NETWORK_ALL = ALL_SUBTYPE,
} network_msg_subtype;

extern bool g_net_stat ;

int networkStart(struct sysinfo *e_sysinfo);
#if 0
fdcm_status_t loadSystemInfo(struct sysinfo *e_sysinfo);
fdcm_status_t saveSystemInfo(struct sysinfo *e_sysinfo);
wlan_airkiss_status_t AirkissStart(struct sysinfo *e_sysinfo);
#endif
int connectWifi(struct sysinfo *e_sysinfo);
void disconnectWifi(void);
int cleanNetworkInfo();
void networkDisconnectPublisher(void);
void NetworkStateAttach(void);

#endif //__GLOBE_AIRKISS_H_