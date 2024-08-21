/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-14     armink       the first version
 */

#define DBG_TAG "cmb_log"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <rtthread.h>
#include <stdio.h>
#include <cm_backtrace.h>

//#define CMB_USING_FAL_FLASH_LOG
//#define CMB_USING_FAL_BACKUP_LOG_TO_FILE

#if defined(CMB_USING_FAL_FLASH_LOG)

#if !defined(PKG_USING_FAL) || !defined(RT_USING_DFS)
#error "please enable the FAL package and DFS component"
#endif

#include <fal.h>
#include <dfs_posix.h>

#ifndef CMB_FAL_FLASH_LOG_PART
#define CMB_FAL_FLASH_LOG_PART         "cmb_log"
#endif

#ifndef CMB_LOG_FILE_PATH
#define CMB_LOG_FILE_PATH              "/log/cmb.log"
#endif

/* cmb flash log partition write granularity, default: 8 bytes */
#ifndef CMB_FLASH_LOG_PART_WG
#define CMB_FLASH_LOG_PART_WG          8
#endif

/* the log length's size which saved in flash */
#define CMB_LOG_LEN_SIZE               MAX(sizeof(size_t), CMB_FLASH_LOG_PART_WG)

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif
#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

static const struct fal_partition *cmb_log_part = NULL;

/**
 * write cmb log to flash partition @see CMB_FLASH_LOG_PART
 *
 * @param log log buffer
 * @param len log length
 */
void cmb_flash_log_write(const char *log, size_t len)
{
    static uint32_t addr = 0;
    uint8_t len_buf[CMB_LOG_LEN_SIZE] = { 0 };
    static rt_bool_t first_write = RT_TRUE;

    if (first_write)
    {
        fal_partition_erase_all(cmb_log_part);
        first_write = RT_FALSE;
    }

    /* write log length */
    rt_memcpy(len_buf, (uint8_t *)&len, sizeof(size_t));
    fal_partition_write(cmb_log_part, addr, len_buf, sizeof(len_buf));
    addr += CMB_LOG_LEN_SIZE;
    /* write log content */
    fal_partition_write(cmb_log_part, addr, (uint8_t *)log, len);
    addr += RT_ALIGN(len, CMB_FLASH_LOG_PART_WG);
}

#if defined(RT_USING_ULOG)

static void ulog_cmb_flash_log_backend_output(struct ulog_backend *backend, rt_uint32_t level, const char *tag, rt_bool_t is_raw,
        const char *log, size_t len)
{
    if (!rt_strcmp(tag, CMB_LOG_TAG))
    {
        cmb_flash_log_write(log, len);
    }
}

int ulog_cmb_flash_log_backend_init(void)
{
    static struct ulog_backend backend;

    cmb_log_part = fal_partition_find(CMB_FAL_FLASH_LOG_PART);
    RT_ASSERT(cmb_log_part != NULL);

    backend.init = RT_NULL;
    backend.output = ulog_cmb_flash_log_backend_output;

    ulog_backend_register(&backend, "cmb_flash_log", RT_FALSE);

    return 0;
}
INIT_APP_EXPORT(ulog_cmb_flash_log_backend_init);

#else
void cmb_flash_log_println(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    va_start(args, fmt);

    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1 - 2)
        length = RT_CONSOLEBUF_SIZE - 3;

    /* add CRLF */
    rt_log_buf[length++] = '\r';
    rt_log_buf[length++] = '\n';

    cmb_flash_log_write(rt_log_buf, length);

    va_end(args);
}
#endif /* RT_USING_ULOG */


#ifdef CMB_USING_FAL_BACKUP_LOG_TO_FILE
int cmb_backup_flash_log_to_file(void)
{
    cmb_log_part = fal_partition_find(CMB_FAL_FLASH_LOG_PART);
    RT_ASSERT(cmb_log_part != NULL);

    size_t len;
    uint32_t addr = 0;
    rt_bool_t has_read_log = RT_FALSE;
    int log_fd = -1;

    while (1)
    {
        fal_partition_read(cmb_log_part, addr, (uint8_t *)&len, sizeof(size_t));
        if (len != 0xFFFFFFFF)
        {
            char log_buf[ULOG_LINE_BUF_SIZE];

            if (!has_read_log)
            {
                has_read_log = RT_TRUE;
                LOG_I("An CmBacktrace log was found on flash. Now will backup it to file ("CMB_LOG_FILE_PATH").");
                //TODO check the folder
                log_fd = open(CMB_LOG_FILE_PATH, O_WRONLY | O_CREAT | O_APPEND);
                if (log_fd < 0) {
                    LOG_E("Open file ("CMB_LOG_FILE_PATH") failed.");
                    break;
                }
            }
            addr += CMB_LOG_LEN_SIZE;
            /* read log content */
            fal_partition_read(cmb_log_part, addr, (uint8_t *)log_buf, MIN(ULOG_LINE_BUF_SIZE, len));
            addr += RT_ALIGN(len, CMB_FLASH_LOG_PART_WG);
            /* backup log to file */
            write(log_fd, log_buf, MIN(ULOG_LINE_BUF_SIZE, len));
        }
        else
        {
            break;
        }
    }

    if (has_read_log)
    {
        if (log_fd >= 0)
        {
            LOG_I("Backup the CmBacktrace flash log to file ("CMB_LOG_FILE_PATH") successful.");
            close(log_fd);
            fal_partition_erase_all(cmb_log_part);
        }
    }

    return 0;
}
INIT_APP_EXPORT(cmb_backup_flash_log_to_file);
#endif /* CMB_USING_FAL_BACKUP_LOG_TO_FILE */

#endif /* defined(CMB_USING_FLASH_LOG_BACKUP) */
