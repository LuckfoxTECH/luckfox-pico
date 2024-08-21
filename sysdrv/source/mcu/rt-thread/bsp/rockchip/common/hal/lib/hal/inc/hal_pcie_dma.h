/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_PCIE_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCIE
 *  @{
 */

#ifndef _HAL_PCIE_DMA_H_
#define _HAL_PCIE_DMA_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup PCIE_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/
/** PCIe DMA Channal */
enum HAL_PCIE_DMA_CHAN {
    DMA_REQ_DMA_CHAN0    = 0,
    DMA_REQ_DMA_CHAN1    = 1,
    DMA_REQ_DMA_CHAN_MAX = 2,
};

/** PCIe DMA Direction */
enum HAL_PCIE_DMA_DIR {
    DMA_FROM_BUS, /**< DMA from remote memory to local memory */
    DMA_TO_BUS, /**< DMA from local memory to remote memory */
};

/**
 * The Channel Control Register for read and write.
 */
union PCIE_DMA_CHAN_CTRL_LO {
    struct {
        uint32_t cb      : 1; // 0
        uint32_t tcb     : 1; // 1
        uint32_t llp     : 1; // 2
        uint32_t lie     : 1; // 3
        uint32_t rie     : 1; // 4
        uint32_t cs      : 2; // 5:6
        uint32_t rsvd1       : 1; // 7
        uint32_t ccs     : 1; // 8
        uint32_t llen        : 1; // 9
        uint32_t b_64s       : 1; // 10
        uint32_t b_64d       : 1; // 11
        uint32_t pf      : 5; // 12:16
        uint32_t rsvd2       : 7; // 17:23
        uint32_t sn      : 1; // 24
        uint32_t ro      : 1; // 25
        uint32_t td      : 1; // 26
        uint32_t tc      : 3; // 27:29
        uint32_t at      : 2; // 30:31
    };
    uint32_t asdword;
};

/**
 * The Channel Control Register high part for read and write.
 */
union PCIE_DMA_CHAN_CTRL_HI {
    struct {
        uint32_t vfenb       : 1; // 0
        uint32_t vfunc       : 8; // 1-8
        uint32_t rsvd0       : 23;   // 9-31
    };
    uint32_t asdword;
};

/**
 * The Channel Weight Register.
 */
union PCIE_DMA_WEIGHT {
    struct {
        uint32_t weight0     : 5; // 0:4
        uint32_t weight1     : 5; // 5:9
        uint32_t weight2     : 5; // 10:14
        uint32_t weight3     : 5; // 15:19
        uint32_t rsvd        : 12;   // 20:31
    };
    uint32_t asdword;
};

/**
 * The Doorbell Register for read and write.
 */
union PCIE_DMA_DB {
    struct {
        uint32_t chnl        : 3; // 0
        uint32_t reserved0   : 28;   // 3:30
        uint32_t stop        : 1; // 31
    };
    uint32_t asdword;
};

/**
 * The Context Registers for read and write.
 */
struct PCIE_DMA_CTX_REGS {
    union PCIE_DMA_CHAN_CTRL_LO ctrllo;
    union PCIE_DMA_CHAN_CTRL_HI ctrlhi;
    uint32_t xfersize;
    uint32_t sarptrlo;
    uint32_t sarptrhi;
    uint32_t darptrlo;
    uint32_t darptrhi;
};

/**
 * The Enable Register for read and write.
 */
union PCIE_DMA_ENB {
    struct {
        uint32_t enb     : 1; // 0
        uint32_t reserved0   : 31;   // 1:31
    };
    uint32_t asdword;
};

/**
 * The Interrupt Status Register for read and write.
 */
union PCIE_DMA_INT_STATUS {
    struct {
        uint32_t donesta     : 8;
        uint32_t rsvd0       : 8;
        uint32_t abortsta    : 8;
        uint32_t rsvd1       : 8;
    };
    uint32_t asdword;
};

/**
 * The Interrupt Clear Register for read and write.
 */
union PCIE_DMA_INI_CLEAR {
    struct {
        uint32_t doneclr     : 8;
        uint32_t rsvd0       : 8;
        uint32_t abortclr    : 8;
        uint32_t rsvd1       : 8;
    };
    uint32_t asdword;
};

/**
 * The DMA table for transmission.
 */
struct DMA_TABLE {
    uint32_t *descs;
    int chn; /**< DMA channel */
    enum HAL_PCIE_DMA_DIR dir;
    uint32_t type;
    union PCIE_DMA_ENB enb;
    struct PCIE_DMA_CTX_REGS ctxReg;
    union PCIE_DMA_WEIGHT weilo;
    union PCIE_DMA_WEIGHT weihi;
    union PCIE_DMA_DB start;
    uint64_t local; /**< DMA transfer local device's memory address */
    uint64_t bus; /**< DMA transfer remote device's memory address */
    uint32_t bufSize; /**< DMA transfer size */
};

/** @} */
/***************************** Function Declare ******************************/

#endif

/** @} */

/** @} */

#endif /* HAL_PCIE_MODULE_ENABLED */
