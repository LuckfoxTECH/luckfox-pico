/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SPINAND_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SPINAND
 *  @{
 */

/** @defgroup SPINAND_How_To_Use How To Use
 *  @{

 HAL_SPINAND is the SPI Nand protocol layer.

 **HAL_SPINAND  controller host supported feature**

 - X1, X4 transfer
 - choose SPI mode

 **The SPINAND  driver can be used as follows**

 Register Host:

 - Init a SPI Nor abstract:
   - Malloc struct SPI_NAND  nor by user;
   - Designated host to SPI_NAND_HOST,  define a suitable spi->mode;
   - Init SPI Nor abstract by calling HAL_SPINAND _Init();

 SPI Nor operations for user:

 - Read, write and erase function groups  like HAL_SPINAND_PageReadRaw();

 **Add New SPI Nand flash**

  Figure out following points for adding a new SPI Nor flash to flash info table:

 - flash array
 - standard SPI (DI / DO), Quad SPI (D0 / D1/ D2 /D3) command
 - feature
   - [3, 2] quad r/w;
 - QE bit
 - META_AREA
 - ECC bits and the way to get ECC status

 @} */

/** @defgroup SPINAND_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
// #define HAL_SPINAND_DEBUG
#ifdef HAL_SPINAND_DEBUG
#define HAL_SPINAND_DBG(...) HAL_DBG(__VA_ARGS__)
#else
#define HAL_SPINAND_DBG(...) do { if (0) { HAL_DBG(__VA_ARGS__); } } while (0)
#endif

/* FLASH_INFO feature */
#define FEA_4BIT_READ                 HAL_BIT(2)
#define FEA_4BIT_PROG                 HAL_BIT(3)
#define FEA_QUAD_LINE_TRANSFER_PRESET HAL_BIT(6)

#define SPINAND_WR_EN_DIS_OP(enable)                              \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD((enable) ? 0x06 : 0x04, 1), \
            HAL_SPI_MEM_OP_NO_ADDR,                               \
            HAL_SPI_MEM_OP_NO_DUMMY,                              \
            HAL_SPI_MEM_OP_NO_DATA)

#define SPINAND_READID_OP(ndummy, buf, len)     \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD(0x9f, 1), \
            HAL_SPI_MEM_OP_ADDR(1, 0, 1),       \
            HAL_SPI_MEM_OP_NO_DUMMY,            \
            HAL_SPI_MEM_OP_DATA_IN(len, buf, 1))

#define SPINAND_SET_FEATURE_OP(reg, valptr)     \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD(0x1f, 1), \
            HAL_SPI_MEM_OP_ADDR(1, reg, 1),     \
            HAL_SPI_MEM_OP_NO_DUMMY,            \
            HAL_SPI_MEM_OP_DATA_OUT(1, valptr, 1))

#define SPINAND_GET_FEATURE_OP(reg, valptr)     \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD(0x0f, 1), \
            HAL_SPI_MEM_OP_ADDR(1, reg, 1),     \
            HAL_SPI_MEM_OP_NO_DUMMY,            \
            HAL_SPI_MEM_OP_DATA_IN(1, valptr, 1))

#define SPINAND_PAGE_READ_OP(addr)              \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD(0x13, 1), \
            HAL_SPI_MEM_OP_ADDR(3, addr, 1),    \
            HAL_SPI_MEM_OP_NO_DUMMY,            \
            HAL_SPI_MEM_OP_NO_DATA)

#define SPINAND_PAGE_READ_FROM_CACHE(dataLines, addr, buf, len)           \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD((dataLines == 4 ? 0x6b : 0x03), 1), \
            HAL_SPI_MEM_OP_ADDR(3, addr << 8, 1),                         \
            HAL_SPI_MEM_OP_NO_DUMMY,                                      \
            HAL_SPI_MEM_OP_DATA_IN(len, buf, dataLines))

#define SPINAND_PROG_EXEC_OP(addr)              \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD(0x10, 1), \
           HAL_SPI_MEM_OP_ADDR(3, addr, 1),     \
           HAL_SPI_MEM_OP_NO_DUMMY,             \
           HAL_SPI_MEM_OP_NO_DATA)

#define SPINAND_PROG_LOAD_TO_CACHE(dataLines, addr, buf, len)             \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD((dataLines == 4 ? 0x32 : 0x02), 1), \
           HAL_SPI_MEM_OP_ADDR(2, addr, 1),                               \
           HAL_SPI_MEM_OP_NO_DUMMY,                                       \
           HAL_SPI_MEM_OP_DATA_OUT(len, buf, dataLines))

#define SPINAND_BLK_ERASE_OP(addr)              \
    HAL_SPI_MEM_OP(HAL_SPI_MEM_OP_CMD(0xd8, 1), \
           HAL_SPI_MEM_OP_ADDR(3, addr, 1),     \
           HAL_SPI_MEM_OP_NO_DUMMY,             \
           HAL_SPI_MEM_OP_NO_DATA)

/* SPINAND_INFO feature */
#define SPINAND_FEA_4BIT_READ HAL_BIT(2)
#define SPINAND_FEA_4BIT_PROG HAL_BIT(3)

/***************************** Structure Definition **************************/

/* SPI_NAND devices info */
struct SPINAND_INFO {
    uint32_t id;

    uint16_t secPerPage;
    uint16_t pagePerBlk;
    uint16_t planePerDie;
    uint16_t blkPerPlane;

    uint8_t feature;

    uint8_t density;  /* capacity in sector = (1 << density), 512B/sector */
    uint8_t maxECCBits;
    uint8_t hasQeBit;

    struct META_AREA meta;
    int32_t (*eccStatus)(struct SPI_NAND *spinand);
};

/********************* Private Variable Definition ***************************/

static int32_t SPINAND_GetEccStatus0(struct SPI_NAND *spinand);
static int32_t SPINAND_GetEccStatus1(struct SPI_NAND *spinand);
static int32_t SPINAND_GetEccStatus2(struct SPI_NAND *spinand);
static int32_t SPINAND_GetEccStatus3(struct SPI_NAND *spinand);
static int32_t SPINAND_GetEccStatus4(struct SPI_NAND *spinand);
static int32_t SPINAND_GetEccStatus5(struct SPI_NAND *spinand);

static const struct SPINAND_INFO s_spiNandTable[] = {
    /* TC58CVG0S0HxAIx */
    { 0x98C2, 4, 0x40, 1, 1024, 0x00, 18, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* TC58CVG1S0HxAIx */
    { 0x98CB, 4, 0x40, 2, 1024, 0x00, 19, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* MX35LF1GE4AB */
    { 0xC212, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* MX35LF2GE4AB */
    { 0xC222, 4, 0x40, 2, 1024, 0x0C, 19, 0x4, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* GD5F1GQ4UAYIG */
    { 0xC8F1, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* MT29F1G01ZAC */
    { 0x2C12, 4, 0x40, 1, 1024, 0x00, 18, 0x1, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* GD5F2GQ40BY2GR */
    { 0xC8D2, 4, 0x40, 2, 1024, 0x0C, 19, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus3 },
    /* GD5F1GQ4RB9IGR */
    { 0xC8D1, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus3 },
    /* IS37SML01G1 */
    { 0xC821, 4, 0x40, 1, 1024, 0x00, 18, 0x1, 0, { 0x08, 0x0C, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* W25N01GV */
    { 0xEFAA, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 0, { 0x04, 0x14, 0x24, 0xFF }, &SPINAND_GetEccStatus1 },
    /* HYF2GQ4UAACAE */
    { 0xC952, 4, 0x40, 1, 2048, 0x4C, 19, 0xE, 1, { 0x04, 0x24, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* HYF2GQ4UDACAE */
    { 0xC922, 4, 0x40, 1, 2048, 0x4C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* HYF2GQ4UHCCAE */
    { 0xC95A, 4, 0x40, 1, 2048, 0x4C, 19, 0xE, 1, { 0x04, 0x24, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* HYF1GQ4UDACAE */
    { 0xC921, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* F50L1G41LB */
    { 0xC801, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 0, { 0x14, 0x24, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* XT26G02A */
    { 0x0BE2, 4, 0x40, 1, 2048, 0x4C, 19, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &SPINAND_GetEccStatus4 },
    /* XT26G01A */
    { 0x0BE1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &SPINAND_GetEccStatus4 },
    /* FS35ND01G-S1 */
    { 0xCDB1, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x10, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus5 },
    /* FS35ND02G-S2 */
    { 0xCDA2, 4, 0x40, 1, 2048, 0x00, 19, 0x4, 0, { 0x10, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus5 },
    /* DS35Q1GA-1B */
    { 0xE571, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* DS35Q2GA-1B */
    { 0xE572, 4, 0x40, 2, 1024, 0x0C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* EM73C044SNC-G */
    { 0xD522, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* EM73D044SNB-G */
    { 0xD520, 4, 0x40, 1, 2048, 0x0C, 19, 0x8, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* ATO25D1GA */
    { 0x9B12, 4, 0x40, 1, 1024, 0x40, 18, 0x1, 1, { 0x14, 0x24, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* XT26G02B */
    { 0x0BF2, 4, 0x40, 1, 2048, 0x4C, 19, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &SPINAND_GetEccStatus5 },
    /* XT26G01B */
    { 0x0BF1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &SPINAND_GetEccStatus4 },
    /* HYF4GQ4UAACBE */
    { 0xC9D4, 8, 0x40, 1, 2048, 0x4C, 20, 0x4, 1, { 0x20, 0x40, 0x24, 0x44 }, &SPINAND_GetEccStatus0 },
    /* FM25S01 */
    { 0xA1A1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x00, 0x04, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* HYF1GQ4UPACAE */
    { 0xC9A1, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* EM73E044SNA-G */
    { 0xD503, 8, 0x40, 1, 2048, 0x4C, 20, 0x8, 1, { 0x04, 0x28, 0x08, 0x2C }, &SPINAND_GetEccStatus0 },
    /* GD5F2GQ5UEYIG */
    { 0xC852, 4, 0x40, 1, 2048, 0x4C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus2 },
    /* GD5F1GQ4R */
    { 0xC8C1, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus3 },
    /* TC58CVG2S0HRAIJ */
    { 0x98ED, 8, 0x40, 1, 2048, 0x0C, 20, 0x8, 0, { 0x04, 0x0C, 0x08, 0x10 }, &SPINAND_GetEccStatus0 },
    /* TC58CVG1S3HRAIJ */
    { 0x98EB, 4, 0x40, 1, 2048, 0x0C, 19, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* TC58CVG0S3HRAIJ */
    { 0x98E2, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* XT26G04A */
    { 0x0BE3, 4, 0x80, 1, 2048, 0x4C, 20, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &SPINAND_GetEccStatus4 },
    /* FS35ND01G-S1Y2 */
    { 0xCDEA, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 0, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* DS35M1GA-1B */
    { 0xE521, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* W25N01GW */
    { 0xEFBA, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 0, { 0x04, 0x14, 0x24, 0xFF }, &SPINAND_GetEccStatus1 },
    /* DS35M2GA-1B */
    { 0xE522, 4, 0x40, 2, 1024, 0x0C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus1 },
    /* MX35UF1GE4AD */
    { 0xC296, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &SPINAND_GetEccStatus0 },
    /* GD5F1GQ5REYIG */
    { 0xC841, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus2 },
    /* GD5F1GM7RExxG */
    { 0xC881, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x14, 0xFF, 0xFF }, &SPINAND_GetEccStatus2 },
};

/********************* Private Function Definition ***************************/
static HAL_Status SPINAND_SPIMemExecOp(struct SPI_NAND_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    if (spi->xfer) {
        return spi->xfer(spi, op);
    } else {
        return HAL_ERROR;
    }
}

static HAL_Status SPINAND_ReadRegOp(struct SPI_NAND *spinand, uint8_t reg, uint8_t *val)
{
    struct HAL_SPI_MEM_OP op = SPINAND_GET_FEATURE_OP(reg, val);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static HAL_Status SPINAND_WriteRegOp(struct SPI_NAND *spinand, uint8_t reg, uint8_t val)
{
    struct HAL_SPI_MEM_OP op = SPINAND_SET_FEATURE_OP(reg, &val);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static int SPINAND_ReadStatus(struct SPI_NAND *spinand, uint8_t reg, uint8_t *status)
{
    return SPINAND_ReadRegOp(spinand, reg, status);
}

static int SPINAND_WriteStatus(struct SPI_NAND *spinand, uint8_t reg, uint8_t status)
{
    return SPINAND_WriteRegOp(spinand, reg, status);
}

/*
 * ecc default:
 * ecc bits: 0xC0[4,5]
 * 0x00, No bit errors were detected
 * 0x01, Bit errors were detected and corrected.
 * 0x10, Multiple bit errors were detected and not corrected.
 * 0x11, Bits errors were detected and corrected, bit error count
 *	exceed the bit flip detection threshold
 */
static int32_t SPINAND_GetEccStatus0(struct SPI_NAND *spinand)
{
    int32_t ret;
    uint8_t ecc, status;

    ret = SPINAND_ReadStatus(spinand, 0xC0, &status);
    if (ret) {
        return SPINAND_ECC_ERROR;
    }

    ecc = (status >> 4) & 0x03;

    if (ecc <= 1) {
        ret = SPINAND_ECC_OK;
    } else if (ecc == 2) {
        ret = SPINAND_ECC_ERROR;
    } else {
        ret = SPINAND_ECC_REFRESH;
    }

    HAL_SPINAND_DBG("%s C0 %x\n", __func__, status);

    return ret;
}

/*
 * ecc spectial type1:
 * ecc bits: 0xC0[4,5]
 * 0x00, No bit errors were detected;
 * 0x01, Bits errors were detected and corrected, bit error count
 *	may reach the bit flip detection threshold;
 * 0x10, Multiple bit errors were detected and not corrected;
 * 0x11, Reserved.
 */
static int32_t SPINAND_GetEccStatus1(struct SPI_NAND *spinand)
{
    int32_t ret;
    uint8_t ecc, status;

    ret = SPINAND_ReadStatus(spinand, 0xC0, &status);
    if (ret) {
        return SPINAND_ECC_ERROR;
    }

    ecc = (status >> 4) & 0x03;

    if (ecc == 0) {
        ret = SPINAND_ECC_OK;
    } else if (ecc == 1) {
        ret = SPINAND_ECC_REFRESH;
    } else {
        ret = SPINAND_ECC_ERROR;
    }

    HAL_SPINAND_DBG("%s C0 %x\n", __func__, status);

    return ret;
}

/*
 * ecc spectial type2:
 * ecc bits: 0xC0[4,5] 0xF0[4,5]
 * [0x0000, 0x0011], No bit errors were detected;
 * [0x0100, 0x0111], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0x1000, 0x1011], Multiple bit errors were detected and
 *	not corrected.
 * [0x1100, 0x1111], reserved.
 */
static int32_t SPINAND_GetEccStatus2(struct SPI_NAND *spinand)
{
    int32_t ret;
    uint8_t ecc, status, status1;

    if (SPINAND_ReadStatus(spinand, 0xC0, &status) != HAL_OK) {
        return SPINAND_ECC_ERROR;
    }
    if (SPINAND_ReadStatus(spinand, 0xF0, &status1) != HAL_OK) {
        return SPINAND_ECC_ERROR;
    }

    ecc = (status >> 4) & 0x03;
    ecc = (ecc << 2) | ((status1 >> 4) & 0x03);

    if (ecc < 7) {
        ret = SPINAND_ECC_OK;
    } else if (ecc == 7) {
        ret = SPINAND_ECC_REFRESH;
    } else {
        ret = SPINAND_ECC_ERROR;
    }

    return ret;
}

/*
 * ecc spectial type3:
 * ecc bits: 0xC0[4,5] 0xF0[4,5]
 * [0x0000, 0x0011], No bit errors were detected;
 * [0x0100, 0x0111], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0x1000, 0x1011], Multiple bit errors were detected and
 *	not corrected.
 * [0x1100, 0x1111], Bit error count equals the bit flip
 *	detectio nthreshold
 */
static int32_t SPINAND_GetEccStatus3(struct SPI_NAND *spinand)
{
    int32_t ret;
    uint8_t ecc, status, status1;

    if (SPINAND_ReadStatus(spinand, 0xC0, &status) != HAL_OK) {
        return SPINAND_ECC_ERROR;
    }
    if (SPINAND_ReadStatus(spinand, 0xF0, &status1) != HAL_OK) {
        return SPINAND_ECC_ERROR;
    }

    ecc = (status >> 4) & 0x03;
    ecc = (ecc << 2) | ((status1 >> 4) & 0x03);

    if (ecc < 7) {
        ret = SPINAND_ECC_OK;
    } else if (ecc == 7 || ecc >= 12) {
        ret = SPINAND_ECC_REFRESH;
    } else {
        ret = SPINAND_ECC_ERROR;
    }

    HAL_SPINAND_DBG("%s C0 %x, F0 %x\n", __func__, status, status1);

    return ret;
}

/*
 * ecc spectial type4:
 * ecc bits: 0xC0[2,5]
 * [0x0000], No bit errors were detected;
 * [0x0001, 0x0111], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0x1000], Multiple bit errors were detected and
 *	not corrected.
 * [0x1100], Bit error count equals the bit flip
 *	detectionthreshold
 * else, reserved
 */
static int32_t SPINAND_GetEccStatus4(struct SPI_NAND *spinand)
{
    int32_t ret;
    uint8_t ecc, status;

    ret = SPINAND_ReadStatus(spinand, 0xC0, &status);
    if (ret) {
        return SPINAND_ECC_ERROR;
    }

    ecc = (status >> 2) & 0x0f;
    if (ecc < 7) {
        ret = SPINAND_ECC_OK;
    } else if (ecc == 7 || ecc == 12) {
        ret = SPINAND_ECC_REFRESH;
    } else {
        ret = SPINAND_ECC_ERROR;
    }

    HAL_SPINAND_DBG("%s C0 %x\n", __func__, status);

    return ret;
}

/*
 * ecc spectial type5:
 * ecc bits: 0xC0[4,6]
 * [0x0], No bit errors were detected;
 * [0x001, 0x011], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0x100], Bit error count equals the bit flip
 *	detectionthreshold
 * [0x101, 0x110], Reserved;
 * [0x111], Multiple bit errors were detected and
 *	not corrected.
 */
static int32_t SPINAND_GetEccStatus5(struct SPI_NAND *spinand)
{
    int32_t ret;
    uint8_t ecc, status;

    ret = SPINAND_ReadStatus(spinand, 0xC0, &status);
    if (ret) {
        return SPINAND_ECC_ERROR;
    }

    ecc = (status >> 4) & 0x07;

    if (ecc < 4) {
        ret = SPINAND_ECC_OK;
    } else if (ecc == 4) {
        ret = SPINAND_ECC_REFRESH;
    } else {
        ret = SPINAND_ECC_ERROR;
    }

    HAL_SPINAND_DBG("%s C0 %x\n", __func__, status);

    return ret;
}

static HAL_Status SPINAND_WaitBusy(struct SPI_NAND *spinand, uint8_t *data, uint64_t timeout)
{
    int32_t ret;
    uint32_t i;
    uint8_t status;

    /* HAL_SPINAND_DBG("%s %lx\n", __func__, timeout); */
    *data = 0;
    for (i = 0; i < timeout; i++) {
        ret = SPINAND_ReadStatus(spinand, 0xC0, &status);
        if (ret != HAL_OK) {
            return ret;
        }

        *data = status;
        if ((status & 0x01) == 0) {
            return HAL_OK;
        }

        HAL_DelayUs(1);
    }
    HAL_SPINAND_DBG("%s error %ld\n", __func__, timeout);

    return HAL_BUSY;
}

static HAL_Status SPINAND_LoadPageOp(struct SPI_NAND *spinand,
                                     uint32_t row)
{
    struct HAL_SPI_MEM_OP op = SPINAND_PAGE_READ_OP(row);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static HAL_Status SPINAND_ReadFromCacheOp(struct SPI_NAND *spinand,
                                          uint32_t addrMix,
                                          uint32_t *buf,
                                          uint32_t len)
{
    struct HAL_SPI_MEM_OP op = SPINAND_PAGE_READ_FROM_CACHE(spinand->readLines,
                                                            addrMix, buf, len);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static HAL_Status SPINAND_ProgOp(struct SPI_NAND *spinand,
                                 uint32_t row)
{
    struct HAL_SPI_MEM_OP op = SPINAND_PROG_EXEC_OP(row);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static HAL_Status SPINAND_WriteToCacheOp(struct SPI_NAND *spinand,
                                         uint32_t addrMix,
                                         uint32_t *buf,
                                         uint32_t len)
{
    struct HAL_SPI_MEM_OP op = SPINAND_PROG_LOAD_TO_CACHE(spinand->progLines,
                                                          addrMix, buf, len);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static HAL_Status SPINAND_EraseOp(struct SPI_NAND *spinand, uint32_t row)
{
    struct HAL_SPI_MEM_OP op = SPINAND_BLK_ERASE_OP(row);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static HAL_Status SPINAND_QuadEnable(struct SPI_NAND *spinand)
{
    int ret = HAL_OK;
    uint8_t status;

    ret = SPINAND_ReadStatus(spinand, 0xB0, &status);
    if (ret != HAL_OK) {
        return ret;
    }

    status |= 1;

    return SPINAND_WriteStatus(spinand, 0xB0, status);
}

static HAL_Status SPINAND_WriteEnableOp(struct SPI_NAND *spinand)
{
    struct HAL_SPI_MEM_OP op = SPINAND_WR_EN_DIS_OP(true);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

static const struct SPINAND_INFO *SPINAND_GerFlashInfo(uint8_t *flashId)
{
    uint32_t i;
    uint32_t id = (flashId[0] << 8) | (flashId[1] << 0);

    for (i = 0; i < HAL_ARRAY_SIZE(s_spiNandTable); i++) {
        if (s_spiNandTable[i].id == id) {
            return &s_spiNandTable[i];
        }
    }

    return NULL;
}

/********************* Public Function Definition ****************************/

/** @defgroup SPINAND_Exported_Functions_Group3 IO Functions
 *  @{
 */

/**
 * @brief  SPI Nand block erase.
 * @param  spinand: SPI_NAND dev.
 * @param  addr: page address.
 * @return HAL_Status.
 */
HAL_Status HAL_SPINAND_EraseBlock(struct SPI_NAND *spinand, uint32_t addr)
{
    uint8_t status;
    int ret;

    HAL_SPINAND_DBG("%s %x\n", __func__, addr);

    ret = SPINAND_WriteEnableOp(spinand);
    if (ret) {
        return ret;
    }

    ret = SPINAND_EraseOp(spinand, addr);
    if (ret) {
        return ret;
    }

    ret = SPINAND_WaitBusy(spinand, &status, 1000 * 1000);
    if (ret) {
        return ret;
    }

    if (status & SPINAND_ERASE_FAIL) {
        return SPINAND_ERASE_FAIL;
    }

    return ret;
}

/**
 * @brief  SPI Nand raw page read.
 * @param  spinand: SPI_NAND dev.
 * @param  addr: page address.
 * @param  pData: destination address.
 * @param  oob: whether pData including oob data.
 * @return return value < 0: transfer error, return HAL_Status;
 *   return value >= 0: transfer ok, return ecc status, check SPINAND_ECC_XXXX;
 */
int32_t HAL_SPINAND_ReadPageRaw(struct SPI_NAND *spinand, uint32_t addr, void *pData, bool oob)
{
    uint8_t status;
    int ret;
    uint32_t addrMix, size;

    HAL_SPINAND_DBG("%s %x\n", __func__, addr);

    ret = SPINAND_LoadPageOp(spinand, addr);
    if (ret) {
        return ret;
    }

    ret = SPINAND_WaitBusy(spinand, &status, 1000 * 1000);
    if (ret < 0) {
        return ret;
    }

    addrMix = spinand->planePerDie == 1 ? 0 : ((addr >> 6) & 0x1) << 12;
    size = oob ? spinand->secPerPage * SPINAND_SECTOR_FULL_SIZE : spinand->secPerPage * SPINAND_SECTOR_SIZE;
    ret = SPINAND_ReadFromCacheOp(spinand, addrMix, pData, size);
    if (ret) {
        return ret;
    }

    return spinand->eccStatus(spinand);
}

/**
 * @brief  SPI Nand raw page program.
 * @param  spinand: SPI_NAND dev.
 * @param  addr: page address.
 * @param  pData: source address.
 * @param  oob: whether pData including oob data.
 * @return HAL_Status.
 */
HAL_Status HAL_SPINAND_ProgPageRaw(struct SPI_NAND *spinand, uint32_t addr, const void *pData, bool oob)
{
    uint8_t status;
    int ret;
    uint32_t addrMix, size;

    HAL_SPINAND_DBG("%s %x\n", __func__, addr);

    ret = SPINAND_WriteEnableOp(spinand);
    if (ret) {
        return ret;
    }

    addrMix = spinand->planePerDie == 1 ? 0 : ((addr >> 6) & 0x1) << 12;
    size = oob ? spinand->secPerPage * SPINAND_SECTOR_FULL_SIZE : spinand->secPerPage * SPINAND_SECTOR_SIZE;
    ret = SPINAND_WriteToCacheOp(spinand, addrMix, (uint32_t *)pData, size);
    if (ret) {
        return ret;
    }

    ret = SPINAND_ProgOp(spinand, addr);
    if (ret) {
        return ret;
    }

    ret = SPINAND_WaitBusy(spinand, &status, 1000 * 1000);
    if (ret) {
        return ret;
    }

    if (status & SPINAND_PROG_FAIL) {
        return SPINAND_PROG_FAIL;
    }

    return ret;
}

/**
 * @brief  SPI Nand block bad block marker.
 * @param  spinand: SPI_NAND dev.
 * @param  addr: page address.
 * @return HAL_Check.
 */
HAL_Check HAL_SPINAND_IsBad(struct SPI_NAND *spinand, uint32_t addr)
{
    uint8_t status;
    uint32_t addrMix;
    uint8_t marker[2] = { };

    HAL_SPINAND_DBG("%s %x\n", __func__, addr);

    SPINAND_LoadPageOp(spinand, addr);

    SPINAND_WaitBusy(spinand, &status, 1000 * 1000);

    addrMix = spinand->secPerPage * SPINAND_SECTOR_SIZE;
    addrMix |= spinand->planePerDie == 1 ? 0 : ((addr >> 6) & 0x1) << 12;
    SPINAND_ReadFromCacheOp(spinand, addrMix, (uint32_t *)marker, sizeof(marker));

    if (marker[0] != 0xff || marker[1] != 0xff) {
        return HAL_TRUE;
    }

    return HAL_FALSE;
}

/**
 * @brief  SPI Nand mark block bad marker.
 * @param  spinand: SPI_NAND dev.
 * @param  addr: page address.
 * @return HAL_Status.
 */
HAL_Status HAL_SPINAND_MarkBad(struct SPI_NAND *spinand, uint32_t addr)
{
    uint8_t status;
    int ret;
    uint32_t addrMix;
    uint8_t marker[2] = { 0, 0 };

    HAL_SPINAND_DBG("%s %x\n", __func__, addr);

    ret = SPINAND_WriteEnableOp(spinand);
    if (ret) {
        return ret;
    }

    addrMix = spinand->secPerPage * SPINAND_SECTOR_SIZE;
    addrMix |= spinand->planePerDie == 1 ? 0 : ((addr >> 6) & 0x1) << 12;
    ret = SPINAND_WriteToCacheOp(spinand, addrMix, (uint32_t *)marker, sizeof(marker));
    if (ret) {
        return ret;
    }

    ret = SPINAND_ProgOp(spinand, addr);
    if (ret) {
        return ret;
    }

    ret = SPINAND_WaitBusy(spinand, &status, 1000 * 1000);
    if (ret) {
        return ret;
    }

    if (status & SPINAND_PROG_FAIL) {
        return SPINAND_PROG_FAIL;
    }

    return ret;
}
/** @} */

/** @defgroup SPINAND_Exported_Functions_Group4 Init and DeInit Functions
 *  @{
 */

/**
 * @brief  SPI_NAND flash module init.
 * @param  spinand: SPI_NAND dev.
 * @return HAL_Status.
 */
HAL_Status HAL_SPINAND_Init(struct SPI_NAND *spinand)
{
    const struct SPINAND_INFO *info;
    uint8_t idByte[5], status;

    if (!spinand->spi) {
        HAL_SPINAND_DBG("%s no host\n", __func__);

        return HAL_INVAL;
    }

    HAL_SPINAND_ReadID(spinand, idByte);
    HAL_SPINAND_DBG("SPI Nand ID: %x %x %x\n", idByte[0], idByte[1], idByte[2]);

    if ((idByte[0] == 0xFF) || (idByte[0] == 0x00)) {
        return HAL_ERROR;
    }

    info = SPINAND_GerFlashInfo(idByte);
    if (!info) {
        return HAL_INVAL;
    }

    spinand->size = (1 << info->density) << 9;
    spinand->secPerPage = info->secPerPage;
    spinand->pagePerBlk = info->pagePerBlk;
    spinand->blkPerPlane = info->blkPerPlane;
    spinand->planePerDie = info->planePerDie;
    spinand->readLines = 1;
    spinand->progLines = 1;
    memcpy(&spinand->meta, &info->meta, sizeof(struct META_AREA));
    spinand->eccStatus = info->eccStatus;

    /* disable block lock */
    SPINAND_WriteStatus(spinand, 0XA0, 0);
    if (spinand->spi->mode & HAL_SPI_TX_QUAD &&
        info->feature & SPINAND_FEA_4BIT_READ) {
        if ((info->hasQeBit && SPINAND_QuadEnable(spinand) == HAL_OK) ||
            !info->hasQeBit) {
            spinand->readLines = 4;
        }
    }

    if (spinand->spi->mode & HAL_SPI_TX_QUAD &&
        info->feature & SPINAND_FEA_4BIT_PROG &&
        spinand->readLines == 4) {
        spinand->progLines = 4;
    }

    SPINAND_ReadStatus(spinand, 0xA0, &status);
    HAL_SPINAND_DBG("SPI Nand A0 = 0x%x\n", status);
    SPINAND_ReadStatus(spinand, 0xB0, &status);
    HAL_SPINAND_DBG("SPI Nand B0 = 0x%x\n", status);
    HAL_SPINAND_DBG("readLines = %x\n", spinand->readLines);
    HAL_SPINAND_DBG("progLines = %x\n", spinand->progLines);

    return HAL_OK;
}

/**
 * @brief  SPI_NAND flash module deinit.
 * @param  spinand: SPI_NAND dev.
 * @return HAL_Status.
 */
HAL_Status HAL_SPINAND_DeInit(struct SPI_NAND *spinand)
{
    return HAL_OK;
}

/** @} */

/** @defgroup SPINAND_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Read flash ID.
 * @param  spinand: SPI_NAND dev.
 * @param  data: address to storage flash ID value.
 * @return HAL_Status.
 */
HAL_Status HAL_SPINAND_ReadID(struct SPI_NAND *spinand, uint8_t *data)
{
    struct HAL_SPI_MEM_OP op = SPINAND_READID_OP(1, data,
                                                 SPINAND_MAX_ID_LEN);

    return SPINAND_SPIMemExecOp(spinand->spi, &op);
}

/**
 * @brief  Get flash capacity.
 * @param  spinand: SPI_NAND dev.
 * @return uint32_t: flash capacity, n bytes.
 */
uint32_t HAL_SPINAND_GetCapacity(struct SPI_NAND *spinand)
{
    return spinand->size;
}

/**
 * @brief  Check if the flash support.
 * @param  flashId: flash id.
 * @return HAL_Check.
 */
HAL_Check HAL_SPINAND_IsFlashSupported(uint8_t *flashId)
{
    uint32_t i;
    uint32_t id;

    if (!flashId) {
        return HAL_FALSE;
    }

    id = (flashId[0] << 8) | (flashId[1] << 0);

    for (i = 0; i < HAL_ARRAY_SIZE(s_spiNandTable); i++) {
        if (s_spiNandTable[i].id == id) {
            return HAL_TRUE;
        }
    }

    return HAL_FALSE;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_SPINAND_MODULE_ENABLED */
