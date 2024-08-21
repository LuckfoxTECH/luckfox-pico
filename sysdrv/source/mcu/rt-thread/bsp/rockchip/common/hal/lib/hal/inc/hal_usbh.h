/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_EHCI_MODULE_ENABLED) || defined(HAL_OHCI_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup USB_HOST
 *  @{
 */

#ifndef  _HAL_USBH_H_
#define  _HAL_USBH_H_

#include "hal_def.h"
#include "hal_ehci.h"
#include "hal_ohci.h"

/***************************** MACRO Definition ******************************/
/** @defgroup USB_HOST_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/*
 * Host controller hardware transfer descriptors memory pool.
 *
 * ED/TD/ITD of OHCI and QH/QTD of EHCI are all allocated from this pool.
 * Allocated unit size is determined by MEM_POOL_UNIT_SIZE.
 * May allocate one or more units depend on hardware descriptor type.
 */
#define MEM_POOL_UNIT_SIZE 64            /**< A fixed hard coding setting. Do not change it!  */
#define MEM_POOL_UNIT_NUM  256           /**< Increase this or heap size if memory allocate failed. */

/* Port reset retry and time-out settings */
#define HUB_DEBOUNCE_TIME       800      /**< Hub connect/disconnect de-bounce time in ms     */
#define PORT_RESET_RETRY        3        /**< port reset retry times                          */
#define PORT_RESET_TIME_MS      50       /**< port reset time (ms)                            */
#define PORT_RESET_RETRY_INC_MS 25       /**< increased reset time (ms) after reset failed    */

/* Endpoint descriptor bEndpointAddress[7] - direction */
#define EP_ADDR_DIR_MASK 0x80
#define EP_ADDR_DIR_IN   0x80
#define EP_ADDR_DIR_OUT  0x00

/* Endpoint descriptor bmAttributes[1:0] - transfer type */
#define EP_ATTR_TT_MASK 0x03
#define EP_ATTR_TT_CTRL 0x00
#define EP_ATTR_TT_ISO  0x01
#define EP_ATTR_TT_BULK 0x02
#define EP_ATTR_TT_INT  0x03

/* USB HCD Error Codes */
#define USBH_OK                0         /**< No error.                                       */
#define USBH_ERR_MEMORY_OUT    -10       /**< Out of memory.                                  */
#define USBH_ERR_IF_ALT_LIMIT  -11       /**< Number of alternative interface > MAX_ALT_PER_IFACE */
#define USBH_ERR_IF_EP_LIMIT   -15       /**< Number of endpoints > MAX_EP_PER_IFACE          */
#define USBH_ERR_NOT_SUPPORTED -101      /**< Device/Class/Transfer not supported             */
#define USBH_ERR_NOT_MATCHED   -103      /**< Not macthed                                     */
#define USBH_ERR_NOT_EXPECTED  -104      /**< Unknown or unexpected                           */
#define USBH_ERR_INVALID_PARAM -105      /**< Invalid parameter                               */
#define USBH_ERR_NOT_FOUND     -106      /**< Device or interface not found                   */
#define USBH_ERR_EP_NOT_FOUND  -107      /**< Endpoint not found                              */
#define USBH_ERR_DESCRIPTOR    -137      /**< Failed to parse USB descriptors                 */
#define USBH_ERR_SET_DEV_ADDR  -139      /**< Failed to set device address                    */
#define USBH_ERR_SET_CONFIG    -151      /**< Failed to set device configuration              */

#define USBH_ERR_TRANSFER     -201       /**< USB transfer error                              */
#define USBH_ERR_TIMEOUT      -203       /**< USB transfer time-out                           */
#define USBH_ERR_ABORT        -205       /**< USB transfer aborted due to disconnect or reset */
#define USBH_ERR_PORT_RESET   -255       /**< Hub port reset failed                           */
#define USBH_ERR_SCH_OVERRUN  -257       /**< USB isochronous schedule overrun                */
#define USBH_ERR_DISCONNECTED -259       /**< USB device was disconnected                     */

#define USBH_ERR_TRANSACTION     -271    /**< USB transaction timeout, CRC, Bad PID, etc.     */
#define USBH_ERR_BABBLE_DETECTED -272    /**< An babble is detected during the transaction    */
#define USBH_ERR_DATA_BUFF       -274    /**< Data buffer overrun or underrun                 */

#define USBH_ERR_CC_NO_ERR     -280      /**< OHCI CC code - no error                         */
#define USBH_ERR_CRC           -281      /**< USB trasfer CRC error                           */
#define USBH_ERR_BIT_STUFF     -282      /**< USB transfer bit stuffing error                 */
#define USBH_ERR_DATA_TOGGLE   -283      /**< USB trasfer data toggle error                   */
#define USBH_ERR_STALL         -284      /**< USB trasfer STALL error                         */
#define USBH_ERR_DEV_NO_RESP   -285      /**< USB trasfer device no response error            */
#define USBH_ERR_PID_CHECK     -286      /**< USB trasfer PID check failure                   */
#define USBH_ERR_UNEXPECT_PID  -287      /**< USB trasfer unexpected PID error                */
#define USBH_ERR_DATA_OVERRUN  -288      /**< USB trasfer data overrun error                  */
#define USBH_ERR_DATA_UNDERRUN -289      /**< USB trasfer data underrun error                 */
#define USBH_ERR_BUFF_OVERRUN  -292      /**< USB trasfer buffer overrun error                */
#define USBH_ERR_BUFF_UNDERRUN -293      /**< USB trasfer buffer underrun error               */
#define USBH_ERR_NOT_ACCESS0   -294      /**< USB trasfer not accessed error                  */
#define USBH_ERR_NOT_ACCESS1   -295      /**< USB trasfer not accessed error                  */

/** USB device speed */
typedef enum {
    USB_SPEED_LOW,                       /**< USB1.0 Low Speed                                */
    USB_SPEED_FULL,                      /**< USB1.1 Full Speed                               */
    USB_SPEED_HIGH                       /**< USB2.0 High Speed                               */
} eUSB_Speed;

#ifndef HAL_EHCI_MODULE_ENABLED
#define NUM_IQH 0
#endif

/***************************** Structure Definition **************************/

struct USB_EP_INFO;

#define HCD_HANDLE_TO_REG(HC) ((HC)->pReg)

/** The EHCI HCD Structure definition */
struct EHCI_HCD {
    struct EHCI_REG *pReg;               /**< EHCI register base address                      */
    struct EHCI_QH *hQH;                 /**< head of reclamation list                        */
    struct EHCI_QH *iQH[NUM_IQH];        /**< qh of periodic frame list                       */
    struct EHCI_QH *qhRemoveList;        /**< qh of remove list                               */
    struct EHCI_QTD *qTDGhost;           /**< used as a terminator qTD                        */
    struct ECHI_ISO_EP *isoEpList;       /**< list of activated isochronous pipes             */
    uint32_t *pfList;                    /**< periodic frame list                             */
};

/** The OHCI HCD Structure definition */
struct OHCI_HCD {
    struct OHCI_REG *pReg;               /**< EHCI register base address                      */
    struct OHCI_HCCA *hcca;              /**< Point to Host Controller Communications Area    */
    struct OHCI_ED *iED[6];              /**< ed of interrupt                                 */
    struct OHCI_ED *edRemoveList;        /**< ed of remove list                               */
};

/** USB HCD Options Structure definition */
struct HCD_OPS {
    HAL_Status (*Init)(void *pHCD);
    HAL_Status (*Shutdown)(void *pHCD);
    HAL_Status (*Suspend)(void *pHCD);
    HAL_Status (*Resume)(void *pHCD);
    HAL_Status (*CtrlXfer)(struct UTR *utr);
    HAL_Status (*BulkXfer)(struct UTR *utr);
    HAL_Status (*IntrXfer)(struct UTR *utr);
    HAL_Status (*IsoXfer)(struct UTR *utr);
    HAL_Status (*QuitXfer)(void *pHCD, struct UTR *utr, struct USB_EP_INFO *ep);

    /** Root hub support */
    HAL_Status (*RthubPortReset)(void *pHCD, int portnum);
    uint32_t (*RthubPortStatus)(void *pHCD, int portnum);
    int (*RthubPolling)(void *pHCD);
};

/**
 * USB HCD Handle Structure definition
 * This wrapped EHCI/OHCI HCD and their opreations, used by Driver and HAL.
 */
struct USB_HCD_HANDLE {
    struct EHCI_HCD ehci;
    struct OHCI_HCD ohci;
    struct HCD_OPS opsEHCI;
    struct HCD_OPS opsOHCI;
    void *coherentMem;
};

/** USB device request setup packet */
__PACKED_STRUCT USB_SETUP_REQ {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};

/** USB Endpoint Info Structure definition */
struct USB_EP_INFO {
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint8_t bInterval;
    uint8_t bToggle;
    uint16_t wMaxPacketSize;
    void *hwPipe;                        /**< point to the HC assocaied endpoint              */
};

/**
 * USB Device Structure definition
 * This describe the deivce attached in EHCI/OHCI port
 */
struct USB_DEV {
    uint8_t portNum;                     /**< The hub port this device connected on           */
    uint8_t devNum;                      /**< device number                                   */
    int speed;                           /**< device speed (low/full/high)                    */

    /** The followings are lightweight USB stack internal used */
    uint8_t ehciPort;                    /**< device assigned to ehci?                        */
    struct USB_EP_INFO ep0;              /**< Endpoint 0                                      */
    struct USB_DEV *next;                /**< link for global usb device list                 */
};

#define IF_PER_UTR 8  /**< number of frames per UTR isochronous transfer (DO NOT modify it!)  */

/** UTR (USB Transfer Request) Structure definition */
struct UTR {
    struct USB_DEV *udev;                /**< point to associated USB device                  */
    struct USB_EP_INFO *ep;              /**< associated endpoint                             */
    struct USB_SETUP_REQ setup;          /**< buffer for setup packet                         */
    uint8_t *buff;                       /**< transfer buffer                                 */
    uint8_t bIsTransferDone;             /**< tansfer done?                                   */
    uint32_t dataLen;                    /**< length of data to be transferred                */
    uint32_t xferLen;                    /**< length of transferred data                      */
    uint8_t bIsoNewSched;                /**< New schedule isochronous transfer               */
    uint16_t isoSF;                      /**< Isochronous start frame number                  */
    uint16_t isoXlen[IF_PER_UTR];        /**< transfer length of isochronous frames           */
    uint8_t *isoBuff[IF_PER_UTR];        /**< transfer buffer address of isochronous frames   */
    int isoStatus[IF_PER_UTR];           /**< transfer status of isochronous frames           */
    int tdCnt;                           /**< number of transfer descriptors                  */
    int status;                          /**< return status                                   */
    int interval;                        /**< interrupt/isochronous interval                  */
    void *context;                       /**< point to deivce proprietary data area           */
    void *pHCD;                          /**< point to hcd that device assigned               */
    void (*CompleteCb)(struct UTR *);    /**< tansfer done call-back function                 */
    struct UTR *next;                    /**< point to the next UTR of the same endpoint      */
};

/**
 * USB Coherent Memory Structure definition
 * This describe the coherent meory for EHCI and OHCI HW structure required.
 */
struct USB_COHERENT_MEM {
    uint8_t *startAddr;                  /**< point to the start address of Coherent Memory   */
    uint8_t *pools[MEM_POOL_UNIT_NUM];   /**< Memory Pool allocate for QH/qTD/TD and etc.     */
    uint8_t *hccaAddr;                   /**< OHCI HCCA HW structure address                  */
    uint8_t *pfListAddr;                 /**< EHCI periodic frame list address                */
    uint8_t poolUsed[MEM_POOL_UNIT_NUM]; /**< mark the Memory Pool usage                      */
};

/** USB Host HW information definition on a soc */
struct HAL_USBH_DEV {
    struct EHCI_REG *ehciReg;            /**< EHCI HW base address                            */
    struct OHCI_REG *ohciReg;            /**< OHCI HW base address                            */
    IRQn_Type ehciIrqNum;                /**< EHCI interrupt Number                           */
    IRQn_Type ohciIrqNum;                /**< OHCI interrupt Number                           */
    uint32_t usbhGateID;                 /**< USB Host Gate ID                                */
    uint32_t usbhArbGateID;              /**< USB Host ARB Gate ID                            */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup USB_HOST_Public_Function_Declare Public Function Declare
 *  @{
 */

/* HAL HCD operate functions */
void *HAL_USBH_GetHcd(struct USB_HCD_HANDLE *hcdHdl, uint8_t ehciPort);
struct HCD_OPS *HAL_USBH_GetHcdOps(struct USB_HCD_HANDLE *hcdHdl, uint8_t ehciPort);
HAL_Status HAL_USBH_CoreInit(struct USB_HCD_HANDLE *hcdHdl, void * *coherentMem);

/* USB Host interrupt functions */
HAL_Status HAL_USBH_MaskIrq(uint8_t isEhci);
HAL_Status HAL_USBH_UnmaskIrq(uint8_t isEhci);

#define ENABLE_OHCI_IRQ()  HAL_USBH_UnmaskIrq(0)
#define DISABLE_OHCI_IRQ() HAL_USBH_MaskIrq(0)
#define ENABLE_EHCI_IRQ()  HAL_USBH_UnmaskIrq(1)
#define DISABLE_EHCI_IRQ() HAL_USBH_MaskIrq(1)

/* USB Host port management functions */
HAL_Status HAL_USBH_ResetPort(struct USB_HCD_HANDLE *hcdHdl, struct USB_DEV *);
HAL_Status HAL_USBH_ConnectCallback(int speed);
HAL_Status HAL_USBH_DisconnectCallback(struct USB_DEV *udev);
HAL_Status HAL_USBH_PollingRtHubs(struct USB_HCD_HANDLE *hcdHdl, struct USB_DEV *udevList);

/* USB Standard Request functions */
HAL_Status HAL_USBH_CtrlXfer(struct USB_HCD_HANDLE *hcdHdl, struct USB_DEV *udev, uint32_t *xferLen, struct UTR *utr, uint32_t timeout);
HAL_Status HAL_USBH_BulkXfer(struct USB_HCD_HANDLE *hcdHdl, struct UTR *utr);
HAL_Status HAL_USBH_IntrXfer(struct USB_HCD_HANDLE *hcdHdl, struct UTR *utr);
HAL_Status HAL_USBH_QuitXfer(struct USB_HCD_HANDLE *hcdHdl, struct USB_DEV *udev, struct USB_EP_INFO *ep);
HAL_Status HAL_USBH_QuitUtr(struct USB_HCD_HANDLE *hcdHdl, struct UTR *utr);

/* USB Host memory management functions */
void *HAL_USBH_AllocPool();
HAL_Status HAL_USBH_FreePool(void *pool);
uint32_t *HAL_USBH_AssignEhciPfList();
uint8_t *HAL_USBH_AssignOhciHcca();

/** @} */

#endif /** _HAL_USBH_H_ */

/** @} */

/** @} */

#endif /** HAL_EHCI_MODULE_ENABLED || HAL_OHCI_MODULE_ENABLED */
