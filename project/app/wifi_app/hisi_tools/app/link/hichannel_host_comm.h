/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: sample common file.
 * Author: Hisilicon
 * Create: 2018-08-04
 */

#ifndef __SAMPLE_COMMON_H__
#define __SAMPLE_COMMON_H__

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "hi_types.h"
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define SAMPLE_CMD_MAX_LEN 1500

/*****************************************************************************
  3 ö�١��ṹ�嶨��
*****************************************************************************/
typedef hi_s32(*sample_cmd_func)(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);
typedef struct {
    hi_char           *cmd_name;    /* �����ַ��� */
    sample_cmd_func      func;        /* �����Ӧ������ */
} sample_cmd_entry_stru;

typedef struct {
    sample_cmd_entry_stru *cmd_tbl;   /* ����� */
    hi_u32               count;     /* �������� */
} sample_cmd_common;

/*****************************************************************************
  4 ��������
*****************************************************************************/
hi_s32 sample_get_cmd_one_arg(const hi_char *pc_cmd, hi_char *pc_arg, hi_u32 pc_arg_len, hi_u32 *pul_cmd_offset);

hi_s32 sample_parse_cmd(hi_void *wdata, hi_char *cmd, ssize_t len, hi_void *msg);

hi_s32 sample_sock_cmd_entry(hi_void *wdata, const char *cmd, ssize_t len, hi_void *msg);

hi_s32 sample_register_cmd(sample_cmd_entry_stru *cmd_tbl, hi_u32 num);

#endif

