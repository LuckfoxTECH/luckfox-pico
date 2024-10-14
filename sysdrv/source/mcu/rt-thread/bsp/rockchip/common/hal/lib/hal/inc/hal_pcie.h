/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_PCIE_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCIE
 *  @{
 */

#ifndef _HAL_PCIE_H_
#define _HAL_PCIE_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup PCIE_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

#define PHY_MODE_PCIE_AGGREGATION 4       /**< PCIE3x4 */
#define PHY_MODE_PCIE_NANBNB      0       /**< P1:PCIE3x2  +  P0:PCIE3x2 */
#define PHY_MODE_PCIE_NANBBI      1       /**< P1:PCIE3x2  +  P0:PCIE3x1*2 */
#define PHY_MODE_PCIE_NABINB      2       /**< P1:PCIE3x1*2 + P0:PCIE3x2 */
#define PHY_MODE_PCIE_NABIBI      3       /**< P1:PCIE3x1*2 + P0:PCIE3x1*2 */

#define PCIE_ATU_REGION_INDEX1 (0x1 << 0)
#define PCIE_ATU_REGION_INDEX0 (0x0 << 0)
#define PCIE_ATU_TYPE_MEM      (0x0 << 0)
#define PCIE_ATU_TYPE_IO       (0x2 << 0)
#define PCIE_ATU_TYPE_CFG0     (0x4 << 0)
#define PCIE_ATU_TYPE_CFG1     (0x5 << 0)

struct HAL_PHY_SNPS_PCIE3_DEV {
    uint32_t phyMode;
};

/** PCIe handler */
struct HAL_PCIE_DEV {
    uint32_t apbBase;
    uint32_t dbiBase;
    uint32_t cfgBase;
    uint8_t lanes;
    uint8_t gen;
    uint8_t firstBusNo;
    uint32_t legacyIrqNum;
    void *phy;
};

struct HAL_PCIE_HANDLE {
    struct HAL_PCIE_DEV *dev;
};
/** @} */
/***************************** Function Declare ******************************/
/** @defgroup PCIE_Public_Function_Declare Public Function Declare
 *  @{
 */

int HAL_PCIE_GetDmaStatus(struct HAL_PCIE_HANDLE *pcie, uint8_t chn, enum HAL_PCIE_DMA_DIR dir);
HAL_Status HAL_PCIE_ConfigDma(struct HAL_PCIE_HANDLE *pcie, struct DMA_TABLE *table);
HAL_Status HAL_PCIE_StartDma(struct HAL_PCIE_HANDLE *pcie, struct DMA_TABLE *table);
HAL_Check HAL_PCIE_LinkUp(struct HAL_PCIE_HANDLE *pcie);
uint32_t HAL_PCIE_GetLTSSM(struct HAL_PCIE_HANDLE *pcie);
HAL_Status HAL_PCIE_Init(struct HAL_PCIE_HANDLE *pcie, struct HAL_PCIE_DEV *dev);
HAL_Status HAL_PCIE_DeInit(struct HAL_PCIE_HANDLE *pcie);
HAL_Status HAL_PCIE_InboundConfig(struct HAL_PCIE_HANDLE *pcie, int32_t index, int32_t bar, uint64_t cpuAddr);
HAL_Status HAL_PCIE_OutboundConfig(struct HAL_PCIE_HANDLE *pcie, int32_t index, int type, uint64_t cpuAddr, uint64_t busAddr, uint32_t size);
int32_t HAL_PCIE_OutboundConfigCFG0(struct HAL_PCIE_HANDLE *pcie, HAL_PCI_DevT bdf, uint32_t size);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_PCIE_MODULE_ENABLED */
