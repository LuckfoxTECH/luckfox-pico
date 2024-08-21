/******************************************************************************
**
**  File Name:   $RCSfile: sbc_dec_bit_alloc_mono.c,v $
**
**  Description: This file contains the code for bit allocation algorithm.
**               It calculates the number of bits required for the encoded stream of data.
**
**  Revision :   $Id: sbc_dec_bit_alloc_mono.c,v 1.8 2004/05/06 14:11:50 slefort Exp $
**
**  Copyright (c) 2002-2008, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/


/*Includes*/
#include "sbc_decoder.h"
#include "sbc_dec_func_declare.h"

/*global arrays*/
const SINT16 as16Offset4[4][4] = {  {-1, 0, 0, 0}, {-2, 0, 0, 1},
    {-2, 0, 0, 1}, {-2, 0, 0, 1}
};
const SINT16 as16Offset8[4][8] = {  {-2, 0, 0, 0, 0, 0, 0, 1},
    {-3, 0, 0, 0, 0, 0, 1, 2},
    {-4, 0, 0, 0, 0, 0, 1, 2},
    {-4, 0, 0, 0, 0, 0, 1, 2}
};

/****************************************************************************
* BitAlloc - Calculates the required number of bits for the given scale factor
* and the number of subbands.
*
* RETURNS : N/A
*/

void sbc_dec_bit_alloc_mono(SBC_DEC_PARAMS *CodecParams)
{
    SINT32 s16MaxBitNeed;   /*to store the max bits needed per sb*/
    SINT32 s16BitCount;     /*the used number of bits*/
    SINT32 s16SliceCount;   /*to store hwo many slices can be put in bitpool*/
    SINT32 s16BitSlice;     /*number of bitslices in bitpool*/
    SINT32 s16Sb;           /*counter for sub-band*/
    SINT32 s16Ch, ChOffset;         /*counter for channel*/
    SINT16 *ps16BitNeed, *pbits; /*temp memory to store required number of bits*/
    SINT32 s16Loudness;     /*used in Loudness calculation*/
    SINT16 *ps16GenBufPtr;
    SINT16 *ps16GenArrPtr;
    SINT16 *ps16GenTabPtr;
    SINT32 s32NumOfSubBands = (SINT32)CodecParams->numOfSubBands;

    if (CodecParams->allocationMethod == SBC_SNR)
        ps16BitNeed = CodecParams->scaleFactor;
    else
        ps16BitNeed = CodecParams->scartchMemForBitAlloc;
    for (s16Ch = 0; s16Ch < CodecParams->numOfChannels; s16Ch++)
    {
        ChOffset = s16Ch * s32NumOfSubBands;
        ps16GenBufPtr = ps16BitNeed + ChOffset;
        /*bitneed values are derived from scale factor*/
        if (CodecParams->allocationMethod == SBC_SNR)
        {
            s16MaxBitNeed = 0;
            for (s16Sb = 0; s16Sb < s32NumOfSubBands; s16Sb++)
            {
                if (*(ps16GenBufPtr) > s16MaxBitNeed)
                    s16MaxBitNeed = *(ps16GenBufPtr);
                ps16GenBufPtr++;
            }
        }
        else
        {
            if (s32NumOfSubBands == 4)
            {
                ps16GenTabPtr = (SINT16 *)as16Offset4[CodecParams->samplingFreq];
            }
            else
            {
                ps16GenTabPtr = (SINT16 *) as16Offset8[CodecParams->samplingFreq];
            }

            s16MaxBitNeed = 0;
            for (s16Sb = 0; s16Sb < s32NumOfSubBands; s16Sb++)
            {
                if (CodecParams->scaleFactor[ChOffset + s16Sb] == 0)
                    *(ps16GenBufPtr) = -5;
                else
                {
                    s16Loudness = CodecParams->scaleFactor[ChOffset + s16Sb] - *ps16GenTabPtr;

                    if (s16Loudness > 0)
                        *(ps16GenBufPtr) = (SINT16)(s16Loudness >> 1);
                    else
                        *(ps16GenBufPtr) = (SINT16)s16Loudness;

                    if (*(ps16GenBufPtr) > s16MaxBitNeed)
                        s16MaxBitNeed = *(ps16GenBufPtr);
                }
                ps16GenBufPtr++;
                ps16GenTabPtr++;
            }

        }
        ps16GenBufPtr -= s32NumOfSubBands;
        /*iterative process to find hwo many bitslices fit into the bitpool*/
        s16BitSlice = s16MaxBitNeed + 1;
        s16BitCount = CodecParams->bitPool;
        s16SliceCount = 0;
        do
        {
            s16BitSlice --;
            s16BitCount -= s16SliceCount;
            s16SliceCount = 0;

            for (s16Sb = s32NumOfSubBands - 1; s16Sb >= 0 ; s16Sb--)
            {
                if (((*ps16GenBufPtr - s16BitSlice) < 16) && ((*ps16GenBufPtr - s16BitSlice) >= 1))
                {
                    if ((*ps16GenBufPtr - s16BitSlice) == 1)
                        s16SliceCount += 2;
                    else
                        s16SliceCount++;
                }
                ps16GenBufPtr++;

            }/*end of for*/
            ps16GenBufPtr -= s32NumOfSubBands;
        }
        while (s16BitCount - s16SliceCount > 0);

        /*Bits are distributed until the last bitslice is reached*/
        pbits = CodecParams->bits;
        ps16GenArrPtr = pbits + ChOffset;
        for (s16Sb = s32NumOfSubBands - 1; s16Sb >= 0; s16Sb--)
        {
            if (*ps16GenBufPtr - s16BitSlice < 2)
                *(ps16GenArrPtr) = 0;
            else
                *(ps16GenArrPtr) = ((*ps16GenBufPtr - s16BitSlice) < 16) ? (*ps16GenBufPtr - s16BitSlice) : 16;

            ps16GenBufPtr++;
            ps16GenArrPtr++;
        }
        ps16GenArrPtr -= s32NumOfSubBands;
        ps16GenBufPtr -= s32NumOfSubBands;
        /*the remaining bits are allocated starting at subband 0*/
        s16Sb = s32NumOfSubBands - 1;
        while ((s16BitCount > 0) && (s16Sb >= 0))
        {
            if ((*(ps16GenArrPtr) >= 2) && (*(ps16GenArrPtr) < 16))
            {
                (*(ps16GenArrPtr))++;
                s16BitCount--;
            }
            else if ((*(ps16GenBufPtr) == s16BitSlice + 1) && (s16BitCount > 1))
            {
                *(ps16GenArrPtr) = 2;
                s16BitCount -= 2;
            }
            s16Sb--;
            ps16GenArrPtr++;
            ps16GenBufPtr++;
        }
        ps16GenArrPtr = pbits + ChOffset;

        s16Sb = s32NumOfSubBands - 1;
        while ((s16BitCount > 0) && (s16Sb >= 0))
        {
            if (*(ps16GenArrPtr) < 16)
            {
                (*(ps16GenArrPtr))++;
                s16BitCount--;
            }
            s16Sb--;
            ps16GenArrPtr++;
        }
    }
}
/*End of BitAlloc() function*/

