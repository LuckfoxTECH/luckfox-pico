/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PCIE_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCI
 *  @{
 */

/** @defgroup PCI_How_To_Use How To Use
 *  @{

 The PCI driver is the core code file for PCI device driver.

 This driver providing basic support for the PCI compliant configuration, and error handling of PCI devices.

 @} */

/** @defgroup PCI_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

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

static uint32_t pci_conv_32_to_size(uint32_t value, uint32_t offset, uint8_t size)
{
    switch (size) {
    case PCI_SIZE_8:

        return (value >> ((offset & 3) * 8)) & 0xff;
    case PCI_SIZE_16:

        return (value >> ((offset & 2) * 8)) & 0xffff;
    default:

        return value;
    }
}

static uint32_t pci_conv_size_to_32(uint32_t old, uint32_t value, uint32_t offset, uint8_t size)
{
    uint32_t off_mask;
    uint32_t val_mask, shift;
    uint32_t ldata, mask;

    switch (size) {
    case PCI_SIZE_8:
        off_mask = 3;
        val_mask = 0xff;
        break;
    case PCI_SIZE_16:
        off_mask = 2;
        val_mask = 0xffff;
        break;
    default:

        return value;
    }
    shift = (offset & off_mask) * 8;
    ldata = (value & val_mask) << shift;
    mask = val_mask << shift;
    value = (old & ~mask) | ldata;

    return value;
}

static int32_t HAL_PCI_WriteConfig(uint32_t cfgBase, int32_t offset, uint32_t value, uint8_t size)
{
    uint32_t va, old;

    va = offset & ~0x3;
    old = readl(cfgBase + va);
    value = pci_conv_size_to_32(old, value, offset, size);
    writel(cfgBase + va, value);

    return 0;
}

static int32_t HAL_PCI_ReadConfig(uint32_t cfgBase, int32_t offset, uint32_t *valuep, uint8_t size)
{
    uint32_t va, value;

    HAL_ASSERT(valuep);

    va = offset & ~0x3;
    value = readl(cfgBase + va);
    *valuep = pci_conv_32_to_size(value, offset, size);

    return 0;
}
/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup PCI_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Write pci config space in pci dword
 * @param  cfgBase: PCIe config space cpu address.
 * @param  offset: offset in byte
 * @param  value: value
 * @return 0 for ok.
 */
int32_t HAL_PCI_WriteConfigDWord(uint32_t cfgBase, int32_t offset, uint32_t value)
{
    return HAL_PCI_WriteConfig(cfgBase, offset, value, PCI_SIZE_32);
}

/**
 * @brief  Read pci config space in pci dword
 * @param  cfgBase: PCIe config space cpu address.
 * @param  offset: offset in byte
 * @param  valuep: value point
 * @return 0 for ok.
 */
int32_t HAL_PCI_ReadConfigDWord(uint32_t cfgBase, int32_t offset, uint32_t *valuep)
{
    return HAL_PCI_ReadConfig(cfgBase, offset, valuep, PCI_SIZE_32);
}

/**
 * @brief  Write pci config space in pci word
 * @param  cfgBase: PCIe config space cpu address.
 * @param  offset: offset in byte
 * @param  value: value
 * @return 0 for ok.
 */
int32_t HAL_PCI_WriteConfigWord(uint32_t cfgBase, int32_t offset, uint32_t value)
{
    return HAL_PCI_WriteConfig(cfgBase, offset, value, PCI_SIZE_16);
}

/**
 * @brief  Read pci config space in pci word
 * @param  cfgBase: PCIe config space cpu address.
 * @param  offset: offset in byte
 * @param  valuep: value point
 * @return 0 for ok.
 */
int32_t HAL_PCI_ReadConfigWord(uint32_t cfgBase, int32_t offset, uint32_t *valuep)
{
    return HAL_PCI_ReadConfig(cfgBase, offset, valuep, PCI_SIZE_16);
}

/**
 * @brief  Write pci config space in byte
 * @param  cfgBase: PCIe config space cpu address.
 * @param  offset: offset in byte
 * @param  value: value
 * @return 0 for ok.
 */
int32_t HAL_PCI_WriteConfigByte(uint32_t cfgBase, int32_t offset, uint32_t value)
{
    return HAL_PCI_WriteConfig(cfgBase, offset, value, PCI_SIZE_8);
}

/**
 * @brief  Read pci config space in byte
 * @param  cfgBase: PCIe config space cpu address.
 * @param  offset: offset in byte
 * @param  valuep: value point
 * @return 0 for ok.
 */
int32_t HAL_PCI_ReadConfigByte(uint32_t cfgBase, int32_t offset, uint32_t *valuep)
{
    return HAL_PCI_ReadConfig(cfgBase, offset, valuep, PCI_SIZE_8);
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_PCIE_MODULE_ENABLED */
