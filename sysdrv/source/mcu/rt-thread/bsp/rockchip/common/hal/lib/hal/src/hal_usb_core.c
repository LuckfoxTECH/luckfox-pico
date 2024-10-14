/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup USB_CORE
 *  @brief USB CORE HAL module driver
 *  @{
 */

/** @defgroup USB_CORE_How_To_Use How To Use
 *  @{

 The USB CORE HAL driver can be used as follows:

 - Fill parameters of Init structure in USB_OTG_CFG structure.

 - Call USB_CoreInit() API to initialize the USB Core peripheral.

 - The upper HAL HCD/PCD driver will call the right routines for its internal processes.

 @} */

/** @defgroup USB_CORE_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/
/********************* Private Variable Definition ***************************/
/********************* Private Function Definition ***************************/
static HAL_Status USB_WaitBitSet(volatile uint32_t *reg, uint32_t bit, uint32_t timeout)
{
    uint32_t i;

    for (i = 0; i <= timeout; i++) {
        if (READ_BIT(*reg, bit)) {
            return HAL_OK;
        }
        HAL_DelayUs(1);
    }

    return HAL_TIMEOUT;
}

static HAL_Status USB_CoreReset(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t count = 0;

    /* Core Soft Reset */
    pUSB->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;

    do {
        HAL_DelayUs(1);
        if (++count > 50) {
            HAL_DBG_ERR("%s HANG! Soft Reset GRSTCTL=0x%08lx", __func__, pUSB->GRSTCTL);

            return HAL_TIMEOUT;
        }
    } while (READ_BIT(pUSB->GRSTCTL, USB_OTG_GRSTCTL_CSRST));

    /* Wait for AHB master IDLE state. */
    count = 0;
    do {
        HAL_DelayUs(1);
        if (++count > 50) {
            HAL_DBG_ERR("%s HANG! AHB Idle GRSTCTL=0x%08lx", __func__, pUSB->GRSTCTL);

            return HAL_TIMEOUT;
        }
    } while (!READ_BIT(pUSB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));

    return HAL_OK;
}

static void USB_EPStopXfer(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP)
{
    if (pEP->isIn == 1) {
        USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
        /* Wait for Nak effect */
        if (USB_WaitBitSet(&USB_INEP(pEP->num)->DIEPINT, USB_OTG_DIEPINT_INEPNE, 100)) {
            HAL_DBG("%s timeout DIEPINT.NAKEFF\n", __func__);
        }

        /* Disable in ep */
        USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
        /* Wait for ep to be disabled */
        if (USB_WaitBitSet(&USB_INEP(pEP->num)->DIEPINT, USB_OTG_DIEPINT_EPDISD, 100)) {
            HAL_DBG("%s timeout DIEPCTL.EPDisable\n", __func__);
        }
        /* Clear EPDISBLD interrupt */
        USB_INEP(pEP->num)->DIEPINT |= USB_OTG_DIEPINT_EPDISD;
        /* Flush TX FIFO */
        USB_FlushTxFifo(pUSB, pEP->txFIFONum);
    } else {
        if (!(pUSB->GINTSTS & USB_OTG_GINTSTS_BOUTNAKEFF)) {
            USB_DEVICE->DCTL |= USB_OTG_DCTL_SGONAK;
            /* Wait for global nak to take effect  */
            if (USB_WaitBitSet(&pUSB->GINTSTS, USB_OTG_GINTSTS_BOUTNAKEFF, 100)) {
                HAL_DBG("%s timeout GINTSTS.GOUTNAKEF\n", __func__);
            }
        }

        /* Disable out ep */
        USB_OUTEP(pEP->num)->DOEPCTL |= (USB_OTG_DOEPCTL_EPDIS | USB_OTG_DOEPCTL_SNAK);
        /* Wait for ep to be disabled */
        if (USB_WaitBitSet(&USB_OUTEP(pEP->num)->DOEPINT, USB_OTG_DOEPINT_EPDISD, 100)) {
            HAL_DBG("%s: timeout DOEPCTL.EPDisable\n", __func__);
        }
        /* Clear EPDISBLD interrupt */
        USB_OUTEP(pEP->num)->DOEPINT |= USB_OTG_DOEPINT_EPDISD;
        /* Remove global NAKs */
        USB_DEVICE->DCTL |= USB_OTG_DCTL_CGONAK;
    }
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup USB_CORE_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:
 *  @{
 */

/**
 * @brief  Initializes the USB Core
 * @param  pUSB USB Instance
 * @param  cfg  pointer to a USB_OTG_CFG structure that contains
 *         the configuration information for the specified pUSB peripheral.
 * @return HAL status
 */
HAL_Status USB_CoreInit(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_CFG cfg)
{
    uint32_t trdtim = 0, phyif = 0, toutcal = 0;

    HAL_USB_PhyInit();

    /* Init The UTMI Interface */
    pUSB->GUSBCFG &= ~(USB_OTG_GUSBCFG_TSDPS |
                       USB_OTG_GUSBCFG_ULPIFSLS |
                       USB_OTG_GUSBCFG_PHYSEL);

    /* Select UTMI Interace */
    pUSB->GUSBCFG &= ~USB_OTG_GUSBCFG_ULPI_UTMI_SEL;

    /* Reset after a PHY select  */
    USB_CoreReset(pUSB);

    /* Set timeout calibration, phyif and turnaround time */
    pUSB->GUSBCFG &= ~(USB_OTG_GUSBCFG_SRPCAP |
                       USB_OTG_GUSBCFG_TRDT |
                       USB_OTG_GUSBCFG_PHYIF |
                       USB_OTG_GUSBCFG_TOCAL);

    trdtim = ((cfg.phyif & USB_PHY_UTMI_MASK) == 8) ? 9 : 5;
    phyif = ((cfg.phyif & USB_PHY_UTMI_MASK) == 8) ? 0 : 1;
    toutcal = 7;

    pUSB->GUSBCFG |= ((trdtim << USB_OTG_GUSBCFG_TRDT_SHIFT) |
                      (phyif << USB_OTG_GUSBCFG_PHYIF_SHIFT) |
                      (toutcal << USB_OTG_GUSBCFG_TOCAL_SHIFT));

    if (cfg.dmaEnable == HAL_ENABLE) {
        pUSB->GAHBCFG |= USB_OTG_GAHBCFG_HBSTLEN_4;
        pUSB->GAHBCFG |= USB_OTG_GAHBCFG_DMAEN;
    }

    return HAL_OK;
}

/**
 * @brief  USB_EnableGlobalInt
 *         Enables the controller's Global Int in the AHB Config reg
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_EnableGlobalInt(struct USB_GLOBAL_REG *pUSB)
{
    pUSB->GAHBCFG |= USB_OTG_GAHBCFG_GINT;

    return HAL_OK;
}

/**
 * @brief  USB_DisableGlobalInt
 *         Disable the controller's Global Int in the AHB Config reg
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_DisableGlobalInt(struct USB_GLOBAL_REG *pUSB)
{
    pUSB->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;

    return HAL_OK;
}

/**
 * @brief  USB_SetCurrentMode : Set functional mode
 * @param  pUSB  Selected device
 * @param  mode   current core mode
 *         This parameter can be one of these values:
 *         @arg USB_OTG_DEVICE_MODE: Peripheral mode
 *         @arg USB_OTG_HOST_MODE: Host mode
 *         @arg USB_OTG_DRD_MODE: Dual Role Device mode
 * @return HAL status
 */
HAL_Status USB_SetCurrentMode(struct USB_GLOBAL_REG *pUSB, eUSB_OTG_mode mode)
{
    uint32_t start, timeoutMs = 110;

    pUSB->GUSBCFG &= ~(USB_OTG_GUSBCFG_FHMOD | USB_OTG_GUSBCFG_FDMOD);

    if (mode == USB_OTG_HOST_MODE) {
        pUSB->GUSBCFG |= USB_OTG_GUSBCFG_FHMOD;
    } else if (mode == USB_OTG_DEVICE_MODE) {
        pUSB->GUSBCFG |= USB_OTG_GUSBCFG_FDMOD;
    } else {
        goto out;
    }

    /* Wait for the controller mode */
    start = HAL_GetTick();
    while (USB_GetMode(pUSB) != mode) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            HAL_DBG_ERR("%s: force mode timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

out:

    return HAL_OK;
}

/**
 * @brief  USB_DevInit : Initializes the USB_OTG controller registers
 *         for device mode
 * @param  pUSB  Selected device
 * @param  cfg   pointer to a USB_OTG_CFG structure that contains
 *         the configuration information for the specified pUSB peripheral.
 * @return HAL status
 */
HAL_Status USB_DevInit(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_CFG cfg)
{
    uint32_t i = 0;

    if (cfg.vbusSensingEnable == 0) {
        /* B-peripheral session valid override enable */
        pUSB->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
        pUSB->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;
    }

    /* Restart the Phy Clock */
    USB_PCGCCTL = 0;

    /* Device mode configuration */
    USB_DEVICE->DCFG |= DCFG_FRAME_INTERVAL_80;

    if (cfg.speed == USB_OTG_SPEED_HIGH) {
        /* Set High speed phy */
        USB_SetDevSpeed(pUSB, USB_OTG_SPEED_HIGH);
    } else {
        /* set High speed phy in Full speed mode */
        USB_SetDevSpeed(pUSB, USB_OTG_SPEED_HIGH_IN_FULL);
    }

    /* Flush the FIFOs */
    USB_FlushTxFifo(pUSB, 0x10); /* all Tx FIFOs */
    USB_FlushRxFifo(pUSB);

    /* Clear all pending Device Interrupts */
    USB_DEVICE->DIEPMSK = 0;
    USB_DEVICE->DOEPMSK = 0;
    USB_DEVICE->DAINT = 0xFFFFFFFF;
    USB_DEVICE->DAINTMSK = 0;

    for (i = 0; i < cfg.epNum; i++) {
        if ((USB_INEP(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) ==
            USB_OTG_DIEPCTL_EPENA) {
            USB_INEP(i)->DIEPCTL = (USB_OTG_DIEPCTL_EPDIS |
                                    USB_OTG_DIEPCTL_SNAK);
        } else {
            USB_INEP(i)->DIEPCTL = 0;
        }

        USB_INEP(i)->DIEPTSIZ = 0;
        USB_INEP(i)->DIEPINT = 0xFF;
    }

    for (i = 0; i < cfg.epNum; i++) {
        if ((USB_OUTEP(i)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) ==
            USB_OTG_DOEPCTL_EPENA) {
            USB_OUTEP(i)->DOEPCTL = (USB_OTG_DOEPCTL_EPDIS |
                                     USB_OTG_DOEPCTL_SNAK);
        } else {
            USB_OUTEP(i)->DOEPCTL = 0;
        }

        USB_OUTEP(i)->DOEPTSIZ = 0;
        USB_OUTEP(i)->DOEPINT = 0xFF;
    }

    USB_DEVICE->DIEPMSK &= ~(USB_OTG_DIEPMSK_TXFURM);

    /* Disable all interrupts. */
    pUSB->GINTMSK = 0;

    /* Clear any pending interrupts */
    pUSB->GINTSTS = 0xFFFFFFFF;

    /* Enable the common interrupts */
    if (cfg.dmaEnable == HAL_DISABLE) {
        pUSB->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;
    }

    /* Enable interrupts matching to the Device mode ONLY */
    pUSB->GINTMSK |= (USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_USBRST |
                      USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_IEPINT |
                      USB_OTG_GINTMSK_OEPINT | USB_OTG_GINTMSK_IISOIXFRM |
                      USB_OTG_GINTMSK_PXFRM_IISOOXFRM | USB_OTG_GINTMSK_WUIM);

    if (cfg.sofEnable) {
        pUSB->GINTMSK |= USB_OTG_GINTMSK_SOFM;
    }

    if (cfg.vbusSensingEnable == HAL_ENABLE) {
        pUSB->GINTMSK |= (USB_OTG_GINTMSK_SRQIM | USB_OTG_GINTMSK_OTGINT);
    }

    return HAL_OK;
}

/**
 * @brief  USB_OTG_FlushTxFifo : Flush a Tx FIFO
 * @param  pUSB  Selected device
 * @param  num  FIFO number
 *         This parameter can be a value from 1 to 15
 *         15 means Flush all Tx FIFOs
 * @return HAL status
 */
HAL_Status USB_FlushTxFifo(struct USB_GLOBAL_REG *pUSB, uint32_t num)
{
    uint32_t count = 0;

    pUSB->GRSTCTL = (USB_OTG_GRSTCTL_TXFFLSH | (uint32_t)(num << 6));

    do {
        if (++count > 200000) {
            return HAL_TIMEOUT;
        }
    } while ((pUSB->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

    return HAL_OK;
}

/**
 * @brief  USB_FlushRxFifo : Flush Rx FIFO
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_FlushRxFifo(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t count = 0;

    pUSB->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

    do {
        if (++count > 200000) {
            return HAL_TIMEOUT;
        }
    } while ((pUSB->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);

    return HAL_OK;
}

/**
 * @brief  USB_SetDevSpeed :Initializes the DevSpd field of DCFG register
 *         depending the PHY type and the enumeration speed of the device.
 * @param  pUSB  Selected device
 * @param  speed  device speed
 *          This parameter can be one of these values:
 *            @arg USB_OTG_SPEED_HIGH: High speed mode
 *            @arg USB_OTG_SPEED_HIGH_IN_FULL: High speed core in Full Speed mode
 *            @arg USB_OTG_SPEED_FULL: Full speed mode
 *            @arg USB_OTG_SPEED_LOW: Low speed mode
 * @return  Hal status
 */
HAL_Status USB_SetDevSpeed(struct USB_GLOBAL_REG *pUSB, uint8_t speed)
{
    USB_DEVICE->DCFG |= speed;

    return HAL_OK;
}

/**
 * @brief  USB_GetDevSpeed :Return the  Dev Speed
 * @param  pUSB  Selected device
 * @return speed : device speed
 *          This parameter can be one of these values:
 *            @arg USB_OTG_SPEED_HIGH: High speed mode
 *            @arg USB_OTG_SPEED_FULL: Full speed mode
 *            @arg USB_OTG_SPEED_LOW: Low speed mode
 */
uint8_t USB_GetDevSpeed(struct USB_GLOBAL_REG *pUSB)
{
    uint8_t speed = 0;

    if ((USB_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) ==
        DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ) {
        speed = USB_OTG_SPEED_HIGH;
    } else if (((USB_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) ==
                DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ) ||
               ((USB_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) ==
                DSTS_ENUMSPD_FS_PHY_48MHZ)) {
        speed = USB_OTG_SPEED_FULL;
    } else if ((USB_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) ==
               DSTS_ENUMSPD_LS_PHY_6MHZ) {
        speed = USB_OTG_SPEED_LOW;
    }

    return speed;
}

/**
 * @brief  Activate and configure an endpoint
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @return HAL status
 */
HAL_Status USB_ActivateEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP)
{
    if (pEP->isIn == 1) {
        USB_DEVICE->DAINTMSK |= USB_OTG_DAINTMSK_IEPM & ((1 << (pEP->num)));
        if ((pEP->type == EP_TYPE_ISOC) && (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH)) {
            USB_DEVICE->DIEPMSK |= USB_OTG_DIEPMSK_NAK;
        }

        if (((USB_INEP(pEP->num)->DIEPCTL) & USB_OTG_DIEPCTL_USBAEP) == 0) {
            USB_INEP(pEP->num)->DIEPCTL |= ((pEP->maxPacket & USB_OTG_DIEPCTL_MPSIZ) |
                                            (pEP->type << 18) | ((pEP->txFIFONum) << 22) |
                                            (USB_OTG_DIEPCTL_SD0PID_SEVNFRM) |
                                            (USB_OTG_DIEPCTL_USBAEP));
        }
    } else {
        USB_DEVICE->DAINTMSK |= USB_OTG_DAINTMSK_OEPM & ((1 << (pEP->num)) << 16);
        if ((pEP->type == EP_TYPE_ISOC) && (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH)) {
            USB_DEVICE->DOEPMSK |= USB_OTG_DOEPMSK_OTEPDM;
        }

        if (((USB_OUTEP(pEP->num)->DOEPCTL) & USB_OTG_DOEPCTL_USBAEP) == 0) {
            USB_OUTEP(pEP->num)->DOEPCTL |= ((pEP->maxPacket & USB_OTG_DOEPCTL_MPSIZ) |
                                             (pEP->type << 18) |
                                             (USB_OTG_DIEPCTL_SD0PID_SEVNFRM) |
                                             (USB_OTG_DOEPCTL_USBAEP));
        }
    }

    return HAL_OK;
}

/**
 * @brief  Activate and configure a dedicated endpoint
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @return HAL status
 */
HAL_Status USB_ActivateDedicatedEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP)
{
    /* Read DEPCTLn register */
    if (pEP->isIn == 1) {
        if (((USB_INEP(pEP->num)->DIEPCTL) & USB_OTG_DIEPCTL_USBAEP) == 0) {
            USB_INEP(pEP->num)->DIEPCTL |= ((pEP->maxPacket & USB_OTG_DIEPCTL_MPSIZ) |
                                            (pEP->type << 18) | ((pEP->num) << 22) |
                                            (USB_OTG_DIEPCTL_SD0PID_SEVNFRM) |
                                            (USB_OTG_DIEPCTL_USBAEP));
        }

        USB_DEVICE->DEACHMSK |= USB_OTG_DAINTMSK_IEPM & ((1 << (pEP->num)));
    } else {
        if (((USB_OUTEP(pEP->num)->DOEPCTL) & USB_OTG_DOEPCTL_USBAEP) == 0) {
            USB_OUTEP(pEP->num)->DOEPCTL |= ((pEP->maxPacket & USB_OTG_DOEPCTL_MPSIZ) |
                                             (pEP->type << 18) | ((pEP->num) << 22) |
                                             (USB_OTG_DOEPCTL_USBAEP));
        }

        USB_DEVICE->DEACHMSK |= USB_OTG_DAINTMSK_OEPM & ((1 << (pEP->num)) << 16);
    }

    return HAL_OK;
}

/**
 * @brief  De-activate and de-initialize an endpoint
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @return HAL status
 */
HAL_Status USB_DeactivateEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP)
{
    /* Read DEPCTLn register */
    if (pEP->isIn == 1) {
        if (USB_INEP(pEP->num)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) {
            USB_EPStopXfer(pUSB, pEP);
        }

        USB_INEP(pEP->num)->DIEPCTL &= ~USB_OTG_DIEPCTL_USBAEP;
        USB_INEP(pEP->num)->DIEPCTL &= ~USB_OTG_DIEPCTL_EPENA;
        USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
        USB_DEVICE->DAINTMSK &= ~(USB_OTG_DAINTMSK_IEPM & ((1 << (pEP->num))));
    } else {
        if (USB_OUTEP(pEP->num)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) {
            USB_EPStopXfer(pUSB, pEP);
        }

        USB_OUTEP(pEP->num)->DOEPCTL &= ~USB_OTG_DOEPCTL_USBAEP;
        USB_OUTEP(pEP->num)->DOEPCTL &= ~USB_OTG_DOEPCTL_EPENA;
        USB_OUTEP(pEP->num)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
        USB_DEVICE->DAINTMSK &= ~(USB_OTG_DAINTMSK_OEPM & ((1 << (pEP->num)) << 16));
    }

    if ((pEP->type == EP_TYPE_ISOC) && (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH)) {
        pEP->isocStart = 0;
    }

    return HAL_OK;
}

/**
 * @brief  De-activate and de-initialize a dedicated endpoint
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @return HAL status
 */
HAL_Status USB_DeactivateDedicatedEndpoint(struct USB_GLOBAL_REG *pUSB,
                                           struct USB_OTG_EP *pEP)
{
    /* Read DEPCTLn register */
    if (pEP->isIn == 1) {
        USB_INEP(pEP->num)->DIEPCTL &= ~USB_OTG_DIEPCTL_USBAEP;
        USB_DEVICE->DEACHMSK &= ~(USB_OTG_DAINTMSK_IEPM & ((1 << (pEP->num))));
    } else {
        USB_OUTEP(pEP->num)->DOEPCTL &= ~USB_OTG_DOEPCTL_USBAEP;
        USB_DEVICE->DEACHMSK &= ~(USB_OTG_DAINTMSK_OEPM & ((1 << (pEP->num)) << 16));
    }

    return HAL_OK;
}

/**
 * @brief  USB_EPStartXfer : setup and starts a transfer over an EP
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @param  dma USB dma enabled or disabled
 *          This parameter can be one of these values:
 *           0 : DMA feature not used
 *           1 : DMA feature used
 * @return HAL status
 */
HAL_Status USB_EPStartXfer(struct USB_GLOBAL_REG *pUSB,
                           struct USB_OTG_EP *pEP, uint8_t dma)
{
    uint16_t pktCnt = 0;

    /* IN endpoint */
    if (pEP->isIn == 1) {
        /* Zero Length Packet? */
        if (pEP->xferLen == 0) {
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);
            USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19));
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);
        } else {
            if (dma == 1) {
                HAL_DCACHE_CleanByRange((uint32_t)pEP->dmaAddr, pEP->xferLen);
            }

            /*
             * Program the transfer size and packet count
             * as follows: xfersize = N * maxPacket +
             * short_packet pktCnt = N + (short_packet
             * exist ? 1 : 0)
             */
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);
            USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT &
                                             (((pEP->xferLen + pEP->maxPacket - 1) /
                                               pEP->maxPacket) << 19));
            USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_XFRSIZ & pEP->xferLen);

            if (pEP->type == EP_TYPE_ISOC) {
                USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_MULCNT);
                USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_MULCNT & (1 << 29));
            }
        }

        if (dma == 1) {
            USB_INEP(pEP->num)->DIEPDMA = (uint32_t)(pEP->dmaAddr);
        } else {
            if (pEP->type != EP_TYPE_ISOC) {
                /* Enable the Tx FIFO Empty Interrupt for this EP */
                if (pEP->xferLen > 0) {
                    USB_DEVICE->DIEPEMPMSK |= 1 << pEP->num;
                }
            }
        }

        if (pEP->type == EP_TYPE_ISOC) {
            if (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH) {
                if (pEP->isocStart == 0) {
                    pEP->isocPending = 1;

                    return HAL_OK;
                }
            } else {
                if ((USB_DEVICE->DSTS & (1 << 8)) == 0) {
                    USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_SODDFRM;
                } else {
                    USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
                }
            }
        }

        /* EP enable, IN data in FIFO */
        USB_INEP(pEP->num)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK |
                                        USB_OTG_DIEPCTL_EPENA);

        if (pEP->type == EP_TYPE_ISOC) {
            /* Write packet after EP is enabled for slave mode */
            USB_WritePacket(pUSB, pEP->pxferBuff, pEP->num, pEP->xferLen, dma);
        }
    } else {
        /* OUT endpoint */
        /*
         * Program the transfer size and packet count as follows:
         * pktCnt = N
         * xfersize = N * maxPacket
         */
        USB_OUTEP(pEP->num)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_XFRSIZ);
        USB_OUTEP(pEP->num)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_PKTCNT);

        if (pEP->xferLen == 0) {
            USB_OUTEP(pEP->num)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_XFRSIZ &
                                              pEP->maxPacket);
            USB_OUTEP(pEP->num)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19));
        } else {
            pktCnt = (pEP->xferLen + pEP->maxPacket - 1) / pEP->maxPacket;
            USB_OUTEP(pEP->num)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT &
                                              (pktCnt << 19));
            USB_OUTEP(pEP->num)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_XFRSIZ &
                                              (pEP->maxPacket * pktCnt));
        }

        if (dma == 1) {
            HAL_DCACHE_CleanByRange((uint32_t)pEP->dmaAddr, pEP->xferLen);
            USB_OUTEP(pEP->num)->DOEPDMA = (uint32_t)pEP->dmaAddr;
        }

        if (pEP->type == EP_TYPE_ISOC) {
            if (USB_GetDevSpeed(pUSB) == USB_OTG_SPEED_HIGH) {
                if (pEP->isocStart == 0) {
                    pEP->isocPending = 1;

                    return HAL_OK;
                }
            } else {
                if ((USB_DEVICE->DSTS & (1 << 8)) == 0) {
                    USB_OUTEP(pEP->num)->DOEPCTL |= USB_OTG_DOEPCTL_SODDFRM;
                } else {
                    USB_OUTEP(pEP->num)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
                }
            }
        }
        /* EP enable */
        USB_OUTEP(pEP->num)->DOEPCTL |= (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
    }

    return HAL_OK;
}

/**
 * @brief  USB_EP0StartXfer : setup and starts a transfer over the EP  0
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @param  dma USB dma enabled or disabled
 *          This parameter can be one of these values:
 *           0 : DMA feature not used
 *           1 : DMA feature used
 * @return HAL status
 */
HAL_Status USB_EP0StartXfer(struct USB_GLOBAL_REG *pUSB,
                            struct USB_OTG_EP *pEP, uint8_t dma)
{
    /* IN endpoint */
    if (pEP->isIn == 1) {
        /* Zero Length Packet? */
        if (pEP->xferLen == 0) {
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);
            USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19));
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);
        } else {
            if (dma == 1) {
                HAL_DCACHE_CleanByRange((uint32_t)pEP->dmaAddr, pEP->xferLen);
            }

            /*
             * Program the transfer size and packet count
             * as follows: xfersize = N * maxPacket +
             * short_packet pktcnt = N + (short_packet
             * exist ? 1 : 0)
             */
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_XFRSIZ);
            USB_INEP(pEP->num)->DIEPTSIZ &= ~(USB_OTG_DIEPTSIZ_PKTCNT);

            if (pEP->xferLen > pEP->maxPacket) {
                pEP->xferLen = pEP->maxPacket;
            }

            USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19));
            USB_INEP(pEP->num)->DIEPTSIZ |= (USB_OTG_DIEPTSIZ_XFRSIZ & pEP->xferLen);
        }

        if (dma == 1) {
            USB_INEP(pEP->num)->DIEPDMA = (uint32_t)(pEP->dmaAddr);
        } else {
            /*
             * Enable the Tx FIFO Empty Interrupt for this EP
             * if the dedicated fifo mode is enabled.
             */

            if (((pUSB->GHWCFG4 & USB_OTG_GHWCFG4_DEDFIFOMODE_MASK) ==
                 USB_OTG_GHWCFG4_DEDFIFOMODE) && (pEP->xferLen > 0U)) {
                USB_DEVICE->DIEPEMPMSK |= 1U << (pEP->num);
            }
        }

        /* EP enable, IN data in FIFO */
        USB_INEP(pEP->num)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);

        /* Write packet after EP is enabled for slave mode */
        if ((dma == 0) && (pEP->xferLen > 0)) {
            USB_WritePacket(pUSB, pEP->pxferBuff, pEP->num, pEP->xferLen, dma);
        }
    } else {
        /* OUT endpoint */
        /*
         * Program the transfer size and packet count as follows:
         * pktcnt = N
         * xfersize = N * maxPacket
         */
        USB_OUTEP(pEP->num)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_XFRSIZ);
        USB_OUTEP(pEP->num)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_PKTCNT);
        USB_OUTEP(pEP->num)->DOEPTSIZ &= ~(USB_OTG_DOEPTSIZ_STUPCNT);

        if (pEP->xferLen > 0) {
            pEP->xferLen = pEP->maxPacket;
        }

        USB_OUTEP(pEP->num)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19));
        USB_OUTEP(pEP->num)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_XFRSIZ & pEP->xferLen);

        if (dma == 1) {
            USB_OUTEP(pEP->num)->DOEPDMA = (uint32_t)(pEP->dmaAddr);
        }

        /* EP enable */
        USB_OUTEP(pEP->num)->DOEPCTL |= (USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
    }

    return HAL_OK;
}

/**
 * @brief  USB_WritePacket : Writes a packet into the Tx FIFO associated
 *         with the EP/channel
 * @param  pUSB  Selected device
 * @param  psrc   pointer to source buffer
 * @param  chEpNum  endpoint or host channel number
 * @param  len  Number of bytes to write
 * @param  dma USB dma enabled or disabled
 *          This parameter can be one of these values:
 *           0 : DMA feature not used
 *           1 : DMA feature used
 * @return HAL status
 */
HAL_Status USB_WritePacket(struct USB_GLOBAL_REG *pUSB, uint8_t *psrc,
                           uint8_t chEpNum, uint16_t len, uint8_t dma)
{
    uint32_t lenWords = 0, i = 0;

    if (dma == 0) {
        lenWords = (len + 3) / 4;
        for (i = 0; i < lenWords; i++, psrc += 4) {
            USB_DFIFO(chEpNum) = __UNALIGNED_UINT32_READ(psrc);
        }
    }

    return HAL_OK;
}

/**
 * @brief  USB_ReadPacket : read a packet from the Tx FIFO associated
 *         with the EP/channel
 * @param  pUSB  Selected device
 * @param  pdest  destination buffer
 * @param  chEpNum  endpoint or host channel number
 * @param  len  Number of bytes to read
 * @param  dma USB dma enabled or disabled
 *          This parameter can be one of these values:
 *           0 : DMA feature not used
 *           1 : DMA feature used
 * @return pointer to destination buffer
 */
void *USB_ReadPacket(struct USB_GLOBAL_REG *pUSB, uint8_t *pdest,
                     uint8_t chEpNum, uint16_t len, uint8_t dma)
{
    uint32_t i = 0;

    if (dma == 0) {
        uint32_t lenWords = (len + 3) / 4;
        for (i = 0; i < lenWords; i++, pdest += 4) {
            __UNALIGNED_UINT32_WRITE(pdest, USB_DFIFO(chEpNum));
        }
    }

    return ((void *)pdest);
}

/**
 * @brief  USB_EPSetStall : set a stall condition over an EP
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @return HAL status
 */
HAL_Status USB_EPSetStall(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP)
{
    if (pEP->isIn == 1) {
        if (((USB_INEP(pEP->num)->DIEPCTL) & USB_OTG_DIEPCTL_EPENA) == 0) {
            USB_INEP(pEP->num)->DIEPCTL &= ~(USB_OTG_DIEPCTL_EPDIS);
        }

        USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_STALL;
    } else {
        if (((USB_OUTEP(pEP->num)->DOEPCTL) & USB_OTG_DOEPCTL_EPENA) == 0) {
            USB_OUTEP(pEP->num)->DOEPCTL &= ~(USB_OTG_DOEPCTL_EPDIS);
        }

        USB_OUTEP(pEP->num)->DOEPCTL |= USB_OTG_DOEPCTL_STALL;
    }

    return HAL_OK;
}

/**
 * @brief  USB_EPClearStall : Clear a stall condition over an EP
 * @param  pUSB  Selected device
 * @param  pEP pointer to endpoint structure
 * @return HAL status
 */
HAL_Status USB_EPClearStall(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP)
{
    if (pEP->isIn == 1) {
        USB_INEP(pEP->num)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
        if (pEP->type == EP_TYPE_INTR || pEP->type == EP_TYPE_BULK) {
            /* DATA0 */
            USB_INEP(pEP->num)->DIEPCTL |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
        }
    } else {
        USB_OUTEP(pEP->num)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
        if (pEP->type == EP_TYPE_INTR || pEP->type == EP_TYPE_BULK) {
            /* DATA0 */
            USB_OUTEP(pEP->num)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
        }
    }

    return HAL_OK;
}

/**
 * @brief  USB_StopDevice : Stop the usb device mode
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_StopDevice(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t i;

    /* Clear Pending interrupt */
    for (i = 0; i < 15; i++) {
        USB_INEP(i)->DIEPINT = 0xFF;
        USB_OUTEP(i)->DOEPINT = 0xFF;
    }
    USB_DEVICE->DAINT = 0xFFFFFFFF;

    /* Clear interrupt masks */
    USB_DEVICE->DIEPMSK = 0;
    USB_DEVICE->DOEPMSK = 0;
    USB_DEVICE->DAINTMSK = 0;

    /* Flush the FIFO */
    USB_FlushRxFifo(pUSB);
    USB_FlushTxFifo(pUSB, 0x10);

    return HAL_OK;
}

/**
 * @brief  USB_SetDevAddress : Stop the usb device mode
 * @param  pUSB  Selected device
 * @param  address  new device address to be assigned
 *          This parameter can be a value from 0 to 255
 * @return HAL status
 */
HAL_Status USB_SetDevAddress(struct USB_GLOBAL_REG *pUSB, uint8_t address)
{
    USB_DEVICE->DCFG &= ~(USB_OTG_DCFG_DAD);
    USB_DEVICE->DCFG |= (address << 4) & USB_OTG_DCFG_DAD;

    return HAL_OK;
}

/**
 * @brief  USB_DevConnect : Connect the USB device by enabling the pull-up/pull-down
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_DevConnect(struct USB_GLOBAL_REG *pUSB)
{
    USB_DEVICE->DCTL &= ~USB_OTG_DCTL_SDIS;

    return HAL_OK;
}

/**
 * @brief  USB_DevDisconnect : Disconnect the USB device by disabling the pull-up/pull-down
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_DevDisconnect(struct USB_GLOBAL_REG *pUSB)
{
    USB_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;
    HAL_DelayMs(3);

    return HAL_OK;
}

/**
 * @brief  USB_ReadInterrupts: return the global USB interrupt status
 * @param  pUSB  Selected device
 * @return HAL status
 */
uint32_t USB_ReadInterrupts(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t v = 0;

    v = pUSB->GINTSTS;
    v &= pUSB->GINTMSK;

    return v;
}

/**
 * @brief  USB_ReadDevAllOutEpInterrupt: return the USB device OUT endpoints interrupt status
 * @param  pUSB  Selected device
 * @return HAL status
 */
uint32_t USB_ReadDevAllOutEpInterrupt(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t v;

    v = USB_DEVICE->DAINT;
    v &= USB_DEVICE->DAINTMSK;

    return ((v & 0xffff0000) >> 16);
}

/**
 * @brief  USB_ReadDevAllInEpInterrupt: return the USB device IN endpoints interrupt status
 * @param  pUSB  Selected device
 * @return HAL status
 */
uint32_t USB_ReadDevAllInEpInterrupt(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t v;

    v = USB_DEVICE->DAINT;
    v &= USB_DEVICE->DAINTMSK;

    return ((v & 0xFFFF));
}

/**
 * @brief  Returns Device OUT EP Interrupt register
 * @param  pUSB  Selected device
 * @param  epNum  endpoint number
 *          This parameter can be a value from 0 to 15
 * @return Device OUT EP Interrupt register
 */
uint32_t USB_ReadDevOutEPInterrupt(struct USB_GLOBAL_REG *pUSB, uint8_t epNum)
{
    uint32_t v;

    v = USB_OUTEP(epNum)->DOEPINT;
    v &= (USB_DEVICE->DOEPMSK | USB_OTG_DOEPINT_STUPPKTRCVD_MASK);

    return v;
}

/**
 * @brief  Returns Device IN EP Interrupt register
 * @param  pUSB  Selected device
 * @param  epNum  endpoint number
 *          This parameter can be a value from 0 to 15
 * @return Device IN EP Interrupt register
 */
uint32_t USB_ReadDevInEPInterrupt(struct USB_GLOBAL_REG *pUSB, uint8_t epNum)
{
    uint32_t v, msk, emp;

    msk = USB_DEVICE->DIEPMSK;
    emp = USB_DEVICE->DIEPEMPMSK;
    msk |= ((emp >> epNum) & 0x1) << 7;
    v = USB_INEP(epNum)->DIEPINT & msk;

    return v;
}

/**
 * @brief  USB_ClearInterrupts: clear a USB interrupt
 * @param  pUSB  Selected device
 * @param  interrupt  interrupt flag
 */
void USB_ClearInterrupts(struct USB_GLOBAL_REG *pUSB, uint32_t interrupt)
{
    pUSB->GINTSTS |= interrupt;
}

/**
 * @brief  Returns USB core mode
 * @param  pUSB  Selected device
 * @return return core mode : Host or Device
 *          This parameter can be one of these values:
 *           0 : Device
 *           1 : Host
 */
uint32_t USB_GetMode(struct USB_GLOBAL_REG *pUSB)
{
    return ((pUSB->GINTSTS) & 0x1);
}

/**
 * @brief  Activate EP0 for Setup transactions
 * @param  pUSB  Selected device
 * @return HAL status
 */
HAL_Status USB_ActivateSetup(struct USB_GLOBAL_REG *pUSB)
{
    /* Set the MPS of the IN EP based on the enumeration speed */
    USB_INEP(0)->DIEPCTL &= ~USB_OTG_DIEPCTL_MPSIZ;

    if ((USB_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) == DSTS_ENUMSPD_LS_PHY_6MHZ) {
        USB_INEP(0)->DIEPCTL |= 3;
    }

    USB_DEVICE->DCTL |= USB_OTG_DCTL_CGINAK;

    return HAL_OK;
}

/**
 * @brief  Prepare the EP0 to start the first control setup
 * @param  pUSB  Selected device
 * @param  dma USB dma enabled or disabled
 *          This parameter can be one of these values:
 *           0 : DMA feature not used
 *           1 : DMA feature used
 * @param  psetup  pointer to setup packet
 * @return HAL status
 */
HAL_Status USB_EP0_OutStart(struct USB_GLOBAL_REG *pUSB,
                            uint8_t dma, uint8_t *psetup)
{
    USB_OUTEP(0)->DOEPTSIZ = 0;
    USB_OUTEP(0)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19));
    USB_OUTEP(0)->DOEPTSIZ |= (3 * 8);

    if (dma == 1) {
        USB_OUTEP(0)->DOEPDMA = (uint32_t)psetup;
        /* EP enable, for Setup request do not clear NAK */
        USB_OUTEP(0)->DOEPCTL = USB_OTG_DOEPCTL_EPENA;
    } else {
        /* Slave mode */
        USB_OUTEP(0)->DOEPCTL = USB_OTG_DOEPCTL_EPENA;
    }

    return HAL_OK;
}

/**
 * @brief  USB_HostInit : Initializes the USB OTG controller registers
 *         for Host mode
 * @param  pUSB  Selected device
 * @param  cfg   pointer to a USB_OTG_CFG structure that contains
 *         the configuration information for the specified pUSB peripheral.
 * @return HAL status
 */
HAL_Status USB_HostInit(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_CFG cfg)
{
    uint32_t i;

    /* Restart the Phy Clock */
    USB_PCGCCTL = 0;

    if (cfg.speed == USB_OTG_SPEED_HIGH) {
        /* Set default Max speed support */
        USB_HOST->HCFG &= ~(USB_OTG_HCFG_FSLSS);
    } else {
        /* Force Device Enumeration to FS/LS mode only */
        USB_HOST->HCFG |= USB_OTG_HCFG_FSLSS;
    }

    /* Make sure the FIFOs are flushed. */
    USB_FlushTxFifo(pUSB, 0x10);  /* all Tx FIFOs */
    USB_FlushRxFifo(pUSB);

    /* Clear all pending HC Interrupts */
    for (i = 0; i < cfg.hcNum; i++) {
        USB_HC(i)->HCINT = 0xFFFFFFFFU;
        USB_HC(i)->HCINTMSK = 0;
    }

    /* Enable VBUS driving */
    USB_DriveVbus(pUSB, 1);

    HAL_DelayMs(200);

    /* Disable all interrupts. */
    pUSB->GINTMSK = 0;

    /* Clear any pending interrupts */
    pUSB->GINTSTS = 0xFFFFFFFFU;

    /* set Rx FIFO size */
    pUSB->GRXFSIZ = (uint32_t)0x200U;
    pUSB->DIEPTXF0_HNPTXFSIZ = (uint32_t)(((0x100U << 16) &
                                           USB_OTG_GNPTXFSIZ_NPTXFD) | 0x200U);
    pUSB->HPTXFSIZ = (uint32_t)(((0xE0U << 16) &
                                 USB_OTG_HPTXFSIZ_PTXFD) | 0x300U);

    /* Enable the common interrupts */
    if (cfg.dmaEnable == HAL_DISABLE) {
        pUSB->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;
    }

    /* Enable interrupts matching to the Host mode ONLY */
    pUSB->GINTMSK |= (USB_OTG_GINTMSK_PRTIM | USB_OTG_GINTMSK_HCIM |
                      USB_OTG_GINTMSK_SOFM | USB_OTG_GINTSTS_DISCINT |
                      USB_OTG_GINTMSK_PXFRM_IISOOXFRM |
                      USB_OTG_GINTMSK_WUIM);

    return HAL_OK;
}

/**
 * @brief  USB_InitFSLSPClkSel : Initializes the FSLSPClkSel field of the
 *         HCFG register on the PHY type and set the right frame interval
 * @param  pUSB  Selected device
 * @param  freq  clock frequency
 *          This parameter can be one of these values:
 *           HCFG_48_MHZ : Full Speed 48 MHz Clock
 *           HCFG_6_MHZ : Low Speed 6 MHz Clock
 * @return HAL status
 */
HAL_Status USB_InitFSLSPClkSel(struct USB_GLOBAL_REG *pUSB, uint8_t freq)
{
    USB_HOST->HCFG &= ~(USB_OTG_HCFG_FSLSPCS);
    USB_HOST->HCFG |= ((uint32_t)freq & USB_OTG_HCFG_FSLSPCS);

    if (freq == HCFG_48_MHZ) {
        USB_HOST->HFIR = (uint32_t)48000U;
    } else if (freq == HCFG_6_MHZ) {
        USB_HOST->HFIR = (uint32_t)6000U;
    }

    return HAL_OK;
}

/**
 * @brief  USB_OTG_ResetPort : Reset Host Port
 * @param  pUSB  Selected device
 * @return HAL status
 * @note : (1)The application must wait at least 10 ms
 *   before clearing the reset bit.
 */
HAL_Status USB_ResetPort(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t hprt0;

    hprt0 = USB_HPRT0;

    hprt0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |
               USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

    USB_HPRT0 = (USB_OTG_HPRT_PRST | hprt0);
    HAL_DelayMs(100);
    /* See Note #1 */
    USB_HPRT0 = ((~USB_OTG_HPRT_PRST) & hprt0);
    HAL_DelayMs(10);

    return HAL_OK;
}

/**
 * @brief  USB_DriveVbus : activate or de-activate vbus
 * @param  pUSB  Selected device
 * @param  state  VBUS state
 *          This parameter can be one of these values:
 *           0 : VBUS Active
 *           1 : VBUS Inactive
 * @return HAL status
 */
HAL_Status USB_DriveVbus(struct USB_GLOBAL_REG *pUSB, uint8_t state)
{
    uint32_t hprt0;

    hprt0 = USB_HPRT0;

    hprt0 &= ~(USB_OTG_HPRT_PENA | USB_OTG_HPRT_PCDET |
               USB_OTG_HPRT_PENCHNG | USB_OTG_HPRT_POCCHNG);

    if (((hprt0 & USB_OTG_HPRT_PPWR) == 0) && (state == 1)) {
        USB_HPRT0 = (USB_OTG_HPRT_PPWR | hprt0);
    }

    if (((hprt0 & USB_OTG_HPRT_PPWR) == USB_OTG_HPRT_PPWR) && (state == 0)) {
        USB_HPRT0 = ((~USB_OTG_HPRT_PPWR) & hprt0);
    }

    return HAL_OK;
}

/**
 * @brief  Return Host Core speed
 * @param  pUSB  Selected device
 * @return speed : Host speed
 *          This parameter can be one of these values:
 *            @arg USB_OTG_SPEED_HIGH: High speed mode
 *            @arg USB_OTG_SPEED_FULL: Full speed mode
 *            @arg USB_OTG_SPEED_LOW: Low speed mode
 */
uint32_t USB_GetHostSpeed(struct USB_GLOBAL_REG *pUSB)
{
    uint32_t hprt0;

    hprt0 = USB_HPRT0;

    return ((hprt0 & USB_OTG_HPRT_PSPD) >> 17);
}

/**
 * @brief  Return Host Current Frame number
 * @param  pUSB  Selected device
 * @return current frame number
 */
uint32_t USB_GetCurrentFrame(struct USB_GLOBAL_REG *pUSB)
{
    return (USB_HOST->HFNUM & USB_OTG_HFNUM_FRNUM);
}

/**
 * @brief  Initialize a host channel
 * @param  pUSB  Selected device
 * @param  chNum  Channel number
 *         This parameter can be a value from 1 to 15
 * @param  epNum  Endpoint number
 *         This parameter can be a value from 1 to 15
 * @param  devAddress  Current device address
 *         This parameter can be a value from 0 to 255
 * @param  speed  Current device speed
 *         This parameter can be one of these values:
 *            @arg USB_OTG_SPEED_HIGH: High speed mode
 *            @arg USB_OTG_SPEED_FULL: Full speed mode
 *            @arg USB_OTG_SPEED_LOW: Low speed mode
 * @param  epType  Endpoint Type
 *         This parameter can be one of these values:
 *            @arg EP_TYPE_CTRL: Control type
 *            @arg EP_TYPE_ISOC: Isochronous type
 *            @arg EP_TYPE_BULK: Bulk type
 *            @arg EP_TYPE_INTR: Interrupt type
 * @param  mps  Max Packet Size
 *         This parameter can be a value from 0 to32K
 * @return HAL state
 */
HAL_Status USB_HCInit(struct USB_GLOBAL_REG *pUSB,
                      uint8_t chNum,
                      uint8_t epNum,
                      uint8_t devAddress,
                      uint8_t speed,
                      uint8_t epType,
                      uint16_t mps)
{
    uint32_t HCcharEpDir, HCcharLowSpeed;

    /* Clear old interrupt conditions for this host channel. */
    USB_HC(chNum)->HCINT = 0xFFFFFFFFU;

    /* Enable channel interrupts required for this transfer. */
    switch (epType) {
    case EP_TYPE_CTRL:
    case EP_TYPE_BULK:
        USB_HC(chNum)->HCINTMSK = USB_OTG_HCINTMSK_XFRCM |
                                  USB_OTG_HCINTMSK_STALLM |
                                  USB_OTG_HCINTMSK_TXERRM |
                                  USB_OTG_HCINTMSK_DTERRM |
                                  USB_OTG_HCINTMSK_AHBERR |
                                  USB_OTG_HCINTMSK_NAKM;

        if (epNum & 0x80U) {
            USB_HC(chNum)->HCINTMSK |= USB_OTG_HCINTMSK_BBERRM;
        } else {
            USB_HC(chNum)->HCINTMSK |= (USB_OTG_HCINTMSK_NYET |
                                        USB_OTG_HCINTMSK_ACKM);
        }
        break;
    case EP_TYPE_INTR:
        USB_HC(chNum)->HCINTMSK = USB_OTG_HCINTMSK_XFRCM |
                                  USB_OTG_HCINTMSK_STALLM |
                                  USB_OTG_HCINTMSK_TXERRM |
                                  USB_OTG_HCINTMSK_DTERRM |
                                  USB_OTG_HCINTMSK_NAKM |
                                  USB_OTG_HCINTMSK_AHBERR |
                                  USB_OTG_HCINTMSK_FRMORM;
        if (epNum & 0x80U) {
            USB_HC(chNum)->HCINTMSK |= USB_OTG_HCINTMSK_BBERRM;
        }

        break;
    case EP_TYPE_ISOC:
        USB_HC(chNum)->HCINTMSK = USB_OTG_HCINTMSK_XFRCM |
                                  USB_OTG_HCINTMSK_ACKM |
                                  USB_OTG_HCINTMSK_AHBERR |
                                  USB_OTG_HCINTMSK_FRMORM;

        if (epNum & 0x80U) {
            USB_HC(chNum)->HCINTMSK |= (USB_OTG_HCINTMSK_TXERRM |
                                        USB_OTG_HCINTMSK_BBERRM);
        }
        break;

    default:

        return HAL_ERROR;
    }

    /* Enable the top level host channel interrupt. */
    USB_HOST->HAINTMSK |= (1UL << (chNum & 0xFU));

    /* Make sure host channel interrupts are enabled. */
    pUSB->GINTMSK |= USB_OTG_GINTMSK_HCIM;

    /* Program the HCCHAR register */
    HCcharEpDir = (epNum & 0x80U) ? (0x1U << 15) & USB_OTG_HCCHAR_EPDIR : 0U;
    HCcharLowSpeed = (speed == HPRT0_PRTSPD_LOW_SPEED) ? (0x1U << 17) & USB_OTG_HCCHAR_LSDEV : 0U;

    USB_HC(chNum)->HCCHAR = (((uint32_t)devAddress << 22) & USB_OTG_HCCHAR_DAD) |
                            ((((uint32_t)epNum & 0x7FU) << 11) & USB_OTG_HCCHAR_EPNUM) |
                            (((uint32_t)epType << 18) & USB_OTG_HCCHAR_EPTYP) |
                            ((uint32_t)mps & USB_OTG_HCCHAR_MPSIZ) |
                            HCcharEpDir | HCcharLowSpeed;

    if (epType == EP_TYPE_INTR) {
        USB_HC(chNum)->HCCHAR |= USB_OTG_HCCHAR_ODDFRM;
    }

    return HAL_OK;
}

/**
 * @brief  Start a transfer over a host channel
 * @param  pUSB  Selected device
 * @param  pHC  pointer to host channel structure
 * @param  dma USB dma enabled or disabled
 *          This parameter can be one of these values:
 *           0 : DMA feature not used
 *           1 : DMA feature used
 * @return HAL state
 */
HAL_Status USB_HCStartXfer(struct USB_GLOBAL_REG *pUSB,
                           struct USB_OTG_HC *pHC, uint8_t dma)
{
    uint32_t tmpReg;
    uint16_t lenWords = 0;
    uint16_t numPackets = 0;
    uint16_t maxHcPktCnt = 256;
    uint8_t isOddFrame = 0;

    if (pHC->speed == USB_OTG_SPEED_HIGH) {
        if ((dma == 0) && (pHC->doPing == 1)) {
            USB_DoPing(pUSB, pHC->chNum);

            return HAL_OK;
        } else if (dma == 1) {
            USB_HC(pHC->chNum)->HCINTMSK &= ~(USB_OTG_HCINTMSK_NYET |
                                              USB_OTG_HCINTMSK_ACKM);
            pHC->doPing = 0;
        }
    }

    /* Compute the expected number of packets associated to the transfer */
    if (pHC->xferLen > 0) {
        numPackets = (uint16_t)((pHC->xferLen + pHC->maxPacket - 1) / pHC->maxPacket);

        if (numPackets > maxHcPktCnt) {
            numPackets = maxHcPktCnt;
            pHC->xferLen = (uint32_t)numPackets * pHC->maxPacket;
        }
    } else {
        numPackets = 1;
    }

    if (pHC->epIsIn) {
        pHC->xferLen = (uint32_t)numPackets * pHC->maxPacket;
    }

    /* Initialize the HCTSIZn register */
    USB_HC(pHC->chNum)->HCTSIZ = (((pHC->xferLen) & USB_OTG_HCTSIZ_XFRSIZ)) |
                                 (((uint32_t)numPackets << 19) & USB_OTG_HCTSIZ_PKTCNT) |
                                 ((((uint32_t)pHC->dataPID) << 29) & USB_OTG_HCTSIZ_DPID);

    if (dma) {
        HAL_DCACHE_CleanByRange((uint32_t)pHC->pxferBuff, pHC->xferLen);

        /* pxferBuff MUST be 32-bits aligned */
        USB_HC(pHC->chNum)->HCDMA = (uint32_t)pHC->dmaAddr;
    }

    isOddFrame = ((uint32_t)USB_HOST->HFNUM & 0x01) ? 0 : 1;
    USB_HC(pHC->chNum)->HCCHAR &= ~USB_OTG_HCCHAR_ODDFRM;
    USB_HC(pHC->chNum)->HCCHAR |= ((uint32_t)isOddFrame << 29);

    /* Set host channel enable */
    tmpReg = USB_HC(pHC->chNum)->HCCHAR;
    tmpReg &= ~USB_OTG_HCCHAR_CHDIS;

    /* make sure to set the correct ep direction */
    if (pHC->epIsIn) {
        tmpReg |= USB_OTG_HCCHAR_EPDIR;
    } else {
        tmpReg &= ~USB_OTG_HCCHAR_EPDIR;
    }

    tmpReg |= USB_OTG_HCCHAR_CHENA;
    USB_HC(pHC->chNum)->HCCHAR = tmpReg;

    if (dma == 0) { /* Slave mode */
        if ((pHC->epIsIn == 0) && (pHC->xferLen > 0)) {
            switch (pHC->epType) {
            /* Non periodic transfer */
            case EP_TYPE_CTRL:
            case EP_TYPE_BULK:

                lenWords = (uint16_t)((pHC->xferLen + 3) / 4);

                /* check if there is enough space in FIFO space */
                if (lenWords > (pUSB->HNPTXSTS & 0xFFFFU)) {
                    /* need to process data in nptxfempty interrupt */
                    pUSB->GINTMSK |= USB_OTG_GINTMSK_NPTXFEM;
                }
                break;
            /* Periodic transfer */
            case EP_TYPE_INTR:
            case EP_TYPE_ISOC:
                lenWords = (uint16_t)((pHC->xferLen + 3) / 4);
                /* check if there is enough space in FIFO space */
                if (lenWords > (USB_HOST->HPTXSTS & 0xFFFFU)) {
                    /* split the transfer */
                    /* need to process data in ptxfempty interrupt */
                    pUSB->GINTMSK |= USB_OTG_GINTMSK_PTXFEM;
                }
                break;

            default:
                break;
            }

            /* Write packet into the Tx FIFO. */
            USB_WritePacket(pUSB, pHC->pxferBuff, pHC->chNum, (uint16_t)pHC->xferLen, 0);
        }
    }

    return HAL_OK;
}

/**
 * @brief Read all host channel interrupts status
 * @param  pUSB  Selected device
 * @return HAL state
 */
uint32_t USB_HCReadInterrupt(struct USB_GLOBAL_REG *pUSB)
{
    return ((USB_HOST->HAINT) & 0xFFFFU);
}

/**
 * @brief  Halt a host channel
 * @param  pUSB  Selected device
 * @param  hcNum  Host Channel number
 *         This parameter can be a value from 1 to 15
 * @return HAL state
 */
HAL_Status USB_HCHalt(struct USB_GLOBAL_REG *pUSB, uint8_t hcNum)
{
    uint32_t count = 0;
    uint32_t HcEpType = (USB_HC(hcNum)->HCCHAR & USB_OTG_HCCHAR_EPTYP) >> 18;

    /* Check for space in the request queue to issue the halt. */
    if ((HcEpType == HCCHAR_CTRL) || (HcEpType == HCCHAR_BULK)) {
        USB_HC(hcNum)->HCCHAR |= USB_OTG_HCCHAR_CHDIS;

        if ((pUSB->HNPTXSTS & (0xFFU << 16)) == 0) {
            USB_HC(hcNum)->HCCHAR &= ~USB_OTG_HCCHAR_CHENA;
            USB_HC(hcNum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;
            USB_HC(hcNum)->HCCHAR &= ~USB_OTG_HCCHAR_EPDIR;
            do {
                if (++count > 1000) {
                    break;
                }
            } while ((USB_HC(hcNum)->HCCHAR & USB_OTG_HCCHAR_CHENA) ==
                     USB_OTG_HCCHAR_CHENA);
        } else {
            USB_HC(hcNum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;
        }
    } else {
        USB_HC(hcNum)->HCCHAR |= USB_OTG_HCCHAR_CHDIS;

        if ((USB_HOST->HPTXSTS & 0xFFU << 16) == 0) {
            USB_HC(hcNum)->HCCHAR &= ~USB_OTG_HCCHAR_CHENA;
            USB_HC(hcNum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;
            USB_HC(hcNum)->HCCHAR &= ~USB_OTG_HCCHAR_EPDIR;
            do {
                if (++count > 1000) {
                    break;
                }
            } while ((USB_HC(hcNum)->HCCHAR & USB_OTG_HCCHAR_CHENA) ==
                     USB_OTG_HCCHAR_CHENA);
        } else {
            USB_HC(hcNum)->HCCHAR |= USB_OTG_HCCHAR_CHENA;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Initiate Do Ping protocol
 * @param  pUSB  Selected device
 * @param  chNum  Host Channel number
 *         This parameter can be a value from 1 to 15
 * @return HAL state
 */
HAL_Status USB_DoPing(struct USB_GLOBAL_REG *pUSB, uint8_t chNum)
{
    uint8_t numPackets = 1;
    uint32_t tmpReg = 0;

    USB_HC(chNum)->HCTSIZ = ((numPackets << 19) & USB_OTG_HCTSIZ_PKTCNT) |
                            USB_OTG_HCTSIZ_DOPING;

    /* Set host channel enable */
    tmpReg = USB_HC(chNum)->HCCHAR;
    tmpReg &= ~USB_OTG_HCCHAR_CHDIS;
    tmpReg |= USB_OTG_HCCHAR_CHENA;
    USB_HC(chNum)->HCCHAR = tmpReg;

    return HAL_OK;
}

/**
 * @brief  Stop Host Core
 * @param  pUSB  Selected device
 * @return HAL state
 */
HAL_Status USB_StopHost(struct USB_GLOBAL_REG *pUSB)
{
    uint8_t i;
    uint32_t count = 0;
    uint32_t value;

    USB_DisableGlobalInt(pUSB);

    /* Flush FIFO */
    USB_FlushTxFifo(pUSB, 0x10);
    USB_FlushRxFifo(pUSB);

    /* Flush out any leftover queued requests. */
    for (i = 0; i <= 15; i++) {
        value = USB_HC(i)->HCCHAR;
        value |= USB_OTG_HCCHAR_CHDIS;
        value &= ~USB_OTG_HCCHAR_CHENA;
        value &= ~USB_OTG_HCCHAR_EPDIR;
        USB_HC(i)->HCCHAR = value;
    }

    /* Halt all channels to put them into a known state. */
    for (i = 0; i <= 15; i++) {
        value = USB_HC(i)->HCCHAR;

        value |= USB_OTG_HCCHAR_CHDIS;
        value |= USB_OTG_HCCHAR_CHENA;
        value &= ~USB_OTG_HCCHAR_EPDIR;

        USB_HC(i)->HCCHAR = value;
        do {
            if (++count > 1000) {
                break;
            }
        } while ((USB_HC(i)->HCCHAR & USB_OTG_HCCHAR_CHENA) ==
                 USB_OTG_HCCHAR_CHENA);
    }

    /* Clear any pending Host interrupts */
    USB_HOST->HAINT = 0xFFFFFFFFU;
    pUSB->GINTSTS = 0xFFFFFFFFU;
    USB_EnableGlobalInt(pUSB);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* defined (HAL_PCD_MODULE_ENABLED) || defined (HAL_HCD_MODULE_ENABLED) */
