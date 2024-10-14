/******************************************************************************
**
**  File Name:   $RCSfile: sbc_basicop.c,v $
**
**  Description: source file for basic opatarion: fast dct, 64-bit multiplication...
**
**  Revision :   $Id: sbc_basicop.c,v 1.18 2004/10/22 14:48:47 mjougit Exp $
**
**  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#include "sbc_decoder.h"
#include "sbc_basicop.h"

/*****************************************************************************/
/*                            C O N S T A N T S                              */
/*****************************************************************************/

/* definition for the fast DST algo */
#if (SBC_IS_64_MULT_IN_DCT == FALSE)
#define COS_PI_SUR_4                (0x00005a82)  /* (0.7071 * (0x8000))      cos(pi/4) */

#define COS_PI_SUR_8                (0x00007641)  /* ((0x8000) * 0.9239)     (cos(pi/8)) */
#define COS_3PI_SUR_8               (0x000030fb)  /* ((0x8000) * 0.3827)     (cos(3*pi/8)) */

#define SBC_1_SUR_2COS_PI_SUR_8     (0x00004545)  /* ((0x8000) / 0.9239)     (1/cos(pi/8)) */
#define SBC_1_SUR_2COS_3PI_SUR_8    (0x0000a73d)  /* ((0x8000) / 0.3827)     (1/cos(3*pi/8)) */

#define SBC_K1             (0x00004CF8) /* SBC_1_SUR_COS_3PI_SUR16/2 */
#define SBC_K2             (0x0000a740) /* 14e80 SBC_1_SUR_COS_PI_SUR_8  */
#define SBC_K3             (0x0000A406) /* 2901b/4 = SBC_1_SUR_4COS_7PI_SUR16 */
#define SBC_K4             (0x00004142) /* 8284/2= SBC_1_SUR_COS_PI_SUR16/2 */
#define SBC_K5             (0x00007332) /* e665/2= SBC_1_SUR_COS_5PI_SUR16/2 */
#define SBC_K6             (0x00004545) /* 8a8b/2= SBC_1_SUR_COS_PI_SUR_8/2 */
#else
#define COS_PI_SUR_4                (0x5A827999)  /* (0.7071 * (0x8000))      cos(pi/4) */

#define COS_PI_SUR_8                (0x7641AF3C)  /* ((0x8000) * 0.9239)     (cos(pi/8)) */
#define COS_3PI_SUR_8               (0x30FBC54D)  /* ((0x8000) * 0.3827)     (cos(3*pi/8)) */

#define SBC_1_SUR_2COS_PI_SUR_8     (0x4545E9EF)  /* ((0x8000) / 0.9239)     (1/cos(pi/8)) */
#define SBC_1_SUR_2COS_3PI_SUR_8    (0xA73D748A)  /* ((0x8000) / 0.3827)     (1/cos(3*pi/8)) */

#define SBC_1_SUR_COS_5PI_SUR16     (0xE664D77D)  /*((0x8000) / 0.5556)      (1/cos(5*pi/16)) */
#define SBC_1_SUR_COS_PI_SUR16      (0x8281F68B)  /*((0x8000) / 0.9808)      (1/cos(pi/16)) */
#define SBC_1_SUR_COS_3PI_SUR16     (0x99F1BD0F)  /*((0x8000) / 0.8315)      (1/cos(3*pi/16)) */
#define SBC_1_SUR_COS_7PI_SUR16     (0x0002901b)  /*((0x8000) / 0.1951)      (1/cos(7*pi/16)) */

#define SBC_K1             (0x4CF8DE87) /* SBC_1_SUR_COS_3PI_SUR16/2 */
#define SBC_K2             (0xa7400000) /* 14e80 SBC_1_SUR_COS_PI_SUR_8 * ?? */
#define SBC_K3             (0xA406CE83) /* 2901b/4 = SBC_1_SUR_4COS_7PI_SUR16 */
#define SBC_K4             (0x4140FB45) /* 8284/2= SBC_1_SUR_COS_PI_SUR16/2 */
#define SBC_K5             (0x73326BBE) /* e665/2= SBC_1_SUR_COS_5PI_SUR16/2 */
#define SBC_K6             (0x4545E9EF) /* 8a8b/2= SBC_1_SUR_COS_PI_SUR_8/2 */
#endif
/* definition for basic DCT  */

#define SBC_SLOW_DCT_SBBUFFER(iIndex, kIndex)                                                   \
{                                                                                               \
    SBC_MULT_32BY16_32_SIMPLIFIED(N[(kIndex*SBC_SUB_BANDS_8)+iIndex] ,x0[iIndex], s32Low);             \
    pOutVect[kIndex] += s32Low;\
}

#define SBC_SLOW_DCT_SBBUFFER_8(Index)                        \
{                                                             \
    pOutVect[Index] = 0;                                     \
    SBC_SLOW_DCT_SBBUFFER(0,Index);                           \
    SBC_SLOW_DCT_SBBUFFER(1,Index);                           \
    SBC_SLOW_DCT_SBBUFFER(2,Index);                           \
    SBC_SLOW_DCT_SBBUFFER(3,Index);                           \
                                                              \
    SBC_SLOW_DCT_SBBUFFER(4,Index);                           \
    SBC_SLOW_DCT_SBBUFFER(5,Index);                           \
    SBC_SLOW_DCT_SBBUFFER(6,Index);                           \
    SBC_SLOW_DCT_SBBUFFER(7,Index);                           \
}


/*
DESCRIPTION
This file contains the code for 64-bit multiplication.
It takes two 32-bit number multiplies and stores them in two 32-bit variables.
INCLUDE FILES: sbc_basicop.h
*/

#if (SBC_C5402_OPT==TRUE)
void SBC_Multiply_32_16_Simplified(SINT16 s16In2Temp, SINT32  s32In1Temp, SINT32 *s32OutLow)                                             \
{
    SINT16 s16In1Hi, s16In1Low;
    SINT32 Temp;
    s16In1Hi = (SINT16)(s32In1Temp >> 16);
    s16In1Low = (SINT16)(s32In1Temp & 0xFFFF);
    if (s32In1Temp < 0)
    {
        s32In1Temp *= -1;
        Temp = ((SINT32)(s16In2Temp * s16In1Low) >> 16) + (SINT32)(s16In2Temp * s16In1Hi);
        Temp *= -1;
    }
    else
    {
        Temp = ((SINT32)(s16In2Temp * s16In1Low) >> 16) + (SINT32)(s16In2Temp * s16In1Hi);
    }
    (*s32OutLow) = Temp << 1;
}

void SBC_Multiply_64(SINT32 s32In1Temp, SINT32 s32In2Temp, SINT32 *s32OutLow, SINT32 *s32OutHi)
{
    SINT32 s32TempVal1, s32TempVal2, s32Carry;
    /*If both the operands are -ve then make them +ve*/
    if ((s32In1Temp < 0) && (s32In2Temp < 0))
    {
        s32In1Temp *= -1;
        s32In2Temp *= -1;
    }
    /*Mutiply two +ve numbers*/
    if ((s32In1Temp > 0) && (s32In2Temp > 0))
    {
        (*s32OutLow) = (s32In1Temp & 0xFFFF) * (s32In2Temp & 0xFFFF);
        (*s32OutHi) = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);
        s32TempVal1 = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);
        s32TempVal2 = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);
        s32Carry = ((((UINT32)((*s32OutLow)) >> 16) & 0xFFFF) + ((s32TempVal1) & 0xFFFF) + ((s32TempVal2) & 0xFFFF)) >> 16;
        (*s32OutLow) += (s32TempVal1 << 16) + (s32TempVal2 << 16);
        (*s32OutHi) += ((s32TempVal1 >> 16) + (s32TempVal2 >> 16) + s32Carry);
    }
    /*Multiply one +ve and the other -ve number*/
    else
    {
        if (s32In1Temp < 0)
            s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;
        else
            s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;
        (*s32OutLow) = (s32In1Temp & 0xFFFF) * (s32In2Temp & 0xFFFF);
        (*s32OutHi) = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);
        s32TempVal1 = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);
        s32TempVal2 = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);
        s32Carry = ((((UINT32)((*s32OutLow)) >> 16) & 0xFFFF) + ((s32TempVal1) & 0xFFFF) + ((s32TempVal2) & 0xFFFF)) >> 16;
        (*s32OutLow) += (s32TempVal1 << 16) + (s32TempVal2 << 16);
        (*s32OutHi) += ((s32TempVal1 >> 16) + (s32TempVal2 >> 16) + s32Carry);

        (*s32OutLow) *= -1;
        s32Carry = 0;
        if ((((*s32OutLow) & 0x80000000) == 0x80000000) && ((((*s32OutLow) + 1) & 0x80000000) == 0x00000000))
            s32Carry += 1;
        (*s32OutHi) ^= 0xFFFFFFFF;
        (*s32OutHi) += s32Carry;
    }
}

void SBC_Multiply_32_32(SINT32 s32In2Temp, SINT32 s32In1Temp, SINT32 *s32OutLow)
{
    SINT32 s32TempVal1, s32TempVal2, s32Carry, s32OutHi;
    s32OutHi = 0;
    /*If both the operands are -ve then make them +ve*/
    if ((s32In1Temp < 0) && (s32In2Temp < 0))
    {
        s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;
        s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;
    }
    /*Mutiply two +ve numbers*/
    if ((s32In1Temp > 0) && (s32In2Temp > 0))
    {
        (*s32OutLow) = (s32In1Temp & 0xFFFF) * (s32In2Temp & 0xFFFF);
        s32OutHi = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);
        s32TempVal1 = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);
        s32TempVal2 = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);
        s32Carry = ((((UINT32)((*s32OutLow)) >> 16) & 0xFFFF) + ((s32TempVal1) & 0xFFFF) + ((s32TempVal2) & 0xFFFF)) >> 16;
        (*s32OutLow) += (s32TempVal1 << 16) + (s32TempVal2 << 16);
        s32OutHi += ((s32TempVal1 >> 16) + (s32TempVal2 >> 16) + s32Carry);
    }
    /*Multiply one +ve and the other -ve number*/
    else
    {
        if (s32In1Temp < 0)
            s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;
        else
            s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;
        (*s32OutLow) = (s32In1Temp & 0xFFFF) * (s32In2Temp & 0xFFFF);
        s32OutHi = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);
        s32TempVal1 = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);
        s32TempVal2 = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);
        s32Carry = ((((UINT32)((*s32OutLow)) >> 16) & 0xFFFF) + ((s32TempVal1) & 0xFFFF) + ((s32TempVal2) & 0xFFFF)) >> 16;
        (*s32OutLow) += (s32TempVal1 << 16) + (s32TempVal2 << 16);
        s32OutHi += ((s32TempVal1 >> 16) + (s32TempVal2 >> 16) + s32Carry);

        (*s32OutLow) ^= 0xFFFFFFFF;
        s32Carry = 0;
        if ((((*s32OutLow) & 0x80000000) == 0x80000000) && ((((*s32OutLow) + 1) & 0x80000000) == 0x00000000))
            s32Carry += 1;
        (*s32OutLow)++;
        s32OutHi ^= 0xFFFFFFFF;
        s32OutHi += s32Carry;
    }
    (*s32OutLow)   = ((((*s32OutLow) >> 15) & 0x1FFFF) | (s32OutHi << 17));
}
#endif

#if (SBC_ARM_OPT==TRUE)
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_16BY16_32_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_16BY16_32_SIMPLIFIED(op1,op2,result)
#else
#if (SBC_IS_64_MULT_IN_DCT==TRUE)
#if (SBC_IS_64_BITS_IN_DEQUANTIZER==TRUE)
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_32BY32_16_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_32BY32_32_SIMPLIFIED(op1,op2,result)
#else
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_32BY32_32_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_32BY32_32_SIMPLIFIED(op1,op2,result)
#endif
#else
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_16BY32_16_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_16BY32_16_SIMPLIFIED(op1,op2,result)
#else
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_16BY32_32_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_16BY32_16_SIMPLIFIED(op1,op2,result)
#endif
#endif
#else
#if (SBC_IS_64_BITS_IN_DEQUANTIZER==TRUE)
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_32BY16_16_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_32BY16_32_SIMPLIFIED(op1,op2,result)
#else
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_32BY16_32_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_32BY16_32_SIMPLIFIED(op1,op2,result)
#endif
#else
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_16BY16_16_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_16BY16_16_SIMPLIFIED(op1,op2,result)
#else
#define SBC_MULT_DCT(op1,op2,result) SBC_MULT_16BY16_32_SIMPLIFIED(op1,op2,result)
#define SBC_MULT_DCT_TEMP(op1,op2,result) SBC_MULT_16BY16_16_SIMPLIFIED(op1,op2,result)
#endif
#endif
#endif
#endif
/*******************************************************************************
**
** Function         SBC_FastDCT4
**
** Description      implementation of fast DCT algorithm
**
**
** Returns          y = dct(x0)
**
**
*******************************************************************************/
#if (SBC_DEC_4_SUBD_INCLUDED == TRUE)
#if (SBC_ARM_OPT==TRUE)
void SBC_FastDCT4(SINT16 *x0, SINT16 *pOutVect)
#else
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
void SBC_FastDCT4(SINT32 *x0, SINT16 *pOutVect)
#else
void SBC_FastDCT4(SINT32 *x0, SINT32 *pOutVect)
#endif
#endif
{
    SINT32 tmp[8], y[4];

#if (SBC_C5402_OPT==TRUE)
#else
#if (SBC_ARM_OPT==TRUE)
#else
#if (SBC_IPAQ_OPT==TRUE)
#else
    SINT32 s32In2Temp, s32HiTemp, s32Temp, s32Carry;
    SINT32 s32In1Temp;
#endif
#endif
#endif

    tmp[0] = x0[0] + x0[3];
    tmp[1] = -(x0[1] + x0[2]);
    tmp[2] = x0[0] - x0[3];
    tmp[3] = x0[2] - x0[1];

    SBC_MULT_DCT(COS_PI_SUR_8, tmp[2], tmp[4]);
    SBC_MULT_DCT(COS_3PI_SUR_8, tmp[2], tmp[5]);
    SBC_MULT_DCT(COS_PI_SUR_8, tmp[3], tmp[6]);
    SBC_MULT_DCT(COS_3PI_SUR_8, tmp[3], tmp[7]);
    SBC_MULT_DCT(COS_PI_SUR_4, (tmp[0] + tmp[1]), y[0]);

    y[1] = (tmp[5] + tmp[6]);
    y[2] = (tmp[7] - tmp[4]);
    y[3] = (tmp[1] - tmp[0]);
#if (SBC_IS_64_BITS_IN_DEQUANTIZER==FALSE)
    y[3] <<= 15;
#endif
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    y[3] >>= 15;
#endif
    //printf("%ld %ld %ld %ld\n",y[0],y[1],y[2],y[3]);
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    pOutVect[0]  = (SINT16)y[0];
    pOutVect[1]  = (SINT16)y[1];
    /*pOutVect[2]  = (SINT16)0;*/
    pOutVect[3]  = (SINT16) - y[1];
    pOutVect[4]  = (SINT16) - y[0];
    pOutVect[5]  = (SINT16)y[2];
    pOutVect[6]  = (SINT16)y[3];
    pOutVect[7]  = (SINT16)y[2];
#else
    pOutVect[0]  = (SINT32)y[0];
    pOutVect[1]  = (SINT32)y[1];
    /*pOutVect[2]  = (SINT32)0;*/
    pOutVect[3]  = (SINT32) - y[1];
    pOutVect[4]  = (SINT32) - y[0];
    pOutVect[5]  = (SINT32)y[2];
    pOutVect[6]  = (SINT32)y[3];
    pOutVect[7]  = (SINT32)y[2];
#endif
}
#endif

/*******************************************************************************
**
** Function         SBC_FastDCT8
**
** Description      implementation of fast DCT algorithm by Feig and Winograd
**
**
** Returns          y = dct(x0)
**
**
*******************************************************************************/
#if (SBC_DEC_8_SUBD_INCLUDED == TRUE)
#if (SBC_ARM_OPT==TRUE)
void SBC_FastDCT8(SINT16 *x0, SINT16 *pOutVect)
#else
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
void SBC_FastDCT8(SINT32 *x0, SINT16 *pOutVect)
#else
void SBC_FastDCT8(SINT32 *x0, SINT32 *pOutVect)
#endif
#endif
{
#if (SBC_C5402_OPT==TRUE)
#else
#if (SBC_ARM_OPT==TRUE)
#else
#if (SBC_IPAQ_OPT==TRUE)
#else
    SINT32 s32Temp, s32Carry, s32HiTemp, s32In2Temp;
    SINT32 s32In1Temp;
#endif
#endif
#endif

#if (SBC_C5402_OPT==TRUE || SBC_OPTIMIZATION==TRUE || SBC_ARM_OPT==TRUE)
    SINT32 x1[4], x2[4], x3[8], tmp0, y[8];
    /*Note that the array index is changed from 0:7 to 1:8*/
    x1[0] = x0[7] + x0[0];
    x3[4] = x0[7] - x0[0];
    x1[1] = x0[6] + x0[1];
    x3[3] = x0[6] - x0[1];
    x1[2] = x0[5] + x0[2];
    x3[1] = x0[5] - x0[2];
    x1[3] = x0[3] + x0[4];
    x3[2] = x0[3] - x0[4];

    x2[0] = x1[0] + x1[3];
    x2[3] = x1[0] - x1[3];
    x2[1] = x1[1] + x1[2];
    x2[2] = x1[1] - x1[2];

    x3[0] = x2[0] - x2[1];


    /* multiply by (1/2) G1*/
    y[0] = x2[0] + x2[1];

    SBC_MULT_DCT((COS_PI_SUR_4), x3[0], y[4]);/*y[4] = (1/2) * COS_PI_SUR_4 * x[1][2];*/
    /* multiply by [1/2) G2*/
    SBC_MULT_DCT_TEMP(COS_PI_SUR_4, (x2[2] - x2[3]), tmp0); /*tmp0 = COS_PI_SUR_4 * (-x[2][2]+x[3][2]);    */
    //printf("%ld %ld %ld \n",(COS_PI_SUR_4), (x2[2]-x2[3]), tmp0);
    SBC_MULT_DCT(SBC_K2, (-x2[2] + tmp0), y[2]);/*y[2] = (1/4) * SBC_1_SUR_COS_PI_SUR_8 * ( x[2][2] + tmp0);*/
    SBC_MULT_DCT(SBC_K6, (x2[2] + tmp0), y[6]);    /*y[6] = (1/4) * SBC_1_SUR_COS_PI_SUR_8 * (-x[2][2] + tmp0);*/
    /* Now multiply by (1/2)G4
       multiply by H_42*/
    x3[5] = x3[2] + x3[4];
    x3[6] = x3[1] - x3[4];
    x3[7] = x3[2] - x3[3];


    /* multiply by 1, G1, G2*/
    SBC_MULT_DCT_TEMP(COS_PI_SUR_4, x3[5], x3[5]);/*tmp1[4][0] =  COS_PI_SUR_4 * x3[5];*/
    SBC_MULT_DCT_TEMP(COS_PI_SUR_4, (-x3[6] + x3[7]), tmp0); /*tmp0 =        COS_PI_SUR_4 * (-x3[6] + x3[7]);   */
    SBC_MULT_DCT_TEMP((SBC_1_SUR_2COS_PI_SUR_8), (-x3[6] + tmp0), x3[7]);   /*tmp1[6][0] = (1/2) * SBC_1_SUR_COS_PI_SUR_8  * (-x3[6] + tmp0);*/
    SBC_MULT_DCT_TEMP((SBC_1_SUR_2COS_3PI_SUR_8), (x3[6] + tmp0), x3[6]);   /*tmp1[5][0] = (1/2) * SBC_1_SUR_COS_3PI_SUR_8 * ( x3[6] + tmp0);*/


    /* multiply by H_41, then by D^-1, and then 1/2 this is G4 */
    /* then multiply by (1/2) to make it (1/2) G4 */

    SBC_MULT_DCT((SBC_K5), (x3[1] + x3[5] - x3[6]), y[1]);/* y[1] = -(1/4) * SBC_1_SUR_COS_5PI_SUR16 * ( tmp1[3][0] + tmp1[4][0] - tmp1[5][0]);*/
    SBC_MULT_DCT(SBC_K4, (-x3[1] + x3[5] + x3[7]), y[3]); /* y[3] = -(1/4) * SBC_1_SUR_COS_PI_SUR16  * (-tmp1[3][0] + tmp1[4][0] + tmp1[6][0]); */
    SBC_MULT_DCT((SBC_K1), (x3[1] + x3[5] + x3[6]), y[7]); /*y[7] =  (1/4) * SBC_1_SUR_COS_3PI_SUR16 * (-tmp1[3][0] - tmp1[4][0] - tmp1[5][0]);*/
    SBC_MULT_DCT(SBC_K3, (-x3[1] + x3[5] - x3[7]), y[5]); /*y[5] = -(1/4) * SBC_1_SUR_COS_7PI_SUR16 * ( tmp1[3][0] - tmp1[4][0] + tmp1[6][0]);  */
    y[5] <<= 1;

    /* Multiply per Zoro matrix 8 rows and 16 colons */
    /*    printf("outvect %08d, %08d = %08d + %08d + %08d\n",(SBC_K1*2), (-tmp1[3][0] - tmp1[4][0] - tmp1[5][0]),
    tmp1[3][0], tmp1[4][0], tmp1[5][0]);*/
#if (SBC_IS_64_BITS_IN_DEQUANTIZER==FALSE)
    y[0] <<= 15;
#endif
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    y[0] >>= 15;
#endif
    //printf("%ld %ld %ld %ld %ld %ld %ld %ld\n",y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7]);
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    pOutVect[0]  = (SINT16)y[4];
    pOutVect[1]  = (SINT16)y[5];
    pOutVect[2]  = (SINT16) - y[6];
    pOutVect[3]  = (SINT16) - y[7];
    /*pOutVect[4]  = (SINT16)0;*/
    pOutVect[5]  = (SINT16)y[7];
    pOutVect[6]  = (SINT16)y[6];
    pOutVect[7]  = (SINT16) - y[5];
    pOutVect[8]  = (SINT16) - y[4];
    pOutVect[9]  = (SINT16)y[3];
    pOutVect[10] = (SINT16)y[2];
    pOutVect[11] = (SINT16)y[1];
    pOutVect[12] = (SINT16) - y[0];
    pOutVect[13] = (SINT16)y[1];
    pOutVect[14] = (SINT16)y[2];
    pOutVect[15] = (SINT16)y[3];
#else
    pOutVect[0]  = (SINT32)y[4];
    pOutVect[1]  = (SINT32)y[5];
    pOutVect[2]  = (SINT32) - y[6];
    pOutVect[3]  = (SINT32) - y[7];
    /*pOutVect[4]  = (SINT32)0;*/
    pOutVect[5]  = (SINT32)y[7];
    pOutVect[6]  = (SINT32)y[6];
    pOutVect[7]  = (SINT32) - y[5];
    pOutVect[8]  = (SINT32) - y[4];
    pOutVect[9]  = (SINT32)y[3];
    pOutVect[10] = (SINT32)y[2];
    pOutVect[11] = (SINT32)y[1];
    pOutVect[12] = (SINT32) - y[0];
    pOutVect[13] = (SINT32)y[1];
    pOutVect[14] = (SINT32)y[2];
    pOutVect[15] = (SINT32)y[3];
#endif
#else
    SINT16 k, i;
    SINT32 s32Low;
    const SINT16 *N;
    N = DCTcoeff8;

    for (k = 0; k < (SBC_SUB_BANDS_8 * 2); k++)
    {
        pOutVect[k] = 0;
        for (i = 0; i < SBC_SUB_BANDS_8; i++)
            SBC_SLOW_DCT_SBBUFFER(i, k);
    }

#endif
    /*    printf("VX = [%08d; %08d; %08d; %08d; %08d; %08d; %08d; %08d] \n     [%08d; %08d; %08d; %08d; %08d; %08d; %08d; %08d]\n", \
            pOutVect[0], pOutVect[1], pOutVect[2], \
            pOutVect[3], pOutVect[4], pOutVect[5], \
            pOutVect[6], pOutVect[7], pOutVect[8], \
            pOutVect[9], pOutVect[10], pOutVect[11], \
            pOutVect[12], pOutVect[13], pOutVect[14], \
            pOutVect[15] );*/

}
#endif
