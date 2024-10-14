/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_BUFMGR_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup BUFMGR
 *  @{
 */

/** @defgroup BUFMGR_Private_Definition Private Definition
 *  @{
 */

/******************** Private Variable Definition ***********************/

static struct BUFFER_MANAGER_REG *HAL_BUFMGR_ParseBase(int chan, int block)
{
    return (struct BUFFER_MANAGER_REG *)(BUFFER_MANAGER_BASE +
                                         (chan * 0x80U + block * 0x40U));
}

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup BUFMGR_Exported_Functions_Group1 WCH Related Functions

This section provides functions allowing to operate WCH of the Buffer Manager:

*  @{
*/

/**
 * @brief  Buffer Manager WCH Interrupt Status Clear
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @param  val: which status to clear
 * @return HAL_Status
 */
HAL_Status HAL_BUFMGR_WCHIntStsClr(int chan, int block, uint32_t val)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    pBufMgr->WCH_MBLK_INTSTS = val;

    return HAL_OK;
}

/**
 * @brief  Buffer Manager WCH Interrupt Status Clear By Index
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @param  idx: which index to clear
 * @return HAL_Status
 */
HAL_Status HAL_BUFMGR_WCHIntStsClrByIdx(int chan, int block, uint32_t idx)
{
    uint32_t val = 0;

    val = HAL_BUFMGR_WCH_INTSTS_FULL_BIT(idx) |
          HAL_BUFMGR_WCH_STS_FULL_BIT(idx) |
          HAL_BUFMGR_WCH_STS_EOH_BIT(idx) |
          HAL_BUFMGR_WCH_STS_EOF_BIT(idx);

    return HAL_BUFMGR_WCHIntStsClr(chan, block, val);
}

/**
 * @brief  Buffer Manager WCH Interrupt Status Clear All
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @return HAL_Status
 */
HAL_Status HAL_BUFMGR_WCHIntStsAllClr(int chan, int block)
{
    return HAL_BUFMGR_WCHIntStsClr(chan, block, 0x1FFFFU);
}

/**
 * @brief  Buffer Manager WCH Interrupt Enable/Disable All
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @param  en: enable/disable WCH Interrupt
 * @return HAL_Status
 */
HAL_Status HAL_BUFMGR_WCHIntAllEn(int chan, int block, uint8_t en)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    if (en) {
        pBufMgr->WCH_MBLK_INT_EN =
            BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF0_FULL_INT_EN_MASK |
            BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF1_FULL_INT_EN_MASK |
            BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF2_FULL_INT_EN_MASK |
            BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF3_FULL_INT_EN_MASK |
            BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF_OVERFLOW_INT_EN_MASK;
    } else {
        pBufMgr->WCH_MBLK_INT_EN = 0x0U;
    }

    return HAL_OK;
}

/**
 * @brief  Buffer Manager WCH Ping Pong Buffer Enable/Disable
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @param  en: enable/disable WCH Ping Pong Buffer
 * @return HAL_Status
 */
HAL_Status HAL_BUFMGR_WCHPpBufEn(int chan, int block, uint8_t en)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    if (en) {
        pBufMgr->WCH_MBLK_CON0 |=
            BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_ENABLE_MASK;
    } else {
        pBufMgr->WCH_MBLK_CON0 &=
            ~BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_ENABLE_MASK;
    }

    return HAL_OK;
}

/**
 * @brief  Get Buffer Length For The last WCH Buffer
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @return the data length
 */
uint32_t HAL_BUFMGR_WCHEndBufLenGet(int chan, int block)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    return pBufMgr->WCH_MBLK_STATUS1 &
           BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_DCOUNTER_MASK;
}

/**
 * @brief  Get The Current WCH Buffer Index
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @return the buffer index
 */
uint32_t HAL_BUFMGR_WCHCurBufIdxGet(int chan, int block)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    return ((pBufMgr->WCH_MBLK_STATUS0 &
             BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_INDEX_MASK) >>
            BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_INDEX_SHIFT);
}

/**
 * @brief  Get The WCH Buffer Interrupt status
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @return the buffer interrupt status
 */
uint32_t HAL_BUFMGR_WCHIntStsGet(int chan, int block)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    return pBufMgr->WCH_MBLK_INTSTS & 0x1FFFFU;
}

/**
 * @brief  Get The WCH Ping-Pong Buffer Size
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @return the buffer size by specified channel and block
 */
uint32_t HAL_BUFMGR_WCHBufSizeGet(int chan, int block)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);

    return ((pBufMgr->WCH_MBLK_CON0 &
             BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_MASK) >>
            BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_SHIFT);
}

/**
 * @brief  Get The Specified WCH Buffer Address
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @param  idx: buffer index
 * @return the buffer address by specified index
 */
uint32_t HAL_BUFMGR_WCHBufAddrGet(int chan, int block, int idx)
{
    struct BUFFER_MANAGER_REG *pBufMgr = HAL_BUFMGR_ParseBase(chan, block);
    uint32_t addr = 0;

    switch (idx) {
    case 0:
        addr = pBufMgr->WCH_MBLK_CON1;
        break;
    case 1:
        addr = pBufMgr->WCH_MBLK_CON2;
        break;
    case 2:
        addr = pBufMgr->WCH_MBLK_CON3;
        break;
    case 3:
        addr = pBufMgr->WCH_MBLK_CON4;
        break;
    default:
        break;
    }

    return addr & 0xFFFFFFFFU;
}

/**
 * @brief  Check WCH Interrupt Status Is Overflow Or Not
 * @param  sts: interrupt status
 * @return HAL_TRUE or HAL_FALSE
 */
HAL_Check HAL_BUFMGR_WCHIntStsIsOVFL(uint32_t sts)
{
    return (sts & HAL_BUFMGR_WCH_INTSTS_OVFL_BIT ? HAL_TRUE : HAL_FALSE);
}

/**
 * @brief  Check WCH Interrupt Status Is Buffer Full Or Not
 * @param  sts: interrupt status
 * @param  idx: buffer index
 * @return HAL_TRUE or HAL_FALSE
 */
HAL_Check HAL_BUFMGR_WCHIntStsIsFull(uint32_t sts, int idx)
{
    return (sts & HAL_BUFMGR_WCH_INTSTS_FULL_BIT(idx) ? HAL_TRUE : HAL_FALSE);
}

/**
 * @brief  Check WCH Buffer Status Is Full Or Not
 * @param  sts: interrupt status
 * @param  idx: buffer index
 * @return HAL_TRUE or HAL_FALSE
 */
HAL_Check HAL_BUFMGR_WCHStsIsBufFull(uint32_t sts, int idx)
{
    return (sts & HAL_BUFMGR_WCH_STS_FULL_BIT(idx) ? HAL_TRUE : HAL_FALSE);
}

/**
 * @brief  Check WCH Buffer Status Is EOH Or Not
 * @param  chan: write channel id
 * @param  block: write channel block id
 * @param  sts: interrupt status
 * @param  idx: buffer index
 * @return HAL_TRUE or HAL_FALSE
 */
HAL_Check HAL_BUFMGR_WCHStsIsEOH(int chan, int block, uint32_t sts, int idx)
{
    uint32_t fSize, bSize;
    struct BUFFER_MANAGER_REG *pBufMgr;

    if (sts & HAL_BUFMGR_WCH_STS_EOH_BIT(idx)) {
        return HAL_TRUE;
    } else if (sts & HAL_BUFMGR_WCH_STS_EOF_BIT(idx)) {
        /*
         * FIXME:
         * The Buffer Manger missed tagging the EOH flag when the frame size
         * is less then the buffer size. In this case, the EOH and EOF tag
         * are both needed to tag for the buffer. So tag the EOH by software
         * checking as a workaround.
         */
        pBufMgr = HAL_BUFMGR_ParseBase(chan, block);
        fSize = pBufMgr->WCH_MBLK_STATUS3 &
                BUFFER_MANAGER_WCH_MBLK_STATUS3_FRAME_END_COUNTER_MASK;
        bSize = (pBufMgr->WCH_MBLK_CON0 &
                 BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_MASK) >>
                BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_SHIFT;

        if (fSize <= bSize) {
            return HAL_TRUE;
        }
    }

    return HAL_FALSE;
}

/**
 * @brief  Check WCH Buffer Status Is EOF Or Not
 * @param  sts: interrupt status
 * @param  idx: buffer index
 * @return HAL_TRUE or HAL_FALSE
 */
HAL_Check HAL_BUFMGR_WCHStsIsEOF(uint32_t sts, int idx)
{
    return (sts & HAL_BUFMGR_WCH_STS_EOF_BIT(idx) ? HAL_TRUE : HAL_FALSE);
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_BUFMGR_MODULE_ENABLED */
