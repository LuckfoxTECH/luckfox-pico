/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_coredump.h
  * @author
  * @version V0.1
  * @date    09-April-2020
  * @brief   coredump driver
  *
  ******************************************************************************
  */
#ifndef __DRV_COREDUMP_H_
#define __DRV_COREDUMP_H_

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>

#define COREDUMP_MAGIC_ID 0X12345678
#define REGS_NUM        32

typedef enum coredump_type
{
    CD_ASSERT,
    CD_EXCEPTION,
} coredump_type_t;

typedef enum coredump_dst
{
    CD_SDCARD,
    CD_FLASH,
    CD_UART,
    CD_NUMS,
} coredump_dst_t;

/* coredump_area must be memmap area access by ldr or str */
typedef struct coredump_area
{
    uint32_t magic_id;
    uint32_t start_addr;
    uint32_t end_addr;
    uint32_t cache;
    void (*prepare)(struct coredump_area *area);
    void *prv_data;
} coredump_area_t;

typedef int (*core_dump_data_prepare_t)(coredump_area_t *area);
typedef int (*core_dump_enter_t)(void);
typedef int (*core_dump_data_save_t)(char *buf, rt_size_t size);
typedef int (*core_dump_exit_t)(void);

typedef struct coredump_ops
{
    int os_type;
    coredump_type_t cd_type;
    core_dump_enter_t cd_enter;
    core_dump_data_save_t cd_data_save;
    core_dump_exit_t cd_exit;
} coredump_ops_t;

extern unsigned int cd_regs[];
extern coredump_area_t cd_area[];
extern int _stext[];
extern int _etext[];

void rt_save_context_to(void);
void register_coredump_ops(coredump_dst_t dst, coredump_ops_t *ops);
int rt_coredump_handler(enum coredump_type cd_type);
int rt_coredump_init(void);

#endif /* end of __DRV_COREDUMP_H_ */