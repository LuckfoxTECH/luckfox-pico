/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if (defined(HAL_GMAC_MODULE_ENABLED) || defined(HAL_GMAC1000_MODULE_ENABLED))

/** @addtogroup RK_HAL_Driver
  * @{
  */

/** @addtogroup GMAC
  * @{
  */

#ifndef __HAL_GMAC_H
#define __HAL_GMAC_H

#include "hal_def.h"
#include "hal_base.h"

/***************************** MACRO Definition ******************************/
/** @defgroup GMAC_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/* GMAC PHY indicates what features are supported by the interface. */
#define HAL_GMAC_PHY_SUPPORTED_10baseT_Half      (1 << 0)
#define HAL_GMAC_PHY_SUPPORTED_10baseT_Full      (1 << 1)
#define HAL_GMAC_PHY_SUPPORTED_100baseT_Half     (1 << 2)
#define HAL_GMAC_PHY_SUPPORTED_100baseT_Full     (1 << 3)
#define HAL_GMAC_PHY_SUPPORTED_1000baseT_Half    (1 << 4)
#define HAL_GMAC_PHY_SUPPORTED_1000baseT_Full    (1 << 5)
#define HAL_GMAC_PHY_SUPPORTED_Autoneg           (1 << 6)
#define HAL_GMAC_PHY_SUPPORTED_TP                (1 << 7)
#define HAL_GMAC_PHY_SUPPORTED_AUI               (1 << 8)
#define HAL_GMAC_PHY_SUPPORTED_MII               (1 << 9)
#define HAL_GMAC_PHY_SUPPORTED_FIBRE             (1 << 10)
#define HAL_GMAC_PHY_SUPPORTED_BNC               (1 << 11)
#define HAL_GMAC_PHY_SUPPORTED_10000baseT_Full   (1 << 12)
#define HAL_GMAC_PHY_SUPPORTED_Pause             (1 << 13)
#define HAL_GMAC_PHY_SUPPORTED_Asym_Pause        (1 << 14)
#define HAL_GMAC_PHY_SUPPORTED_2500baseX_Full    (1 << 15)
#define HAL_GMAC_PHY_SUPPORTED_Backplane         (1 << 16)
#define HAL_GMAC_PHY_SUPPORTED_1000baseKX_Full   (1 << 17)
#define HAL_GMAC_PHY_SUPPORTED_10000baseKX4_Full (1 << 18)
#define HAL_GMAC_PHY_SUPPORTED_10000baseKR_Full  (1 << 19)
#define HAL_GMAC_PHY_SUPPORTED_10000baseR_FEC    (1 << 20)
#define HAL_GMAC_PHY_SUPPORTED_1000baseX_Half    (1 << 21)
#define HAL_GMAC_PHY_SUPPORTED_1000baseX_Full    (1 << 22)

#define HAL_GMAC_PHY_DEFAULT_FEATURES (HAL_GMAC_PHY_SUPPORTED_Autoneg | \
                                       HAL_GMAC_PHY_SUPPORTED_TP |      \
                                       HAL_GMAC_PHY_SUPPORTED_MII)

#define HAL_GMAC_PHY_10BT_FEATURES (HAL_GMAC_PHY_SUPPORTED_10baseT_Half | \
                                    HAL_GMAC_PHY_SUPPORTED_10baseT_Full)

#define HAL_GMAC_PHY_100BT_FEATURES (HAL_GMAC_PHY_SUPPORTED_100baseT_Half | \
                                     HAL_GMAC_PHY_SUPPORTED_100baseT_Full)

#define HAL_GMAC_PHY_1000BT_FEATURES (HAL_GMAC_PHY_SUPPORTED_1000baseT_Half | \
                                      HAL_GMAC_PHY_SUPPORTED_1000baseT_Full)

#define HAL_GMAC_PHY_BASIC_FEATURES (HAL_GMAC_PHY_10BT_FEATURES |  \
                                     HAL_GMAC_PHY_100BT_FEATURES | \
                                     HAL_GMAC_PHY_DEFAULT_FEATURES)

#define HAL_GMAC_PHY_GBIT_FEATURES (HAL_GMAC_PHY_BASIC_FEATURES | \
                                    HAL_GMAC_PHY_1000BT_FEATURES)

/* GMAC flow ctrl Definition */
#define HAL_GMAC_FLOW_OFF  0
#define HAL_GMAC_FLOW_RX   1
#define HAL_GMAC_FLOW_TX   2
#define HAL_GMAC_FLOW_AUTO (HAL_GMAC_FLOW_TX | HAL_GMAC_FLOW_RX)

/* GMAC descriptions and buffers Definition */
#define HAL_GMAC_ALIGN(x, a)      (((x) + (a) - 1) & ~((a) - 1))
#define HAL_GMAC_DESCRIPTOR_WORDS 4
#define HAL_GMAC_DESCRIPTOR_SIZE  (HAL_GMAC_DESCRIPTOR_WORDS * 4)
#define HAL_GMAC_BUFFER_ALIGN     64
#define HAL_GMAC_MAX_FRAME_SIZE   1518
#define HAL_GMAC_MAX_PACKET_SIZE  HAL_GMAC_ALIGN(HAL_GMAC_MAX_FRAME_SIZE, HAL_GMAC_BUFFER_ALIGN)

/***************************** Structure Definition **************************/
struct GMAC_HANDLE;

/**
  * @brief  GMAC PHY Speed
  */
typedef enum {
    PHY_SPEED_10M   = 10,
    PHY_SPEED_100M  = 100,
    PHY_SPEED_1000M = 1000,
} eGMAC_PHY_SPEED;

/**
 * @brief  GMAC PHY Duplex
 */
typedef enum {
    PHY_DUPLEX_HALF = 0,
    PHY_DUPLEX_FULL = 1,
} eGMAC_PHY_DUPLEX;

/**
 * @brief  GMAC PHY Auto Negrotetion
 */
typedef enum {
    PHY_AUTONEG_DISABLE = 0,
    PHY_AUTONEG_ENABLE  = 1,
} eGMAC_PHY_NEGROTETION;

/**
  * @brief  GMAC PHY Interface Mode
  */
typedef enum {
    PHY_INTERFACE_MODE_MII,
    PHY_INTERFACE_MODE_RMII,
    PHY_INTERFACE_MODE_RGMII,
    PHY_INTERFACE_MODE_NONE,
} eGMAC_PHY_Interface;

/**
  * @brief  GMAC DMA IRQ Status
  */
typedef enum {
    DMA_UNKNOWN  = 0x0,
    DMA_HANLE_RX = 0x1,
    DMA_HANLE_TX = 0x2,
    DMA_TX_ERROR = 0x10,
    DMA_RX_ERROR = 0x20,
} eGMAC_IRQ_Status;

/**
  * @brief  GMAC PHY OPS Structure Definition
  */
struct GMAC_PHY_OPS {
    HAL_Status (*init)(struct GMAC_HANDLE *pGMAC);     /**< Will be called during HAL_GMAC_PHYInit(). */
    HAL_Status (*config)(struct GMAC_HANDLE *pGMAC);   /**< Called to configure the PHY, and modify the PHY,
                                                            based on the results. Should be called after HAL_GMAC_PHYInit(). */
    HAL_Status (*startup)(struct GMAC_HANDLE *pGMAC);  /**< Called when starting up the PHY */
    HAL_Status (*shutdown)(struct GMAC_HANDLE *pGMAC); /**< Called when bringing down the PHY */
    HAL_Status (*reset)(struct GMAC_HANDLE *pGMAC);    /**< Called when hardware reset */
    HAL_Status (*softreset)(struct GMAC_HANDLE *pGMAC);/**< Called when soft reset */
};

/**
  * @brief  GMAC PHY Config Structure Definition
  */
struct GMAC_PHY_Config {
    eGMAC_PHY_Interface interface;/**< Ethernet interface mode. */
    int16_t phyAddress;           /**< Ethernet PHY address,
                                       This parameter must be a number between Min = 0 and Max = 31 */
    eGMAC_PHY_NEGROTETION neg;    /**< Selects or disable the AutoNegotiation mode for the external PHY,
                                       The AutoNegotiation allows an automatic setting of the Speed
                                       (10/100/1000 Mbps) and the mode (half/full-duplex). */
    eGMAC_PHY_SPEED speed;        /**< Sets the Ethernet speed: 10/100/1000 Mbps
                                       while disable AutoNegotiation. */
    eGMAC_PHY_DUPLEX duplexMode;  /**< Selects the Ethernet duplex mode: Half-Duplex or
                                       Full-Duplex mode while disable AutoNegotiation. */
    eGMAC_PHY_SPEED maxSpeed;     /**< Sets the Ethernet max speed: 10/100/1000 Mbps. */
    uint32_t features;            /**< Sets the Ethernet PHY features. */
};

/**
  * @brief  GMAC PHY Status Structure Definition
  */
struct GMAC_PHY_STATUS {
    eGMAC_PHY_Interface interface;    /**< Ethernet interface mode. */

    /* forced speed & duplex (no autoneg)
     * partner speed & duplex & pause (autoneg)
     */
    eGMAC_PHY_SPEED speed;            /**< Ethernet speed. */
    eGMAC_PHY_DUPLEX duplex;          /**< Ethernet duplex. */
    eGMAC_PHY_SPEED maxSpeed;         /**< Ethernet max speed. */
    eGMAC_PHY_NEGROTETION neg;        /**< Ethernet AutoNegotiation or not. */

    /* The most recently read link state */
    int link;                         /**< Ethernet current link. */
    int oldLink;                      /**< Store the Ethernet last link. */

    uint32_t features;                /**< Ethernet PHY actual features. */
    uint32_t advertising;             /**< Ethernet PHY advertising features. */
    uint32_t supported;               /**< Ethernet PHY supported features. */

    int16_t addr;                     /**< Ethernet PHY address. */
    int16_t pause;                    /**< Ethernet PHY address. */
    uint32_t phyID;                   /**< Ethernet PHY ID. */
};

/**
  * @brief  GMAC Link Config Structure Definition
  */
struct GMAC_Link {
    uint32_t speedMask;
    uint32_t speed10;
    uint32_t speed100;
    uint32_t speed1000;
    uint32_t duplex;
};

/**
  * @brief  GMAC DMA Descriptors Data Structure Definition
  */
struct GMAC_Desc {
    uint32_t des0; /**< DMA Descriptors first word */
    uint32_t des1; /**< DMA Descriptors second word */
    uint32_t des2; /**< DMA Descriptors third word */
    uint32_t des3; /**< DMA Descriptors four word */
};

/**
  * @brief  GMAC DMA Transfer Status Structure Definition
  */
struct GMAC_DMAStats {
    uint32_t txUndeflowIRQ;
    uint32_t txProcessStoppedIRQ;
    uint32_t txJabberIRQ;
    uint32_t rxOverflowIRQ;
    uint32_t rxBufUnavIRQ;
    uint32_t rxProcessStoppedIRQ;
    uint32_t rxWatchdogIRQ;
    uint32_t txEarlyIRQ;
    uint32_t fatalBusErrorIRQ;
    uint32_t normalIRQN;
    uint32_t rxNormalIRQN;
    uint32_t txNormallIRQN;

    uint32_t rxEarlyIRQ;
    uint32_t thresHold;
    uint32_t txPktN;
    uint32_t rxPktN;
    uint32_t txBytesN;
    uint32_t rxBytesN;
    uint32_t txErrors;
    uint32_t rxErrors;
};

/**
  * @brief  GMAC device information Structure Definition
  */
struct GMAC_DEVICE_INFO {
    uint32_t miiAddrShift;
    uint32_t miiAddrMask;
    uint32_t miiRegShift;
    uint32_t miiRegMask;
    uint32_t clkCsrShift;
    uint32_t clkCsrMask;
};

/**
  * @brief  GMAC Handle Structure Definition
  */
struct GMAC_HANDLE {
    struct GMAC_REG *pReg;                /**< Register base address */
    uint32_t clkCSR;                      /**< clock csr value, div for MDC clock */

    struct GMAC_DEVICE_INFO mac;          /**< MAC information */

    struct GMAC_PHY_OPS phyOps;           /**< phy ops callback function */
    struct GMAC_PHY_Config phyConfig;     /**< phy config provied by user */
    struct GMAC_PHY_STATUS phyStatus;     /**< phy status */

    struct GMAC_Link link;                /**< GMAC link config */
    struct GMAC_DMAStats extraStatus;     /**< GMAC DMA transfer status */

    struct GMAC_Desc *rxDescs;            /**< First Rx descriptor pointer */
    struct GMAC_Desc *txDescs;            /**< First Tx descriptor pointer */
    uint8_t *txBuf;                       /**< First Tx buffer pointer */
    uint8_t *rxBuf;                       /**< First Tx buffer pointer */
    uint32_t txDescIdx;                   /**< Current Tx descriptor index */
    uint32_t rxDescIdx;                   /**< Current Rx descriptor pointer */
    uint32_t txSize;                      /**< Tx descriptor size*/
    uint32_t rxSize;                      /**< Rx descriptor size */
};

/**
  * @brief  GMAC HW Information Definition
  */
struct HAL_GMAC_DEV {
    struct GMAC_REG *pReg;
    eCLOCK_Name clkID;
    uint32_t clkGateID;
    eCLOCK_Name pclkID;
    uint32_t pclkGateID;
    IRQn_Type irqNum;
    ePM_RUNTIME_ID runtimeID;
};

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup GMAC_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_GMAC_Init(struct GMAC_HANDLE *pGMAC, struct GMAC_REG *pReg,
                         uint32_t freq, eGMAC_PHY_Interface interface,
                         bool extClk);
HAL_Status HAL_GMAC_DeInit(struct GMAC_HANDLE *pGMAC);
HAL_Status HAL_GMAC_Start(struct GMAC_HANDLE *pGMAC, uint8_t *addr);
HAL_Status HAL_GMAC_Stop(struct GMAC_HANDLE *pGMAC);
void HAL_GMAC_EnableDmaIRQ(struct GMAC_HANDLE *pGMAC);
void HAL_GMAC_DisableDmaIRQ(struct GMAC_HANDLE *pGMAC);
HAL_Status HAL_GMAC_DMATxDescInit(struct GMAC_HANDLE *pGMAC,
                                  struct GMAC_Desc *txDescs,
                                  uint8_t *txBuff, uint32_t txBuffCount);
HAL_Status HAL_GMAC_DMARxDescInit(struct GMAC_HANDLE *pGMAC,
                                  struct GMAC_Desc *rxDescs,
                                  uint8_t *rxBuff, uint32_t rxBuffCount);
eGMAC_IRQ_Status HAL_GMAC_IRQHandler(struct GMAC_HANDLE *pGMAC);
HAL_Status HAL_GMAC_AdjustLink(struct GMAC_HANDLE *pGMAC, int32_t txDelay,
                               int32_t rxDelay);
uint32_t HAL_GMAC_GetTXIndex(struct GMAC_HANDLE *pGMAC);
uint32_t HAL_GMAC_GetRXIndex(struct GMAC_HANDLE *pGMAC);
uint8_t *HAL_GMAC_GetTXBuffer(struct GMAC_HANDLE *pGMAC);
uint8_t *HAL_GMAC_GetRXBuffer(struct GMAC_HANDLE *pGMAC);
HAL_Status HAL_GMAC_Send(struct GMAC_HANDLE *pGMAC,
                         void *packet, uint32_t length);
uint8_t *HAL_GMAC_Recv(struct GMAC_HANDLE *pGMAC, int32_t *length);
void HAL_GMAC_CleanRX(struct GMAC_HANDLE *pGMAC);
void HAL_GMAC_WriteHWAddr(struct GMAC_HANDLE *pGMAC, uint8_t *enetAddr);
HAL_Status HAL_GMAC_PHYInit(struct GMAC_HANDLE *pGMAC,
                            struct GMAC_PHY_Config *config);
HAL_Status HAL_GMAC_PHYStartup(struct GMAC_HANDLE *pGMAC);
HAL_Status HAL_GMAC_PHYUpdateLink(struct GMAC_HANDLE *pGMAC);
HAL_Status HAL_GMAC_PHYParseLink(struct GMAC_HANDLE *pGMAC);
int32_t HAL_GMAC_MDIORead(struct GMAC_HANDLE *pGMAC, int32_t mdioAddr,
                          int32_t mdioReg);
HAL_Status HAL_GMAC_MDIOWrite(struct GMAC_HANDLE *pGMAC, int32_t mdioAddr,
                              int32_t mdioReg, uint16_t mdioVal);
void HAL_GMAC_SetToRGMII(struct GMAC_HANDLE *pGMAC,
                         int32_t txDelay, int32_t rxDelay);
void HAL_GMAC_SetToRMII(struct GMAC_HANDLE *pGMAC);
void HAL_GMAC_SetRGMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed);
void HAL_GMAC_SetRMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed);
void HAL_GMAC_SetExtclkSrc(struct GMAC_HANDLE *pGMAC, bool extClk);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_GMAC_MODULE_ENABLED */
