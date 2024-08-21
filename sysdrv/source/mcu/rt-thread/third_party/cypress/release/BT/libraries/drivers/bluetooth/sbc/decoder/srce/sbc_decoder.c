/*****************************************************************************
**
**  Name    sbc_decoder.c
**
**  Description
**  This file contains the code for bit allocation algorithm.
**  It calculates the number of bits required for the encoded stream of data.
**
**  Copyright (c) 2002-2008, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
*****************************************************************************/

#include <string.h>
#include "sbc_decoder.h"
#include "sbc_basicop.h"

SINT16 SBC_Decoder(SBC_DEC_PARAMS *strDecParams)
{
#if (SBC_OPTIMIZATION==FALSE && SBC_ARM_OPT==FALSE)
    SINT16 s16Blk, s16Sb;
#endif

    UINT8 *pu8PtrHead;  /*start of the packet filling*/
#if (SBC_MINIMUM_BUFFER_COPIES==FALSE)
    UINT8 *pu8PtrTail;  /*end of the packet filling*/
#endif
    UINT16 u16CountTemp;    /* to store the number of bytes filled by fill routine & also used for temporary storage*/
#if (SBC_OPTIMIZATION==FALSE && SBC_ARM_OPT==FALSE)
    SINT16 *ps16SecondaryPcmBuff;   /*to store the PCM samples in order*/
#endif

    SINT32 s32NumOfChannels;
    SINT32 s32NumOfSubBands;
    SINT32 s32NumOfBlocks;
    SINT16 s16Status;

    /*scartch Mem init*/
    if (strDecParams == SBC_NULL)
        return (SBC_MEM_FAILURE);
    if (strDecParams->s32ScartchMem == SBC_NULL)
        return (SBC_MEM_FAILURE);

    /*Replace the old packet content with the new packets*/
    u16CountTemp = strDecParams->u16PrevPacLen;

    pu8PtrHead = strDecParams->packet;
    if (u16CountTemp > strDecParams->u16ReaminingBytes)
    {
        u16CountTemp = strDecParams->u16ReaminingBytes;
    }
    strDecParams->u16ReaminingBytes -= u16CountTemp;

#if (SBC_MINIMUM_BUFFER_COPIES==FALSE)
    pu8PtrTail = pu8PtrHead + u16CountTemp;
    u16CountTemp = strDecParams->u16ReaminingBytes;
    if (strDecParams->u16PrevPacLen != 0)
    {
        /* for(u16Count=0; u16Count < u16RetVal; u16Count++)
        *pu8PtrHead++ = *pu8PtrTail++; */
        /* Can not use memcpy because destination and source regions can overlap */
        memmove(pu8PtrHead, pu8PtrTail, u16CountTemp);

    }
    pu8PtrHead += u16CountTemp;
#endif

    /*Replace the old data bytes with new data */
    u16CountTemp = (*strDecParams->FillBufferFP)(pu8PtrHead, (UINT16)(SBC_MAX_PACKET_LENGTH - strDecParams->u16ReaminingBytes));
    s32NumOfBlocks = strDecParams->numOfBlocks;
    s32NumOfChannels = strDecParams->numOfChannels;
    s32NumOfSubBands = strDecParams->numOfSubBands;

#if (SBC_GKI_BUFFERBASED==TRUE)
    /* let the application allocatin the pcm buffer */
    if (!(*strDecParams->AllocBufferFP)(&(strDecParams->pcmBuffer), \
                                        (UINT16)((s32NumOfBlocks * s32NumOfChannels *   s32NumOfSubBands) * sizeof(SINT16))))
    {
        /* could not allocate buffer */
        return SBC_FAILURE;
    }
#endif

    strDecParams->u16ReaminingBytes += u16CountTemp;

    if (strDecParams->u16ReaminingBytes >= SBC_HEADER_SIZE)
    {
        /* s16Status = DecUnPacking(pstrDecParams);*/
        if ((s16Status = DecUnPacking(strDecParams)) < SBC_SUCCESS)
            return (s16Status);

        SbcSynthesisFilter(strDecParams);

#if (SBC_OPTIMIZATION==TRUE || SBC_ARM_OPT==TRUE)
        (*strDecParams->EmptyBufferFP)(strDecParams->pcmBuffer,
                                       (UINT16)(s32NumOfBlocks *   s32NumOfChannels *  s32NumOfSubBands));
#else
        /*allign all the PCM values continously*/
        u16CountTemp = 0;
        ps16SecondaryPcmBuff = (SINT16 *)strDecParams->s32ScartchMem;
        for (s16Blk = 0; s16Blk < s32NumOfBlocks; s16Blk++)
        {
            for (s16Sb = 0; s16Sb < s32NumOfSubBands; s16Sb++)
            {
                ps16SecondaryPcmBuff[u16CountTemp++] =
                    *(strDecParams->pcmBuffer + s16Blk * BLK + 0 * CHL + s16Sb);
                if (strDecParams->channelMode != SBC_MONO)
                {
                    ps16SecondaryPcmBuff[u16CountTemp++] =
                        *(strDecParams->pcmBuffer + s16Blk * BLK + 1 * CHL +
                          s16Sb);
                }
            }
        }/*end of s16Blk loop*/

        (*strDecParams->EmptyBufferFP)(ps16SecondaryPcmBuff,
                                       (UINT16)(s32NumOfBlocks *   s32NumOfChannels *  s32NumOfSubBands));
#endif
    }
    return SBC_SUCCESS;
}
