/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_EHCI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup EHCI
 *  @{
 */

/** @defgroup EHCI_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

static const uint16_t g_sitdOutSmask [] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f };

/********************* Private Function Definition ***************************/

static HAL_Status EHCI_IsoSplitXfer(struct UTR *utr, struct ECHI_ISO_EP *isoEp);

/*
 *  Inspect the iTD can be reclaimed or not. If yes, collect the transaction results.
 *  Return:  1 - reclaimed
 *           0 - not completed
 */
static HAL_Check EHCI_ReviewItd(struct EHCI_REG *pReg, struct EHCI_ITD *itd)
{
    struct UTR *utr;
    uint32_t frameIdx = itd->schedFrameIdx;
    uint32_t frameCurr = (pReg->FRINDEX >> 3) & 0x3FF;
    int i, fidx;

    // HAL_DBG("R - %d %d, 0x%lx\n", frameCurr, frameIdx, itd->transaction[0]);

    if (frameCurr == frameIdx) {
        for (i = 0; i < 8; i++) {
            if (itd->transaction[i] & ITD_STATUS_ACTIVE) {
                return HAL_FALSE; /* Have any not completed frames */
            }
        }
    } else if (frameCurr > frameIdx) {
        if ((frameCurr - frameIdx) > EHCI_ISO_RCLM_RANGE) {
            return HAL_FALSE; /* Don't touch it */
        }
    } else {
        if (frameCurr + FL_SIZE - frameIdx > EHCI_ISO_RCLM_RANGE) {
            return HAL_FALSE; /* Don't touch it */
        }
    }

    /* Reclaim this iTD */
    utr = itd->utr;
    fidx = itd->fIdx;
    for (i = 0; i < 8; i++) {
        if (!(itd->transMask & (0x1 << i))) {
            continue; /* Don't scheduled micro-frame  */
        }
        if (ITD_STATUS(itd->transaction[i])) {
            if (itd->transaction[i] & ITD_STATUS_ACTIVE) {
                utr->isoStatus[fidx] = USBH_ERR_NOT_ACCESS0;
                utr->status = USBH_ERR_NOT_ACCESS0;
            } else if (itd->transaction[i] & ITD_STATUS_BABBLE) {
                utr->isoStatus[fidx] = USBH_ERR_BABBLE_DETECTED;
                utr->status = USBH_ERR_TRANSFER;
            } else if (itd->transaction[i] & ITD_STATUS_BUFF_ERR) {
                utr->isoStatus[fidx] = USBH_ERR_DATA_BUFF;
                utr->status = USBH_ERR_TRANSFER;
            } else {
                utr->isoStatus[fidx] = USBH_ERR_TRANSACTION;
                utr->status = USBH_ERR_TRANSFER;
            }
        } else {
            utr->isoStatus[fidx] = 0;
            utr->isoXlen[fidx] = ITD_XFER_LEN(itd->transaction[i]);
        }
        fidx++;
    }
    utr->tdCnt--;

    if (utr->tdCnt == 0) { /* All iTD of this UTR done */
        utr->bIsTransferDone = 1;
        if (utr->CompleteCb) {
            utr->CompleteCb(utr);
        }
    }

    return HAL_TRUE; /* To be reclaimed */
}

/*
 *  Inspect the siTD can be reclaimed or not. If yes, collect the transaction results.
 *  Return:  1 - reclaimed
 *           0 - not completed
 */
static HAL_Check EHCI_ReviewSitd(struct EHCI_REG *pReg, struct EHCI_SITD *sitd)
{
    struct UTR *utr;
    uint32_t frameIdx = sitd->schedFrameIdx;
    uint32_t frameCurr = (pReg->FRINDEX >> 3) & 0x3FF;
    int fidx;
    uint32_t totalBytesToTransfer;

    if (frameCurr == frameIdx) {
        if (SITD_STATUS(sitd->stsCtrl) == SITD_STATUS_ACTIVE) {
            return HAL_FALSE;
        }
    } else if (frameCurr > frameIdx) {
        if ((frameCurr - frameIdx) > EHCI_ISO_RCLM_RANGE) {
            return HAL_FALSE; /* Don't touch it */
        }
    } else {
        if (frameCurr + FL_SIZE - frameIdx > EHCI_ISO_RCLM_RANGE) {
            return HAL_FALSE; /* Don't touch it */
        }
    }

    /* Reclaim this siTD */
    utr = sitd->utr;
    fidx = sitd->fIdx;

    if (SITD_STATUS(sitd->stsCtrl)) {
        if (sitd->stsCtrl & SITD_STATUS_ACTIVE) {
            utr->isoStatus[fidx] = USBH_ERR_NOT_ACCESS0;
        } else if (sitd->stsCtrl & SITD_BABBLE_DETECTED) {
            utr->isoStatus[fidx] = USBH_ERR_BABBLE_DETECTED;
            utr->status = USBH_ERR_TRANSFER;
        } else if (sitd->stsCtrl & SITD_STATUS_BUFF_ERR) {
            utr->isoStatus[fidx] = USBH_ERR_DATA_BUFF;
            utr->status = USBH_ERR_TRANSFER;
        } else {
            utr->isoStatus[fidx] = USBH_ERR_TRANSACTION;
            utr->status = USBH_ERR_TRANSFER;
        }
    } else {
        totalBytesToTransfer = (sitd->stsCtrl & SITD_XFER_CNT_MASK) >>
                               SITD_XFER_CNT_SHIFT;
        utr->isoXlen[fidx] = utr->isoXlen[fidx] - totalBytesToTransfer;
        utr->isoStatus[fidx] = 0;
    }
    utr->tdCnt--;

    if (utr->tdCnt == 0) {  /* All iTD of this UTR done  */
        utr->bIsTransferDone = 1;
        if (utr->CompleteCb) {
            utr->CompleteCb(utr);
        }
    }

    return HAL_TRUE;  /* To be reclaimed */
}

static void EHCI_WriteItdInfo(struct UTR *utr, struct EHCI_ITD *itd)
{
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep; /* Reference to isochronous endpoint */
    uint32_t bufPageAddr;
    int i;

    bufPageAddr = itd->bufBase & 0xFFFFF000; /* 4K page */

    for (i = 0; i < 7; i++) {
        itd->bufPage[i] = bufPageAddr + (0x1000 * i);
    }

    itd->bufPage[0] |= (udev->devNum) | ((ep->bEndpointAddress & 0xF) << ITD_EP_NUM_SHIFT);
    itd->bufPage[1] |= ep->wMaxPacketSize;

    if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_IN) {
        itd->bufPage[1] |= ITD_DIR_IN;
    } else {
        itd->bufPage[1] |= ITD_DIR_OUT;
    }

    itd->bufPage[2] |= (ep->wMaxPacketSize + 1023) / 1024;
}

static void EHCI_WriteItdMicroFrame(struct UTR *utr, int fidx, struct EHCI_ITD *itd, int mf)
{
    uint32_t bufAddr;

    /* Xfer buffer start address of this frame */
    bufAddr = (uint32_t)(utr->isoBuff[fidx]);

    itd->transaction[mf] = ITD_STATUS_ACTIVE | /* Status */
                           ((utr->isoXlen[fidx] & 0xFFF) << ITD_XLEN_SHIFT) | /* Transaction Length */
                           ((bufAddr & 0xFFFFF000) - (itd->bufBase & 0xFFFFF000)) | /* PG */
                           (bufAddr & 0xFFF); /* Transaction offset */
}

static void EHCI_RemoveIsoEpFromList(struct EHCI_HCD *ehci, struct ECHI_ISO_EP *isoEp)
{
    struct ECHI_ISO_EP *p;

    if (ehci->isoEpList == isoEp) {
        ehci->isoEpList = isoEp->next; /* It's the first entry, remove it */

        return;
    }

    p = ehci->isoEpList; /* Find the previous entry of isoEp */
    while (p->next != NULL) {
        if (p->next == isoEp) {
            break;
        }
        p = p->next;
    }

    if (p->next == NULL) {
        return; /* Not found */
    }
    p->next = isoEp->next; /* Remove isoEp from list */
}

static void EHCI_AddItdToIsoEp(struct ECHI_ISO_EP *isoEp, struct EHCI_ITD *itd)
{
    struct EHCI_ITD *p;

    itd->next = NULL;

    if (isoEp->iTDList == NULL) {
        isoEp->iTDList = itd;

        return;
    }

    /* Find the tail entry of isoEp->iTDList */
    p = isoEp->iTDList;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = itd;
}

static void EHCI_AddSitdToIsoEp(struct ECHI_ISO_EP *isoEp,
                                struct EHCI_SITD *sitd)
{
    struct EHCI_SITD *p;

    sitd->next = NULL;

    if (isoEp->siTDdoneList == NULL) {
        isoEp->siTDdoneList = sitd;

        return;
    }

    /* Find the tail entry of isoEp->iTDList */
    p = isoEp->siTDdoneList;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = sitd;
}

static void EHCI_WriteSitdInfo(struct UTR *utr, struct EHCI_SITD *sitd)
{
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep; /* Reference to isochronous endpoint */
    uint32_t bufPageAddr;
    int xlen = utr->isoXlen[sitd->fIdx];
    int scnt;

    sitd->chrst = (udev->portNum << SITD_PORT_NUM_SHIFT) |
                  /* (udev->parent->iface->udev->dev_num << SITD_HUB_ADDR_SHIFT) | */
                  ((ep->bEndpointAddress & 0xF) << SITD_EP_NUM_SHIFT) |
                  (udev->devNum << SITD_DEV_ADDR_SHIFT);

    bufPageAddr = ((uint32_t)utr->isoBuff[sitd->fIdx]) & 0xFFFFF000;
    sitd->bufPage[0] = (uint32_t)(utr->isoBuff[sitd->fIdx]);
    sitd->bufPage[1] = bufPageAddr + 0x1000;

    scnt = (xlen + 187) / 188;

    if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_IN) {
        sitd->chrst |= SITD_XFER_IN;
        sitd->sched = (1 << (scnt + 2)) - 1;
        sitd->sched = (sitd->sched << 10) | 0x1;
        /* sitd->sched <<= 1; */
    } else {
        sitd->chrst |= SITD_XFER_OUT;
        sitd->sched = g_sitdOutSmask[scnt - 1];
        if (scnt > 1) {
            sitd->bufPage[1] |= (0x1 << 3); /* Transaction position (TP)  01b: Begin */
        }
        sitd->bufPage[1] |= scnt; /* Transaction count (T-Count) */
    }

    if (sitd->fIdx == IF_PER_UTR) {
        sitd->sched |= SITD_IOC;
    }

    sitd->stsCtrl = (xlen << SITD_XFER_CNT_SHIFT) | SITD_STATUS_ACTIVE;
    sitd->backLink = SITD_LIST_END;
}

static void EHCI_SitdAdjustSchedule(struct EHCI_HCD *ehci,
                                    struct EHCI_SITD *sitd)
{
    struct EHCI_SITD *hlink = (struct EHCI_SITD *)ehci->pfList[sitd->schedFrameIdx];
    uint32_t uframeMask = 0x00;

    while (hlink && !HLINK_IS_TERMINATED(hlink) && HLINK_IS_SITD(hlink)) {
        hlink = SITD_PTR(hlink);
        if (hlink != sitd) {
            if ((hlink->chrst & SITD_XFER_IO_MASK) == SITD_XFER_IN) {
                uframeMask |= (hlink->sched & 0xFF);         /* mark micro-frames used by IN S-mask   */
                uframeMask |= ((hlink->sched >> 8) & 0xFF);  /* mark micro-frames used by IN C-mask   */
            } else {
                uframeMask |= (hlink->sched & 0xFF);         /* mark micro-frames used by OUT S-mask  */
            }
        }
        hlink = SITD_PTR(hlink->nextLink);
    }

    uframeMask = uframeMask | (uframeMask << 8);             /* mark both S-mask and C-mask           */

    if (uframeMask) {
        /* Shift afterward one micro-frame until no conflicts. */
        while (1) {
            if (sitd->sched & uframeMask) {
                sitd->sched = (sitd->sched & 0xFFFF0000) | ((sitd->sched << 1) & 0xFFFF);
            } else {
                break; /* no conflit, done. */
            }
        }
    }
}

static HAL_Status EHCI_IsoSplitXfer(struct UTR *utr, struct ECHI_ISO_EP *isoEp)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)utr->pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct USB_EP_INFO *ep = utr->ep; /* Reference to isochronous endpoint */
    struct EHCI_SITD *sitd, *sitdNext, *sitdList = NULL;
    int i;
    int fidx; /* Index to the 8 iso frames of UTR */

/*    if (utr->udev->parent == NULL) */
    {
        HAL_DBG_ERR("siso xfer - parent lost!\n");

        return HAL_INVAL;
    }

    /* Allocate siTDs */
    for (i = 0; i < IF_PER_UTR; i++) { /* allocate all siTDs required by UTR */
        sitd = (struct EHCI_SITD *)HAL_USBH_AllocPool();
        if (sitd == NULL) {
            goto error;
        }

        if (sitdList == NULL) { /* link all siTDs */
            sitdList = sitd;
        } else {
            sitd->next = sitdList;
            sitdList = sitd;
        }
    }

    utr->tdCnt = IF_PER_UTR;

    /* Fill and link all siTDs */
    utr->isoSF = isoEp->nextFrame;
    fidx = 0; /* Index to UTR iso frmes (total IF_PER_UTR) */

    for (sitd = sitdList; (sitd != NULL); fidx++) {
        if (fidx >= IF_PER_UTR) { /* unlikely */
            HAL_DBG_ERR("EHCI driver siTD bug!?\n");
            goto error;
        }

        sitd->utr = utr;
        sitd->fIdx = fidx; /* Index to UTR's n'th IF_PER_UTR frame */

        EHCI_WriteSitdInfo(utr, sitd);

        sitdNext = sitd->next; /* remember the next itd */

        // USB_debug("Link iTD 0x%x, %d\n", (int)itd, isoEp->nextFrame);
        /* Link iTD to period frame list */
        sitd->schedFrameIdx = isoEp->nextFrame; /* remember it for reclamation scan */
        DISABLE_EHCI_IRQ();
        EHCI_SitdAdjustSchedule(ehci, sitd);
        EHCI_AddSitdToIsoEp(isoEp, sitd); /* add to software itd list */
        sitd->nextLink = ehci->pfList[sitd->schedFrameIdx]; /* keep the next link */
        ehci->pfList[sitd->schedFrameIdx] = SITD_HLNK_SITD(sitd);
        isoEp->nextFrame = (isoEp->nextFrame + ep->bInterval) % FL_SIZE;
        ENABLE_EHCI_IRQ();

        sitd = sitdNext;
    }

    pReg->USBCMD |= EHCI_USBCMD_PSEN_MASK; /* periodic list enable */

    return HAL_OK;

error:
    while (sitdList != NULL) {
        sitd = sitdList;
        sitdList = sitd->next;
        HAL_USBH_FreePool(sitd);
    }

    return HAL_ERROR;
}

/**
 * @brief  Execute a iso transfer request.
 * @param  utr: the UTR transfer to be quit
 * @return HAL_Status
 */
HAL_Status HAL_EHCI_IsoXfer(struct UTR *utr)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)utr->pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct USB_EP_INFO *ep = utr->ep; /* Reference to isochronous endpoint */
    struct ECHI_ISO_EP *isoEp; /* Software iso endpoint descriptor */
    struct EHCI_ITD *itd, *itdNext, *itdList = NULL;
    int i, itdCnt;
    int transMask; /* Bit mask of used xfer in an iTD */
    int fidx; /* Index to the 8 iso frames of UTR */
    int interval; /* Frame interval of iTD */

    if (ep->hwPipe != NULL) {
        /* Get reference of the isochronous endpoint */
        isoEp = (struct ECHI_ISO_EP *)ep->hwPipe;

        if (utr->bIsoNewSched) {
            isoEp->nextFrame = (((pReg->FRINDEX + (EHCI_ISO_DELAY * 8)) &
                                 EHCI_FRINDEX_FI_MASK) >> 3) & 0x3FF;
        }
    } else {
        /* First time transfer of this iso endpoint */
        isoEp = (struct ECHI_ISO_EP *)HAL_USBH_AllocPool();
        if (isoEp == NULL) {
            return HAL_ERROR;
        }

        memset(isoEp, 0, sizeof(*isoEp));
        isoEp->ep = ep;
        isoEp->nextFrame = (((pReg->FRINDEX + (EHCI_ISO_DELAY * 8)) &
                             EHCI_FRINDEX_FI_MASK) >> 3) & 0x3FF;

        ep->hwPipe = isoEp;

        /* Add this isoEp into isoEpList */
        DISABLE_EHCI_IRQ();
        isoEp->next = ehci->isoEpList;
        ehci->isoEpList = isoEp;
        ENABLE_EHCI_IRQ();
    }

    if (utr->udev->speed == USB_SPEED_FULL) {
        return EHCI_IsoSplitXfer(utr, isoEp);
    }

    /* Allocate iTDs */
    if (ep->bInterval < 2) {         /* transfer interval is 1 micro-frame  */
        transMask = 0xFF;
        itdCnt = 1;                  /* required 1 iTD for one UTR          */
        interval = 1;                /* iTD frame interval of this endpoint */
    } else if (ep->bInterval < 4) {  /* transfer interval is 2 micro-frames */
        transMask = 0x55;
        itdCnt = 2;                  /* required 2 iTDs for one UTR         */
        interval = 1;                /* iTD frame interval of this endpoint */
    } else if (ep->bInterval < 8) {  /* transfer interval is 4 micro-frames */
        transMask = 0x44;
        itdCnt = 4;                  /* required 4 iTDs for one UTR         */
        interval = 1;                /* iTD frame interval of this endpoint */
    } else if (ep->bInterval < 16) { /* transfer interval is 8 micro-frames */
        transMask = 0x08;            /* there's 1 transfer in one iTD       */
        itdCnt = 8;                  /* required 8 iTDs for one UTR         */
        interval = 1;                /* iTD frame interval of this endpoint */
    } else if (ep->bInterval < 32) { /* transfer interval is 16 micro-frames */
        transMask = 0x10;            /* there's 1 transfer in one iTD       */
        itdCnt = 8;                  /* required 8 iTDs for one UTR         */
        interval = 2;                /* iTD frame interval of this endpoint */
    } else if (ep->bInterval < 64) { /* transfer interval is 32 micro-frames */
        transMask = 0x02;            /* there's 1 transfer in one iTD       */
        itdCnt = 8;                  /* required 8 iTDs for one UTR         */
        interval = 4;                /* iTD frame interval of this endpoint */
    } else {                         /* transfer interval is 64 micro-frame */
        transMask = 0x04;            /* there's 1 transfer in one iTD       */
        itdCnt = 8;                  /* required 8 iTDs for one UTR         */
        interval = 8;                /* iTD frame interval of this endpoint */
    }

    for (i = 0; i < itdCnt; i++) {   /* allocate all iTDs required by UTR   */
        itd = (struct EHCI_ITD *)HAL_USBH_AllocPool();
        if (itd == NULL) {
            goto error;
        }

        if (itdList == NULL) { /* link all iTDs */
            itdList = itd;
        } else {
            itd->next = itdList;
            itdList = itd;
        }
    }

    utr->tdCnt = itdCnt;

    /* Fill and link all iTDs */
    utr->isoSF = isoEp->nextFrame;
    fidx = 0; /* Index to UTR iso frmes (total IF_PER_UTR) */

    for (itd = itdList; (itd != NULL);) {
        if (fidx >= IF_PER_UTR) { /* unlikely */
            HAL_DBG_ERR("EHCI driver ITD bug!?\n");
            goto error;
        }

        itd->utr = utr;
        itd->fIdx = fidx; /* index to UTR's n'th IF_PER_UTR frame */
        /* iTD buffer base is buffer of the first UTR iso frame serviced by this iTD */
        itd->bufBase = (uint32_t)(utr->isoBuff[fidx]);
        itd->transMask = transMask;

        EHCI_WriteItdInfo(utr, itd);

        for (i = 0; i < 8; i++) { /* settle xfer into micro-frames */
            if (!(transMask & (0x1 << i))) {
                itd->transaction[i] = 0; /* not accesed */
                continue; /* not scheduled micro-frame */
            }

            EHCI_WriteItdMicroFrame(utr, fidx, itd, i);

            fidx++; /* preceed to next UTR iso frame */

            if (fidx == IF_PER_UTR) { /* is the last scheduled micro-frame? */
                /* raise interrupt on completed */
                itd->transaction[i] |= ITD_IOC;
                break;
            }
        }

        itdNext = itd->next; /* Remember the next itd */

        // USB_debug("Link iTD 0x%x, %d\n", (int)itd, isoEp->nextFrame);
        /* Link iTD to period frame list */
        DISABLE_EHCI_IRQ();
        itd->schedFrameIdx = isoEp->nextFrame; /* remember it for reclamation scan */
        EHCI_AddItdToIsoEp(isoEp, itd); /* add to software itd list */
        itd->nextLink = ehci->pfList[itd->schedFrameIdx];  /* keep the next link */
        ehci->pfList[itd->schedFrameIdx] = ITD_HLNK_ITD(itd);
        isoEp->nextFrame = (isoEp->nextFrame + interval) % FL_SIZE;
        ENABLE_EHCI_IRQ();

        itd = itdNext;
    }

    pReg->USBCMD |= EHCI_USBCMD_PSEN_MASK; /* periodic list enable */

    return HAL_OK;

error:

    while (itdList != NULL) {
        itd = itdList;
        itdList = itd->next;
        HAL_USBH_FreePool(itd);
    }

    return HAL_ERROR;
}

/**
 * @brief  If it's an isochronous endpoint, quit current transfer via UTR or hardware EP.
 * @param  utr: the UTR transfer to be quit
 * @param  ep: the endpoint to be quit
 * @return HAL_Status
 */
HAL_Status HAL_EHCI_QuitIsoXfer(struct UTR *utr, struct USB_EP_INFO *ep)
{
    struct EHCI_HCD *ehci;
    struct EHCI_REG *pReg;
    struct ECHI_ISO_EP *isoEp;
    struct EHCI_ITD *itd, *itdNext, *p;
    uint32_t frameIdx;
    uint32_t frameCurr;

    if (ep == NULL) {
        if (utr == NULL) {
            return HAL_INVAL;
        }

        if (utr->ep == NULL) {
            return HAL_INVAL;
        }

        ep = utr->ep;
    }

    if ((ep->bmAttributes & EP_ATTR_TT_MASK) != EP_ATTR_TT_ISO) {
        return HAL_INVAL; /* not isochronous endpoint */
    }

    /* It's an iso endpoint. Remove it as required. */
    ehci = (struct EHCI_HCD *)utr->pHCD;
    isoEp = ehci->isoEpList;
    while (isoEp != NULL) { /* Search all activated iso endpoints  */
        if (isoEp->ep == ep) {
            break;
        }
        isoEp = isoEp->next;
    }
    if (isoEp == NULL) {
        return 0;  /* should have been removed */
    }

    pReg = HCD_HANDLE_TO_REG(ehci);
    itd = isoEp->iTDList; /* get the first iTD from isoEp's iTD list */
    while (itd != NULL) { /* traverse all iTDs of itd list */
        itdNext = itd->next; /* remember the next iTD */
        utr = itd->utr;

        /* Remove this iTD from period frame list */
        frameIdx = itd->schedFrameIdx;

        /*
         * Prevent to race with Host Controller. If the iTD to be removed is
         * located in current or next frame, wait until HC passed through it.
         */
        while (1) {
            frameCurr = (pReg->FRINDEX >> 3) & 0x3FF;
            if ((frameCurr == frameIdx) || (((frameCurr + 1) % 1024) == frameIdx)) {
                continue;
            }
            break;
        }

        if (ehci->pfList[frameIdx] == ITD_HLNK_ITD(itd)) {
            /* is the first entry, just change to next */
            ehci->pfList[frameIdx] = itd->nextLink;
        } else {
            p = ITD_PTR(ehci->pfList[frameIdx]); /* find the preceding iTD */
            while ((ITD_PTR(p->nextLink) != itd) && (p != NULL)) {
                p = ITD_PTR(p->nextLink);
            }

            if (p == NULL) { /* link list out of control! */
                HAL_DBG_ERR("%s: An iTD lost reference to periodic frame list! 0x%lx on %ld\n", __func__, (uint32_t)itd, frameIdx);
            } else { /* Remove iTD from list */
                p->nextLink = itd->nextLink;
            }
        }

        utr->tdCnt--;

        if (utr->tdCnt == 0) { /* All iTD of this UTR done */
            utr->bIsTransferDone = 1;
            if (utr->CompleteCb) {
                utr->CompleteCb(utr);
            }
            utr->status = USBH_ERR_ABORT;
        }
        HAL_USBH_FreePool(itd);
        itd = itdNext;
    }

    /* Remove isoEp from isoEpList */
    EHCI_RemoveIsoEpFromList(ehci, isoEp);
    HAL_USBH_FreePool(isoEp); /* Free this isoEp */
    ep->hwPipe = NULL;

    if (ehci->isoEpList == NULL) {
        pReg->USBCMD &= ~EHCI_USBCMD_PSEN_MASK;
    }

    return HAL_OK;
}

/**
 * @brief  Scan several earlier frames and drop unserviced iTD/siTD if found.
 * @param  ehci: the EHCI HCD phandle
 * @return HAL_Status
 */
HAL_Status HAL_EHCI_ScanIsochronousList(struct EHCI_HCD *ehci)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct ECHI_ISO_EP *isoEp = ehci->isoEpList;
    struct EHCI_ITD *itd, *itdPre, *p;
    struct EHCI_SITD *sitd, *sitdPre, *sp;
    uint32_t frameIdx;

    DISABLE_EHCI_IRQ();

    while (isoEp != NULL) { /* Search all activated iso endpoints */
        /* Scan all iTDs */
        itd = isoEp->iTDList; /* Get the first iTD from isoEp's iTD list */
        itdPre = NULL;
        while (itd != NULL) { /* Traverse all iTDs of itd list */
            if (EHCI_ReviewItd(pReg, itd)) { /* Inspect and reclaim iTD */
                /* Remove this iTD from period frame list */
                frameIdx = itd->schedFrameIdx;
                if (ehci->pfList[frameIdx] == ITD_HLNK_ITD(itd)) {
                    /* Is the first entry, just change to next */
                    ehci->pfList[frameIdx] = itd->nextLink;
                } else {
                    p = ITD_PTR(ehci->pfList[frameIdx]); /* Find the preceding iTD */
                    while ((ITD_PTR(p->nextLink) != itd) && (p != NULL)) {
                        p = ITD_PTR(p->nextLink);
                    }

                    if (p == NULL) { /* Link list out of control */
                        HAL_DBG_ERR("An iTD lost refernece to periodic frame list! 0x%lx -> %ld\n", (uint32_t)itd, frameIdx);
                    } else { /* Remove iTD from list */
                        p->nextLink = itd->nextLink;
                    }
                }

                /* Remove this iTD from isoEp's iTD list */
                if (itdPre == NULL) {
                    isoEp->iTDList = itd->next;
                } else {
                    itdPre->next = itd->next;
                }
                p = itd->next;
                HAL_USBH_FreePool(itd);
                itd = p;
            } else {
                itdPre = itd;
                itd = itd->next; /* Traverse to the next iTD of iTD list */
            }
        }

        /* Scan all siTDs */
        sitd = isoEp->siTDdoneList; /* Get the first siTD from isoEp's siTD list */
        sitdPre = NULL;
        while (sitd != NULL) { /* Traverse all siTDs of sitd list */
            if (EHCI_ReviewSitd(pReg, sitd)) { /* inspect and reclaim siTD */
                /* Remove this siTD from period frame list */
                frameIdx = sitd->schedFrameIdx;
                if (ehci->pfList[frameIdx] == SITD_HLNK_SITD(sitd)) {
                    /* Is the first entry, just change to next */
                    ehci->pfList[frameIdx] = sitd->nextLink;
                } else {
                    sp = SITD_PTR(ehci->pfList[frameIdx]); /* Find the preceding siTD */
                    while ((SITD_PTR(sp->nextLink) != sitd) && (sp != NULL)) {
                        sp = SITD_PTR(sp->nextLink);
                    }

                    if (sp == NULL) { /* Link list out of control */
                        HAL_DBG_ERR("An siTD lost reference to periodic frame list! 0x%lx -> %ld\n", (uint32_t)sitd, frameIdx);
                    } else { /* remove iTD from list */
                        sp->nextLink = sitd->nextLink;
                    }
                }

                /* Remove this siTD from isoEp's siTD list */
                if (sitdPre == NULL) {
                    isoEp->siTDdoneList = sitd->next;
                } else {
                    sitdPre->next = sitd->next;
                }
                sp = sitd->next;
                HAL_USBH_FreePool(sitd);
                sitd = sp;
            } else {
                sitdPre = sitd;
                sitd = sitd->next; /* Traverse to the next siTD of siTD list */
            }
        }

        isoEp = isoEp->next;
    }

    ENABLE_EHCI_IRQ();

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_EHCI_MODULE_ENABLED */
