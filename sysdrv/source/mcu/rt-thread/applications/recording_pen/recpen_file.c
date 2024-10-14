/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-24     Jair Wu      First version
 *
 */

#include "recpen_file.h"

#define DBG_SECTION_NAME    "FILE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"
#include "setting_backlight.h"
#include "setting_brightness.h"
#include "setting_rec_seglen.h"

void get_app_info(void)
{
    struct recpen_app_info app_info;
    FILE *fd;

    fd = fopen(APP_INFO_FILE, "rb");
    if (!fd)
    {
        g_app_info.brightness = BRIGHTNESS_HW_MAX;
        g_app_info.record_seglen = get_seglen();
        return;
    }
    fread((char *)&app_info, 1, sizeof(struct recpen_app_info), fd);
    if (app_info.language < RECPEN_APP_LANG_CN || app_info.language >= RECPEN_APP_LANG_MAX)
        g_app_info.language = RECPEN_APP_LANG_CN;
    else
        g_app_info.language = app_info.language;
    g_app_info.backlight_timeout = app_info.backlight_timeout;
    /* confirm validity */
    g_app_info.backlight_timeout = get_bl_timeout();
    g_app_info.brightness = app_info.brightness;
    if (g_app_info.brightness > BRIGHTNESS_HW_MAX)
        g_app_info.brightness = BRIGHTNESS_HW_MAX;
    if (g_app_info.brightness < BRIGHTNESS_HW_MIN)
        g_app_info.brightness = BRIGHTNESS_HW_MIN;

    if (app_info.playmode < PLAYMODE_ONCE || app_info.playmode >= PLAYMODE_MAX)
        g_app_info.playmode = PLAYMODE_ONCE;
    else
        g_app_info.playmode = app_info.playmode;
    g_app_info.record_seglen = app_info.record_seglen;
    /* confirm validity */
    g_app_info.record_seglen = get_seglen();
    g_app_info.file.cur_file = app_info.file.cur_file;
    fclose(fd);
}

void save_app_info(void)
{
    FILE *fd;

    fd = fopen(APP_INFO_FILE, "wb+");
    if (!fd)
        return;
    fwrite((char *)&g_app_info, 1, sizeof(struct recpen_app_info), fd);
    fclose(fd);
}

int rootfs_check(void)
{
    rt_device_t root_dev;
    int format = 0;

    root_dev = rt_device_find("root");
    RT_ASSERT(root_dev);
    if (dfs_filesystem_get_mounted_path(root_dev) == NULL)
    {
        LOG_W("root is not mounted");
        dfs_mkfs("elm", "root");
        if (dfs_mount("root", "/", "elm", 0, 0) < 0)
        {
            LOG_E("mount root failed");
            format |= ROOT_NO_MOUNTED;
        }
    }
#ifdef RT_SDCARD_MOUNT_POINT
    rt_device_t sd_dev;
    int retry = 0;
    if (access(RT_SDCARD_MOUNT_POINT, F_OK) < 0)
    {
        LOG_I("create sd mount point %s.", RT_SDCARD_MOUNT_POINT);
        mkdir(RT_SDCARD_MOUNT_POINT, 0);
    }

    /* Wait for sd0 be registed */
    do
    {
        sd_dev = rt_device_find("sd0");
        rt_thread_mdelay(10);
        retry++;
        if (retry > 100)
            break;
    }
    while (!sd_dev);
    if (sd_dev)
    {
        if (dfs_filesystem_get_mounted_path(sd_dev) == NULL)
        {
            if (dfs_mount("sd0", RT_SDCARD_MOUNT_POINT, "elm", 0, 0) < 0)
            {
                LOG_I("sd0 is not mounted");
                format |= SD0_NO_MOUNTED;
            }
        }
    }
#endif

    return format;
}

uint32_t check_audio_type(char *file_name)
{
    char *str;
    str = strstr(file_name, ".");
    if (str)
    {
        if (strcmp(str + 1, "wav") == 0)
            return RT_EOK;
        if (strcmp(str + 1, "mp3") == 0)
            return RT_EOK;
        if (strcmp(str + 1, "opus") == 0)
            return RT_EOK;
        if (strcmp(str + 1, "ogg") == 0)
            return RT_EOK;
    }

    return -RT_ERROR;
}

/* get audio file name in path with index */
uint32_t get_audio(const char *path, char *file_name, uint32_t index)
{
    DIR *dir = NULL;
    struct dirent *dr = NULL;
    uint32_t cnt = 0;

    dir = opendir(path);
    if (NULL == dir)
    {
        LOG_E("open dir %s fail", path);
        sprintf(file_name, "No recording file%c", '\0');
        return -RT_ERROR;
    }

    while (cnt < index)
    {
        dr = readdir(dir);
        if (dr != NULL)
        {
            if (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, ".."))
                continue;

            if (dr->d_type == 1)
            {
                LOG_D("dr->d_name=%s", dr->d_name);
                if (check_audio_type(dr->d_name) == RT_EOK)
                    cnt++;
            }
        }
        else
        {
            break;
        }
    }
    closedir(dir);
    if (dr)
    {
        LOG_D("found file %s.", dr->d_name);
        memcpy(file_name, dr->d_name, strlen(dr->d_name));
        file_name[strlen(dr->d_name)] = '\0';

        return RT_EOK;
    }
    if (cnt == 0)
        sprintf(file_name, "No recording file%c", '\0');

    return -RT_ERROR;
}

/* remove file and return left file count */
uint32_t remove_file(const char *path, char *file_name)
{
    char str[256];
    sprintf(str, "%s/%s", path, file_name);
    if (unlink(str) == 0)
        return scan_audio(path);

    return -RT_ERROR;
}

/* return audio file count in path */
uint32_t scan_audio(const char *path)
{
    DIR *dir = NULL;
    struct dirent *dr = NULL;
    uint32_t cnt = 0;
    int ret;

    ret = access(path, F_OK);
    if (ret < 0)
    {
        mkdir(path, 0);
        return 0;
    }

    dir = opendir(path);
    if (NULL == dir)
    {
        LOG_E("open dir %s fail", path);
        return 0;
    }

    LOG_D("open dir %s success :%p", path, dir);
    while (1)
    {
        dr = readdir(dir);
        if (dr != NULL)
        {
            if (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, ".."))
                continue;

            LOG_D("d_name = %s dtype = %d", dr->d_name, dr->d_type);

            if (dr->d_type == 1)
            {
                LOG_D("dr->d_name=%s", dr->d_name);
                if (check_audio_type(dr->d_name) == RT_EOK)
                    cnt++;
            }
        }
        if (dr == NULL)
        {
            closedir(dir);
            break;
        }
    }

    LOG_D("PATH [%s]: have %ld files", path, cnt);
    return cnt;
}

rt_err_t recpen_next_file()
{
    uint32_t index;
    if (g_app_info.file.cur_file < g_app_info.file.total_file)
        index = g_app_info.file.cur_file + 1;
    else
        index = 1;
    if (g_app_info.file.cur_file == index)
        return -RT_ERROR;
    if (get_audio(RECORD_DIR_PATH, g_app_info.file.cur_file_name, index) != RT_EOK)
        return -RT_ERROR;
    g_app_info.file.cur_file = index;
    save_app_info();

    return RT_EOK;
}

rt_err_t recpen_prev_file()
{
    uint32_t index;
    if (g_app_info.file.cur_file > 1)
        index = g_app_info.file.cur_file - 1;
    else
        index = g_app_info.file.total_file;
    if (g_app_info.file.cur_file == index)
        return -RT_ERROR;
    if (get_audio(RECORD_DIR_PATH, g_app_info.file.cur_file_name, index) != RT_EOK)
        return -RT_ERROR;
    g_app_info.file.cur_file = index;
    save_app_info();

    return RT_EOK;
}
