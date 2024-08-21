/******************************************************************************
**
**  File Name:   $RCSfile: sbc_basicop.h,v $
**
**  Description: This is the interface file for sbc_basicop.c services.
**
**  Revision :   $Id: sbc_basicop.h,v 1.16 2004/10/22 14:48:48 mjougit Exp $
**
**  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/
#ifndef SBC_BASICOP_H
#define SBC_BASICOP_H

#include "sbc_decoder.h"

#if (SBC_ARM_OPT==TRUE)
void SBC_FastDCT8(SINT16 *x0, SINT16 *pOutVect);
void SBC_FastDCT4(SINT16 *x0, SINT16 *pOutVect);
#else
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
void SBC_FastDCT8(SINT32 *x0, SINT16 *pOutVect);
void SBC_FastDCT4(SINT32 *x0, SINT16 *pOutVect);
#else
void SBC_FastDCT8(SINT32 *x0, SINT32 *pOutVect);
void SBC_FastDCT4(SINT32 *x0, SINT32 *pOutVect);
#endif
#endif

#if (SBC_ARM_OPT==TRUE)
#if (SBC_ARM_INLINE_ASM == TRUE)
#define SBC_MULT_16BY16_16_SIMPLIFIED(s16In2,s32In1,s32outLow) \
{\
    __asm\
    {\
        MUL s32outLow,s16In2,s32In1;\
        MOV s32outLow,s32outLow,asr 15;\
    }\
}
#else
#define SBC_MULT_16BY16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT32)s32In1*(SINT32)s16In2)>>15)
#endif /* SBC_ARM_INLINE_ASM */
#else
#if (SBC_C5402_OPT==TRUE)
#define SBC_MULT_64(s32In1, s32In2, s32OutLow, s32OutHi) SBC_Multiply_64(s32In1,s32In2,&s32OutLow,&s32OutHi);

#define SBC_MULT_32BY16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) SBC_Multiply_32_16_Simplified(s16In2,s32In1,&s32OutLow);
/*#define SBC_MULT_32BY16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=s16In2*s32In1;*/

#define SBC_MULT_32_32(s32In2, s32In1, s32OutLow) SBC_Multiply_32_32(s32In2,s32In1,&s32OutLow);

#define SBC_MULT_32_32_SIMPLIFIED(s32In2, s32In1, s32OutLow) SBC_Multiply_32_32(s32In2,s32In1,&s32OutLow)

#else

#if (SBC_IPAQ_OPT==TRUE)
#define SBC_MULT_64(s32In1, s32In2, s32OutLow, s32OutHi)  \
{\
        s32OutLow=(SINT32)((SINT64)s32In1*(SINT64)s32In2);\
        s32OutHi=(SINT32)(((SINT64)s32In1*(SINT64)s32In2)>>32);\
}
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define WINDOW_OFFSET 1
#else
#define WINDOW_OFFSET 0
#endif
#define SBC_ACCU_16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow+=(SINT32)(((SINT32)s32In1*(SINT32)s16In2)>>(0+WINDOW_OFFSET))
#define SBC_ACCU_16_32_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow+=(SINT32)(((SINT64)s32In1*(SINT64)s16In2)>>(16+WINDOW_OFFSET))
#define SBC_ACCU_32_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow+=(SINT32)(((SINT64)s32In1*(SINT64)s16In2)>>32)
#define SBC_MULT_16BY16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT32)s32In1*(SINT32)s16In2)>>15)
#define SBC_MULT_16BY16_32_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT32)s32In1*(SINT32)s16In2))
#define SBC_MULT_16BY32_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s16In2)>>31)
#define SBC_MULT_16BY32_32_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s16In2)>>16)
#define SBC_MULT_32BY16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s16In2)>>30)
#define SBC_MULT_32BY16_32_SIMPLIFIED(s32In2, s32In1, s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s32In2)>>15)
#define SBC_MULT_32BY32_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s16In2)>>46)
#define SBC_MULT_32BY32_32_SIMPLIFIED(s32In2, s32In1, s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s32In2)>>31)
#define SBC_MULT_32_32(s32In2, s32In1, s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s32In2)>>15)
#define SBC_MULT_32_32_SIMPLIFIED(s32In2, s32In1, s32OutLow) s32OutLow=(SINT32)(((SINT64)s32In1*(SINT64)s32In2)>>32)
#else
#define SBC_MULT_64(s32In1, s32In2, s32OutLow, s32OutHi)                                \
{                                                                                       \
    \
    s32In1Temp = s32In1;                                                                \
    s32In2Temp = s32In2;                                                                \
    \
    /*If both the operands are -ve then make them +ve*/                                 \
    if( (s32In1Temp < 0) && (s32In2Temp < 0) )                                          \
    {                                                                                   \
        s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;                                     \
        s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;                                     \
    }                                                                                   \
    /*Mutiply two +ve numbers*/                                                         \
    if( (s32In1Temp > 0) && (s32In2Temp > 0) )                                          \
    {                                                                                   \
        s32OutLow = (s32In1Temp & 0xFFFF) * (s32In2Temp & 0xFFFF);                     \
        s32OutHi = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);     \
        s32Temp = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);            \
        s32Carry = (UINT32)((s32OutLow>>16)&0xFFFF);\
        s32Carry += (s32Temp&0xFFFF);\
        s32OutLow += (s32Temp << 16) ;                        \
        s32OutHi += (s32Temp >> 16);            \
        s32Temp = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);            \
        s32Carry += (s32Temp)&0xFFFF;\
        s32Carry>>=16;\
        s32OutLow += (s32Temp << 16);\
        s32OutHi += (s32Temp >> 16);\
        s32OutHi += s32Carry;\
    }                                                                                   \
    /*Multiply one +ve and the other -ve number*/                                       \
    else                                                                                \
    {                                                                                   \
        if(s32In1Temp < 0)                                                              \
            s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;                                 \
        else                                                                            \
            s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;                                 \
        s32OutLow = (s32In1Temp & 0xFFFF) * (s32In2Temp & 0xFFFF);                     \
        s32OutHi = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);     \
        s32Temp = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);            \
        s32Carry = ((UINT32)(s32OutLow>>16)&0xFFFF);\
        s32Carry+=(s32Temp)&0xFFFF;\
        s32OutLow += (s32Temp << 16) ;                        \
        s32OutHi += (s32Temp >> 16);            \
        s32Temp = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);            \
        s32Carry += (s32Temp)&0xFFFF;\
        s32Carry>>=16;\
        s32OutLow += (s32Temp << 16);\
        s32OutHi += (s32Temp >> 16);\
        s32OutHi += s32Carry;\
        \
        s32OutLow ^= 0xFFFFFFFF;                                                       \
        s32Carry = 0;                                                                   \
        if( ((s32OutLow & 0x80000000) == 0x80000000) && (((s32OutLow + 1) & 0x80000000) == 0x00000000) ) \
            s32Carry += 1;                                                              \
        s32OutLow += 1;                                                                \
        s32OutHi ^= 0xFFFFFFFF;                                                        \
        s32OutHi += s32Carry;                                                          \
    }                                                                                   \
}
#define SBC_MULT_64_32(s32In1,s32In2,s32HiTemp)\
{  \
    \
    s32In1Temp = s32In1;                                                                \
    s32In2Temp = s32In2;                                                                \
    \
    /*If both the operands are -ve then make them +ve*/                                 \
    if( (s32In1Temp < 0) && (s32In2Temp < 0) )                                          \
    {                                                                                   \
        s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;                                     \
        s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;                                     \
    }                                                                                   \
    /*Mutiply two +ve numbers*/                                                         \
    if( (s32In1Temp > 0) && (s32In2Temp > 0) )                                          \
    {                                                                                   \
        s32HiTemp = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);     \
        s32Temp = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);            \
        s32Carry = (s32Temp&0xFFFF);\
        s32HiTemp += (s32Temp >> 16);            \
        s32Temp = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);            \
        s32Carry += (s32Temp)&0xFFFF;\
        s32Carry>>=16;\
        s32HiTemp += (s32Temp >> 16);\
        s32HiTemp += s32Carry;\
    }                                                                                   \
    /*Multiply one +ve and the other -ve number*/                                       \
    else                                                                                \
    {                                                                                   \
        if(s32In1Temp < 0)                                                              \
            s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;                                 \
        else                                                                            \
            s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;                                 \
        s32HiTemp = ((s32In1Temp >> 16) & 0xFFFF) * ((s32In2Temp >> 16)  & 0xFFFF);     \
        s32Temp = (s32In1Temp & 0xFFFF) * ((s32In2Temp >> 16) & 0xFFFF);            \
        s32Carry =(s32Temp)&0xFFFF;\
        s32HiTemp += (s32Temp >> 16);            \
        s32Temp = ((s32In1Temp >> 16) & 0xFFFF) * (s32In2Temp & 0xFFFF);            \
        s32Carry += (s32Temp)&0xFFFF;\
        s32Carry>>=16;\
        s32HiTemp += (s32Temp >> 16);\
        s32HiTemp += s32Carry;\
        \
        s32HiTemp ^= 0xFFFFFFFF;                                                        \
    }                                                                                   \
}
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define WINDOW_OFFSET 1
#else
#define WINDOW_OFFSET 0
#endif
#define SBC_ACCU_16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT32)s32In1*(SINT32)s16In2)>>(0+WINDOW_OFFSET))
#define SBC_ACCU_16_32_SIMPLIFIED(s16In2, s32In1 , s32OutLow) \
    {\
        SBC_MULT_16BY32_32_SIMPLIFIED(s16In2, s32In1 , s32OutLow); \
        s32OutLow>>=(WINDOW_OFFSET); \
    };

#define SBC_ACCU_32_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow)   \
    {\
        SBC_MULT_16BY32_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow); \
        s32OutLow>>=1; \
    };

#define SBC_MULT_16BY16_32_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT32)s32In1*(SINT32)s16In2))
#define SBC_MULT_16BY16_16_SIMPLIFIED(s16In2, s32In1 , s32OutLow) s32OutLow=(SINT32)(((SINT32)s32In1*(SINT32)s16In2)>>15)
#define SBC_MULT_16BY32_16_SIMPLIFIED(s32In2, s32In1 , s32OutLow)\
{  \
    s32HiTemp = 0;                                                          \
    SBC_MULT_64(s32In2,s32In1 , s32OutLow, s32HiTemp);           \
    s32OutLow   = (((s32OutLow>>31)&0x1) | (s32HiTemp << 1));          \
}
#define SBC_MULT_16BY32_32_SIMPLIFIED(s32In2, s32In1 , s32OutLow)\
{  \
    s32HiTemp = 0;                                                          \
    SBC_MULT_64(s32In2,s32In1 , s32OutLow, s32HiTemp);           \
    s32OutLow   = (((s32OutLow>>16)&0xFFFF) | (s32HiTemp << 16));          \
}

#define SBC_MULT_32BY16_16_SIMPLIFIED(s32In2, s32In1 , s32OutLow)\
{  \
    s32HiTemp = 0;                                                          \
    SBC_MULT_64(s32In2,s32In1 , s32OutLow, s32HiTemp);           \
    s32OutLow   = (((s32OutLow>>30)&0x3) | (s32HiTemp << 2));          \
}
#define SBC_MULT_32BY16_32_SIMPLIFIED(s32In2, s32In1, s32OutLow)\
{  \
    s32HiTemp = 0;                                                          \
    SBC_MULT_64(s32In2,s32In1 , s32OutLow, s32HiTemp);           \
    s32OutLow   = (((s32OutLow>>15)&0x1FFFF) | (s32HiTemp << 17));          \
}

#define SBC_MULT_32BY32_16_SIMPLIFIED(s32In2, s32In1 , s32OutLow) \
{\
    s32HiTemp=0;\
    SBC_MULT_64_32(s32In2,s32In1,s32HiTemp);\
    s32OutLow   = (s32HiTemp >>14);          \
}
#define SBC_MULT_32BY32_32_SIMPLIFIED(s32In2, s32In1, s32OutLow) \
{  \
    s32HiTemp = 0;                                                          \
    SBC_MULT_64(s32In2,s32In1 , s32OutLow, s32HiTemp);           \
    s32OutLow   = (((s32OutLow>>31)&0x1) | (s32HiTemp << 1));          \
}
#define SBC_MULT_32_32_SIMPLIFIED(s32In2, s32In1, s32OutLow) SBC_MULT_32BY32_SIMPLIFIED(s32In2, s32In1, s32OutLow)

#define SBC_MULT_32BY32_SIMPLIFIED(s32In2, s32In1, s32OutLow)             \
{  \
    s32In2Temp=s32In2;\
    s32In1Temp=s32In1;\
    if (s32In1Temp<0 && s32In2Temp<0)\
    {\
        s32In1Temp=(s32In1Temp^0xFFFFFFFF)+1;\
        s32In2Temp=(s32In2Temp^0xFFFFFFFF)+1;\
    }\
    if (s32In1Temp>0 && s32In2Temp>0)\
    {\
        s32OutLow=(s32In1Temp>>16)*(s32In2Temp>>16);\
        s32OutLow+=((s32In1Temp & 0xFFFF)*(s32In2Temp>>16));\
        s32OutLow+=((s32In2Temp & 0xFFFF)*(s32In1Temp>>16));\
    }\
    else\
    {\
        if(s32In1Temp < 0)\
            s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;\
        else\
            s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;\
        s32OutLow=(s32In1Temp>>16)*(s32In2Temp>>16);\
        s32OutLow+=((s32In1Temp & 0xFFFF)*(s32In2Temp>>16));\
        s32OutLow+=((s32In2Temp & 0xFFFF)*(s32In1Temp>>16));\
        s32OutLow=(s32OutLow^0xFFFFFFFF)+1;\
    }\
}

#define SBC_MULT_32BY16_SIMPLIFIED(s16In2, s32In1 , s32OutLow)                                              \
{                                                                                           \
    s32In1Temp = s32In1;                                                                    \
    s32In2Temp = (SINT32)s16In2;                                                                    \
\
    if (s32In1Temp<0 && s32In2Temp<0)\
    {\
        s32In1Temp=(s32In1Temp^0xFFFFFFFF)+1;\
        s32In2Temp=(s32In2Temp^0xFFFFFFFF)+1;\
    }\
    if (s32In1Temp>0 && s32In2Temp>0)\
    {\
        s32OutLow=(s32In1Temp>>16)*s32In2Temp;\
        s32OutLow+=((s32In1Temp & 0xFFFF)*s32In2Temp)>>16;\
    }\
    else\
    {\
        if(s32In1Temp < 0)\
            s32In1Temp = (s32In1Temp ^ 0xFFFFFFFF) + 1;\
        else\
            s32In2Temp = (s32In2Temp ^ 0xFFFFFFFF) + 1;\
        s32OutLow=(s32In1Temp>>16)*s32In2Temp;\
        s32OutLow+=((s32In1Temp & 0xFFFF)*s32In2Temp)>>16;\
        s32OutLow=(s32OutLow^0xFFFFFFFF)+1;\
    }                                                                                       \
}


#endif
#endif
#endif
#endif
