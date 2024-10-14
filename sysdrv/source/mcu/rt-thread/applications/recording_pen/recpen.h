/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-04     Jair Wu      First version
 *
 */

#ifndef __RECPEN_H__
#define __RECPEN_H__

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

#include "dialog_box.h"
#include "display.h"
#include "recpen_display.h"
#include "recpen_file.h"
#include "recpen_key.h"
#include "recpen_player.h"
#include "recpen_record.h"
#include "toast.h"

#define FUEL_GAUGE_DEVICE_NAME          "fuel_gauge"

#define MAX_FILE_NAME                   128
#define RECORD_DIR_PATH                 RECPEN_RECORD_FILE_DIR
#define APP_INFO_FILE                   "recpen_app.inf"
#define MAX_LABEL_LEN                   32

#define RECPEN_MAIN_TASK_STACK_SIZE     2048
#define RECPEN_MAIN_TASK_PRIORITY       20

#define RECPEN_KEY_TASK_STACK_SIZE      512
#define RECPEN_KEY_TASK_PRIORITY        16

#define RECPEN_DISPLAY_TASK_STACK_SIZE  4096
#define RECPEN_DISPLAY_TASK_PRIORITY    15

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
#define KEY_DOWN            (KEY_FUNC3)
#define KEY_UP              (KEY_FUNC2)
#define KEY_FAST_FORWARD    (KEY_FUNC3 | KEY_LONG_PRESS)
#define KEY_FAST_BACKWARD   (KEY_FUNC2 | KEY_LONG_PRESS_START | KEY_RELEASE)
#define KEY_DOWN_RELEASE    (KEY_FUNC3 | KEY_LONG_PRESS)
#define KEY_UP_RELEASE      (KEY_FUNC2 | KEY_LONG_PRESS_START | KEY_RELEASE)
#define KEY_MENU            (KEY_FUNC5 | KEY_LONG_PRESS_START)
#define KEY_OK              (KEY_FUNC5)
#define KEY_CANCEL          (KEY_FUNC5 | KEY_LONG_PRESS_START)
#define KEY_SELECT          (KEY_FUNC1)

#ifndef MIN
#define MIN(x,y) (x < y ? x : y)
#endif
#ifndef MAX
#define MAX(x,y) (x > y ? x : y)
#endif

#define REFRESH_DISP_TICK       (0)
#define REFRESH_MEDIA_INFO      (0x1UL << 0)
#define REFRESH_FILE_INFO       (0x1UL << 1)
#define REFRESH_DATE            (0x1UL << 2)
#define REFRESH_WIFI            (0x1UL << 3)
#define REFRESH_BT              (0x1UL << 4)
#define REFRESH_BAT             (0x1UL << 5)
#define REFRESH_PLAY_STATE      (0x1UL << 6)

#define EVENT_VBUS_PLUGIN       (0x1UL << 0)
#define EVENT_VBUS_PLUGOUT      (0x1UL << 1)
#define EVENT_POWER_KEY_RAISE   (0x1UL << 2)
#define EVENT_POWER_KEY_FALL    (0x1UL << 3)
#define EVENT_POWER_KEY_LPRESS  (0x1UL << 4)

enum recpen_app_menu_page
{
    MENU_NONE_PAGE = -1,
    MENU_WELCOME_PAGE = 0,
    MENU_MAIN_PAGE,
    MENU_SETTING_PAGE,
    MENU_SETTING_DATE_PAGE,
    MENU_SETTING_LANGUAGE_PAGE,
    MENU_SETTING_BACKLIGHT_PAGE,
    MENU_SETTING_BL_TIMEOUT_PAGE,
    MENU_SETTING_BL_BRIGHTNESS_PAGE,
    MENU_SETTING_PLAYMODE_PAGE,
    MENU_SETTING_RECORD_PAGE,
    MENU_SETTING_REC_SEGLEN_PAGE,
    MENU_REC_PAGE,
    MENU_UDISK_PAGE,
    MENU_FILE_PAGE,
    MENU_MAX_PAGE,
};

enum recpen_app_msg_type
{
    TYPE_INIT_FINISH,
    TYPE_REFRESH,
    TYPE_SHOW_MENU,
    TYPE_KEY_PRESS,
    TYPE_PLAYBACK_DONE,
    TYPE_RECORD_DONE,
    TYPE_REBOOT,
};

enum recpen_app_state
{
    RECPEN_APP_STATE_INIT,
    RECPEN_APP_STATE_IDLE,
    RECPEN_APP_STATE_BUSY,
};

enum recpen_language
{
    RECPEN_APP_LANG_CN,
    RECPEN_APP_LANG_EN,
    RECPEN_APP_LANG_MAX,
};

enum recpen_playmode
{
    PLAYMODE_ONCE,
    PLAYMODE_SINGLECYCLE,
    PLAYMODE_LISTCYCLE,
    PLAYMODE_MAX
};

struct recpen_label
{
    char text[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN];
    lv_obj_t *label;
    void (*func)(void);
    int value;
};

struct recpen_app_msg
{
    size_t type;
    size_t value;
};

struct recpen_file_info
{
    char cur_file_name[MAX_FILE_NAME];
    size_t total_file;
    size_t cur_file;
};

struct recpen_app_info
{
    enum recpen_app_state state;
    enum recpen_app_menu_page page;
    uint8_t language;

    rt_device_t fuel_gauge;
    struct rt_fg_status fg_status;
    size_t bat;
    bool charging;

    struct tm *date;
    time_t time;

    recorder_handle_t recorder;
    recorder_state_t recorder_state;
    int record_seglen;
    uint32_t record_time;
    player_handle_t player;
    player_state_t player_state;
    int playmode;
    uint32_t player_cur_time;
    uint32_t player_total_time;
    struct recpen_file_info file;

#ifdef RT_USING_VOP
    rt_display_data_t disp;

    uint8_t *fb;
    uint32_t fblen;
#endif
    uint16_t now_page;

    int backlight;
    int backlight_timeout;
    int brightness;
    rt_device_t bl_device;
    rt_timer_t bl_timer;

    rt_mq_t main_mq;
    rt_mq_t display_mq;
    int waiting;
    rt_sem_t wait_sem;
};

extern struct recpen_app_info g_app_info;

#endif // __RECPEN_H__
