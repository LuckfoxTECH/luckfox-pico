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

#include "rkdef.h"
#include "drv_keyctrl.h"
#include "keyctrl_task.h"
#include "globe_system_stat.h"
#include "flash_play.h"
#include "sys_ctrl.h"

#define DBG_SECTION_NAME            "KEY_CTRL"
#include  "rkdebug.h"

#if (defined RT_USING_BOARD_RK2108B_EVB) || (defined RT_USING_BOARD_RK2108_EVB)
#define KEYCTRL_BUTTON_NUM      0x8

#define KEYCTRL_KEY_MAP_CODE0   0x1
#define KEYCTRL_KEY_MAP_CODE1   0x2
#define KEYCTRL_KEY_MAP_CODE2   0x3
#define KEYCTRL_KEY_MAP_CODE3   0x4
#define KEYCTRL_KEY_MAP_CODE4   0x5
#define KEYCTRL_KEY_MAP_CODE5   0x6
#define KEYCTRL_KEY_MAP_CODE6   0x7
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE0   0x8
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE1   0x9
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE2   0xa
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE3   0xb
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE4   0xc
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE5   0xd
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE6   0xe
#define KEYCTRL_KEY_MAP_COMBINATION_CODE0   0xf     /* KEY0 and KEY1 combination */

typedef struct
{
    uint16_t    long_Preess_value;
    uint16_t    short_Press_value;
    KEY_CTRL_ID key_Id;
} KEY_Crtl_Info;

KEY_Crtl_Info KEY_Ctrl_Register[KEYCTRL_BUTTON_NUM] =
{
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE0, KEYCTRL_KEY_MAP_CODE0, KEY_CTRL_0},
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE1, KEYCTRL_KEY_MAP_CODE1, KEY_CTRL_1},
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE2, KEYCTRL_KEY_MAP_CODE2, KEY_CTRL_2},
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE3, KEYCTRL_KEY_MAP_CODE3, KEY_CTRL_3},
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE4, KEYCTRL_KEY_MAP_CODE4, KEY_CTRL_4},
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE5, KEYCTRL_KEY_MAP_CODE5, KEY_CTRL_5},
    {KEYCTRL_KEY_MAP_LONG_TIME_CODE6, KEYCTRL_KEY_MAP_CODE6, KEY_CTRL_6},
    {0,                   KEYCTRL_KEY_MAP_COMBINATION_CODE0, KEY_CTRL_COMB},
};

static KEYCTRL_CMD_INFO keyCtrlCmd;
static uint8 keyCtrlVal = 0;
KEY_CTRL_ID keyCtrlID = KEY_CTRL_NUM;
uint32_t key_Ctrl_Press_Time = 0;
uint8_t key_Ctrl_Is_Trigger = 0;

static void rtos_test_fun(KEYCTRL_CMD_INFO button_test) ;
static void rtos_local_ota(void);


static int key_ctrl_send_vkey(KEYCTRL_CMD_INFO *data)
{
    if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_PowerOff)
    {
        return 0 ;
    }

    if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_OTA)
    {
        // printf("Current System stat is GptSystemStat_Ctrl_Stat_OTA\n");
        return 0 ;
    }

    if (RtosTestModeFun.TestMode)
    {
        rtos_test_fun(*data) ;
        return 0 ;
    }
    // forbidden key control function during sysntem init.
    if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_Init)
    {
        LOGD("Current System stat is GptSystemStat_Ctrl_Stat_Init\n");
        return 0 ;
    }
    // if(RtosSystemStat_Fun.GetSystemStat() == GptSystemStat_Ctrl_Stat_NetInit){
    //  printf("forbidden key control function during sysntem init\n");
    //  return 0 ;
    // }

    // printf("===Key control System:%d===\n", RtosSystemStat_Fun.GetSystemStat());
    if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_SetWifi)
    {
        return 0 ;
    }
    if (sys_event_send(CTRL_MSG_TYPE_VKEY, CTRL_MSG_SUB_TYPE_AD_BUTTON, (uint32_t)data, 0) != 0)
    {
        LOGE("send vkey error\n");
        return -1;
    }
    return 0;
}

#define MAX_TIME 4294967295
uint32_t key_ctrl_d_time(uint32_t time1, uint32_t time2)
{
    uint32_t d_time = 0;
    if (time1 <= time2)
        d_time = time2 - time1;
    else
        d_time = MAX_TIME - time1 + time2;
    return d_time;
}

static void key_ctrl_short_press(KEY_Crtl_Info *button)
{
    if (key_Ctrl_Is_Trigger == 0)
    {
        //LOGD("%s: Enter \n",__func__);
        KEYCTRL_CMD_INFO   *p = &keyCtrlCmd;
        p->cmd = KEY_CTRL_CMD_SHORT_PRESS;
        p->id = button->key_Id;
        LOGD("%s: key_ctrl_send_vkey [cmd:%d, id:%d] ", __func__, p->cmd, p->id);
        key_ctrl_send_vkey(p);
    }
}

void key_ctrl_long_press(KEY_Crtl_Info *button)
{
    KEYCTRL_CMD_INFO *p = &keyCtrlCmd;
    p->cmd = KEY_CTRL_CMD_LONG_PRESS;
    p->id = button->key_Id;

    if (key_Ctrl_Is_Trigger == 0)
    {
        //LOGD("%s: Enter ",__func__);
        if (RtosTestModeFun.TestFlag)
        {
            //LOGD("%s: 1111111 ",__func__);
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Test) ;
            // Rtos_Flash_Fun.gptTonePlay(ENTER_TEST_MODE) ;
            RtosTestModeFun.TestFlag = false ;
        }
        else if (RtosTestModeFun.OTAFlag)
        {
            //LOGD("%s: 2222222 ",__func__);
            RtosTestModeFun.OTA = true ;
            rtos_local_ota() ;
        }
        else
        {
            LOGD("%s: key_ctrl_send_vkey [cmd:%d, id:%d] ", __func__, p->cmd, p->id);
            key_ctrl_send_vkey(p);
        }

        key_Ctrl_Is_Trigger = 1;
    }
}

static void key_ctrl_release_cmd(KEY_Crtl_Info *button)
{
    if (key_Ctrl_Is_Trigger)
    {
        KEYCTRL_CMD_INFO *p = &keyCtrlCmd;
        p->cmd = KEY_CTRL_CMD_RELEASE;
        p->id = button->key_Id;
        key_ctrl_send_vkey(p);
        LOGD("%s: key_ctrl_release_cmd [cmd:%d, id:%d] ", __func__, p->cmd, p->id);
    }
}

static void key_ctrl_combination(KEY_Crtl_Info *button)
{
    KEYCTRL_CMD_INFO *p = &keyCtrlCmd;
    p->cmd = KEY_CTRL_CMD_COMBINATION;
    p->id = button->key_Id;
    key_ctrl_send_vkey(p);
}


//extern int otaMode;
void key_ctrl_task(void *arg)
{
    LOGD(" Enter");
    KEY_CTRL_ID keyctrl_id = KEY_CTRL_NUM;

    while (1)
    {
        KEY_Crtl_Info *keyctrl_info = NULL;
        if (/*!otaMode &&*/ keyCtrlID != KEY_CTRL_NUM && keyCtrlID != KEY_CTRL_ALL_RELEASE)
        {
            if (keyCtrlID != keyctrl_id && key_Ctrl_Is_Trigger == 0)
            {
                if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_Init &&
                        keyCtrlID == BOARD_TEST_BUTTON_ID)
                {
                    RtosTestModeFun.TestFlag = true ;
                }
                else if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_Init &&
                         keyCtrlID == BOARD_OTA_BUTTON_ID)
                {
                    //RtosTestModeFun.OTAFlag = true ;
                }
                else
                {
                }

                keyctrl_id = keyCtrlID;
            }
        }
        else if (/*!otaMode &&*/ keyctrl_id != KEY_CTRL_NUM && keyCtrlID == KEY_CTRL_ALL_RELEASE)
        {
            if (keyCtrlVal == 0)
            {
                keyctrl_info = &KEY_Ctrl_Register[keyctrl_id];
                LOGD("release");
                key_ctrl_release_cmd(keyctrl_info);
                keyctrl_id = KEY_CTRL_NUM;
                key_Ctrl_Is_Trigger = 0;
            }
        }

        if (/*!otaMode &&*/ keyctrl_id != KEY_CTRL_NUM && keyCtrlVal != 0 &&
                            keyCtrlVal < KEYCTRL_KEY_MAP_LONG_TIME_CODE0 &&
                            keyCtrlID != KEY_CTRL_ALL_RELEASE)
        {
            //RELEASE
            keyctrl_info = &KEY_Ctrl_Register[keyctrl_id];
            key_ctrl_short_press(keyctrl_info);

            //keyctrl_id = keyCtrlID;
            keyctrl_id = KEY_CTRL_NUM;
            keyCtrlID = KEY_CTRL_ALL_RELEASE;
            key_Ctrl_Is_Trigger = 0;
        }

        if (/*!otaMode &&*/ keyctrl_id != KEY_CTRL_NUM &&
                            keyCtrlVal > KEYCTRL_KEY_MAP_CODE6 &&
                            keyCtrlVal < KEYCTRL_KEY_MAP_COMBINATION_CODE0)
        {
            keyctrl_info = &KEY_Ctrl_Register[keyctrl_id];
            key_ctrl_long_press(keyctrl_info);
            //keyctrl_id = KEY_CTRL_NUM;
        }

        /* combination key */
        if (/*!otaMode &&*/ keyctrl_id != KEY_CTRL_NUM &&
                            keyCtrlVal == KEYCTRL_KEY_MAP_COMBINATION_CODE0)
        {
            keyctrl_info = &KEY_Ctrl_Register[keyctrl_id];
            LOGD("combination pressed ");
            key_ctrl_combination(keyctrl_info);
        }

        rt_thread_delay(10);
    }
}

static int key_ctrl_get_id(uint8 keyVale)
{
    int i = 0;
    int keyID = KEY_CTRL_NUM;

    if (keyVale == 0)
        return keyID;

    for (i = 0; i < KEY_CTRL_NUM; i++)
    {
        if (KEY_Ctrl_Register[i].short_Press_value == keyVale || KEY_Ctrl_Register[i].long_Preess_value == keyVale)
        {
            keyID = KEY_Ctrl_Register[i].key_Id;
            break;
        }
    }

    return keyID;
}

static rt_err_t keyctrl_get_Key_value_cb(rt_device_t dev, rt_size_t size)
{
    uint8_t ch;
    rt_size_t i;
    rt_err_t ret = RT_EOK;

//    rt_kputs("keyctrl_get_Key_value_cb Enter \n");

    if (size == 0)  //means key long press release ???
    {
        keyCtrlVal =  0;    // no key press
        keyCtrlID = KEY_CTRL_ALL_RELEASE;
//        rt_kputs("keyCtrlID = KEY_CTRL_ALL_RELEASE \n");
    }

    for (i = 0; i < size; i++)
    {
        /* read a char */
        if (rt_device_read(dev, 0, &ch, 1))
        {
            keyCtrlVal =  ch;
            keyCtrlID = key_ctrl_get_id(ch);
//            rt_kputs("dev read \n");
        }
        else
        {
            ret = rt_get_errno();
            //keyCtrlID = KEY_CTRL_ALL_RELEASE;
//            rt_kputs("dev read error\n");
        }
    }

    return ret;
}

static rt_device_t keyctrl_dev = RT_NULL;
static rt_uint8_t *read_buffer  = RT_NULL;

static int key_ctrl_device_init()
{
    rt_err_t result;
    struct rt_keyctrl_info info;

    keyctrl_dev = rt_device_find("keyctrl");
    RT_ASSERT(keyctrl_dev != RT_NULL);

    result = rt_device_control(keyctrl_dev, RT_KEYCTRL_CTRL_GET_INFO, &info);
    if (result != RT_EOK)
    {
        rt_kprintf("device : %s cmd RT_KEYCTRL_CTRL_GET_INFO failed\n",
                   keyctrl_dev->parent.name);
        return result;
    }

    read_buffer = rt_malloc(info.count);
    if (read_buffer == RT_NULL)
    {
        rt_kprintf("no memory for read_buffer!\r\n");
        return -RT_ENOMEM;
    }

    result = rt_device_init(keyctrl_dev);
    if (result != RT_EOK)
    {
        rt_kprintf("To initialize device:%s failed. The error code is %d\n",
                   keyctrl_dev->parent.name, result);
        return result;
    }

    result = rt_device_open(keyctrl_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if (result != RT_EOK)
    {
        rt_kprintf("To open device:%s failed. The error code is %d\n",
                   keyctrl_dev->parent.name, result);
        return result;
    }

    rt_device_set_rx_indicate(keyctrl_dev, keyctrl_get_Key_value_cb);

    return RT_EOK;
}
#endif


#ifdef RT_USING_BOARD_AUDIO_DEMO_RK2108_V10
#include "hal_base.h"
uint8_t gRun = 1;
void key_task(void *arg)
{
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_B6,
                         PIN_CONFIG_MUX_FUNC0);

    KEYCTRL_CMD_INFO data;
    while (1)
    {
        rt_thread_delay(10);
        if (HAL_GPIO_GetPinLevel(GPIO1, GPIO_PIN_B6) == GPIO_LOW && gRun)
        {
            LOGI("========================Yes, Get gpio key==================\n");
            Rtos_Flash_Fun.gptTonePlay(TONE_SETUP_NET_CODE) ;
            rt_thread_delay(200);

            data.cmd = KEY_CTRL_CMD_SHORT_PRESS;
            data.id = KEY_CTRL_NUM; // send a virtual key value
            if (sys_event_send(CTRL_MSG_TYPE_VKEY, CTRL_MSG_SUB_TYPE_AD_BUTTON, (uint32_t)&data, 0) != 0)
            {
                LOGE("send vkey error\n");
                return -1;
            }

            gRun = 0;
        }
        else if (HAL_GPIO_GetPinLevel(GPIO1, GPIO_PIN_B6) == GPIO_HIGH)
        {
            gRun = 1;
        }

    }
}
#endif


rt_thread_t g_key_ctrl_thread;
#define KEY_CTRL_THREAD_STACK_SIZE  (1024)

void key_ctrl_init()
{
#if (defined RT_USING_BOARD_RK2108B_EVB) || (defined RT_USING_BOARD_RK2108_EVB)
    int ret = RT_ERROR;
    ret = key_ctrl_device_init();
    if (ret != RT_EOK)
    {
        LOGD("Key Ctrl device init Fail");
        return;
    }

    g_key_ctrl_thread = rt_thread_create("key_ctrl_task",
                                         key_ctrl_task,
                                         RT_NULL,
                                         KEY_CTRL_THREAD_STACK_SIZE,
                                         RT_THREAD_PRIORITY_MAX / 2 - 2,
                                         10);

#elif (defined RT_USING_BOARD_AUDIO_DEMO_RK2108_V10)
#warning "No key ctrl in this board type."

    g_key_ctrl_thread = rt_thread_create("key_task",
                                         key_task,
                                         RT_NULL,
                                         KEY_CTRL_THREAD_STACK_SIZE,
                                         RT_THREAD_PRIORITY_MAX / 2 - 2,
                                         10);

#endif

    if (g_key_ctrl_thread != RT_NULL)
    {
        LOGD("create key_ctrl_task OK");
        rt_thread_startup(g_key_ctrl_thread);
    }

}

void key_ctrl_deinit()
{
#if (defined RT_USING_BOARD_RK2108B_EVB) || (defined RT_USING_BOARD_RK2108_EVB)
    if (read_buffer)
    {
        rt_free(read_buffer);
        read_buffer = RT_NULL;
    }
    rt_device_set_rx_indicate(keyctrl_dev, RT_NULL);
    rt_device_close(keyctrl_dev);
    keyctrl_dev = RT_NULL;
    rt_thread_delete(g_key_ctrl_thread);
#endif

    LOGD("end");
}

//---------------------------------------------------------------------------
RT_UNUSED static void rtos_test_fun(KEYCTRL_CMD_INFO button_test)
{
}

//RtosTestModeFun.WifiStat
RT_UNUSED static void rtos_test_fun_wifi(void)
{
    // static bool gpt_test_fun_wifi_flag = false ;
    // if(gpt_test_fun_wifi_flag){
    //  return ;
    // }
    // gpt_test_fun_wifi_flag = true ;
    // while(RtosTestModeFun.WifiStat == 0){
    //  OS_MSleep(100) ;
    //  // printf("Test Wifi return...........\n");
    // }
    // if(RtosTestModeFun.WifiStat == 1){
    //  //  扫描成功
    //      Rtos_Flash_Fun.gptTonePlay(WIFI_TEST_SUCCESS) ;
    // }else{
    //  //  扫描失败
    //  Rtos_Flash_Fun.gptTonePlay(WIFI_TEST_FAIL) ;
    // }
    return;
}

RT_UNUSED static void rtos_ota_function(void)
{
    // ota_status_t stat;

    // if ((stat =ota_get_image(OTA_PROTOCOL_FILE, SD_OTA_URL)) == OTA_STATUS_OK) {
    //  if (ota_verify_image(OTA_VERIFY_NONE, NULL)  != OTA_STATUS_OK) {
    //      printf("OTA file verify image failed\n");
    //      Rtos_Flash_Fun.gptTonePlay(TONE_OTA_ERR) ;

    //  }
    //  else
    //  {
    //      printf("\n--- ota--- ok!\n");
    //      Rtos_Flash_Fun.gptTonePlay(TONE_OTA_OK) ;

    //  }
    // }
    // else{
    //  printf("\n----- ota  error!\n");
    //  if(stat == OTA_STATUS_ERROR)
    //  {
    //      Rtos_Flash_Fun.gptTonePlay(TONE_OTA_ERR) ;
    //  }
    //  else
    //  {
    //      Rtos_Flash_Fun.gptTonePlay(TONE_OTA_ERR) ;
    //  }

    // }
    // rtthread_delay(100);
    // ota_reboot();
    return ;
}

RT_UNUSED static void rtos_local_ota(void)
{
    // RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat,SystemStat_Ctrl_Stat_OTA) ;
    // if(rtos_sd_ota_check() == -1)
    // {
    //  cmd_upgrade_exec(NULL) ;
    // }else{
    //  Rtos_Flash_Fun.gptTonePlay(TONE_OTA_START) ;
    //  rtos_ota_function() ;
    // }

    return ;
}
