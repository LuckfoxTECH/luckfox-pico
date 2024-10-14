/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_OHCI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup OHCI
 *  @{
 */

/** @defgroup OHCI_How_To_Use How To Use
 *  @{

 The OHCI HAL can be used as follows:

 - The core data structure OHCI_HCD control and manage the OHCI HAL lib,
     pReg is the HW Register and the base address passed from glue layer,
     the other members are allocated and managed by OHCI HAL.
 - The OHCI HAL operations include Init, Suspend, Resume and various Xfer APIs,
     all of them are registed in HAL_OHCI_OpsInit() which need to invoke in
     glue layer.

 @} */

/** @defgroup OHCI_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define OHCI_TD_INIT(td, u) \
{                           \
    (td)->utr = (u);        \
}

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

static void OHCI_MoveEdToRemovList(struct OHCI_HCD *ohci, struct OHCI_ED *ed)
{
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    struct OHCI_ED *p;

    HAL_DBG("%s: 0x%lx (0x%lx)\n", __func__, (uint32_t)ed, ed->info);
    DISABLE_OHCI_IRQ();

    /* Check if this ED found in edRemoveList */
    p = ohci->edRemoveList;
    while (p) {
        if (p == ed) {
            /* This ED found in edRemoveList, do nothing */
            ENABLE_OHCI_IRQ();

            return;
        }
        p = p->next;
    }

    ed->info |= ED_SKIP; /* Ask OHCI controller skip this ED */
    ed->next = ohci->edRemoveList;
    ohci->edRemoveList = ed;  /* Insert to the head of edRemoveList */
    ENABLE_OHCI_IRQ();
    pReg->INTRSTATUS = OHCI_INTRSTATUS_SF_MASK;
    pReg->INTRENABLE |= OHCI_INTRENABLE_SF_MASK;
    HAL_DelayMs(2); /* Full speed wait 2 ms is enough */
}

static void OHCI_InitHccaIntrTable(struct OHCI_HCD *ohci)
{
    struct OHCI_ED *ed;
    int i, idx, interval;

    memset(ohci->hcca->intrTable, 0, sizeof(ohci->hcca->intrTable));

    for (i = 5; i >= 0; i--) { /* interval = i^2 */
        ohci->iED[i] = (struct OHCI_ED *)HAL_USBH_AllocPool();
        ohci->iED[i]->info = ED_SKIP;

        interval = 0x1 << i;

        for (idx = interval - 1; idx < 32; idx += interval) {
            if (ohci->hcca->intrTable[idx] == 0) { /* is empty list, insert directly */
                ohci->hcca->intrTable[idx] = (uint32_t)ohci->iED[i];
            } else {
                ed = (struct OHCI_ED *)ohci->hcca->intrTable[idx];

                while (1) {
                    if (ed == ohci->iED[i]) {
                        break; /* already chained by previous visit */
                    }
                    if (ed->nextED == 0) { /* reach end of list? */
                        ed->nextED = (uint32_t)ohci->iED[i];
                        break;
                    }
                    ed = (struct OHCI_ED *)ed->nextED;
                }
            }
        }
    }
}

static struct OHCI_ED *OHCI_GetIntTreeHeadNode(struct OHCI_HCD *ohci,
                                               int interval)
{
    int i;

    for (i = 0; i < 5; i++) {
        interval >>= 1;
        if (interval == 0) {
            return ohci->iED[i];
        }
    }

    return ohci->iED[5]; /* for interval >= 32 */
}

static int OHCI_GetInterval(int interval)
{
    int i, bInterval = 1;

    for (i = 0; i < 5; i++) {
        interval >>= 1;
        if (interval == 0) {
            return bInterval;
        }
        bInterval *= 2;
    }

    return 32;  /* for interval >= 32 */
}

static HAL_Status OHCI_Reset(struct OHCI_REG *pReg)
{
    /* Disable HC interrupts  */
    pReg->INTRDISABLE = OHCI_INTRDISABLE_MIE_MASK;

    /* HC Reset requires max 10 ms delay  */
    pReg->CONTROL = 0;
    pReg->CMDSTATUS = OHCI_CMDSTATUS_HCR_MASK;

    HAL_DelayMs(10);

    /* Check if OHCI reset completed? */
    if ((pReg->CMDSTATUS & OHCI_CMDSTATUS_HCR_MASK) != 0) {
        return HAL_TIMEOUT;
    }

    pReg->RH_STATUS = OHCI_RH_STATUS_OCI_MASK | OHCI_RH_STATUS_LPS_MASK;
    pReg->CONTROL = HCFS_RESET;

    HAL_DelayMs(10);

    /* Check if OHCI reset completed? */
    if ((pReg->CMDSTATUS & OHCI_CMDSTATUS_HCR_MASK) != 0) {
        return HAL_TIMEOUT;
    }

    return HAL_OK;
}

static HAL_Status OHCI_Init(void *pHCD)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    uint32_t fmInterval;

    ohci->hcca = (struct OHCI_HCCA *)HAL_USBH_AssignOhciHcca();

    if (OHCI_Reset(pReg) < 0) {
        return HAL_ERROR;
    }

    ohci->edRemoveList = NULL;

    OHCI_InitHccaIntrTable(ohci);

    /*
     * Tell the controller where the control and bulk lists are
     * The lists are empty now.
     */
    pReg->ED_CONTROLHEAD = 0; /* control ED list head */
    pReg->ED_BULKHEAD = 0; /* bulk ED list head */

    pReg->HCCA = (uint32_t)ohci->hcca; /* HCCA area */

    /* periodic start 90% of frame interval */
    fmInterval = 0x2edf; /* 11,999 */
    pReg->PERIODICSTART = (fmInterval * 9) / 10;

    /* set FSLargestDataPacket, 10,104 for 0x2edf frame interval */
    fmInterval |= ((((fmInterval - 210) * 6) / 7) << 16);
    pReg->FMINTERVAL = fmInterval;

    pReg->LSTHRESH = 0x628;

    /* start controller operations */
    pReg->CONTROL = HCFS_OPER | (0x3 << OHCI_CONTROL_CBSR_MASK);

    pReg->RH_DESCRIPTORA = (pReg->RH_DESCRIPTORA | (1 << 9)) &
                           ~OHCI_RH_DESCRIPTORA_PSM_MASK;
    pReg->RH_STATUS = OHCI_RH_STATUS_LPSC_MASK;

    pReg->INTRENABLE = OHCI_INTRENABLE_MIE_MASK | OHCI_INTRENABLE_WDH_MASK |
                       OHCI_INTRENABLE_SF_MASK;

    /* POTPGT delay is bits 24-31, in 20 ms units. */
    HAL_DelayMs(20);

    return HAL_OK;
}

static HAL_Status OHCI_Suspend(void *pHCD)
{
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG((struct OHCI_HCD *)pHCD);

    /* set port suspend if connected */
    if (pReg->RH_PORTSTATUS[0] & 0x1) {
        pReg->RH_PORTSTATUS[0] = 0x4;
    }

    /* enable Device Remote Wakeup */
    pReg->RH_STATUS |= OHCI_RH_STATUS_DRWE_MASK;

    /* enable USBH RHSC interrupt for system wakeup */
    pReg->INTRENABLE |= OHCI_INTRENABLE_RHSC_MASK | OHCI_INTRENABLE_RD_MASK;

    /* set Host Controller enter suspend state */
    pReg->CONTROL = (pReg->CONTROL & ~OHCI_CONTROL_HCFS_MASK) |
                    (3 << OHCI_CONTROL_HCFS_SHIFT);

    return HAL_OK;
}

static HAL_Status OHCI_Resume(void *pHCD)
{
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG((struct OHCI_HCD *)pHCD);

    pReg->CONTROL = (pReg->CONTROL & ~OHCI_CONTROL_HCFS_MASK) |
                    (1U << OHCI_CONTROL_HCFS_SHIFT);
    pReg->CONTROL = (pReg->CONTROL & ~OHCI_CONTROL_HCFS_MASK) |
                    (2U << OHCI_CONTROL_HCFS_SHIFT);

    if (pReg->RH_PORTSTATUS[0] & 0x4U) {
        pReg->RH_PORTSTATUS[0] = 0x8U;
    }

    return HAL_OK;
}

static HAL_Status OHCI_Shutdown(void *pHCD)
{
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG((struct OHCI_HCD *)pHCD);

    OHCI_Suspend(pHCD);
    pReg->RH_STATUS = OHCI_RH_STATUS_LPS_MASK;

    return HAL_OK;
}

/*
 *  Quit current trasnfer via UTR or hardware EP.
 */
static HAL_Status OHCI_QuitXfer(void *pHCD, struct UTR *utr,
                                struct USB_EP_INFO *ep)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)pHCD;
    struct OHCI_ED *ed;

    if (utr != NULL) {
        if (utr->ep == NULL) {
            return HAL_INVAL;
        }

        ed = (struct OHCI_ED *)(utr->ep->hwPipe);
        if (!ed) {
            return HAL_INVAL;
        }

        /* add the endpoint to remove list, it will be removed on the next start of frame */
        OHCI_MoveEdToRemovList(ohci, ed);
        utr->ep->hwPipe = NULL;
    }

    if ((ep != NULL) && (ep->hwPipe != NULL)) {
        ed = (struct OHCI_ED *)(ep->hwPipe);
        /* add the endpoint to remove list, it will be removed on the next start of frame */
        OHCI_MoveEdToRemovList(ohci, ed);
        ep->hwPipe = NULL;
    }

    return HAL_OK;
}

static uint32_t OHCI_MakeEdInfo(struct USB_DEV *udev, struct USB_EP_INFO *ep)
{
    uint32_t info;
    uint8_t ep0MaxPacketSize;

    if (ep == NULL) { /* is a control endpoint */
        /* control endpoint direction is from TD  */
        ep0MaxPacketSize = udev->speed == USB_SPEED_LOW ? 8 : 64;
        info = (ep0MaxPacketSize << 16)         /* Control endpoint Maximum Packet Size from device descriptor */
               | ED_DIR_BY_TD                   /* Direction (Get direction From TD)      */
               | ED_FORMAT_GENERAL              /* General format                         */
               | (0 << ED_CTRL_EN_SHIFT);       /* Endpoint address 0                     */
    } else {                                    /* Other endpoint direction is from endpoint descriptor */
        info = (ep->wMaxPacketSize << 16);      /* Maximum Packet Size from endpoint      */

        info |= ((ep->bEndpointAddress & 0xf) << ED_CTRL_EN_SHIFT); /* Endpoint Number */

        if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_IN) {
            info |= ED_DIR_IN;
        } else {
            info |= ED_DIR_OUT;
        }

        if ((ep->bmAttributes & EP_ATTR_TT_MASK) == EP_ATTR_TT_ISO) {
            info |= ED_FORMAT_ISO;
        } else {
            info |= ED_FORMAT_GENERAL;
        }
    }

    info |= ((udev->speed == USB_SPEED_LOW)  ? ED_SPEED_LOW : ED_SPEED_FULL); /* Speed */
    info |= (udev->devNum); /* Function Address */

    return info;
}

static void OHCI_WriteTd(struct OHCI_TD *td, uint32_t info,
                         uint8_t *buff, uint32_t dataLen)
{
    td->info = info;
    td->cbp = (uint32_t)((!buff || !dataLen) ? 0 : buff);
    td->be = (uint32_t)((!buff || !dataLen) ? 0 : (uint32_t)buff + dataLen - 1);
    td->bufStart = td->cbp;
    // HAL_DBG("TD [0x%lx]: 0x%lx, 0x%lx, 0x%lx\n", (uint32_t)td, td->info, td->cbp, td->be);
}

static HAL_Status OHCI_CtrlXfer(struct UTR *utr)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)utr->pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    struct USB_DEV *udev;
    struct OHCI_ED *ed;
    struct OHCI_TD *tdSetup, *tdData, *tdStatus;
    uint32_t info;

    udev = utr->udev;

    /* Allocate ED and TDs */
    tdSetup = (struct OHCI_TD *)HAL_USBH_AllocPool();
    OHCI_TD_INIT(tdSetup, utr);

    if (utr->dataLen > 0) {
        tdData = (struct OHCI_TD *)HAL_USBH_AllocPool();
        OHCI_TD_INIT(tdData, utr);
    } else {
        tdData = NULL;
    }

    tdStatus = (struct OHCI_TD *)HAL_USBH_AllocPool();
    if (tdStatus == NULL) {
        HAL_USBH_FreePool(tdSetup);
        if (utr->dataLen > 0) {
            HAL_USBH_FreePool(tdData);
        }

        return HAL_ERROR;
    }

    OHCI_TD_INIT(tdStatus, utr);

    /* Check if there's any transfer pending on this endpoint... */
    if (udev->ep0.hwPipe == NULL) {
        ed = (struct OHCI_ED *)HAL_USBH_AllocPool();
        if (ed == NULL) {
            HAL_USBH_FreePool(tdSetup);
            HAL_USBH_FreePool(tdStatus);
            if (utr->dataLen > 0) {
                HAL_USBH_FreePool(tdData);
            }

            return HAL_ERROR;
        }
    } else {
        ed = (struct OHCI_ED *)udev->ep0.hwPipe;
    }

    /* prepare SETUP stage TD */
    info = TD_CC | TD_T_DATA0 | TD_TYPE_CTRL;
    OHCI_WriteTd(tdSetup, info, (uint8_t *)&utr->setup, 8);
    tdSetup->ed = ed;

    /* prepare DATA stage TD */
    if (utr->dataLen > 0) {
        if (!(utr->setup.bmRequestType & 0x80)) { /* REQ_TYPE_OUT */
            info = (TD_CC | TD_R | TD_DP_OUT | TD_T_DATA1 |
                    TD_TYPE_CTRL | TD_CTRL_DATA);
        } else {
            info = (TD_CC | TD_R | TD_DP_IN | TD_T_DATA1 |
                    TD_TYPE_CTRL | TD_CTRL_DATA);
        }

        OHCI_WriteTd(tdData, info, utr->buff, utr->dataLen);
        tdData->ed = ed;
        tdSetup->nextTD = (uint32_t)tdData;
        tdSetup->next = tdData;
        tdData->nextTD = (uint32_t)tdStatus;
        tdData->next = tdStatus;
    } else {
        tdSetup->nextTD = (uint32_t)tdStatus;
        tdSetup->next = tdStatus;
    }

    /* prepare STATUS stage TD */
    ed->info = OHCI_MakeEdInfo(udev, NULL);
    if (!(utr->setup.bmRequestType & 0x80)) { /* REQ_TYPE_OUT */
        info = (TD_CC | TD_DP_IN | TD_T_DATA1 | TD_TYPE_CTRL);
    } else {
        info = (TD_CC | TD_DP_OUT | TD_T_DATA1 | TD_TYPE_CTRL);
    }

    OHCI_WriteTd(tdStatus, info, NULL, 0);
    tdStatus->ed = ed;
    tdStatus->nextTD = 0;
    tdStatus->next = 0;

    /* prepare ED */
    ed->tailP = 0;
    ed->headP = (uint32_t)tdSetup;
    ed->info = OHCI_MakeEdInfo(udev, NULL);
    ed->nextED = 0;

    //HAL_DBG("TD SETUP [0x%lx]: 0x%lx, 0x%lx, 0x%lx, 0x%lx\n", (uint32_t)tdSetup, tdSetup->Info, tdSetup->CBP, tdSetup->BE, tdSetup->NextTD);
    //if (tdData)
    //    HAL_DBG("TD DATA  [0x%lx]: 0x%lx, 0x%lx, 0x%lx, 0x%lx\n", (uint32_t)tdData, tdData->Info, tdData->CBP, tdData->BE, tdData->NextTD);
    //HAL_DBG("TD STATUS [0x%lx]: 0x%lx, 0x%lx, 0x%lx, 0x%lx\n", (uint32_t)tdStatus, tdStatus->Info, tdStatus->CBP, tdStatus->BE, tdStatus->NextTD);
    HAL_DBG("Xfer ED 0x%lx: 0x%lx 0x%lx 0x%lx 0x%lx\n", (uint32_t)ed, ed->info, ed->tailP, ed->headP, ed->nextED);

    if (utr->dataLen > 0) {
        utr->tdCnt = 3;
    } else {
        utr->tdCnt = 2;
    }

    utr->ep = &udev->ep0; /* driver can find EP from UTR */
    udev->ep0.hwPipe = (void *)ed; /* driver can find ED from EP */

    /* Start transfer */
    DISABLE_OHCI_IRQ();
    pReg->ED_CONTROLHEAD = (uint32_t)ed; /* Link ED to OHCI */
    pReg->CONTROL |= OHCI_CONTROL_CLE_MASK; /* enable control list */
    ENABLE_OHCI_IRQ();
    pReg->CMDSTATUS = OHCI_CMDSTATUS_CLF_MASK; /* start Control list */

    return HAL_OK;
}

static HAL_Status OHCI_BulkXfer(struct UTR *utr)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)utr->pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep;
    struct OHCI_ED *ed;
    struct OHCI_TD *td, *tdPrev, *tdList = NULL;
    uint32_t info;
    uint32_t dataLen, xferLen;
    int8_t bIsNewED = 0;
    uint8_t *buff;

    info = OHCI_MakeEdInfo(udev, ep);

    /* Check if there's any transfer pending on this endpoint... */
    ed = (struct OHCI_ED *)pReg->ED_BULKHEAD; /* get the head of bulk endpoint list */
    while (ed != NULL) {
        if (ed->info == info) { /* have transfer of this EP not completed? */
            if ((ed->headP & 0xFFFFFFF0) != (ed->tailP & 0xFFFFFFF0)) {
                return HAL_BUSY; /* endpoint is busy */
            } else {
                break; /* ED already there... */
            }
        }
        ed = (struct OHCI_ED *)ed->nextED;
    }

    if (ed == NULL) {
        bIsNewED = 1;
        ed = (struct OHCI_ED *)HAL_USBH_AllocPool();
        if (ed == NULL) {
            return HAL_ERROR;
        }
        ed->info = info;
        ed->headP = 0;
        HAL_DBG("Link BULK ED 0x%lx: 0x%lx 0x%lx 0x%lx 0x%lx\n", (uint32_t)ed, ed->info, ed->tailP, ed->headP, ed->nextED);
    }

    ep->hwPipe = (void *)ed;

    /* Prepare TDs */
    utr->tdCnt = 0;
    dataLen = utr->dataLen;
    buff = utr->buff;

    do {
        if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_OUT) {
            info = (TD_CC | TD_R | TD_DP_OUT | TD_TYPE_BULK);
        } else {
            info = (TD_CC | TD_R | TD_DP_IN | TD_TYPE_BULK);
        }

        info &= ~(1 << 25); /* Data toggle from ED toggleCarry bit */

        if (dataLen > 4096) { /* maximum transfer length is 4K for each TD */
            xferLen = 4096;
        } else {
            xferLen = dataLen; /* remaining data length < 4K */
        }
        td = (struct OHCI_TD *)HAL_USBH_AllocPool();  /* allocate a TD */
        if (td == NULL) {
            goto Error;
        }
        /* fill this TD */
        OHCI_TD_INIT(td, utr);
        OHCI_WriteTd(td, info, buff, xferLen);
        td->ed = ed;

        utr->tdCnt++; /* increase TD count, for recalim counter */
        buff += xferLen; /* advanced buffer pointer */
        dataLen -= xferLen;

        /* chain to end of TD list */
        if (tdList == NULL) {
            tdList = td;
        } else {
            tdPrev = tdList;
            while (tdPrev->nextTD != 0) {
                tdPrev = (struct OHCI_TD *)tdPrev->nextTD;
            }
            tdPrev->nextTD = (uint32_t)td;
        }
    } while (dataLen > 0);

    /* Start transfer */
    utr->status = 0;
    DISABLE_OHCI_IRQ();
    ed->headP = (ed->headP & 0x2) | (uint32_t)tdList; /* keep toggleCarry bit */
    if (bIsNewED) {
        ed->headP = (uint32_t)tdList;
        /* Link ED to OHCI Bulk List */
        ed->nextED = pReg->ED_BULKHEAD;
        pReg->ED_BULKHEAD = (uint32_t)ed;
    }
    ENABLE_OHCI_IRQ();
    pReg->CONTROL |= OHCI_CONTROL_BLE_MASK; /* enable bulk list */
    pReg->CMDSTATUS = OHCI_CMDSTATUS_BLF_MASK; /* start bulk list */

    return HAL_OK;

Error:
    while (tdList != NULL) {
        td = tdList;
        tdList = (struct OHCI_TD *)tdList->nextTD;
        HAL_USBH_FreePool(td);
    }
    HAL_USBH_FreePool((void *)ed);

    return HAL_ERROR;
}

static HAL_Status OHCI_IntrXfer(struct UTR *utr)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)utr->pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep;
    struct OHCI_ED *ed, *ied;
    struct OHCI_TD *td, *tdNew;
    uint32_t info;
    int8_t bIsNewED = 0;

    if (utr->dataLen > 64) { /* USB 1.1 interrupt transfer maximum packet size is 64 */
        return HAL_INVAL;
    }

    tdNew = (struct OHCI_TD *)HAL_USBH_AllocPool();
    if (tdNew == NULL) {
        return HAL_ERROR;
    }

    OHCI_TD_INIT(tdNew, utr);
    ied = OHCI_GetIntTreeHeadNode(ohci, ep->bInterval); /* get head node of this interval */

    /* Find if this ED was already in the list */
    info = OHCI_MakeEdInfo(udev, ep);
    ed = ied;
    while (ed != NULL) {
        if (ed->info == info) {
            break;
        }
        ed = (struct OHCI_ED *)ed->nextED;
    }

    if (ed == NULL) { /* ED not found, create it */
        bIsNewED = 1;
        ed = (struct OHCI_ED *)HAL_USBH_AllocPool();
        if (ed == NULL) {
            return HAL_ERROR;
        }
        ed->info = info;
        ed->headP = 0;
        ed->bInterval = ep->bInterval;

        td = (struct OHCI_TD *)HAL_USBH_AllocPool();  /* allocate the initial dummy TD for ED */
        if (td == NULL) {
            HAL_USBH_FreePool((void *)ed);
            HAL_USBH_FreePool(tdNew);

            return HAL_ERROR;
        }
        td->utr = NULL;
        ed->headP = (uint32_t)td; /* Let both HeadP and TailP point to dummy TD */
        ed->tailP = ed->headP;
    } else {
        td = (struct OHCI_TD *)(ed->tailP & ~0xf); /* TailP always point to the dummy TD */
    }
    ep->hwPipe = (void *)ed;

    /* Prepare TD */
    if ((ep->bEndpointAddress & EP_ADDR_DIR_MASK) == EP_ADDR_DIR_OUT) {
        info = (TD_CC | TD_R | TD_DP_OUT | TD_TYPE_INT);
    } else {
        info = (TD_CC | TD_R | TD_DP_IN | TD_TYPE_INT);
    }

    /* Keep data toggle */
    info = (info & ~(1 << 25)) | (td->info & (1 << 25));

    /* fill this TD */
    OHCI_WriteTd(td, info, utr->buff, utr->dataLen);
    td->ed = ed;
    td->nextTD = (uint32_t)tdNew;
    td->utr = utr;
    utr->tdCnt = 1; /* increase TD count, for recalim counter */
    utr->status = 0;

    /* Hook ED and TD list to HCCA interrupt table */
    DISABLE_OHCI_IRQ();

    ed->tailP = (uint32_t)tdNew;
    if (bIsNewED) {
        /* Add to list of the same interval */
        ed->nextED = ied->nextED;
        ied->nextED = (uint32_t)ed;
    }

    ENABLE_OHCI_IRQ();

    //HAL_DBG("Link INT ED 0x%x: 0x%x 0x%x 0x%x 0x%x\n", (int)ed, ed->info, ed->tailP, ed->headP, ed->nextED);

    pReg->CONTROL |= OHCI_CONTROL_PLE_MASK; /* periodic list enable */

    return HAL_OK;
}

static HAL_Status OHCI_IsoXfer(struct UTR *utr)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)utr->pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    struct USB_DEV *udev = utr->udev;
    struct USB_EP_INFO *ep = utr->ep;
    struct OHCI_ED *ed, *ied;
    struct OHCI_TD *td, *tdList, *lastTd;
    int i;
    uint32_t info;
    uint32_t bufAddr;
    int8_t bIsNewED = 0;

    ied = OHCI_GetIntTreeHeadNode(ohci, ep->bInterval);  /* get head node of this interval */

    /* Find if this ED was already in the list */
    info = OHCI_MakeEdInfo(udev, ep);
    ed = ied;
    while (ed != NULL) {
        if (ed->info == info) {
            break;
        }
        ed = (struct OHCI_ED *)ed->nextED;
    }

    if (ed == NULL) { /* ED not found, create it */
        bIsNewED = 1;
        ed = (struct OHCI_ED *)HAL_USBH_AllocPool(); /* allocate an Endpoint Descriptor */
        if (ed == NULL) {
            return USBH_ERR_MEMORY_OUT;
        }
        ed->info = info;
        ed->headP = 0;
        ed->bInterval = ep->bInterval;
    } else {
        ep->hwPipe = (void *)ed;
    }

    /* Prepare TDs */
    if (utr->bIsoNewSched) { /* Is the starting of isochronous streaming? */
        ed->nextSF = ohci->hcca->frameNum + OHCI_ISO_DELAY;
    }

    utr->tdCnt = 0;
    utr->isoSF = ed->nextSF;

    lastTd = NULL;
    tdList = NULL;

    for (i = 0; i < IF_PER_UTR; i++) {
        utr->isoStatus[i] = USBH_ERR_NOT_ACCESS1;

        td = (struct OHCI_TD *)HAL_USBH_AllocPool();
        if (td == NULL) {
            goto Error;
        }
        /* fill this TD*/
        OHCI_TD_INIT(td, utr);
        bufAddr = (uint32_t)(utr->isoBuff[i]);
        td->info = (TD_CC | TD_TYPE_ISO) | ed->nextSF;
        ed->nextSF += OHCI_GetInterval(ed->bInterval);
        td->cbp = bufAddr & ~0xFFF;
        td->be = bufAddr + utr->isoXlen[i] - 1;
        td->psw[0] = 0xE000 | (bufAddr & 0xFFF);

        td->ed = ed;
        utr->tdCnt++; /* increase TD count, for reclaim counter */

        /* chain to end of TD list */
        if (tdList == NULL) {
            tdList = td;
        } else {
            lastTd->nextTD = (uint32_t)td;
        }

        lastTd = td;
    }

    /* Hook ED and TD list to HCCA interrupt table */
    utr->status = 0;
    DISABLE_OHCI_IRQ();

    if ((ed->headP & ~0x3) == 0) {
        ed->headP = (ed->headP & 0x2) | (uint32_t)tdList; /* keep toggleCarry bit */
    } else {
        /* find the tail of TDs under this ED */
        td = (struct OHCI_TD *)(ed->headP & ~0x3);
        while (td->nextTD != 0) {
            td = (struct OHCI_TD *)td->nextTD;
        }
        td->nextTD = (uint32_t)tdList;
    }

    if (bIsNewED) {
        /* Add to list of the same interval */
        ed->nextED = ied->nextED;
        ied->nextED = (uint32_t)ed;
    }

    ENABLE_OHCI_IRQ();
    HAL_DBG("Link ISO ED 0x%lx: 0x%lx 0x%lx 0x%lx 0x%lx\n", (uint32_t)ed, ed->info, ed->tailP, ed->headP, ed->nextED);
    /* enable periodic list and isochronous transfer */
    pReg->CONTROL |= OHCI_CONTROL_PLE_MASK | OHCI_CONTROL_IE_MASK;

    return HAL_OK;

Error:
    while (tdList != NULL) {
        td = tdList;
        tdList = (struct OHCI_TD *)tdList->nextTD;
        HAL_USBH_FreePool(td);
    }
    HAL_USBH_FreePool((void *)ed);

    return HAL_ERROR;
}

static HAL_Status OHCI_RhPortReset(void *pHCD, int port)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    int retry;
    int resetTime, debounce;

    resetTime = PORT_RESET_TIME_MS;

    for (retry = 0; retry < PORT_RESET_RETRY; retry++) {
        pReg->RH_PORTSTATUS[port] = OHCI_RH_PORTSTATUS_PRS_MASK;
        debounce = resetTime * 1000;

        do {
            /* If device is disconnected or port enabled, we can stop port reset. */
            if (((pReg->RH_PORTSTATUS[port] & OHCI_RH_PORTSTATUS_CCS_MASK) == 0) ||
                ((pReg->RH_PORTSTATUS[port] &
                  (OHCI_RH_PORTSTATUS_PES_MASK | OHCI_RH_PORTSTATUS_CCS_MASK)) ==
                 (OHCI_RH_PORTSTATUS_PES_MASK | OHCI_RH_PORTSTATUS_CCS_MASK))) {
                goto Done;
            }
            HAL_DelayUs(1);
            debounce--;
        } while (debounce > 0);

        resetTime += PORT_RESET_RETRY_INC_MS;
    }

    HAL_DBG("OHCI port %d, reset failed!\n", port + 1);

    return HAL_ERROR;

Done:
    if ((pReg->RH_PORTSTATUS[port] & OHCI_RH_PORTSTATUS_CCS_MASK) == 0) { /* check again if device disconnected */
        pReg->RH_PORTSTATUS[port] = OHCI_RH_PORTSTATUS_CSC_MASK; /* clear CSC */

        return HAL_ERROR;
    }

    return HAL_OK; /* port reset success */
}

static uint32_t OHCI_RtHubPortStatus(void *pHCD, int portNum)
{
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG((struct OHCI_HCD *)pHCD);

    return (pReg->RH_PORTSTATUS[portNum] & OHCI_RH_PORTSTATUS_CSC_MASK);
}

static int OHCI_RtHubPolling(void *pHCD)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    int change = 0;

    /* clear unwanted port change status */
    pReg->RH_PORTSTATUS[0] = OHCI_RH_PORTSTATUS_OCIC_MASK |
                             OHCI_RH_PORTSTATUS_PRSC_MASK |
                             OHCI_RH_PORTSTATUS_PSSC_MASK |
                             OHCI_RH_PORTSTATUS_PESC_MASK;

    if ((pReg->RH_PORTSTATUS[0] & OHCI_RH_PORTSTATUS_CSC_MASK) == 0) {
        goto Out;
    }

    HAL_DBG("OHCI port 1 status change: 0x%lx\n", pReg->RH_PORTSTATUS[0]);

    /* connect status change */
    pReg->RH_PORTSTATUS[0] = OHCI_RH_PORTSTATUS_CSC_MASK; /* clear CSC */

    if (pReg->RH_PORTSTATUS[0] & OHCI_RH_PORTSTATUS_CCS_MASK) {
        if (OHCI_RhPortReset(pHCD, 0) != HAL_OK) {
            goto Out;
        }

        /* Port reset success... */
        change = 1;
    }

Out:

    return change;
}

static void OHCI_TdDone(struct OHCI_TD *td)
{
    struct UTR *utr = td->utr;
    uint32_t info;
    int cc;

    info = td->info;

    HAL_DBG("OHCI TD Done: 0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx\n", (uint32_t)td, td->info, td->cbp, td->nextTD, td->be);

    /* ISO ... drivers see per-TD length/status */
    if ((info & TD_TYPE_MASK) == TD_TYPE_ISO) {
        uint16_t sf;
        int idx;

        sf = info & 0xFFFF;
        idx = ((sf + 0x10000 - utr->isoSF) & 0xFFFF) / OHCI_GetInterval(td->ed->bInterval);
        if (idx >= IF_PER_UTR) {
            HAL_DBG_ERR("ISO invalid index!! %d, %d\n", sf, utr->isoSF);
            goto Out;
        }

        cc = (td->psw[0] >> 12) & 0xF;
        if (cc == 0xF) { /* this frame was not transferred */
            HAL_DBG("ISO F %d N/A!\n", sf);
            utr->isoStatus[idx] = USBH_ERR_SCH_OVERRUN;
            goto Out;
        }
        if ((cc != 0) && (cc != CC_DATA_UNDERRUN)) {
            utr->isoStatus[idx] = USBH_ERR_CC_NO_ERR - cc;
            goto Out;
        }
        utr->isoStatus[idx] = 0;
        utr->isoXlen[idx] = td->psw[0] & 0x7FF;
    } else {
        cc = TD_CC_GET(info);

        /* short packet is fine */
        if ((cc != CC_NOERROR) && (cc != CC_DATA_UNDERRUN)) {
            HAL_DBG_ERR("TD error, CC = 0x%x\n", cc);
            if (cc == CC_STALL) {
                utr->status = USBH_ERR_STALL;
            } else {
                utr->status = USBH_ERR_TRANSFER;
            }
        }

        switch (info & TD_TYPE_MASK) {
        case TD_TYPE_CTRL:
            if (info & TD_CTRL_DATA) {
                if (td->cbp == 0) {
                    utr->xferLen += td->be - td->bufStart + 1;
                } else {
                    utr->xferLen += td->cbp - td->bufStart;
                }
            }
            break;

        case TD_TYPE_BULK:
        case TD_TYPE_INT:
            if (td->cbp == 0) {
                utr->xferLen += td->be - td->bufStart + 1;
            } else {
                utr->xferLen += td->cbp - td->bufStart;
            }
            break;
        }
    }

Out:
    utr->tdCnt--;

    /* If all TDs are done, call-back to requester. */
    if (utr->tdCnt == 0) {
        utr->bIsTransferDone = 1;
        if (utr->CompleteCb) {
            utr->CompleteCb(utr);
        }
    }
}

/* in IRQ context */
static void OHCI_RemoveEd(struct OHCI_HCD *ohci)
{
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);
    struct OHCI_ED *ed, *edPre, *ied;
    struct OHCI_TD *td, *tdNext;
    struct UTR *utr;
    int found;

    while (ohci->edRemoveList != NULL) {
        HAL_DBG("Remove ED: 0x%lx, %d\n", (uint32_t)ohci->edRemoveList, ohci->edRemoveList->bInterval);
        edPre = ohci->edRemoveList;
        found = 0;

        /* Remove endpoint from Control List if found */
        if ((edPre->info & ED_EP_ADDR_MASK) == 0) {
            if (pReg->ED_CONTROLHEAD == (uint32_t)edPre) {
                pReg->ED_CONTROLHEAD = (uint32_t)edPre->nextED;
                found = 1;
            } else {
                ed = (struct OHCI_ED *)pReg->ED_CONTROLHEAD;
                while (ed != NULL) {
                    if (ed->nextED == (uint32_t)edPre) {
                        ed->nextED = edPre->nextED;
                        found = 1;
                    }
                    ed = (struct OHCI_ED *)ed->nextED;
                }
            }
        }
        /* Remove INT or ISO endpoint from HCCA interrupt table */
        else if (edPre->bInterval > 0) {
            ied = OHCI_GetIntTreeHeadNode(ohci, edPre->bInterval);

            ed = ied;
            while (ed != NULL) {
                if (ed->nextED == (uint32_t)edPre) {
                    ed->nextED = edPre->nextED;
                    found = 1;
                    break;
                }
                ed = (struct OHCI_ED *)ed->nextED;
            }
        }
        /* Remove endpoint from Bulk List if found */
        else {
            if (pReg->ED_BULKHEAD == (uint32_t)edPre) {
                ed = (struct OHCI_ED *)edPre;
                pReg->ED_BULKHEAD = edPre->nextED;
                found = 1;
            } else {
                ed = (struct OHCI_ED *)pReg->ED_BULKHEAD;
                while (ed != NULL) {
                    if (ed->nextED == (uint32_t)edPre) {
                        ed->nextED = edPre->nextED;
                        found = 1;
                    }
                    ed = (struct OHCI_ED *)ed->nextED;
                }
            }
        }

        /* Remove and free all TDs under this endpoint */
        if (found) {
            td = (struct OHCI_TD *)(edPre->headP & ~0x3);
            if (td != NULL) {
                while (td != NULL) {
                    utr = td->utr;
                    tdNext = (struct OHCI_TD *)td->nextTD;
                    HAL_USBH_FreePool(td);
                    td = tdNext;

                    utr->tdCnt--;
                    if (utr->tdCnt == 0) {
                        utr->status = USBH_ERR_ABORT;
                        utr->bIsTransferDone = 1;
                        if (utr->CompleteCb) {
                            utr->CompleteCb(utr);
                        }
                    }
                }
            }
        }

        /* Done. Remove this ED from edRemoveList and free it. */
        ohci->edRemoveList = edPre->next;
        HAL_USBH_FreePool((void *)edPre);
    }
}

/** @} */

/********************* Public Function Definition ****************************/
/** @defgroup OHCI_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  OHCI interrupt handler
 * @param  pHCD: phandle of the EHCI HCD
 * @return HAL_Status
 * OHCI interrupt handler is always wrapped by the driver.
 */
HAL_Status HAL_OHCI_IRQHandler(void *pHCD)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);

    struct OHCI_TD *td, *tdPrev, *tdNext;
    uint32_t intrSts;

    intrSts = pReg->INTRSTATUS;

    HAL_DBG("ohci intrSts = 0x%lx\n", intrSts);

    if ((pReg->INTRENABLE & OHCI_INTRENABLE_SF_MASK) &&
        (intrSts & OHCI_INTRSTATUS_SF_MASK)) {
        intrSts &= ~OHCI_INTRSTATUS_SF_MASK;

        pReg->INTRDISABLE = OHCI_INTRDISABLE_SF_MASK;
        OHCI_RemoveEd(ohci);
        pReg->INTRSTATUS = OHCI_INTRSTATUS_SF_MASK;
    }

    if (intrSts & OHCI_INTRSTATUS_WDH_MASK) {
        //HAL_DBG("!%02x\n", pReg->FMNUMBER & 0xff);
        intrSts &= ~OHCI_INTRSTATUS_WDH_MASK;
        /* reverse done list */
        td = (struct OHCI_TD *)(ohci->hcca->doneHead & TD_ADDR_MASK);
        ohci->hcca->doneHead = 0;
        tdPrev = NULL;
        pReg->INTRSTATUS = OHCI_INTRSTATUS_WDH_MASK;

        while (td != NULL) {
            //HAL_DBG("Done list TD 0x%lx => 0x%lx\n", (uint32_t)td, (uint32_t)td->nextTD);
            tdNext = (struct OHCI_TD *)(td->nextTD & TD_ADDR_MASK);
            td->nextTD = (uint32_t)tdPrev;
            tdPrev = td;
            td = tdNext;
        }
        td = tdPrev; /* first TD of the reversed done list */

        /* Reclaim TDs */
        while (td != NULL) {
            HAL_DBG("Reclaim TD 0x%lx, next 0x%lx\n", (uint32_t)td, td->nextTD);
            tdNext = (struct OHCI_TD *)td->nextTD;
            OHCI_TdDone(td);
            HAL_USBH_FreePool(td);
            td = tdNext;
        }
    }

    if (intrSts & OHCI_INTRSTATUS_RHSC_MASK) {
        pReg->INTRDISABLE = OHCI_INTRDISABLE_RHSC_MASK;
    }

    pReg->INTRSTATUS = intrSts;

    return HAL_OK;
}

/**
 * @brief  Get the device speed in OHCI root hub port
 * @param  pHCD: phandle of the OHCI HCD
 * @param  portNum: the port number will be visited
 */
int HAL_OHCI_GetRhPortSpeed(void *pHCD, int portNum)
{
    struct OHCI_HCD *ohci = (struct OHCI_HCD *)pHCD;
    struct OHCI_REG *pReg = HCD_HANDLE_TO_REG(ohci);

    return (pReg->RH_PORTSTATUS[portNum] & OHCI_RH_PORTSTATUS_LSDA_MASK) ?
           USB_SPEED_LOW : USB_SPEED_FULL;
}

/** @} */

/** @defgroup OHCI_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  OHCI operations register, invoked by glue layer.
 * @param  ops: phandle of the HCD OPS
 * @return HAL_Status
 */
HAL_Status HAL_OHCI_OpsInit(struct HCD_OPS *ops)
{
    ops->Init = OHCI_Init;
    ops->Shutdown = OHCI_Shutdown;
    ops->Suspend = OHCI_Suspend;
    ops->Resume = OHCI_Resume;
    ops->CtrlXfer = OHCI_CtrlXfer;
    ops->BulkXfer = OHCI_BulkXfer;
    ops->IntrXfer = OHCI_IntrXfer;
    ops->IsoXfer = OHCI_IsoXfer;
    ops->QuitXfer = OHCI_QuitXfer;
    ops->RthubPortReset = OHCI_RhPortReset;
    ops->RthubPolling = OHCI_RtHubPolling;
    ops->RthubPortStatus = OHCI_RtHubPortStatus;

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_OHCI_MODULE_ENABLED */
