/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_EHCI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup EHCI
 *  @{
 */

#ifndef _HAL_EHCI_H_
#define _HAL_EHCI_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup EHCI_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** Preserved number of frames while scheduling EHCI isochronous transfer */
#define EHCI_ISO_DELAY 2

/**
 * When inspecting activated iTD/siTD, unconditionally reclaim iTD/isTD
 * scheduled in just elapsed EHCI_ISO_RCLM_RANGE ms.
 */
#define EHCI_ISO_RCLM_RANGE 32

#define FL_SIZE 512                           /**< frame list size can be 256, 512, or 1024   */
#define NUM_IQH 10                            /**< depends on FL_SIZE, 256:9, 512:10, 1024:11 */

/** Interrupt Threshold Control (1, 2, 4, 6, .. 64) */
#define UCMDR_INT_THR_CTRL (0x1 << EHCI_USBCMD_ITC_SHIFT)

/*************************** Bit definition for qTD **************************/
#define QTD_LIST_END 0x1                      /**< Indicate the terminate of qTD list.        */
#define QTD_PTR(x)   ((struct EHCI_QTD *)((uint32_t)(x) & ~0x1F))

/* Status: qTD Token[7:0] */
#define QTD_STS_PS_OUT         (0U << 0)      /**< directs the HC to issue an OUT PID         */
#define QTD_STS_PS_PING        (1U << 0)      /**< directs the HC to issue an PING PID        */
#define QTD_STS_SPLIT_STRAT    (0U << 1)      /**< directs the HC to issue an Start split     */
#define QTD_STS_SPLIT_COMPLETE (1U << 1)      /**< directs the HC to issue an Complete split  */
#define QTD_STS_MISS_MF        (1U << 2)      /**< miss a required complete-split transaction */
#define QTD_STS_XactErr        (1U << 3)      /**< Transaction Error occurred                 */
#define QTD_STS_BABBLE         (1U << 4)      /**< Babble Detected                            */
#define QTD_STS_DATA_BUFF_ERR  (1U << 5)      /**< Data Buffer Error                          */
#define QTD_STS_HALT           (1U << 6)      /**< Halted                                     */
#define QTD_STS_ACTIVE         (1U << 7)      /**< Active                                     */

/* PID: qTD Token[9:8] */
#define QTD_PID_MASK       (0x3U << 8)
#define QTD_PID_OUT        (0U << 8)          /**< generates token (E1H)                      */
#define QTD_PID_IN         (1U << 8)          /**< generates token (69H)                      */
#define QTD_PID_SETUP      (2U << 8)          /**< generates token (2DH)                      */
#define QTD_ERR_COUNTER    (3U << 10)         /**< Token[11:10]                               */
#define QTD_IOC            (1U << 15)         /**< Token[15] - Interrupt On Complete          */
#define QTD_TODO_LEN_SHIFT 16                 /**< Token[31:16] - Total Bytes to Transfer     */
#define QTD_TODO_LEN(x)    (((x) >> 16) & 0x7FFF)
#define QTD_DT             (1U << 31)         /**< Token[31] - Data Toggle                    */

/*************************** Bit definition for QH ***************************/
/* HLink[0] T field of "Queue Head Horizontal Link Pointer" */
#define QH_HLNK_END 0x1

/* HLink[2:1] Typ field of "Queue Head Horizontal Link Pointer" */
#define QH_HLNK_ITD(x)  (((uint32_t)(x) & ~0x1F) | 0x0)
#define QH_HLNK_QH(x)   (((uint32_t)(x) & ~0x1F) | 0x2)
#define QH_HLNK_SITD(x) (((uint32_t)(x) & ~0x1F) | 0x4)
#define QH_HLNK_FSTN(x) (((uint32_t)(x) & ~0x1F) | 0x6)
#define QH_PTR(x)       ((struct EHCI_QH *)((uint32_t)(x) & ~0x1F))

/* Bit fields of "Endpoint Characteristics" */
#define QH_NAK_RL         (4U << 28)          /**< Chrst[31:28] - NAK Count Reload             */
#define QH_CTRL_EP_FLAG   (1U << 27)          /**< Chrst[27] - Control Endpoint Flag           */
#define QH_RCLM_LIST_HEAD (1U << 15)          /**< Chrst[15] - Head of Reclamation List Flag   */
#define QH_DTC            (1U << 14)          /**< Chrst[14] - Data Toggle Control             */
#define QH_EPS_FULL       (0U << 12)          /**< Chrst[13:12] - Endpoint Speed (Full)        */
#define QH_EPS_LOW        (1U << 12)          /**< Chrst[13:12] - Endpoint Speed (Low)         */
#define QH_EPS_HIGH       (2U << 12)          /**< Chrst[13:12] - Endpoint Speed (High)        */
#define QH_I_NEXT         (1U << 7)           /**< Chrst[7] - Inactivate on Next Transaction   */

/* Bit fields of "Endpoint Capabilities" */
#define QH_MULT_SHIFT     30                  /**< Cap[31:30] - High-Bandwidth Pipe Multiplier */
#define QH_HUB_PORT_SHIFT 23                  /**< Cap[29:23] - Hub Port Number                */
#define QH_HUB_ADDR_SHIFT 16                  /**< Cap[22:16] - Hub Addr                       */
#define QH_C_MASK_MASK    0xFF00              /**< Cap[15:8]  - uFrame C-mask                  */
#define QH_S_MASK_MASK    0x00FF              /**< Cap[7:0]   - uFrame S-mask                  */

/*************************  Bit definition for iTD  **************************/
/* Next_Link[2:1] Typ field of "Next Schedule Element Pointer"  Typ field */
#define ITD_HLNK_ITD(x)  (((uint32_t)(x) & ~0x1F) | 0x0)
#define ITD_HLNK_QH(x)   (((uint32_t)(x) & ~0x1F) | 0x2)
#define ITD_HLNK_SITD(x) (((uint32_t)(x) & ~0x1F) | 0x4)
#define ITD_HLNK_FSTN(x) (((uint32_t)(x) & ~0x1F) | 0x6)
#define ITD_PTR(x)       ((struct EHCI_ITD *)((uint32_t)(x) & ~0x1F))

/* Transaction[8] */
#define ITD_STATUS(x)       (((x) >> 28) & 0xF)
#define ITD_STATUS_ACTIVE   (0x80000000UL)              /**< Active                           */
#define ITD_STATUS_BUFF_ERR (0x40000000UL)              /**< Data Buffer Error                */
#define ITD_STATUS_BABBLE   (0x20000000UL)              /**< Babble Detected                  */
#define ITD_STATUS_XACT_ERR (0x10000000UL)              /**< Transcation Error                */

#define ITD_XLEN_SHIFT    16
#define ITD_XFER_LEN(x)   (((x) >> 16) & 0xFFF)
#define ITD_IOC           (1U << 15)
#define ITD_PG_SHIFT      12
#define ITD_XFER_OFF_MASK 0xFFF

/* bufPage[7] */
#define ITD_BUFF_PAGE_SHIFT 12
/* bufPage[0] */
#define ITD_EP_NUM_SHIFT   8
#define ITD_EP_NUM(itd)    (((itd)->bufPage[0] >> 8) & 0xF)
#define ITD_DEV_ADDR_SHIFT 0
#define ITD_DEV_ADDR(itd)  ((itd)->bufPage[0] & 0x7F)
/* bufPage[1] */
#define ITD_DIR_IN          (1U << 11)
#define ITD_DIR_OUT         (0U << 11)
#define ITD_MAX_PKTSZ_SHIFT 0
#define ITD_MAX_PKTSZ(itd)  ((itd)->bufPage[1] & 0x7FF)

/*************************** Bit definition for SiTD *************************/
#define SITD_LIST_END 0x1                    /**< Indicate the terminate of siTD list.        */

#define SITD_XFER_IO_MASK (1U << 31)
#define SITD_XFER_IN      (1U << 31)
#define SITD_XFER_OUT     (0U << 31)

#define SITD_PORT_NUM_SHIFT 24
#define SITD_HUB_ADDR_SHIFT 16
#define SITD_EP_NUM_SHIFT   8
#define SITD_DEV_ADDR_SHIFT 0

#define SITD_IOC            (1U << 31)
#define SITD_XFER_CNT_SHIFT 16
#define SITD_XFER_CNT_MASK  (0x3FFU << SITD_XFER_CNT_SHIFT)

#define SITD_STATUS(x)         ((x) & 0xFC)
#define SITD_STATUS_ACTIVE     0x80
#define SITD_STATUS_ERR        0x40
#define SITD_STATUS_BUFF_ERR   0x20
#define SITD_BABBLE_DETECTED   0x10
#define SITD_STATUS_XFER_ERR   0x08
#define SITD_STATUS_MISSED_MF  0x04
#define SITD_STATUS_ERROR_MASK 0x78

/* Next_Link[2:1] Typ field of "Next Schedule Element Pointer"  Typ field */
#define SITD_HLNK_ITD(x)  (((uint32_t)(x) & ~0x1F) | 0x0)
#define SITD_HLNK_QH(x)   (((uint32_t)(x) & ~0x1F) | 0x2)
#define SITD_HLNK_SITD(x) (((uint32_t)(x) & ~0x1F) | 0x4)
#define SITD_HLNK_FSTN(x) (((uint32_t)(x) & ~0x1F) | 0x6)
#define SITD_PTR(x)       ((struct EHCI_SITD *)((uint32_t)(x) & ~0x1F))

#define HLINK_IS_TERMINATED(x) (((uint32_t)(x) & 0x1) ? 1 : 0)
#define HLINK_IS_SITD(x)       ((((uint32_t)(x) & 0x6) == 0x4) ? 1 : 0)

/***************************** Structure Definition **************************/

struct UTR;
struct EHCI_HCD;
struct USB_EP_INFO;
struct HCD_OPS;

/** Queue Element Transfer Descriptor (qTD) */
struct EHCI_QTD {
    uint32_t qTDNext;                          /**< Next qTD Pointer                          */
    uint32_t qTDAltNext;                       /**< Alternate Next qTD Pointer                */
    uint32_t token;                            /**< qTD Token                                 */
    uint32_t bufPage[5];                       /**< qTD Buffer Page Pointer List              */

    /* The following members are used by EHCI HAL libary */
    struct UTR *utr;                           /**< associated UTR                            */
    uint32_t xferLen;                          /**< assigned transfer transfer length         */
    struct EHCI_QH *qh;                        /**< The QH that this qTD belong to.           */
    struct EHCI_QTD *next;                     /**< link to qtd_list in the same QH           */
} __ALIGNED(32);

/** Queue Head (QH) Structure definition */
struct EHCI_QH {
    /* EHCI Specification 0.95 Section 3.6, Queue Head Structure definition */
    uint32_t hLink;                           /**< Queue Head Horizontal Link Pointer         */
    uint32_t chrst;                           /**< Endpoint Characteristics: QH DWord 1       */
    uint32_t cap;                             /**< Endpoint Capabilities: QH DWord 2          */
    uint32_t qTDCurr;                         /**< Current qTD Pointer                        */

    /* The followings are qTD Transfer Overlay */
    uint32_t qTDNextOL;                       /**< Next qTD Pointer                           */
    uint32_t qTDAltNextOL;                    /**< Alternate Next qTD Pointer                 */
    uint32_t tokenOL;                         /**< qTD Token                                  */
    uint32_t bufPageOL[5];                    /**< qTD Buffer Page Pointer List               */

    /* The following members are used by EHCI HAL libary */
    struct EHCI_QTD *dummy;                   /**< point to the inactive dummy qTD            */
    struct EHCI_QTD *qTDList;                 /**< currently linked qTD transfers             */
    struct EHCI_QTD *doneList;                /**< currently linked qTD transfers             */
    struct EHCI_QH *next;                     /**< point to the next QH in remove list        */
} __ALIGNED(32);

/** Isochronous (High-Speed) Transfer Descriptor (iTD) */
struct EHCI_ITD {
    uint32_t nextLink;                        /**< Next Link Pointer                          */
    uint32_t transaction[8];                  /**< Transaction Status and Control             */
    uint32_t bufPage[7];                      /**< Buffer Page Pointer List                   */

    /* The following members are used by EHCI HAL libary */
    struct ECHI_ISO_EP *isoEp;                /**< associated isochronous information block   */
    struct UTR *utr;                          /**< associated UTR                             */
    uint32_t bufBase;                         /**< buffer base address                        */
    uint8_t fIdx;                             /**< iTD's first index to UTR iso frames        */
    uint8_t transMask;                        /**< mask of activated transactions in iTD      */
    uint32_t schedFrameIdx;                   /**< scheduled frame index                      */
    struct EHCI_ITD *next;                    /**< used by software to maintain iTD list      */
} __ALIGNED(32);

/** Split Isochronous (Full-Speed) Transfer Descriptor (siTD) */
struct EHCI_SITD {
    uint32_t nextLink;                        /**< Next Link Pointer                          */
    uint32_t chrst;                           /**< Endpoint and Transaction Translator Characteristics */
    uint32_t sched;                           /**< Micro-frame Schedule Control               */
    uint32_t stsCtrl;                         /**< siTD Transfer Status and Control           */
    uint32_t bufPage[2];                      /**< Buffer Page Pointer List                   */
    uint32_t backLink;                        /**< siTD Back Link Pointer                     */

    /* The following members are used by USB Host libary */
    struct ECHI_ISO_EP *isoEp;                /**< associated isochronous information block   */
    struct UTR *utr;                          /**< associated UTR                             */
    uint8_t fIdx;                             /**< iTD's first index to UTR iso frames        */
    uint32_t schedFrameIdx;                   /**< scheduled frame index                      */
    struct EHCI_SITD *next;                   /**< used by software to maintain siTD list     */
} __ALIGNED(32);

/** Isochronous endpoint transfer information block. (Software only) */
struct ECHI_ISO_EP {
    struct USB_EP_INFO *ep;
    uint32_t nextFrame;                       /**< frame number of next scheduling            */
    struct EHCI_ITD *iTDList;                 /**< Reference to a list of installed iTDs      */
    struct EHCI_ITD *iTDDoneList;             /**< Reference to a list of completed iTDs      */
    struct EHCI_SITD *siTDList;               /**< Reference to a list of installed siTDs     */
    struct EHCI_SITD *siTDdoneList;           /**< Reference to a list of completed siTDs     */
    struct ECHI_ISO_EP *next;                 /**< used by software to maintain ISO EP list   */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup EHCI_Private_Definition Private Definition
 *  @{
 */

HAL_Status HAL_EHCI_IsoXfer(struct UTR *utr);
HAL_Status HAL_EHCI_QuitIsoXfer(struct UTR *utr, struct USB_EP_INFO *ep);
HAL_Status HAL_EHCI_ScanIsochronousList(struct EHCI_HCD *ehci);

/** @} */
/** @defgroup EHCI_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_EHCI_IRQHandler(void *pHCD);
HAL_Status HAL_EHCI_RelinquishPort(void *pHCD, int portNum);
HAL_Status HAL_EHCI_OpsInit(struct HCD_OPS *ops);

/** @} */

#endif /** _HAL_EHCI_H_ */

/** @} */

/** @} */

#endif /** HAL_EHCI_MODULE_ENABLED */
