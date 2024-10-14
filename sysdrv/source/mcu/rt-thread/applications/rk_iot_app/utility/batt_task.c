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

#include <rtthread.h>
#include "batt_task.h"
#include "globe_system_stat.h"
#include "globe_player.h"
#include "rkdebug.h"


uint8_t charge_irq_stat = 0;

BAT_STATE current_state = BAT_STATE_NULL;

#define THREAD_STACK_SIZE                               (4 * 1024)
#define TASK_BAT_MSG_RECV_DELAY_MS                              (0)

#define TASK_DELAY_TIME                                         (200)
#define BAT_LOW_POWER_MSG_TIME_INTERVAL                         (20000)//10s 发送一次低电提示消息
#define BAT_DATA_UPDATE                                         (60 * 1000)// 1s更新一次
//#define BAT_DATA_UPDATE                                         (5 * 1000)// 1s更新一次

#define BAT_ARRAY_NUM                                           (10)

#define BAT_ARRAY_SET                                           (1)
#define BAT_ARRAY_CLEAR                                         (0)

#define BAT_RANGE_STAT_CHARGE                                   (0xff)

#define BAT_LEVEL_HOLD_TIME_S                                   (10)
#define BAT_LOWER_POWER_TIME_S                                  (60)


// static BAT_STATE get_currrent_bat_state(uint32_t vol);

int bat_state_charge()
{
    if (current_state != BAT_STATE_CHARGEING) return 0;
    else return 1;

}
bool rtos_charging_flag = false ;
void rtos_bat_wait_dc_out(void)
{
    //todo ......
}


rt_thread_t g_bat_charge_ctrl_thread = RT_NULL;
/***********************************************************************

************************************************************************/
uint16_t bat_array[BAT_ARRAY_NUM];
static uint16_t get_bat_range_average(uint32_t vol)
{
    //TODO: need process ......
    return 0;
}


static void update_bat_array_clear(uint32_t vol, uint8_t clear)
{
    static uint8_t index = 0;
    if (clear == BAT_ARRAY_CLEAR)
    {
        memset(bat_array, 0, sizeof(bat_array));
        index = 0;
    }
    else
    {
        bat_array[index] = vol;
        index++;
        if (index >= BAT_ARRAY_NUM) index = 0;
    }
}

void bat_charge_ctrl_task()
{
    static uint8_t current_charge = 0;

    BAT_STATE tmp;
    uint32_t vol;
    int32_t update_bat_msg = BAT_DATA_UPDATE;

    uint8_t current_bat_level = 0;

    rt_thread_delay(10);
    while (1)
    {
        // not effect on ota or other test mode

        /* TODO: need RK process .......*/
        //
        vol = 3850;
        tmp = BAT_STATE_NORMAL_POWER;
        if (tmp == BAT_STATE_CHARGEING)
        {
            if (current_charge == 0)
            {
            }
        }
        else
        {
            current_charge = 0;

        }
        if (tmp != BAT_STATE_CHARGEING)
        {
            update_bat_array_clear(vol, BAT_ARRAY_SET);
        }

        if (update_bat_msg <= 0)
        {
            update_bat_msg = BAT_DATA_UPDATE;
            current_bat_level = get_bat_range_average(vol);
            static int old_vol = 0 ;
            if (old_vol != current_bat_level)
            {
                old_vol = current_bat_level ;
                rtos_post_battery(current_bat_level) ;
            }
        }
        else if (update_bat_msg > 0)
        {
            update_bat_msg -= TASK_DELAY_TIME;
        }
        current_state = tmp;
        rt_thread_delay(TASK_DELAY_TIME);
    }
}

//static int first_start = 1;

void bat_charge_task_init(void)
{
    if (current_state == BAT_STATE_CHARGEING)
    {
        rtos_bat_wait_dc_out();
    }

    g_bat_charge_ctrl_thread = rt_thread_create("bat_charge_task",
                               bat_charge_ctrl_task,
                               RT_NULL,
                               THREAD_STACK_SIZE,
                               RT_THREAD_PRIORITY_MAX / 2 + 1, 5);
    if (g_bat_charge_ctrl_thread != RT_NULL)
    {
        rt_thread_startup(g_bat_charge_ctrl_thread);
        LOGD("create bat charge ctrl task success");
    }
}

void bat_charge_task_deinit(void)
{
    //rt_kprintf("%s enter ...\n",__func__);
    /* TODO: need process ......*/
    //

    if (g_bat_charge_ctrl_thread)
        rt_thread_delete(g_bat_charge_ctrl_thread);
}
