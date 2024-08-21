/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    adapter_type.h
  * @version V0.0.1
  * @brief   the module is to bridge data types for different os.
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#ifndef __ADAPTER_TYPE_H__
#define __ADAPTER_TYPE_H__

#if defined(__RT_THREAD__)
#include <rtdef.h>
#elif defined(__RK_OS__)
#include "rkos_typedef.h"
#endif

#if defined(__RT_THREAD__)

/* redefine the basic data type */
typedef rt_err_t                        ret_err_t;
typedef rt_size_t                       ret_size_t;
typedef rt_base_t                       dt_base_t;
typedef rt_ubase_t                      dt_ubase_t;
typedef rt_tick_t                       rk_tick_t;
typedef int                             dt_cmd_t;

/* redefine system err code */
#define RET_SYS_EOK                     (RT_EOK)
#define RET_SYS_ERROR                   (-RT_ERROR)
#define RET_SYS_ETIMEOUT                (-RT_ETIMEOUT)  /**< Timed out */
#define RET_SYS_EFULL                   (-RT_EFULL)     /**< The resource is full */
#define RET_SYS_EEMPTY                  (-RT_EEMPTY)    /**< The resource is empty */
#define RET_SYS_ENOMEM                  (-RT_ENOMEM)    /**< No memory */
#define RET_SYS_ENOSYS                  (-RT_ENOSYS)    /**< No system */
#define RET_SYS_EBUSY                   (-RT_EBUSY)     /**< Busy */
#define RET_SYS_EIO                     (-RT_EIO)       /**< IO error */
#define RET_SYS_EINTR                   (-RT_EINTR)     /**< Interrupted system call */
#define RET_SYS_EINVAL                  (-RT_EINVAL)    /**< Invalid argument */

#define RK_HW_CACHE_FLUSH                (RT_HW_CACHE_FLUSH)
#define RK_HW_CACHE_INVALIDATE           (RT_HW_CACHE_INVALIDATE)

#define RK_NULL                          RT_NULL
#define RK_IPC_FLAG_FIFO                 RT_IPC_FLAG_FIFO
#define RK_WAITING_FOREVER               RT_WAITING_FOREVER
#define MACRO_ASSERT(EX)                 RT_ASSERT(EX)

#elif defined(__RK_OS__)

/* redefine the basic data type */
typedef rk_err_t                        ret_err_t;      /**< Type for error number */
typedef unsigned long                   ret_size_t;
typedef long                            dt_base_t;
typedef unsigned long                   dt_ubase_t;
typedef unsigned long                   rk_tick_t;
typedef unsigned long                   dt_cmd_t;

/* redefine system err code */
#define RET_SYS_EOK                     (RK_SUCCESS)    /**< There is no error */
#define RET_SYS_ERROR                   (RK_ERROR)      /**< A generic error happens */
#define RET_SYS_ETIMEOUT                (RK_TIMEOUT)    /**< Timed out */
#define RET_SYS_EFULL                   (-6)            /**< The resource is full */
#define RET_SYS_EEMPTY                  (RK_EMPTY)      /**< The resource is empty */
#define RET_SYS_ENOMEM                  (-7)            /**< No memory */
#define RET_SYS_ENOSYS                  (-8)            /**< No system */
#define RET_SYS_EBUSY                   (-9)            /**< Busy */
#define RET_SYS_EIO                     (-10)           /**< IO error */
#define RET_SYS_EINTR                   (-11)           /**< Interrupted system call */
#define RET_SYS_EINVAL                  (RK_PARA_ERR)   /**< Invalid argument */
#define RET_SYS_EXIST                   (RK_EXIST)

#define RK_HW_CACHE_FLUSH                (0x01)
#define RK_HW_CACHE_INVALIDATE           (0x02)

#define RK_NULL                         (0)
#define RK_IPC_FLAG_FIFO                (0)
#define RK_WAITING_FOREVER              MAX_DELAY

#define MACRO_ASSERT(EX)                RK_ASSERT(EX)

#endif

#endif
