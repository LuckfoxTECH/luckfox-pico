/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_I2STDM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup I2STDM
 *  @{
 */

/** @defgroup I2STDM_How_To_Use How To Use
 *  @{

 The I2STDM driver can be used as follows:

 @} */

/** @defgroup I2STDM_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define I2STDM_DMA_BURST_SIZE (8) /* size * width: 8*4 = 32 bytes */

/*
 * TXCR
 * transmit operation control register
 */
#define I2STDM_TXCR_PATH_SHIFT(x) (23 + (x) * 2)
#define I2STDM_TXCR_PATH_MASK(x)  (0x3 << I2STDM_TXCR_PATH_SHIFT(x))
#define I2STDM_TXCR_PATH(x, v)    ((v) << I2STDM_TXCR_PATH_SHIFT(x))
#define I2STDM_TXCR_CSR(x)        (x << I2STDM_TXCR_TCSR_SHIFT)
#define I2STDM_TXCR_HWT           (1 << I2STDM_TXCR_HWT_SHIFT)
#define I2STDM_TXCR_SJM_R         (0 << I2STDM_TXCR_SJM_SHIFT)
#define I2STDM_TXCR_SJM_L         (1 << I2STDM_TXCR_SJM_SHIFT)
#define I2STDM_TXCR_FBM_MSB       (0 << I2STDM_TXCR_FBM_SHIFT)
#define I2STDM_TXCR_FBM_LSB       (1 << I2STDM_TXCR_FBM_SHIFT)
#define I2STDM_TXCR_IBM_NORMAL    (0 << I2STDM_TXCR_IBM_SHIFT)
#define I2STDM_TXCR_IBM_LSJM      (1 << I2STDM_TXCR_IBM_SHIFT)
#define I2STDM_TXCR_IBM_RSJM      (2 << I2STDM_TXCR_IBM_SHIFT)
#define I2STDM_TXCR_PBM_MODE(x)   ((x) << I2STDM_TXCR_PBM_SHIFT)
#define I2STDM_TXCR_TFS_I2S       (0 << I2STDM_TXCR_TFS_SHIFT)
#define I2STDM_TXCR_TFS_PCM       (1 << I2STDM_TXCR_TFS_SHIFT)
#define I2STDM_TXCR_TFS_TDM_PCM   (2 << I2STDM_TXCR_TFS_SHIFT)
#define I2STDM_TXCR_TFS_TDM_I2S   (3 << I2STDM_TXCR_TFS_SHIFT)
#define I2STDM_TXCR_VDW(x)        ((x - 1) << I2STDM_TXCR_VDW_SHIFT)

/*
 * RXCR
 * receive operation control register
 */
#define I2STDM_RXCR_PATH_SHIFT(x) (17 + (x) * 2)
#define I2STDM_RXCR_PATH_MASK(x)  (0x3 << I2STDM_RXCR_PATH_SHIFT(x))
#define I2STDM_RXCR_PATH(x, v)    ((v) << I2STDM_RXCR_PATH_SHIFT(x))
#define I2STDM_RXCR_CSR(x)        (x << I2STDM_RXCR_RCSR_SHIFT)
#define I2STDM_RXCR_HWT           (1 << I2STDM_RXCR_HWT_SHIFT)
#define I2STDM_RXCR_SJM_R         (0 << I2STDM_RXCR_SJM_SHIFT)
#define I2STDM_RXCR_SJM_L         (1 << I2STDM_RXCR_SJM_SHIFT)
#define I2STDM_RXCR_FBM_MSB       (0 << I2STDM_RXCR_FBM_SHIFT)
#define I2STDM_RXCR_FBM_LSB       (1 << I2STDM_RXCR_FBM_SHIFT)
#define I2STDM_RXCR_IBM_NORMAL    (0 << I2STDM_RXCR_IBM_SHIFT)
#define I2STDM_RXCR_IBM_LSJM      (1 << I2STDM_RXCR_IBM_SHIFT)
#define I2STDM_RXCR_IBM_RSJM      (2 << I2STDM_RXCR_IBM_SHIFT)
#define I2STDM_RXCR_PBM_MODE(x)   ((x) << I2STDM_RXCR_PBM_SHIFT)
#define I2STDM_RXCR_TFS_I2S       (0 << I2STDM_RXCR_TFS_SHIFT)
#define I2STDM_RXCR_TFS_PCM       (1 << I2STDM_RXCR_TFS_SHIFT)
#define I2STDM_RXCR_TFS_TDM_PCM   (2 << I2STDM_RXCR_TFS_SHIFT)
#define I2STDM_RXCR_TFS_TDM_I2S   (3 << I2STDM_RXCR_TFS_SHIFT)
#define I2STDM_RXCR_VDW(x)        ((x - 1) << I2STDM_RXCR_VDW_SHIFT)

/*
 * CKR
 * clock generation register
 */
#define I2STDM_CKR_TRCM(x)     (x << I2STDM_CKR_TRCM_SHIFT)
#define I2STDM_CKR_TRCM_TXRX   (0 << I2STDM_CKR_TRCM_SHIFT)
#define I2STDM_CKR_TRCM_TXONLY (1 << I2STDM_CKR_TRCM_SHIFT)
#define I2STDM_CKR_TRCM_RXONLY (2 << I2STDM_CKR_TRCM_SHIFT)
#define I2STDM_CKR_MSS_MASTER  (0 << I2STDM_CKR_MSS_SHIFT)
#define I2STDM_CKR_MSS_SLAVE   (1 << I2STDM_CKR_MSS_SHIFT)
#define I2STDM_CKR_CKP_NEG     (0 << I2STDM_CKR_CKP_SHIFT)
#define I2STDM_CKR_CKP_POS     (1 << I2STDM_CKR_CKP_SHIFT)
#define I2STDM_CKR_RLP_NORMAL  (0 << I2STDM_CKR_RLP_SHIFT)
#define I2STDM_CKR_RLP_OPPSITE (1 << I2STDM_CKR_RLP_SHIFT)
#define I2STDM_CKR_TLP_NORMAL  (0 << I2STDM_CKR_TLP_SHIFT)
#define I2STDM_CKR_TLP_OPPSITE (1 << I2STDM_CKR_TLP_SHIFT)
#define I2STDM_CKR_MDIV(x)     ((x - 1) << I2STDM_CKR_MDIV_SHIFT)
#define I2STDM_CKR_RSD(x)      ((x - 1) << I2STDM_CKR_RSD_SHIFT)
#define I2STDM_CKR_TSD(x)      ((x - 1) << I2STDM_CKR_TSD_SHIFT)

/*
 * DMACR
 * DMA control register
 */
#define I2STDM_DMACR_RDE_DISABLE (0 << I2STDM_DMACR_RDE_SHIFT)
#define I2STDM_DMACR_RDE_ENABLE  (1 << I2STDM_DMACR_RDE_SHIFT)
#define I2STDM_DMACR_RDL(x)      ((x - 1) << I2STDM_DMACR_RDL_SHIFT)
#define I2STDM_DMACR_TDE_DISABLE (0 << I2STDM_DMACR_TDE_SHIFT)
#define I2STDM_DMACR_TDE_ENABLE  (1 << I2STDM_DMACR_TDE_SHIFT)
#define I2STDM_DMACR_TDL(x)      ((x) << I2STDM_DMACR_TDL_SHIFT)

/*
 * XFER
 * Transfer start register
 */
#define I2STDM_XFER_RXS_STOP  (0 << I2STDM_XFER_RXS_SHIFT)
#define I2STDM_XFER_RXS_START (1 << I2STDM_XFER_RXS_SHIFT)
#define I2STDM_XFER_TXS_STOP  (0 << I2STDM_XFER_TXS_SHIFT)
#define I2STDM_XFER_TXS_START (1 << I2STDM_XFER_TXS_SHIFT)

/*
 * CLR
 * clear SCLK domain logic register
 */
#define I2STDM_CLR_RXC (1 << I2STDM_CLR_RXC_SHIFT)
#define I2STDM_CLR_TXC (1 << I2STDM_CLR_TXC_SHIFT)

/* channel select */
#define I2STDM_CSR_SHIFT 15
#define I2STDM_CHN_2     (0 << I2STDM_CSR_SHIFT)
#define I2STDM_CHN_4     (1 << I2STDM_CSR_SHIFT)
#define I2STDM_CHN_6     (2 << I2STDM_CSR_SHIFT)
#define I2STDM_CHN_8     (3 << I2STDM_CSR_SHIFT)

/*
 * TDM_CTRL
 * TDM ctrl register
 */
#define TDM_FSYNC_WIDTH_SEL1_SHIFT 18
#define TDM_FSYNC_WIDTH_SEL1_MSK   (0X7 << TDM_FSYNC_WIDTH_SEL1_SHIFT)
#define TDM_FSYNC_WIDTH_SEL1(x)    ((x - 1) << TDM_FSYNC_WIDTH_SEL1_SHIFT)
#define TDM_FSYNC_WIDTH_SEL0_MSK   HAL_BIT(17)
#define TDM_FSYNC_WIDTH_HALF_FRAME 0
#define TDM_FSYNC_WIDTH_ONE_FRAME  HAL_BIT(17)
#define TDM_SHIFT_CTRL_SHIFT       14
#define TDM_SHIFT_CTRL_MSK         (0x7 << TDM_SHIFT_CTRL_SHIFT)
#define TDM_SHIFT_CTRL(x)          ((x) << TDM_SHIFT_CTRL_SHIFT)
#define TDM_SLOT_BIT_WIDTH_SHIFT   9
#define TDM_SLOT_BIT_WIDTH_MSK     (0x1f << TDM_SLOT_BIT_WIDTH_SHIFT)
#define TDM_SLOT_BIT_WIDTH(x)      ((x - 1) << TDM_SLOT_BIT_WIDTH_SHIFT)
#define TDM_FRAME_WIDTH_SHIFT      0
#define TDM_FRAME_WIDTH_MSK        (0x1ff << TDM_FRAME_WIDTH_SHIFT)
#define TDM_FRAME_WIDTH(x)         ((x - 1) << TDM_FRAME_WIDTH_SHIFT)

/*
 * CLKDIV
 * Mclk div register
 */
#define I2STDM_CLKDIV_TX_MDIV(x) ((x - 1) << I2STDM_CLKDIV_TX_MDIV_SHIFT)
#define I2STDM_CLKDIV_RX_MDIV(x) ((x - 1) << I2STDM_CLKDIV_RX_MDIV_SHIFT)

#ifdef HAL_I2STDM_TDM_MULTI_LANES
/*
 * Example: RK3308
 *
 * Use I2S0_2CH as Clk-Gen to serve TDM_MULTI_LANES
 *
 * I2S0_2CH ----> BCLK,I2S_LRCK --------> I2S_8CH_0/1_RX/TX (Slave TRCM-NONE)
 *     |
 *     |--------> BCLK,TDM_SYNC --------> TDM Device  (Slave)
 *
 * Note:
 *
 * I2S0_2CH_MCLK: BCLK
 * I2S0_2CH_SCLK: I2S_LRCK (GPIO4_B5)
 * I2S0_2CH_LRCK: TDM_SYNC (GPIO4_B6)
 *
 * --- a/lib/bsp/RK3308/hal_bsp.c
 * +++ b/lib/bsp/RK3308/hal_bsp.c
 * @@ -171,6 +171,13 @@ struct HAL_I2STDM_DEV g_i2sTdm1Dev =
 *          .dmaReqCh = DMA_REQ_I2S_8CH_1_TX,
 *          .dmac = DMA1,
 *      },
 * +#ifdef HAL_I2STDM_TDM_MULTI_LANES
 * +    .gpioI2sLrck = GPIO4;
 * +    .gpioTdmFsync = GPIO4;
 * +    .pinI2sLrck = GPIO_PIN_B5;
 * +    .pinTdmFsync = GPIO_PIN_B6;
 * +    .isTdmMultiLanes = true;
 * +#endif
 */

#define REF_TIMEOUT_COUNT 1000
#endif

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

static HAL_Status I2STDM_SetSampleRate(struct HAL_I2STDM_DEV *i2sTdm,
                                       eAUDIO_streamType stream,
                                       eAUDIO_sampleRate sampleRate)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;
    uint32_t mclkRate, bclkRate, divBclk, divLrck;
    HAL_Status ret = HAL_OK;

    if (i2sTdm->trcmMode) {
        if (i2sTdm->trcmMode == TRCM_TXONLY) {
            mclkRate = HAL_CRU_ClkGetFreq(i2sTdm->mclkTx);
        } else {
            mclkRate = HAL_CRU_ClkGetFreq(i2sTdm->mclkRx);
        }

        bclkRate = i2sTdm->bclkFs * sampleRate;
        HAL_ASSERT(bclkRate != 0);
        divBclk = HAL_DivRoundClosest(mclkRate, bclkRate);
        divLrck = bclkRate / sampleRate;
        MODIFY_REG(reg->CLKDIV,
                   I2STDM_CLKDIV_TX_MDIV_MASK | I2STDM_CLKDIV_RX_MDIV_MASK,
                   I2STDM_CLKDIV_TX_MDIV(divBclk) | I2STDM_CLKDIV_RX_MDIV(divBclk));
        MODIFY_REG(reg->CKR,
                   I2STDM_CKR_TSD_MASK | I2STDM_CKR_RSD_MASK,
                   I2STDM_CKR_TSD(divLrck) | I2STDM_CKR_RSD(divLrck));
    } else {
        if (stream == AUDIO_STREAM_PLAYBACK) {
            mclkRate = HAL_CRU_ClkGetFreq(i2sTdm->mclkTx);
        } else {
            mclkRate = HAL_CRU_ClkGetFreq(i2sTdm->mclkRx);
        }

        bclkRate = i2sTdm->bclkFs * sampleRate;
        HAL_ASSERT(bclkRate != 0);
        divBclk = HAL_DivRoundClosest(mclkRate, bclkRate);
        divLrck = bclkRate / sampleRate;

        if (stream == AUDIO_STREAM_PLAYBACK) {
            MODIFY_REG(reg->CLKDIV,
                       I2STDM_CLKDIV_TX_MDIV_MASK,
                       I2STDM_CLKDIV_TX_MDIV(divBclk));
            MODIFY_REG(reg->CKR,
                       I2STDM_CKR_TSD_MASK,
                       I2STDM_CKR_TSD(divLrck));
        } else {
            MODIFY_REG(reg->CLKDIV,
                       I2STDM_CLKDIV_RX_MDIV_MASK,
                       I2STDM_CLKDIV_RX_MDIV(divBclk));
            MODIFY_REG(reg->CKR,
                       I2STDM_CKR_RSD_MASK,
                       I2STDM_CKR_RSD(divLrck));
        }
    }

    return ret;
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup I2STDM_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  i2sTdm suspend.
 * @param  i2sTdm: the handle of i2sTdm.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_Supsend(struct HAL_I2STDM_DEV *i2sTdm)
{
    return HAL_OK;
}

/**
 * @brief  i2sTdm resume.
 * @param  i2sTdm: the handle of i2sTdm.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_Resume(struct HAL_I2STDM_DEV *i2sTdm)
{
    return HAL_OK;
}

/** @} */

/** @defgroup I2STDM_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup I2STDM_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup I2STDM_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Init i2sTdm controller.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  config: init config for i2sTdm init.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_Init(struct HAL_I2STDM_DEV *i2sTdm, struct AUDIO_INIT_CONFIG *config)
{
    uint32_t mask = 0, val = 0, tdmVal = 0;
    bool isMaster = config->master;
    bool clkInvert = config->clkInvert;
    bool frmInvert = config->frameInvert;
    uint16_t rxMap = config->rxMap;
    uint16_t txMap = config->txMap;
    struct I2STDM_REG *reg = i2sTdm->pReg;

    mask = I2STDM_TXCR_TFS_MASK;

    switch (config->format) {
    case AUDIO_FMT_I2S:
        val = I2STDM_TXCR_TFS_I2S;
        break;

    case AUDIO_FMT_LEFT_J:
        val = I2STDM_TXCR_TFS_I2S | I2STDM_TXCR_IBM_LSJM;
        mask |= I2STDM_TXCR_IBM_MASK;
        break;

    case AUDIO_FMT_RIGHT_J:
        val = I2STDM_TXCR_TFS_I2S | I2STDM_TXCR_IBM_RSJM;
        mask |= I2STDM_TXCR_IBM_MASK;
        break;

    case AUDIO_FMT_PCM:
    /* fallthrough */
    case AUDIO_FMT_PCM_DELAY1:
    /* fallthrough */
    case AUDIO_FMT_PCM_DELAY2:
    /* fallthrough */
    case AUDIO_FMT_PCM_DELAY3:
        mask |= I2STDM_TXCR_PBM_MASK;
        val = I2STDM_TXCR_TFS_PCM |
              I2STDM_TXCR_PBM_MODE(config->format - AUDIO_FMT_PCM);
        break;

    case AUDIO_FMT_TDM_PCM:
    /* fallthrough */
    case AUDIO_FMT_TDM_PCM_L_SHIFT_MODE0:
    /* fallthrough */
    case AUDIO_FMT_TDM_PCM_L_SHIFT_MODE1:
    /* fallthrough */
    case AUDIO_FMT_TDM_PCM_L_SHIFT_MODE2:
    /* fallthrough */
    case AUDIO_FMT_TDM_PCM_L_SHIFT_MODE3:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_PCM;
        tdmVal = TDM_SHIFT_CTRL(config->format - AUDIO_FMT_TDM_PCM);
        break;

    case AUDIO_FMT_TDM_I2S_HALF_FRAME:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_I2S;
        tdmVal = TDM_SHIFT_CTRL(0) | TDM_FSYNC_WIDTH_HALF_FRAME;
        break;

    case AUDIO_FMT_TDM_I2S_ONE_FRAME:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_I2S;
        tdmVal = TDM_SHIFT_CTRL(0) | TDM_FSYNC_WIDTH_ONE_FRAME;
        break;

    case AUDIO_FMT_TDM_LEFT_J_HALF_FRAME:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_I2S;
        tdmVal = TDM_SHIFT_CTRL(1) | TDM_FSYNC_WIDTH_HALF_FRAME;
        break;

    case AUDIO_FMT_TDM_LEFT_J_ONE_FRAME:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_I2S;
        tdmVal = TDM_SHIFT_CTRL(1) | TDM_FSYNC_WIDTH_ONE_FRAME;
        break;

    case AUDIO_FMT_TDM_RIGHT_J_HALF_FRAME:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_I2S;
        tdmVal = TDM_SHIFT_CTRL(2) | TDM_FSYNC_WIDTH_HALF_FRAME;
        break;

    case AUDIO_FMT_TDM_RIGHT_J_ONE_FRAME:
        i2sTdm->isTdm = true;
        val = I2STDM_TXCR_TFS_TDM_I2S;
        tdmVal = TDM_SHIFT_CTRL(2) | TDM_FSYNC_WIDTH_ONE_FRAME;
        break;

    default:
        val = I2STDM_TXCR_TFS_I2S;
        break;
    }

    MODIFY_REG(reg->TXCR, mask, val);
    MODIFY_REG(reg->RXCR, mask, val);

    if (i2sTdm->isTdm) {
        mask = TDM_FSYNC_WIDTH_SEL0_MSK | TDM_SHIFT_CTRL_MSK;
        MODIFY_REG(reg->TDM_TXCTRL, mask, tdmVal);
        MODIFY_REG(reg->TDM_RXCTRL, mask, tdmVal);
    }

    mask = I2STDM_CKR_MSS_MASK;
    val = isMaster ? I2STDM_CKR_MSS_MASTER : I2STDM_CKR_MSS_SLAVE;
    MODIFY_REG(reg->CKR, mask, val);

    mask = I2STDM_CKR_CKP_MASK;
    val = clkInvert ? I2STDM_CKR_CKP_POS : I2STDM_CKR_CKP_NEG;
    MODIFY_REG(reg->CKR, mask, val);

    mask = I2STDM_CKR_RLP_MASK | I2STDM_CKR_TLP_MASK;
    val = frmInvert ? I2STDM_CKR_RLP_OPPSITE | I2STDM_CKR_TLP_OPPSITE :
                      I2STDM_CKR_RLP_NORMAL | I2STDM_CKR_TLP_NORMAL;
    MODIFY_REG(reg->CKR, mask, val);

    HAL_ASSERT(config->trcmMode <= TRCM_RXONLY);
    MODIFY_REG(reg->CKR, I2STDM_CKR_LRCK_COMMON_MASK,
               config->trcmMode << I2STDM_CKR_LRCK_COMMON_SHIFT);

    if (config->trcmMode == TRCM_TXONLY) {
        if (i2sTdm->muxTxSel) {
            HAL_CRU_ClkSetMux(i2sTdm->muxTxSel, 1);
        }
    } else if (config->trcmMode == TRCM_RXONLY) {
        if (i2sTdm->muxRxSel) {
            HAL_CRU_ClkSetMux(i2sTdm->muxRxSel, 1);
        }
    }
    i2sTdm->trcmMode = isMaster ? config->trcmMode : TRCM_NONE;

    /* channel re-mapping */
    if (txMap) {
        MODIFY_REG(reg->TXCR,
                   I2STDM_TXCR_PATH_MASK(0) |
                   I2STDM_TXCR_PATH_MASK(1) |
                   I2STDM_TXCR_PATH_MASK(2) |
                   I2STDM_TXCR_PATH_MASK(3),
                   I2STDM_TXCR_PATH(0, txMap & 0x3) |
                   I2STDM_TXCR_PATH(1, (txMap >> 4) & 0x3) |
                   I2STDM_TXCR_PATH(2, (txMap >> 8) & 0x3) |
                   I2STDM_TXCR_PATH(3, (txMap >> 12) & 0x3));
    }

    if (rxMap) {
        MODIFY_REG(reg->RXCR,
                   I2STDM_RXCR_PATH_MASK(0) |
                   I2STDM_RXCR_PATH_MASK(1) |
                   I2STDM_RXCR_PATH_MASK(2) |
                   I2STDM_RXCR_PATH_MASK(3),
                   I2STDM_RXCR_PATH(0, rxMap & 0x3) |
                   I2STDM_RXCR_PATH(1, (rxMap >> 4) & 0x3) |
                   I2STDM_RXCR_PATH(2, (rxMap >> 8) & 0x3) |
                   I2STDM_RXCR_PATH(3, (rxMap >> 12) & 0x3));
    }

    return HAL_OK;
}

/**
 * @brief  DeInit i2sTdm controller.
 * @param  i2sTdm: the handle of i2sTdm.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_DeInit(struct HAL_I2STDM_DEV *i2sTdm)
{
    /* Do nothing. */
    return HAL_OK;
}

/** @} */

/** @defgroup I2STDM_Exported_Functions_Group5 Other Functions
 *  @{
 */

#ifdef HAL_I2STDM_TDM_MULTI_LANES
#define NSAMPLES 4
HAL_UNUSED static void HAL_I2STDM_GpioClkMeas(struct GPIO_REG *pGPIO,
                                              ePINCTRL_GPIO_PINS pin)
{
    int h[NSAMPLES], l[NSAMPLES], i;

    for (i = 0; i < NSAMPLES; i++) {
        h[i] = 0; l[i] = 0;
        while (HAL_GPIO_GetPinLevel(pGPIO, pin)) {
            h[i]++;
        }
        while (!HAL_GPIO_GetPinLevel(pGPIO, pin)) {
            l[i]++;
        }
    }

    for (i = 0; i < NSAMPLES; i++) {
        HAL_DBG_ERR("H[%d]: %2d, L[%d]: %2d\n", i, h[i], i, l[i]);
    }
}

static void HAL_I2STDM_UntilGpioClkSync(struct HAL_I2STDM_DEV *i2sTdm,
                                        eAUDIO_streamType stream,
                                        int *high, int *low, int *high2, int *low2)
{
    int h = 0, l = 0, h2 = 0, l2 = 0, dc_h = 0, dc_l = 0;

    //HAL_I2STDM_GpioClkMeas(i2sTdm->gpioTdmFsync, i2sTdm->pinTdmFsync);
    //HAL_I2STDM_GpioClkMeas(i2sTdm->gpioI2sLrck, i2sTdm->pinI2sLrck);

    /*
     * debounce: at least one cycle found, otherwise, the clk ref maybe
     * not on the fly.
     */
    /* check HIGH-Level */
    while (HAL_GPIO_GetPinLevel(i2sTdm->gpioTdmFsync, i2sTdm->pinTdmFsync)) {
        dc_h++;
        if (dc_h >= REF_TIMEOUT_COUNT) {
            HAL_DBG("Failed to find HIGH-Level\n");

            return;
        }
    }
    /* check LOW-Level */
    while (!HAL_GPIO_GetPinLevel(i2sTdm->gpioTdmFsync, i2sTdm->pinTdmFsync)) {
        dc_l++;
        if (dc_l >= REF_TIMEOUT_COUNT) {
            HAL_DBG("Failed to find LOW-Level\n");

            return;
        }
    }

    while (HAL_GPIO_GetPinLevel(i2sTdm->gpioTdmFsync, i2sTdm->pinTdmFsync)) {
        h++;
    }

    while (!HAL_GPIO_GetPinLevel(i2sTdm->gpioTdmFsync, i2sTdm->pinTdmFsync)) {
        l++;
    }

    while (!HAL_GPIO_GetPinLevel(i2sTdm->gpioI2sLrck, i2sTdm->pinI2sLrck)) {
        l2++;
    }

    if (stream == AUDIO_STREAM_CAPTURE) {
        while (HAL_GPIO_GetPinLevel(i2sTdm->gpioI2sLrck, i2sTdm->pinI2sLrck)) {
            h2++;
        }
    }

    *high = h;
    *high2 = h2;
    *low = l;
    *low2 = l2;
}

/**
 * @brief  Enable i2sTdm multi lanes.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 * @note   Must be invoked with irq disabled for atomic ops.
 */
static HAL_Status HAL_I2STDM_MultiLanesEnable(struct HAL_I2STDM_DEV *i2sTdm,
                                              eAUDIO_streamType stream)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;
    int h = 0, l = 0, h2 = 0, l2 = 0;

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_TDE_MASK, I2STDM_DMACR_TDE_ENABLE);
        HAL_DelayUs(1);
        HAL_I2STDM_UntilGpioClkSync(i2sTdm, stream, &h, &l, &h2, &l2);
        MODIFY_REG(reg->XFER, I2STDM_XFER_TXS_MASK, I2STDM_XFER_TXS_START);
    } else {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_RDE_MASK, I2STDM_DMACR_RDE_ENABLE);
        HAL_I2STDM_UntilGpioClkSync(i2sTdm, stream, &h, &l, &h2, &l2);
        MODIFY_REG(reg->XFER, I2STDM_XFER_RXS_MASK, I2STDM_XFER_RXS_START);
    }

    HAL_DBG("STREAM[%d]: HIGH: %d, LOW: %d, HIGH2: %d, LOW2: %d\n",
            stream, h, l, h2, l2);

    return HAL_OK;
}
#endif

/**
 * @brief  Enable i2sTdm controller.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_Enable(struct HAL_I2STDM_DEV *i2sTdm, eAUDIO_streamType stream)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;

#ifdef HAL_I2STDM_TDM_MULTI_LANES
    if (i2sTdm->isTdmMultiLanes) {
        return HAL_I2STDM_MultiLanesEnable(i2sTdm, stream);
    }
#endif

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_TDE_MASK, I2STDM_DMACR_TDE_ENABLE);
        MODIFY_REG(reg->XFER, I2STDM_XFER_TXS_MASK, I2STDM_XFER_TXS_START);
    } else {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_RDE_MASK, I2STDM_DMACR_RDE_ENABLE);
        MODIFY_REG(reg->XFER, I2STDM_XFER_RXS_MASK, I2STDM_XFER_RXS_START);
    }

    return HAL_OK;
}

/**
 * @brief  Disable i2sTdm controller.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_Disable(struct HAL_I2STDM_DEV *i2sTdm, eAUDIO_streamType stream)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_TDE_MASK, I2STDM_DMACR_TDE_DISABLE);
        MODIFY_REG(reg->XFER, I2STDM_XFER_TXS_MASK, I2STDM_XFER_TXS_STOP);
        HAL_DelayUs(150);
        MODIFY_REG(reg->CLR, I2STDM_CLR_TXC_MASK, I2STDM_CLR_TXC);
    } else {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_RDE_MASK, I2STDM_DMACR_RDE_DISABLE);
        MODIFY_REG(reg->XFER, I2STDM_XFER_RXS_MASK, I2STDM_XFER_RXS_STOP);
        HAL_DelayUs(150);
        MODIFY_REG(reg->CLR, I2STDM_CLR_RXC_MASK, I2STDM_CLR_RXC);
    }

    return HAL_OK;
}

/**
 * @brief  Enable i2sTdm controller under trcm conditions.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @param  doXfer: whether start to handle xfer registers.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_TxRxEnable(struct HAL_I2STDM_DEV *i2sTdm, eAUDIO_streamType stream,
                                 bool doXfer)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_TDE_MASK, I2STDM_DMACR_TDE_ENABLE);
    } else {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_RDE_MASK, I2STDM_DMACR_RDE_ENABLE);
    }

    if (doXfer) {
        HAL_CRU_ClkResetSyncAssert(2, i2sTdm->rsts);
        HAL_DelayUs(10);
        MODIFY_REG(reg->XFER, I2STDM_XFER_TXS_MASK | I2STDM_XFER_RXS_MASK,
                   I2STDM_XFER_TXS_START | I2STDM_XFER_RXS_START);
        HAL_CRU_ClkResetSyncDeassert(2, i2sTdm->rsts);
    }

    return HAL_OK;
}

/**
 * @brief  Disable i2sTdm controller under trcm conditions.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @param  doXfer: whether start to handle xfer registers.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_TxRxDisable(struct HAL_I2STDM_DEV *i2sTdm, eAUDIO_streamType stream,
                                  bool doXfer)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_TDE_MASK, I2STDM_DMACR_TDE_DISABLE);
    } else {
        MODIFY_REG(reg->DMACR, I2STDM_DMACR_RDE_MASK, I2STDM_DMACR_RDE_DISABLE);
    }

    if (doXfer) {
        MODIFY_REG(reg->XFER, I2STDM_XFER_TXS_MASK | I2STDM_XFER_RXS_MASK,
                   I2STDM_XFER_TXS_STOP | I2STDM_XFER_RXS_STOP);
        HAL_DelayUs(150);
        MODIFY_REG(reg->CLR, I2STDM_CLR_TXC_MASK | I2STDM_CLR_RXC_MASK,
                   I2STDM_CLR_TXC | I2STDM_CLR_RXC);
    }

    return HAL_OK;
}

/**
 * @brief  Config i2sTdm controller.
 * @param  i2sTdm: the handle of i2sTdm.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @param  params: audio params.
 * @return HAL_Status
 */
HAL_Status HAL_I2STDM_Config(struct HAL_I2STDM_DEV *i2sTdm, eAUDIO_streamType stream,
                             struct AUDIO_PARAMS *params)
{
    struct I2STDM_REG *reg = i2sTdm->pReg;
    uint32_t val = 0, mask = 0;
    HAL_Status ret = HAL_OK;
    bool isMaster;

    if (i2sTdm->isTdm) {
        val = READ_BIT(reg->TDM_TXCTRL, TDM_SLOT_BIT_WIDTH_MSK) >> TDM_SLOT_BIT_WIDTH_SHIFT;
        i2sTdm->bclkFs = params->channels * (val + 1);
        mask = TDM_FRAME_WIDTH_MSK;
        val = TDM_FRAME_WIDTH(i2sTdm->bclkFs);
        MODIFY_REG(reg->TDM_TXCTRL, mask, val);
        MODIFY_REG(reg->TDM_RXCTRL, mask, val);
    }

    isMaster = (READ_BIT(reg->CKR, I2STDM_CKR_MSS_MASK) == I2STDM_CKR_MSS_MASTER);
    if (isMaster) {
        I2STDM_SetSampleRate(i2sTdm, stream, params->sampleRate);
    }

    switch (params->channels) {
    case 8:
        val = I2STDM_CHN_8;
        break;
    case 6:
        val = I2STDM_CHN_6;
        break;
    case 4:
        val = I2STDM_CHN_4;
        break;
    case 2:
        val = I2STDM_CHN_2;
        break;
    default:

        return HAL_INVAL;
    }

    if (stream == AUDIO_STREAM_CAPTURE) {
        val |= I2STDM_RXCR_VDW(params->sampleBits);
        MODIFY_REG(reg->RXCR,
                   I2STDM_RXCR_VDW_MASK | I2STDM_RXCR_RCSR_MASK,
                   val);
    } else {
        val |= I2STDM_TXCR_VDW(params->sampleBits);
        MODIFY_REG(reg->TXCR,
                   I2STDM_TXCR_VDW_MASK | I2STDM_TXCR_TCSR_MASK,
                   val);
    }

    MODIFY_REG(reg->DMACR, I2STDM_DMACR_TDL_MASK, I2STDM_DMACR_TDL(I2STDM_DMA_BURST_SIZE));
    MODIFY_REG(reg->DMACR, I2STDM_DMACR_RDL_MASK, I2STDM_DMACR_RDL(I2STDM_DMA_BURST_SIZE));

    return ret;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_I2STDM_MODULE_ENABLED */
