/******************************************************************************
**
**  File Name:   $RCSfile: sbc_dec_bit_alloc_ste.c,v $
**
**  Description: This file contains the code for bit allocation algorithm.
**       It calculates the number of bits required for the encoded stream of data.
**
**  Revision :   $Id: sbc_dec_bit_alloc_ste.c,v 1.8 2004/05/06 14:11:50 slefort Exp $
**
**  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

/*Includes*/
#include "sbc_decoder.h"
#include "sbc_dec_func_declare.h"

/*global arrays*/
extern const SINT16 as16Offset4[4][4];
extern const SINT16 as16Offset8[4][8];
/****************************************************************************
* BitAlloc - Calculates the required number of bits for the given scale factor
* and the number of subbands.
*
* RETURNS : N/A
*/

#if (SBC_DEC_STEREO_INCLUDED == TRUE)
void sbc_dec_bit_alloc_ste(SBC_DEC_PARAMS *CodecParams)
{
    SINT16 maxBitNeed = 0;
    SINT16 bitCount;
    SINT16 sliceCount;
    SINT16 bitSlice;
    SINT16 sb, ch;
    SINT16 *bitNeed, *pbitNeed, *pScaleFactor, *pBit;
    SINT16 loudness;
    SINT32 s32NumOfSubBands = (SINT32)CodecParams->numOfSubBands;

    /*bitneed values are derived from scale factor*/
    maxBitNeed = 0;
    /*pScaleFactor=CodecParams->scaleFactor;*/
    if (CodecParams->allocationMethod == SBC_SNR)
    {
        bitNeed = CodecParams->scaleFactor;
        pbitNeed = bitNeed;
        for (ch = 0; ch < 2 * s32NumOfSubBands; ch++)
        {
            if (*pbitNeed > maxBitNeed)
                maxBitNeed = *pbitNeed;
            pbitNeed++;
        }
    }
    else
    {
        bitNeed = CodecParams->scartchMemForBitAlloc;
        pbitNeed = bitNeed;
        pScaleFactor = CodecParams->scaleFactor;
        for (ch = 0; ch < 2; ch++)
        {
            for (sb = 0; sb < s32NumOfSubBands; sb++)
            {
                if (*pScaleFactor == 0)
                    *pbitNeed = -5;
                else
                {
                    if (s32NumOfSubBands == 4)
                        loudness = *pScaleFactor - as16Offset4[CodecParams->samplingFreq][sb];
                    else
                        loudness = *pScaleFactor - as16Offset8[CodecParams->samplingFreq][sb];
                    if (loudness > 0)
                        *pbitNeed = loudness >> 1;
                    else
                        *pbitNeed = loudness;
                    if (*pbitNeed > maxBitNeed)
                        maxBitNeed = *pbitNeed;
                }
                pScaleFactor++;
                pbitNeed++;
            }
        }
    }

    /*iterative process to find out hwo many bitslices fit into the bitpool*/
    bitSlice = maxBitNeed + 1;
    bitCount = CodecParams->bitPool;
    sliceCount = 0;
    do
    {
        bitSlice --;
        bitCount -= sliceCount;
        pbitNeed = bitNeed;
        sliceCount = 0;
        for (ch = 2 * s32NumOfSubBands - 1; ch >= 0; ch--)
        {
            if ((*pbitNeed < bitSlice + 16) && (*pbitNeed >= bitSlice + 1))
            {
                if (*pbitNeed == bitSlice + 1)
                    sliceCount += 2;
                else
                    sliceCount++;
            }
            pbitNeed++;
        }/*end of ch for*/
    }
    while (bitCount - sliceCount > 0);  /*end of do-while*/

    if (bitCount - sliceCount == 0)
    {
        bitCount -= sliceCount;
        bitSlice --;
    }

    /*Bits are distributed until the last bitslice is reached*/
    pbitNeed = bitNeed;
    pBit = CodecParams->bits;
    for (ch = 0; ch < 2 * s32NumOfSubBands; ch++)
    {
        if (*pbitNeed < bitSlice + 2)
            *pBit = 0;
        else
            *pBit = ((*pbitNeed - bitSlice) < 16) ? (*pbitNeed - bitSlice) : 16;
        pbitNeed++;
        pBit++;
    }

    /*the remaining bits are allocated starting at subband 0*/
    sb = 0;
    pBit = CodecParams->bits;
    while ((bitCount > 0) && (sb < s32NumOfSubBands))
    {
        if ((*pBit >= 2) && (*pBit < 16))
        {
            (*pBit)++;
            bitCount--;
        }
        else if ((bitNeed[sb] == bitSlice + 1) && (bitCount > 1))
        {
            *pBit = 2;
            bitCount -= 2;
        }
        if (bitCount <= 0)
            break;
        if ((*(pBit + s32NumOfSubBands) >= 2) && (*(pBit + s32NumOfSubBands) < 16))
        {
            (*(pBit + s32NumOfSubBands))++;
            bitCount--;
        }
        else if ((bitNeed[s32NumOfSubBands + sb] == bitSlice + 1) && (bitCount > 1))
        {
            *(pBit + s32NumOfSubBands) = 2;
            bitCount -= 2;
        }
        pBit++;
        sb++;
    }

    sb = 0;
    pBit = CodecParams->bits;
    while ((bitCount > 0) && (sb < s32NumOfSubBands))
    {
        if (*pBit < 16)
        {
            (*pBit)++;
            bitCount--;
            if (bitCount <= 0)
                break;
        }
        if (*(pBit + s32NumOfSubBands) < 16)
        {
            (*(pBit + s32NumOfSubBands))++;
            bitCount--;
        }
        sb++;
    }
}
#endif
/*End of BitAlloc() function*/
