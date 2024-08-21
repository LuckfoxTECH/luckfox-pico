/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_CPU_TOPOLOGY_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CPU_TOPOLOGY
 *  @{
 */

#ifndef HAL_CPU_TOPOLOGY_H
#define HAL_CPU_TOPOLOGY_H

/***************************** Function Declare ******************************/
/** @defgroup CPU_TOPOLOGY_Public_Function_Declare Public Function Declare
 *  @{
 */

/**
 * @brief  Get cpu id by mpidr.
 * @param  mpidr: cpu mpidr value.
 * @return cpu id.
 */
uint32_t HAL_CPU_TOPOLOGY_GetCpuIdByMpidr(int32_t mpidr);

/**
 * @brief  Get cpu id of the current cpu.
 * @return cpu id of the current cpu.
 */
uint32_t HAL_CPU_TOPOLOGY_GetCurrentCpuId(void);

/**
 * @brief  Get cpu affinity valule by cpu id.
 * @param  cpuId: cpu id.
 * @return cpu affinity valule.
 */
uint32_t HAL_CPU_TOPOLOGY_GetCpuAffByCpuId(uint32_t cpuId);

/** @} */

#endif

/** @} */

/** @} */

#endif
