/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_FSPI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup FSPI
 *  @{
 */

#ifndef _HAL_FSPI_H_
#define _HAL_FSPI_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup FSPI_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define HAL_FSPI_QUAD_ENABLE
#define HAL_FSPI_SPEED_THRESHOLD 50000000

/***************************** Structure Definition **************************/
/** FSPI_CTRL register datalines, addrlines and cmdlines value */
#define FSPI_LINES_X1 (0)
#define FSPI_LINES_X2 (1)
#define FSPI_LINES_X4 (2)

/** FSPI_CTRL bit union */
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
} FSPICTRL_DATA;

/** FSPI_CMD register rw value without shift */
#define FSPI_READ  (0)
#define FSPI_WRITE (1)

/** FSPI_CMD regitser addrbits value without shift */
#define FSPI_ADDR_0BITS  (0)
#define FSPI_ADDR_24BITS (1)
#define FSPI_ADDR_32BITS (2)
#define FSPI_ADDR_XBITS  (3)

/** FSPI_CMD bit union */
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
} FSPICMD_DATA;

typedef enum {
    DEV_UNKNON = 0,
    DEV_NOR,
    DEV_PSRAM,
    DEV_SPINAND,
} eFSPI_devType;

struct HAL_FSPI_XMMC_DEV {
    eFSPI_devType type;
    uint32_t ctrl;
    uint32_t readCmd;
    uint32_t writeCmd;
    uint8_t cell;
};

/** XIP may be not accessble, so place it in sram or psram */
struct HAL_FSPI_HOST {
    struct FSPI_REG *instance;
    uint32_t sclkGate;
    uint32_t hclkGate;
    uint32_t xipClkGate;
    eCLOCK_Name sclkID;
    IRQn_Type irqNum;
    uint32_t xipMemCode; /**< Better under icache */
    uint32_t xipMemData; /**< Better under dcache */
    uint8_t cs; /**< Should be defined by user in each operation */
    uint8_t mode; /**< Should be defined by user, referring to hal_spi_mem.h */
    uint8_t cell; /**< Record DLL cell for PM resume, Set depend on corresponding device */
    uint32_t xmmcCtrl; /**< Set depend on corresponding device */
    struct HAL_FSPI_XMMC_DEV xmmcDev[FSPI_CHIP_CNT]; /**< Set depend on corresponding device */
};

#define HAL_FSPI_MAX_DELAY_LINE_CELLS (0xFFU)

/** Training step */
#define HAL_FSPI_DLL_TRANING_STEP 5
/** Valid DLL winbow */
#define HAL_FSPI_DLL_TRANING_VALID_WINDOW 40

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup FSPI_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_FSPI_Init(struct HAL_FSPI_HOST *host);
HAL_Status HAL_FSPI_DeInit(struct HAL_FSPI_HOST *host);
HAL_Status HAL_FSPI_XferStart(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op);
HAL_Status HAL_FSPI_XferData(struct HAL_FSPI_HOST *host, uint32_t len, void *data, uint32_t dir);
HAL_Status HAL_FSPI_XferData_DMA(struct HAL_FSPI_HOST *host, uint32_t len, void *data, uint32_t dir);
HAL_Status HAL_FSPI_XferDone(struct HAL_FSPI_HOST *host);
HAL_Status HAL_FSPI_SpiXfer(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op);
HAL_Status HAL_FSPI_IRQHelper(struct HAL_FSPI_HOST *host);
HAL_Status HAL_FSPI_MaskDMAInterrupt(struct HAL_FSPI_HOST *host);
HAL_Status HAL_FSPI_UnmaskDMAInterrupt(struct HAL_FSPI_HOST *host);
HAL_Status HAL_FSPI_XmmcSetting(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op);
HAL_Status HAL_FSPI_XmmcRequest(struct HAL_FSPI_HOST *host, uint8_t on);
HAL_Status HAL_FSPI_SetDelayLines(struct HAL_FSPI_HOST *host, uint16_t cells);
HAL_Status HAL_FSPI_DLLDisable(struct HAL_FSPI_HOST *host);
uint32_t HAL_FSPI_GetXMMCStatus(struct HAL_FSPI_HOST *host);
uint32_t HAL_FSPI_GetMaxIoSize(struct HAL_FSPI_HOST *host);
uint32_t HAL_FSPI_GetMaxDllCells(struct HAL_FSPI_HOST *host);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_FSPI_MODULE_ENABLED */
