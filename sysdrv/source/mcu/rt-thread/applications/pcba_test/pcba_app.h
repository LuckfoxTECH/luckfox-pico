/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-15 14:26:53
 * @Company: Rockchip
 * @LastEditTime: 2020-05-26 11:44:05
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __PCBA_APP_H__
#define __PCBA_APP_H__

#include <dfs_posix.h>
#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "audio_server.h"
#include "drivers/fuel_gauge.h"
#include "drivers/rtc.h"
#include "drv_display.h"
#include "drv_heap.h"
#include "hal_base.h"
#include "littlevgl2rtt.h"
#include "lvgl/lvgl.h"
#ifdef RT_USING_EXT_OPUS_DECODER
#include "opus_parser.h"
#endif
#include "unistd.h"
#include "rtconfig.h"

#include "display.h"
#include "pcba_disp.h"
#include "pcba_key.h"
#include "pcba_player.h"
#include "pcba_record.h"

#define FUEL_GAUGE_DEVICE_NAME          "fuel_gauge"

#define MAX_FILE_NAME                   128
#define RECORD_DIR_PATH                 PCBA_RECORD_FILE_DIR    /*"/pcba_test/"*/

#define PCBA_APP_MAIN_TASK_STACK_SIZE     2048
#define PCBA_APP_MAIN_TASK_PRIORITY       14

#define PCBA_APP_KEY_TASK_STACK_SIZE      512
#define PCBA_APP_KEY_TASK_PRIORITY        14

#define PCBA_APP_DISP_TASK_STACK_SIZE      4096
#define PCBA_APP_DISP_TASK_PRIORITY        14

#define DBG_HERE    LOG_D("run here %s: %d", __func__,__LINE__);

#define KEY_FUNC1               0x00000001
#define KEY_FUNC2               0x00000002
#define KEY_FUNC3               0x00000004
#define KEY_FUNC4               0x00000008
#define KEY_FUNC5               0x00000010
#define KEY_FUNC6               0x00000020
#define KEY_FUNC7               0x00000040
#define KEY_LONG_PRESS_START    0x10000000
#define KEY_LONG_PRESS          0x20000000
#define KEY_RELEASE             0x40000000

#define KEY_RECORD          (KEY_FUNC1)
#define KEY_PLAY            (KEY_FUNC5)
#define KEY_UP              (KEY_FUNC2)
#define KEY_DOWN            (KEY_FUNC3)
#define KEY_FAST_FORWARD    (KEY_FUNC3 | KEY_LONG_PRESS)
#define KEY_FAST_BACKWARD   (KEY_FUNC2 | KEY_LONG_PRESS_START | KEY_RELEASE)
#define KEY_DOWN_RELEASE    (KEY_FUNC3 | KEY_LONG_PRESS)
#define KEY_UP_RELEASE      (KEY_FUNC2 | KEY_LONG_PRESS_START | KEY_RELEASE)
#define KEY_MENU            (KEY_FUNC5 | KEY_LONG_PRESS_START)
#define KEY_OK              (KEY_FUNC5)
#define KEY_CANCEL          (KEY_FUNC5 | KEY_LONG_PRESS_START)
#define KEY_SELECT          (KEY_FUNC1)

#define REFRESH_DISP_TICK       (0)
#define REFRESH_MEDIA_INFO      (0x1UL << 0)
#define REFRESH_FILE_INFO       (0x1UL << 1)
#define REFRESH_DATE            (0x1UL << 2)
#define REFRESH_WIFI            (0x1UL << 3)
#define REFRESH_BT              (0x1UL << 4)
#define REFRESH_BAT             (0x1UL << 5)
#define REFRESH_PLAY_STATE      (0x1UL << 6)

enum pcba_test_page
{
    MENU_NONE_PAGE = -1,
    MENU_WELCOME_PAGE = 0,
    MENU_MAIN_PAGE,
    MENU_SCREEN_TEST_PAGE,
    MENU_KEY_TEST_PAGE,
    MENU_RECORD_TEST_PAGE,
    MENU_PHONE_SPEAK_TEST_PAGE,
    MENU_EMMC_STORAGE_TEST_PAGE,
    MENU_BATTERY_TEST_PAGE,
    MENU_TEMPERATE_TEST_PAGE,
    MENU_RTC_TEST_PAGE,
    MENU_WIFI_TEST_PAGE,
    MENU_BT_TEST_PAGE,
    MENU_RESULT_PAGE,
    MENU_MAX_PAGE,
};

enum pcba_test_msg_type
{
    TYPE_INIT_FINISH,
    TYPE_REFRESH,
    TYPE_SHOW_MENU,
    TYPE_KEY_PRESS,
    TYPE_PLAYBACK_DONE,
    TYPE_RECORD_DONE,
    TYPE_REBOOT,
};

enum pcba_test_state
{
    PCBA_TEST_STATE_INIT,
    PCBA_TEST_STATE_RUN,
    PCBA_TEST_STATE_IDLE,
};


enum pcba_playmode
{
    PLAYMODE_ONCE,
    PLAYMODE_SINGLECYCLE,
    PLAYMODE_LISTCYCLE,
    PLAYMODE_MAX
};

struct pcba_rec_file_info
{
    char cur_file_name[MAX_FILE_NAME];
    size_t total_file;
    size_t cur_file;
};

struct pcba_app_msg
{
    size_t type;
    size_t value;
};

struct pcba_app_st
{
    enum pcba_test_state state;
    enum pcba_test_msg_type page;

    rt_device_t fuel_gauge;
    struct rt_fg_status fg_status;

    recorder_handle_t recorder;
    recorder_state_t recorder_state;

    int record_seglen;
    uint32_t record_time;
    player_handle_t player;
    player_state_t player_state;
    int playmode;
    uint32_t player_cur_time;
    uint32_t player_total_time;
    uint8_t player_disable_callback;
    struct pcba_rec_file_info file;

    size_t battery;
    struct tm *date;
    time_t time;

    int16_t now_page;

    int backlight;
    int backlight_timeout;
    rt_device_t bl_device;
    rt_timer_t bl_timer;

    rt_mq_t main_mq;
    rt_mq_t display_mq;
};

extern struct pcba_app_st g_pcba;

#endif