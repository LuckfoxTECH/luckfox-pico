/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_CPU_TOPOLOGY_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CPU_TOPOLOGY
 *  @{
 */

/** @defgroup CPU_TOPOLOGY_How_To_Use How To Use
 *  @{

 The CPU_TOPOLOGY driver can be used as follows:
 - Invoke HAL_CPU_TOPOLOGY_GetCpuIdByMpidr() to get cpu id by mpidr.
 - Invoke HAL_CPU_TOPOLOGY_GetCurrentCpuId() to get cpu id of the current cpu.
 - Invoke HAL_CPU_TOPOLOGY_GetCpuAffByCpuId() to get cpu affinity valule by cpu id.

 @} */

/** @defgroup CPU_TOPOLOGY_Private_Definition Private Definition
 *  @{
 */

/********************* Private Function Definition ***************************/
#if PLATFORM_CLUSTER1_CORE_COUNT
static uint32_t CPU_getCpuIdByMpidr(int32_t mpidr)
{
    uint32_t cpuId, clusterId;

    if (mpidr & MPIDR_MT_MASK) {
        cpuId = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
        clusterId = (mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK;
    } else {
        cpuId = (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;
        clusterId = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
    }

    if (!clusterId) {
        return cpuId;
    } else {
        return (PLATFORM_CLUSTER0_CORE_COUNT + cpuId);
    };
}

#else
static uint32_t CPU_getCpuIdByMpidr(int32_t mpidr)
{
    uint32_t cpuId;

    if (mpidr & MPIDR_MT_MASK) {
        cpuId = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
    } else {
        cpuId = (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;
    }

    return cpuId;
}
#endif

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup CPU_TOPOLOGY_Exported_Functions_Group5 Other Functions
 *  @{
 */
uint32_t HAL_CPU_TOPOLOGY_GetCpuIdByMpidr(int32_t mpidr)
{
    return CPU_getCpuIdByMpidr(mpidr);
}

uint32_t HAL_CPU_TOPOLOGY_GetCurrentCpuId(void)
{
    return CPU_getCpuIdByMpidr(__get_MPIDR());
}

uint32_t HAL_CPU_TOPOLOGY_GetCpuAffByCpuId(uint32_t cpuId)
{
    return CPU_GET_AFFINITY(cpuId, 0);
}
/** @} */

/** @} */

/** @} */

#endif
