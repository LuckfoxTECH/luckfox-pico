/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oam log interface's implementation.(in rom).
 * Author: Hisilicon
 * Create: 2020-08-04
 */

/*****************************************************************************
    1 头文件包含
*****************************************************************************/
#include "oam_ext_if.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PRINT   printk

hi_void oal_print_nlogs(
        const hi_char* pfile_name,
        const hi_char* pfuc_name,
        hi_u16         us_line_no,
        void*          pfunc_addr,
        hi_u8          clog_level,
        hi_u8          uc_param_cnt,
        hi_char*       fmt,
        hi_u32 p1, hi_u32 p2, hi_u32 p3, hi_u32 p4)
{
    hi_char buffer[OAM_PRINT_FORMAT_LENGTH] = {0};
    hi_char* level = NULL;

    hi_unref_param(pfile_name);
    hi_unref_param(pfuc_name);
    hi_unref_param(pfunc_addr);

    if (clog_level > oam_get_log_level()) {
        return;
    }
    switch (clog_level) {
        case OAM_LOG_LEVEL_INFO:
            level = "[INFO]";
            break;
        case OAM_LOG_LEVEL_WARNING:
            level = "[WARN]";
            break;
        case OAM_LOG_LEVEL_ERROR:
            level = "[ERROR]";
            break;
        default:
            break;
    }

    if (level == HI_NULL) {
        return;
    }

    if (snprintf_s(buffer, OAM_PRINT_FORMAT_LENGTH, OAM_PRINT_FORMAT_LENGTH - 1,
        "%s:%d:%s \r\n", level, us_line_no, fmt) == -1) {
        return;
    }

    switch (uc_param_cnt) {
        case 0: /* case 0 param_cnt */
            printk(buffer);
            break;
        case 1: /* case 1 param_cnt */
            printk(buffer, p1);
            break;
        case 2: /* case 2 param_cnt */
            printk(buffer, p1, p2);
            break;
        case 3: /* case 3 param_cnt */
             printk(buffer, p1, p2, p3);
            break;
        case 4: /* case 4 param_cnt */
             printk(buffer, p1, p2, p3, p4);
            break;
        default:
            break;
    }
    return;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

