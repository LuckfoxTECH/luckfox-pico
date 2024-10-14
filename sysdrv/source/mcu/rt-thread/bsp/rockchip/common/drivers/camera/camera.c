/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    camera.c
  * @version V0.0.1
  * @brief   camera device abstract for rk625
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup Vicap
 *  @{
 */

/** @addtogroup Camera
 *  @{
 */

/** @defgroup Camera_How_To_Use How To Use
 *  @{

 The camera driver is used to implement image sensor driver.The driver provides
 a unified framework based on the struct rk_camera_device for implementing image
 sensor driver.The struct rk_camera_device needs to be embedded into a larger user-defined
 struct of sensor driver.The work must be completed for author of sensor driver is belowed:

- (A) Implement ops field of struct rk_camera_device.
- (B) Assign the i2c bus name for sensor in i2c_name field of struct rk_camera_device.
- (C) Implement the control command in the control interface of ops field which is embedded
      in struct rk_camera_device.
- (D) Register image sensor with rk_camera_register(...).

 @} */

#if defined(__RT_THREAD__)
#include "camera.h"
#elif defined(__RK_OS__)
#include "driver/camera.h"
#endif

#ifdef CAMERA_MODULE_COMPILED

#if defined(__RK_OS__)
#include <string.h>
#include <stdlib.h>
#endif

/********************* Public Function Definition ****************************/

/** @defgroup CAMERA_Public_Functions Public Functions
 *  @{
 */

#if defined(__RT_THREAD__)

/**
 * @brief  Control the camera device for application.
 * @param dev : The camera device object in kernel.
 * @param cmd : The command to control camera device.
 * @param args : The parameter of command.
 * @return : return system status.
 */
static ret_err_t rk_camera_control(rk_device *dev,
                                   dt_cmd_t cmd,
                                   void *args)
{
    struct rk_camera_device *camera;

    MACRO_ASSERT(dev != RK_NULL);
    camera = (struct rk_camera_device *)dev;
    if (cmd <= RK_DEVICE_CTRL_CAMERA_CTRL ||
            cmd == RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG ||
            cmd == RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE ||
            cmd == RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL)
    {
        if (camera->ops)
            return (camera->ops->control(camera, cmd, args));
    }
    else
    {
        if (camera->ctrl)
            return (camera->ctrl->control(camera->ctrl->ctrl_dev, cmd, args));
    }
}

/**
 * @brief  Init the camera device for application.
 * @param dev : The camera device object in kernel.
 * @return : return system status.
 */
static ret_err_t rk_camera_init(rk_device *dev)
{
    struct rk_camera_device *camera;

    MACRO_ASSERT(dev != RK_NULL);

    camera = (struct rk_camera_device *)dev;
    if (camera->ops->init)
    {
        return (camera->ops->init(camera));
    }

    return RET_SYS_ENOSYS;
}

/**
 * @brief  Init the camera ctrl device for application.
 * @param dev : The camera ctrl device object in kernel.
 * @return : return system status.
 */
ret_err_t rk_camera_ctrl_init(struct rk_camera_device *camera, struct rk_camera_ctrl *ctrl)
{
    MACRO_ASSERT(ctrl != RK_NULL);
    MACRO_ASSERT(camera != RK_NULL);
    camera->ctrl = ctrl;
    if (camera->ctrl->init)
    {
        return (camera->ctrl->init(ctrl->ctrl_dev));
    }

    return RET_SYS_ENOSYS;
}

/**
 * @brief  Open the camera device for application.
 * @param dev : The camera device object in kernel.
 * @param oflag : The open flag of camera device.
 * @return : return system status.
 */
static ret_err_t rk_camera_open(rk_device *dev, uint16_t oflag)
{
    struct rk_camera_device *camera;

    MACRO_ASSERT(dev != RK_NULL)

    camera = (struct rk_camera_device *)dev;
    if (camera->ops->open)
    {
        return (camera->ops->open(camera, RT_DEVICE_OFLAG_RDWR));
    }
    return RET_SYS_ENOSYS;
}

/**
 * @brief  Close the camera device for application.
 * @param dev : The camera device object in kernel.
 * @return : return system status.
 */
static ret_err_t rk_camera_close(rk_device *dev)
{
    struct rk_camera_device *camera;

    MACRO_ASSERT(dev != RK_NULL);

    camera = (struct rk_camera_device *)dev;
    if (camera->ops->close)
    {
        return (camera->ops->close(camera));
    }

    return RET_SYS_ENOSYS;
}


const static struct rt_device_ops rk_camera_dev_ops =
{
    rk_camera_init,
    rk_camera_open,
    rk_camera_close,
    RK_NULL,
    RK_NULL,
    rk_camera_control
};

/**
 * @brief  register camera device on RT-Thread OS.
 * @param camera : The camera device to be registered.
 * @param name : The name of registered camera.
 * @param data : The private data of camera device.
 * @return : return system status.
 */
ret_err_t rk_rtthread_camera_register(struct rk_camera_device *camera,
                                      const char *name,
                                      void *data)
{
    uint32_t ret;
    rk_device *device;
    MACRO_ASSERT(camera != RK_NULL);

    device = &(camera->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RK_NULL;
    device->tx_complete = RK_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rk_camera_dev_ops;
#else

    device->init        = rk_camera_init;
    device->open        = rk_camera_open;
    device->close       = rk_camera_close;
    device->read        = RK_NULL;
    device->write       = RK_NULL;
    device->control     = rk_camera_control;
#endif
    device->user_data   = data;

    ret = rt_device_register(device, name, RT_DEVICE_FLAG_STANDALONE);

    return ret;

}

#elif defined(__RK_OS__)

/**
 * @brief  Control the camera device for application.
 * @param dev : The camera device object in kernel.
 * @param cmd : The command to control camera device.
 * @param args : The parameter of command.
 * @return : return system status.
 */
static ret_err_t rk_camera_control(HDC dev,
                                   dt_cmd_t cmd,
                                   void *args)
{
    struct rk_camera_device *camera;

    MACRO_ASSERT(dev != RK_NULL);
    camera = (struct rk_camera_device *)dev;

    return (camera->ops->control(camera, cmd, args));
}

/**
 * @brief  create camera device instance on RKOS OS.
 * @param dev_id : The device object id of camera device to be registered.
 * @param arg : The private data of camera device.
 * @return : return device handle.
 */
HDC rk_rkos_camera_create(uint8_t dev_id, void *arg)
{
    struct rk_camera_device *camera = (struct rk_camera_device *)arg;
    rk_device *dev = &camera->parent;

    dev->dev_class_id = DEV_CLASS_CAMERA;
    dev->dev_object_id = dev_id;
    dev->suspend = RK_NULL;
    dev->resume = RK_NULL;
    dev->control = rk_camera_control;
    dev->priv_data = arg;
    rk_strncpy(dev->name, camera->name, RK_NAME_MAX);

    return dev;
}

/**
 * @brief  delete camera device instance on RKOS OS.
 * @param dev_id : The device object id of camera device to be deleted.
 * @param arg : The private data of camera device.
 * @return : return system status.
 */
ret_err_t rk_rkos_camera_delete(uint8_t dev_id, void *arg)
{
    struct rk_camera_device *camera = (struct rk_camera_device *)arg;
    rk_device *dev = &camera->parent;

    dev->control = RK_NULL;
    dev->priv_data = arg;

#ifdef CONFIG_FWANALYSIS_SEGMENT
    FW_RemoveSegment(SEGMENT_ID_CAMERA_DEV);
#endif

    return RET_SYS_EOK;

}

/**
 * @brief  register camera device on RKOS OS.
 * @param camera : The camera device to be registered.
 * @param dev_object_id : The device object id of camera device.
 * @param data : The private data of camera device.
 * @return : return system status.
 */
ret_err_t rk_rkos_camera_register(struct rk_camera_device *camera,
                                  uint8_t dev_object_id,
                                  void *data)
{
    ret_err_t ret;
    rk_device *dev = RK_NULL;

    ret = rkdev_create(DEV_CLASS_CAMERA, dev_object_id, data);
    if (ret != RET_SYS_EOK)
    {
        rk_kprintf("camera create failure\n");
        return RET_SYS_ERROR;
    }

    dev = (rk_device *)rkdev_find(DEV_CLASS_CAMERA, dev_object_id);
    if (dev)
    {
        if ((uint32_t)dev != (uint32_t)camera)
            rk_kprintf("the create camera is not matched!\n");
        return RET_SYS_ERROR;
    }
    rk_kprintf("camera create success\n");
    return RET_SYS_EOK;
}

#endif

/**
 * @brief  register camera device for application.
 * @param camera : The camera device to be registered.
 * @param name : The name of the camera to be registered.
 * @param data : The private data of camera device.
 * @return : return system status.
 */
ret_err_t rk_camera_register(struct rk_camera_device *camera,
                             const char *name,
                             void *data)
{
    ret_err_t ret;

#if defined(__RT_THREAD__)
    ret = rk_rtthread_camera_register(camera, name, data);
#elif defined(__RK_OS__)
    uint8_t dev_id = 0;
    char *tmp;

    tmp = strstr(name, "_");
    tmp += 1;
    dev_id = (uint8_t)atoi(tmp);
    ret = rk_rkos_camera_register(camera, dev_id, data);
#endif

    return ret;

}

int pltfrm_camera_module_pix_fmt2csi2_dt(int src_pix_fmt)
{
    int ret = 0;

    switch (src_pix_fmt)
    {
    case V4L2_MBUS_FMT_RGB444_2X8_PADHI_BE:
    case V4L2_MBUS_FMT_RGB444_2X8_PADHI_LE:
        ret = CSI2_DT_RGB444;
        break;
    case V4L2_MBUS_FMT_RGB555_2X8_PADHI_BE:
    case V4L2_MBUS_FMT_RGB555_2X8_PADHI_LE:
        ret = CSI2_DT_RGB555;
        break;
    case V4L2_MBUS_FMT_BGR565_2X8_BE:
    case V4L2_MBUS_FMT_BGR565_2X8_LE:
    case V4L2_MBUS_FMT_RGB565_2X8_BE:
    case V4L2_MBUS_FMT_RGB565_2X8_LE:
        ret = CSI2_DT_RGB565;
        break;
    case V4L2_MBUS_FMT_RGB666_1X18:
        ret = CSI2_DT_RGB666;
        break;
    case V4L2_MBUS_FMT_RGB888_1X24:
    case V4L2_MBUS_FMT_RGB888_2X12_BE:
    case V4L2_MBUS_FMT_RGB888_2X12_LE:
        ret = CSI2_DT_RGB888;
        break;
    case V4L2_MBUS_FMT_Y8_1X8:
    case V4L2_MBUS_FMT_UV8_1X8:
    case V4L2_MBUS_FMT_UYVY8_1_5X8:
    case V4L2_MBUS_FMT_VYUY8_1_5X8:
    case V4L2_MBUS_FMT_YUYV8_1_5X8:
    case V4L2_MBUS_FMT_YVYU8_1_5X8:
    case V4L2_MBUS_FMT_UYVY8_2X8:
    case V4L2_MBUS_FMT_VYUY8_2X8:
    case V4L2_MBUS_FMT_YUYV8_2X8:
    case V4L2_MBUS_FMT_YVYU8_2X8:
        ret = CSI2_DT_YUV420_8b;
        break;
    case V4L2_MBUS_FMT_Y10_1X10:
    case V4L2_MBUS_FMT_YUYV10_2X10:
    case V4L2_MBUS_FMT_YVYU10_2X10:
        ret = CSI2_DT_YUV420_10b;
        break;
    case V4L2_MBUS_FMT_Y12_1X12:
        ret = CSI2_DT_YUV420_10b;
        break;
    case V4L2_MBUS_FMT_UYVY8_1X16:
    case V4L2_MBUS_FMT_VYUY8_1X16:
    case V4L2_MBUS_FMT_YUYV8_1X16:
    case V4L2_MBUS_FMT_YVYU8_1X16:
    case V4L2_MBUS_FMT_YDYUYDYV8_1X16:
        ret = CSI2_DT_YUV422_8b;
        break;
    case V4L2_MBUS_FMT_YUYV10_1X20:
    case V4L2_MBUS_FMT_YVYU10_1X20:
    case V4L2_MBUS_FMT_YUV10_1X30:
        ret = CSI2_DT_YUV422_10b;
        break;
    case V4L2_MBUS_FMT_SBGGR8_1X8:
    case V4L2_MBUS_FMT_SGBRG8_1X8:
    case V4L2_MBUS_FMT_SGRBG8_1X8:
    case V4L2_MBUS_FMT_SRGGB8_1X8:
        ret = CSI2_DT_RAW8;
        break;
    case V4L2_MBUS_FMT_SBGGR10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SGBRG10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SGRBG10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SRGGB10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SBGGR10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SGBRG10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SGRBG10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SRGGB10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADHI_BE:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADHI_LE:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADLO_BE:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADLO_LE:
    case V4L2_MBUS_FMT_SBGGR10_1X10:
    case V4L2_MBUS_FMT_SGBRG10_1X10:
    case V4L2_MBUS_FMT_SGRBG10_1X10:
    case V4L2_MBUS_FMT_SRGGB10_1X10:
        ret = CSI2_DT_RAW10;
        break;
    case V4L2_MBUS_FMT_SBGGR12_1X12:
    case V4L2_MBUS_FMT_SGBRG12_1X12:
    case V4L2_MBUS_FMT_SGRBG12_1X12:
    case V4L2_MBUS_FMT_SRGGB12_1X12:
        ret = CSI2_DT_RAW12;
        break;
    default:
        break;
    }
    return ret;
}

#endif

/** @} */  // CAMERA_Public_Functions

/** @} */  // Camera

/** @} */  // VICAP

/** @} */  // RKBSP_Common_Driver
