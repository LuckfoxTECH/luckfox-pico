/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_QPIPSRAM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup QPIPSRAM
 *  @{
 */

#ifndef _HAL_QPIPSRAM_H_
#define _HAL_QPIPSRAM_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup QPIPSRAM_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define QPIPSRAM_PROTO_STR(a, b, c)            (((a) << 8) | ((b) << 4) | (c))
#define QPIPSRAM_GET_PROTOCOL_CMD_BITS(proto)  (((proto) >> 8) & 0xf)
#define QPIPSRAM_GET_PROTOCOL_ADDR_BITS(proto) (((proto) >> 4) & 0xf)
#define QPIPSRAM_GET_PROTOCOL_DATA_BITS(proto) ((proto) & 0xf)

enum QPIPSRAM_PROTOCOL {
    QPIPSRAM_PROTO_1_1_1 = QPIPSRAM_PROTO_STR(1, 1, 1),
    QPIPSRAM_PROTO_1_1_2 = QPIPSRAM_PROTO_STR(1, 1, 2),
    QPIPSRAM_PROTO_1_1_4 = QPIPSRAM_PROTO_STR(1, 1, 4),
    QPIPSRAM_PROTO_1_1_8 = QPIPSRAM_PROTO_STR(1, 1, 8),
    QPIPSRAM_PROTO_1_2_2 = QPIPSRAM_PROTO_STR(1, 2, 2),
    QPIPSRAM_PROTO_1_4_4 = QPIPSRAM_PROTO_STR(1, 4, 4),
    QPIPSRAM_PROTO_1_8_8 = QPIPSRAM_PROTO_STR(1, 8, 8),
    QPIPSRAM_PROTO_2_2_2 = QPIPSRAM_PROTO_STR(2, 2, 2),
    QPIPSRAM_PROTO_4_4_4 = QPIPSRAM_PROTO_STR(4, 4, 4),
    QPIPSRAM_PROTO_8_8_8 = QPIPSRAM_PROTO_STR(8, 8, 8),
};

#define QPIPSRAM_SPEED_MAX     133000000
#define QPIPSRAM_SPEED_DEFAULT 80000000

/***************************** Structure Definition **************************/

struct QPIPSRAM_HOST {
    uint32_t speed;
    uint32_t mode;
    uint8_t flags;
    uint8_t cs;
    uint32_t xipMem; /**< XIP data mapped memory */
    uint32_t xipMemCode; /**< XIP code mapped memory */
    HAL_Status (*xfer)(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op);
    HAL_Status (*xipConfig)(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op, uint32_t on);
    void (*cs_gpio_take)(void);
    void (*cs_gpio_release)(void);

    void *userdata;
};

struct QPI_PSRAM {
    struct QPIPSRAM_HOST *spi;
    uint8_t id[2];
    uint8_t addrWidth;
    uint8_t readOpcode;
    uint8_t readDummy;
    uint8_t programOpcode;
    uint8_t programDummy;

    HAL_Status (*readReg)(struct QPI_PSRAM *psram, uint8_t opcode, uint8_t *buf, uint32_t len);
    HAL_Status (*writeReg)(struct QPI_PSRAM *psram, uint8_t opcode, uint8_t *buf, uint32_t len);

    int32_t (*read)(struct QPI_PSRAM *psram, uint32_t from,
                    uint32_t len, uint8_t *read_buf);
    int32_t (*write)(struct QPI_PSRAM *psram, uint32_t to,
                     uint32_t len, const uint8_t *write_buf);

    enum QPIPSRAM_PROTOCOL readProto;
    enum QPIPSRAM_PROTOCOL writeProto;

    uint32_t size;
    uint32_t pageSize;
};
/** @} */

/***************************** Function Declare ******************************/
/** @defgroup QPIPSRAM_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_QPIPSRAM_Init(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_DeInit(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_XIPEnable(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_XIPDisable(struct QPI_PSRAM *psram);
HAL_Check HAL_QPIPSRAM_IsPsramSupported(uint8_t *id);
int32_t HAL_QPIPSRAM_ReadData(struct QPI_PSRAM *psram, uint32_t from, void *buf, uint32_t len);
int32_t HAL_QPIPSRAM_ProgData(struct QPI_PSRAM *psram, uint32_t to, void *buf, uint32_t len);
uint32_t HAL_QPIPSRAM_GetCapacity(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_ReadID(struct QPI_PSRAM *psram, uint8_t *data, uint8_t len);
HAL_Status HAL_QPIPSRAM_Supsend(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_Resume(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_EnterQPI(struct QPI_PSRAM *psram);
HAL_Status HAL_QPIPSRAM_ExitQPI(struct QPI_PSRAM *psram);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_QPIPSRAM_MODULE_ENABLED */
