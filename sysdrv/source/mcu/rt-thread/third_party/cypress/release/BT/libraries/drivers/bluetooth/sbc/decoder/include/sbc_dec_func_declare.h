/******************************************************************************
**
**  File Name:   $RCSfile: sbc_dec_func_declare.h,v $
**
**  Description: function declarations
**
**  Revision :   $Id: sbc_dec_func_declare.h,v 1.4 2004/04/23 16:02:18 slefort Exp $
**
**  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#ifndef SBCFUNCDECLARE_H
#define SBCFUNCDECLARE_H

#include "sbc_decoder.h"
void Mult64(SINT32 s32In1, SINT32 s32In2, SINT32 *s32OutLow, SINT32 *s32OutHi);
void sbc_dec_bit_alloc_mono(SBC_DEC_PARAMS *CodecParams);
void sbc_dec_bit_alloc_ste(SBC_DEC_PARAMS *CodecParams);
SINT16 SBC_Decoder(SBC_DEC_PARAMS *strDecParams);
SINT16 SBC_Decoder_Init(SBC_DEC_PARAMS *);
/*void DecQuantizer(SINT16 blk, SBC_DEC_PARAMS *);*/
/*void SbcSynthesisFilter(SINT32 *, SINT32 *, SINT16 *, SINT32 *, SINT16);*/
void SbcSynthesisFilter(SBC_DEC_PARAMS *strDecParams);
SINT16 DecUnPacking(SBC_DEC_PARAMS *strDecParams);
void SbcSynthesisFilter4sb(SBC_DEC_PARAMS *strDecParams);
void SbcSynthesisFilter8sb(SBC_DEC_PARAMS *strDecParams);

#endif
