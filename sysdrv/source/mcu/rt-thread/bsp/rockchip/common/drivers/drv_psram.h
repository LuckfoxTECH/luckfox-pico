/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_psram.h
  * @version V1.0
  * @brief   psram interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-30     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#ifndef __DRV_PSRAM_H__
#define __DRV_PSRAM_H__

#if defined(RT_USING_QPIPSRAM_FSPI_HOST_CS3)
#define RK_PSRAM_MAX_DEV    4
#elif defined(RT_USING_QPIPSRAM_FSPI_HOST_CS2)
#define RK_PSRAM_MAX_DEV    3
#elif defined(RT_USING_QPIPSRAM_FSPI_HOST_CS1)
#define RK_PSRAM_MAX_DEV    2
#else
#define RK_PSRAM_MAX_DEV    1
#endif

typedef enum
{
    RK_PSRAM_STATE_NODEV = 0,
    RK_PSRAM_STATE_BUSY,
} rk_psram_state;

struct rk_psram_device
{
    struct rt_device parent;

    rt_uint32_t size; /* Byte */

    struct rk_psram_driver_ops *ops;
    rk_psram_state state;
    void *priv;
};

struct rk_psram_driver_ops
{
    rt_size_t (*read)(struct rk_psram_device *device, rt_off_t offset, rt_uint8_t *data, rt_uint32_t length);
    rt_size_t (*write)(struct rk_psram_device *device, rt_off_t offset, const rt_uint8_t *data, rt_uint32_t length);
    rt_err_t (*suspend)(struct rk_psram_device *device);
    rt_err_t (*resume)(struct rk_psram_device *device);
};

rt_size_t rk_psram_read(struct rk_psram_device *dev, rt_off_t pos, rt_uint8_t *data, rt_size_t size);
rt_size_t rk_psram_write(struct rk_psram_device *dev, rt_off_t pos, const rt_uint8_t *data, rt_size_t size);

/*
 * Psram PM operation, no input param
 */
rt_err_t rk_psram_suspend(void);
rt_err_t rk_psram_resume(void);

rt_err_t rk_psram_register(struct rk_psram_driver_ops *ops, rt_uint32_t size, char *name, void *priv);

#endif
