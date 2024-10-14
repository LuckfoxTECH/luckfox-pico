/**
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 * @file    resample_48to16_4ch.c
 * @author  Cherry Chen / Xing Zheng / XiaoTan Luo
 * @version V0.1
 * @date    16-12-2019
 * @brief   Resample 4 channels pcm data from 48k to 16k.
 ******************************************************************************
 */

#include <rtdevice.h>
#include <rtthread.h>

#include "rk_audio.h"
#include "drv_heap.h"
#include "resample.h"

static const struct
{
    short sNumPolyPhases;
    short sSampleIncrement;
    short sNumTaps;
    short sCoefs[1][64];
} SRCFilter_0_3333 =
{
    1,
    3,
    64,
    {
        1 , 12 , 43 , 93 , 139 , 140 , 67 , -54 , -140 , -108 , 40 ,
        185 , 177 , -21 , -255 , -285 , -19 , 345 , 441 , 87 , -469 ,
        -681 , -211 , 650 , 1085 , 449 , -989 , -1969 , -1083 , 2038 ,
        6156 , 9067 , 9067 , 6156 , 2038 , -1083 , -1969 , -989 , 449 ,
        1085 , 650 , -211 , -681 , -469 , 87 , 441 , 345 , -19 , -285 ,
        -255 , -21 , 177 , 185 , 40 , -108 , -140 , -54 , 67 , 140 ,
        139 , 93 , 43 , 12 , 1
    },
};

/**
 * resample init.
 * @param unsigned long ulInputRate  e.g: 48000.
 * @param unsigned long ulOutputRate e.g: 16000.
 * @return rt_err_t if init success, return RT_EOK.
 */
rt_err_t resample_48to16_4ch_init(SRCState *pSRC, unsigned long ulInputRate, unsigned long ulOutputRate)
{
    long lNumPolyPhases, lSampleIncrement, lNumTaps;

    rt_memset(pSRC->Left, 0, NUMTAPS * 2 * 4);
    pSRC->lastSample1 = 0;
    pSRC->lastSample2 = 0;
    pSRC->lastSample3 = 0;
    pSRC->lastSample4 = 0;
    switch ((ulOutputRate << 16) / ulInputRate)
    {
        case 0x00005555:
        {
            lNumPolyPhases = SRCFilter_0_3333.sNumPolyPhases;
            lNumTaps = SRCFilter_0_3333.sNumTaps;
            lSampleIncrement = SRCFilter_0_3333.sSampleIncrement;
            pSRC->psFilter = (short *)SRCFilter_0_3333.sCoefs;
            break;
        }
        default:
        {
            return -RT_ERROR;
        }
    }
    /**
     * Make sure that the number of taps in the filter matches the number of
     * taps supported by our filtering code.
     */
    if (lNumTaps != NUMTAPS)
    {
        return -RT_ERROR;
    }
    /* Initialize the persistent SRC state. */
    pSRC->lFilterOffset = 0;
    pSRC->lFilterIncr = lSampleIncrement * NUMTAPS;
    pSRC->lFilterSize = lNumPolyPhases * NUMTAPS;

    return RT_EOK;
}

/**
 * process resample.
 * @param ssrc_type *psInData1, input 4ch 16bit pcm data.
 * @param ssrc_type *psOutData1, output 4ch 16bit pcm data
 * and mute the 4th ch.
 * @param long lNumInputSamples, each channel's samples.
 * @param long plNumOutputSamples, each channel's samples.
 */
void resample_48to16_4ch_process(SRCState *pSRC, ssrc_type *psInData1, ssrc_type *psOutData1,
                                 long lNumInputSamples, long *plNumOutputSamples)
{
    long long lOutData1, lOutData2, lOutData3;
    short *psPtr1;
    ssrc_type  *psPtr2, *psPtr3, *psPtr4;
    ssrc_type *psSample1, *psSample2, *psSample3, *psSample4;
    ssrc_type *psInData2 = psInData1 + 1;
    ssrc_type *psOutData2 = psOutData1 + 1;
    ssrc_type *psInData3 = psInData1 + 2;
    ssrc_type *psOutData3 = psOutData1 + 2;
    ssrc_type *psOutData4 = psOutData1 + 3;
    short sCoeff;
    int iLoop;
    int i = 0;
    static int count = 0;
    int tsize = sizeof(ssrc_type);

    /* Append the new data to the filter memory buffers. */
    memcpy(psInData1 - (NUMTAPS * 4), pSRC->Left, (NUMTAPS * 4 * tsize));

    /* Point to the last sample of the pre-existing audio data. */
    while (1)
    {
        psSample1 = psInData1 - 4 * i;
        psSample2 = psInData2 - 4 * i;
        psSample3 = psInData3 - 4 * i;
        i++;
        if (((*psSample1 == pSRC->lastSample1) &&
                (*psSample2 == pSRC->lastSample2) &&
                (*psSample3 == pSRC->lastSample3)) ||
                (i >= NUMTAPS))
        {
            break;
        }
    }

    /* Compute the number of output samples which we will produce. */
    iLoop = ((((lNumInputSamples + i - 1) * pSRC->lFilterSize) -
              pSRC->lFilterOffset - 1) / pSRC->lFilterIncr);

    *plNumOutputSamples = iLoop;

    /* Loop through each output sample. */
    while (iLoop--)
    {
        /* Increment the offset into the filter. */
        pSRC->lFilterOffset += pSRC->lFilterIncr;

        /**
         * See if we've passed the entire filter, indicating that we've
         * consumed one of the input samples.
         */
        while (pSRC->lFilterOffset >= pSRC->lFilterSize)
        {
            /**
             * We have, so wrap the filter offset (i.e. treat the filter as if
             * it were a circular buffer in memory).
             */
            pSRC->lFilterOffset -= pSRC->lFilterSize;
            /* Consume the input sample. */
            {
                psSample1 += 4;
                psSample2 += 4;
                psSample3 += 4;
            }
        }
        /* Get pointers to the filter and the two sample data streams. */
        psPtr1 = pSRC->psFilter + pSRC->lFilterOffset;
        count++;

        psPtr2 = psSample1;
        psPtr3 = psSample2;
        psPtr4 = psSample3;

        /**
         * Run the filter over the two sample streams.  The number of MACs here
         * must agree with NUMTAPS.
         */
        lOutData1 = 0;
        lOutData2 = 0;
        lOutData3 = 0;
        sCoeff = *psPtr1++;
        i = 0;
        while (i < NUMTAPS)
        {
            lOutData1 += (long long)sCoeff * (long long)(*psPtr2);
            lOutData2 += (long long)sCoeff * (long long)(*psPtr3);
            lOutData3 += (long long)sCoeff * (long long)(*psPtr4);
            if (i != (NUMTAPS - 1))
            {
                sCoeff = *psPtr1++;
                psPtr2 -= 4;
                psPtr3 -= 4;
                psPtr4 -= 4;
            }
            i++;
        }

        /* Write out the samples. */
        *psOutData1 = (ssrc_type)(lOutData1 >> 15);
        psOutData1 += 4;

        *psOutData2 = (ssrc_type)(lOutData2 >> 15);
        psOutData2 += 4;

        *psOutData3 = (ssrc_type)(lOutData3 >> 15);
        psOutData3 += 4;

        *psOutData4 = 0;
    }
    /**
     * Copy the tail of the filter memory buffer to the beginning, therefore
     * "remembering" the last few samples to use when processing the next batch
     * of samples.
     */
    pSRC->lastSample1 = *psSample1;
    pSRC->lastSample2 = *psSample2;
    pSRC->lastSample3 = *psSample3;
    pSRC->lastSample4 = *psSample4;

    memcpy(pSRC->Left, psInData1 + (lNumInputSamples * 4) - (NUMTAPS * 4), (NUMTAPS * 4 * tsize));
}
