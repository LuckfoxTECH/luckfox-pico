/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup USB_PHY
 *  @brief USB PHY HAL module driver
 *  @{
 */

/** @defgroup USB_PHY_How_To_Use How To Use
 *  @{

 The USB PHY HAL driver can be used as follows:

 @} */

/** @defgroup USB_PHY_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/
/********************* Private Variable Definition ***************************/
/********************* Private Function Definition ***************************/
/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup USB_PHY_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 *  @{
 */

#ifdef USB_PHY_CON_BASE
/**
 * @brief  Suspend the USB PHY.
 * @return HAL status
 */
HAL_Status HAL_USB_PhySuspend(void)
{
    WRITE_REG_MASK_WE(USB_PHY_CON_BASE, USB_PHY_SUSPEND_MASK,
                      USB_PHY_SUSPEND_VAL << USB_PHY_CON_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Resume the USB PHY.
 * @return HAL status
 */
HAL_Status HAL_USB_PhyResume(void)
{
    WRITE_REG_MASK_WE(USB_PHY_CON_BASE, USB_PHY_RESUME_MASK,
                      USB_PHY_RESUME_VAL << USB_PHY_CON_SHIFT);

    return HAL_OK;
}
#else
HAL_Status HAL_USB_PhySuspend(void)
{
    return HAL_OK;
}

HAL_Status HAL_USB_PhyResume(void)
{
    return HAL_OK;
}
#endif

/** @} */

/** @defgroup PCD_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:
 *  @{
 */

/**
 * @brief  Initialize the USB PHY.
 * @return HAL status
 * @attention these APIs allow direct use in the HAL layer.
 */
#if defined(USB_M31PHY_BASE)
HAL_Status HAL_USB_PhyInit(void)
{
    HAL_CRU_ClkResetDeassert(SRST_USB2PHYPO);
    /* Select USB controller UTMI interface to phy */
    WRITE_REG_MASK_WE(USB_PHY_CON_BASE, USB_PHY_RESUME_MASK,
                      USB_PHY_RESUME_VAL << USB_PHY_CON_SHIFT);
    /* Wait for UTMI clk stable */
    HAL_DelayMs(2);

    return HAL_OK;
}
#elif defined(USB_SNPS_PHY_BASE)
HAL_Status HAL_USB_PhyInit(void)
{
    /* GRF, 0x0348: bit[3:0] = 0x01 */
    WRITE_REG_MASK_WE(USB_PHY_CON_BASE,
                      USB_PHY_SUSPEND_MASK,
                      USB_PHY_SUSPEND_VAL << USB_PHY_CON_SHIFT);

    /* Wait for UTMI clk stable */
    HAL_CRU_ClkResetAssert(SRST_OTG_USBPHY);
    HAL_DelayUs(15);

    /* GRF, 0x0348: bit[3:0] = 0x02 */
    WRITE_REG_MASK_WE(USB_PHY_CON_BASE,
                      USB_PHY_RESUME_MASK,
                      USB_PHY_RESUME_VAL << USB_PHY_CON_SHIFT);
    HAL_DelayUs(1500);
    HAL_CRU_ClkResetDeassert(SRST_OTG_USBPHY);
    HAL_DelayUs(2);

    return HAL_OK;
}
#elif defined(USB_INNO_PHY_BASE)
HAL_Status HAL_USB_PhyInit(void)
{
    /* Reset USB PHY only for Swallow FPGA */
#if defined(SOC_SWALLOW) && defined(IS_FPGA)
    *(volatile uint32_t *)(CRU_BASE + 0x1004U) = 0x1;
    HAL_DelayUs(500);
    *(volatile uint32_t *)(CRU_BASE + 0x1004U) = 0x0;
    HAL_DelayUs(2000);
#endif

    return HAL_OK;
}
#else
HAL_Status HAL_USB_PhyInit(void)
{
    return HAL_OK;
}
#endif

/** @} */

/** @} */

/** @} */

#endif /* defined (HAL_PCD_MODULE_ENABLED) || defined (HAL_HCD_MODULE_ENABLED) */
