# Rockchip HAL SPI_MEM User Guide {#Rockchip_User_Guide_HAL_SPI_MEM}

Release Version: V1.0.0

Release Date: 2021-05-11

Security Level: □Top-Secret   □Secret   □Internal   ■Public

**DISCLAIMER**

THIS DOCUMENT IS PROVIDED “AS IS”. ROCKCHIP ELECTRONICS CO., LTD.(“ROCKCHIP”)DOES NOT PROVIDE ANY WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR OTHERWISE, WITH RESPECT TO THE ACCURACY, RELIABILITY, COMPLETENESS,MERCHANTABILITY, FITNESS FOR ANY PARTICULAR PURPOSE OR NON-INFRINGEMENT OF ANY REPRESENTATION, INFORMATION AND CONTENT IN THIS DOCUMENT. THIS DOCUMENT IS FOR REFERENCE ONLY. THIS DOCUMENT MAY BE UPDATED OR CHANGED WITHOUT ANY NOTICE AT ANY TIME DUE TO THE UPGRADES OF THE PRODUCT OR ANY OTHER REASONS.

**Trademark Statement**

"Rockchip", "瑞芯微", "瑞芯" shall be Rockchip’s registered trademarks and owned by Rockchip. All the other trademarks or registered trademarks mentioned in this document shall be owned by their respective owners.

**All rights reserved. ©2021. Rockchip Electronics Co., Ltd.**

Beyond the scope of fair use, neither any entity nor individual shall extract, copy, or distribute this document in any form in whole or in part without the written approval of Rockchip.

Rockchip Electronics Co., Ltd.

No.18 Building, A District, No.89, software Boulevard Fuzhou, Fujian,PRC

Website:     [www.rock-chips.com](http://www.rock-chips.com)

Customer service Tel:  +86-4007-700-590

Customer service Fax:  +86-591-83951833

Customer service e-Mail:  [fae@rock-chips.com](mailto:fae@rock-chips.com)mailto:fae@rock-chips.com)

---

**Preface**

**Overview**

Rockchip HAL SPI_MEM user guide.

**Product Version**

| **Chipset**  | **Kernel Version** |
| ------------ | ------------------ |
| All platform | Bare mental、RTOS  |

**Intended Audience**

This document (this guide) is mainly intended for:

Technical support engineers

Software development engineers

**Revision History**

| **Version** | **Author** | **Date**   | **Change Description** |
| ----------- | ---------- | ---------- | ---------------------- |
| V1.0.0      | Jon Lin    | 2021-05-11 | Initial version        |

---

**Contents**

[TOC]

---

## Introduction

There are many devices that support SPI protocol on the market, and RK  has FSPI and SPI IP that support SPI protocol, so a layer of SPI_MEM is  added to unify the device driver and host interfaces, SPI_MEM in the framework applies as follows, using SPI Nor as an example:

![SPIFLASH_Layer](Rockchip_User_Guide_HAL_SPI_MEM/SPIFLASH_Layer.png)

Support mainly involves two points：

- Data packets made by the SPI Nor protocol layer according to the SPI_MEM specification
- The host interface function in SPI XFER covered by SPI Nor protocol  layer parses SPI_MEM specification compliant packets and performs  accordingly

## Specification Description

Main structures are as follows：

```c
struct HAL_SPI_MEM_OP {
    struct {
        uint8_t buswidth;
        uint8_t opcode;
    } cmd;

    struct {
        uint8_t nbytes;
        uint8_t buswidth;
        uint32_t val;
    } addr;

    struct {
        uint8_t a2dIdle;
        uint8_t nbytes;
        uint8_t buswidth;
    } dummy;

    struct {
        uint8_t buswidth;
        enum SPI_MEM_DATA_DIR dir;
        unsigned int nbytes;
        /**< buf.{in,out} must be DMA-able. */
        union {
            void *in;
            const void *out;
        } buf;
    } data;
};
```

注释：

- Obviously，SPI_MEM pack the data with the format: CMD(output) + ADDR(output) + DUMMY(output) + DATA(output/input)
- Compatible devices：
    - SPI Nor
    - SPI Nand
    - QPI Psram
- Compatible controllers：
    - FSPI
    - SPI

## Application Note

Take the read register command of SPI Nor devices and FSPI master devices as an example。

Packages made by the SPI Nor protocol layer under the SPI_MEM specification:

```c
static HAL_Status SNOR_ReadWriteReg(struct SPI_NOR *nor, struct HAL_SPI_MEM_OP *op, void *buf)
{
    if (op->data.dir == HAL_SPI_MEM_DATA_IN) {
        op->data.buf.in = buf;
    } else {
        op->data.buf.out = buf;
    }

    return SNOR_SPIMemExecOp(nor->spi, op);
}

static HAL_Status SNOR_ReadReg(struct SPI_NOR *nor, uint8_t code, uint8_t *val, uint32_t len)
{
    struct HAL_SPI_MEM_OP op = HAL_SPI_MEM_OP_FORMAT(HAL_SPI_MEM_OP_CMD(code, 1),
                                                     HAL_SPI_MEM_OP_NO_ADDR,
                                                     HAL_SPI_MEM_OP_NO_DUMMY,
                                                     HAL_SPI_MEM_OP_DATA_IN(len, NULL, 1));
    HAL_Status ret;

    /* HAL_SNOR_DBG("%s %x %lx\n", __func__, code, len); */
    ret = SNOR_ReadWriteReg(nor, &op, val);
    if (ret) {
        HAL_SNOR_DBG("error %d reading %x\n", ret, code);
    }

    return ret;
}
```

The host interface function parses SPI_MEM-compliant packets:

```c
HAL_Status HAL_FSPI_SpiXfer(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    HAL_Status ret = HAL_OK;
    uint32_t dir = op->data.dir;
    void *pData = NULL;

    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    if (op->data.buf.in) {
        pData = (void *)op->data.buf.in;
    } else if (op->data.buf.out) {
        pData = (void *)op->data.buf.out;
    }

    HAL_FSPI_XferStart(host, op);
    if (pData) {
#if defined(HAL_FSPI_DMA_ENABLED)
        if ((op->data.nbytes >= FSPI_NOR_FLASH_PAGE_SIZE) &&
            (dir == HAL_SPI_MEM_DATA_IN)) {
            ret = HAL_FSPI_XferData_DMA(host, op->data.nbytes, pData, dir);
        } else
#endif
        ret = HAL_FSPI_XferData(host, op->data.nbytes, pData, dir);
        if (ret) {
            FSPI_DBG("%s xfer data failed ret %d\n", __func__, ret);

            return ret;
        }
    }

    return HAL_FSPI_XferDone(host);
}
```
