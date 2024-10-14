# Rockchip HAL SPI_MEM 用户指南 {#Rockchip_User_Guide_HAL_SPI_MEM}

发布版本：V1.0.0

日期：2021-05-11

文件密级：□绝密   □秘密   □内部资料   ■公开

**免责声明**

本文档按“现状”提供，瑞芯微电子股份有限公司（“本公司”，下同）不对本文档的任何陈述、信息和内容的准确性、可靠性、完整性、适销性、特定目的性和非侵权性提供任何明示或暗示的声明或保证。本文档仅作为使用指导的参考。

由于产品版本升级或其他原因，本文档将可能在未经任何通知的情况下，不定期进行更新或修改。

**商标声明**

“Rockchip”、“瑞芯微”、“瑞芯”均为本公司的注册商标，归本公司所有。

本文档可能提及的其他所有注册商标或商标，由其各自拥有者所有。

**版权所有 © 2021 瑞芯微电子股份有限公司**

超越合理使用范畴，非经本公司书面许可，任何单位和个人不得擅自摘抄、复制本文档内容的部分或全部，并不得以任何形式传播。

瑞芯微电子股份有限公司

Rockchip Electronics Co., Ltd.

地址：     福建省福州市铜盘路软件园A区18号

网址：     [www.rock-chips.com](http://www.rock-chips.com)

客户服务电话： +86-4007-700-590

客户服务传真： +86-591-83951833

客户服务邮箱： [fae@rock-chips.com](mailto:fae@rock-chips.com)

---

**前言**

**概述**

Rockchip HAL SPI_MEM 模块使用指南。

**产品版本**

| **芯片名称** | **内核版本** |
| ------------ | ------------ |
| 所有平台     | 裸系统、RTOS |

**读者对象**

本文档（本指南）主要适用于以下工程师：

技术支持工程师

软件开发工程师

**修订记录**

| **版本号** | **作者** | **修改日期** | **修改说明** |
| ---------- | -------- | ------------ | ------------ |
| V1.0.0     | 林鼎强   | 2021-05-11   | 初始版本     |

---

**目录**

[TOC]

---

## 简介

市面上支持 SPI 协议的器件较多，且 RK 有 FSPI 和 SPI 两种 IP 都支持 SPI 协议，所以需要添加一层 SPI_MEM 来统一 device driver 和 host 接口：

SPI_MEM 在框架中的应用如下，以 SPI Nor 为例：

![SPIFLASH_Layer](Rockchip_User_Guide_HAL_SPI_MEM/SPIFLASH_Layer.png)

支持主要涉及以下两点：

- SPI Nor 协议层根据 SPI_MEM 规范制作的数据包
- SPI Nor 协议层套接的 SPI XFER 中的 host 接口函数解析符合 SPI_MEM 规范的数据包，并完成相应操作

## 规范说明

主要结构体如下：

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

- 显而易见，SPI_MEM 将数据包规范为 CMD(output) + ADDR(output) + DUMMY(output) + DATA(output/input)
- 已知兼容以下器件：
    - SPI Nor
    - SPI Nand
    - QPI Psram
- 已知兼容以下主控：
    - FSPI
    - SPI

## 应用说明

以 SPI Nor 器件和 FSPI 主控设备的读颗粒寄存器命令为例。

SPI Nor 协议层根据 SPI_MEM 规范制作的数据包：

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

 host 接口函数解析符合 SPI_MEM 规范的数据包：

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
