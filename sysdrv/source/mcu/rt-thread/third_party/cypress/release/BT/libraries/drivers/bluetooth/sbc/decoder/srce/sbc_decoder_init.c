/*****************************************************************************
**
**  Name    sbc_decoder.c
**
**  Description
**  This file contains the code for bit allocation algorithm.
**  It calculates the number of bits required for the encoded stream of data.
**
**  Copyright (c) 2002, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
*****************************************************************************/

#include <string.h>
/*#include "gki.h"*/
#include "sbc_decoder.h"

SINT32 DecMaxShiftCounter;
/****************************************************************************
* SBC_Decoder_Init - Allocates the memory and Initializes the arrays to be
* used by synthesis filter
*
* RETURNS : N/A
*/
SINT16 SBC_Decoder_Init(SBC_DEC_PARAMS *strDecParams)
{
    UINT8 *u8PtrTail;
    UINT16 u16NumOfJunkBytes;
    UINT16 u16Count, u16LoopCount, tmp16;
    SINT16 s16LoopIndexK;
    UINT8 u8CRC, u8XoredVal;            /*CRC value*/

    /*static mem init*/

    /*Initalize the vector V*/
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    strDecParams->VX = (SINT16 *)strDecParams->s32StaticMem;
    memset(strDecParams->VX, 0, (DEC_VX_BUFFER_SIZE)*sizeof(SINT16));
#else
    strDecParams->VX = (SINT32 *)strDecParams->s32StaticMem;
    memset(strDecParams->VX, 0, (DEC_VX_BUFFER_SIZE)*sizeof(SINT32));
#endif

    strDecParams->ShiftCounter[0] = 0;
    strDecParams->ShiftCounter[1] = 0;

#if (SBC_MINIMUM_BUFFER_COPIES==FALSE)
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    strDecParams->packet = (UINT8 *)(strDecParams->s32StaticMem + (DEC_VX_BUFFER_SIZE / 2));
#else
    strDecParams->packet = (UINT8 *)(strDecParams->s32StaticMem + DEC_VX_BUFFER_SIZE);
#endif
#endif

    /*get the starting point of the packet and move the data to make it as
    first byte*/
    strDecParams->u16PrevPacLen = 0;
    u16Count = (*strDecParams->FillBufferFP)(strDecParams->packet, SBC_MAX_PACKET_LENGTH);
    strDecParams->u16ReaminingBytes = u16Count;
    u8PtrTail = strDecParams->packet;
    u16NumOfJunkBytes = 0;
    while (*u8PtrTail != 0x9C)
    {
        u8PtrTail++;

        if (++u16NumOfJunkBytes >= strDecParams->u16ReaminingBytes)
        {
            strDecParams->u16ReaminingBytes = 0;
            return SBC_SYNC_FAILURE;
        }
    }

    if (u16NumOfJunkBytes)
    {
#if (SBC_GKI_BUFFERBASED==FALSE)
        memcpy(strDecParams->packet, u8PtrTail, strDecParams->u16ReaminingBytes - u16NumOfJunkBytes);
#endif

        strDecParams->u16ReaminingBytes -= u16NumOfJunkBytes;

        /* Replace the junk bytes with new data */
        u16Count = (*strDecParams->FillBufferFP)(u8PtrTail + strDecParams->u16ReaminingBytes, u16NumOfJunkBytes);

        strDecParams->u16ReaminingBytes += u16Count;

        u8PtrTail = strDecParams->packet;

    }

    /* Make sure there is enough data in the header to perform init */
    if (strDecParams->u16ReaminingBytes < SBC_HEADER_SIZE)
    {
        return SBC_EMPTY_FAILURE;
    }

    u8PtrTail++; /* sync + 1 */

    strDecParams->samplingFreq = (*u8PtrTail & 0xC0) >> 6;  /*sampling freq*/
    strDecParams->numOfBlocks = (((*u8PtrTail & 0x30) >> 4) + 1) * 4;   /*number of blocks*/
    strDecParams->channelMode = (*u8PtrTail & 0x0C) >> 2;   /*channel mode: mono, dual...*/
    strDecParams->allocationMethod = (*u8PtrTail & 0x02) >> 1;  /*Loudness or SNR*/

    if (strDecParams->channelMode == SBC_MONO)
        strDecParams->numOfChannels = 1;
    else
        strDecParams->numOfChannels = 2;

    if (*u8PtrTail & 1) /*Number of subBands*/
    {
        /*        DRV_TRACE_DEBUG0 ("SBC_SUB_BANDS_8");*/
        strDecParams->numOfSubBands = SBC_SUB_BANDS_8;
    }
    else
    {
        /*        DRV_TRACE_DEBUG0 ("SBC_SUB_BANDS_4");*/
        strDecParams->numOfSubBands = SBC_SUB_BANDS_4;
    }

    /* Configure counter limit for shifting optimization */
    if (strDecParams->numOfSubBands == SBC_SUB_BANDS_4)
    {
        if (strDecParams->numOfChannels == 1)
            DecMaxShiftCounter = ((DEC_VX_BUFFER_SIZE - (SBC_SUB_BANDS_4) * 20) >> 3) << 3;
        else
            DecMaxShiftCounter = ((DEC_VX_BUFFER_SIZE - (2 * SBC_SUB_BANDS_4) * 20) >> 4) << 2;
    }
    else
    {
        if (strDecParams->numOfChannels == 1)
            DecMaxShiftCounter = ((DEC_VX_BUFFER_SIZE - (SBC_SUB_BANDS_8) * 20) >> 4) << 4;
        else
            DecMaxShiftCounter = ((DEC_VX_BUFFER_SIZE - (2 * SBC_SUB_BANDS_8) * 20) >> 5) << 4;
    }

    u16LoopCount = (strDecParams->numOfChannels * strDecParams->numOfSubBands);
#if (SBC_DEC_SANITY_CHECK == TRUE)
    /* let's make sure we have enough data to calculate the CRC */
    /* calculate the packet len */
    if (strDecParams->channelMode == SBC_JOINT_STEREO)
        tmp16 = 4 + ((4 * u16LoopCount) / 8) + 1;
    else /* SBC_STEREO, MONO and DUAL */
        tmp16 = 4 + ((4 * u16LoopCount) / 8);

    /* make sure we have enough data in the buffer */
    if (strDecParams->u16ReaminingBytes < tmp16)
    {
        return SBC_EMPTY_FAILURE;
    }
#endif

    /*------------- find CRC -----------------*/
    u8PtrTail = strDecParams->packet + 1; /*Initialize the ptr sync + 1 */
    u8CRC = 0x0F;
    u16LoopCount = (u16LoopCount >> 1);
    /*
    The loops is run from the start of the packet till the scale factor
    parameters. In case of JS, 'join' parameter is included in the packet
    so, that many more bytes are included in u8CRC calculation.
    */

    for (tmp16 = 1; tmp16 < (u16LoopCount + 4); tmp16++)
    {
        /*skip sync word and CRC bytes*/
        if (tmp16 != 3)
        {
            for (s16LoopIndexK = 7; s16LoopIndexK >= 0; s16LoopIndexK--)
            {
                u8XoredVal = (u8CRC >> 7) ^ ((*u8PtrTail >> s16LoopIndexK) & 0x01);
                u8CRC <<= 1;
                if (u8XoredVal != 0)
                    u8CRC ^= 0x1D;
                u8CRC &= 0xFF;
            }

        }
        u8PtrTail++;
    }
#if (SBC_DEC_STEREO_INCLUDED == TRUE)
    if (strDecParams->channelMode == SBC_JOINT_STEREO)
    {
        for (s16LoopIndexK = 7; s16LoopIndexK >= (8 - strDecParams->numOfSubBands); s16LoopIndexK--)
        {
            u8XoredVal = (u8CRC >> 7) ^ ((*u8PtrTail >> s16LoopIndexK) & 0x01);
            u8CRC <<= 1;
            if (u8XoredVal != 0)
                u8CRC ^= 0x1D;
            u8CRC &= 0xFF;
        }
    }
#endif
    /*--------- CRC calculation ends here -------------*/
    u8PtrTail = strDecParams->packet;   /*Initialize the ptr*/
    if (*(u8PtrTail + 3) != u8CRC)
    {
        /* there were a corruption of the header or a wrong sync */
        /* we have no idea what the frame length was */
        /* let's try to find the next sync */
        return SBC_CRC_FAILURE;
    }

#if (SBC_DEC_SANITY_CHECK == TRUE)
    /* we need to make sure we have enough data before decoding the sbc frame */
    /* some applicaiton are not able to provide intire number of frame */
    /* for those application this sanity check is necessary */

    /* calculate the packet len */
    if (strDecParams->channelMode == SBC_MONO || strDecParams->channelMode == SBC_DUAL)
        tmp16 += (((strDecParams->numOfBlocks * strDecParams->numOfChannels * strDecParams->bitPool) + 7) / 8);
    else /* SBC_STEREO and SBC_JOINT_STEREO*/
        tmp16 += (((strDecParams->numOfBlocks * strDecParams->bitPool) + 7) / 8);

    /* make sure we have enough data in the buffer */
    if (strDecParams->u16ReaminingBytes < tmp16)
    {
        strDecParams->u16PrevPacLen = 0;
        return SBC_EMPTY_FAILURE;
    }
#endif

    u8PtrTail++; /* sync + 2 */
    strDecParams->bitPool = *u8PtrTail;

#if (SBC_ARM_OPT==FALSE && SBC_IPAQ_OPT==FALSE && SBC_OPTIMIZATION==FALSE)
    strDecParams->scartchMemForFilter = (SINT32 *)strDecParams->s32ScartchMem;
#endif

    strDecParams->scartchMemForBitAlloc = (SINT16 *)strDecParams->s32ScartchMem;
#if (SBC_MINIMUM_BUFFER_COPIES==TRUE)
#if (SBC_ARM_OPT==TRUE)
    strDecParams->sbBuffer = (SINT16 *)(strDecParams->s32ScartchMem + 8);
    strDecParams->scaleFactor = (SINT16 *)(strDecParams->s32ScartchMem + 8 + 128);
    strDecParams->bits = (SINT16 *)(strDecParams->s32ScartchMem + 8 + 128 + 8);
#else
    strDecParams->sbBuffer = (SINT32 *)(strDecParams->s32ScartchMem + 8);
    strDecParams->scaleFactor = (SINT16 *)(strDecParams->s32ScartchMem + 8 + 256);
    strDecParams->bits = (SINT16 *)(strDecParams->s32ScartchMem + 8 + 256 + 8);
#endif
#else
#if (SBC_ARM_OPT==TRUE)
    strDecParams->sbBuffer = (SINT16 *)(strDecParams->s32ScartchMem + 240);
    strDecParams->scaleFactor = (SINT16 *)(strDecParams->s32ScartchMem + 240 + 128);
    strDecParams->bits = (SINT16 *)(strDecParams->s32ScartchMem + 240 + 128 + 8);
    strDecParams->pcmBuffer = (SINT16 *)(strDecParams->s32ScartchMem + 240 + 128 + 8 + 8);
#else
    strDecParams->sbBuffer = (SINT32 *)(strDecParams->s32ScartchMem + 240);
    strDecParams->scaleFactor = (SINT16 *)(strDecParams->s32ScartchMem + 240 + 256);
    strDecParams->bits = (SINT16 *)(strDecParams->s32ScartchMem + 240 + 256 + 8);
    strDecParams->pcmBuffer = (SINT16 *)(strDecParams->s32ScartchMem + 240 + 256 + 8 + 8);
#endif
#endif

#if (SBC_DEC_STEREO_INCLUDED==FALSE)
    if (strDecParams->channelMode != SBC_MONO)
    {
        return SBC_FAILURE;
    }
#endif

#if (SBC_DEC_4_SUBD_INCLUDED==FALSE)
    if (strDecParams->numOfSubBands == SBC_SUB_BANDS_4)
    {
        return SBC_FAILURE;
    }
#endif

#if (SBC_DEC_8_SUBD_INCLUDED==FALSE)
    if (strDecParams->numOfSubBands == SBC_SUB_BANDS_8)
    {
        return SBC_FAILURE;
    }
#endif

    return SBC_SUCCESS;
}
