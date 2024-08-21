/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_OHCI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup OHCI
 *  @{
 */

#ifndef _HAL_OHCI_H_
#define _HAL_OHCI_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup OHCI_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** Preserved number frames while scheduling OHCI isochronous transfer */
#define OHCI_ISO_DELAY 4

/*
 * Host controller functional state.
 * for HCFS(HcControl[7:6])
 */
#define HCFS_RESET   (0UL << OHCI_CONTROL_HCFS_SHIFT)
#define HCFS_RESUME  (1UL << OHCI_CONTROL_HCFS_SHIFT)
#define HCFS_OPER    (2UL << OHCI_CONTROL_HCFS_SHIFT)
#define HCFS_SUSPEND (3UL << OHCI_CONTROL_HCFS_SHIFT)

/**************************  Bit definition for ED  **************************/
#define ED_CTRL_FA_SHIFT  0                 /**< Info[6:0]   - Function address             */
#define ED_CTRL_EN_SHIFT  7                 /**< Info[10:7]  - Endpoint number              */
#define ED_CTRL_DIR_SHIFT 11                /**< Info[12:11] - Direction                    */
#define ED_CTRL_MPS_SHIFT 16                /**< Info[26:16] - Maximum packet size          */

#define ED_FUNC_ADDR_MASK   (0x7fU)
#define ED_EP_ADDR_MASK     (0xfU << 7)
#define ED_DIR_MASK         (0x3U << 11)
#define ED_SPEED_MASK       (1U << 13)
#define ED_MAX_PK_SIZE_MASK (0x7ffU << 16)

#define ED_DIR_BY_TD      (0U << ED_CTRL_DIR_SHIFT)
#define ED_DIR_OUT        (1U << ED_CTRL_DIR_SHIFT)
#define ED_DIR_IN         (2U << ED_CTRL_DIR_SHIFT)
#define ED_SPEED_FULL     (0U << 13)        /**< Info[13] - 0: is full speed device         */
#define ED_SPEED_LOW      (1U << 13)        /**< Info[13] - 1: is low speed device          */
#define ED_SKIP           (1U << 14)        /**< Info[14] - 1: HC skip this ED              */
#define ED_FORMAT_GENERAL (0U << 15)        /**< Info[15] - 0: is a general TD              */
#define ED_FORMAT_ISO     (1U << 15)        /**< Info[15] - 1: is an isochronous TD         */
#define ED_HEADP_HALT     (1U << 0)         /**< HeadP[0] - 1: Halt; 0: Not                 */

/**************************  Bit definition for TD  **************************/
#define TD_ADDR_MASK 0xFFFFFFFCU

/* TD control field */
#define TD_CC             0xF0000000U
#define TD_CC_GET(td)     ((td >> 28) & 0x0F)
#define TD_CC_SET(td, cc) (td) = ((td) & 0x0FFFFFFF) | (((cc) & 0x0F) << 28)
#define TD_T_DATA0        0x02000000U
#define TD_T_DATA1        0x03000000U
#define TD_R              0x00040000U
#define TD_DP             0x00180000U
#define TD_DP_IN          0x00100000U
#define TD_DP_OUT         0x00080000U
#define MAXPSW            8U

/* steel TD reserved bits to keep driver data */
#define TD_TYPE_MASK (0x3U << 16)
#define TD_TYPE_CTRL (0x0U << 16)
#define TD_TYPE_BULK (0x1U << 16)
#define TD_TYPE_INT  (0x2U << 16)
#define TD_TYPE_ISO  (0x3U << 16)
#define TD_CTRL_MASK (0x7U << 15)
#define TD_CTRL_DATA (1U << 15)

/** OHCI CC status mapping definition */
typedef enum {
    /* mapping of the OHCI CC status to error codes */
    CC_NOERROR,                             /**< No  Error                                  */
    CC_CRC,                                 /**< CRC Error                                  */
    CC_BITSTUFF,                            /**< Bit Stuff                                  */
    CC_DATA_TOGGLE,                         /**< Data Toggle                                */
    CC_STALL,                               /**< Stall                                      */
    CC_NOTRESPONSE,                         /**< DevNotResp                                 */
    CC_PID_CHECK,                           /**< PIDCheck                                   */
    CC_UNEXPECTED_PID,                      /**< UnExpPID                                   */
    CC_DATA_OVERRUN,                        /**< DataOver                                   */
    CC_DATA_UNDERRUN,                       /**< DataUnder                                  */
    CC_RESERVED1,                           /**< reserved                                   */
    CC_RESERVED2,                           /**< reserved                                   */
    CC_BUFFER_OVERRUN,                      /**< BufferOver                                 */
    CC_BUFFER_UNDERRUN,                     /**< BuffUnder                                  */
    CC_NOT_ACCESS                           /**< Not Access                                 */
} eOCHI_ccStatus;

/***************************** Structure Definition **************************/

struct UTR;
struct USB_DEV;
struct HCD_OPS;

/** OHCI Endpoint Descriptor (ED) Structure definition */
struct OHCI_ED {
    /* OHCI spec. Endpoint descriptor  */
    uint32_t info;                     /**< Endpoint Descriptor info                        */
    uint32_t tailP;                    /**< TD Queue Tail Pointer                           */
    uint32_t headP;                    /**< TD Queue Head Pointer                           */
    uint32_t nextED;                   /**< Next Endpoint Descriptor                        */

    /* The following members are used by OHCI HAL libary*/
    uint8_t bInterval;
    uint16_t nextSF;                   /**< for isochronous transfer, recording the next SF */
    struct OHCI_ED *next;              /**< point to the next ED in remove list             */
} __ALIGNED(32);

/** OHCI General Transfer Descriptor (TD) Structure definition */
struct OHCI_TD {
    uint32_t info;                          /**< Transfer Descriptor info                   */
    uint32_t cbp;                           /**< Current Buffer Pointer                     */
    uint32_t nextTD;                        /**< Next TD                                    */
    uint32_t be;                            /**< Buffer End                                 */
    uint32_t psw[4];                        /**< PSW 0~7                                    */

    /* The following members are used by USB Host libary.   */
    uint32_t bufStart;                      /**< Buffer Start                               */
    struct OHCI_ED *ed;                     /**< The ED that this TD belong to.             */
    struct UTR *utr;                        /**< Associated UTR                             */
    struct OHCI_TD *next;                   /**< Point to next TD of the same UTR           */
} __ALIGNED(32);

/**
 * The HCCA (Host Controller Communications Area) is a 256 byte
 * structure defined in the OHCI spec. that the host controller is
 * told the base address of.  It must be 256-byte aligned.
 */
struct OHCI_HCCA {
    uint32_t intrTable[32];                 /**< Interrupt ED table                         */
    uint16_t frameNum;                      /**< current frame number                       */
    uint16_t pad1;                          /**< set to 0 on each frame_no change           */
    uint32_t doneHead;                      /**< info returned for an interrupt             */
    uint8_t reserved[116];
} __ALIGNED(256);

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup OHCI_Public_Function_Declare Public Function Declare
 *  @{
 */

int HAL_OHCI_GetRhPortSpeed(void *pHCD, int portNum);
HAL_Status HAL_OHCI_IRQHandler(void *pHCD);
HAL_Status HAL_OHCI_OpsInit(struct HCD_OPS *ops);

/** @} */

#endif /** _HAL_OHCI_H_ */

/** @} */

/** @} */

#endif /** HAL_OHCI_MODULE_ENABLED */
