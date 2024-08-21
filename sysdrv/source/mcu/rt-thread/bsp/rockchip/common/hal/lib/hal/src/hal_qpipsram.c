/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_QPIPSRAM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup QPIPSRAM
 *  @{
 */

/** @defgroup QPIPSRAM_How_To_Use How To Use
 *  @{

 The PSRAM driver can be used as follows:

 - Init a SPI psram abstract:
     - Malloc struct QPI_PSRAM psram by user;
     - Designated host to struct QPI_PSRAM psram->spi by user;
     - Init spi psram abstract by calling HAL_QPIPSRAM_Init();
 - Enable or disable XIP:
     - HAL_QPIPSRAM_XIPEnable()
     - HAL_QPIPSRAM_XIPDisable()

 @} */

/** @defgroup QPIPSRAM_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
// #define HAL_QPIPSRAM_DEBUG
#ifdef HAL_QPIPSRAM_DEBUG
#define HAL_QPIPSRAM_DBG(...) HAL_DBG(__VA_ARGS__)
#else
#define HAL_QPIPSRAM_DBG(...) do { if (0) { HAL_DBG(__VA_ARGS__); } } while (0)
#endif

/*Command Set*/
#define QPIPSRAM_OP_RDID       0x9F
#define QPIPSRAM_OP_READ_1_1_1 0x0B
#define QPIPSRAM_OP_PP_1_1_1   0x02
#define QPIPSRAM_OP_READ_1_4_4 0xEB
#define QPIPSRAM_OP_PP_1_4_4   0x38
#define QPIPSRAM_OP_ENQPI      0x35
#define QPIPSRAM_OP_EIXTQPI    0xF5
#define QPIPSRAM_OP_HALFSLEEP  0xC0

#define QPIPSRAM_READWRITE_MAX_IOSIZE (1024 * 8) /* 8KB */

#define QPIPSRAM_SIZE_MAX 0x2000000 /* 32MB */
#define QPIPSRAM_SIZE_MIN 0x80000 /* 512KB */

#define QPIPSRAM_PSRAM_PAGE_SIZE_MAX 0x400 /* 1024B */
#define QPIPSRAM_PSRAM_PAGE_SIZE_MIN 0x200 /* 512B */

/********************* Private Structure Definition **************************/

struct QPI_PSRAM_ID {
    uint8_t id;
    uint8_t kgdId;
};

/********************* Private Variable Definition ***************************/

HAL_SECTION_SRAM_CODE static const struct QPI_PSRAM_ID qpiPsramId[] =
{
    { 0x0D, 0x5D },
};

/********************* Private Function Definition ***************************/

static HAL_Status QPIPSRAM_SPIMemExecOp(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    if (spi->xfer) {
        return spi->xfer(spi, op);
    } else {
        return HAL_ERROR;
    }
}

static HAL_Status QPIPSRAM_XipExecOp(struct QPIPSRAM_HOST *spi, struct HAL_SPI_MEM_OP *op, uint32_t on)
{
    if (spi->xipConfig) {
        return spi->xipConfig(spi, op, on);
    } else {
        return HAL_ERROR;
    }
}

static HAL_Status QPIPSRAM_XmmcInit(struct QPI_PSRAM *psram)
{
    struct HAL_SPI_MEM_OP opRead = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(psram->readOpcode, 1),
                                                         HAL_SPI_MEM_OP_ADDR(psram->addrWidth, 0, 1),
                                                         HAL_SPI_MEM_OP_DUMMY(psram->readDummy, 1),
                                                         HAL_SPI_MEM_OP_DATA_IN(0, NULL, 1));
    struct HAL_SPI_MEM_OP opWrite = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(psram->programOpcode, 1),
                                                          HAL_SPI_MEM_OP_ADDR(psram->addrWidth, 0, 1),
                                                          HAL_SPI_MEM_OP_DUMMY(psram->programDummy, 1),
                                                          HAL_SPI_MEM_OP_DATA_OUT(0, NULL, 1));

    /* get read transfer protocols. */
    opRead.cmd.buswidth = QPIPSRAM_GET_PROTOCOL_CMD_BITS(psram->readProto);
    opRead.addr.buswidth = QPIPSRAM_GET_PROTOCOL_ADDR_BITS(psram->readProto);
    opRead.dummy.buswidth = opRead.addr.buswidth;
    opRead.data.buswidth = QPIPSRAM_GET_PROTOCOL_DATA_BITS(psram->readProto);

    /* convert the dummy cycles to the number of bytes */
    opRead.dummy.nbytes = (psram->readDummy * opRead.dummy.buswidth) / 8;

    /* HAL_QPIPSRAM_DBG("%s %x %x %x %x\n", __func__, psram->readOpcode, psram->readDummy, opRead.dummy.buswidth, opRead.data.buswidth); */
    QPIPSRAM_XipExecOp(psram->spi, &opRead, 0);

    /* get write transfer protocols. */
    opWrite.cmd.buswidth = QPIPSRAM_GET_PROTOCOL_CMD_BITS(psram->writeProto);
    opWrite.addr.buswidth = QPIPSRAM_GET_PROTOCOL_ADDR_BITS(psram->writeProto);
    opWrite.dummy.buswidth = opWrite.addr.buswidth;
    opWrite.data.buswidth = QPIPSRAM_GET_PROTOCOL_DATA_BITS(psram->writeProto);
    opWrite.dummy.nbytes = (psram->programDummy * opWrite.dummy.buswidth) / 8;

    /* HAL_QPIPSRAM_DBG("%s %x %x %x %x\n", __func__, psram->readOpcode, psram->readDummy, opWrite.dummy.buswidth, opWrite.data.buswidth); */
    QPIPSRAM_XipExecOp(psram->spi, &opWrite, 0);

    return HAL_OK;
}

static HAL_Status QPIPSRAM_ReadWriteReg(struct QPI_PSRAM *psram, struct HAL_SPI_MEM_OP *op, void *buf)
{
    if (op->data.dir == HAL_SPI_MEM_DATA_IN) {
        op->data.buf.in = buf;
    } else {
        op->data.buf.out = buf;
    }

    return QPIPSRAM_SPIMemExecOp(psram->spi, op);
}

static HAL_Status QPIPSRAM_WriteReg(struct QPI_PSRAM *psram, uint8_t opcode, uint8_t *buf, uint32_t len)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(opcode, 1),
                                                     HAL_SPI_MEM_OP_NO_ADDR,
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_DATA_OUT(len, NULL, 1));

    /* HAL_QPIPSRAM_DBG("%s %x %ld\n", __func__, opcode, len); */

    return QPIPSRAM_ReadWriteReg(psram, &op, buf);
}

static int32_t QPIPSRAM_ReadData(struct QPI_PSRAM *psram, uint32_t from, uint32_t len, uint8_t *buf)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(psram->readOpcode, 1),
                                                     HAL_SPI_MEM_OP_ADDR(psram->addrWidth, from, 1),
                                                     HAL_SPI_MEM_OP_DUMMY(psram->readDummy, 1),
                                                     HAL_SPI_MEM_OP_DATA_IN(len, buf, 1));
    int32_t ret;

    /* get transfer protocols. */
    op.cmd.buswidth = QPIPSRAM_GET_PROTOCOL_CMD_BITS(psram->readProto);
    op.addr.buswidth = QPIPSRAM_GET_PROTOCOL_ADDR_BITS(psram->readProto);
    op.dummy.buswidth = op.addr.buswidth;
    op.data.buswidth = QPIPSRAM_GET_PROTOCOL_DATA_BITS(psram->readProto);

    /* HAL_QPIPSRAM_DBG("%s %x %lx %lx %lx\n", __func__, psram->readDummy, op.data.nbytes, from, op.addr.val); */
    /* convert the dummy cycles to the number of bytes */
    op.dummy.nbytes = (psram->readDummy * op.dummy.buswidth) / 8;

    ret = QPIPSRAM_SPIMemExecOp(psram->spi, &op);
    if (ret) {
        return 0;
    }

    return len;
}

static int32_t QPIPSRAM_WriteData(struct QPI_PSRAM *psram, uint32_t to, uint32_t len, const uint8_t *buf)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(psram->programOpcode, 1),
                                                     HAL_SPI_MEM_OP_ADDR(psram->addrWidth, to, 1),
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_DATA_OUT(len, buf, 1));
    int32_t ret;

    /* get transfer protocols. */
    op.cmd.buswidth = QPIPSRAM_GET_PROTOCOL_CMD_BITS(psram->writeProto);
    op.addr.buswidth = QPIPSRAM_GET_PROTOCOL_ADDR_BITS(psram->writeProto);
    op.data.buswidth = QPIPSRAM_GET_PROTOCOL_DATA_BITS(psram->writeProto);

    op.data.nbytes = len < op.data.nbytes ? len : op.data.nbytes;

    ret = QPIPSRAM_SPIMemExecOp(psram->spi, &op);
    if (ret) {
        return 0;
    }

    return op.data.nbytes;
}

#ifdef HAL_QPIPSRAM_HALF_SLEEP_ENABLED
static HAL_Status QPIPSRAM_HalfSleep(struct QPI_PSRAM *psram)
{
    return psram->writeReg(psram, QPIPSRAM_OP_HALFSLEEP, NULL, 0);
}
#endif

static uint32_t QPIPSRAM_PageSizeDetect(struct QPI_PSRAM *psram)
{
    uint32_t off;
    uint8_t pattern[2], read;

    for (off = QPIPSRAM_PSRAM_PAGE_SIZE_MIN; off <= QPIPSRAM_PSRAM_PAGE_SIZE_MAX; off <<= 1) {
        pattern[0] = 0;
        HAL_QPIPSRAM_ProgData(psram, 0, &pattern, 1);
        pattern[0] = 0x5a;
        pattern[1] = 0xa5;
        HAL_QPIPSRAM_ProgData(psram, off - 1, &pattern, 2);
        HAL_QPIPSRAM_ProgData(psram, 0, &read, 1);
        if (read == pattern[1]) {
            return off;
        }
    }

    return QPIPSRAM_READWRITE_MAX_IOSIZE;
}

static uint32_t QPIPSRAM_SizeDetect(struct QPI_PSRAM *psram)
{
    uint32_t size;
    uint8_t pattern, read[16], off = 0;
    uint32_t total = 0;

    for (size = QPIPSRAM_SIZE_MIN; size <= QPIPSRAM_SIZE_MAX; size <<= 1) {
        total++;
    }

    if (total > sizeof(read)) {
        return 0;
    }

    memset(read, 0, total);
    HAL_QPIPSRAM_ProgData(psram, 0, read, total);

    pattern = 0xa5;
    off = 0;
    for (size = QPIPSRAM_SIZE_MIN; size <= QPIPSRAM_SIZE_MAX; size <<= 1) {
        HAL_QPIPSRAM_ProgData(psram, size + off, &pattern, 1);
        off++;
    }

    HAL_QPIPSRAM_ReadData(psram, 0, read, total);
    HAL_DBG_HEX("r: ", read, 1, total);
    off = 0;
    for (size = QPIPSRAM_SIZE_MIN; size <= QPIPSRAM_SIZE_MAX; size <<= 1) {
        /* HAL_QPIPSRAM_DBG("%x %x %x %x\n", pattern[0], pattern[1], read[off], read[off + 1]); */
        if (read[off] == pattern) {
            return size;
        }
        off++;
    }

    return 0;
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup QPISPRAM_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 *  @{
 */

/**
 * @brief  QPIPsram suspend.
 * @param  psram: psram dev.
 * @return HAL_Status
 */
HAL_Status HAL_QPIPSRAM_Supsend(struct QPI_PSRAM *psram)
{
    HAL_Status ret = HAL_OK;

    if (psram->spi->mode & HAL_SPI_XIP) {
        ret = HAL_QPIPSRAM_XIPDisable(psram);
        if (ret) {
            return ret;
        }
    }

#ifdef HAL_QPIPSRAM_HALF_SLEEP_ENABLED
    if (psram->readProto == QPIPSRAM_PROTO_4_4_4 &&
        psram->writeProto == QPIPSRAM_PROTO_4_4_4) {
        ret = HAL_QPIPSRAM_ExitQPI(psram);
        if (ret) {
            return ret;
        }

        ret = QPIPSRAM_HalfSleep(psram);
    }
#endif

    return ret;
}

/**
 * @brief  QPIPsram resume.
 * @param  psram: psram dev.
 * @return HAL_Status
 */
HAL_Status HAL_QPIPSRAM_Resume(struct QPI_PSRAM *psram)
{
    HAL_Status ret = HAL_OK;

#ifdef HAL_QPIPSRAM_HALF_SLEEP_ENABLED
    if (psram->readProto == QPIPSRAM_PROTO_4_4_4 &&
        psram->writeProto == QPIPSRAM_PROTO_4_4_4) {
        psram->writeReg(psram, 0xff, NULL, 0);
        HAL_CPUDelayUs(150);

        ret = HAL_QPIPSRAM_EnterQPI(psram);
        if (ret) {
            return ret;
        }
    }
#endif

    if (psram->spi->mode & HAL_SPI_XIP) {
        ret = HAL_QPIPSRAM_XIPEnable(psram);
    }

    return ret;
}

/** @} */

/** @defgroup QPIPSRAM_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  QPIPsram read data in SPI mode.
 * @param  psram: psram dev.
 * @param  from: byte address.
 * @param  buf: source address.
 * @param  len: number of bytes.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
int32_t HAL_QPIPSRAM_ReadData(struct QPI_PSRAM *psram, uint32_t from, void *buf, uint32_t len)
{
    int32_t ret;
    uint8_t *pBuf = (uint8_t *)buf;
    uint32_t size, remain = len;
    uint32_t off;

    /* HAL_QPIPSRAM_DBG("%s from 0x%08lx, len %lx\n", __func__, from, len); */
    if (from >= psram->size || len > psram->size || (from + len) > psram->size) {
        return HAL_INVAL;
    }

    off = from & (psram->pageSize - 1);

    while (remain) {
        size = HAL_MIN(psram->pageSize, remain);
        if (off && ((off + size) > psram->pageSize)) {
            size = psram->pageSize - off;
            off = 0;
        }
        ret = psram->read(psram, from, size, pBuf);
        if (ret != (int32_t)size) {
            HAL_QPIPSRAM_DBG("%s %lu ret= %ld %ld\n", __func__, from >> 9, ret, (int32_t)size);

            return ret;
        }
        remain -= size;
        from += size;
        pBuf += size;
    }

    return len;
}

/**
 * @brief  QPIPsram write data in SPI mode.
 * @param  psram: psram dev.
 * @param  to: byte address.
 * @param  buf: source address.
 * @param  len: number of bytes.
 * @return If the transfer is successful, return the transfer length, or error code.
 */
int32_t HAL_QPIPSRAM_ProgData(struct QPI_PSRAM *psram, uint32_t to, void *buf, uint32_t len)
{
    int32_t ret;
    uint8_t *pBuf = (uint8_t *)buf;
    uint32_t size, remain = len;
    uint32_t off;

    /* HAL_QPIPSRAM_DBG("%s to 0x%08lx, len %lx\n", __func__, to, len); */
    if (to >= psram->size || len > psram->size || (to + len) > psram->size) {
        return HAL_INVAL;
    }

    off = to & (psram->pageSize - 1);

    while (remain) {
        size = HAL_MIN(psram->pageSize, remain);
        if (off && ((off + size) > psram->pageSize)) {
            size = psram->pageSize - off;
            off = 0;
        }
        ret = psram->write(psram, to, size, pBuf);
        if (ret != (int32_t)size) {
            HAL_QPIPSRAM_DBG("%s %lu ret= %ld %ld\n", __func__, to >> 9, ret, (int32_t)size);

            return ret;
        }
        remain -= size;
        to += size;
        pBuf += size;
    }

    return len;
}

/** @} */

/** @defgroup QPIPSRAM_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Psram init.
 * @param  psram: psram dev.
 * @return HAL_Status
 */
HAL_Status HAL_QPIPSRAM_Init(struct QPI_PSRAM *psram)
{
    uint8_t idByte[3];

    if (!psram->spi) {
        HAL_QPIPSRAM_DBG("%s no host\n", __func__);

        return HAL_INVAL;
    }

    psram->read = QPIPSRAM_ReadData;
    psram->write = QPIPSRAM_WriteData;
    psram->writeReg = QPIPSRAM_WriteReg;

#ifdef HAL_QPIPSRAM_HALF_SLEEP_ENABLED
    uint32_t loop = 15;

    psram->writeReg(psram, 0xff, NULL, 0);
reinit:
    loop--;
#endif

    HAL_QPIPSRAM_ExitQPI(psram);
    HAL_QPIPSRAM_ReadID(psram, idByte, sizeof(idByte));
    HAL_QPIPSRAM_DBG("QPIPsram ID: %x %x %x\n", idByte[0], idByte[1], idByte[2]);
    psram->id[0] = idByte[0];
    psram->id[1] = idByte[1];

    if (!HAL_QPIPSRAM_IsPsramSupported(idByte)) {
#ifdef HAL_QPIPSRAM_HALF_SLEEP_ENABLED
        if (loop) {
            HAL_CPUDelayUs(10);
            goto reinit;
        }
#endif

        return HAL_NODEV;
    }

    /* Temporarily fixed configuration */
    if (psram->spi->mode & HAL_SPI_TX_QUAD &&
        psram->spi->mode & HAL_SPI_RX_QUAD) {
        psram->addrWidth = 3;
        psram->readOpcode = QPIPSRAM_OP_READ_1_4_4;
        if (psram->spi->mode & HAL_SPI_XIP) {
            psram->readProto = QPIPSRAM_PROTO_4_4_4;
        } else {
            psram->readProto = QPIPSRAM_PROTO_1_4_4;
        }
        psram->readDummy = 6;
        psram->programOpcode = QPIPSRAM_OP_PP_1_4_4;
        if (psram->spi->mode & HAL_SPI_XIP) {
            psram->writeProto = QPIPSRAM_PROTO_4_4_4;
        } else {
            psram->writeProto = QPIPSRAM_PROTO_1_4_4;
        }
        psram->programDummy = 0;
    } else {
        psram->addrWidth = 3;
        psram->readOpcode = QPIPSRAM_OP_READ_1_1_1;
        psram->readProto = QPIPSRAM_PROTO_1_1_1;
        psram->readDummy = 8;
        psram->programOpcode = QPIPSRAM_OP_PP_1_1_1;
        psram->writeProto = QPIPSRAM_PROTO_1_1_1;
        psram->programDummy = 0;
    }

    if (psram->readProto == QPIPSRAM_PROTO_4_4_4 &&
        psram->writeProto == QPIPSRAM_PROTO_4_4_4) {
        HAL_QPIPSRAM_EnterQPI(psram);
    } else {
        HAL_QPIPSRAM_ExitQPI(psram);
    }

    switch (idByte[2] >> 5) {
    case 7:
        psram->size = 0x100000;
        psram->pageSize = 512;
        break;
    case 0:
        psram->size = 0x200000;
        psram->pageSize = 512;
        break;
    case 1:
        psram->size = 0x400000;
        psram->pageSize = 1024;
        break;
    case 2:
        psram->size = 0x800000;
        psram->pageSize = 1024;
        break;
    default:
        psram->size = QPIPSRAM_SIZE_MAX * 2;
        psram->pageSize = QPIPSRAM_READWRITE_MAX_IOSIZE;
        if (!(psram->spi->mode & HAL_SPI_XIP)) {
            psram->size = QPIPSRAM_SizeDetect(psram);
            psram->pageSize = QPIPSRAM_PageSizeDetect(psram);
        }
    }

    HAL_QPIPSRAM_DBG("QPIPsram size= 0x%lx Bytes\n", psram->size);
    HAL_QPIPSRAM_DBG("QPIPsram page size= 0x%lx Bytes\n", psram->pageSize);

    QPIPSRAM_XmmcInit(psram);
    if (psram->spi->mode & HAL_SPI_XIP) {
        HAL_QPIPSRAM_XIPEnable(psram);
    }

    return HAL_OK;
}

/**
 * @brief  Psram deinit.
 * @param  psram: psram dev.
 * @return HAL_Status
 */
HAL_Status HAL_QPIPSRAM_DeInit(struct QPI_PSRAM *psram)
{
    /* TO-DO*/

    return HAL_OK;
}

/** @} */

/** @defgroup QPIPSRAM_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Psram XIP mode enable.
 * @param  psram: psram dev.
 * @return HAL_Status
 * Access data in memory map mode.
 *  Adding half sleep resume cs# and clock redundancy.
 */
HAL_Status HAL_QPIPSRAM_XIPEnable(struct QPI_PSRAM *psram)
{
    return QPIPSRAM_XipExecOp(psram->spi, NULL, 1);
}

/**
 * @brief  Psram XIP mode disable.
 * @param  psram: psram dev.
 * @return HAL_Status
 * Adding half sleep suspend try.
 */
HAL_Status HAL_QPIPSRAM_XIPDisable(struct QPI_PSRAM *psram)
{
    return QPIPSRAM_XipExecOp(psram->spi, NULL, 0);
}

/**
 * @brief  Check if psram support.
 * @param  id: psram id.
 * @return HAL_Check.
 */
HAL_Check HAL_QPIPSRAM_IsPsramSupported(uint8_t *id)
{
    uint32_t i;

    if (!id) {
        return HAL_FALSE;
    }

    for (i = 0; i < HAL_ARRAY_SIZE(qpiPsramId); i++) {
        if ((id[0] == qpiPsramId[i].id) &&
            (id[1] == qpiPsramId[i].kgdId)) {
            return HAL_TRUE;
        }
    }

    return HAL_FALSE;
}

/**
 * @brief  Get psram capacity.
 * @param  psram: psram dev.
 * @return uint32_t: psram capacity, n bytes.
 */
uint32_t HAL_QPIPSRAM_GetCapacity(struct QPI_PSRAM *psram)
{
    return psram->size;
}

/**
 * @brief  Get psram capacity.
 * @param  psram: psram dev.
 * @param  data: id buffer
 * @param  len: id length
 * @return uint32_t: psram capacity, n bytes.
 */
HAL_Status HAL_QPIPSRAM_ReadID(struct QPI_PSRAM *psram, uint8_t *data, uint8_t len)
{
    HAL_Status ret;
    uint8_t *id = data;
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(QPIPSRAM_OP_RDID, 1),
                                                     HAL_SPI_MEM_OP_ADDR(3, 0xa5a5a5a5, 1),
                                                     HAL_SPI_MEM_OP_DUMMY(0, 1),
                                                     HAL_SPI_MEM_OP_DATA_IN(len, data, 1));

    /* get transfer protocols. */
    op.cmd.buswidth = 1;
    op.addr.buswidth = 1;
    op.dummy.buswidth = 1;
    op.data.buswidth = 1;

    op.dummy.nbytes = 0;

    ret = QPIPSRAM_SPIMemExecOp(psram->spi, &op);
    if (ret) {
        HAL_QPIPSRAM_DBG("error reading JEDEC ID%x %x\n", id[0], id[1]);

        return HAL_ERROR;
    }

    return ret;
}

/**
 * @brief  Enter QPI mode.
 * @param  psram: psram dev.
 * @return HAL_Status
 */
HAL_Status HAL_QPIPSRAM_EnterQPI(struct QPI_PSRAM *psram)
{
    return psram->writeReg(psram, QPIPSRAM_OP_ENQPI, NULL, 0);
}

/**
 * @brief  Exit QPI mode.
 * @param  psram: psram dev.
 * @return HAL_Status
 */
HAL_Status HAL_QPIPSRAM_ExitQPI(struct QPI_PSRAM *psram)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(QPIPSRAM_OP_EIXTQPI, 4),
                                                     HAL_SPI_MEM_OP_NO_ADDR,
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_NO_DATA);

    return QPIPSRAM_SPIMemExecOp(psram->spi, &op);
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_QPIPSRAM_MODULE_ENABLED */
