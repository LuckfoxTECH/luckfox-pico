/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_SFC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SFC
 *  @{
 */

#ifndef _HAL_SFC_H_
#define _HAL_SFC_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup SFC_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define HAL_SFC_QUAD_ENABLE

/***************************** Structure Definition **************************/
/** SFC_CTRL register datalines, addrlines and cmdlines value */
#define SFC_LINES_X1 (0)
#define SFC_LINES_X2 (1)
#define SFC_LINES_X4 (2)

/** SFC_CTRL bit union */
typedef union {
    uint32_t d32;
    struct {
        unsigned mode : 1; /**< spi mode select */
        unsigned sps : 1; /**< shift in phase at: posedge 1: negedge */
        unsigned reserved3_2 : 2;
        unsigned scic : 4; /**< sclk_idle_level_cycles */
        unsigned cmdlines : 2; /**< cmd bits number */
        unsigned addrlines : 2; /**< address bits number */
        unsigned datalines : 2; /**< data bits number */
        unsigned reserved14_15 : 2;
        unsigned addrbits : 5;
        unsigned reserved31_21 : 11;
    } b;
} SFCCTRL_DATA;

/** SFC_CMD register rw value without shift */
#define SFC_READ  (0)
#define SFC_WRITE (1)

/** SFC_CMD regitser addrbits value without shift */
#define SFC_ADDR_0BITS  (0)
#define SFC_ADDR_24BITS (1)
#define SFC_ADDR_32BITS (2)
#define SFC_ADDR_XBITS  (3)

/** SFC_CMD bit union */
typedef union {
    uint32_t d32;
    struct {
        unsigned cmd : 8; /**< command that will send to Serial Flash */
        unsigned dummybits : 4; /**< dummy bits number */
        unsigned rw : 1; /**< 0:read, 1: write */
        unsigned readmode : 1; /**< continuous read mode */
        unsigned addrbits : 2; /**< address bits number */
        unsigned datasize : 14; /**< transferred bytes number */
        unsigned cs : 2; /**< chip select */
    } b;
} SFCCMD_DATA;

typedef enum {
    DEV_NOR = 0,
    DEV_PSRAM,
} eSFC_devType;

struct HAL_SFC_XMMC_DEV {
    eSFC_devType type;
    uint32_t ctrl;
    uint32_t readCmd;
    uint32_t writeCmd;
};

struct HAL_SFC_HOST {
    struct SFC_REG *instance;
    eCLOCK_Name sclkID;
    IRQn_Type irqNum;
    HAL_LockStatus status;
    uint32_t version;
    struct HAL_SFC_XMMC_DEV xmmcDev[SFC_CHIP_CNT];
};
/** @} */
/***************************** Function Declare ******************************/
/** @defgroup SFC_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_SFC_Init(struct HAL_SFC_HOST *host);
HAL_Status HAL_SFC_DeInit(struct HAL_SFC_HOST *host);
#ifdef HAL_SNOR_MODULE_ENABLED
HAL_Status HAL_SFC_SpiXfer(struct SNOR_HOST *spi, struct HAL_SPI_MEM_OP *op);
#endif
HAL_Status HAL_SFC_IRQHelper(struct HAL_SFC_HOST *host);
HAL_Status HAL_SFC_MaskDMAInterrupt(struct HAL_SFC_HOST *host);
HAL_Status HAL_SFC_UnmaskDMAInterrupt(struct HAL_SFC_HOST *host);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_SFC_MODULE_ENABLED */
