/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oam log interface's header file.(in rom).
 * Author: Hisilicon
 * Create: 2020-08-04
 */

#ifndef __OAM_LOG_H__
#define __OAM_LOG_H__

#include "hi_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  10 º¯ÊýÉùÃ÷
*****************************************************************************/
hi_u32 oam_log_level_set(hi_u32 log_level);

hi_void oal_print_nlogs(
        const hi_char* pfile_name,
        const hi_char* pfuc_name,
        hi_u16         us_line_no,
        void*          pfunc_addr,
        hi_u8          clog_level,
        hi_u8          uc_param_cnt,
        hi_char*       fmt,
        hi_u32 p1, hi_u32 p2, hi_u32 p3, hi_u32 p4);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
#endif /* end of oam_log.h */
