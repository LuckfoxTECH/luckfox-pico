/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_I2C_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup I2C
 *  @brief I2C HAL module driver
 *  @{
 */

/** @defgroup I2C_How_To_Use How To Use
 *  @{

 The I2C HAL driver can be used as follows:

 - Declare a I2C_HANDLE handle structure, for example:
   ```
   I2C_HANDLE instance;
   ```
 - Invoke HAL_I2C_Init() API to initialize base address and bourate:
   - Base register address;
   - DIV base on speed.

 - Invoke HAL_I2C_ConfigureMode() API and HAL_I2C_SetupMsg() to program mode:
   - I2C mode;
   - Device address and register address;
   - Current message.

 - There are two modes of transfer:
   - Blocking mode: The communication is performed in polling mode by calling
     HAL_I2C_Transfer() and HAL_I2C_IRQHandler() with I2C_POLL type;
   - No-Blocking mode: The communication is performed using Interrupts.The
     HAL_I2C_Transfer() with I2C_IT type, HAL_I2C_IRQHandler() is used for
     interrupt mode.

 - Invoke HAL_I2C_Close() to finish this transfer.

 - Invoke HAL_I2C_DeInit() if necessary.

 @} */

/** @defgroup I2C_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/* REG_CON bits */
#define REG_CON_EN       (0x1 << I2C_CON_I2C_EN_SHIFT)
#define REG_CON_MOD(mod) ((mod) << I2C_CON_I2C_MODE_SHIFT)
#define REG_CON_MOD_MASK (I2C_CON_I2C_MODE_MASK)
#define REG_CON_START    (0x1 << I2C_CON_START_SHIFT)
#define REG_CON_STOP     (0x1 << I2C_CON_STOP_SHIFT)
#define REG_CON_LASTACK  (0x1 << I2C_CON_ACK_SHIFT) /* send NACK after last received byte */
#define REG_CON_ACTACK   (0x1 << I2C_CON_ACT2NAK_SHIFT) /* stop if NACK is received */

#define REG_CON_TUNING_MASK  (0xff << I2C_CON_DATA_UPD_ST_SHIFT)
#define REG_CON_SDA_CFG(cfg) ((cfg) << I2C_CON_DATA_UPD_ST_SHIFT)
#define REG_CON_STA_CFG(cfg) ((cfg) << I2C_CON_START_SETUP_SHIFT)
#define REG_CON_STO_CFG(cfg) ((cfg) << I2C_CON_STOP_SETUP_SHIFT)

/* REG_IEN/REG_IPD bits */
#define REG_INT_BTF    (0x1 << I2C_IEN_BTFIEN_SHIFT) /* a byte was transmitted */
#define REG_INT_BRF    (0x1 << I2C_IEN_BRFIEN_SHIFT) /* a byte was received */
#define REG_INT_MBTF   (0x1 << I2C_IEN_MBTFIEN_SHIFT) /* master data transmit finished */
#define REG_INT_MBRF   (0x1 << I2C_IEN_MBRFIEN_SHIFT) /* master data receive finished */
#define REG_INT_START  (0x1 << I2C_IEN_STARTIEN_SHIFT) /* START condition generated */
#define REG_INT_STOP   (0x1 << I2C_IEN_STOPIEN_SHIFT) /* STOP condition generated */
#define REG_INT_NAKRCV (0x1 << I2C_IEN_NAKRCVIEN_SHIFT) /* NACK received */
#define REG_INT_ALL    (0xff)

/* Disable i2c all irqs */
#define IEN_ALL_DISABLE 0

/* TX data */
#define MAX_TX_DATA_REGISTER_CNT 8

/********************* Private Structure Definition **************************/

struct I2C_SPEC_VALUES {
    uint32_t minLowNS;
    uint32_t minHighNS;
    uint32_t maxRiseNS;
    uint32_t maxFallNS;
};

/********************* Private Variable Definition ***************************/

static const struct I2C_SPEC_VALUES standardModeSpec = {
    .minLowNS = 4700,
    .minHighNS = 4000,
    .maxRiseNS = 1000,
    .maxFallNS = 300,
};

static const struct I2C_SPEC_VALUES fastModeSpec = {
    .minLowNS = 1300,
    .minHighNS = 600,
    .maxRiseNS = 300,
    .maxFallNS = 300,
};

static const struct I2C_SPEC_VALUES fastModePlusSpec = {
    .minLowNS = 500,
    .minHighNS = 260,
    .maxRiseNS = 120,
    .maxFallNS = 120,
};

/********************* Private Function Definition ***************************/

/**
  * @brief  Get the I2C timing specification.
  * @param  speed: desired I2C bus speed.
  * @return matched i2c spec values
  */
static const struct I2C_SPEC_VALUES *I2C_GetSpec(eI2C_BusSpeed speed)
{
    if (speed == I2C_1000K) {
        return &fastModePlusSpec;
    } else if (speed == I2C_400K) {
        return &fastModeSpec;
    } else {
        return &standardModeSpec;
    }
}

/**
  * @brief  Clean the I2C pending interrupt.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
static HAL_Status I2C_CleanIPD(struct I2C_HANDLE *pI2C)
{
    WRITE_REG(pI2C->pReg->IPD, REG_INT_ALL);

    return HAL_OK;
}

/**
  * @brief  Disable the I2C interrupt.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
static HAL_Status I2C_DisableIRQ(struct I2C_HANDLE *pI2C)
{
    WRITE_REG(pI2C->pReg->IEN, IEN_ALL_DISABLE);

    return HAL_OK;
}

/**
  * @brief  Disable the I2C controller.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
static HAL_Status I2C_Disable(struct I2C_HANDLE *pI2C)
{
    uint32_t val = READ_REG(pI2C->pReg->CON) & REG_CON_TUNING_MASK;

    WRITE_REG(pI2C->pReg->CON, val);

    return HAL_OK;
}

/**
  * @brief  Send I2C start signal.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
static HAL_Status I2C_Start(struct I2C_HANDLE *pI2C)
{
    uint32_t val = READ_REG(pI2C->pReg->CON) & REG_CON_TUNING_MASK;

    I2C_CleanIPD(pI2C);

    pI2C->state = STATE_START;
    if (pI2C->type == I2C_IT) {
        WRITE_REG(pI2C->pReg->IEN, REG_INT_START);
    }

    /* enable adapter with correct mode, send START condition */
    val |= REG_CON_EN | REG_CON_MOD(pI2C->mode) | REG_CON_START | pI2C->cfg;

    /* if we want to react to NACK, set ACTACK bit */
    if (!(pI2C->msg.flags & HAL_I2C_M_IGNORE_NAK)) {
        val |= REG_CON_ACTACK;
    }

    WRITE_REG(pI2C->pReg->CON, val);

    return HAL_OK;
}

/**
  * @brief  Send I2C stop signal.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  error: report the error for the stop.
  * @return HAL status
  */
static HAL_Status I2C_Stop(struct I2C_HANDLE *pI2C, HAL_Status error)
{
    uint32_t ctrl;

    pI2C->processed = 0;
    pI2C->error = error;

    if (pI2C->isLastMSG || error) {
        if (pI2C->type == I2C_IT) {
            /* Enable stop interrupt */
            WRITE_REG(pI2C->pReg->IEN, REG_INT_STOP);
        }

        pI2C->state = STATE_STOP;

        ctrl = READ_REG(pI2C->pReg->CON);
        ctrl |= REG_CON_STOP;
        WRITE_REG(pI2C->pReg->CON, ctrl);

        return HAL_BUSY;
    } else {
        /* To start the next message. */
        pI2C->state = STATE_IDLE;

        /*
         * The HW is actually not capable of REPEATED START. But we can
         * get the intended effect by resetting its internal state
         * and issuing an ordinary START.
         */
        ctrl = READ_REG(pI2C->pReg->CON) & REG_CON_TUNING_MASK;
        WRITE_REG(pI2C->pReg->CON, ctrl);

        return HAL_OK;
    }
}

/**
  * @brief  Prepare for reading i2c data.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
static HAL_Status I2C_PrepareRead(struct I2C_HANDLE *pI2C)
{
    uint32_t len = pI2C->msg.len - pI2C->processed;
    uint32_t con;

    con = READ_REG(pI2C->pReg->CON);

    /*
     * The hw can read up to 32 bytes at a time. If we need more than one
     * chunk, send an ACK after the last byte of the current chunk.
     */
    if (len > 32) {
        len = 32;
        con &= ~REG_CON_LASTACK;
    } else {
        con |= REG_CON_LASTACK;
    }

    /* make sure we are in plain RX mode if we read a second chunk */
    if (pI2C->processed != 0) {
        con &= ~REG_CON_MOD_MASK;
        con |= REG_CON_MOD(REG_CON_MOD_RX);
    }

    WRITE_REG(pI2C->pReg->CON, con);
    WRITE_REG(pI2C->pReg->MRXCNT, len);

    return HAL_OK;
}

/**
  * @brief  Fill transmit buffer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
static HAL_Status I2C_FillTransmitBuf(struct I2C_HANDLE *pI2C)
{
    uint32_t i, j;
    uint32_t val, cnt = 0;
    uint8_t byte;

    for (i = 0; i < MAX_TX_DATA_REGISTER_CNT; ++i) {
        val = 0;
        for (j = 0; j < 4; ++j) {
            if ((pI2C->processed == pI2C->msg.len) && (cnt != 0)) {
                break;
            }

            if (pI2C->processed == 0 && cnt == 0) {
                byte = (pI2C->msg.addr & 0x7f) << 1;
            } else {
                byte = pI2C->msg.buf[pI2C->processed++];
            }

            val |= byte << (j * 8);
            cnt++;
        }

        WRITE_REG(pI2C->pReg->TXDATA[i], val);
        if (pI2C->processed == pI2C->msg.len) {
            break;
        }
    }

    WRITE_REG(pI2C->pReg->MTXCNT, cnt);

    return HAL_OK;
}

/**
  * @brief  Handle I2C start interrupt for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  ipd: interrupt pending status
  * @return HAL status
  */
static HAL_Status I2C_HandleStart(struct I2C_HANDLE *pI2C, uint32_t ipd)
{
    if (!(ipd & REG_INT_START)) {
        I2C_Stop(pI2C, HAL_ERROR);
        HAL_DBG_ERR("unexpected irq in START: 0x%lx\n", ipd);
        I2C_CleanIPD(pI2C);

        return HAL_BUSY;
    }

    /* ack interrupt */
    WRITE_REG(pI2C->pReg->IPD, REG_INT_START);

    /* disable start bit */
    WRITE_REG(pI2C->pReg->CON, READ_REG(pI2C->pReg->CON) & ~REG_CON_START);

    /* enable appropriate interrupts and transition */
    if (pI2C->mode == REG_CON_MOD_TX) {
        if (pI2C->type == I2C_IT) {
            WRITE_REG(pI2C->pReg->IEN, REG_INT_MBTF | REG_INT_NAKRCV);
        }
        pI2C->state = STATE_WRITE;
        I2C_FillTransmitBuf(pI2C);
    } else {
        /* in any other case, we are going to be reading. */
        if (pI2C->type == I2C_IT) {
            WRITE_REG(pI2C->pReg->IEN, REG_INT_MBRF | REG_INT_NAKRCV);
        }
        pI2C->state = STATE_READ;
        I2C_PrepareRead(pI2C);
    }

    return HAL_BUSY;
}

/**
  * @brief  Handle I2C write interrupt for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  ipd: interrupt pending status
  * @return HAL status
  */
static HAL_Status I2C_HandleWrite(struct I2C_HANDLE *pI2C, uint32_t ipd)
{
    if (!(ipd & REG_INT_MBTF)) {
        I2C_Stop(pI2C, HAL_ERROR);
        HAL_DBG_ERR("unexpected irq in WRITE: 0x%lx\n", ipd);
        I2C_CleanIPD(pI2C);

        return HAL_BUSY;
    }

    /* ack interrupt */
    WRITE_REG(pI2C->pReg->IPD, REG_INT_MBTF);

    /* are we finished? */
    if (pI2C->processed == pI2C->msg.len) {
        return I2C_Stop(pI2C, pI2C->error);
    } else {
        I2C_FillTransmitBuf(pI2C);

        return HAL_BUSY;
    }
}

/**
  * @brief  Handle I2C read interrupt for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  ipd: interrupt pending status
  * @return HAL status
  */
static HAL_Status I2C_HandleRead(struct I2C_HANDLE *pI2C, uint32_t ipd)
{
    uint32_t len = pI2C->msg.len - pI2C->processed;
    uint32_t i, val = 0;
    uint8_t byte;

    /* we only care for MBRF here. */
    if (!(ipd & REG_INT_MBRF)) {
        return HAL_BUSY;
    }

    /* ack interrupt */
    WRITE_REG(pI2C->pReg->IPD, REG_INT_MBRF);

    /* Can only handle a maximum of 32 bytes at a time */
    if (len > 32) {
        len = 32;
    }

    /* read the data from receive buffer */
    for (i = 0; i < len; ++i) {
        if (i % 4 == 0) {
            val = READ_REG(pI2C->pReg->RXDATA[i / 4]);
        }

        byte = (val >> ((i % 4) * 8)) & 0xff;
        pI2C->msg.buf[pI2C->processed++] = byte;
    }

    /* are we finished? */
    if (pI2C->processed == pI2C->msg.len) {
        return I2C_Stop(pI2C, pI2C->error);
    } else {
        I2C_PrepareRead(pI2C);

        return HAL_BUSY;
    }
}

/**
  * @brief  Handle I2C stop interrupt for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  ipd: interrupt pending status
  * @return HAL status
  */
static HAL_Status I2C_HandleStop(struct I2C_HANDLE *pI2C, uint32_t ipd)
{
    uint32_t con;

    if (!(ipd & REG_INT_STOP)) {
        I2C_Stop(pI2C, HAL_ERROR);
        HAL_DBG_ERR("unexpected irq in STOP: 0x%lx\n", ipd);
        I2C_CleanIPD(pI2C);

        return HAL_BUSY;
    }

    /* ack interrupt */
    WRITE_REG(pI2C->pReg->IPD, REG_INT_STOP);

    /* disable stop bit */
    con = READ_REG(pI2C->pReg->CON);
    con &= ~REG_CON_STOP;
    WRITE_REG(pI2C->pReg->CON, con);

    pI2C->state = STATE_IDLE;

    return pI2C->error ? pI2C->error : HAL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup I2C_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
  * @brief  Auto adapte the clock div base on input clock rate and desired speed.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  rate: I2C sclk rate, unit is HZ.
  * @return HAL status
  */
HAL_Status HAL_I2C_AdaptDIV(struct I2C_HANDLE *pI2C, uint32_t rate)
{
    const struct I2C_SPEC_VALUES *spec;
    uint32_t rateKHZ, speedKHZ;
    uint32_t minTotalDIV, minLowDIV, minHighDIV, minHoldDIV;
    uint32_t lowDIV, highDIV, extraDIV, extraLowDIV;
    uint32_t minLowNS, minHighNS;
    uint32_t startSetup = 0;

    switch (pI2C->speed) {
    case I2C_1000K:
        speedKHZ = 1000;
        break;
    case I2C_400K:
        speedKHZ = 400;
        break;
    default:
        /* default start setup time may not enough for 100K */
        startSetup = 1;
        speedKHZ = 100;
        break;
    }

    rateKHZ = HAL_DIV_ROUND_UP(rate, 1000);
    spec = I2C_GetSpec(pI2C->speed);

    minTotalDIV = HAL_DIV_ROUND_UP(rateKHZ, speedKHZ * 8);

    minHighNS = spec->maxRiseNS + spec->minHighNS;
    minHighDIV = HAL_DIV_ROUND_UP(rateKHZ * minHighNS, 8 * 1000000);

    minLowNS = spec->maxFallNS + spec->minLowNS;
    minLowDIV = HAL_DIV_ROUND_UP(rateKHZ * minLowNS, 8 * 1000000);

    minHighDIV = (minHighDIV < 1) ? 2 : minHighDIV;
    minLowDIV = (minLowDIV < 1) ? 2 : minLowDIV;

    minHoldDIV = minHighDIV + minLowDIV;

    if (minHoldDIV >= minTotalDIV) {
        highDIV = minHighDIV;
        lowDIV = minLowDIV;
    } else {
        extraDIV = minTotalDIV - minHoldDIV;
        extraLowDIV = HAL_DIV_ROUND_UP(minLowDIV * extraDIV, minHoldDIV);

        lowDIV = minLowDIV + extraLowDIV;
        highDIV = minHighDIV + (extraDIV - extraLowDIV);
    }

    highDIV--;
    lowDIV--;

    if (highDIV > 0xffff || lowDIV > 0xffff) {
        return HAL_INVAL;
    }

    pI2C->cfg = REG_CON_SDA_CFG(1) | REG_CON_STA_CFG(startSetup);
    WRITE_REG(pI2C->pReg->CLKDIV, (highDIV << I2C_CLKDIV_CLKDIVH_SHIFT) | lowDIV);
    /* 1 for data hold/setup time is enough */
    WRITE_REG(pI2C->pReg->CON, REG_CON_SDA_CFG(1) | REG_CON_STA_CFG(startSetup));

    return HAL_OK;
}

/**
  * @brief  Handle I2C interrupt for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_IRQHandler(struct I2C_HANDLE *pI2C)
{
    uint32_t ipd = READ_REG(pI2C->pReg->IPD);
    HAL_Status result = HAL_BUSY;

    if (pI2C->state == STATE_IDLE) {
        HAL_DBG_WRN("irq in STATE_IDLE, ipd = 0x%lx\n", ipd);
        I2C_CleanIPD(pI2C);
        /* Terminate this transfer. */
        result = HAL_INVAL;
        goto out;
    }

    /* Clean interrupt bits we don't care about */
    ipd &= ~(REG_INT_BRF | REG_INT_BTF);

    if (ipd & REG_INT_NAKRCV) {
        /*
         * We got a NACK in the last operation. Depending on whether
         * IGNORE_NAK is set, we have to stop the operation and report
         * an error.
         */
        WRITE_REG(pI2C->pReg->IPD, REG_INT_NAKRCV);
        ipd &= ~REG_INT_NAKRCV;

        if (!(pI2C->msg.flags & HAL_I2C_M_IGNORE_NAK)) {
            /*
             * Still return busy status, and would finish transfer
             * after the stop handled.
             */
            if (pI2C->speed == I2C_100K) {
                HAL_DelayUs(1);
            }

            I2C_Stop(pI2C, HAL_NODEV);
            goto out;
        }
    }

    /* is there anything left to handle? */
    if ((ipd & REG_INT_ALL) == 0) {
        goto out;
    }

    switch (pI2C->state) {
    case STATE_START:
        result = I2C_HandleStart(pI2C, ipd);
        break;
    case STATE_WRITE:
        result = I2C_HandleWrite(pI2C, ipd);
        break;
    case STATE_READ:
        result = I2C_HandleRead(pI2C, ipd);
        break;
    case STATE_STOP:
        /* finish transfer */
        result = I2C_HandleStop(pI2C, ipd);
    case STATE_IDLE:
        break;
    }

out:

    return result;
}

/**
  * @brief  Setup current message for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  addr: slave device address.
  * @param  buf: pointer to the buffer for I2C transfer.
  * @param  len: data length for I2C transfer.
  * @param  flags: flags for I2C transfer.
  * @return HAL status
  */
HAL_Status HAL_I2C_SetupMsg(struct I2C_HANDLE *pI2C, uint16_t addr, uint8_t *buf,
                            uint16_t len, uint16_t flags)
{
    HAL_ASSERT(pI2C != NULL);

    pI2C->msg.addr = addr;
    pI2C->msg.buf = buf;
    pI2C->msg.len = len;
    pI2C->msg.flags = flags;

    return HAL_OK;
}

/**
  * @brief  Configure the I2C mode.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  mode: mode for I2C transfer.
  * @param  addr: slave device address with r/w bit and vaild bit.
  * @param  regAddr: register address with vaild bit.
  * @return HAL status
  */
HAL_Status HAL_I2C_ConfigureMode(struct I2C_HANDLE *pI2C, eI2C_Mode mode,
                                 uint32_t addr, uint32_t regAddr)
{
    HAL_ASSERT(pI2C != NULL);

    if (mode == REG_CON_MOD_REGISTER_TX) {
        WRITE_REG(pI2C->pReg->MRXADDR, addr);
        WRITE_REG(pI2C->pReg->MRXRADDR, regAddr);
    }

    pI2C->mode = mode;

    return HAL_OK;
}

/**
  * @brief  Start the I2C transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  type: using poll or interrupt.
  * @param  last: Last message or not for this transmit.
  * @return HAL status
  */
HAL_Status HAL_I2C_Transfer(struct I2C_HANDLE *pI2C, eI2C_TransferType type, bool last)
{
    HAL_ASSERT(pI2C != NULL);

    pI2C->type = type;
    pI2C->isLastMSG = last;

    /* Reset value */
    pI2C->processed = 0;
    pI2C->error = HAL_OK;
    pI2C->state = STATE_IDLE;

    I2C_Start(pI2C);

    return HAL_OK;
}

/**
  * @brief  Force to stop the I2C transfer without interrupt.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_ForceStop(struct I2C_HANDLE *pI2C)
{
    HAL_ASSERT(pI2C != NULL);

    /* Force a STOP condition without interrupt */
    I2C_DisableIRQ(pI2C);
    MODIFY_REG(pI2C->pReg->CON, ~REG_CON_TUNING_MASK,
               REG_CON_EN | REG_CON_STOP);

    pI2C->state = STATE_IDLE;

    return HAL_OK;
}

/**
  * @brief  Close the I2C transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_Close(struct I2C_HANDLE *pI2C)
{
    HAL_ASSERT(pI2C != NULL);

    I2C_DisableIRQ(pI2C);
    I2C_Disable(pI2C);

    return HAL_OK;
}

/**
  * @brief  Handle I2C write finish for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_WriteFinish(struct I2C_HANDLE *pI2C)
{
    if (READ_REG(pI2C->pReg->IPD) & REG_INT_MBTF) {
        return HAL_OK;
    }

    return HAL_BUSY;
}

/**
  * @brief  Handle I2C write stop finish for transfer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_StopFinish(struct I2C_HANDLE *pI2C)
{
    if (REG_INT_STOP & READ_REG(pI2C->pReg->IPD)) {
        return HAL_OK;
    }

    return HAL_BUSY;
}

/**
  * @brief  Send I2C start TX with u32 buffer.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  buf: data buffer include device addr and register addr.
  * @param  len32: buffer word length.
  * @param  len8: buffer byte length.
  * @return HAL status
  */
HAL_Status HAL_I2C_StartTXU32(struct I2C_HANDLE *pI2C, uint32_t *buf,
                              uint16_t len32, uint16_t len8)
{
    int i;

    HAL_ASSERT(len32 <= MAX_TX_DATA_REGISTER_CNT);

    I2C_CleanIPD(pI2C);

    for (i = 0; i < len32; i++) {
        WRITE_REG(pI2C->pReg->TXDATA[i], buf[i]);
    }

    /* enable adapter with correct mode 0, send START condition */
    WRITE_REG(pI2C->pReg->CON, REG_CON_EN | REG_CON_START | pI2C->cfg);
    WRITE_REG(pI2C->pReg->MTXCNT, len8);

    return HAL_OK;
}

/**
  * @brief  Send I2C start TX.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  addr: device address
  * @param  buf: data buffer pointer
  * @param  len: data byte length
  * @return HAL status
  */
HAL_Status HAL_I2C_StartTX(struct I2C_HANDLE *pI2C, uint16_t addr,
                           uint8_t *buf, uint16_t len)
{
    uint32_t i, j;
    uint32_t val, cnt = 0;
    uint8_t byte;

    pI2C->processed = 0;
    I2C_CleanIPD(pI2C);
    for (i = 0; i < MAX_TX_DATA_REGISTER_CNT; ++i) {
        val = 0;
        for (j = 0; j < 4; ++j) {
            if ((pI2C->processed == len) && (cnt != 0)) {
                break;
            }

            if (pI2C->processed == 0 && cnt == 0) {
                byte = (addr & 0x7f) << 1;
            } else {
                byte = buf[pI2C->processed++];
            }

            val |= byte << (j * 8);
            cnt++;
        }

        WRITE_REG(pI2C->pReg->TXDATA[i], val);
        if (pI2C->processed == len) {
            break;
        }
    }

    /* enable adapter with correct mode 0, send START condition */
    WRITE_REG(pI2C->pReg->CON, REG_CON_EN | REG_CON_START | pI2C->cfg);
    WRITE_REG(pI2C->pReg->MTXCNT, cnt);

    return HAL_OK;
}

/**
  * @brief  Send I2C stop TX.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_StopTX(struct I2C_HANDLE *pI2C)
{
    uint32_t ctrl;

    ctrl = READ_REG(pI2C->pReg->CON);
    ctrl &= ~REG_CON_START;
    ctrl |= REG_CON_STOP;
    WRITE_REG(pI2C->pReg->CON, ctrl);

    return HAL_OK;
}

/**
  * @brief  Send I2C close TX.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_CloseTX(struct I2C_HANDLE *pI2C)
{
    WRITE_REG(pI2C->pReg->MTXCNT, 0);
    WRITE_REG(pI2C->pReg->CON, 0);

    return HAL_OK;
}

/** @} */

/** @defgroup I2C_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
  * @brief  Initialize the I2C according to the specified parameters.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @param  pReg: I2C controller register base address.
  * @param  rate: I2C bus input clock rate.
  * @param  speed: I2C bus output speed.
  * @return HAL status
  */
HAL_Status HAL_I2C_Init(struct I2C_HANDLE *pI2C, struct I2C_REG *pReg, uint32_t rate,
                        eI2C_BusSpeed speed)
{
    /* Check the I2C handle allocation */
    HAL_ASSERT(pI2C != NULL);

    pI2C->pReg = pReg;
    pI2C->speed = speed;
    HAL_ASSERT(IS_I2C_INSTANCE(pI2C->pReg));

    /* Init speed */
    return HAL_I2C_AdaptDIV(pI2C, rate);
}

/**
  * @brief  De Initialize the I2C peripheral.
  * @param  pI2C: pointer to a I2C_HANDLE structure that contains
  *               the information for I2C module.
  * @return HAL status
  */
HAL_Status HAL_I2C_DeInit(struct I2C_HANDLE *pI2C)
{
    /* TO-DO */
    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_I2C_MODULE_ENABLED */
