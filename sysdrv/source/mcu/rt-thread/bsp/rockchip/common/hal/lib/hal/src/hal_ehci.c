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

/** @defgroup EHCI_How_To_Use How To Use
 *  @{

 The EHCI HAL can be used as follows:

 - The core data structure EHCI_HCD control and manage the EHCI HAL lib,
     pReg is the HW Register and the base address passed from glue layer,
     the other members are allocated and managed by EHCI HAL.
 - The EHCI HAL operations include Init, Suspend, Resume and various Xfer APIs,
     all of them are registed in HAL_EHCI_OpsInit() which need to invoke in
     glue layer.
 - The isochronous transfer related APIs implement in file hal_ehci_iso.c

 @} */

/** @defgroup EHCI_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define EHCI_QH_INIT(qh)               \
{                                      \
    (qh)->qTDCurr = QTD_LIST_END;      \
    (qh)->qTDNextOL = QTD_LIST_END;    \
    (qh)->qTDAltNextOL = QTD_LIST_END; \
    (qh)->tokenOL = QTD_STS_HALT;      \
}

#define EHCI_QTD_INIT(qtd, u)         \
{                                     \
    (qtd)->qTDNext = QTD_LIST_END;    \
    (qtd)->qTDAltNext = QTD_LIST_END; \
    (qtd)->token = 0x1197B3F;         \
    (qtd)->utr = (u);                 \
}

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

static void EHCI_InitPeriodicFrameList(struct EHCI_HCD *ehci)
{
    struct EHCI_QH *qh;
    int i, idx, interval;
    uint32_t size;

    size = FL_SIZE * sizeof(uint32_t);
    ehci->pfList = HAL_USBH_AssignEhciPfList();
    memset(ehci->pfList, 0, size);

    ehci->isoEpList = NULL;

    for (i = NUM_IQH - 1; i >= 0; i--) {
        ehci->iQH[i] = (struct EHCI_QH *)HAL_USBH_AllocPool();
        ehci->iQH[i]->hLink = QH_HLNK_END;
        ehci->iQH[i]->qTDCurr = (uint32_t)ehci->qTDGhost;
        ehci->iQH[i]->qTDNextOL = QTD_LIST_END;
        ehci->iQH[i]->qTDAltNextOL = (uint32_t)ehci->qTDGhost;
        ehci->iQH[i]->tokenOL = QTD_STS_HALT;

        interval = 0x1 << i; /* interval = i^2 */

        for (idx = interval - 1; idx < FL_SIZE; idx += interval) {
            if (ehci->pfList[idx] == 0) {
                /* is empty list, insert directly */
                ehci->pfList[idx] = QH_HLNK_QH(ehci->iQH[i]);
            } else {
                qh = QH_PTR(ehci->pfList[idx]);
                while (1) {
                    if (qh == ehci->iQH[i]) {
                        /* already chained by previous visit */
                        break;
                    }

                    if (qh->hLink == QH_HLNK_END) {
                        /* reach end of list? */
                        qh->hLink = QH_HLNK_QH(ehci->iQH[i]);
                        break;
                    }
                    qh = QH_PTR(qh->hLink);
                }
            }
        }
    }
}

static struct EHCI_QH *EHCI_GetIntTreeHeadNode(struct EHCI_HCD *ehci,
                                               int interval)
{
    int i;

    interval /= 8; /* each frame list entry for 8 micro-frame */

    for (i = 0; i < NUM_IQH - 1; i++) {
        interval >>= 1;
        if (interval == 0) {
            return ehci->iQH[i];
        }
    }

    return ehci->iQH[NUM_IQH - 1];
}

static uint8_t EHCI_MakeIntrSchMask(int bInterval)
{
    int order, interval;

    interval = 1;
    while (bInterval > 1) {
        interval *= 2;
        bInterval--;
    }

    if (interval < 2) {
        return 0xFFU; /* interval 1 */
    }
    if (interval < 4) {
        return 0x55U; /* interval 2 */
    }
    if (interval < 8) {
        return 0x22U; /* interval 4 */
    }

    for (order = 0; (interval > 1); order++) {
        interval >>= 1U;
    }

    return (0x1U << (order % 8));
}

static HAL_Status EHCI_Reset(struct EHCI_REG *pReg)
{
    int timeout;

    /*
     * Make sure that the EHCI is halted for Software should not set [HCRESET]
     * to a one when the HCHalted bit in the USBSTS register is a zero.
     */
    timeout = 2 * 1000; /* 2ms */
    pReg->USBCMD = 0;
    while ((!(pReg->USBSTS & EHCI_USBSTS_HCHalted_MASK)) && (timeout > 0)) {
        HAL_DelayMs(1);
        timeout -= 1000;
    }
    if (!(pReg->USBSTS & EHCI_USBSTS_HCHalted_MASK)) {
        return HAL_ERROR;
    }

    /* Reset EHCI host controller */
    timeout = 250 * 1000; /* EHCI reset time-out 250 ms */
    pReg->USBCMD |= EHCI_USBCMD_HCRST_MASK;
    while ((pReg->USBCMD & EHCI_USBCMD_HCRST_MASK) && (timeout > 0)) {
        HAL_DelayMs(1);
        timeout -= 1000;
    }
    if (pReg->USBCMD & EHCI_USBCMD_HCRST_MASK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

static HAL_Status EHCI_Init(void *pHCD)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    int ret;
    uint32_t regVal = 0;

    /* Reset EHCI controller */
    ret = EHCI_Reset(pReg);
    if (ret) {
        HAL_DBG_ERR("EHCI reset error %d\n", ret);

        return ret;
    }

    /* Disable all interrupts and clear pending interrupts */
    pReg->USBINTR = 0;
    pReg->USBSTS = 0x3f;

    ehci->qTDGhost = (struct EHCI_QTD *)HAL_USBH_AllocPool();
    EHCI_QTD_INIT(ehci->qTDGhost, NULL);

    /*
     * QTD_STS_HALT, EHCI_VisitQtd() will not remove a qTD with this mark.
     * It represents a qhost qTD.
     */
    ehci->qTDGhost->token = 0x11197B3F;

    /* Initialize asynchronous list */
    ehci->qhRemoveList = NULL;

    /* Create the QH list head with H-bit 1 */
    ehci->hQH = (struct EHCI_QH *)HAL_USBH_AllocPool();

    /* circular link to itself, the only one QH */
    ehci->hQH->hLink = QH_HLNK_QH(ehci->hQH);
    /* it's the head of reclamation list */
    ehci->hQH->chrst = QH_RCLM_LIST_HEAD;
    ehci->hQH->qTDCurr = (uint32_t)ehci->qTDGhost;
    ehci->hQH->qTDNextOL = QTD_LIST_END;
    ehci->hQH->qTDAltNextOL = (uint32_t)ehci->qTDGhost;
    ehci->hQH->tokenOL = QTD_STS_HALT;

    /* Set the Current Asynchronous List Address */
    pReg->ASYNCLISTADDR = (uint32_t)ehci->hQH;

    /* Initialize periodic list */
    EHCI_InitPeriodicFrameList(ehci);

    /* Set the Periodic Frame List Base Address */
    pReg->PERIODICLISTBASE = (uint32_t)ehci->pfList;

    regVal |= UCMDR_INT_THR_CTRL;
    if (FL_SIZE == 256) {
        regVal |= (0x2 << EHCI_USBCMD_FLSZ_SHIFT);
    } else if (FL_SIZE == 512) {
        regVal |= (0x1 << EHCI_USBCMD_FLSZ_SHIFT);
    } else if (FL_SIZE == 1024) {
        regVal |= (0x0 << EHCI_USBCMD_FLSZ_SHIFT);
    } else {
        return HAL_ERROR; /* Invalid FL_SIZE setting */
    }

    pReg->USBCMD = regVal;

    /* Start to run */
    pReg->USBCMD |= EHCI_USBCMD_RUN_MASK;
    pReg->CONFIGFLAG = 0x1; /* Enable port routing to EHCI */
    pReg->PORTSC[0] = EHCI_PORTSC_PP_MASK; /* Enable port 1 port power */

    HAL_DelayMs(10);

    /* Interrupt enable */
    pReg->USBINTR = EHCI_USBINTR_USBIEN_MASK | EHCI_USBINTR_UERRIEN_MASK |
                    EHCI_USBINTR_HSERREN_MASK | EHCI_USBINTR_IAAEN_MASK;

    return HAL_OK;
}

static HAL_Status EHCI_Suspend(void *pHCD)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG((struct EHCI_HCD *)pHCD);

    if (pReg->PORTSC[0] & 0x1) {
        pReg->PORTSC[0] |= EHCI_PORTSC_SUSPEND_MASK;
    }

    return HAL_OK;
}

static HAL_Status EHCI_Resume(void *pHCD)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG((struct EHCI_HCD *)pHCD);

    if (pReg->PORTSC[0] & 0x1) {
        pReg->PORTSC[0] = (pReg->PORTSC[0] & ~EHCI_PORTSC_SUSPEND_MASK) |
                          EHCI_PORTSC_FPR_MASK;
    }

    return HAL_OK;
}

static HAL_Status EHCI_Shutdown(void *pHCD)
{
    return EHCI_Suspend(pHCD);
}

static void EHCI_MoveQhToRemoveList(struct EHCI_HCD *ehci, struct EHCI_QH *qh)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct EHCI_QH *q;

    // HAL_DBG("%s: 0x%lx (0x%lx)\n", __func__, (uint32_t)qh, qh->chrst);

    /* Check if this ED found in ed_remove_list */
    q = ehci->qhRemoveList;
    while (q) {
        if (q == qh) {
            /* This QH has already in qhRemoveList. */
            return;
        }
        q = q->next;
    }

    DISABLE_EHCI_IRQ();

    /* Search asynchronous frame list and remove qh if found in list. */
    q = ehci->hQH;
    while (QH_PTR(q->hLink) != ehci->hQH) {
        if (QH_PTR(q->hLink) == qh) {
            /* q's next QH is qh, found, remove qh from list */
            q->hLink = qh->hLink;
            qh->next = ehci->qhRemoveList; /* Add qh to qhRemoveList */
            ehci->qhRemoveList = qh;
            pReg->USBCMD |= EHCI_USBCMD_IAAD_MASK; /* Trigger IAA interrupt */
            ENABLE_EHCI_IRQ();

            return; /* done */
        }

        /* Advance to next QH in asynchronous list */
        q = QH_PTR(q->hLink);
    }

    /* Search periodic frame list and remove qh if found in list. */
    q = ehci->iQH[NUM_IQH - 1];
    while (q->hLink != QH_HLNK_END) {
        if (QH_PTR(q->hLink) == qh) {
            /* q's next QH is qh, found, remove qh from list */
            q->hLink = qh->hLink;
            qh->next = ehci->qhRemoveList; /* Add qh to qhRemoveList */
            ehci->qhRemoveList = qh;
            pReg->USBCMD |= EHCI_USBCMD_IAAD_MASK; /* Trigger IAA interrupt */

            ENABLE_EHCI_IRQ();

            return; /* done */
        }

        /* Advance to next QH in asynchronous list */
        q = QH_PTR(q->hLink);
    }

    ENABLE_EHCI_IRQ();
}

static void EHCI_AppendQtdToQhList(struct EHCI_QH *qh, struct EHCI_QTD *qtd)
{
    struct EHCI_QTD *q;

    if (qh->qTDList == NULL) {
        qh->qTDList = qtd;
    } else {
        q = qh->qTDList;
        while (q->next != NULL) {
            q = q->next;
        }
        q->next = qtd;
    }
}

static void EHCI_WriteQh(struct USB_DEV *udev, struct USB_EP_INFO *ep,
                         struct EHCI_QH *qh)
{
    uint32_t chrst, cap;
    uint8_t ep0MaxPacketSize; /* should be global? */

    /* Write QH DWord 1 - Endpoint Characteristics */
    if (ep == NULL) {
        /* Is control endpoint? */
        ep0MaxPacketSize = udev->speed == USB_SPEED_LOW ? 8U : 64U;
        chrst = QH_DTC | QH_NAK_RL | (ep0MaxPacketSize << 16);
        if (udev->speed != USB_SPEED_HIGH) {
            /* non-high-speed control endpoint */
            chrst |= QH_CTRL_EP_FLAG;
        }
    } else {
        /* Not a control endpoint */
        chrst = QH_NAK_RL | (ep->wMaxPacketSize << 16);
        chrst |= ((ep->bEndpointAddress & 0xfU) << 8); /* Endpoint Address */
    }

    if (udev->speed == USB_SPEED_LOW) {
        chrst |= QH_EPS_LOW;
    } else if (udev->speed == USB_SPEED_FULL) {
        chrst |= QH_EPS_FULL;
    } else {
        chrst |= QH_EPS_HIGH;
    }

    chrst |= udev->devNum;
    qh->chrst = chrst;

    /* Write QH DWord 2 - Endpoint Capabilities */
    if (udev->speed == USB_SPEED_HIGH) {
        cap = 1U << QH_MULT_SHIFT;
    }

    qh->cap = cap;
}

static void EHCI_WriteQtdBufPage(struct EHCI_QTD *qtd, uint32_t bufAddr,
                                 int xferLen)
{
    int i;

    qtd->xferLen = xferLen;
    qtd->bufPage[0] = bufAddr;

    bufAddr = (bufAddr + 0x1000) & ~0xFFF;

    for (i = 1; i < 5; i++) {
        qtd->bufPage[i] = bufAddr;
        bufAddr += 0x1000;
    }
}

static HAL_Status EHCI_CtrlXfer(struct UTR *utr)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)utr->pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct USB_DEV *udev = utr->udev;
    struct EHCI_QH *qh;
    struct EHCI_QTD *qtdSetup, *qtdData, *qtdStatus;
    uint32_t token;
    int isNewQh = 0;

    if (utr->dataLen > 0) {
        if (((uint32_t)utr->buff + utr->dataLen) >
            (((uint32_t)utr->buff & ~0xFFF) + 0x5000)) {
            return HAL_INVAL;
        }
    }

    /* Allocate and link QH  */
    if (udev->ep0.hwPipe != NULL) {
        qh = (struct EHCI_QH *)udev->ep0.hwPipe;
        if (qh->qTDList) {
            return HAL_BUSY;
        }
    } else {
        qh = (struct EHCI_QH *)HAL_USBH_AllocPool();
        if (qh == NULL) {
            return HAL_ERROR; /* out of memory */
        }

        EHCI_QH_INIT(qh);
        udev->ep0.hwPipe = (void *)qh; /* Driver can find QH from EP */
        isNewQh = 1;
    }

    EHCI_WriteQh(udev, NULL, qh);
    utr->ep = &udev->ep0; /* Driver can find EP from UTR */

    /* Allocate qTDs */
    qtdSetup = (struct EHCI_QTD *)HAL_USBH_AllocPool();
    EHCI_QTD_INIT(qtdSetup, utr);

    if (utr->dataLen > 0) {
        qtdData = (struct EHCI_QTD *)HAL_USBH_AllocPool();
        EHCI_QTD_INIT(qtdData, utr);
    } else {
        qtdData = NULL;
    }

    qtdStatus = (struct EHCI_QTD *)HAL_USBH_AllocPool();
    if (qtdStatus == NULL) {
        /* out of memory? */
        if (qtdSetup) {
            HAL_USBH_FreePool(qtdSetup);
        }
        if (qtdData) {
            HAL_USBH_FreePool(qtdData);
        }

        return HAL_ERROR; /* out of memory */
    }

    EHCI_QTD_INIT(qtdStatus, utr);
    HAL_DBG("qh=0x%lx, qtdSetup=0x%lx, qtdData=0x%lx, qtdStatus=0x%lx\n", (uint32_t)qh, (uint32_t)qtdSetup, (uint32_t)qtdData, (uint32_t)qtdStatus);

    /* Prepare SETUP stage qTD */
    qtdSetup->qh = qh;
    //qtdSetup->utr = utr;
    EHCI_WriteQtdBufPage(qtdSetup, (uint32_t)&utr->setup, 8);
    EHCI_AppendQtdToQhList(qh, qtdSetup);
    qtdSetup->token = (8 << 16) | QTD_ERR_COUNTER | QTD_PID_SETUP | QTD_STS_ACTIVE;

    /* Prepare DATA stage qTD */
    if (utr->dataLen > 0) {
        qtdSetup->qTDNext = (uint32_t)qtdData;
        qtdData->qTDNext = (uint32_t)qtdStatus;

        if (!(utr->setup.bmRequestType & 0x80)) {
            /* REQ_TYPE_OUT */
            token = QTD_ERR_COUNTER | QTD_PID_OUT | QTD_STS_ACTIVE;
        } else {
            token = QTD_ERR_COUNTER | QTD_PID_IN | QTD_STS_ACTIVE;
        }

        qtdData->qh = qh;
        //qtdData->utr = utr;
        EHCI_WriteQtdBufPage(qtdData, (uint32_t)utr->buff, utr->dataLen);
        EHCI_AppendQtdToQhList(qh, qtdData);
        qtdData->token = QTD_DT | (utr->dataLen << 16) | token;
    } else {
        qtdSetup->qTDNext = (uint32_t)qtdStatus;
    }

    /* Prepare USTSR stage qTD */
    qtdStatus->qTDNext = (uint32_t)ehci->qTDGhost;
    qtdStatus->qTDAltNext = QTD_LIST_END;

    if (!(utr->setup.bmRequestType & 0x80)) {
        /* REQ_TYPE_OUT */
        token = QTD_ERR_COUNTER | QTD_PID_IN | QTD_STS_ACTIVE;
    } else {
        token = QTD_ERR_COUNTER | QTD_PID_OUT | QTD_STS_ACTIVE;
    }

    qtdStatus->qh = qh;
    //qtdStatus->utr = utr;
    EHCI_AppendQtdToQhList(qh, qtdStatus);
    qtdStatus->token = QTD_DT | QTD_IOC | token;

    /* Update QH overlay */
    qh->qTDCurr = 0;
    qh->qTDNextOL = (uint32_t)qtdSetup;
    qh->qTDAltNextOL = QTD_LIST_END;
    qh->tokenOL = 0;

    /* Link QH and start asynchronous transfer */
    if (isNewQh) {
        qh->hLink = ehci->hQH->hLink;
        ehci->hQH->hLink = QH_HLNK_QH(qh);
    }

    /* Start asynchronous transfer */
    pReg->USBCMD |= EHCI_USBCMD_ASEN_MASK;

    return HAL_OK;
}

static HAL_Status EHCI_BulkXfer(struct UTR *utr)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)utr->pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep;
    struct EHCI_QH *qh;
    struct EHCI_QTD *qtd, *qtdPrev;
    uint32_t dataLen, xferLen;
    uint8_t *buff;
    uint32_t token;
    int isNewQh = 0;

    if (ep->hwPipe != NULL) {
        qh = (struct EHCI_QH *)ep->hwPipe;
        if (qh->qTDList) {
            return HAL_BUSY;
        }
    } else {
        qh = (struct EHCI_QH *)HAL_USBH_AllocPool();
        if (qh == NULL) {
            return HAL_INVAL;
        }
        EHCI_QH_INIT(qh);
        isNewQh = 1;
        EHCI_WriteQh(udev, ep, qh);
        ep->hwPipe = (void *)qh; /* Associate QH with endpoint */
    }

    /* Prepare qTDs */
    dataLen = utr->dataLen;
    buff = utr->buff;
    qtdPrev = NULL;

    while (dataLen > 0) {
        qtd = (struct EHCI_QTD *)HAL_USBH_AllocPool();
        if (qtd == NULL) {
            /* Failed to allocate a qTD */
            qtd = qh->qTDList;
            while (qtd != NULL) {
                qtdPrev = qtd;
                qtd = qtd->next;
                HAL_USBH_FreePool(qtdPrev);
            }

            if (isNewQh) {
                HAL_USBH_FreePool(qh);
                ep->hwPipe = NULL;
            }

            return HAL_INVAL;
        }
        EHCI_QTD_INIT(qtd, utr);

        if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_OUT) {
            token = QTD_ERR_COUNTER | QTD_PID_OUT | QTD_STS_ACTIVE;
        } else {
            token = QTD_ERR_COUNTER | QTD_PID_IN | QTD_STS_ACTIVE;
        }

        if (dataLen > 0x4000) {
            /* Force maximum x'fer length 16K per qTD */
            xferLen = 0x4000;
        } else {
            /* Remaining data length < 4K */
            xferLen = dataLen;
        }

        qtd->qh = qh;
        qtd->qTDNext = (uint32_t)ehci->qTDGhost;
        qtd->qTDAltNext = QTD_LIST_END; //(uint32_t)ehci->qTDGhost;
        EHCI_WriteQtdBufPage(qtd, (uint32_t)buff, xferLen);
        EHCI_AppendQtdToQhList(qh, qtd);
        qtd->token = (xferLen << 16) | token;

        buff += xferLen; /* Advanced buffer pointer */
        dataLen -= xferLen;
        if (dataLen == 0) {
            /* Is this the latest qTD? */
            qtd->token |= QTD_IOC; /* Ask to raise an interrupt on the last qTD */
            qtd->qTDNext = (uint32_t)ehci->qTDGhost; /* qTD list end */
        }

        if (qtdPrev != NULL) {
            qtdPrev->qTDNext = (uint32_t)qtd;
        }
        qtdPrev = qtd;
    }

    //HAL_DBG("utr=0x%lx, qh=0x%lx, qtd=0x%lx\n", (uint32_t)utr, (uint32_t)qh, (uint32_t)qh->qTDList);

    qtd = qh->qTDList;

//  qh->qTDCurr = 0; //(uint32_t)qtd;
    qh->qTDNextOL = (uint32_t)qtd;
//  qh->qTDAltNextOL = QTD_LIST_END;

    /* Link QH and start asynchronous transfer */
    if (isNewQh) {
        memcpy(&(qh->bufPageOL[0]), &(qtd->bufPage[0]), 20);
        qh->qTDCurr = (uint32_t)qtd;
        qh->tokenOL = 0; /* qtd->token; */

        if (utr->ep->bToggle) {
            qh->tokenOL |= QTD_DT;
        }

        qh->hLink = ehci->hQH->hLink;
        ehci->hQH->hLink = QH_HLNK_QH(qh);
    }

    /* Start asynchronous transfer */
    pReg->USBCMD |= EHCI_USBCMD_ASEN_MASK;

    return HAL_OK;
}

static HAL_Status EHCI_IntrXfer(struct UTR *utr)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)utr->pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep;
    struct EHCI_QH *qh, *iqh;
    struct EHCI_QTD *qtd, *qtdDummy;
    uint32_t token;

    qtdDummy = (struct EHCI_QTD *)HAL_USBH_AllocPool();
    if (qtdDummy == NULL) {
        return HAL_INVAL;
    }
    EHCI_QTD_INIT(qtdDummy, NULL);
    qtdDummy->token &= ~(QTD_STS_ACTIVE | QTD_STS_HALT);

    if (ep->hwPipe != NULL) {
        qh = (struct EHCI_QH *)ep->hwPipe;
    } else {
        qh = (struct EHCI_QH *)HAL_USBH_AllocPool();
        if (qh == NULL) {
            HAL_USBH_FreePool(qtdDummy);

            return HAL_INVAL;
        }
        EHCI_QH_INIT(qh);
        EHCI_WriteQh(udev, ep, qh);
        qh->chrst &= ~0xF0000000;

        if (udev->speed == USB_SPEED_HIGH) {
            qh->cap = (0x1 << QH_MULT_SHIFT) | (qh->cap & 0xff) |
                      EHCI_MakeIntrSchMask(ep->bInterval);
        } else {
            qh->cap = (0x1 << QH_MULT_SHIFT) |
                      (qh->cap & ~(QH_C_MASK_MASK | QH_S_MASK_MASK)) | 0x7802;
        }
        ep->hwPipe = (void *)qh; /* associate QH with endpoint */

        /* Allocate another dummy qTD */
        qtd = (struct EHCI_QTD *)HAL_USBH_AllocPool();
        if (qtd == NULL) {
            HAL_USBH_FreePool(qtdDummy);
            HAL_USBH_FreePool(qh);

            return HAL_ERROR;
        }
        EHCI_QTD_INIT(qtd, NULL);
        qtd->token &= ~(QTD_STS_ACTIVE | QTD_STS_HALT);

        qh->dummy = qtdDummy;
        qh->qTDNextOL = (uint32_t)qtdDummy;
        qh->tokenOL = 0; /* !Active & !Halted */

        /* link QH */
        if (udev->speed == USB_SPEED_HIGH) {
            /* Get head node of this interval */
            iqh = EHCI_GetIntTreeHeadNode(ehci, ep->bInterval);
        } else {
            iqh = EHCI_GetIntTreeHeadNode(ehci, ep->bInterval * 8);
        }
        qh->hLink = iqh->hLink; /* Add to list of the same interval */
        iqh->hLink = QH_HLNK_QH(qh);

        qtdDummy = qtd;
    }

    qtd = qh->dummy; /* Use the current dummy qTD */
    qtd->qTDNext = (uint32_t)qtdDummy;
    qtd->utr = utr;
    qh->dummy = qtdDummy; /* Give the new dummy qTD */

    /**Prepare qTD */
    if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_OUT) {
        token = QTD_ERR_COUNTER | QTD_PID_OUT;
    } else {
        token = QTD_ERR_COUNTER | QTD_PID_IN;
    }

    qtd->qh = qh;
    qtd->qTDAltNext = QTD_LIST_END;
    EHCI_WriteQtdBufPage(qtd, (uint32_t)utr->buff, utr->dataLen);
    EHCI_AppendQtdToQhList(qh, qtd);
    qtd->token = QTD_IOC | (utr->dataLen << 16) | token | QTD_STS_ACTIVE;

    // HAL_DBG("%s: qh 0x%lx, 0x%lx, 0x%lx\n", __func__, (uint32_t)qh, (uint32_t)qh->chrst, (uint32_t)qh->cap);

    pReg->USBCMD |= EHCI_USBCMD_PSEN_MASK; /* periodic list enable */

    return HAL_OK;
}

/* Quit current trasnfer via UTR or hardware EP */
static HAL_Status EHCI_QuitXfer(void *pHCD, struct UTR *utr,
                                struct USB_EP_INFO *ep)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)pHCD;
    struct EHCI_QH *qh;

    // HAL_DBG("%s: utr 0x%lx, ep 0x%lx\n", __func__, (uint32_t)utr, (uint32_t)ep);

    DISABLE_EHCI_IRQ();
    if (HAL_EHCI_QuitIsoXfer(utr, ep) == HAL_OK) {
        ENABLE_EHCI_IRQ();

        return HAL_OK;
    }
    ENABLE_EHCI_IRQ();

    if (utr != NULL) {
        if (utr->ep == NULL) {
            return HAL_NODEV;
        }

        qh = (struct EHCI_QH *)(utr->ep->hwPipe);
        if (!qh) {
            return HAL_NODEV;
        }

        /* Add the QH to remove list, it will be removed on the next IAAD interrupt */
        EHCI_MoveQhToRemoveList(ehci, qh);
        utr->ep->hwPipe = NULL;
    }

    if ((ep != NULL) && (ep->hwPipe != NULL)) {
        qh = (struct EHCI_QH *)(ep->hwPipe);
        /* Add the QH to remove list, it will be removed on the next IAAD interrupt */
        EHCI_MoveQhToRemoveList(ehci, qh);
        ep->hwPipe = NULL;
    }
    HAL_DelayMs(2);

    return HAL_OK;
}

static HAL_Check EHCI_VisitQtd(struct EHCI_QTD *qtd)
{
    if ((qtd->token == 0x11197B3F) || (qtd->token == 0x1197B3F)) {
        return HAL_FALSE; /* A Dummy qTD or qTD on writing, don't touch it. */
    }
    // HAL_DBG("Visit qtd 0x%lx - 0x%lx\n", (uint32_t)qtd, qtd->token);

    if ((qtd->token & QTD_STS_ACTIVE) == 0) {
        if (qtd->token & (QTD_STS_HALT | QTD_STS_DATA_BUFF_ERR |
                          QTD_STS_BABBLE | QTD_STS_XactErr | QTD_STS_MISS_MF)) {
            HAL_DBG_ERR("qTD 0x%lx error token 0x%lx, bufPage 0x%lx\n", (uint32_t)qtd, qtd->token, qtd->bufPage[0]);
            if (qtd->utr->status == 0) {
                qtd->utr->status = USBH_ERR_TRANSACTION;
            }
        } else {
            if ((qtd->token & QTD_PID_MASK) != QTD_PID_SETUP) {
                qtd->utr->xferLen += qtd->xferLen - QTD_TODO_LEN(qtd->token);
                // HAL_DBG("0x%lx  utr->xferLen += %d\n", qtd->token, qtd->xferLen - QTD_TODO_LEN(qtd->token));
            }
        }

        return HAL_TRUE;
    }

    return HAL_FALSE;
}

static void EHCI_ScanAsynchronousList(struct EHCI_HCD *ehci)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct EHCI_QH *qh, *qhTmp;
    struct EHCI_QTD *qtdPrev = NULL, *qtd, *qtdTmp;
    struct UTR *utr;

    qh = QH_PTR(ehci->hQH->hLink);
    while (qh != ehci->hQH) {
        HAL_DBG("Scan qh=0x%lx, 0x%lx\n", (uint32_t)qh, qh->tokenOL);

        utr = NULL;
        qtd = qh->qTDList;
        while (qtd != NULL) {
            if (EHCI_VisitQtd(qtd)) { /* if TRUE, reclaim this qtd */
                /* qTD is completed, will remove it */
                utr = qtd->utr;
                if (qtd == qh->qTDList) {
                    qh->qTDList = qtd->next; /* Unlink the qTD from qtd_list */
                } else {
                    qtdPrev->next = qtd->next; /* Unlink the qTD from qtd_list */
                }
                qtdTmp = qtd; /* Remember this qTD for freeing later */
                qtd = qtd->next; /* Advance to the next qTD*/

                qtdTmp->next = qh->doneList; /* Push this qTD to QH's done list */
                qh->doneList = qtdTmp;
            } else {
                qtdPrev = qtd; /* Remember this qTD as a preceder */
                qtd = qtd->next; /* Advance to next qTD */
            }
        }

        qhTmp = qh;
        qh = QH_PTR(qh->hLink); /* Advance to the next QH */

        /* If all TDs are done, call-back to requester and then remove this QH. */
        if ((qhTmp->qTDList == NULL) && utr) {
            // HAL_DBG("T %d [%d]\n", (qhTmp->Chrst>>8)&0xf, (qhTmp->OL_Token&QTD_DT) ? 1 : 0);
            if (qhTmp->tokenOL & QTD_DT) {
                utr->ep->bToggle = 1;
            } else {
                utr->ep->bToggle = 0;
            }

            utr->bIsTransferDone = 1;
            if (utr->CompleteCb) {
                utr->CompleteCb(utr);
            }

            /* Trigger IAA to reclaim done_list */
            pReg->USBCMD |= EHCI_USBCMD_IAAD_MASK;
        }
    }
}

static void EHCI_ScanPeriodicFrameList(struct EHCI_HCD *ehci)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    struct EHCI_QH *qh;
    struct EHCI_QTD *qtd, *qNext;
    struct UTR *utr;

    /* Scan interrupt frame list */
    qh = ehci->iQH[NUM_IQH - 1];
    while (qh != NULL) {
        qtd = qh->qTDList;
        if (qtd == NULL) {
            /* empty QH */
            qh = QH_PTR(qh->hLink); /* Advance to the next QH */
            continue;
        }

        while (qtd != NULL) {
            qNext = qtd->next;
            if (EHCI_VisitQtd(qtd)) { /* if TRUE, reclaim this qtd */
                qh->qTDList = qtd->next; /* Proceed to next qTD or NULL */
                qtd->next = qh->doneList; /* Push qTD into the done list */
                qh->doneList = qtd; /* Move qTD to done list */
            }
            qtd = qNext;
        }

        qtd = qh->doneList;

        while (qtd != NULL) {
            utr = qtd->utr;

            if (qh->tokenOL & QTD_DT) {
                utr->ep->bToggle = 1;
            } else {
                utr->ep->bToggle = 0;
            }

            utr->bIsTransferDone = 1;
            if (utr->CompleteCb) {
                utr->CompleteCb(utr);
            }

            pReg->USBCMD |= EHCI_USBCMD_IAAD_MASK; /* Trigger IAA to reclaim done_list */
            qtd = qtd->next;
        }

        qh = QH_PTR(qh->hLink); /* Advance to the next QH */
    }

    /* Scan isochronous frame list */
    HAL_EHCI_ScanIsochronousList(ehci);
}

static void EHCI_IaadRemoveQh(struct EHCI_HCD *ehci)
{
    struct EHCI_QH *qh;
    struct EHCI_QTD *qtd;
    struct UTR *utr;

    /* Remove all QHs in qhRemoveList... */
    while (ehci->qhRemoveList != NULL) {
        qh = ehci->qhRemoveList;
        ehci->qhRemoveList = qh->next;

        // HAL_DBG("%s: remove QH 0x%lx\n", __func__, (uint32_t)qh);

        while (qh->doneList) { /* We can free the qTDs now */
            qtd = qh->doneList;
            qh->doneList = qtd->next;
            HAL_USBH_FreePool(qtd);
        }

        if (qh->qTDList != NULL) { /* Still have incomplete qTDs? */
            utr = qh->qTDList->utr;
            while (qh->qTDList) {
                qtd = qh->qTDList;
                qh->qTDList = qtd->next;
                HAL_USBH_FreePool(qtd);
            }
            utr->status = USBH_ERR_ABORT;
            utr->bIsTransferDone = 1;
            if (utr->CompleteCb) {
                utr->CompleteCb(utr); /* call back */
            }
        }
        HAL_USBH_FreePool(qh); /* Free the QH */
    }

    /* Free all qTD in done_list of each asynchronous QH */
    qh = QH_PTR(ehci->hQH->hLink);
    while (qh != ehci->hQH) {
        while (qh->doneList) { /* We can free the qTDs now */
            qtd = qh->doneList;
            qh->doneList = qtd->next;
            HAL_USBH_FreePool(qtd);
        }
        qh = QH_PTR(qh->hLink); /* Advance to the next QH */
    }

    /* Free all qTD in done_list of each QH of periodic frame list */
    qh = ehci->iQH[NUM_IQH - 1];
    while (qh != NULL) {
        while (qh->doneList) { /* We can free the qTDs now */
            qtd = qh->doneList;
            qh->doneList = qtd->next;
            HAL_USBH_FreePool(qtd);
        }
        qh = QH_PTR(qh->hLink); /* Advance to the next QH */
    }
}

static HAL_Status EHCI_RhPortReset(void *pHCD, int port)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG((struct EHCI_HCD *)pHCD);
    int retry;
    int resetTime, debounceTime;

    resetTime = PORT_RESET_TIME_MS;

    for (retry = 0; retry < PORT_RESET_RETRY; retry++) {
        pReg->PORTSC[port] = (pReg->PORTSC[port] | EHCI_PORTSC_PRST_MASK) &
                             ~EHCI_PORTSC_PE_MASK;
        HAL_DelayMs(resetTime); /* Wait at least 50 ms */
        pReg->PORTSC[port] &= ~EHCI_PORTSC_PRST_MASK;
        debounceTime = resetTime * 1000;

        do {
            if (!(pReg->PORTSC[port] & EHCI_PORTSC_CCS_MASK) ||
                ((pReg->PORTSC[port] & (EHCI_PORTSC_CCS_MASK | EHCI_PORTSC_PE_MASK)) ==
                 (EHCI_PORTSC_CCS_MASK | EHCI_PORTSC_PE_MASK))) {
                goto done;
            }
            HAL_DelayUs(1);
            debounceTime--;
        } while (debounceTime > 0);

        resetTime += PORT_RESET_RETRY_INC_MS;
    }

    HAL_DBG("EHCI port %d - port reset failed!\n", port + 1);

    return HAL_ERROR;

done:
    /* Check again if device disconnected */
    if ((pReg->PORTSC[port] & EHCI_PORTSC_CCS_MASK) == 0) {
        pReg->PORTSC[port] |= EHCI_PORTSC_CSC_MASK; /* Clear CSC */

        return HAL_ERROR;
    }
    pReg->PORTSC[port] |= EHCI_PORTSC_PEC_MASK; /* Clear port enable change status  */
    pReg->USBSTS |= EHCI_USBSTS_PCD_MASK; /* Clear port change detect status */

    return HAL_OK; /* Port reset success */
}

static uint32_t EHCI_RtHubPortStatus(void *pHCD, int portNum)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG((struct EHCI_HCD *)pHCD);

    return (pReg->PORTSC[portNum] & EHCI_PORTSC_CSC_MASK);
}

static int EHCI_RtHubPolling(void *pHCD)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG((struct EHCI_HCD *)pHCD);
    uint32_t portStatus;
    int debounceTime;
    int change = 0;

    /* Connect status change */
    HAL_DBG("EHCI port1 status change: 0x%lx\n", pReg->PORTSC[0]);

    /* Port de-bounce */
    debounceTime = HUB_DEBOUNCE_TIME * 1000;
    portStatus = pReg->PORTSC[0] & EHCI_PORTSC_CCS_MASK;

    do {
        if (portStatus != (pReg->PORTSC[0] & EHCI_PORTSC_CCS_MASK)) {
            /* Reset stable time counting  */
            debounceTime = HUB_DEBOUNCE_TIME * 1000;
            portStatus = pReg->PORTSC[0] & EHCI_PORTSC_CCS_MASK;
        }
        HAL_DelayUs(1);
        debounceTime--;
    } while (debounceTime > 0);

    /* Clear connect status change bit */
    pReg->PORTSC[0] |= EHCI_PORTSC_CSC_MASK;
    if (portStatus == EHCI_PORTSC_CCS_MASK) {
        /* A new device connected. */
        if (EHCI_RhPortReset(pHCD, 0) != HAL_OK) {
            /* Port reset failed, maybe an USB 1.1 device */
            HAL_EHCI_RelinquishPort(pHCD, 0);
        } else {
            change = 1;
        }
    }

    return change;
}

/** @} */

/********************* Public Function Definition ****************************/
/** @defgroup EHCI_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  EHCI interrupt handler
 * @param  pHCD: phandle of the EHCI HCD
 * @return HAL_Status
 * EHCI interrupt handler is always wrapped by the driver.
 */
HAL_Status HAL_EHCI_IRQHandler(void *pHCD)
{
    struct EHCI_HCD *ehci = (struct EHCI_HCD *)pHCD;
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG(ehci);
    volatile uint32_t intrSts;

    intrSts = pReg->USBSTS;
    pReg->USBSTS = intrSts; /* Clear interrupt status */

    HAL_DBG("ehci intrSts = 0x%lx\n\n", intrSts);

    if (intrSts & EHCI_USBSTS_UERRINT_MASK) {
        HAL_DBG_ERR("Transfer error!\n");
    }

    if (intrSts & EHCI_USBSTS_USBINT_MASK) {
        /*
         * Some transfers completed, travel asynchronous
         * and periodic lists to find and reclaim them.
         */
        EHCI_ScanAsynchronousList(ehci);
        EHCI_ScanPeriodicFrameList(ehci);
    }

    if (intrSts & EHCI_USBSTS_IAA_MASK) {
        EHCI_IaadRemoveQh(ehci);
    }

    return HAL_OK;
}

/**
 * @brief  EHCI relinquish port owner
 * @param  pHCD: phandle of the EHCI HCD
 * @param  portNum: the port number will be visited
 * @return HAL_Status
 */
HAL_Status HAL_EHCI_RelinquishPort(void *pHCD, int portNum)
{
    struct EHCI_REG *pReg = HCD_HANDLE_TO_REG((struct EHCI_HCD *)pHCD);

    pReg->PORTSC[portNum] |= EHCI_PORTSC_PO_MASK;  /* Change port owner to OHCI */
    pReg->PORTSC[portNum] |= EHCI_PORTSC_CSC_MASK; /* Clear all status change bits */

    return HAL_OK;
}

/** @} */

/** @defgroup EHCI_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  EHCI operations register, invoked by glue layer.
 * @param  ops: phandle of the HCD OPS
 * @return HAL_Status
 */
HAL_Status HAL_EHCI_OpsInit(struct HCD_OPS *ops)
{
    ops->Init = EHCI_Init;
    ops->Shutdown = EHCI_Shutdown;
    ops->Suspend = EHCI_Suspend;
    ops->Resume = EHCI_Resume;
    ops->CtrlXfer = EHCI_CtrlXfer;
    ops->BulkXfer = EHCI_BulkXfer;
    ops->IntrXfer = EHCI_IntrXfer;
    ops->IsoXfer = HAL_EHCI_IsoXfer;
    ops->QuitXfer = EHCI_QuitXfer;
    ops->RthubPortReset = EHCI_RhPortReset;
    ops->RthubPolling = EHCI_RtHubPolling;
    ops->RthubPortStatus = EHCI_RtHubPortStatus;

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_EHCI_MODULE_ENABLED */
