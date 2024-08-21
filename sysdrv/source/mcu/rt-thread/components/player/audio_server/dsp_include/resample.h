/**
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 * @file    resample.h
 * @author  Cherry Chen / XiaoTan Luo
 * @version V0.1
 * @date    16-12-2019
 *
 * based on
 *
 *        SRC.H - Contains function prototypes for the routines in the sample
 *        rate converter library.
 *
 *        Copyright (c) 1999 Cirrus Logic, Inc.
 *
 ******************************************************************************
 */

#ifndef __RESAMPLE_H__
#define __RESAMPLE_H__

/****************************************************************************
 *
 * The number of taps in the sample rate conversion filters.  This is an
 * indication of the number of MACs in the unrolled filter loop.  There is a
 * direct correlation between the number of taps and the execution time of
 * the sample rate converter.  There is one MAC per tap per output sample.
 * This is also the amount of extra memory required before any input data
 * buffer passed to the sample rate converter.
 *
 ****************************************************************************
 */

#define NUMTAPS (64)

/****************************************************************************
 *
 * The perisistent state of the sample rate converter.
 *
 ****************************************************************************
 */

typedef short ssrc_type;

typedef struct
{
    ssrc_type lastSample;
    ssrc_type Left[NUMTAPS];
} SRCbuf;

typedef struct
{
    /* The internal, persistent state of the sample rate converter. */
    unsigned char ucInternal[12];
    short *psFilter;
    int lFilterIncr;
    int lFilterSize;
    int lFilterOffset;
    ssrc_type lastSample1;
    ssrc_type lastSample2;
    ssrc_type lastSample3;
    ssrc_type lastSample4;
    ssrc_type Left[NUMTAPS * 4];
} SRCState;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

rt_err_t resample_48to16_4ch_init(SRCState *pSRC, unsigned long ulInputRate,
        unsigned long ulOutputRate);
void resample_48to16_4ch_process(SRCState *pSRC, ssrc_type *psInData, ssrc_type *psOutData,
                                        long lNumInputSamples, long *plNumOutputSamples);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RESAMPLE_H__ */
