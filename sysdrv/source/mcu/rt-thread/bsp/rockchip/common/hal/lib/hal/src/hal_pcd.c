/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PCD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCD
 *  @brief USB PCD HAL module driver
 *  @{
 */

/** @defgroup PCD_How_To_Use How To Use
 *  @{

 The PCD HAL driver can be used as follows:

 - Declare a struct PCD_HANDLE handle structure, for example:
   ```
   struct PCD_HANDLE pPCD;
   ```
 - Fill parameters of Init structure in HCD handle

 - Initialize the PCD low level resources:
     - Enable the PCD/USB Low Level interface clocks;
     - Configure PCD NVIC interrupt;

 - Call HAL_PCD_Init() API to initialize the PCD peripheral (Core, Device core, ...)

 - Associate the Upper USB device stack to the HAL PCD Driver:
   ```
   pPCD.pData = pdev;
   ```
 - Enable PCD transmission and reception:
   ```
   HAL_PCD_Start();
   ```

 @} */

/** @defgroup PCD_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
static HAL_Status PCD_WriteEmptyTxFifo(struct PCD_HANDLE *pPCD, uint32_t epNum);

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup PCD_Exported_Functions_Group2 State and Errors Functions
 *  @brief    Peripheral State functions
 *

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/**
 * @brief  Return the PCD handle state.
 * @param  pPCD PCD handle
 * @return HAL state
 */
ePCD_state HAL_PCD_GetState(struct PCD_HANDLE *pPCD)
{
    return pPCD->pcdState;
}
/** @} */

/** @defgroup PCD_Exported_Functions_Group3 IO Functions
 *  @brief    Data transfers functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  Start The USB OTG Device.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_Start(struct PCD_HANDLE *pPCD)
{
    USB_DevConnect(pPCD->pReg);
    __HAL_PCD_ENABLE(pPCD);

    return HAL_OK;
}

/**
 * @brief  Stop The USB OTG Device.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_Stop(struct PCD_HANDLE *pPCD)
{
    __HAL_PCD_DISABLE(pPCD);
    USB_StopDevice(pPCD->pReg);
    USB_DevDisconnect(pPCD->pReg);

    return HAL_OK;
}

/**
 * @brief  Handle PCD interrupt request.
 * @param  pPCD PCD handle
 */
void HAL_PCD_IRQHandler(struct PCD_HANDLE *pPCD)
{
    struct USB_GLOBAL_REG *pUSB = pPCD->pReg;
    uint32_t i = 0, epIntr = 0, epInt = 0, epNum = 0;
    uint32_t fifoEmptyMsk = 0, temp = 0;
    uint32_t pktCnt = 0;
    struct USB_OTG_EP *pEP = NULL;

    /* ensure that we are in device mode */
    if (USB_GetMode(pPCD->pReg) == USB_OTG_MODE_DEVICE) {
        /* avoid spurious interrupt */
        if (__HAL_PCD_IS_INVALID_INTERRUPT(pPCD)) {
            return;
        }

        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_MMIS)) {
            /* incorrect mode, acknowledge the interrupt */
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_MMIS);
        }

        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_OEPINT)) {
            epNum = 0;

            /* Read in the device interrupt bits */
            epIntr = USB_ReadDevAllOutEpInterrupt(pPCD->pReg);

            while (epIntr) {
                if (epIntr & 0x1) {
                    epInt = USB_ReadDevOutEPInterrupt(pPCD->pReg, epNum);
                    /* Clear OUT endpoint interrupts */
                    CLEAR_OUT_EP_INTR(epNum, epInt);

                    /* Don't process XferCompl interrupt if it is a setup packet */
                    if ((epNum == 0) && (epInt & (USB_OTG_DOEPINT_STUP |
                                                  USB_OTG_DOEPINT_STUPPKTRCVD))) {
                        epInt &= ~USB_OTG_DOEPINT_XFRC;
                    }

                    if ((epInt & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC) {
                        if (pPCD->cfg.dmaEnable == 1) {
                            if (pPCD->outEp[epNum].xferLen > 0) {
                                pktCnt = HAL_DIV_ROUND_UP(pPCD->outEp[epNum].xferLen,
                                                          pPCD->outEp[epNum].maxPacket);
                                pPCD->outEp[epNum].xferCount = (USB_OTG_DOEPTSIZ_XFRSIZ & (pPCD->outEp[epNum].maxPacket * pktCnt)) - (USB_OUTEP(epNum)->DOEPTSIZ & USB_OTG_DOEPTSIZ_XFRSIZ);
                                HAL_DCACHE_InvalidateByRange((uint32_t)pPCD->outEp[epNum].dmaAddr,
                                                             pPCD->outEp[epNum].xferCount);
                            } else {
                                pPCD->outEp[epNum].xferCount = pPCD->outEp[epNum].maxPacket -
                                                               (USB_OUTEP(epNum)->DOEPTSIZ & USB_OTG_DOEPTSIZ_XFRSIZ);
                            }

                            pPCD->outEp[epNum].pxferBuff += pPCD->outEp[epNum].xferCount;

                            if ((epNum == 0) && (pPCD->outEp[epNum].xferLen == 0)) {
                                /* this is ZLP, so prepare EP0 for next setup */
                                USB_EP0_OutStart(pPCD->pReg, 1, pPCD->setupBuf);
                            } else {
                                pPCD->outEp[epNum].xferLen = 0;
                                HAL_PCD_DataOutStageCallback(pPCD, epNum);
                            }
                        }
                    }

                    if ((epInt & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP) {
                        if (pPCD->cfg.dmaEnable == 1) {
                            HAL_DCACHE_InvalidateByRange((uint32_t)(pPCD->setupBuf), sizeof(pPCD->setupBuf));
                            /* Inform the upper layer that a setup packet is available */
                            HAL_PCD_SetupStageCallback(pPCD);
                        }
                    }

                    /*
                     * This is starting point for high speed ISOC-OUT transfer,
                     * synchronization done with first out token received from
                     * host while corresponding EP is disabled. Device does not
                     * know initial frame in which out token will come. For this
                     * HW generates OUTTKNEPDIS - out token is received while EP
                     * is disabled. Upon getting this interrupt SW starts isoc
                     * out transfer.
                     */
                    if (((epInt & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS) &&
                        (pPCD->outEp[epNum].type == EP_TYPE_ISOC) &&
                        (pPCD->outEp[epNum].isocStart == 0) &&
                        (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH)) {
                        if ((USB_DEVICE->DSTS & (1 << 8)) == 0) {
                            USB_OUTEP(epNum)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
                        } else {
                            USB_OUTEP(epNum)->DOEPCTL |= USB_OTG_DOEPCTL_SODDFRM;
                        }

                        if (pPCD->outEp[epNum].isocPending == 1) {
                            USB_OUTEP(epNum)->DOEPCTL |= (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
                            pPCD->outEp[epNum].isocPending = 0;
                        }
                        pPCD->outEp[epNum].isocStart = 1;
                        USB_DEVICE->DOEPMSK &= ~(USB_OTG_DOEPMSK_OTEPDM);
                    }
                }
                epNum++;
                epIntr >>= 1;
            }
        }

        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_IEPINT)) {
            /* Read in the device interrupt bits */
            epIntr = USB_ReadDevAllInEpInterrupt(pPCD->pReg);

            epNum = 0;

            while (epIntr) {
                if (epIntr & 0x1) { /* In ITR */
                    epInt = USB_ReadDevInEPInterrupt(pPCD->pReg, epNum);
                    /* Clear IN endpoint interrupts */
                    CLEAR_IN_EP_INTR(epNum, epInt);

                    if ((epInt & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC) {
                        fifoEmptyMsk = 0x1 << epNum;
                        USB_DEVICE->DIEPEMPMSK &= ~fifoEmptyMsk;

                        if (pPCD->cfg.dmaEnable == 1) {
                            pPCD->inEp[epNum].pxferBuff += pPCD->inEp[epNum].maxPacket;
                        }

                        /* For both DMA mode and Slave mode */
                        if ((epNum == 0) && (pPCD->inEp[epNum].xferLen == 0) &&
                            ((pPCD->setupBuf[0] & 0x80) == 0)) {
                            /* this is ZLP, so prepare EP0 for next setup */
                            USB_EP0_OutStart(pPCD->pReg, 1, pPCD->setupBuf);
                        } else {
                            pPCD->inEp[epNum].xferLen = 0;
                            HAL_PCD_DataInStageCallback(pPCD, epNum);
                        }
                    }

                    if ((epInt & USB_OTG_DIEPINT_TXFE) == USB_OTG_DIEPINT_TXFE) {
                        PCD_WriteEmptyTxFifo(pPCD, epNum);
                    }

                    /*
                     * This is starting point for high speed ISOC-IN transfer,
                     * synchronization done with first IN token received from
                     * host while corresponding EP is disabled. Device does not
                     * know when first one token will arrive from host. NAK
                     * interrupt for ISOC-IN means that token has arrived and
                     * ZLP was sent in response to that as there was no data in
                     * FIFO. SW is basing on this interrupt to obtain frame in
                     * which token has come and start isoc in transfer.
                     */
                    if (((epInt & USB_OTG_DIEPINT_NAK) == USB_OTG_DIEPINT_NAK) &&
                        (pPCD->inEp[epNum].type == EP_TYPE_ISOC) &&
                        (pPCD->inEp[epNum].isocStart == 0) &&
                        (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH)) {
                        if ((USB_DEVICE->DSTS & (1 << 8)) == 0) {
                            USB_INEP(epNum)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
                        } else {
                            USB_INEP(epNum)->DIEPCTL |= USB_OTG_DIEPCTL_SODDFRM;
                        }

                        if (pPCD->inEp[epNum].isocPending == 1) {
                            USB_INEP(epNum)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);
                            pPCD->inEp[epNum].isocPending = 0;
                        }
                        pPCD->inEp[epNum].isocStart = 1;
                    }
                }
                epNum++;
                epIntr >>= 1;
            }
        }

        /* Handle Resume Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_WKUINT)) {
            /* Clear the Remote Wake-up Signaling */
            USB_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

            if (pPCD->lpmState == LPM_L1) {
                pPCD->lpmState = LPM_L0;
                HAL_PCDEx_LpmCallback(pPCD, PCD_LPM_L0_ACTIVE);
            } else {
                HAL_PCD_ResumeCallback(pPCD);
            }
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_WKUINT);
        }

        /* Handle Suspend Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_USBSUSP)) {
            if (!pPCD->cfg.suspendEnable &&
                ((USB_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS) &&
                ((USB_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) != USB_OTG_DSTS_ENUMSPD_0)) {
                /*
                 * If the Bvalid signal is always high, the usb core
                 * will enter suspend state when disconnet from Host.
                 */
                HAL_DBG("USB Disconnect! DSTS: 0x%08lx\n", USB_DEVICE->DSTS);
                HAL_PCD_DisconnectCallback(pPCD);
                USB_StopDevice(pUSB);
            } else {
                HAL_PCD_SuspendCallback(pPCD);
            }

            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_USBSUSP);
        }

        /* Handle LPM Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_LPMINT)) {
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_LPMINT);
            if (pPCD->lpmState == LPM_L0) {
                pPCD->lpmState = LPM_L1;
                HAL_PCDEx_LpmCallback(pPCD, PCD_LPM_L1_ACTIVE);
            } else {
                HAL_PCD_SuspendCallback(pPCD);
            }
        }

        /* Handle Reset Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_USBRST)) {
            USB_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
            USB_FlushTxFifo(pPCD->pReg, 0x10);

            for (i = 0; i < pPCD->cfg.epNum; i++) {
                USB_INEP(i)->DIEPINT = 0xFF;
                USB_INEP(i)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
                USB_OUTEP(i)->DOEPINT = 0xFF;
                USB_OUTEP(i)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
            }

            USB_DEVICE->DAINT = 0xFFFFFFFF;
            USB_DEVICE->DAINTMSK |= 0x10001;

            USB_DEVICE->DOEPMSK |= (USB_OTG_DOEPMSK_STUPM |
                                    USB_OTG_DOEPMSK_XFRCM |
                                    USB_OTG_DOEPMSK_EPDM |
                                    USB_OTG_DOEPMSK_OTEPSPRM);
            USB_DEVICE->DIEPMSK |= (USB_OTG_DIEPMSK_TOM |
                                    USB_OTG_DIEPMSK_XFRCM |
                                    USB_OTG_DIEPMSK_EPDM);

            /* Set Default Address to 0 */
            USB_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD;

            HAL_PCD_ResetCallback(pPCD);

            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_USBRST);
        }

        /* Handle Enumeration done Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_ENUMDNE)) {
            USB_ActivateSetup(pPCD->pReg);

            pPCD->enumSpeed = USB_GetDevSpeed(pPCD->pReg);

            /* setup EP0 to receive SETUP packets */
            USB_EP0_OutStart(pPCD->pReg, pPCD->cfg.dmaEnable, pPCD->setupBuf);

            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_ENUMDNE);
        }

        /* Handle RxQLevel Interrupt for slave mode */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_RXFLVL)) {
            USB_MASK_INTERRUPT(pPCD->pReg, USB_OTG_GINTSTS_RXFLVL);
            temp = pUSB->GRXSTSP;
            pEP = &pPCD->outEp[temp & USB_OTG_GRXSTSP_EPNUM];
            epNum = temp & USB_OTG_GRXSTSP_EPNUM;

            if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) == STS_DATA_UPDT) {
                if ((temp & USB_OTG_GRXSTSP_BCNT) != 0) {
                    USB_ReadPacket(pUSB, pEP->pxferBuff, epNum,
                                   (temp & USB_OTG_GRXSTSP_BCNT) >> 4,
                                   pPCD->cfg.dmaEnable);
                    pEP->pxferBuff += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                    pEP->xferCount += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                }
            } else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) == STS_XFER_COMP) {
                if (pEP->xferCount != 0) {
                    HAL_PCD_DataOutStageCallback(pPCD, epNum);
                } else if ((epNum == 0) && (pEP->xferCount == 0)) {
                    USB_EP0_OutStart(pPCD->pReg, 1, pPCD->setupBuf);
                }
            } else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) == STS_SETUP_UPDT) {
                USB_ReadPacket(pUSB, pPCD->setupBuf, epNum, 8, pPCD->cfg.dmaEnable);
                pEP->xferCount += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
            } else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) == STS_SETUP_COMP) {
                /* Inform the upper layer that a setup packet is available */
                HAL_PCD_SetupStageCallback(pPCD);
            } else {
                HAL_DBG_WRN("unknown status 0x%08lx\n", temp);
            }
            USB_UNMASK_INTERRUPT(pPCD->pReg, USB_OTG_GINTSTS_RXFLVL);
        }

        /* Handle SOF Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_SOF)) {
            HAL_PCD_SOFCallback(pPCD);
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_SOF);
        }

        /* Handle Incomplete ISO IN Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_IISOIXFR)) {
            HAL_PCD_ISOINIncompleteCallback(pPCD, epNum);
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_IISOIXFR);
        }

        /* Handle Incomplete ISO OUT Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT)) {
            HAL_PCD_ISOOUTIncompleteCallback(pPCD, epNum);
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
        }

        /* Handle Connection event Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_SRQINT)) {
            HAL_PCD_ConnectCallback(pPCD);
            __HAL_PCD_CLEAR_FLAG(pPCD, USB_OTG_GINTSTS_SRQINT);
        }

        /* Handle Disconnection event Interrupt */
        if (__HAL_PCD_GET_FLAG(pPCD, USB_OTG_GINTSTS_OTGINT)) {
            temp = pPCD->pReg->GOTGINT;

            if ((temp & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET) {
                HAL_DBG("USB Disconnect!\n");
                HAL_PCD_DisconnectCallback(pPCD);
                USB_StopDevice(pUSB);
            }

            pPCD->pReg->GOTGINT |= temp;
        }
    }
}

/**
 * @brief  Data OUT stage callback.
 * @param  pPCD PCD handle
 * @param  epNum endpoint number
 */
__WEAK void HAL_PCD_DataOutStageCallback(struct PCD_HANDLE *pPCD, uint8_t epNum)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_DataOutStageCallback could be implemented in the user file.
     */
}

/**
 * @brief  Data IN stage callback.
 * @param  pPCD PCD handle
 * @param  epNum endpoint number
 */
__WEAK void HAL_PCD_DataInStageCallback(struct PCD_HANDLE *pPCD, uint8_t epNum)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_DataInStageCallback could be implemented in the user file.
     */
}
/**
 * @brief  Setup stage callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_SetupStageCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_SetupStageCallback could be implemented in the user file.
     */
}

/**
 * @brief  USB Start Of Frame callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_SOFCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_SOFCallback could be implemented in the user file.
     */
}

/**
 * @brief  USB Reset callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_ResetCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_ResetCallback could be implemented in the user file.
     */
}

/**
 * @brief  Suspend event callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_SuspendCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_SuspendCallback could be implemented in the user file.
     */
}

/**
 * @brief  Resume event callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_ResumeCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_ResumeCallback could be implemented in the user file.
     */
}

/**
 * @brief  Incomplete ISO OUT callback.
 * @param  pPCD PCD handle
 * @param  epNum endpoint number
 */
__WEAK void HAL_PCD_ISOOUTIncompleteCallback(struct PCD_HANDLE *pPCD, uint8_t epNum)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_ISOOUTIncompleteCallback could be implemented in the user file.
     */
}

/**
 * @brief  Incomplete ISO IN  callback.
 * @param  pPCD PCD handle
 * @param  epNum endpoint number
 */
__WEAK void HAL_PCD_ISOINIncompleteCallback(struct PCD_HANDLE *pPCD, uint8_t epNum)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_ISOINIncompleteCallback could be implemented in the user file.
     */
}

/**
 * @brief  Connection event callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_ConnectCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_ConnectCallback could be implemented in the user file.
     */
}

/**
 * @brief  Disconnection event callback.
 * @param  pPCD PCD handle
 */
__WEAK void HAL_PCD_DisconnectCallback(struct PCD_HANDLE *pPCD)
{
    /*
     * NOTE:
     * This function Should not be modified, when the callback is needed,
     * the HAL_PCD_DisconnectCallback could be implemented in the user file.
     */
}

/** @} */

/** @defgroup PCD_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:
 *  @{
 */

/**
 * @brief  Initializes the PCD according to the specified parameters
 *         in the struct USB_OTG_CFG and create the associated handle.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_Init(struct PCD_HANDLE *pPCD)
{
    uint32_t i = 0;

    /* Check the PCD handle allocation */
    HAL_ASSERT(pPCD != NULL);

    /* Check the parameters */
    HAL_ASSERT(IS_PCD_INSTANCE(pPCD->pReg));

    pPCD->pcdState = HAL_PCD_STATE_BUSY;

    /* Disable the Interrupts */
    __HAL_PCD_DISABLE(pPCD);

    /*Init the Core (common init.) */
    USB_CoreInit(pPCD->pReg, pPCD->cfg);

    /* Force Device Mode*/
    USB_SetCurrentMode(pPCD->pReg, USB_OTG_DEVICE_MODE);

    /* Init endpoints structures */
    for (i = 0; i < 15; i++) {
        /* Init ep structure */
        pPCD->inEp[i].isIn = 1;
        pPCD->inEp[i].num = i;
        pPCD->inEp[i].txFIFONum = i;
        /* Control until ep is activated */
        pPCD->inEp[i].type = EP_TYPE_CTRL;
        pPCD->inEp[i].maxPacket = 0;
        pPCD->inEp[i].pxferBuff = 0;
        pPCD->inEp[i].xferLen = 0;
        pPCD->pReg->DIEPTXF[i] = 0;
    }

    for (i = 0; i < 15; i++) {
        pPCD->outEp[i].isIn = 0;
        pPCD->outEp[i].num = i;
        /* Control until ep is activated */
        pPCD->outEp[i].type = EP_TYPE_CTRL;
        pPCD->outEp[i].maxPacket = 0;
        pPCD->outEp[i].pxferBuff = 0;
        pPCD->outEp[i].xferLen = 0;
    }

    /* Init Device */
    USB_DevInit(pPCD->pReg, pPCD->cfg);

    pPCD->pcdState = HAL_PCD_STATE_READY;

    /* Activate LPM */
    if (pPCD->cfg.lpmEnable == 1) {
        HAL_PCDEx_ActivateLPM(pPCD);
    }

    USB_DevDisconnect(pPCD->pReg);

    return HAL_OK;
}

/**
 * @brief  DeInitializes the PCD peripheral.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_DeInit(struct PCD_HANDLE *pPCD)
{
    /* Check the PCD handle allocation */
    HAL_ASSERT(pPCD != NULL);

    pPCD->pcdState = HAL_PCD_STATE_BUSY;

    /* Stop Device */
    HAL_PCD_Stop(pPCD);

    pPCD->pcdState = HAL_PCD_STATE_RESET;

    return HAL_OK;
}

/** @} */

/** @defgroup PCD_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Connect the USB device.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_DevConnect(struct PCD_HANDLE *pPCD)
{
    USB_DevConnect(pPCD->pReg);

    return HAL_OK;
}

/**
 * @brief  Disconnect the USB device.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_DevDisconnect(struct PCD_HANDLE *pPCD)
{
    USB_DevDisconnect(pPCD->pReg);

    return HAL_OK;
}

/**
 * @brief  Set the USB Device address.
 * @param  pPCD PCD handle
 * @param  address new device address
 * @return HAL status
 */
HAL_Status HAL_PCD_SetAddress(struct PCD_HANDLE *pPCD, uint8_t address)
{
    USB_SetDevAddress(pPCD->pReg, address);

    return HAL_OK;
}

/**
 * @brief  Open and configure an endpoint.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @param  ep_mps endpoint max packet size
 * @param  epType endpoint type
 * @return HAL status
 */
HAL_Status HAL_PCD_EPOpen(struct PCD_HANDLE *pPCD, uint8_t epAddr, uint16_t ep_mps, uint8_t epType)
{
    HAL_Status ret = HAL_OK;
    struct USB_OTG_EP *pEP;

    if ((epAddr & 0x80) == 0x80) {
        pEP = &pPCD->inEp[epAddr & 0x7F];
    } else {
        pEP = &pPCD->outEp[epAddr & 0x7F];
    }

    pEP->num = epAddr & 0x7F;
    pEP->isIn = (0x80 & epAddr) != 0;
    pEP->maxPacket = ep_mps;
    pEP->type = epType & EP_TYPE_MSK;

    if (pEP->isIn) {
        /* Assign TxFIFO Number for IN endpoints */
        pEP->txFIFONum = pEP->num ? (pEP->num >> 1) + 1 : 0;
    }

    /* Set initial data PID. */
    if (pEP->type == EP_TYPE_BULK) {
        pEP->dataPID = 0;
    }

    USB_ActivateEndpoint(pPCD->pReg, pEP);

    return ret;
}

/**
 * @brief  Deactivate an endpoint.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @return HAL status
 */
HAL_Status HAL_PCD_EPClose(struct PCD_HANDLE *pPCD, uint8_t epAddr)
{
    struct USB_OTG_EP *pEP;

    if ((epAddr & 0x80) == 0x80) {
        pEP = &pPCD->inEp[epAddr & 0x7F];
    } else {
        pEP = &pPCD->outEp[epAddr & 0x7F];
    }

    pEP->num = epAddr & 0x7F;
    pEP->isIn = (0x80 & epAddr) != 0;

    USB_DeactivateEndpoint(pPCD->pReg, pEP);

    return HAL_OK;
}

/**
 * @brief  Receive an amount of data.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @param  pBuf pointer to the reception buffer
 * @param  len amount of data to be received
 * @return HAL status
 */
HAL_Status HAL_PCD_EPReceive(struct PCD_HANDLE *pPCD, uint8_t epAddr, uint8_t *pBuf, uint32_t len)
{
    struct USB_OTG_EP *pEP;

    pEP = &pPCD->outEp[epAddr & 0x7F];

    /* setup and start the Xfer */
    pEP->pxferBuff = pBuf;
    pEP->xferLen = len;
    pEP->xferCount = 0;
    pEP->isIn = 0;
    pEP->num = epAddr & 0x7F;

    if (pPCD->cfg.dmaEnable == 1) {
        pEP->dmaAddr = HAL_CpuAddrToDmaAddr((uint32_t)pBuf);
    }

    if ((epAddr & 0x7F) == 0) {
        USB_EP0StartXfer(pPCD->pReg, pEP, pPCD->cfg.dmaEnable);
    } else {
        USB_EPStartXfer(pPCD->pReg, pEP, pPCD->cfg.dmaEnable);
    }

    return HAL_OK;
}

/**
 * @brief  Get Received Data Size.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @return Data Size
 */
uint16_t HAL_PCD_EPGetRxCount(struct PCD_HANDLE *pPCD, uint8_t epAddr)
{
    return pPCD->outEp[epAddr & 0xF].xferCount;
}

/**
 * @brief  Send an amount of data.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @param  pBuf pointer to the transmission buffer
 * @param  len amount of data to be sent
 * @return HAL status
 */
HAL_Status HAL_PCD_EPTransmit(struct PCD_HANDLE *pPCD, uint8_t epAddr,
                              uint8_t *pBuf, uint32_t len)
{
    struct USB_OTG_EP *pEP;

    pEP = &pPCD->inEp[epAddr & 0x7F];

    /* setup and start the Xfer */
    pEP->pxferBuff = pBuf;
    pEP->xferLen = len;
    pEP->xferCount = 0;
    pEP->isIn = 1;
    pEP->num = epAddr & 0x7F;

    if (pPCD->cfg.dmaEnable == 1) {
        pEP->dmaAddr = HAL_CpuAddrToDmaAddr((uint32_t)pBuf);
    }

    if ((epAddr & 0x7F) == 0) {
        USB_EP0StartXfer(pPCD->pReg, pEP, pPCD->cfg.dmaEnable);
    } else {
        USB_EPStartXfer(pPCD->pReg, pEP, pPCD->cfg.dmaEnable);
    }

    return HAL_OK;
}

/**
 * @brief  Set a STALL condition over an endpoint.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @return HAL status
 */
HAL_Status HAL_PCD_EPSetStall(struct PCD_HANDLE *pPCD, uint8_t epAddr)
{
    struct USB_OTG_EP *pEP;

    if ((epAddr & 0x0F) > pPCD->cfg.epNum) {
        return HAL_ERROR;
    }

    if ((0x80 & epAddr) == 0x80) {
        pEP = &pPCD->inEp[epAddr & 0x7F];
    } else {
        pEP = &pPCD->outEp[epAddr];
    }

    pEP->isStall = 1;
    pEP->num = epAddr & 0x7F;

    if (pEP->num != 0) {
        pEP->isIn = ((epAddr & 0x80) == 0x80);
    } else if (pEP->num == 0) {
        if (pPCD->setupBuf[0] & 0x80) {
            pEP->isIn = 1;
        } else {
            pEP->isIn = 0;
        }
    }

    USB_EPSetStall(pPCD->pReg, pEP);
    if ((epAddr & 0x7F) == 0) {
        USB_EP0_OutStart(pPCD->pReg, pPCD->cfg.dmaEnable, pPCD->setupBuf);
    }

    return HAL_OK;
}

/**
 * @brief  Clear a STALL condition over in an endpoint.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @return HAL status
 */
HAL_Status HAL_PCD_EPClrStall(struct PCD_HANDLE *pPCD, uint8_t epAddr)
{
    struct USB_OTG_EP *pEP;

    if ((epAddr & 0x0F) > pPCD->cfg.epNum) {
        return HAL_ERROR;
    }

    if ((0x80 & epAddr) == 0x80) {
        pEP = &pPCD->inEp[epAddr & 0x7F];
    } else {
        pEP = &pPCD->outEp[epAddr];
    }

    pEP->isStall = 0;
    pEP->num = epAddr & 0x7F;
    pEP->isIn = ((epAddr & 0x80) == 0x80);

    USB_EPClearStall(pPCD->pReg, pEP);

    return HAL_OK;
}

/**
 * @brief  Flush an endpoint.
 * @param  pPCD PCD handle
 * @param  epAddr endpoint address
 * @return HAL status
 */
HAL_Status HAL_PCD_EPFlush(struct PCD_HANDLE *pPCD, uint8_t epAddr)
{
    if ((epAddr & 0x80) == 0x80) {
        USB_FlushTxFifo(pPCD->pReg, epAddr & 0x7F);
    } else {
        USB_FlushRxFifo(pPCD->pReg);
    }

    return HAL_OK;
}

/**
 * @brief  Activate remote wakeup signalling.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_ActivateRemoteWakeup(struct PCD_HANDLE *pPCD)
{
    struct USB_GLOBAL_REG *pUSB = pPCD->pReg;

    if ((USB_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS) {
        /* Activate Remote wakeup signaling */
        USB_DEVICE->DCTL |= USB_OTG_DCTL_RWUSIG;
    }

    return HAL_OK;
}

/**
 * @brief  De-activate remote wakeup signalling.
 * @param  pPCD PCD handle
 * @return HAL status
 */
HAL_Status HAL_PCD_DeActivateRemoteWakeup(struct PCD_HANDLE *pPCD)
{
    struct USB_GLOBAL_REG *pUSB = pPCD->pReg;

    /* De-activate Remote wakeup signaling */
    USB_DEVICE->DCTL &= ~(USB_OTG_DCTL_RWUSIG);

    return HAL_OK;
}
/** @} */

/**
 * @brief  Check FIFO for the next packet to be loaded.
 * @param  pPCD PCD handle
 * @param  epNum  endpoint number
 * @return HAL status
 */
static HAL_Status PCD_WriteEmptyTxFifo(struct PCD_HANDLE *pPCD, uint32_t epNum)
{
    struct USB_GLOBAL_REG *pUSB = pPCD->pReg;
    struct USB_OTG_EP *pEP;
    uint32_t len = 0;
    uint32_t lenWords;
    uint32_t fifoEmptyMsk = 0;

    pEP = &pPCD->inEp[epNum];
    len = pEP->xferLen - pEP->xferCount;

    if (len > pEP->maxPacket) {
        len = pEP->maxPacket;
    }

    lenWords = (len + 3) / 4;

    while ((USB_INEP(epNum)->DTXFSTS & USB_OTG_DTXFSTS_INEPTFSAV) > lenWords &&
           pEP->xferCount < pEP->xferLen &&
           pEP->xferLen != 0) {
        /* Write the FIFO */
        len = pEP->xferLen - pEP->xferCount;

        if (len > pEP->maxPacket) {
            len = pEP->maxPacket;
        }

        lenWords = (len + 3) / 4;

        USB_WritePacket(pUSB, pEP->pxferBuff, epNum, len, pPCD->cfg.dmaEnable);

        pEP->pxferBuff += len;
        pEP->xferCount += len;
    }

    if (len <= 0) {
        fifoEmptyMsk = 0x1 << epNum;
        USB_DEVICE->DIEPEMPMSK &= ~fifoEmptyMsk;
    }

    return HAL_OK;
}

/** @} */

/** @} */

#endif /* HAL_PCD_MODULE_ENABLED */
