/******************************************************************************
**
**  File Name:   $RCSfile: sbc_unpacking.c,v $
**
**  Description: source file for Unpacking data. This file contains the unpacking
**                  data from the incoming bit-stream.
**
**  Revision :   $Id: sbc_unpacking.c,v 1.23 2004/10/22 14:50:09 mjougit Exp $
**
**  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#include "sbc_decoder.h"
#include "sbc_dec_func_declare.h"
#include "sbc_basicop.h"
SINT16 DecUnPacking(SBC_DEC_PARAMS *strDecParams)
{
#if (SBC_ARM_OPT==FALSE && SBC_IPAQ_OPT==FALSE && SBC_C5402_OPT==FALSE)
    SINT32 s32HiTemp, s32In1Temp, s32In2Temp, s32Carry, s32Temp;
#endif
#if (SBC_ARM_OPT==TRUE)
    SINT16 *SbBuffer;
    SINT32 s32Low2;
    const SINT32 as32LevelLookUp[] = {0, 1073741824 >> 14, 357913941 >> 14, 153391689 >> 14, 71582788 >> 14, 34636833 >> 14, 17043521 >> 14, \
                                      8454660 >> 14, 4210752 >> 14, 2101256 >> 14, 1049601 >> 14, 524544 >> 14, 262208 >> 14, 131088 >> 14, 65540 >> 14, 32769 >> 14, 16384 >> 14
                                     };
#else
    SINT32 *SbBuffer;
#if (SBC_IS_64_BITS_IN_DEQUANTIZER==TRUE)
    const SINT32 as32LevelLookUp[17] = {0, 1073741824, 357913941, 153391689, 71582788, 34636833, 17043521, \
                                        8454660, 4210752, 2101256, 1049601, 524544, 262208, 131088, 65540, 32769, 16384
                                       };
#else
    const SINT32 as32LevelLookUp[] = {0, 1073741824 >> 14, 357913941 >> 14, 153391689 >> 14, 71582788 >> 14, 34636833 >> 14, 17043521 >> 14, \
                                      8454660 >> 14, 4210752 >> 14, 2101256 >> 14, 1049601 >> 14, 524544 >> 14, 262208 >> 14, 131088 >> 14, 65540 >> 14, 32769 >> 14, 16384 >> 14
                                     };
#endif
#endif
    SINT32 s32Level, s32Low1;
    /*SINT16 s16ChOffset;*/

    UINT8 *packetPtr;   /*the packet pointer*/
    register SINT32 s16Blk;     /*block counter*/
    SINT32 s16ChI;      /*channel counter*/ /* Reused with s16LoopIndexI */
    SINT32 s16SbJ;      /*sub-band counter*/ /* Reused with s16LoopIndexJ */
    UINT32 s16PresentBit;   /**/
    SINT32 s16LoopIndexK;   /*loop counter*/
    UINT32 u16QuantizedVal; /*to store quantized value*/
    UINT32 tmpu32;
    UINT32 u16LoopCount;    /*loop counter*/
    UINT8 u8XoredVal;       /*to store xored value*/
    UINT8 u8CRC;            /*CRC value*/
    SINT16 *ps16GenBufPtr, *pScaleFactor;

    SINT32 s32NumOfChannels;
    SINT32 s32NumOfSubBands;

    packetPtr = strDecParams->packet;   /*Initialize the ptr*/

    if (*packetPtr++ != (UINT8)0x9C) /*Sync word*/
    {
        strDecParams->u16PrevPacLen = 1;
        return SBC_SYNC_FAILURE;
    }

    packetPtr++;
    s32NumOfSubBands = strDecParams->numOfSubBands;
    s32NumOfChannels = strDecParams->numOfChannels;
    strDecParams->bitPool = *packetPtr++;
    u16LoopCount = (s32NumOfChannels * s32NumOfSubBands);

#if (SBC_DEC_SANITY_CHECK == TRUE)
    /* let's make sure we have enough data to calculate the CRC */
    /* calculate the packet len */
    if (strDecParams->channelMode == SBC_JOINT_STEREO)
        tmpu32 = 4 + ((4 * u16LoopCount) / 8) + 1;
    else /* SBC_STEREO, MONO and DUAL */
        tmpu32 = 4 + ((4 * u16LoopCount) / 8);

    /* make sure we have enough data in the buffer */
    if (strDecParams->u16ReaminingBytes < tmpu32)
    {
        /* do not drop current data */
        strDecParams->u16PrevPacLen = 0;
        return SBC_EMPTY_FAILURE;
    }
#endif

    /*------------- find CRC -----------------*/
    packetPtr = strDecParams->packet + 1; /*Initialize the ptr*/
    u8CRC = 0x0F;
    u16LoopCount = (u16LoopCount >> 1);
    /*
    The loops is run from the start of the packet till the scale factor
    parameters. In case of JS, 'join' parameter is included in the packet
    so, that many more bytes are included in u8CRC calculation.
    */

    for (s16SbJ = 1; s16SbJ < (SINT32)(u16LoopCount + 4); s16SbJ++)
    {
        /*skip sync word and CRC bytes*/
        if (s16SbJ != 3)
        {
            for (s16LoopIndexK = 7; s16LoopIndexK >= 0; s16LoopIndexK--)
            {
                u8XoredVal = (u8CRC >> 7) ^ ((*packetPtr >> s16LoopIndexK) & 0x01);
                u8CRC <<= 1;
                if (u8XoredVal != 0)
                    u8CRC ^= 0x1D;
                u8CRC &= 0xFF;
            }

        }
        packetPtr++;
    }
#if (SBC_DEC_STEREO_INCLUDED == TRUE)
    if (strDecParams->channelMode == SBC_JOINT_STEREO)
    {
        for (s16LoopIndexK = 7; s16LoopIndexK >= (8 - s32NumOfSubBands); s16LoopIndexK--)
        {
            u8XoredVal = (u8CRC >> 7) ^ ((*packetPtr >> s16LoopIndexK) & 0x01);
            u8CRC <<= 1;
            if (u8XoredVal != 0)
                u8CRC ^= 0x1D;
            u8CRC &= 0xFF;
        }
    }
#endif
    /*--------- CRC calculation ends here -------------*/
    packetPtr = strDecParams->packet;   /*Initialize the ptr*/
    packetPtr += 3;
    /* check CRC */
    if (*packetPtr++ != u8CRC)
    {
        /* there were a corruption of the header or a wrong sync */
        /* we have no idea what the frame length was */
        /* let's try to find the next sync */
        strDecParams->u16PrevPacLen = 1;
        return SBC_CRC_FAILURE;
    }

#if (SBC_DEC_SANITY_CHECK == TRUE)
    /* we need to make sure we have enough data before decoding the sbc frame */
    /* some applicaiton are not able to provide intire number of frame */
    /* for those application this sanity check is necessary */

    /* calculate the packet len */
    if (strDecParams->channelMode == SBC_MONO || strDecParams->channelMode == SBC_DUAL)
        tmpu32 += (((strDecParams->numOfBlocks * s32NumOfChannels * strDecParams->bitPool) + 7) / 8);
    else /* SBC_STEREO and SBC_JOINT_STEREO*/
        tmpu32 += (((strDecParams->numOfBlocks * strDecParams->bitPool) + 7) / 8);

    /* make sure we have enough data in the buffer */
    if (strDecParams->u16ReaminingBytes < tmpu32)
    {
        strDecParams->u16PrevPacLen = 0;
        return SBC_EMPTY_FAILURE;
    }
#endif

#if (SBC_DEC_STEREO_INCLUDED == TRUE)
    if (strDecParams->channelMode == SBC_JOINT_STEREO)
    {
        u8XoredVal = 0x80;
        for (s16SbJ = 0; s16SbJ < (s32NumOfSubBands - 1); s16SbJ++)
        {
            strDecParams->join[s16SbJ] = *packetPtr & u8XoredVal;
            u8XoredVal >>= 1;
        }
        strDecParams->join[s16SbJ] = 0;
        if (s32NumOfSubBands == SBC_SUB_BANDS_8)
        {
            packetPtr++;
            /*u16ByteCount++;*/
        }
    }
#endif
    s16SbJ = s32NumOfChannels * s32NumOfSubBands;
#if (SBC_DEC_STEREO_INCLUDED == TRUE) && (SBC_DEC_4_SUBD_INCLUDED == TRUE)
    if ((strDecParams->channelMode == SBC_JOINT_STEREO) &&
            (s32NumOfSubBands == SBC_SUB_BANDS_4))
    {
        /*getting scale factor*/
        ps16GenBufPtr = strDecParams->scaleFactor;
        for (s16ChI = s16SbJ - 1; s16ChI >= 0; s16ChI--)
        {
            *ps16GenBufPtr++ = (*packetPtr++ & 0x0F);
            /*u16ByteCount++;*/
            s16ChI--;
            *ps16GenBufPtr++ = (*packetPtr & 0xF0) >> 4;
        }
        s16PresentBit = 4;
    }
    else
#endif
    {
        /*getting Scale factor*/
        ps16GenBufPtr = strDecParams->scaleFactor ;
        for (s16ChI = s16SbJ - 1; s16ChI >= 0; s16ChI--)
        {
            *ps16GenBufPtr++ = (*packetPtr & 0xF0) >> 4;
            s16ChI--;
            *ps16GenBufPtr++ = (*packetPtr++  & 0x0F);
            /*u16ByteCount++;*/
        }
        s16PresentBit = 8;
    }

    /*bit allocation*/
#if (SBC_DEC_STEREO_INCLUDED == TRUE)
    if ((strDecParams->channelMode == SBC_STEREO) ||
            (strDecParams->channelMode == SBC_JOINT_STEREO))
    {
        sbc_dec_bit_alloc_ste(strDecParams);
    }
    else
#endif
    {
        sbc_dec_bit_alloc_mono(strDecParams);
    }

    /*Unpack samples*/
    SbBuffer = strDecParams->sbBuffer;
    pScaleFactor = strDecParams->scaleFactor;
    ps16GenBufPtr = strDecParams->bits;
    for (s16Blk = (strDecParams->numOfBlocks - 1); s16Blk >= 0 ; s16Blk--)
    {
        for (s16ChI = s16SbJ - 1; s16ChI >= 0; s16ChI--)
        {
            /*store the number of bits required and the quantized
                sb sample to ease the coding*/
            /*u16LoopCount = *ps16GenBufPtr++;*/
            u16QuantizedVal = 0;
            u16LoopCount = *ps16GenBufPtr;

            while (u16LoopCount > (UINT16)s16PresentBit)
            {
                u16LoopCount -= s16PresentBit;
                tmpu32 = *packetPtr;
                u16QuantizedVal |= (tmpu32 & (((UINT32)1 << s16PresentBit) - 1)) << u16LoopCount;
                packetPtr++;
                s16PresentBit = 8;
            }
            s16PresentBit -= u16LoopCount;
            tmpu32 = *packetPtr;
            u16QuantizedVal |= (tmpu32 >> s16PresentBit) & (((UINT32)1 << u16LoopCount) - 1);

            u16LoopCount = *ps16GenBufPtr++;

#if (SBC_ARM_OPT==TRUE)
            s32Level = (1 << u16LoopCount) - 2;
            s32Low1 = (((SINT32)(u16QuantizedVal << 1) - s32Level) << *pScaleFactor);
            __asm{MUL s32Low2, as32LevelLookUp[u16LoopCount], s32Low1}
            *SbBuffer = (SINT16)(s32Low2 >> 17);
#else
            s32Level = (1 << u16LoopCount) - 1;
            s32Low1 = ((((SINT32)(u16QuantizedVal << 1) + 1) - s32Level) << (*pScaleFactor));
            SBC_MULT_32BY16_32_SIMPLIFIED(s32Low1, as32LevelLookUp[u16LoopCount], *SbBuffer);
#if (SBC_IS_64_BITS_IN_DEQUANTIZER==FALSE)
            *SbBuffer >>= 1;
#endif
#endif
            pScaleFactor++;
            SbBuffer++;
        }
        pScaleFactor -= s16SbJ;
        ps16GenBufPtr -= s16SbJ;
    }

    strDecParams->u16PrevPacLen = (packetPtr - strDecParams->packet) + 1;
    return SBC_SUCCESS;
}
