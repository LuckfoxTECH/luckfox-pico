/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vicap_test.c
  * @version V0.0.1
  * @brief   camera interface processor test for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-10     ISP Team      first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_SWALLOW_RGB

#define SWALLOW_FPGA_TEST 0

#if SWALLOW_FPGA_TEST
#include "drv_mipi_dphy.h"
#include "camera.h"
#endif

void swallow_test_rgb()
{
#if SWALLOW_FPGA_TEST
    /* mipi dphy init */
    struct rk_mipi_dphy_device *mipi;
    int flag = 0;
    mipi = (struct rk_mipi_dphy_device *)rt_device_find("mipi_dphy");
    if (mipi == NULL)
    {
        rt_kprintf("get mipi device handle failed\n");
        return -1;
    }
    mipi->dev.ops->control(&mipi->dev, (int)1, (void *)&flag);

    struct rk_camera_device *camera;
    camera = (struct rk_camera_device *)rt_device_find("sensor_0");
    if (camera == NULL)
    {
        rt_kprintf("get sensor_0 device handle failed\n");
        return -1;
    }
    camera->ops->control(camera, (int)1, (void *)&flag);
#endif

}

#endif
