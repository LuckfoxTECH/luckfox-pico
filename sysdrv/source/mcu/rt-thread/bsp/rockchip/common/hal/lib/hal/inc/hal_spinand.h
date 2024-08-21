/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_SPINAND_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SPINAND
 *  @{
 */

#ifndef _HAL_SPINAND_H_
#define _HAL_SPINAND_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup SPINAND_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define SPINAND_SPEED_MAX     133000000
#define SPINAND_SPEED_DEFAULT 80000000

#define SPINAND_SECTOR_FULL_SIZE 528
#define SPINAND_SECTOR_SIZE      512

#define SPINAND_MAX_ID_LEN 4

/* status register */
#define SPINAND_BUSY        HAL_BIT(0)
#define SPINAND_ERASE_FAIL  HAL_BIT(2)
#define SPINAND_PROG_FAIL   HAL_BIT(3)
#define SPINAND_ECC_MASK    HAL_GENMASK(5, 4)
#define SPINAND_ECC_OK      (0 << 4)
#define SPINAND_ECC_REFRESH (1 << 4)
#define SPINAND_ECC_ERROR   (2 << 4)

/***************************** Structure Definition **************************/
struct SPI_NAND_HOST {
    uint32_t speed;
    uint32_t mode;
    uint8_t flags;
    HAL_Status (*xfer)(struct SPI_NAND_HOST *spi, struct HAL_SPI_MEM_OP *op);

    void *userdata;
};

/* In some case, fews mega area in ECC protect is needed, for example RK ftl */
struct META_AREA {
    uint8_t off0;
    uint8_t off1;
    uint8_t off2;
    uint8_t off3;
};

/* SPI_NAND dev handler */
struct SPI_NAND {
    struct SPI_NAND_HOST *spi;
    uint32_t *pageBuf; /**< Dma buffer */

    uint32_t size; /**< size in Byte */
    uint16_t secPerPage;
    uint16_t pagePerBlk;
    uint16_t blkPerPlane;
    uint16_t planePerDie;

    uint8_t readLines;
    uint8_t progLines;
    struct META_AREA meta;
    int32_t (*eccStatus)(struct SPI_NAND *spinand);
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup SPINAND_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_SPINAND_EraseBlock(struct SPI_NAND *spinand, uint32_t addr);
int32_t HAL_SPINAND_ReadPageRaw(struct SPI_NAND *spinand, uint32_t addr, void *pData, bool oob);
HAL_Status HAL_SPINAND_ProgPageRaw(struct SPI_NAND *spinand, uint32_t addr, const void *pData, bool oob);
HAL_Status HAL_SPINAND_Init(struct SPI_NAND *nor);
HAL_Status HAL_SPINAND_DeInit(struct SPI_NAND *nor);
HAL_Status HAL_SPINAND_ReadID(struct SPI_NAND *spinand, uint8_t *data);
uint32_t HAL_SPINAND_GetCapacity(struct SPI_NAND *spinand);
HAL_Check HAL_SPINAND_IsBad(struct SPI_NAND *spinand, uint32_t addr);
HAL_Status HAL_SPINAND_MarkBad(struct SPI_NAND *spinand, uint32_t addr);
HAL_Check HAL_SPINAND_IsFlashSupported(uint8_t *flashId);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_SPINAND_MODULE_ENABLED */
