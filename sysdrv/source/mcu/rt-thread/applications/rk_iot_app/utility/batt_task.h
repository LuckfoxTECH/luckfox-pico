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

#ifndef _BATT_TASK_H_
#define _BATT_TASK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct bat_charge_ctrl_msg
{
    uint16_t    cmd;
    uint32_t    data;
};
typedef enum
{
    BAT_MSG_CHARGE_IN,      //changer plug in
    BAT_MSG_CHARGE_OUT,     //changer pull out
    BAT_MSG_CHARGE_FULL,    //chager full
    BAT_MSG_LOW_POWER,      //low power
    BAT_MSG_LOW_POWER_DOWN, //low power to power down
    BAT_MSG_UPDATE_BAT,     //update battery value
    BAT_MSG_GET,            //other thread to query battery valye
    BAT_MSG_POWER_DOWN,
    BAT_MSG_NULL,
} bat_msg_t;
typedef enum
{
    BAT_STATE_LOW_POWER_DOWN,
    BAT_STATE_LOW_POWER,//
    BAT_STATE_NORMAL_POWER,
    BAT_STATE_HIGH_POWER,
    BAT_STATE_CHARGEING,//
    BAT_STATE_CHARGE_FULL,
    BAT_STATE_NULL,
} BAT_STATE;

extern bool rtos_charging_flag ;
void bat_charge_task_init(void);
void bat_charge_task_deinit(void);

#endif

