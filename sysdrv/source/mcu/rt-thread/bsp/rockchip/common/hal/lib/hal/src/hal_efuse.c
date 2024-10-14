/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_EFUSE_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup EFUSE
 *  @{
 */

/** @defgroup EFUSE_How_To_Use How To Use
 *  @{

 The EFUSE driver can be used as follows:

 - invoke efuse functions to read data form efuse or write data to efuse.

 @} */

/** @defgroup EFUSE_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#define BIT_PER_BYTE    8
#define EFUSE_SIZE_BIT  ((EFUSE_CTL_AUTO_CTRL_ADDR_AUTO_MASK >> \
            EFUSE_CTL_AUTO_CTRL_ADDR_AUTO_SHIFT) + 1)
#define EFUSE_SEIZ_BYTE (EFUSE_SIZE_BIT / BIT_PER_BYTE)

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup EFUSE_Exported_Functions_Group3 IO Functions
 *  @{
 */

/**
 * @brief  read one byte data from efuse.
 * @param  reg: efuse base address.
 * @param  offset: data offset in efuse.
 * @param  val: output data from efuse.
 * @return HAL_Status.
 */
HAL_Status HAL_EFUSE_ReadByte(struct EFUSE_CTL_REG *reg, uint32_t offset, uint8_t *val)
{
    uint32_t addr;
    uint32_t start, timeoutMs = 20;

    HAL_CRU_ClkEnable(CLK_EFUSE_GATE);
    HAL_CRU_ClkEnable(PCLK_EFUSE_GATE);

    HAL_ASSERT(IS_EFUSE_CTL_INSTANCE(reg));
    HAL_ASSERT(offset < EFUSE_SEIZ_BYTE);
    HAL_ASSERT(val != NULL);

    /* 8 column bits will output same time when read */
    addr = offset << EFUSE_CTL_AUTO_CTRL_ADDR_AUTO_SHIFT;
    addr &= EFUSE_CTL_AUTO_CTRL_ADDR_AUTO_MASK;
    WRITE_REG(reg->AUTO_CTRL, addr | (1 << EFUSE_CTL_AUTO_CTRL_ENB_SHIFT) |
              (1 << EFUSE_CTL_AUTO_CTRL_PG_R_SHIFT));
    HAL_DelayUs(5);

    start = HAL_GetTick();
    while (!(READ_REG(reg->INT_STATUS) | EFUSE_CTL_INT_STATUS_FINISH_INT_MASK)) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            HAL_CRU_ClkDisable(CLK_EFUSE_GATE);
            HAL_CRU_ClkDisable(PCLK_EFUSE_GATE);

            return HAL_TIMEOUT;
        }
    }

    *val = READ_REG(reg->DOUT);
    WRITE_REG(reg->INT_STATUS, 1 << EFUSE_CTL_INT_STATUS_FINISH_INT_SHIFT);

    HAL_CRU_ClkDisable(CLK_EFUSE_GATE);
    HAL_CRU_ClkDisable(PCLK_EFUSE_GATE);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_EFUSE_MODULE_ENABLED */
