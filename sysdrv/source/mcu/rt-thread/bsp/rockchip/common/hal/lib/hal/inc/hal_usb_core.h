/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup USB_CORE
 *  @{
 */

#ifndef _HAL_USB_CORE_H_
#define _HAL_USB_CORE_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup USB_CORE_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** USB Core Mode */
#define USB_OTG_MODE_DEVICE 0U
#define USB_OTG_MODE_HOST   1U
#define USB_OTG_MODE_DRD    2U

/** USB Core Speed */
#define USB_OTG_SPEED_HIGH         0U
#define USB_OTG_SPEED_HIGH_IN_FULL 1U
#define USB_OTG_SPEED_LOW          2U
#define USB_OTG_SPEED_FULL         3U

/** USB Core MPS */
#define USB_OTG_HS_MAX_PACKET_SIZE 512U
#define USB_OTG_FS_MAX_PACKET_SIZE 64U
#define USB_OTG_MAX_EP0_SIZE       64U

/** USB PHY UTMI Width */
#define USB_PHY_UTMI_WIDTH_8  8
#define USB_PHY_UTMI_WIDTH_16 16
#define USB_PHY_UTMI_MASK     0x0000FFFFU

/** USB Core Phy Frequency */
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ (0 << 1)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ (1 << 1)
#define DSTS_ENUMSPD_LS_PHY_6MHZ           (2 << 1)
#define DSTS_ENUMSPD_FS_PHY_48MHZ          (3 << 1)

/** USB CORE Frame Interval */
#define DCFG_FRAME_INTERVAL_80 0U
#define DCFG_FRAME_INTERVAL_85 1U
#define DCFG_FRAME_INTERVAL_90 2U
#define DCFG_FRAME_INTERVAL_95 3U

/** USB EP0 MPS */
#define DEP0CTL_MPS_64 0U
#define DEP0CTL_MPS_32 1U
#define DEP0CTL_MPS_16 2U
#define DEP0CTL_MPS_8  3U

/** USB EP Speed */
#define EP_SPEED_LOW  0U
#define EP_SPEED_FULL 1U
#define EP_SPEED_HIGH 2U

/** USB EP Type */
#define EP_TYPE_CTRL 0U
#define EP_TYPE_ISOC 1U
#define EP_TYPE_BULK 2U
#define EP_TYPE_INTR 3U
#define EP_TYPE_MSK  3U

/** USB STS Defines */
#define STS_GOUT_NAK   1U
#define STS_DATA_UPDT  2U
#define STS_XFER_COMP  3U
#define STS_SETUP_COMP 4U
#define STS_SETUP_UPDT 6U

/** HCFG SPEED Defines */
#define HCFG_30_60_MHZ 0U
#define HCFG_48_MHZ    1U
#define HCFG_6_MHZ     2U

/** HPRT0 PRTSPD SPEED Defines */
#define HPRT0_PRTSPD_HIGH_SPEED 0U
#define HPRT0_PRTSPD_FULL_SPEED 1U
#define HPRT0_PRTSPD_LOW_SPEED  2U

#define HCCHAR_CTRL 0U
#define HCCHAR_ISOC 1U
#define HCCHAR_BULK 2U
#define HCCHAR_INTR 3U

#define HC_PID_DATA0 0U
#define HC_PID_DATA2 1U
#define HC_PID_DATA1 2U
#define HC_PID_SETUP 3U

#define GRXSTS_PKTSTS_IN              2U
#define GRXSTS_PKTSTS_IN_XFER_COMP    3U
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR 5U
#define GRXSTS_PKTSTS_CH_HALTED       7U

#define USB_OTG_GLOBAL_BASE       0x000U
#define USB_OTG_DEVICE_BASE       0x800U
#define USB_OTG_IN_ENDPOINT_BASE  0x900U
#define USB_OTG_OUT_ENDPOINT_BASE 0xB00U
#define USB_OTG_EP_REG_SIZE       0x20U
#define USB_OTG_HOST_BASE         0x400U
#define USB_OTG_HOST_PORT_BASE    0x440U
#define USB_OTG_HOST_CHANNEL_BASE 0x500U
#define USB_OTG_HOST_CHANNEL_SIZE 0x20U
#define USB_OTG_PCGCCTL_BASE      0xE00U
#define USB_OTG_FIFO_BASE         0x1000U
#define USB_OTG_FIFO_SIZE         0x1000U

#define USB_PCGCCTL  (*(__IO uint32_t *)((uint32_t)pUSB + USB_OTG_PCGCCTL_BASE))
#define USB_HPRT0    (*(__IO uint32_t *)((uint32_t)pUSB + USB_OTG_HOST_PORT_BASE))
#define USB_DEVICE   ((struct USB_DEVICE_REG *)((uint32_t)pUSB + USB_OTG_DEVICE_BASE))
#define USB_INEP(i)  ((struct USB_IN_EP_REG *)((uint32_t)pUSB + USB_OTG_IN_ENDPOINT_BASE + (i) * USB_OTG_EP_REG_SIZE))
#define USB_OUTEP(i) ((struct USB_OUT_EP_REG *)((uint32_t)pUSB + USB_OTG_OUT_ENDPOINT_BASE + (i) * USB_OTG_EP_REG_SIZE))
#define USB_DFIFO(i) (*(__IO uint32_t *)((uint32_t)pUSB + USB_OTG_FIFO_BASE + (i) * USB_OTG_FIFO_SIZE))
#define USB_HC(i)    ((struct USB_HOST_CH_REG *)((uint32_t)pUSB + USB_OTG_HOST_CHANNEL_BASE + (i) * USB_OTG_HOST_CHANNEL_SIZE))
#define USB_HOST     ((struct USB_HOST_REG *)((uint32_t)pUSB + USB_OTG_HOST_BASE))

#define USB_MASK_INTERRUPT(__INSTANCE__, __INTERRUPT__)   ((__INSTANCE__)->GINTMSK &= ~(__INTERRUPT__))
#define USB_UNMASK_INTERRUPT(__INSTANCE__, __INTERRUPT__) ((__INSTANCE__)->GINTMSK |= (__INTERRUPT__))
#define CLEAR_IN_EP_INTR(__EPNUM__, __INTERRUPT__)        (USB_INEP(__EPNUM__)->DIEPINT = (__INTERRUPT__))
#define CLEAR_OUT_EP_INTR(__EPNUM__, __INTERRUPT__)       (USB_OUTEP(__EPNUM__)->DOEPINT = (__INTERRUPT__))

/***************************** Structure Definition **************************/
/** OTG Mode definition */
typedef enum {
    USB_OTG_DEVICE_MODE = 0U,
    USB_OTG_HOST_MODE   = 1U,
    USB_OTG_DRD_MODE    = 2U
} eUSB_OTG_mode;

/** URB States definition */
typedef enum {
    URB_IDLE = 0U,
    URB_DONE,
    URB_NOTREADY,
    URB_NYET,
    URB_ERROR,
    URB_STALL
} eUSB_OTG_urbState;

/** URB States definition */
typedef enum {
    HC_IDLE = 0U,
    HC_XFRC,
    HC_HALTED,
    HC_NAK,
    HC_NYET,
    HC_STALL,
    HC_XACTERR,
    HC_BBLERR,
    HC_DATATGLERR
} eUSB_OTG_hcState;

/** USB Initialization Structure definition */
struct USB_OTG_CFG {
    uint32_t ep0Mps;                /*!< Set the Endpoint 0 Max Packet size.                                */
    uint32_t epNum;                 /*!< Device Endpoints number.
                                         This parameter depends on the used USB core.
                                         Must be a number between Min_Data = 1 and Max_Data = 15            */
    uint32_t hcNum;                 /*!< Host Channels number.
                                         This parameter Depends on the used USB core.
                                         Must be a number between Min_Data = 1 and Max_Data = 15            */
    uint32_t phyif;                 /*!< USB PHY interface.                                                 */
    uint32_t speed;                 /*!< USB Core speed.                                                    */
    uint32_t dmaEnable : 1;         /*!< Enable or disable of the USB embedded DMA.                         */
    uint32_t sofEnable : 1;         /*!< Enable or disable the output of the SOF signal.                    */
    uint32_t lpmEnable : 1;         /*!< Enable or disable Link Power Management.                           */
    uint32_t vbusSensingEnable : 1; /*!< Enable or disable the VBUS Sensing feature.                        */
    uint32_t suspendEnable : 1;     /*!< Enable or disable the auto suspend feature.                        */
};

/** EP Initialization Structure definition */
struct USB_OTG_EP {
    uint8_t num;            /*!< Endpoint number
                                 This parameter must be a number between Min_Data = 1 and Max_Data = 15   */
    uint8_t isIn;           /*!< Endpoint direction
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 1    */
    uint8_t isStall;        /*!< Endpoint stall condition
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 1    */
    uint8_t isocStart;      /*!< Enable high speed isoc transfer                                          */
    uint8_t isocPending;    /*!< Pending high speed isoc transfer                                         */
    uint8_t type;           /*!< Endpoint type                                                            */
    uint8_t dataPID;        /*!< Initial data PID
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 1    */
    uint8_t *pxferBuff;     /*!< Pointer to transfer buffer                                               */
    uint16_t txFIFONum;     /*!< Transmission FIFO number
                                 This parameter must be a number between Min_Data = 1 and Max_Data = 15   */
    uint32_t dmaAddr;       /*!< 32 bits aligned transfer buffer address                                  */
    uint32_t maxPacket;     /*!< Endpoint Max packet size
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 64KB */
    uint32_t xferLen;       /*!< Current transfer length                                                  */
    uint32_t xferCount;     /*!< Partial transfer length in case of multi packet transfer                 */
};

/** HCD Initialization Structure definition */
struct USB_OTG_HC {
    uint8_t chNum;         /*!< Host channel number.
                                This parameter must be a number between Min_Data = 1 and Max_Data = 15     */
    uint8_t dataPID;       /*!< Initial data PID.
                                This parameter must be a number between Min_Data = 0 and Max_Data = 1      */
    uint8_t devAddr;       /*!< USB device address.
                                This parameter must be a number between Min_Data = 1 and Max_Data = 255    */
    uint8_t doPing;        /*!< Enable or disable the use of the PING protocol for HS mode.                */
    uint8_t epNum;         /*!< Endpoint number.
                                This parameter must be a number between Min_Data = 1 and Max_Data = 15     */
    uint8_t epIsIn;        /*!< Endpoint direction
                                This parameter must be a number between Min_Data = 0 and Max_Data = 1      */
    uint8_t epType;        /*!< Endpoint Type.                                                             */
    uint8_t speed;         /*!< USB Host speed.                                                            */
    uint8_t toggleIn;      /*!< IN transfer current toggle flag.
                                This parameter must be a number between Min_Data = 0 and Max_Data = 1      */
    uint8_t toggleOut;     /*!< OUT transfer current toggle flag
                                This parameter must be a number between Min_Data = 0 and Max_Data = 1      */
    uint8_t *pxferBuff;    /*!< Pointer to transfer buffer.                                                */
    uint16_t maxPacket;    /*!< Endpoint Max packet size.
                                This parameter must be a number between Min_Data = 0 and Max_Data = 64KB   */
    uint32_t dmaAddr;      /*!< 32 bits aligned transfer buffer address.                                   */
    uint32_t errCount;     /*!< Host channel error count.*/
    uint32_t xferLen;      /*!< Current transfer length.                                                   */
    uint32_t xferCount;    /*!< Partial transfer length in case of multi packet transfer.                  */
    eUSB_OTG_urbState urbState; /*!< URB state. This parameter can be any value of @ref eUSB_OTG_urbState */
    eUSB_OTG_hcState hcState;   /*!< Host Channel state. This parameter can be any value of @ref eUSB_OTG_hcState  */
};

/** USB HW information definition on a soc */
struct HAL_USB_DEV {
    struct USB_GLOBAL_REG *pReg;
    uint32_t hclkGateID;
    uint32_t utmiclkGateID;
    uint32_t usbPhyGateID;
    IRQn_Type irqNum;
    struct USB_OTG_CFG cfg;
};

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup USB_CORE_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status USB_CoreInit(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_CFG cfg);
HAL_Status USB_DevInit(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_CFG cfg);
HAL_Status USB_EnableGlobalInt(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_DisableGlobalInt(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_SetCurrentMode(struct USB_GLOBAL_REG *pUSB, eUSB_OTG_mode mode);
HAL_Status USB_SetDevSpeed(struct USB_GLOBAL_REG *pUSB, uint8_t speed);
HAL_Status USB_FlushRxFifo(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_FlushTxFifo(struct USB_GLOBAL_REG *pUSB, uint32_t num);
HAL_Status USB_ActivateEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP);
HAL_Status USB_DeactivateEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP);
HAL_Status USB_ActivateDedicatedEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP);
HAL_Status USB_DeactivateDedicatedEndpoint(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP);
HAL_Status USB_EPStartXfer(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP, uint8_t dma);
HAL_Status USB_EP0StartXfer(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP, uint8_t dma);
HAL_Status USB_WritePacket(struct USB_GLOBAL_REG *pUSB, uint8_t *psrc, uint8_t chEpNum, uint16_t len, uint8_t dma);
void *USB_ReadPacket(struct USB_GLOBAL_REG *pUSB, uint8_t *pdest, uint8_t chEpNum, uint16_t len, uint8_t dma);
HAL_Status USB_EPSetStall(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP);
HAL_Status USB_EPClearStall(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_EP *pEP);
HAL_Status USB_SetDevAddress(struct USB_GLOBAL_REG *pUSB, uint8_t address);
HAL_Status USB_DevConnect(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_DevDisconnect(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_StopDevice(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_ActivateSetup(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_EP0_OutStart(struct USB_GLOBAL_REG *pUSB, uint8_t dma, uint8_t *psetup);
uint8_t USB_GetDevSpeed(struct USB_GLOBAL_REG *pUSB);
uint32_t USB_GetMode(struct USB_GLOBAL_REG *pUSB);
uint32_t USB_ReadInterrupts(struct USB_GLOBAL_REG *pUSB);
uint32_t USB_ReadDevAllOutEpInterrupt(struct USB_GLOBAL_REG *pUSB);
uint32_t USB_ReadDevOutEPInterrupt(struct USB_GLOBAL_REG *pUSB, uint8_t epNum);
uint32_t USB_ReadDevAllInEpInterrupt(struct USB_GLOBAL_REG *pUSB);
uint32_t USB_ReadDevInEPInterrupt(struct USB_GLOBAL_REG *pUSB, uint8_t epNum);
void USB_ClearInterrupts(struct USB_GLOBAL_REG *pUSB, uint32_t interrupt);

HAL_Status USB_HostInit(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_CFG cfg);
HAL_Status USB_InitFSLSPClkSel(struct USB_GLOBAL_REG *pUSB, uint8_t freq);
HAL_Status USB_ResetPort(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_DriveVbus(struct USB_GLOBAL_REG *pUSB, uint8_t state);
uint32_t USB_GetHostSpeed(struct USB_GLOBAL_REG *pUSB);
uint32_t USB_GetCurrentFrame(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_HCInit(struct USB_GLOBAL_REG *pUSB,
                      uint8_t chNum,
                      uint8_t epNum,
                      uint8_t devAddress,
                      uint8_t speed,
                      uint8_t epType,
                      uint16_t mps);
HAL_Status USB_HCStartXfer(struct USB_GLOBAL_REG *pUSB, struct USB_OTG_HC *pHC, uint8_t dma);
uint32_t USB_HCReadInterrupt(struct USB_GLOBAL_REG *pUSB);
HAL_Status USB_HCHalt(struct USB_GLOBAL_REG *pUSB, uint8_t hcNum);
HAL_Status USB_DoPing(struct USB_GLOBAL_REG *pUSB, uint8_t chNum);
HAL_Status USB_StopHost(struct USB_GLOBAL_REG *pUSB);

/** @} */

#endif /* _HAL_USB_CORE_H_ */

/** @} */

/** @} */

#endif /* HAL_PCD_MODULE_ENABLED || HAL_HCD_MODULE_ENABLED */
