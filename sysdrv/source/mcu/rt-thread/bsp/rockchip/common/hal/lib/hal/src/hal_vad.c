/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_VAD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VAD
 *  @{
 */

/** @defgroup VAD_How_To_Use How To Use
 *  @{

 The VAD driver can be used as follows:

 @} */

/** @defgroup VAD_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/* CONTROL */
#define VAD_CONTROL_VAD_DET_CHNL(x)         ((x) << VAD_CONTROL_VAD_DET_CHANNEL_SHIFT)
#define VAD_CONTROL_VOICE_H16B              (0x0U << VAD_CONTROL_VOICE_24BIT_SAT_SHIFT)
#define VAD_CONTROL_VOICE_SAT_24TO16        (0x1U << VAD_CONTROL_VOICE_24BIT_SAT_SHIFT)
#define VAD_CONTROL_VOICE_24BIT_ALIGN_8_31B (0x0U << VAD_CONTROL_VOICE_24BIT_ALIGN_MODE_SHIFT)
#define VAD_CONTROL_VOICE_24BIT_ALIGN_0_23B (0x1U << VAD_CONTROL_VOICE_24BIT_ALIGN_MODE_SHIFT)
#define VAD_CONTROL_VOICE_CHNL_16B          (0x0U << VAD_CONTROL_VOICE_CHANNEL_BITWIDTH_SHIFT)
#define VAD_CONTROL_VOICE_CHNL_24B          (0x1U << VAD_CONTROL_VOICE_CHANNEL_BITWIDTH_SHIFT)
#define VAD_CONTROL_VOICE_CHNL_NUM(x)       ((x - 1) << VAD_CONTROL_VOICE_CHANNEL_NUM_SHIFT)
#define VAD_CONTROL_VAD_MODE(x)             ((x) << VAD_CONTROL_VAD_MODE_SHIFT)
#define VAD_CONTROL_SRC_ADDR_MODE_INC       (0x0U << VAD_CONTROL_SOURCE_FIXADDR_EN_SHIFT)
#define VAD_CONTROL_SRC_ADDR_MODE_FIXED     (0x1U << VAD_CONTROL_SOURCE_FIXADDR_EN_SHIFT)
#define VAD_CONTROL_INCR_BURST_LEN(x)       ((x - 1) << VAD_CONTROL_INCR_LENGTH_SHIFT)
#define VAD_CONTROL_SRC_BURST_NUM(x)        ((x - 1) << VAD_CONTROL_SOURCE_BURST_NUM_SHIFT)
#define VAD_CONTROL_SRC_BURST_SIGNLE        (0x0U << VAD_CONTROL_SOURCE_BURST_SHIFT)
#define VAD_CONTROL_SRC_BURST_INCR          (0x1U << VAD_CONTROL_SOURCE_BURST_SHIFT)
#define VAD_CONTROL_SRC_BURST_INCR4         (0x3U << VAD_CONTROL_SOURCE_BURST_SHIFT)
#define VAD_CONTROL_SRC_BURST_INCR8         (0x5U << VAD_CONTROL_SOURCE_BURST_SHIFT)
#define VAD_CONTROL_SRC_BURST_INCR16        (0x7U << VAD_CONTROL_SOURCE_BURST_SHIFT)
#define VAD_CONTROL_SRC_SEL(x)              ((x) << VAD_CONTROL_SOURCE_SELECT_SHIFT)
#define VAD_CONTROL_VAD_EN                  (0x1U << VAD_CONTROL_VAD_EN_SHIFT)
#define VAD_CONTROL_VAD_DIS                 (0x0U << VAD_CONTROL_VAD_EN_SHIFT)
/* INT */
#define VAD_INT_VAD_DATA_TRANS_INT_EN (0x1U << VAD_INT_VAD_DATA_TRANS_INT_EN_SHIFT)
#define VAD_INT_ERROR_INT_EN          (0x1U << VAD_INT_ERROR_INT_EN_SHIFT)
#define VAD_INT_VAD_DET_INT_EN        (0x1U << VAD_INT_VAD_DET_INT_EN_SHIFT)
/* AUX_CON0 */
#ifdef VAD_AUX_CON0_OFFSET
#define VAD_AUX_CON0_BUS_WRITE_EN            (0x1U << VAD_AUX_CON0_BUS_WRITE_EN_SHIFT)
#define VAD_AUX_CON0_BUS_WRITE_DIS           (0x0U << VAD_AUX_CON0_BUS_WRITE_EN_SHIFT)
#define VAD_AUX_CON0_RAM_ITF_EN              (0x0U << VAD_AUX_CON0_DIS_RAM_ITF_SHIFT)
#define VAD_AUX_CON0_RAM_ITF_DIS             (0x1U << VAD_AUX_CON0_DIS_RAM_ITF_SHIFT)
#define VAD_AUX_CON0_DATA_TRANS_TRIG_INT_EN  (0x1U << VAD_AUX_CON0_DATA_TRANS_TRIG_INT_EN_SHIFT)
#define VAD_AUX_CON0_DATA_TRANS_TRIG_INT_DIS (0x0U << VAD_AUX_CON0_DATA_TRANS_TRIG_INT_EN_SHIFT)
#endif
/* AUX_CON1 */
#ifdef VAD_AUX_CON1_OFFSET
#define VAD_AUX_CON1_DATA_TRANS_WORD_THD(x)    ((x - 1) << VAD_AUX_CON1_DATA_TRANS_WORD_THD_SHIFT)
#define VAD_AUX_CON1_DATA_TRANS_INT_MODE_KBYTE (0x0U << VAD_AUX_CON1_DATA_TRANS_INT_MODE_SEL_SHIFT)
#define VAD_AUX_CON1_DATA_TRANS_INT_MODE_WORD  (0x1U << VAD_AUX_CON1_DATA_TRANS_INT_MODE_SEL_SHIFT)
#endif

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

__WEAK const struct AUDIO_SRC_ADDR_MAP g_audioSrcAddrMaps[] = { 0 };

/********************* Private Function Definition ***************************/

static HAL_Status VAD_Get_Audio_Src_Info(struct HAL_VAD_DEV *vad, uint32_t addr)
{
    const struct AUDIO_SRC_ADDR_MAP *item;

    for (item = g_audioSrcAddrMaps; item->addr != 0; item++) {
        if ((item->addr >> 12) == (addr >> 12)) {
            vad->audioSrc = item->id;
            vad->audioSrcAddr = item->addr;

            return HAL_OK;
        }
    }

    return HAL_ERROR;
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup VAD_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  vad suspend.
 * @param  vad: vad vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Supsend(struct HAL_VAD_DEV *vad)
{
    return HAL_OK;
}

/**
 * @brief  vad resume.
 * @param  vad: vad vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Resume(struct HAL_VAD_DEV *vad)
{
    return HAL_OK;
}

/** @} */

/** @defgroup VAD_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup VAD_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup VAD_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Init vad controller.
 * @param  vad: the handle of struct HAL_VAD_DEV.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Init(struct HAL_VAD_DEV *vad)
{
    struct VAD_REG *reg = vad->pReg;
    uint32_t val, mask;

    HAL_CRU_ClkEnable(vad->hclk);

    vad->mode = VAD_STOREMODE_AFTER_EN;
    vad->audioDetChnl = 0;
    vad->vswitch = 0;

    WRITE_REG(reg->RAM_START_ADDR, vad->ramStartAddr);
    WRITE_REG(reg->RAM_END_ADDR, vad->ramEndAddr);
    vad->vbuf.begin = vad->ramStartAddr;
    WRITE_REG(reg->VS_ADDR, vad->audioSrcAddr);

    val = VAD_CONTROL_VAD_DET_CHNL(vad->audioDetChnl);
    val |= VAD_CONTROL_SRC_SEL(vad->audioSrc);
    val |= VAD_CONTROL_VAD_MODE(vad->mode);
    val |= VAD_CONTROL_SRC_ADDR_MODE_INC | VAD_CONTROL_SRC_BURST_INCR;

    mask = VAD_CONTROL_VAD_DET_CHANNEL_MASK |
           VAD_CONTROL_SOURCE_SELECT_MASK |
           VAD_CONTROL_VAD_MODE_MASK |
           VAD_CONTROL_SOURCE_FIXADDR_EN_MASK |
           VAD_CONTROL_SOURCE_BURST_MASK;

    MODIFY_REG(reg->CONTROL, mask, val);

    return HAL_OK;
}

/**
 * @brief  DeInit vad controller.
 * @param  vad: The handle of struct vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_DeInit(struct HAL_VAD_DEV *vad)
{
    HAL_CRU_ClkDisable(vad->hclk);

    return HAL_OK;
}

/** @} */

/** @defgroup VAD_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Start VAD.
 * @param  vad: The handle of struct vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Start(struct HAL_VAD_DEV *vad)
{
    struct VAD_REG *reg = vad->pReg;
    uint32_t val, mask;

    MODIFY_REG(reg->CONTROL, VAD_CONTROL_VAD_EN_MASK, VAD_CONTROL_VAD_EN);

    val = VAD_INT_ERROR_INT_EN | VAD_INT_VAD_DET_INT_EN;
    mask = VAD_INT_ERROR_INT_EN_MASK | VAD_INT_VAD_DET_INT_EN_MASK;

    MODIFY_REG(reg->INT, mask, val);

    return HAL_OK;
}

/**
 * @brief  Stop VAD.
 * @param  vad: The handle of struct vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Stop(struct HAL_VAD_DEV *vad)
{
    struct VAD_REG *reg = vad->pReg;
    struct VAD_BUF *vbuf = &vad->vbuf;
    uint32_t val;

    val = READ_REG(reg->CONTROL);
    if ((val & VAD_CONTROL_VAD_EN_MASK) == VAD_CONTROL_VAD_DIS) {
        return HAL_OK;
    }
    MODIFY_REG(reg->CONTROL, VAD_CONTROL_VAD_EN_MASK, VAD_CONTROL_VAD_DIS);
    val = READ_REG(reg->CONTROL);
    vad->h_16Bit = (val & VAD_CONTROL_VOICE_24BIT_SAT_MASK) == VAD_CONTROL_VOICE_H16B;
    val = READ_REG(reg->RAM_END_ADDR);
    vbuf->end = val + 0x8; /* ram_end_addr means the base of last entry */
    val = READ_REG(reg->INT);
    val &= HAL_BIT(8);
    vbuf->loop = val;
    val = READ_REG(reg->RAM_CUR_ADDR);
    if (!val) {
        vbuf->size = 0;
        vbuf->cur = vbuf->begin;

        return HAL_OK;
    }
    vbuf->cur = val;

    if (vbuf->loop) {
        vbuf->size = vbuf->end - vbuf->begin;
        vbuf->pos = vbuf->cur;
    } else {
        vbuf->size = vbuf->cur - vbuf->begin;
        vbuf->end = vbuf->cur;
        vbuf->pos = vbuf->begin;
    }

    return HAL_OK;
}

/**
 * @brief  Pause VAD.
 * @param  vad: The handle of struct vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Pause(struct HAL_VAD_DEV *vad)
{
    struct VAD_REG *reg = vad->pReg;
    uint32_t val;

    val = READ_REG(reg->CONTROL);
    if ((val & VAD_CONTROL_VAD_EN_MASK) == VAD_CONTROL_VAD_DIS) {
        return HAL_OK;
    }
    MODIFY_REG(reg->CONTROL, VAD_CONTROL_VAD_EN_MASK, VAD_CONTROL_VAD_DIS);

    return HAL_OK;
}

/**
 * @brief  Config params for VAD.
 * @param  vad: The handle of struct vad.
 * @param  params: The audio params.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_Config(struct HAL_VAD_DEV *vad, struct AUDIO_PARAMS *params)
{
    struct VAD_REG *reg = vad->pReg;
    uint32_t val, mask, channels;

    switch (params->sampleBits) {
    case AUDIO_SAMPLEBITS_16:
        val = VAD_CONTROL_VOICE_CHNL_16B;
        vad->sampleBytes = 2;
        break;
    case AUDIO_SAMPLEBITS_32:
        val = VAD_CONTROL_VOICE_CHNL_24B;
        vad->sampleBytes = 4;
        break;
    default:

        return HAL_INVAL;
    }

    MODIFY_REG(reg->CONTROL, VAD_CONTROL_VOICE_CHANNEL_BITWIDTH_MASK, val);

    channels = params->channels;
    mask = VAD_CONTROL_VOICE_CHANNEL_NUM_MASK |
           VAD_CONTROL_INCR_LENGTH_MASK;
    val = VAD_CONTROL_VOICE_CHNL_NUM(channels) |
          VAD_CONTROL_INCR_BURST_LEN(channels);
    vad->channels = channels;
    MODIFY_REG(reg->CONTROL, mask, val);

    return HAL_OK;
}

/**
 * @brief  Set the audio stream source for VAD.
 * @param  vad: The handle of struct vad.
 * @param  source: The audio controllers' base addr.
 *         depends on bit SOURCE_FIXADDR_EN in register 'VAD_CONTROL'.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_SetAudioSource(struct HAL_VAD_DEV *vad, uint32_t source)
{
    struct VAD_REG *reg = vad->pReg;
    HAL_Status ret;

    ret = VAD_Get_Audio_Src_Info(vad, source);
    HAL_ASSERT(ret == HAL_OK);

    WRITE_REG(reg->VS_ADDR, vad->audioSrcAddr);
    MODIFY_REG(reg->CONTROL, VAD_CONTROL_SOURCE_SELECT_MASK,
               VAD_CONTROL_SRC_SEL(vad->audioSrc));

    return ret;
}

/**
 * @brief  Set VAD work mode.
 * @param  vad: The handle of struct vad.
 * @param  mode: vad store voice data mode.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_SetMode(struct HAL_VAD_DEV *vad, eVAD_storeMode mode)
{
    struct VAD_REG *reg = vad->pReg;

    vad->mode = mode;
    MODIFY_REG(reg->CONTROL, VAD_CONTROL_VAD_MODE_MASK,
               VAD_CONTROL_VAD_MODE(vad->mode));

    return HAL_OK;
}

/**
 * @brief  Set the ring buffer time for VAD.
 * @param  vad: The handle of struct vad.
 * @param  bufferTime: buffer time in msec.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_SetBufferTime(struct HAL_VAD_DEV *vad, uint32_t bufferTime)
{
    vad->bufferTime = bufferTime;

    return HAL_OK;
}

/**
 * @brief  Set detect channel for VAD.
 * @param  vad: The handle of struct vad.
 * @param  detectChannel: The channel index for detection.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_SetDetectChannel(struct HAL_VAD_DEV *vad, uint32_t detectChannel)
{
    struct VAD_REG *reg = vad->pReg;

    vad->audioDetChnl = detectChannel;

    MODIFY_REG(reg->CONTROL, VAD_CONTROL_VAD_DET_CHANNEL_MASK,
               VAD_CONTROL_VAD_DET_CHNL(vad->audioDetChnl));

    return HAL_OK;
}

/**
 * @brief  config ring buffer start and end address for VAD.
 * @param  vad: The handle of struct vad.
 * @param  start: The begin address.
 * @param  end: The end address.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_ConfigRingBuffer(struct HAL_VAD_DEV *vad, uint32_t start, uint32_t end)
{
    struct VAD_REG *reg = vad->pReg;

    vad->vbuf.begin = start;
    vad->vbuf.end = end;
    vad->vbuf.size = end - start;

    vad->ramStartAddr = start;
    /* ramEndAddr means the last entry address to store data,
     * so need minus 0x8.
     */
    vad->ramEndAddr = end - 0x8;

    WRITE_REG(reg->RAM_START_ADDR, vad->ramStartAddr);
    WRITE_REG(reg->RAM_END_ADDR, vad->ramEndAddr);

    return HAL_OK;
}

/**
 * @brief  The IrqHandler of VAD.
 * @param  vad: The handle of struct HAL_VAD_DEV.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_IrqHandler(struct HAL_VAD_DEV *vad)
{
    struct VAD_REG *reg = vad->pReg;
    uint32_t val;

    val = READ_REG(reg->INT);
    WRITE_REG(reg->INT, val);

    //HAL_DBG("%s: irq status: 0x%lx\n", __func__, val);

    return HAL_OK;
}

/**
 * @brief  Get vad current address.
 * @param  vad: The handle of struct HAL_VAD_DEV.
 * @param  addr: The pointer to save current address.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_GetCurAddr(struct HAL_VAD_DEV *vad, uint32_t *addr)
{
    struct VAD_REG *reg = vad->pReg;

    *addr = READ_REG(reg->RAM_CUR_ADDR);

    return HAL_OK;
}

/**
 * @brief  Get vad interupt register status.
 * @param  vad: The handle of struct HAL_VAD_DEV.
 * @param  status: The pointer to save interrupt register status.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_GetIrqStatus(struct HAL_VAD_DEV *vad, uint32_t *status)
{
    struct VAD_REG *reg = vad->pReg;

    *status = READ_REG(reg->INT);

    return HAL_OK;
}

/**
 * @brief  Enable vad interrupt.
 * @param  vad: The handle of struct HAL_VAD_DEV.
 * @param  type: Interrupt type.
 * @param  enable: Enable flag.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_EnableInt(struct HAL_VAD_DEV *vad, eVAD_intType type, uint32_t enable)
{
    struct VAD_REG *reg = vad->pReg;
    uint32_t mask, val;

    switch (type) {
    case VAD_INTTYPE_DET:
        mask = VAD_INT_VAD_DET_INT_EN_MASK;
        val = (enable & 0x1) << VAD_INT_VAD_DET_INT_EN_SHIFT;
        break;
    case VAD_INTTYPE_TRANS:
        mask = VAD_INT_VAD_DATA_TRANS_INT_EN_MASK;
        val = (enable & 0x1) << VAD_INT_VAD_DATA_TRANS_INT_EN_SHIFT;
        break;
    default:

        return HAL_INVAL;
    }

    MODIFY_REG(reg->INT, mask, val);

    return HAL_OK;
}

/**
 * @brief  enable periods data irq.
 * @param  vad: The handle of struct vad.
 * @param  words: period size in units of words(32 bits).
 * @return HAL_Status
 */
HAL_Status HAL_VAD_SetPeriodSize(struct HAL_VAD_DEV *vad, uint32_t words)
{
#if defined(VAD_AUX_CON0_OFFSET) && defined(VAD_AUX_CON1_OFFSET)
    struct VAD_REG *reg = vad->pReg;

    MODIFY_REG(reg->AUX_CON1,
               VAD_AUX_CON1_DATA_TRANS_INT_MODE_SEL_MASK |
               VAD_AUX_CON1_DATA_TRANS_WORD_THD_MASK,
               VAD_AUX_CON1_DATA_TRANS_INT_MODE_WORD |
               VAD_AUX_CON1_DATA_TRANS_WORD_THD(words));
    MODIFY_REG(reg->AUX_CON0,
               VAD_AUX_CON0_DATA_TRANS_TRIG_INT_EN_MASK,
               VAD_AUX_CON0_DATA_TRANS_TRIG_INT_EN);
#endif

    return HAL_OK;
}

/**
 * @brief  Enable store data through ahb bus interface.
 * @param  vad: The handle of struct vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_EnableBusMode(struct HAL_VAD_DEV *vad)
{
#if defined(VAD_AUX_CON0_OFFSET)
    struct VAD_REG *reg = vad->pReg;

    MODIFY_REG(reg->AUX_CON0,
               VAD_AUX_CON0_DIS_RAM_ITF_MASK |
               VAD_AUX_CON0_BUS_WRITE_EN_MASK,
               VAD_AUX_CON0_RAM_ITF_DIS |
               VAD_AUX_CON0_BUS_WRITE_EN);
#endif

    return HAL_OK;
}

/**
 * @brief  Disable store data through ahb bus interface.
 * @param  vad: The handle of struct vad.
 * @return HAL_Status
 */
HAL_Status HAL_VAD_DisableBusMode(struct HAL_VAD_DEV *vad)
{
#if defined(VAD_AUX_CON0_OFFSET)
    struct VAD_REG *reg = vad->pReg;

    MODIFY_REG(reg->AUX_CON0,
               VAD_AUX_CON0_DIS_RAM_ITF_MASK |
               VAD_AUX_CON0_BUS_WRITE_EN_MASK,
               VAD_AUX_CON0_RAM_ITF_EN |
               VAD_AUX_CON0_BUS_WRITE_DIS);
#endif

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_VAD_MODULE_ENABLED */
