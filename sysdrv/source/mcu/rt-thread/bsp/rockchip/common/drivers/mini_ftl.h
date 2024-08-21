/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    mini_ftl.h
  * @version V1.0
  * @brief   spi nand special mini ftl head file
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-23     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#ifndef MINI_FTL_H__
#define MINI_FTL_H__

int mftl_mtd_read(rt_mtd_t *mtd, loff_t from, struct mtd_io_desc *ops);
int mftl_mtd_write(rt_mtd_t *mtd, loff_t to, struct mtd_io_desc *ops);
int mftl_mtd_erase(rt_mtd_t *mtd, loff_t addr, size_t len);

int mini_ftl_register(struct mtd_info *mtd);

#endif