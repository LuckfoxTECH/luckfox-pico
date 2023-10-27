/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
* Description: file hi_types.h.
* Author: HiSilicon
* Create: 2019-4-3
*/
/**
* @file hi_types.h
*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2018. All rights reserved. \n
*
* 数据类型说明
*/

#ifndef __HI_TYPES_H__
#define __HI_TYPES_H__

#include <hi_types_base.h>
#include <hi_errno.h>


/* linux错误码 */
#define OAL_SUCC                0
#define OAL_EFAIL               1   /* 内核通用错误返回值 -1 */
#define OAL_EIO                 5   /* I/O error */
#define OAL_ENOMEM              12  /* Out of memory */
#define OAL_EFAUL               14  /* Bad address */
#define OAL_EBUSY               16  /* Device or resource busy */
#define OAL_ENODEV              19  /* No such device */
#define OAL_EINVAL              22  /* Invalid argument */

#define oal_reference(data) ((void)(data))


#endif // __HI_TYPES_H__

