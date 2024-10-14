/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PCD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCDEx
 *  @brief USB PCD Extended HAL module driver
 *  @{
 */

/** @defgroup PCDEx_How_To_Use How To Use
 *  @{

 The PCD Extended HAL driver can be used as follows:

 @} */

/** @defgroup PCDEx_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/
/********************* Private Variable Definition ***************************/
/********************* Private Function Definition ***************************/
#ifdef USB_M31PHY_BASE
static ePCD_bcdMsg USB_M31phyBcdDetect(struct PCD_HANDLE *pPCD)
{
    uint32_t lineState;
    ePCD_bcdMsg msg;

    HAL_USB_PhyInit();

    /*
     * Put the PHY in normal operation of OpMode, and
     * set the PHY to enter the suspend mode to disable
     * connect to USB Host, and set the utmi termselect
     * to FS/LS termination, and set the utmi xcvrselect
     * to FS transceiver, disable DP/DM pulldown resistor.
     */
    WRITE_REG_MASK_WE(USB_PHY_CON_BASE, USB_PHY_SUSPEND_MASK,
                      0x051U << USB_PHY_CON_SHIFT);

    /* Enable usb connect to pull up DP or DM */
    USB_DevConnect(pPCD->pReg);

    /* Wait for 40ms */
    HAL_DelayMs(40);

    /* Voltage Source on DP or DM, Probe on DP and DM */
    lineState = READ_BIT(USB_PHY_STATUS_BASE, USB_PHY_LINESTATE_MASK) >>
                USB_PHY_LINESTATE_SHIFT;
    switch (lineState) {
    case 1:
        msg = PCD_BCD_STD_DOWNSTREAM_PORT;
        break;
    case 3:
        msg = PCD_BCD_DEDICATED_CHARGING_PORT;
        break;
    default:
        msg = PCD_BCD_STD_DOWNSTREAM_PORT;
        break;
    }

    return msg;
}
#endif
/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup PCDEx_Exported_Functions_Group5 Other Function
 *  @{
 */

/**
 * @brief  Set Tx FIFO
 * @param  pPCD PCD handle
 * @param  fifo The number of Tx fifo
 * @param  size Fifo size
 * @return HAL status
 */
HAL_Status HAL_PCDEx_SetTxFiFo(struct PCD_HANDLE *pPCD, uint8_t fifo, uint16_t size)
{
    uint8_t i = 0;
    uint32_t txOffset = 0;

    /* Set TxFIFO only if the Dynamic FIFO Sizing is enabled and dedicated FIFO mode */
    if (((pPCD->pReg->GHWCFG2 & USB_OTG_GHWCFG2_DYNFIFOSIZING_MASK) != USB_OTG_GHWCFG2_DYNFIFOSIZING) ||
        ((pPCD->pReg->GHWCFG4 & USB_OTG_GHWCFG4_DEDFIFOMODE_MASK) != USB_OTG_GHWCFG4_DEDFIFOMODE)) {
        return HAL_OK;
    }
    /*
     * TXn min size = 16 words. (n  : Transmit FIFO index)
     * When a TxFIFO is not used, the Configuration should be as follows:
     *     case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
     *     --> Txm can use the space allocated for Txn.
     *     case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
     *     --> Txn should be configured with the minimum space of 16 words
     * The FIFO is used optimally when used TxFIFOs are allocated in the top
     * of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
     * When DMA is used 3n * FIFO locations should be reserved for internal DMA registers
     */
    txOffset = pPCD->pReg->GRXFSIZ;

    if (fifo == 0) {
        pPCD->pReg->DIEPTXF0_HNPTXFSIZ = (uint32_t)(((uint32_t)size << 16) | txOffset);
    } else {
        txOffset += (pPCD->pReg->DIEPTXF0_HNPTXFSIZ) >> 16;
        fifo = fifo >> 1;
        for (i = 0; i < fifo; i++) {
            txOffset += (pPCD->pReg->DIEPTXF[i] >> 16);
        }

        /* Multiply Tx_Size by 2 to get higher performance */
        pPCD->pReg->DIEPTXF[fifo] = (uint32_t)(((uint32_t)size << 16) | txOffset);
    }

    return HAL_OK;
}

/**
 * @brief  Set Rx FIFO
 * @param  pPCD PCD handle
 * @param  size Size of Rx fifo
 * @return HAL status
 */
HAL_Status HAL_PCDEx_SetRxFiFo(struct PCD_HANDLE *pPCD, uint16_t size)
{
    /*  Set RxFIFO only if the dynamic FIFO Sizing is Enabled */
    if (pPCD->pReg->GHWCFG2 & USB_OTG_GHWCFG2_DYNFIFOSIZING) {
        pPCD->pReg->GRXFSIZ = size;
    }

    return HAL_OK;
}

/**
 * @brief  Activate LPM Feature
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCDEx_ActivateLPM(struct PCD_HANDLE *pPCD)
{
    struct USB_GLOBAL_REG *pUSB = pPCD->pReg;

    pPCD->lpmState = LPM_L0;
    pUSB->GINTMSK |= USB_OTG_GINTMSK_LPMINTM;
    pUSB->GLPMCFG |= (USB_OTG_GLPMCFG_LPMEN | USB_OTG_GLPMCFG_LPMACK | USB_OTG_GLPMCFG_ENBESL);

    return HAL_OK;
}

/**
 * @brief  DeActivate LPM feature.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCDEx_DeActivateLPM(struct PCD_HANDLE *pPCD)
{
    struct USB_GLOBAL_REG *pUSB = pPCD->pReg;

    pUSB->GINTMSK &= ~USB_OTG_GINTMSK_LPMINTM;
    pUSB->GLPMCFG &= ~(USB_OTG_GLPMCFG_LPMEN | USB_OTG_GLPMCFG_LPMACK | USB_OTG_GLPMCFG_ENBESL);

    return HAL_OK;
}

/**
 * @brief  Send LPM message to user layer callback.
 * @param  pPCD PCD handle
 * @param  msg LPM message
 */
__WEAK void HAL_PCDEx_LpmCallback(struct PCD_HANDLE *pPCD, ePCD_lpmMsg msg)
{
    /*
     * NOTE : This function Should not be modified, when the callback is needed,
     *        the HAL_PCDEx_LpmCallback could be implemented in the user file
     */
}

/**
 * @brief  Send BatteryCharging message to user layer callback.
 * @param  pPCD PCD handle
 * @param  msg bcd message
 */
__WEAK void HAL_PCDEx_BcdCallback(struct PCD_HANDLE *pPCD, ePCD_bcdMsg msg)
{
    /*
     * NOTE : This function Should not be modified, when the callback is needed,
     *        the HAL_PCDEx_BcdCallback could be implemented in the user file
     */
}

/**
 * @brief  Handle BatteryCharging Process.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCDEx_BcdDetect(struct PCD_HANDLE *pPCD)
{
    ePCD_bcdMsg msg = PCD_BCD_DEFAULT_STATE;

#if defined(USB_M31PHY_BASE)
    msg = USB_M31phyBcdDetect(pPCD);
#else
    /* TODO: the other PHYs (e.g. INNO, NaNeng...) */
#endif

    HAL_PCDEx_BcdCallback(pPCD, msg);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_PCD_MODULE_ENABLED */
