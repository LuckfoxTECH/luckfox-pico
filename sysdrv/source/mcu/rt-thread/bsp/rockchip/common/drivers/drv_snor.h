/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_snor.h
  * @version V1.0
  * @brief   spi nor driver headfile
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Dingqiang Lin   the first version
  * 2019-06-27     Dingqiang Lin   support FSPI
  * 2020-09-23     Dingqiang Lin   Support scan and attach two independent spi nor
  *
  ******************************************************************************
  */

#ifdef RT_USING_SNOR

#ifndef __DRV_SFC_NOR_MTD_H__
#define __DRV_SFC_NOR_MTD_H__

#include <drivers/mtd_nor.h>

/*
 * Support case:
 *   one FSPI(SFC) host spiflash;
 *   one SPI host spiflash;
 *   one FSPI(SFC) host spiflash in dev 0, one SPI host spiflash in dev 1;
 * Currently we set SPIFLASH_DEV_NUM value 1 to decrease rw area space.
 */
#define SPIFLASH_DEV_NUM 1

enum spiflash_host
{
    TYPE_UNKNOWN,
    SPIFLASH_FSPI_HOST,
    SPIFLASH_SPI_HOST,
    SPIFLASH_SFC_HOST,
};

struct spiflash_device
{
    struct rt_mtd_nor_device dev;
    struct rt_mutex lock;
    struct SPI_NOR nor;
    struct SNOR_HOST host;
    enum spiflash_host type;
    rt_base_t level;
};

rt_err_t rk_snor_xip_suspend(void);
rt_err_t rk_snor_xip_resume(void);
rt_err_t rk_snor_suspend(void);
rt_err_t rk_snor_resume(void);
rt_err_t snor_read_uuid(struct rt_mtd_nor_device *dev, uint8_t *buf);

#endif

#endif