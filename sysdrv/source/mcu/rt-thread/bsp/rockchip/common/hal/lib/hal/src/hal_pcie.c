/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PCIE_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCIE
 *  @{
 */

/** @defgroup PCIE_How_To_Use How To Use
 *  @{

Thi PCIe drvier is code for dwc pcie controller driver.

The PCIe driver can be used as follows:

After u-boot or Linux PCIe initialization and enumeration process, we can using the HAL PCIe drivers to do the basically works, like:

- Check PCIe status:
    - Wait for PCIe link up by Calling HAL_PCIE_LinkUp()

- Using DMA in poll:
    - Calling HAL_PCIE_ConfigDma() to config uDMA block transfer table
    - Calling HAL_PCIE_StartDma() to start uDMA
    - Wait for uDMA finished by calling HAL_PCIE_GetDmaStatus()
- Configuration:
    - Setting iATU by calling HAL_PCIE_InboundConfig() and HAL_PCIE_OutboundConfig()
    - Setting outbound iATU for CFG by calling HAL_PCIE_OutboundConfigCFG0()

 @} */

/** @defgroup PCIE_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#ifdef PCIE_DEBUG
#define PCIE_DBG HAL_DBG
#else
#define PCIE_DBG
#endif

#define PCIE_CLIENT_LTSSM_STATUS 0x300
#define SMLH_LINKUP              BIT(16)
#define RDLH_LINKUP              BIT(17)

#define PCIE_ATU_OFFSET 0x300000

#define PCIE_ATU_ENABLE          BIT(31)
#define PCIE_ATU_BAR_MODE_ENABLE BIT(30)

#define PCIE_ATU_UNR_REGION_CTRL1 0x00
#define PCIE_ATU_UNR_REGION_CTRL2 0x04
#define PCIE_ATU_UNR_LOWER_BASE   0x08
#define PCIE_ATU_UNR_UPPER_BASE   0x0C
#define PCIE_ATU_UNR_LOWER_LIMIT  0x10
#define PCIE_ATU_UNR_LOWER_TARGET 0x14
#define PCIE_ATU_UNR_UPPER_TARGET 0x18
#define PCIE_ATU_UNR_UPPER_LIMIT  0x20

#define LINK_WAIT_IATU 9

#define PCIE_DMA_OFFSET 0x380000

#define PCIE_DMA_WR_ENB        0xc
#define PCIE_DMA_WR_CTRL_LO    0x200
#define PCIE_DMA_WR_CTRL_HI    0x204
#define PCIE_DMA_WR_XFERSIZE   0x208
#define PCIE_DMA_WR_SAR_PTR_LO 0x20c
#define PCIE_DMA_WR_SAR_PTR_HI 0x210
#define PCIE_DMA_WR_DAR_PTR_LO 0x214
#define PCIE_DMA_WR_DAR_PTR_HI 0x218
#define PCIE_DMA_WR_WEILO      0x18
#define PCIE_DMA_WR_WEIHI      0x1c
#define PCIE_DMA_WR_DOORBELL   0x10
#define PCIE_DMA_WR_INT_STATUS 0x4c
#define PCIE_DMA_WR_INT_MASK   0x54
#define PCIE_DMA_WR_INT_CLEAR  0x58

#define PCIE_DMA_RD_ENB        0x2c
#define PCIE_DMA_RD_CTRL_LO    0x300
#define PCIE_DMA_RD_CTRL_HI    0x304
#define PCIE_DMA_RD_XFERSIZE   0x308
#define PCIE_DMA_RD_SAR_PTR_LO 0x30c
#define PCIE_DMA_RD_SAR_PTR_HI 0x310
#define PCIE_DMA_RD_DAR_PTR_LO 0x314
#define PCIE_DMA_RD_DAR_PTR_HI 0x318
#define PCIE_DMA_RD_WEILO      0x38
#define PCIE_DMA_RD_WEIHI      0x3c
#define PCIE_DMA_RD_DOORBELL   0x30
#define PCIE_DMA_RD_INT_STATUS 0xa0
#define PCIE_DMA_RD_INT_MASK   0xa8
#define PCIE_DMA_RD_INT_CLEAR  0xac

#define BIT(nr) (1 << (nr))
/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

static inline void writel(uint32_t address, uint32_t value)
{
    *((volatile uint32_t *)address) = value;
}

static inline uint32_t readl(uint32_t address)
{
    return *((volatile uint32_t *)address);
}

static inline void HAL_PCIE_DbiWritel(struct HAL_PCIE_HANDLE *pcie, uint32_t reg, uint32_t val)
{
    writel(pcie->dev->dbiBase + reg, val);
}

static inline uint32_t HAL_PCIE_DbiReadl(struct HAL_PCIE_HANDLE *pcie, uint32_t reg)
{
    return readl(pcie->dev->dbiBase + reg);
}

static inline void HAL_PCIE_ApbWritel(struct HAL_PCIE_HANDLE *pcie, uint32_t reg, uint32_t val)
{
    writel(pcie->dev->apbBase + reg, val);
}

static inline uint32_t HAL_PCIE_ApbReadl(struct HAL_PCIE_HANDLE *pcie, uint32_t reg)
{
    return readl(pcie->dev->apbBase + reg);
}

static void HAL_PCIE_StartDmaWrite(struct HAL_PCIE_HANDLE *pcie, struct DMA_TABLE *cur, int ctrOffset)
{
    HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_RD_ENB, cur->enb.asdword);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_CTRL_LO, cur->ctxReg.ctrllo.asdword);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_CTRL_HI, cur->ctxReg.ctrlhi.asdword);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_XFERSIZE, cur->ctxReg.xfersize);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_SAR_PTR_LO, cur->ctxReg.sarptrlo);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_SAR_PTR_HI, cur->ctxReg.sarptrhi);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_DAR_PTR_LO, cur->ctxReg.darptrlo);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_RD_DAR_PTR_HI, cur->ctxReg.darptrhi);
    HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_RD_DOORBELL, cur->start.asdword);
}

static void HAL_PCIE_StartDmaRead(struct HAL_PCIE_HANDLE *pcie, struct DMA_TABLE *cur, int ctrOffset)
{
    HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_WR_ENB, cur->enb.asdword);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_CTRL_LO, cur->ctxReg.ctrllo.asdword);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_CTRL_HI, cur->ctxReg.ctrlhi.asdword);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_XFERSIZE, cur->ctxReg.xfersize);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_SAR_PTR_LO, cur->ctxReg.sarptrlo);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_SAR_PTR_HI, cur->ctxReg.sarptrhi);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_DAR_PTR_LO, cur->ctxReg.darptrlo);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_DAR_PTR_HI, cur->ctxReg.darptrhi);
    HAL_PCIE_DbiWritel(pcie, ctrOffset + PCIE_DMA_WR_WEILO, cur->weilo.asdword);
    HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_WR_DOORBELL, cur->start.asdword);
}

static int32_t HAL_PCIE_GetFreeOutboundAtu(struct HAL_PCIE_HANDLE *pcie)
{
    char i;
    uint32_t off;

    for (i = 0; i < 8; i++) {
        off = PCIE_ATU_OFFSET + 0x200 * i;
        if (!(HAL_PCIE_DbiReadl(pcie, off + PCIE_ATU_UNR_REGION_CTRL2) & PCIE_ATU_ENABLE)) {
            return i;
        }
    }

    return -1;
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup PCIE_Exported_Functions_Group2 State and Errors Functions
 *  @{
 */

/**
 * @brief  Get DMA status, Clear DMA status when done or abort.
 * @param  pcie: PCIe host.
 * @param  chn: PCIe DMA channal to check.
 * @param  dir: PCIe DMA channal's direction.
 * @return 1: DMA done; 0: No status; -1: DMA abort.
 */
int HAL_PCIE_GetDmaStatus(struct HAL_PCIE_HANDLE *pcie, uint8_t chn, enum HAL_PCIE_DMA_DIR dir)
{
    union PCIE_DMA_INT_STATUS status;
    union PCIE_DMA_INI_CLEAR clears;
    int ret = 0;

    if (dir == DMA_TO_BUS) {
        status.asdword = HAL_PCIE_DbiReadl(pcie, PCIE_DMA_OFFSET + PCIE_DMA_WR_INT_STATUS);
        if (status.donesta & BIT(chn)) {
            clears.doneclr = 0x1 << chn;
            HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_WR_INT_CLEAR, clears.asdword);
            ret = 1;
        }

        if (status.abortsta & BIT(chn)) {
            clears.abortclr = 0x1 << chn;
            HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_WR_INT_CLEAR, clears.asdword);
            ret = -1;
        }
    } else {
        status.asdword = HAL_PCIE_DbiReadl(pcie, PCIE_DMA_OFFSET + PCIE_DMA_RD_INT_STATUS);

        if (status.donesta & BIT(chn)) {
            clears.doneclr = 0x1 << chn;
            HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_RD_INT_CLEAR, clears.asdword);
            ret = 1;
        }

        if (status.abortsta & BIT(chn)) {
            clears.abortclr = 0x1 << chn;
            HAL_PCIE_DbiWritel(pcie, PCIE_DMA_OFFSET + PCIE_DMA_RD_INT_CLEAR, clears.asdword);
            ret = -1;
        }
    }

    return ret;
}

/** @} */

/** @defgroup PCIE_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  Config DMA table from user configuration to hw parameter.
 * @param  pcie: PCIe host.
 * @param  table: PCIe DMA table.
 * @return HAL_Status.
 */
HAL_Status HAL_PCIE_ConfigDma(struct HAL_PCIE_HANDLE *pcie, struct DMA_TABLE *table)
{
    table->enb.enb = 0x1;
    table->ctxReg.ctrllo.lie = 0x1;
    table->ctxReg.ctrllo.rie = 0x0;
    table->ctxReg.ctrllo.td = 0x1;
    table->ctxReg.ctrlhi.asdword = 0x0;
    table->ctxReg.xfersize = table->bufSize;
    if (table->dir == DMA_FROM_BUS) {
        table->ctxReg.sarptrlo = (uint32_t)(table->bus & 0xffffffff);
        table->ctxReg.sarptrhi = (uint32_t)(table->bus >> 32);
        table->ctxReg.darptrlo = (uint32_t)(table->local & 0xffffffff);
        table->ctxReg.darptrhi = (uint32_t)(table->local >> 32);
    } else if (table->dir == DMA_TO_BUS) {
        table->ctxReg.sarptrlo = (uint32_t)(table->local & 0xffffffff);
        table->ctxReg.sarptrhi = (uint32_t)(table->local >> 32);
        table->ctxReg.darptrlo = (uint32_t)(table->bus & 0xffffffff);
        table->ctxReg.darptrhi = (uint32_t)(table->bus >> 32);
    }
    table->weilo.weight0 = 0x0;
    table->start.stop = 0x0;
    table->start.chnl = table->chn;

    return HAL_OK;
}

/**
 * @brief  Start DMA transmition.
 * @param  pcie: PCIe host.
 * @param  table: PCIe DMA table.
 * @return HAL_Status.
 */
HAL_Status HAL_PCIE_StartDma(struct HAL_PCIE_HANDLE *pcie, struct DMA_TABLE *table)
{
    int dir = table->dir;
    int chn = table->chn;
    int ctrOffset = PCIE_DMA_OFFSET + chn * 0x200;

    if (dir == DMA_FROM_BUS) {
        HAL_PCIE_StartDmaWrite(pcie, table, ctrOffset);
    } else if (dir == DMA_TO_BUS) {
        HAL_PCIE_StartDmaRead(pcie, table, ctrOffset);
    }

    return HAL_OK;
}

/**
 * @brief  Check whether the link has been linked.
 * @param  pcie: PCIe host.
 * @return HAL_Check.
 */
HAL_Check HAL_PCIE_LinkUp(struct HAL_PCIE_HANDLE *pcie)
{
    uint32_t val;

    val = HAL_PCIE_ApbReadl(pcie, PCIE_CLIENT_LTSSM_STATUS);
    if ((val & (RDLH_LINKUP | SMLH_LINKUP)) == 0x30000) {
        return HAL_TRUE;
    }

    return HAL_FALSE;
}

/**
 * @brief  Get PCIe controller LTSSM.
 * @param  pcie: PCIe host.
 * @return uint32_t. LTSSM value.
 */
uint32_t HAL_PCIE_GetLTSSM(struct HAL_PCIE_HANDLE *pcie)
{
    return HAL_PCIE_ApbReadl(pcie, PCIE_CLIENT_LTSSM_STATUS);
}

/** @} */

/** @defgroup PCIE_Exported_Functions_Group4 Init and DeInit Functions
 *  @{
 */

/**
 * @brief  Initialial PCIe host.
 * @param  pcie: PCIe host.
 * @param  dev: PCIe device.
 * @return HAL_Status.
 */
HAL_Status HAL_PCIE_Init(struct HAL_PCIE_HANDLE *pcie, struct HAL_PCIE_DEV *dev)
{
    pcie->dev = dev;

    return HAL_OK;
}

/**
 * @brief  De-Initialial PCIe host.
 * @param  pcie: PCIe host.
 * @return HAL_Status.
 */
HAL_Status HAL_PCIE_DeInit(struct HAL_PCIE_HANDLE *pcie)
{
    return HAL_OK;
}

/** @} */

/** @defgroup PCIe_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Setting PCIe inbound atu.
 * @param  pcie: PCIe host.
 * @param  index: atu index.
 * @param  bar: bar index.
 * @param  cpuAddr: mapped memory address.
 * @return HAL_Status.
 */
HAL_Status HAL_PCIE_InboundConfig(struct HAL_PCIE_HANDLE *pcie, int32_t index, int32_t bar, uint64_t cpuAddr)
{
    uint32_t val, off;
    char i;

    off = PCIE_ATU_OFFSET + 0x200 * index + 0x100;
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_LOWER_TARGET, cpuAddr & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_UPPER_TARGET, (cpuAddr >> 32) & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_REGION_CTRL1, 0);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_REGION_CTRL2, PCIE_ATU_ENABLE | PCIE_ATU_BAR_MODE_ENABLE | (bar << 8));
    for (i = 0; i < 5000; i++) {
        val = HAL_PCIE_DbiReadl(pcie, off + PCIE_ATU_UNR_REGION_CTRL2);
        if (val & PCIE_ATU_ENABLE) {
            return HAL_OK;
        }
        HAL_DelayUs(LINK_WAIT_IATU);
    }

    return HAL_ERROR;
}

/**
 * @brief  Setting PCIe outbound atu.
 * @param  pcie: PCIe host.
 * @param  index: atu index.
 * @param  type: outbound type.
 * @param  cpuAddr: mapped memory address.
 * @param  busAddr: pci bus address.
 * @param  size: outbound atu size limit.
 * @return HAL_Status.
 */
HAL_Status HAL_PCIE_OutboundConfig(struct HAL_PCIE_HANDLE *pcie, int32_t index, int type, uint64_t cpuAddr, uint64_t busAddr, uint32_t size)
{
    uint32_t val, off;
    int32_t i;

    off = PCIE_ATU_OFFSET + 0x200 * index;
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_LOWER_BASE, cpuAddr & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_UPPER_BASE, (cpuAddr >> 32) & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_LOWER_LIMIT, (cpuAddr + size - 1) & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_LOWER_TARGET, busAddr & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_UPPER_TARGET, (busAddr >> 32) & 0xFFFFFFFF);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_REGION_CTRL1, type);
    HAL_PCIE_DbiWritel(pcie, off + PCIE_ATU_UNR_REGION_CTRL2, PCIE_ATU_ENABLE);
    for (i = 0; i < 5000; i++) {
        val = HAL_PCIE_DbiReadl(pcie, off + PCIE_ATU_UNR_REGION_CTRL2);
        if (val & PCIE_ATU_ENABLE) {
            return HAL_OK;
        }
        HAL_DelayUs(LINK_WAIT_IATU);
    }

    return HAL_ERROR;
}

/**
 * @brief  Setting PCIe config space outbound atu.
 * @param  pcie: PCIe host.
 * @param  dev: pci device.
 * @param  size: atu size.
 * @return -1 - fail, >= 0 return the atu index.
 */
int32_t HAL_PCIE_OutboundConfigCFG0(struct HAL_PCIE_HANDLE *pcie, HAL_PCI_DevT dev, uint32_t size)
{
    int32_t index;
    int b = PCI_BUS(dev) - pcie->dev->firstBusNo;
    int df;

    index = HAL_PCIE_GetFreeOutboundAtu(pcie);
    if (index < 0) {
        return -1;
    }

    df = PCI_MASK_BUS(dev);
    dev = PCI_ADD_BUS(b, df);
    HAL_PCIE_OutboundConfig(pcie, index, PCIE_ATU_TYPE_CFG0, pcie->dev->cfgBase, dev << 8, size);

    return index;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_PCIE_MODULE_ENABLED */
