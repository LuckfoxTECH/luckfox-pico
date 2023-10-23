/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: sample common file.
 * Author: Hisilicon
 * Create: 2018-08-04
 */

#ifndef __SAMPLE_COMMON_H__
#define __SAMPLE_COMMON_H__

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "hi_types.h"
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define SAMPLE_CMD_MAX_LEN 1500

/*****************************************************************************
  3 枚举、结构体定义
*****************************************************************************/
typedef hi_s32(*sample_cmd_func)(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);
typedef struct {
    hi_char           *cmd_name;    /* 命令字符串 */
    sample_cmd_func      func;        /* 命令对应处理函数 */
} sample_cmd_entry_stru;

typedef struct {
    sample_cmd_entry_stru *cmd_tbl;   /* 命令表 */
    hi_u32               count;     /* 命令总数 */
} sample_cmd_common;

#define CMD_SENDMSG_NETCFG			0x01
#define CMD_SENDMSG_GETMAC			0x02
#define CMD_SENDMSG_GETIP			0x03
#define CMD_SENDMSG_SETFILTER			0x04	
#define CMD_SENDMSG_KEEPLIVE			0x05
#define CMD_SENDMSG_STANDBY			0x06
#define CMD_SENDMSG_DEEPSLEEP			0x07
#define CMD_SENDMSG_STARTAP			0x08
#define CMD_SENDMSG_STARTOTA			0x09
#define CMD_SENDMSG_OTADATA			0x0a
#define CMD_SENDMSG_OTAWRITERET			0x0b
#define CMD_SENDMSG_OTARET			0x0c
#define CMD_SENDMSG_GET_RSSI			0x0d
#define CMD_SENDMSG_GET_VERSION			0x0e
#define CMD_SENDMSG_GET_WAKECODE		0x0f

#define CMD_SENDMSG_FACTORY_RESET		0x10
#define CMD_SENDMSG_PIR_SET			0x11
#define CMD_SENDMSG_PIR_GET			0x12
#define CMD_SENDMSG_TUYA_SET			0x13
#define CMD_SENDMSG_GETALL			0x14
#define CMD_SENDMSG_PIR_CLR			0x17

/*****************************************************************************
  4 函数声明
*****************************************************************************/
hi_s32 sample_get_cmd_one_arg(const hi_char *pc_cmd, hi_char *pc_arg, hi_u32 pc_arg_len, hi_u32 *pul_cmd_offset);

hi_s32 sample_parse_cmd(hi_void *wdata, hi_char *cmd, ssize_t len, hi_void *msg);

hi_s32 sample_sock_cmd_entry(hi_void *wdata, const char *cmd, ssize_t len, hi_void *msg);

hi_s32 sample_register_cmd(sample_cmd_entry_stru *cmd_tbl, hi_u32 num);

#endif

