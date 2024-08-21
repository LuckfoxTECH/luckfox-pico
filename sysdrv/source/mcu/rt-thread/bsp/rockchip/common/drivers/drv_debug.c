/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_debug.c
  * @version V0.1
  * @brief   Support HAL debug
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-08-21     Tao Huang       first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup Debug
 *  @{
 */

#include <rthw.h>
#include "hal_base.h"

#ifdef RT_USING_CMBACKTRACE
#include "cm_backtrace.h"
#endif

/********************* Public Function Definition ****************************/

/** @defgroup Debug_Public_Functions Public Functions
 *  @{
 */

/**
  * @brief  Override HAL_AssertFailed
  * @param  file: pointer to the source file name
  * @param  line: HAL_ASSERT error line source number
  * @retval None
  */
void HAL_AssertFailed(const char *file, uint32_t line)
{
    HAL_DBG_ERR("assert failed at %s %lu\n", file, line);
#ifdef RT_USING_CMBACKTRACE
    cm_backtrace_assert(cmb_get_sp());
#endif
    while (1)
        ;
}

/** @} */  // Debug_Public_Functions

/** @} */  // Debug

/** @} */  // RKBSP_Driver_Reference
