/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_I2S_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup I2S
 *  @{
 */

/** @defgroup I2S_How_To_Use How To Use
 *  @{

 The I2S driver can be used as follows:

 @} */

/** @defgroup I2S_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define I2S_DMA_BURST_SIZE (8) /* size * width: 8*4 = 32 bytes */

/*
 * TXCR
 * transmit operation control register
 */
#define I2S_TXCR_PATH_SHIFT(x) (23 + (x) * 2)
#define I2S_TXCR_PATH_MASK(x)  (0x3 << I2S_TXCR_PATH_SHIFT(x))
#define I2S_TXCR_PATH(x, v)    ((v) << I2S_TXCR_PATH_SHIFT(x))
#define I2S_TXCR_CSR(x)        ((x) << I2S_TXCR_TCSR_SHIFT)
#define I2S_TXCR_HWT           (1 << I2S_TXCR_HWT_SHIFT)
#define I2S_TXCR_SJM_R         (0 << I2S_TXCR_SJM_SHIFT)
#define I2S_TXCR_SJM_L         (1 << I2S_TXCR_SJM_SHIFT)
#define I2S_TXCR_FBM_MSB       (0 << I2S_TXCR_FBM_SHIFT)
#define I2S_TXCR_FBM_LSB       (1 << I2S_TXCR_FBM_SHIFT)
#define I2S_TXCR_IBM_NORMAL    (0 << I2S_TXCR_IBM_SHIFT)
#define I2S_TXCR_IBM_LSJM      (1 << I2S_TXCR_IBM_SHIFT)
#define I2S_TXCR_IBM_RSJM      (2 << I2S_TXCR_IBM_SHIFT)
#define I2S_TXCR_PBM_MODE(x)   ((x) << I2S_TXCR_PBM_SHIFT)
#define I2S_TXCR_TFS_I2S       (0 << I2S_TXCR_TFS_SHIFT)
#define I2S_TXCR_TFS_PCM       (1 << I2S_TXCR_TFS_SHIFT)
#define I2S_TXCR_TFS_TDM_PCM   (2 << I2S_TXCR_TFS_SHIFT)
#define I2S_TXCR_TFS_TDM_I2S   (3 << I2S_TXCR_TFS_SHIFT)
#define I2S_TXCR_VDW(x)        (((x) - 1) << I2S_TXCR_VDW_SHIFT)

/*
 * RXCR
 * receive operation control register
 */
#define I2S_RXCR_PATH_SHIFT(x) (17 + (x) * 2)
#define I2S_RXCR_PATH_MASK(x)  (0x3 << I2S_RXCR_PATH_SHIFT(x))
#define I2S_RXCR_PATH(x, v)    ((v) << I2S_RXCR_PATH_SHIFT(x))
#define I2S_RXCR_CSR(x)        ((x) << I2S_RXCR_RCSR_SHIFT)
#define I2S_RXCR_HWT           (1 << I2S_RXCR_HWT_SHIFT)
#define I2S_RXCR_SJM_R         (0 << I2S_RXCR_SJM_SHIFT)
#define I2S_RXCR_SJM_L         (1 << I2S_RXCR_SJM_SHIFT)
#define I2S_RXCR_FBM_MSB       (0 << I2S_RXCR_FBM_SHIFT)
#define I2S_RXCR_FBM_LSB       (1 << I2S_RXCR_FBM_SHIFT)
#define I2S_RXCR_IBM_NORMAL    (0 << I2S_RXCR_IBM_SHIFT)
#define I2S_RXCR_IBM_LSJM      (1 << I2S_RXCR_IBM_SHIFT)
#define I2S_RXCR_IBM_RSJM      (2 << I2S_RXCR_IBM_SHIFT)
#define I2S_RXCR_PBM_MODE(x)   ((x) << I2S_RXCR_PBM_SHIFT)
#define I2S_RXCR_TFS_I2S       (0 << I2S_RXCR_TFS_SHIFT)
#define I2S_RXCR_TFS_PCM       (1 << I2S_RXCR_TFS_SHIFT)
#define I2S_RXCR_TFS_TDM_PCM   (2 << I2S_RXCR_TFS_SHIFT)
#define I2S_RXCR_TFS_TDM_I2S   (3 << I2S_RXCR_TFS_SHIFT)
#define I2S_RXCR_VDW(x)        (((x) - 1) << I2S_RXCR_VDW_SHIFT)

/*
 * CKR
 * clock generation register
 */
#define I2S_CKR_TRCM(x)     ((x) << I2S_CKR_TRCM_SHIFT)
#define I2S_CKR_TRCM_TXRX   (0 << I2S_CKR_TRCM_SHIFT)
#define I2S_CKR_TRCM_TXONLY (1 << I2S_CKR_TRCM_SHIFT)
#define I2S_CKR_TRCM_RXONLY (2 << I2S_CKR_TRCM_SHIFT)
#define I2S_CKR_MSS_MASTER  (0 << I2S_CKR_MSS_SHIFT)
#define I2S_CKR_MSS_SLAVE   (1 << I2S_CKR_MSS_SHIFT)
#define I2S_CKR_CKP_NEG     (0 << I2S_CKR_CKP_SHIFT)
#define I2S_CKR_CKP_POS     (1 << I2S_CKR_CKP_SHIFT)
#define I2S_CKR_RLP_NORMAL  (0 << I2S_CKR_RLP_SHIFT)
#define I2S_CKR_RLP_OPPSITE (1 << I2S_CKR_RLP_SHIFT)
#define I2S_CKR_TLP_NORMAL  (0 << I2S_CKR_TLP_SHIFT)
#define I2S_CKR_TLP_OPPSITE (1 << I2S_CKR_TLP_SHIFT)
#define I2S_CKR_MDIV(x)     (((x) - 1) << I2S_CKR_MDIV_SHIFT)
#define I2S_CKR_RSD(x)      (((x) - 1) << I2S_CKR_RSD_SHIFT)
#define I2S_CKR_TSD(x)      (((x) - 1) << I2S_CKR_TSD_SHIFT)

/*
 * DMACR
 * DMA control register
 */
#define I2S_DMACR_RDE_DISABLE (0 << I2S_DMACR_RDE_SHIFT)
#define I2S_DMACR_RDE_ENABLE  (1 << I2S_DMACR_RDE_SHIFT)
#define I2S_DMACR_RDL(x)      (((x) - 1) << I2S_DMACR_RDL_SHIFT)
#define I2S_DMACR_TDE_DISABLE (0 << I2S_DMACR_TDE_SHIFT)
#define I2S_DMACR_TDE_ENABLE  (1 << I2S_DMACR_TDE_SHIFT)
#define I2S_DMACR_TDL(x)      ((x) << I2S_DMACR_TDL_SHIFT)

/*
 * XFER
 * Transfer start register
 */
#define I2S_XFER_RXS_STOP  (0 << I2S_XFER_RXS_SHIFT)
#define I2S_XFER_RXS_START (1 << I2S_XFER_RXS_SHIFT)
#define I2S_XFER_TXS_STOP  (0 << I2S_XFER_TXS_SHIFT)
#define I2S_XFER_TXS_START (1 << I2S_XFER_TXS_SHIFT)

/*
 * CLR
 * clear SCLK domain logic register
 */
#define I2S_CLR_RXC (1 << I2S_CLR_RXC_SHIFT)
#define I2S_CLR_TXC (1 << I2S_CLR_TXC_SHIFT)

/* channel select */
#define I2S_CSR_SHIFT 15
#define I2S_CHN_2     (0 << I2S_CSR_SHIFT)
#define I2S_CHN_4     (1 << I2S_CSR_SHIFT)
#define I2S_CHN_6     (2 << I2S_CSR_SHIFT)
#define I2S_CHN_8     (3 << I2S_CSR_SHIFT)

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

static HAL_Status I2S_SetSampleRate(struct HAL_I2S_DEV *i2s,
                                    eAUDIO_sampleRate sampleRate)
{
    struct I2S_REG *reg = i2s->pReg;
    uint32_t mclkRate, bclkRate, divBclk, divLrck;
    HAL_Status ret = HAL_OK;

    mclkRate = HAL_CRU_ClkGetFreq(i2s->mclk);
    bclkRate = i2s->bclkFs * sampleRate;
    HAL_ASSERT(bclkRate != 0);
    divBclk = HAL_DivRoundClosest(mclkRate, bclkRate);
    divLrck = bclkRate / sampleRate;

    MODIFY_REG(reg->CKR, I2S_CKR_MDIV_MASK, I2S_CKR_MDIV(divBclk));
    MODIFY_REG(reg->CKR, I2S_CKR_TSD_MASK, I2S_CKR_TSD(divLrck));
    MODIFY_REG(reg->CKR, I2S_CKR_RSD_MASK, I2S_CKR_RSD(divLrck));

    return ret;
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup I2S_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  i2s suspend.
 * @param  i2s: the handle of i2s.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_Supsend(struct HAL_I2S_DEV *i2s)
{
    return HAL_OK;
}

/**
 * @brief  i2s resume.
 * @param  i2s: the handle of i2s.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_Resume(struct HAL_I2S_DEV *i2s)
{
    return HAL_OK;
}

/** @} */

/** @defgroup I2S_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup I2S_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup I2S_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Init i2s controller.
 * @param  i2s: the handle of i2s.
 * @param  config: init config for i2s init.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_Init(struct HAL_I2S_DEV *i2s, struct AUDIO_INIT_CONFIG *config)
{
    uint32_t mask = 0, val = 0;
    bool isMaster = config->master;
    bool clkInvert = config->clkInvert;
    struct I2S_REG *reg = i2s->pReg;

    HAL_CRU_ClkEnable(i2s->hclk);

    mask = I2S_CKR_MSS_MASK;
    val = isMaster ? I2S_CKR_MSS_MASTER : I2S_CKR_MSS_SLAVE;
    MODIFY_REG(reg->CKR, mask, val);

    mask = I2S_CKR_CKP_MASK;
    val = clkInvert ? I2S_CKR_CKP_POS : I2S_CKR_CKP_NEG;
    MODIFY_REG(reg->CKR, mask, val);

    return HAL_OK;
}

/**
 * @brief  DeInit i2s controller.
 * @param  i2s: the handle of i2s.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_DeInit(struct HAL_I2S_DEV *i2s)
{
    HAL_CRU_ClkDisable(i2s->hclk);

    return HAL_OK;
}

/** @} */

/** @defgroup I2S_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable i2s controller.
 * @param  i2s: the handle of i2s.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_Enable(struct HAL_I2S_DEV *i2s, eAUDIO_streamType stream)
{
    struct I2S_REG *reg = i2s->pReg;

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2S_DMACR_TDE_MASK, I2S_DMACR_TDE_ENABLE);
        MODIFY_REG(reg->XFER, I2S_XFER_TXS_MASK, I2S_XFER_TXS_START);
    } else {
        MODIFY_REG(reg->DMACR, I2S_DMACR_RDE_MASK, I2S_DMACR_RDE_ENABLE);
        MODIFY_REG(reg->XFER, I2S_XFER_RXS_MASK, I2S_XFER_RXS_START);
    }

    return HAL_OK;
}

/**
 * @brief  Disable i2s controller.
 * @param  i2s: the handle of i2s.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_Disable(struct HAL_I2S_DEV *i2s, eAUDIO_streamType stream)
{
    struct I2S_REG *reg = i2s->pReg;

    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->DMACR, I2S_DMACR_TDE_MASK, I2S_DMACR_TDE_DISABLE);
        MODIFY_REG(reg->XFER, I2S_XFER_TXS_MASK, I2S_XFER_TXS_STOP);
        HAL_DelayUs(150);
        MODIFY_REG(reg->CLR, I2S_CLR_TXC_MASK, I2S_CLR_TXC);
    } else {
        MODIFY_REG(reg->DMACR, I2S_DMACR_RDE_MASK, I2S_DMACR_RDE_DISABLE);
        MODIFY_REG(reg->XFER, I2S_XFER_RXS_MASK, I2S_XFER_RXS_STOP);
        HAL_DelayUs(150);
        MODIFY_REG(reg->CLR, I2S_CLR_RXC_MASK, I2S_CLR_RXC);
    }

    return HAL_OK;
}

/**
 * @brief  Config i2s controller.
 * @param  i2s: the handle of i2s.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @param  params: audio params.
 * @return HAL_Status
 */
HAL_Status HAL_I2S_Config(struct HAL_I2S_DEV *i2s, eAUDIO_streamType stream,
                          struct AUDIO_PARAMS *params)
{
    struct I2S_REG *reg = i2s->pReg;
    uint32_t val = 0;
    HAL_Status ret = HAL_OK;
    bool isMaster;

    isMaster = (READ_BIT(reg->CKR, I2S_CKR_MSS_MASK) == I2S_CKR_MSS_MASTER);
    if (isMaster) {
        I2S_SetSampleRate(i2s, params->sampleRate);
    }

    val = I2S_TXCR_VDW(params->sampleBits);
    switch (params->channels) {
    case 8:
        val |= I2S_CHN_8;
        break;
    case 6:
        val |= I2S_CHN_6;
        break;
    case 4:
        val |= I2S_CHN_4;
        break;
    case 2:
        val |= I2S_CHN_2;
        break;
    default:

        return HAL_INVAL;
    }

    if (stream == AUDIO_STREAM_CAPTURE) {
        MODIFY_REG(reg->RXCR, I2S_RXCR_VDW_MASK | I2S_RXCR_RCSR_MASK, val);
    } else {
        MODIFY_REG(reg->TXCR, I2S_TXCR_VDW_MASK | I2S_TXCR_TCSR_MASK, val);
    }

    MODIFY_REG(reg->DMACR, I2S_DMACR_TDL_MASK, I2S_DMACR_TDL(I2S_DMA_BURST_SIZE));
    MODIFY_REG(reg->DMACR, I2S_DMACR_RDL_MASK, I2S_DMACR_RDL(I2S_DMA_BURST_SIZE));

    return ret;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_I2S_MODULE_ENABLED */
