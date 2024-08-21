/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup USB_PHY
 *  @{
 */

#ifndef _HAL_USB_PHY_H_
#define _HAL_USB_PHY_H_

#include "hal_def.h"

/***************************** Structure Definition **************************/
/***************************** Function Declare ******************************/
/** @defgroup USB_PHY_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_USB_PhySuspend(void);
HAL_Status HAL_USB_PhyResume(void);
HAL_Status HAL_USB_PhyInit(void);
/** @} */

#endif /* _HAL_USB_PHY_H_ */

/** @} */

/** @} */

#endif /* HAL_PCD_MODULE_ENABLED || HAL_HCD_MODULE_ENABLED */
