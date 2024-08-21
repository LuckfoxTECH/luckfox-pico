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
#include <rtdef.h>
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include "audio_server.h"

#include "flash_play.h"
#include "globe_system_stat.h"
#include "rec_pub.h"
#include "link_queue.h"
#include "globe_player.h"
#include "sys_queue.h"
#include "flash_play.h"
#include "globe_record.h"
#include "sys_ctrl.h"

#define DBG_SECTION_NAME    "G_RECORD"
#include "rkdebug.h"

#define RECORD_STACK_SIZE                               (4 * 1024)

#define RECORD_BUF_NODE_NUM                     (30)            //  node number
#define RECORD_BUF_NODE_SIZE                   (1280)          //  size of node


typedef struct _rtos_record_private
{
    bool Run ;
    rt_mq_t Queue;
    rt_thread_t Thread;

    bool RecordRun ;

    // GptRecordData_Def* RecordDataHead ;
    // GptRecordData_Def* RecordDataEnd ;
    // uint8_t RecordDataNum ;
} rtos_record_private_def;

static rtos_record_private_def rtos_record_private ;
RtosRecordFun_Def RtosRecordFun ;


static uint32_t wechat_record_count = 0 ;
static uint32_t vad_record_count = 0 ;
static bool rtos_timer_once = false ;
// static OS_Timer_t WechatTime = {0} ;
static rt_timer_t WechatTime;
static bool timer_run = false ;
static void WechatTimerCallback(void *data)
{
    wechat_record_count++ ;
    if (wechat_record_count >= 58 * 5)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Wechat) ;
        rtos_record_send(RtosRecordCmd_Stop) ;
        wechat_record_count = 0 ;
        rt_timer_stop(WechatTime);
        rtos_timer_once = false ;
    }

    return ;
}
static void VadTimerCallback(void *data)
{
    vad_record_count++ ;
    if (vad_record_count >= 15 * 5)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Wechat) ;
        rtos_record_send(RtosRecordCmd_Cannel) ;
        vad_record_count = 0 ;
        rt_timer_stop(WechatTime);
        rtos_timer_once = false ;
    }

    return ;
}

static void rtos_record_timer_start(uint8_t type, bool val)
{
    if (val == true && rtos_timer_once == true)
    {
        return ;
    }
    if (val == true)
    {
        rtos_timer_once = true ;
    }
    else
    {
        rtos_timer_once = false ;
    }
    timer_run = true ;
    if (WechatTime == NULL)
    {
        WechatTime = rt_timer_create("Wechat_timer",
                                     type == RtosRecordCmd_WechatStart ? WechatTimerCallback : VadTimerCallback,
                                     NULL,
                                     200,
                                     RT_TIMER_FLAG_PERIODIC);
        if (WechatTime)
            rt_timer_start(WechatTime);

        wechat_record_count = 0 ;
        vad_record_count = 0 ;
    }
    else
    {
        vad_record_count = 0 ;
        wechat_record_count = 0 ;
        rt_timer_start(WechatTime);
    }
    return ;
}


static void rtos_record_timer_stop(void)
{
    if (timer_run == false)
    {
        return ;
    }
    rtos_timer_once = false ;
    wechat_record_count = 0 ;
    vad_record_count = 0 ;
    rt_timer_stop(WechatTime);
    return ;
}


uint8_t pcm_buf[RECORD_BUF_NODE_NUM * RECORD_BUF_NODE_SIZE] = {0} ;
static void rtos_record_data_init(void) ;
static void rtos_record_task(void *arg)
{
    rtos_record_msg rec_data;
    // int i = 0 ;
    // struct pcm_config config = {
    //     .channels = 1,
    //     .rate = 8000,
    //     .period_size = RECORD_BUF_NODE_SIZE / 2 ,//2560,
    //     .period_count = 2,
    //     .format = PCM_FORMAT_S16_LE,
    // };

    int ret = 0 ;
    int vad_ret = 0 ;
    uint8_t type = 0 ;
    bool pcm_open = false ;
    bool vad_flag = false ;
    // int samplerate = 8000; ///< 8000 / 16000
    // int channels = 1;      ///< 1/2
    LOGI("Start Record task ......") ;
    // SEARCH_FOREACH_CONFIG(samplerate, channels, config, i);
    // if (config == NULL) {
    //  printf("invalid audio cap param.");
    //  //f_close(&file);
    //  //goto exit_thread;
    // }
    while (rtos_record_private.Run)
    {
        memset(&rec_data, 0, sizeof(rtos_record_msg)) ;
        ret = rtos_msg_recv(rtos_record_private.Queue, &rec_data, RT_WAITING_FOREVER) ;
        if (0 != ret)
        {
            LOGE("Rec cmd Err\n") ;
            continue ;
        }
        // LOGD("Rec CMD:%d.........\n" , rec_data.cmd) ;
        if (rtos_record_private.RecordRun == true &&
                (rec_data.cmd == RtosRecordCmd_AIStart ||
                 rec_data.cmd == RtosRecordCmd_WechatStart ||
                 rec_data.cmd == RtosRecordCmd_ZToEStart ||
                 rec_data.cmd == RtosRecordCmd_EtoZStart
                ))
        {
            RtosRecordFun.RecordDataEnd->flag = false,
                                         pcm_open  = false ;

            type = rec_data.cmd ;
//            rtos_record_http_send(GptRecordHttpMsgCmd_CANNEL , type , 0 , 0) ;
            continue ;
        }
        if (rec_data.cmd == RtosRecordCmd_Stop && rtos_record_private.RecordRun == false)
        {
            rtos_record_timer_stop() ;
            continue ;
        }
        if (rec_data.cmd == RtosRecordCmd_Cannel && rtos_record_private.RecordRun == false)
        {
//            rtos_record_http_send(GptRecordHttpMsgCmd_CANNEL , type , 0 , 0) ;
            rtos_record_timer_stop() ;
            continue ;
        }
        if (rec_data.cmd == RtosRecordCmd_AIStart ||
                rec_data.cmd == RtosRecordCmd_WechatStart ||
                rec_data.cmd == RtosRecordCmd_ZToEStart ||
                rec_data.cmd == RtosRecordCmd_EtoZStart)
        {

            RtosSystemStat_Fun.LoopAI_Type = rec_data.cmd ;
        }

        vad_flag = false ;
//        rtos_vad_init() ;
        rtos_record_data_init() ;

        //  Start record report task
        LOGI(" Start Record http task") ;
//        rtos_record_http_send(GptRecordHttpMsgCmd_START , rec_data.cmd , 0 , 10) ;
        extern void rtos_music_stop(void) ;
        rtos_music_stop() ;
        type = rec_data.cmd ;
        rtos_record_private.RecordRun = true ;

        rtos_record_timer_start(type, false) ;
        RtosRecordFun.RecordDataNum = 0 ;
        while (rtos_record_private.RecordRun)
        {
            memset(&rec_data, 0, sizeof(rtos_record_msg)) ;
            ret = rtos_msg_recv(rtos_record_private.Queue, &rec_data, 0) ;
            if (rec_data.cmd == RtosRecordCmd_Stop || rec_data.cmd == RtosRecordCmd_Cannel)
            {
                //rtos_vad_stop() ;
                break ;
            }

            if (pcm_open == false)
            {
                //  open PCM
                //to do......
                //
                LOGI(" open PCM") ;
                pcm_open = true ;
            }

            if (RtosRecordFun.RecordDataEnd->next == RtosRecordFun.RecordDataHead)
            {
                RtosRecordFun.RecordDataHead->flag = false ;
                memset(RtosRecordFun.RecordDataHead->add, 0, RECORD_BUF_NODE_SIZE) ;
                RtosRecordFun.RecordDataHead = RtosRecordFun.RecordDataHead->next ;
                RtosRecordFun.RecordDataNum-- ;
            }

            RtosRecordFun.RecordDataEnd->flag = false,
                                         memset(RtosRecordFun.RecordDataEnd->add, 0, RECORD_BUF_NODE_SIZE) ;

            //  PCM Read
            //to do .......
            //

            // LOGI("PCM read addr: 0x%p    size: %d\n" , RtosRecordFun.RecordDataEnd->add , ret) ;
            if (type != RtosRecordCmd_WechatStart)
            {
                //  VAD Detect
//                vad_ret = rtos_vad_stat((char *)RtosRecordFun.RecordDataEnd->add, RECORD_BUF_NODE_SIZE ) ;

                RtosRecordFun.RecordDataEnd->flag = true ;
                RtosRecordFun.RecordDataEnd = RtosRecordFun.RecordDataEnd->next ;
                RtosRecordFun.RecordDataNum++ ;
                // LOGI("vad:%d - num:%d ...................\n" , vad_ret , RtosRecordFun.RecordDataNum) ;
                if (vad_ret == 1 || vad_ret == 2)
                {
                    rtos_record_timer_start(type, true) ;
                    if (vad_flag == false)
                    {
                        vad_flag = true ;
                        while (1)
                        {
                            if (RtosRecordFun.RecordDataNum <= 9)
                            {
                                break ;
                            }

                            RtosRecordFun.RecordDataHead->flag = false ;
                            memset(RtosRecordFun.RecordDataHead->add, 0, RECORD_BUF_NODE_SIZE) ;
                            RtosRecordFun.RecordDataHead = RtosRecordFun.RecordDataHead->next ;
                            RtosRecordFun.RecordDataNum-- ;

                        }
                    }

//                    rtos_record_http_send(GptRecordHttpMsgCmd_RECORDING , type , RECORD_BUF_NODE_SIZE , (uint32_t)RtosRecordFun.RecordDataHead->add ) ;
                    // RtosRecordFun.RecordDataHead->flag = false ;
                    // RtosRecordFun.RecordDataHead = RtosRecordFun.RecordDataHead->next ;
                    // RtosRecordFun.RecordDataNum-- ;
                }
                else if (vad_ret == 3)
                {
                    LOGI("VAD End") ;
                    rec_data.cmd = RtosRecordCmd_Stop ;

                    break ;
                }
                else
                {
                }
            }
            else
            {
                RtosRecordFun.RecordDataNum++ ;
                RtosRecordFun.RecordDataEnd->flag = true  ;
                RtosRecordFun.RecordDataEnd = RtosRecordFun.RecordDataEnd->next ;
//                rtos_record_http_send(GptRecordHttpMsgCmd_RECORDING , type , RECORD_BUF_NODE_SIZE , (uint32_t)RtosRecordFun.RecordDataEnd->add ) ;
                // RtosRecordFun.RecordDataEnd->flag = false ;
                // RtosRecordFun.RecordDataEnd = RtosRecordFun.RecordDataEnd->next ;
                // RtosRecordFun.RecordDataNum = 1 ;
            }
        }
        //rtos_vad_stop() ;
        if (pcm_open == true)
        {
            //  CLose PCM
            LOGI("Close PCM") ;
            pcm_open  = false ;
            // to do ......
            //
        }
        if (rec_data.cmd == RtosRecordCmd_Stop)
        {
            //  upload all left data
            int tmp_num = RtosRecordFun.RecordDataNum ;
            for (int i = 0; i < tmp_num ; ++i)
            {
                if ((tmp_num - 1) == i)
                {
                    LOGI("Last times data.") ;
//                    rtos_record_http_send(GptRecordHttpMsgCmd_STOP , type , RECORD_BUF_NODE_SIZE , (uint32_t)RtosRecordFun.RecordDataHead->add) ;
                    // RtosRecordFun.RecordDataHead->flag = false ;
                    // RtosRecordFun.RecordDataHead = RtosRecordFun.RecordDataEnd;

                }
                else
                {
//                    rtos_record_http_send(GptRecordHttpMsgCmd_RECORDING , type ,RECORD_BUF_NODE_SIZE , (uint32_t)RtosRecordFun.RecordDataHead->add  ) ;
                    // RtosRecordFun.RecordDataHead->flag = false ;
                    // RtosRecordFun.RecordDataHead = RtosRecordFun.RecordDataHead->next ;
                }
                // OS_MSleep(1) ;
                rt_thread_delay(1);
            }
            LOGI("Upload left data.") ;
            // RtosRecordFun.RecordDataNum = 0 ;
        }
        else if (rec_data.cmd == RtosRecordCmd_Cannel)
        {
            //  Clean all left data ,close http
            RtosRecordFun.RecordDataNum = 0 ;
//            rtos_record_http_send(GptRecordHttpMsgCmd_CANNEL , type , 0 , 0) ;
            LOGI("Clean up left data and clsoe http") ;
        }
        rt_thread_delay(50);

        if (rec_data.cmd == RtosRecordCmd_Stop)
        {
            // Rtos_Flash_Fun.gptTonePlay(SEND_WEBCHAT_END) ;
        }
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        rtos_record_timer_stop() ;
        rtos_record_private.RecordRun = false ;
    }
    rtos_record_private.Run = false ;
    rt_thread_delete(rtos_record_private.Thread);

    return ;
}

void rtos_record_data_deinit(void)
{

    rtos_record_private.Run = false ;
    rtos_record_private.RecordRun = false ;
    rtos_record_send(RtosRecordCmd_Cannel) ;
    while (rtos_record_private.Thread)
        rt_thread_delay(10);
    return ;
}

RtosRecordData_Def RtosRecordNodeData[RECORD_BUF_NODE_NUM] ;
static void rtos_record_data_init(void)
{
    LOGI("Record data init") ;
    for (int i = 0 ; i < RECORD_BUF_NODE_NUM; i++)
    {
        RtosRecordNodeData[i].flag = false,
                             RtosRecordNodeData[i].add = pcm_buf + i * RECORD_BUF_NODE_SIZE ;
        memset(pcm_buf + i * RECORD_BUF_NODE_SIZE, 0, RECORD_BUF_NODE_SIZE) ;
        if (i != 0)
        {
            RtosRecordNodeData[i - 1].next = &RtosRecordNodeData[i] ;
        }
        else
        {
            RtosRecordNodeData[i].next = NULL ;
        }
    }
    RtosRecordNodeData[RECORD_BUF_NODE_NUM - 1].next = &RtosRecordNodeData[0] ;
    RtosRecordFun.RecordDataHead = &RtosRecordNodeData[0] ;
    RtosRecordFun.RecordDataEnd = &RtosRecordNodeData[0] ;
    return ;
}


int rtos_record_send(rtos_record_cmd cmd)
{
    LOGI("Send msg to Record task") ;
    rtos_record_msg data  ;
    memset(&data, 0, sizeof(rtos_record_msg)) ;
    data.cmd = cmd ;
    rtos_msg_send(rtos_record_private.Queue, &data, sizeof(rtos_record_msg), 5) ;
    return 0 ;
}

void rtos_record_init(void)
{
    if (rtos_record_private.Run == true)
    {
        return ;
    }

    memset(&RtosRecordFun, 0, sizeof(RtosRecordFun_Def)) ;
    RtosRecordFun.RecordDataHead = NULL ;
    RtosRecordFun.RecordDataEnd = NULL ;
    RtosRecordFun.RecordDataNum = 0 ;
    RtosRecordFun.Send = rtos_record_send ;

    memset(&rtos_record_private, 0, sizeof(rtos_record_private_def)) ;
    rtos_record_private.RecordRun = false ;
    rtos_record_private.Run = true ;
    // rtos_record_private.RecordDataHead = NULL ;
    // rtos_record_private.RecordDataEnd = NULL ;
    // rtos_record_private.RecordDataNum = 0 ;
    rtos_msg_init(&rtos_record_private.Queue, "rtos_record", 5, sizeof(rtos_record_msg)) ;

    rtos_record_private.Thread = rt_thread_create("rtos_record_task",
                                 rtos_record_task, RT_NULL,
                                 RECORD_STACK_SIZE,
                                 RT_THREAD_PRIORITY_MAX / 2, 5);
    if (rtos_record_private.Thread == RT_NULL)
    {
        rt_kprintf("thread create error\n");
        return ;
    }
    else
        rt_thread_startup(rtos_record_private.Thread);

    LOGI("Create Record task OK") ;
    return ;
}
