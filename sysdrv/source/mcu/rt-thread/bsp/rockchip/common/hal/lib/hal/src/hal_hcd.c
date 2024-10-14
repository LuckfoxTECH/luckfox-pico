/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_HCD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup HCD
 *  @brief USB HCD HAL module driver
 *  @{
 */

/** @defgroup HCD_How_To_Use How To Use
 *  @{

 The HCD HAL driver can be used as follows:

 - Declare a struct HCD_HANDLE handle structure, for example:
     struct HCD_HANDLE pHCD;

 - Fill parameters of Init structure in HCD handle

 - Initialize the HCD low level resources:
     - Enable the HCD/USB Low Level interface clocks;
     - Configure HCD NVIC interrupt;

 - Call HAL_HCD_Init() API to initialize the HCD peripheral (Core, Host core, ...)

 - Associate the Upper USB Host stack to the HAL HCD Driver:
     - pHCD.pData = phost;

 - Enable HCD transmission and reception:
     - HAL_HCD_Start();

 @} */

/** @defgroup HCD_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/
/********************* Private Variable Definition ***************************/
/********************* Private Function Definition ***************************/
static void HCD_HC_IN_IRQHandler(struct HCD_HANDLE *pHCD, uint8_t chNum);
static void HCD_HC_OUT_IRQHandler(struct HCD_HANDLE *pHCD, uint8_t chNum);
static void HCD_RXQLVL_IRQHandler(struct HCD_HANDLE *pHCD);
static void HCD_Port_IRQHandler(struct HCD_HANDLE *pHCD);

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup HCD_Exported_Functions_Group2 State and Errors Functions
 *  @brief    HCD State functions
 *

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/**
 * @brief  Return the HCD handle state.
 * @param  pHCD HCD handle
 * @return HAL state
 */
eHCD_state HAL_HCD_GetState(struct HCD_HANDLE *pHCD)
{
    return pHCD->hcdState;
}

/**
 * @brief  Return URB state for a channel.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @return URB state.
 *         This parameter can be one of these values:
 *            URB_IDLE/
 *            URB_DONE/
 *            URB_NOTREADY/
 *            URB_NYET/
 *            URB_ERROR/
 *            URB_STALL
 */
eUSB_OTG_urbState HAL_HCD_HCGetURBState(struct HCD_HANDLE *pHCD, uint8_t chNum)
{
    return pHCD->hc[chNum].urbState;
}

/**
 * @brief  Return the last host transfer size.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @return last transfer size in byte
 */
uint32_t HAL_HCD_HCGetXferCount(struct HCD_HANDLE *pHCD, uint8_t chNum)
{
    return pHCD->hc[chNum].xferCount;
}

/**
 * @brief  Return the Host Channel state.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @return Host channel state
 *         This parameter can be one of these values:
 *            HC_IDLE/
 *            HC_XFRC/
 *            HC_HALTED/
 *            HC_NYET/
 *            HC_NAK/
 *            HC_STALL/
 *            HC_XACTERR/
 *            HC_BBLERR/
 *            HC_DATATGLERR
 */
eUSB_OTG_hcState HAL_HCD_HCGetState(struct HCD_HANDLE *pHCD, uint8_t chNum)
{
    return pHCD->hc[chNum].hcState;
}

/**
 * @brief  Return the current Host frame number.
 * @param  pHCD HCD handle
 * @return Current Host frame number
 */
uint32_t HAL_HCD_GetCurrentFrame(struct HCD_HANDLE *pHCD)
{
    return USB_GetCurrentFrame(pHCD->pReg);
}

/**
 * @brief  Return the Host enumeration speed.
 * @param  pHCD HCD handle
 * @return Enumeration speed
 */
uint32_t HAL_HCD_GetCurrentSpeed(struct HCD_HANDLE *pHCD)
{
    return USB_GetHostSpeed(pHCD->pReg);
}

/** @} */

/** @defgroup HCD_Exported_Functions_Group3 IO Functions
 *  @brief    HCD IO operation functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  Submit a new URB for processing.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @param  direction Channel number.
 *         This parameter can be one of these values:
 *           0 : Output / 1 : Input
 * @param  epType Endpoint Type.
 *         This parameter can be one of these values:
 *            EP_TYPE_CTRL: Control type/
 *            EP_TYPE_ISOC: Isochronous type/
 *            EP_TYPE_BULK: Bulk type/
 *            EP_TYPE_INTR: Interrupt type/
 * @param  token Endpoint Type.
 *         This parameter can be one of these values:
 *            0: HC_PID_SETUP / 1: HC_PID_DATA1
 * @param  pbuff pointer to URB data
 * @param  length Length of URB data
 * @param  doPing activate do ping protocol (for high speed only).
 *         This parameter can be one of these values:
 *           0 : do ping inactive / 1 : do ping active
 * @return HAL status
 */
HAL_Status HAL_HCD_HCSubmitRequest(struct HCD_HANDLE *pHCD,
                                   uint8_t chNum,
                                   uint8_t direction,
                                   uint8_t epType,
                                   uint8_t token,
                                   uint8_t *pbuff,
                                   uint16_t length,
                                   uint8_t doPing)
{
    pHCD->hc[chNum].epIsIn = direction;
    pHCD->hc[chNum].epType = epType;

    if (token == 0) {
        pHCD->hc[chNum].dataPID = HC_PID_SETUP;
    } else {
        pHCD->hc[chNum].dataPID = HC_PID_DATA1;
    }

    /* Manage Data Toggle */
    switch (epType) {
    case EP_TYPE_CTRL:
        if ((token == 1) && (direction == 0)) { /*send data */
            if (length == 0) {
                /* For Status OUT stage, Length==0, Status Out PID = 1 */
                pHCD->hc[chNum].toggleOut = 1;
            }

            /* Set the Data Toggle bit as per the Flag */
            if (pHCD->hc[chNum].toggleOut == 0) {  /* Put the PID 0 */
                pHCD->hc[chNum].dataPID = HC_PID_DATA0;
            } else {  /* Put the PID 1 */
                pHCD->hc[chNum].dataPID = HC_PID_DATA1;
            }
        }
        break;

    case EP_TYPE_BULK:
        if (direction == 0) {
            /* Set the Data Toggle bit as per the Flag */
            if (pHCD->hc[chNum].toggleOut == 0) {
                /* Put the PID 0 */
                pHCD->hc[chNum].dataPID = HC_PID_DATA0;
            } else {
                /* Put the PID 1 */
                pHCD->hc[chNum].dataPID = HC_PID_DATA1;
            }
        } else {
            if (pHCD->hc[chNum].toggleIn == 0) {
                pHCD->hc[chNum].dataPID = HC_PID_DATA0;
            } else {
                pHCD->hc[chNum].dataPID = HC_PID_DATA1;
            }
        }

        break;
    case EP_TYPE_INTR:
        if (direction == 0) {
            /* Set the Data Toggle bit as per the Flag */
            if (pHCD->hc[chNum].toggleOut == 0) {
                /* Put the PID 0 */
                pHCD->hc[chNum].dataPID = HC_PID_DATA0;
            } else {
                /* Put the PID 1 */
                pHCD->hc[chNum].dataPID = HC_PID_DATA1;
            }
        } else {
            if (pHCD->hc[chNum].toggleIn == 0) {
                pHCD->hc[chNum].dataPID = HC_PID_DATA0;
            } else {
                pHCD->hc[chNum].dataPID = HC_PID_DATA1;
            }
        }
        break;

    case EP_TYPE_ISOC:
        pHCD->hc[chNum].dataPID = HC_PID_DATA0;
        break;

    default:
        break;
    }

    pHCD->hc[chNum].pxferBuff = pbuff;
    pHCD->hc[chNum].xferLen = length;
    pHCD->hc[chNum].urbState = URB_IDLE;
    pHCD->hc[chNum].xferCount = 0;
    pHCD->hc[chNum].chNum = chNum;
    pHCD->hc[chNum].hcState = HC_IDLE;

    if (pHCD->cfg.dmaEnable == 1) {
        pHCD->hc[chNum].dmaAddr = HAL_CpuAddrToDmaAddr((uint32_t)pbuff);
    }

    return USB_HCStartXfer(pHCD->pReg, &(pHCD->hc[chNum]), (uint8_t)pHCD->cfg.dmaEnable);
}

/**
 * @brief  Handle HCD interrupt request.
 * @param  pHCD HCD handle
 */
void HAL_HCD_IRQHandler(struct HCD_HANDLE *pHCD)
{
    struct USB_GLOBAL_REG *pUSB = pHCD->pReg;
    uint32_t i = 0, interrupt = 0;

    /* ensure that we are in device mode */
    if (USB_GetMode(pHCD->pReg) == USB_OTG_MODE_HOST) {
        /* avoid spurious interrupt */
        if (__HAL_HCD_IS_INVALID_INTERRUPT(pHCD)) {
            return;
        }

        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT)) {
            /* incorrect mode, acknowledge the interrupt */
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
        }

        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_IISOIXFR)) {
            /* incorrect mode, acknowledge the interrupt */
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_IISOIXFR);
        }

        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_PTXFE)) {
            /* incorrect mode, acknowledge the interrupt */
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_PTXFE);
        }

        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_MMIS)) {
            /* incorrect mode, acknowledge the interrupt */
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_MMIS);
        }

        /* Handle Host Disconnect Interrupts */
        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_DISCINT)) {
            /* Cleanup HPRT */
            USB_HPRT0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |
                           USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

            /* Handle Host Port Interrupts */
            HAL_HCD_Disconnect_Callback(pHCD);
            USB_InitFSLSPClkSel(pHCD->pReg, HCFG_48_MHZ);
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_DISCINT);
        }

        /* Handle Host Port Interrupts */
        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_HPRTINT)) {
            HCD_Port_IRQHandler(pHCD);
        }

        /* Handle Host SOF Interrupts */
        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_SOF)) {
            HAL_HCD_SOF_Callback(pHCD);
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_SOF);
        }

        /* Handle Host channel Interrupts */
        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_HCINT)) {
            interrupt = USB_HCReadInterrupt(pHCD->pReg);
            for (i = 0; i < pHCD->cfg.hcNum; i++) {
                if (interrupt & (1UL << (i & 0xFU))) {
                    if ((USB_HC(i)->HCCHAR) & USB_OTG_HCCHAR_EPDIR) {
                        HCD_HC_IN_IRQHandler(pHCD, (uint8_t)i);
                    } else {
                        HCD_HC_OUT_IRQHandler(pHCD, (uint8_t)i);
                    }
                }
            }
            __HAL_HCD_CLEAR_FLAG(pHCD, USB_OTG_GINTSTS_HCINT);
        }

        /* Handle Rx Queue Level Interrupts */
        if (__HAL_HCD_GET_FLAG(pHCD, USB_OTG_GINTSTS_RXFLVL)) {
            USB_MASK_INTERRUPT(pHCD->pReg, USB_OTG_GINTSTS_RXFLVL);

            HCD_RXQLVL_IRQHandler(pHCD);

            USB_UNMASK_INTERRUPT(pHCD->pReg, USB_OTG_GINTSTS_RXFLVL);
        }
    }
}

/**
 * @brief  SOF callback.
 * @param  pHCD HCD handle
 */
__WEAK void HAL_HCD_SOF_Callback(struct HCD_HANDLE *pHCD)
{
    /* NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_HCD_SOF_Callback could be implemented in the user file.
     */
}

/**
 * @brief Connection Event callback.
 * @param  pHCD HCD handle
 */
__WEAK void HAL_HCD_Connect_Callback(struct HCD_HANDLE *pHCD)
{
    /* NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_HCD_Connect_Callback could be implemented in the user file.
     */
}

/**
 * @brief  Disconnection Event callback.
 * @param  pHCD HCD handle
 */
__WEAK void HAL_HCD_Disconnect_Callback(struct HCD_HANDLE *pHCD)
{
    /* NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_HCD_Disconnect_Callback could be implemented in the user file.
     */
}

/**
 * @brief  Port Enabled  Event callback.
 * @param  pHCD: HCD handle
 */
__WEAK void HAL_HCD_PortEnabled_Callback(struct HCD_HANDLE *pHCD)
{
    /* NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_HCD_Disconnect_Callback could be implemented in the user file.
     */
}

/**
 * @brief  Port Disabled  Event callback.
 * @param  pHCD: HCD handle
 */
__WEAK void HAL_HCD_PortDisabled_Callback(struct HCD_HANDLE *pHCD)
{
    /* NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_HCD_Disconnect_Callback could be implemented in the user file.
     */
}

/**
 * @brief  Notify URB state change callback.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @param  urbState:
 *          This parameter can be one of these values:
 *            URB_IDLE/
 *            URB_DONE/
 *            URB_NOTREADY/
 *            URB_NYET/
 *            URB_ERROR/
 *            URB_STALL/
 */
__WEAK void HAL_HCD_HCNotifyURBChange_Callback(struct HCD_HANDLE *pHCD, uint8_t chNum, eUSB_OTG_urbState urbState)
{
    /* NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_HCD_HCNotifyURBChange_Callback could be implemented in the user file.
     */
}

/** @} */

/** @defgroup HCD_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:
 *  @{
 */

/**
 * @brief  Initialize the host driver.
 * @param  pHCD HCD handle
 * @return HAL status
 */
HAL_Status HAL_HCD_Init(struct HCD_HANDLE *pHCD)
{
    /* Check the HCD handle allocation */
    HAL_ASSERT(pHCD != NULL);

    /* Check the parameters */
    HAL_ASSERT(IS_HCD_INSTANCE(pHCD->pReg));

    pHCD->hcdState = HAL_HCD_STATE_BUSY;

    /* Disable the Interrupts */
    __HAL_HCD_DISABLE(pHCD);

    /*Init the Core (common init.) */
    USB_CoreInit(pHCD->pReg, pHCD->cfg);

    /* Force Host Mode*/
    USB_SetCurrentMode(pHCD->pReg, USB_OTG_HOST_MODE);

    /* Init Host */
    USB_HostInit(pHCD->pReg, pHCD->cfg);

    pHCD->hcdState = HAL_HCD_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  Initialize a host channel.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @param  epNum Endpoint number.
 *         This parameter can be a value from 1 to 15
 * @param  devAddress  Current device address
 *         This parameter can be a value from 0 to 255
 * @param  speed Current device speed.
 *         This parameter can be one of these values:
 *            HCD_SPEED_HIGH: High speed mode,
 *            HCD_SPEED_FULL: Full speed mode,
 *            HCD_SPEED_LOW: Low speed mode
 * @param  epType Endpoint Type.
 *         This parameter can be one of these values:
 *            EP_TYPE_CTRL: Control type,
 *            EP_TYPE_ISOC: Isochronous type,
 *            EP_TYPE_BULK: Bulk type,
 *            EP_TYPE_INTR: Interrupt type
 * @param  mps Max Packet Size.
 *         This parameter can be a value from 0 to32K
 * @return HAL status
 */
HAL_Status HAL_HCD_HCInit(struct HCD_HANDLE *pHCD,
                          uint8_t chNum,
                          uint8_t epNum,
                          uint8_t devAddress,
                          uint8_t speed,
                          uint8_t epType,
                          uint16_t mps)
{
    HAL_Status status = HAL_OK;

    pHCD->hc[chNum].doPing = 0;
    pHCD->hc[chNum].devAddr = devAddress;
    pHCD->hc[chNum].maxPacket = mps;
    pHCD->hc[chNum].chNum = chNum;
    pHCD->hc[chNum].epType = epType;
    pHCD->hc[chNum].epNum = epNum & 0x7FU;
    pHCD->hc[chNum].epIsIn = ((epNum & 0x80U) == 0x80U);
    pHCD->hc[chNum].speed = speed;

    status = USB_HCInit(pHCD->pReg,
                        chNum,
                        epNum,
                        devAddress,
                        speed,
                        epType,
                        mps);

    return status;
}

/**
 * @brief  Halt a host channel.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @return HAL status
 */
HAL_Status HAL_HCD_HCHalt(struct HCD_HANDLE *pHCD, uint8_t chNum)
{
    HAL_Status status = HAL_OK;

    USB_HCHalt(pHCD->pReg, chNum);

    return status;
}

/**
 * @brief  DeInitialize the host driver.
 * @param  pHCD HCD handle
 * @return HAL status
 */
HAL_Status HAL_HCD_DeInit(struct HCD_HANDLE *pHCD)
{
    /* Check the HCD handle allocation */
    HAL_ASSERT(pHCD != NULL);

    pHCD->hcdState = HAL_HCD_STATE_BUSY;

    __HAL_HCD_DISABLE(pHCD);

    pHCD->hcdState = HAL_HCD_STATE_RESET;

    return HAL_OK;
}

/** @} */

/** @defgroup HCD_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Start the host driver.
 * @param  pHCD HCD handle
 * @return HAL status
 */
HAL_Status HAL_HCD_Start(struct HCD_HANDLE *pHCD)
{
    __HAL_HCD_ENABLE(pHCD);
    USB_DriveVbus(pHCD->pReg, 1);

    return HAL_OK;
}

/**
 * @brief  Stop the host driver.
 * @param  pHCD HCD handle
 * @return HAL status
 */
HAL_Status HAL_HCD_Stop(struct HCD_HANDLE *pHCD)
{
    USB_StopHost(pHCD->pReg);

    return HAL_OK;
}

/**
 * @brief  Reset the host port.
 * @param  pHCD HCD handle
 * @return HAL status
 */
HAL_Status HAL_HCD_ResetPort(struct HCD_HANDLE *pHCD)
{
    return USB_ResetPort(pHCD->pReg);
}

/** @} */

/**
 * @brief  Handle Host Channel IN interrupt requests.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @return none
 */
static void HCD_HC_IN_IRQHandler(struct HCD_HANDLE *pHCD, uint8_t chNum)
{
    struct USB_GLOBAL_REG *pUSB = pHCD->pReg;
    uint32_t tmpReg = 0;

    if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_AHBERR) {
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_AHBERR);
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_ACK) {
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_ACK);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_STALL) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        pHCD->hc[chNum].hcState = HC_STALL;
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NAK);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_STALL);
        USB_HCHalt(pHCD->pReg, chNum);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_DTERR) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NAK);
        pHCD->hc[chNum].hcState = HC_DATATGLERR;
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_DTERR);
    }

    if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_FRMOR) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_FRMOR);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_XFRC) {
        if (pHCD->cfg.dmaEnable) {
            pHCD->hc[chNum].xferCount = pHCD->hc[chNum].xferLen -
                                        (USB_HC(chNum)->HCTSIZ &
                                         USB_OTG_HCTSIZ_XFRSIZ);
            HAL_DCACHE_InvalidateByRange((uint32_t)pHCD->hc[chNum].pxferBuff,
                                         pHCD->hc[chNum].xferCount);
        }

        pHCD->hc[chNum].hcState = HC_XFRC;
        pHCD->hc[chNum].errCount = 0;
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_XFRC);

        if ((pHCD->hc[chNum].epType == EP_TYPE_CTRL) ||
            (pHCD->hc[chNum].epType == EP_TYPE_BULK)) {
            __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
            USB_HCHalt(pHCD->pReg, chNum);
            __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NAK);
        } else if (pHCD->hc[chNum].epType == EP_TYPE_INTR) {
            USB_HC(chNum)->HCCHAR |= USB_OTG_HCCHAR_ODDFRM;
            pHCD->hc[chNum].urbState = URB_DONE;
            HAL_HCD_HCNotifyURBChange_Callback(pHCD, chNum,
                                               pHCD->hc[chNum].urbState);
        }
        pHCD->hc[chNum].toggleIn ^= 1;
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_CHH) {
        __HAL_HCD_MASK_HALT_HC_INT(chNum);

        if (pHCD->hc[chNum].hcState == HC_XFRC) {
            pHCD->hc[chNum].urbState = URB_DONE;
        } else if (pHCD->hc[chNum].hcState == HC_STALL) {
            pHCD->hc[chNum].urbState = URB_STALL;
        } else if ((pHCD->hc[chNum].hcState == HC_XACTERR) ||
                   (pHCD->hc[chNum].hcState == HC_DATATGLERR)) {
            pHCD->hc[chNum].errCount++;
            if (pHCD->hc[chNum].errCount > 3) {
                pHCD->hc[chNum].errCount = 0;
                pHCD->hc[chNum].urbState = URB_ERROR;
            } else {
                pHCD->hc[chNum].urbState = URB_NOTREADY;
            }

            /* re-activate the channel  */
            tmpReg = USB_HC(chNum)->HCCHAR;
            tmpReg &= ~USB_OTG_HCCHAR_CHDIS;
            tmpReg |= USB_OTG_HCCHAR_CHENA;
            USB_HC(chNum)->HCCHAR = tmpReg;
        } else if (pHCD->hc[chNum].hcState == HC_NAK) {
            pHCD->hc[chNum].urbState = URB_NOTREADY;
            /* re-activate the channel  */
            tmpReg = USB_HC(chNum)->HCCHAR;
            tmpReg &= ~USB_OTG_HCCHAR_CHDIS;
            tmpReg |= USB_OTG_HCCHAR_CHENA;
            USB_HC(chNum)->HCCHAR = tmpReg;
        }
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_CHH);
        HAL_HCD_HCNotifyURBChange_Callback(pHCD, chNum,
                                           pHCD->hc[chNum].urbState);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_TXERR) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        pHCD->hc[chNum].errCount++;
        pHCD->hc[chNum].hcState = HC_XACTERR;
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_TXERR);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_NAK) {
        if (pHCD->hc[chNum].epType == EP_TYPE_INTR) {
            pHCD->hc[chNum].errCount = 0;
            __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
            USB_HCHalt(pHCD->pReg, chNum);
        } else if ((pHCD->hc[chNum].epType == EP_TYPE_CTRL) ||
                   (pHCD->hc[chNum].epType == EP_TYPE_BULK)) {
            pHCD->hc[chNum].errCount = 0;
            if (!pHCD->cfg.dmaEnable) {
                pHCD->hc[chNum].hcState = HC_NAK;
                __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
                USB_HCHalt(pHCD->pReg, chNum);
            }
        }

        /* Clear the NAK flag before re-enabling the channel for new IN request */
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NAK);
    }
}

/**
 * @brief  Handle Host Channel OUT interrupt requests.
 * @param  pHCD HCD handle
 * @param  chNum Channel number.
 *         This parameter can be a value from 1 to 15
 * @return none
 */
static void HCD_HC_OUT_IRQHandler(struct HCD_HANDLE *pHCD, uint8_t chNum)
{
    struct USB_GLOBAL_REG *pUSB = pHCD->pReg;
    uint32_t tmpReg = 0;

    if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_AHBERR) {
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_AHBERR);
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_ACK) {
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_ACK);
        if (pHCD->hc[chNum].doPing == 1 && !pHCD->cfg.dmaEnable) {
            pHCD->hc[chNum].doPing = 0;
            pHCD->hc[chNum].urbState = URB_NOTREADY;
            __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
            USB_HCHalt(pHCD->pReg, chNum);
        }
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_NYET) {
        pHCD->hc[chNum].hcState = HC_NYET;
        pHCD->hc[chNum].doPing = 1;
        pHCD->hc[chNum].errCount = 0;
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NYET);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_FRMOR) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_FRMOR);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_XFRC) {
        pHCD->hc[chNum].errCount = 0;
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_XFRC);
        pHCD->hc[chNum].hcState = HC_XFRC;
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_STALL) {
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_STALL);
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        pHCD->hc[chNum].hcState = HC_STALL;
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_NAK) {
        pHCD->hc[chNum].errCount = 0;
        pHCD->hc[chNum].hcState = HC_NAK;

        if (pHCD->hc[chNum].doPing == 0) {
            if (pHCD->hc[chNum].speed == HCD_SPEED_HIGH) {
                pHCD->hc[chNum].doPing = 1;
            }
        }
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NAK);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_TXERR) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        pHCD->hc[chNum].hcState = HC_XACTERR;
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_TXERR);
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_DTERR) {
        __HAL_HCD_UNMASK_HALT_HC_INT(chNum);
        USB_HCHalt(pHCD->pReg, chNum);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_NAK);
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_DTERR);
        pHCD->hc[chNum].hcState = HC_DATATGLERR;
    } else if ((USB_HC(chNum)->HCINT) & USB_OTG_HCINT_CHH) {
        __HAL_HCD_MASK_HALT_HC_INT(chNum);

        if (pHCD->hc[chNum].hcState == HC_XFRC) {
            pHCD->hc[chNum].urbState = URB_DONE;
            if ((pHCD->hc[chNum].epType == EP_TYPE_BULK) ||
                (pHCD->hc[chNum].epType == EP_TYPE_INTR)) {
                pHCD->hc[chNum].toggleOut ^= 1;
            }
        } else if (pHCD->hc[chNum].hcState == HC_NAK) {
            pHCD->hc[chNum].urbState = URB_NOTREADY;
        } else if (pHCD->hc[chNum].hcState == HC_NYET) {
            pHCD->hc[chNum].urbState = URB_NOTREADY;
        } else if (pHCD->hc[chNum].hcState == HC_STALL) {
            pHCD->hc[chNum].urbState = URB_STALL;
        } else if ((pHCD->hc[chNum].hcState == HC_XACTERR) ||
                   (pHCD->hc[chNum].hcState == HC_DATATGLERR)) {
            pHCD->hc[chNum].errCount++;
            if (pHCD->hc[chNum].errCount > 3) {
                pHCD->hc[chNum].errCount = 0;
                pHCD->hc[chNum].urbState = URB_ERROR;
            } else {
                pHCD->hc[chNum].urbState = URB_NOTREADY;
            }

            /* re-activate the channel  */
            tmpReg = USB_HC(chNum)->HCCHAR;
            tmpReg &= ~USB_OTG_HCCHAR_CHDIS;
            tmpReg |= USB_OTG_HCCHAR_CHENA;
            USB_HC(chNum)->HCCHAR = tmpReg;
        }
        __HAL_HCD_CLEAR_HC_INT(chNum, USB_OTG_HCINT_CHH);
        HAL_HCD_HCNotifyURBChange_Callback(pHCD, chNum,
                                           pHCD->hc[chNum].urbState);
    }
}

/**
 * @brief  Handle Rx Queue Level interrupt requests.
 * @param  pHCD HCD handle
 * @return none
 */
static void HCD_RXQLVL_IRQHandler(struct HCD_HANDLE *pHCD)
{
    struct USB_GLOBAL_REG *pUSB = pHCD->pReg;
    uint8_t chNum = 0;
    uint32_t pktSts;
    uint32_t pktCnt;
    uint32_t temp = 0;
    uint32_t tmpReg = 0;

    temp = pHCD->pReg->GRXSTSP;
    chNum = temp & USB_OTG_GRXSTSP_EPNUM;
    pktSts = (temp & USB_OTG_GRXSTSP_PKTSTS) >> 17;
    pktCnt = (temp & USB_OTG_GRXSTSP_BCNT) >> 4;

    switch (pktSts) {
    case GRXSTS_PKTSTS_IN:
        /* Read the data into the host buffer. */
        if ((pktCnt > 0) && (pHCD->hc[chNum].pxferBuff != (void *)0)) {
            USB_ReadPacket(pHCD->pReg, pHCD->hc[chNum].pxferBuff, chNum,
                           pktCnt, pHCD->cfg.dmaEnable);

            /*manage multiple Xfer */
            pHCD->hc[chNum].pxferBuff += pktCnt;
            pHCD->hc[chNum].xferCount += pktCnt;

            if ((USB_HC(chNum)->HCTSIZ & USB_OTG_HCTSIZ_PKTCNT) > 0) {
                /* re-activate the channel when more packets are expected */
                tmpReg = USB_HC(chNum)->HCCHAR;
                tmpReg &= ~USB_OTG_HCCHAR_CHDIS;
                tmpReg |= USB_OTG_HCCHAR_CHENA;
                USB_HC(chNum)->HCCHAR = tmpReg;
                pHCD->hc[chNum].toggleIn ^= 1;
            }
        }
        break;

    case GRXSTS_PKTSTS_DATA_TOGGLE_ERR:
        break;
    case GRXSTS_PKTSTS_IN_XFER_COMP:
    case GRXSTS_PKTSTS_CH_HALTED:
    default:
        break;
    }
}

/**
 * @brief  Handle Host Port interrupt requests.
 * @param  pHCD HCD handle
 */
static void HCD_Port_IRQHandler(struct HCD_HANDLE *pHCD)
{
    struct USB_GLOBAL_REG *pUSB = pHCD->pReg;
    uint32_t hprt0, hprt0Modify;

    /* Handle Host Port Interrupts */
    hprt0 = USB_HPRT0;
    hprt0Modify = USB_HPRT0;

    hprt0Modify &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |
                     USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

    /* Check whether Port Connect detected */
    if ((hprt0 & USB_OTG_HPRT_PCDET) == USB_OTG_HPRT_PCDET) {
        if ((hprt0 & USB_OTG_HPRT_PCSTS) == USB_OTG_HPRT_PCSTS) {
            USB_MASK_INTERRUPT(pHCD->pReg,
                               USB_OTG_GINTSTS_DISCINT);
            HAL_HCD_Connect_Callback(pHCD);
        }
        hprt0Modify |= USB_OTG_HPRT_PCDET;
    }

    /* Check whether Port Enable Changed */
    if ((hprt0 & USB_OTG_HPRT_PENCHNG) == USB_OTG_HPRT_PENCHNG) {
        hprt0Modify |= USB_OTG_HPRT_PENCHNG;

        if ((hprt0 & USB_OTG_HPRT_PENA) == USB_OTG_HPRT_PENA) {
            HAL_HCD_PortEnabled_Callback(pHCD);
            HAL_HCD_Connect_Callback(pHCD);
        } else {
            HAL_HCD_PortDisabled_Callback(pHCD);
            /* Cleanup HPRT */
            USB_HPRT0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |
                           USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

            USB_UNMASK_INTERRUPT(pHCD->pReg, USB_OTG_GINTSTS_DISCINT);
        }
    }

    /* Check For an overcurrent */
    if ((hprt0 & USB_OTG_HPRT_POCCHNG) == USB_OTG_HPRT_POCCHNG) {
        hprt0Modify |= USB_OTG_HPRT_POCCHNG;
    }

    /* Clear Port Interrupts */
    USB_HPRT0 = hprt0Modify;
}

/** @} */

/** @} */

#endif /* HAL_HCD_MODULE_ENABLED */
