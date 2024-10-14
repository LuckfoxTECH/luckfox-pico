/******************************************************************************
**
**  File Name:   $RCSfile: sbc_synthesis.c,v $
**
**  Description: This is the source file for SBC analysis filter.
**
**  Revision :   $Id: sbc_synthesis.c,v 1.31 2004/10/22 14:48:48 mjougit Exp $
**
**  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.
**  Widcomm Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/


#include <string.h>
#include "sbc_decoder.h"
#include "sbc_basicop.h"

#define SBC_MAX_SINT16 (32767)
#define SBC_MIN_SINT16 (-32768)

#if (SBC_IS_64_BITS_IN_WINDOWING==TRUE)
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define WINDOW_ACCU(op1,op2,result) SBC_ACCU_16_32_SIMPLIFIED(op1,op2,result)
#else
#define WINDOW_ACCU(op1,op2,result) SBC_ACCU_32_16_SIMPLIFIED(op1,op2,result)
#endif
#define WIND_4_SUBBANDS_0_1       (SINT32)(0x007DBCC8)    /*(0.00383720193 * 0x8000)*/
#define WIND_4_SUBBANDS_0_2       (SINT32)(0x01659F45)    /*(0.0109137620 * 0x8000)*/
#define WIND_4_SUBBANDS_0_3       (SINT32)(0x034FEE2C)    /*(0.0258767811 * 0x8000)*/
#define WIND_4_SUBBANDS_0_4       (SINT32)(0x115B1ED2)    /*(0.135593274 * 0x8000)*/
#define WIND_4_SUBBANDS_0_5       (SINT32)(0x25AC1FF2)    /*(0.294315332 * 0x8000)*/
#define WIND_4_SUBBANDS_1_0       (SINT32)(0x001194E6)    /*(0.000536548976 * 0x8000)*/
#define WIND_4_SUBBANDS_1_1       (SINT32)(0x007F88E4)    /*(0.00389205149 * 0x8000)*/
#define WIND_4_SUBBANDS_1_2       (SINT32)(0x029DBAA3)    /*(0.0204385087 * 0x8000)*/
#define WIND_4_SUBBANDS_1_3       (SINT32)(0x00C8F2BC)    /*(0.00613245186 * 0x8000)*/
#define WIND_4_SUBBANDS_1_4       (SINT32)(0x18F55C90)    /*(0.194987841 * 0x8000)*/
#define WIND_4_SUBBANDS_1_5       (SINT32)(0x2412F251)    /*(0.281828203 * 0x8000)*/
#define WIND_4_SUBBANDS_1_6       (SINT32)(0xF60FAF37)    /*(-0.0776463494 * 0x8000)*/
#define WIND_4_SUBBANDS_1_7       (SINT32)(0x041EEE40)    /*(0.0321939290 * 0x8000)*/
#define WIND_4_SUBBANDS_1_8       (SINT32)(0xFF9BB9D5)    /*(-0.00306012286 * 0x8000)*/
#define WIND_4_SUBBANDS_1_9       (SINT32)(0x00599403)    /*(0.00273370904 * 0x8000)*/
#define WIND_4_SUBBANDS_2_0       (SINT32)(0x0030E2D3)    /*(0.00149188357 * 0x8000)*/
#define WIND_4_SUBBANDS_2_1       (SINT32)(0x003D239B)    /*(0.00186581691 * 0x8000)*/
#define WIND_4_SUBBANDS_2_2       (SINT32)(0x03B23341)    /*(0.0288757392 * 0x8000)*/
#define WIND_4_SUBBANDS_2_3       (SINT32)(0xFC4F91D4)    /*(-0.0288217274 * 0x8000)*/
#define WIND_4_SUBBANDS_2_4       (SINT32)(0x1F91CA46)    /*(0.246636662 * 0x8000)*/

#define WIND_8_SUBBANDS_0_1       (SINT32)(0x0041EC6A)    /*(0.00201182542 * 0x80000000)*/
#define WIND_8_SUBBANDS_0_2       (SINT32)(0x00B97348)    /*(0.00565949473* 0x80000000)*/
#define WIND_8_SUBBANDS_0_3       (SINT32)(0x01A7ECEF)    /*(0.0129371806 * 0x80000000)*/
#define WIND_8_SUBBANDS_0_4       (SINT32)(0x08B4307A)    /*(0.0679989431 * 0x80000000)*/
#define WIND_8_SUBBANDS_0_5       (SINT32)(0x12CF6C75)    /*(0.146955068 * 0x80000000)*/

#define WIND_8_SUBBANDS_1_0       (SINT32)(0x00052173)    /*(0.000156575398 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_1       (SINT32)(0x0044EF48)    /*(0.00210371989 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_2       (SINT32)(0x01071B96)    /*(0.00802941163 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_3       (SINT32)(0x01223EBA)    /*(0.00885757540 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_4       (SINT32)(0x0A9F3E9A)    /*(0.0829847578 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_5       (SINT32)(0x129C226F)    /*(0.145389847 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_6       (SINT32)(0xF9312891)    /*(-0.0531873032 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_7       (SINT32)(0x01F5F424)    /*(0.0153184106 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_8       (SINT32)(0xFF8D6793)    /*(-0.00349717454* 0x80000000)*/
#define WIND_8_SUBBANDS_1_9       (SINT32)(0x003A72E7)    /*(0.00178371725 * 0x80000000)*/

#define WIND_8_SUBBANDS_2_0       (SINT32)(0x000B3F71)    /*(0.000343256425 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_1       (SINT32)(0x00415B75)    /*(0.00199454554 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_2       (SINT32)(0x0156B3CA)    /*(0.0104584443 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_3       (SINT32)(0x005FD0FF)    /*(0.00292408442 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_4       (SINT32)(0x0C7D59B6)    /*(0.0975753918 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_5       (SINT32)(0x120435FA)    /*(0.140753505* 0x80000000)*/
#define WIND_8_SUBBANDS_2_6       (SINT32)(0xFAFF95FC)    /*(-0.0390751381* 0x80000000)*/
#define WIND_8_SUBBANDS_2_7       (SINT32)(0x02138653)    /*(0.0162208471* 0x80000000)*/
#define WIND_8_SUBBANDS_2_8       (SINT32)(0xFFC9F10E)    /*(-0.00164973098* 0x80000000)*/
#define WIND_8_SUBBANDS_2_9       (SINT32)(0x003060F4)    /*(0.00147640169 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_0       (SINT32)(0x00122C7D)    /*(0.000554620202* 0x80000000)*/
#define WIND_8_SUBBANDS_3_1       (SINT32)(0x0034F8B6)    /*(0.00161656283 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_2       (SINT32)(0x01A1B38B)    /*(0.0127472335 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_3       (SINT32)(0xFF5EEB73)    /*(-0.00491578024 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_4       (SINT32)(0x0E3BB16F)    /*(0.111196689 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_5       (SINT32)(0x110ECEF0)    /*(0.133264415* 0x80000000)*/
#define WIND_8_SUBBANDS_3_6       (SINT32)(0xFCA86E7E)    /*(-0.0261098752* 0x80000000)*/
#define WIND_8_SUBBANDS_3_7       (SINT32)(0x0209291F)    /*(0.0159045603* 0x80000000)*/
#define WIND_8_SUBBANDS_3_8       (SINT32)(0xFFFA2413)    /*(-0.000178805361* 0x80000000)*/
#define WIND_8_SUBBANDS_3_9       (SINT32)(0x00255A62)    /*(0.00113992507 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_0       (SINT32)(0x001AFF89)    /*(0.000823919506 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_1       (SINT32)(0x001D8FD2)    /*(0.000902154502 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_2       (SINT32)(0x01E0224C)    /*(0.0146525263 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_3       (SINT32)(0xFE20435D)    /*(-0.0146404076 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_4       (SINT32)(0x0FC721F9)    /*(0.123264548 * 0x80000000)*/
#else
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
#define WINDOW_ACCU(op1,op2,result) SBC_ACCU_16_16_SIMPLIFIED(op1,op2,result)
#else
#define WINDOW_ACCU(op1,op2,result) SBC_ACCU_16_32_SIMPLIFIED(op1,op2,result)
#endif
#define WIND_4_SUBBANDS_0_1       (SINT16)(0x007e)    /*(0.00383720193 * 0x8000)*/
#define WIND_4_SUBBANDS_0_2       (SINT16)(0x0165)    /*(0.0109137620 * 0x8000)*/
#define WIND_4_SUBBANDS_0_3       (SINT16)(0x0350)    /*(0.0258767811 * 0x8000)*/
#define WIND_4_SUBBANDS_0_4       (SINT16)(0x115b)    /*(0.135593274 * 0x8000)*/
#define WIND_4_SUBBANDS_0_5       (SINT16)(0x25ac)    /*(0.294315332 * 0x8000)*/
#define WIND_4_SUBBANDS_1_0       (SINT16)(0x0012)    /*(0.000536548976 * 0x8000)*/
#define WIND_4_SUBBANDS_1_1       (SINT16)(0x0080)    /*(0.00389205149 * 0x8000)*/
#define WIND_4_SUBBANDS_1_2       (SINT16)(0x029e)    /*(0.0204385087 * 0x8000)*/
#define WIND_4_SUBBANDS_1_3       (SINT16)(0x00c9)    /*(0.00613245186 * 0x8000)*/
#define WIND_4_SUBBANDS_1_4       (SINT16)(0x18f5)    /*(0.194987841 * 0x8000)*/
#define WIND_4_SUBBANDS_1_5       (SINT16)(0x2413)    /*(0.281828203 * 0x8000)*/
#define WIND_4_SUBBANDS_1_6       (SINT16)(0xf610)    /*(-0.0776463494 * 0x8000)*/
#define WIND_4_SUBBANDS_1_7       (SINT16)(0x041f)    /*(0.0321939290 * 0x8000)*/
#define WIND_4_SUBBANDS_1_8       (SINT16)(0xff9c)    /*(-0.00306012286 * 0x8000)*/
#define WIND_4_SUBBANDS_1_9       (SINT16)(0x005a)    /*(0.00273370904 * 0x8000)*/
#define WIND_4_SUBBANDS_2_0       (SINT16)(0x0031)    /*(0.00149188357 * 0x8000)*/
#define WIND_4_SUBBANDS_2_1       (SINT16)(0x003d)    /*(0.00186581691 * 0x8000)*/
#define WIND_4_SUBBANDS_2_2       (SINT16)(0x03b2)    /*(0.0288757392 * 0x8000)*/
#define WIND_4_SUBBANDS_2_3       (SINT16)(0xfc50)    /*(-0.0288217274 * 0x8000)*/
#define WIND_4_SUBBANDS_2_4       (SINT16)(0x1f92)    /*(0.246636662 * 0x8000)*/

#define WIND_8_SUBBANDS_0_1       (SINT16)(0x0041)    /*(0.00201182542 * 0x80000000)*/
#define WIND_8_SUBBANDS_0_2       (SINT16)(0x00b9)    /*(0.00565949473* 0x80000000)*/
#define WIND_8_SUBBANDS_0_3       (SINT16)(0x01a7)    /*(0.0129371806 * 0x80000000)*/
#define WIND_8_SUBBANDS_0_4       (SINT16)(0x08b4)    /*(0.0679989431 * 0x80000000)*/
#define WIND_8_SUBBANDS_0_5       (SINT16)(0x12cf)    /*(0.146955068 * 0x80000000)*/

#define WIND_8_SUBBANDS_1_0       (SINT16)(0x0005)    /*(0.000156575398 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_1       (SINT16)(0x0045)    /*(0.00210371989 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_2       (SINT16)(0x0107)    /*(0.00802941163 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_3       (SINT16)(0x0122)    /*(0.00885757540 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_4       (SINT16)(0x0aa0)    /*(0.0829847578 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_5       (SINT16)(0x129c)    /*(0.145389847 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_6       (SINT16)(0xf932)    /*(-0.0531873032 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_7       (SINT16)(0x01f6)    /*(0.0153184106 * 0x80000000)*/
#define WIND_8_SUBBANDS_1_8       (SINT16)(0xff8e)    /*(-0.00349717454* 0x80000000)*/
#define WIND_8_SUBBANDS_1_9       (SINT16)(0x003b)    /*(0.00178371725 * 0x80000000)*/

#define WIND_8_SUBBANDS_2_0       (SINT16)(0x000b)    /*(0.000343256425 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_1       (SINT16)(0x0041)    /*(0.00199454554 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_2       (SINT16)(0x0157)    /*(0.0104584443 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_3       (SINT16)(0x0060)    /*(0.00292408442 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_4       (SINT16)(0x0c7d)    /*(0.0975753918 * 0x80000000)*/
#define WIND_8_SUBBANDS_2_5       (SINT16)(0x1204)    /*(0.140753505* 0x80000000)*/
#define WIND_8_SUBBANDS_2_6       (SINT16)(0xfb00)    /*(-0.0390751381* 0x80000000)*/
#define WIND_8_SUBBANDS_2_7       (SINT16)(0x0214)    /*(0.0162208471* 0x80000000)*/
#define WIND_8_SUBBANDS_2_8       (SINT16)(0xffca)    /*(-0.00164973098* 0x80000000)*/
#define WIND_8_SUBBANDS_2_9       (SINT16)(0x0030)    /*(0.00147640169 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_0       (SINT16)(0x0012)    /*(0.000554620202* 0x80000000)*/
#define WIND_8_SUBBANDS_3_1       (SINT16)(0x0035)    /*(0.00161656283 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_2       (SINT16)(0x01a2)    /*(0.0127472335 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_3       (SINT16)(0xff5f)    /*(-0.00491578024 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_4       (SINT16)(0x0e3c)    /*(0.0.111196689 * 0x80000000)*/
#define WIND_8_SUBBANDS_3_5       (SINT16)(0x110f)    /*(0.133264415* 0x80000000)*/
#define WIND_8_SUBBANDS_3_6       (SINT16)(0xfca9)    /*(-0.0261098752* 0x80000000)*/
#define WIND_8_SUBBANDS_3_7       (SINT16)(0x0209)    /*(0.0159045603* 0x80000000)*/
#define WIND_8_SUBBANDS_3_8       (SINT16)(0xfffb)    /*(-0.000178805361* 0x80000000)*/
#define WIND_8_SUBBANDS_3_9       (SINT16)(0x0025)    /*(0.00113992507 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_0       (SINT16)(0x001b)    /*(0.000823919506 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_1       (SINT16)(0x001e)    /*(0.000902154502 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_2       (SINT16)(0x01e0)    /*(0.0146525263 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_3       (SINT16)(0xfe21)    /*(-0.0146404076 * 0x80000000)*/
#define WIND_8_SUBBANDS_4_4       (SINT16)(0x0fc7)    /*(0.123264548 * 0x80000000)*/
#endif

/*GLOBALS*/
#if (SBC_ARM_OPT==FALSE && SBC_IPAQ_OPT==FALSE)
extern const SINT32 coeffFor4SBs[];
extern const SINT32 coeffFor8SBs[];
#endif
/*extern const SINT16 DCTcoeff4[];
extern const SINT16 DCTcoeff8[];*/
extern SINT32 DecMaxShiftCounter;

#if (SBC_OPTIMIZATION == TRUE || SBC_ARM_OPT==TRUE || SBC_IPAQ_OPT==TRUE)
#if (SBC_ARM_OPT==TRUE)
#define SBC_WINDOW_CALC_4_0                                                     \
{                                                                               \
    __asm\
{\
    MUL s32Hi,WIND_4_SUBBANDS_0_1,(VX[ChOffset+SBC_SUB_BANDS_4*19]+VX[ChOffset+SBC_SUB_BANDS_4*3]);\
    MLA s32Hi,WIND_4_SUBBANDS_0_2,(VX[ChOffset+SBC_SUB_BANDS_4*4]-VX[ChOffset+SBC_SUB_BANDS_4*16]),s32Hi;\
    MLA s32Hi,WIND_4_SUBBANDS_0_3,(VX[ChOffset+SBC_SUB_BANDS_4*7]+VX[ChOffset+SBC_SUB_BANDS_4*15]),s32Hi;\
    MLA s32Hi,WIND_4_SUBBANDS_0_4,(VX[ChOffset+SBC_SUB_BANDS_4*8]-VX[ChOffset+SBC_SUB_BANDS_4*12]),s32Hi;\
    MLA s32Hi,WIND_4_SUBBANDS_0_5,VX[ChOffset+SBC_SUB_BANDS_4*11],s32Hi;\
    MVN s32Hi,s32Hi,asr 11;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
}\
    pcmBuffer[ChSbOffset]=(SINT16)s32Hi;\
}

#define SBC_WINDOW_CALC_4_1_3                                               \
{                                                                               \
    __asm\
{\
    MUL s32Hi,WIND_4_SUBBANDS_1_0,VX[ChOffset+1];\
    MUL s32Hi2,WIND_4_SUBBANDS_1_0,VX[ChOffset+SBC_SUB_BANDS_4*19+3];\
    MLA s32Hi,WIND_4_SUBBANDS_1_1,VX[ChOffset+SBC_SUB_BANDS_4*3+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_1,VX[ChOffset+SBC_SUB_BANDS_4*16+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_2,VX[ChOffset+SBC_SUB_BANDS_4*4+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_2,VX[ChOffset+SBC_SUB_BANDS_4*15+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_3,VX[ChOffset+SBC_SUB_BANDS_4*7+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_3,VX[ChOffset+SBC_SUB_BANDS_4*12+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_4,VX[ChOffset+SBC_SUB_BANDS_4*8+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_4,VX[ChOffset+SBC_SUB_BANDS_4*11+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_5,VX[ChOffset+SBC_SUB_BANDS_4*11+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_5,VX[ChOffset+SBC_SUB_BANDS_4*8+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_6,VX[ChOffset+SBC_SUB_BANDS_4*12+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_6,VX[ChOffset+SBC_SUB_BANDS_4*7+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_7,VX[ChOffset+SBC_SUB_BANDS_4*15+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_7,VX[ChOffset+SBC_SUB_BANDS_4*4+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_8,VX[ChOffset+SBC_SUB_BANDS_4*16+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_8,VX[ChOffset+SBC_SUB_BANDS_4*3+3],s32Hi2;\
    MLA s32Hi,WIND_4_SUBBANDS_1_9,VX[ChOffset+SBC_SUB_BANDS_4*19+1],s32Hi;\
    MLA s32Hi2,WIND_4_SUBBANDS_1_9,VX[ChOffset+3],s32Hi2;\
    MVN s32Hi,s32Hi,asr 11;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
    MVN s32Hi2,s32Hi2,asr 11;\
    CMP s32Hi2,ThresholdP;\
    MOVPL s32Hi2,ThresholdP;\
    CMP s32Hi2,ThresholdN;\
    MOVMI s32Hi2,ThresholdN;\
}\
    pcmBuffer[ChSbOffset+s32NumOfChannels]=(SINT16)s32Hi;\
    pcmBuffer[ChSbOffset+3*s32NumOfChannels]=(SINT16)s32Hi2;\
}
#define SBC_WINDOW_CALC_4_2                                                 \
{                                                                               \
    __asm\
{\
    MUL s32Hi,WIND_4_SUBBANDS_2_0,(VX[ChOffset+2]+VX[ChOffset+SBC_SUB_BANDS_4*19+2]);\
    MLA s32Hi,WIND_4_SUBBANDS_2_1,(VX[ChOffset+SBC_SUB_BANDS_4*3+2]+VX[ChOffset+SBC_SUB_BANDS_4*16+2]),s32Hi;\
    MLA s32Hi,WIND_4_SUBBANDS_2_2,(VX[ChOffset+SBC_SUB_BANDS_4*4+2]+VX[ChOffset+SBC_SUB_BANDS_4*15+2]),s32Hi;\
    MLA s32Hi,WIND_4_SUBBANDS_2_3,(VX[ChOffset+SBC_SUB_BANDS_4*7+2]+VX[ChOffset+SBC_SUB_BANDS_4*12+2]),s32Hi;\
    MLA s32Hi,WIND_4_SUBBANDS_2_4,(VX[ChOffset+SBC_SUB_BANDS_4*8+2]+VX[ChOffset+SBC_SUB_BANDS_4*11+2]),s32Hi;\
    MVN s32Hi,s32Hi,asr 11;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
}\
    pcmBuffer[ChSbOffset+2*s32NumOfChannels]=(SINT16)s32Hi;\
}

#define SBC_WINDOW_CALC_8_0                                                     \
{                                                                               \
    __asm\
{\
    MUL s32Hi,WIND_8_SUBBANDS_0_1,(VX[ChOffset+SBC_SUB_BANDS_8*3]+VX[ChOffset+SBC_SUB_BANDS_8*19]);\
    MLA s32Hi,WIND_8_SUBBANDS_0_2,(VX[ChOffset+SBC_SUB_BANDS_8*4]-VX[ChOffset+SBC_SUB_BANDS_8*16]),s32Hi;\
    MLA s32Hi,WIND_8_SUBBANDS_0_3,(VX[ChOffset+SBC_SUB_BANDS_8*7]+VX[ChOffset+SBC_SUB_BANDS_8*15]),s32Hi;\
    MLA s32Hi,WIND_8_SUBBANDS_0_4,(VX[ChOffset+SBC_SUB_BANDS_8*8]-VX[ChOffset+SBC_SUB_BANDS_8*12]),s32Hi;\
    MLA s32Hi,WIND_8_SUBBANDS_0_5,VX[ChOffset+SBC_SUB_BANDS_8*11],s32Hi;\
    MVN s32Hi,s32Hi,asr 10;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
    }\
    pcmBuffer[ChSbOffset]=(SINT16)s32Hi;\
}
#define SBC_WINDOW_CALC_8_1_7                                                       \
{                                                                               \
    __asm\
    {\
    MUL s32Hi,WIND_8_SUBBANDS_1_0,VX[ChOffset+1];\
    MUL s32Hi2,WIND_8_SUBBANDS_1_0,VX[ChOffset+SBC_SUB_BANDS_8*19+7];\
    MLA s32Hi,WIND_8_SUBBANDS_1_1,VX[ChOffset+SBC_SUB_BANDS_8*3+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_1,VX[ChOffset+SBC_SUB_BANDS_8*16+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_2,VX[ChOffset+SBC_SUB_BANDS_8*4+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_2,VX[ChOffset+SBC_SUB_BANDS_8*15+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_3,VX[ChOffset+SBC_SUB_BANDS_8*7+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_3,VX[ChOffset+SBC_SUB_BANDS_8*12+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_4,VX[ChOffset+SBC_SUB_BANDS_8*8+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_4,VX[ChOffset+SBC_SUB_BANDS_8*11+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_5,VX[ChOffset+SBC_SUB_BANDS_8*11+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_5,VX[ChOffset+SBC_SUB_BANDS_8*8+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_6,VX[ChOffset+SBC_SUB_BANDS_8*12+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_6,VX[ChOffset+SBC_SUB_BANDS_8*7+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_7,VX[ChOffset+SBC_SUB_BANDS_8*15+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_7,VX[ChOffset+SBC_SUB_BANDS_8*4+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_8,VX[ChOffset+SBC_SUB_BANDS_8*16+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_8,VX[ChOffset+SBC_SUB_BANDS_8*3+7],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_1_9,VX[ChOffset+SBC_SUB_BANDS_8*19+1],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_1_9,VX[ChOffset+7],s32Hi2;\
    MVN s32Hi,s32Hi,asr 10;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
    MVN s32Hi2,s32Hi2,asr 10;\
    CMP s32Hi2,ThresholdP;\
    MOVPL s32Hi2,ThresholdP;\
    CMP s32Hi2,ThresholdN;\
    MOVMI s32Hi2,ThresholdN;\
    }\
    pcmBuffer[ChSbOffset+s32NumOfChannels]=(SINT16)s32Hi;\
    pcmBuffer[ChSbOffset+7*s32NumOfChannels]=(SINT16)s32Hi2;\
}
#define SBC_WINDOW_CALC_8_2_6                                                   \
{                                                                               \
    __asm\
    {\
    MUL s32Hi,WIND_8_SUBBANDS_2_0,VX[ChOffset+2];\
    MUL s32Hi2,WIND_8_SUBBANDS_2_0,VX[ChOffset+SBC_SUB_BANDS_8*19+6];\
    MLA s32Hi,WIND_8_SUBBANDS_2_1,VX[ChOffset+SBC_SUB_BANDS_8*3+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_1,VX[ChOffset+SBC_SUB_BANDS_8*16+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_2,VX[ChOffset+SBC_SUB_BANDS_8*4+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_2,VX[ChOffset+SBC_SUB_BANDS_8*15+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_3,VX[ChOffset+SBC_SUB_BANDS_8*7+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_3,VX[ChOffset+SBC_SUB_BANDS_8*12+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_4,VX[ChOffset+SBC_SUB_BANDS_8*8+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_4,VX[ChOffset+SBC_SUB_BANDS_8*11+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_5,VX[ChOffset+SBC_SUB_BANDS_8*11+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_5,VX[ChOffset+SBC_SUB_BANDS_8*8+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_6,VX[ChOffset+SBC_SUB_BANDS_8*12+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_6,VX[ChOffset+SBC_SUB_BANDS_8*7+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_7,VX[ChOffset+SBC_SUB_BANDS_8*15+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_7,VX[ChOffset+SBC_SUB_BANDS_8*4+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_8,VX[ChOffset+SBC_SUB_BANDS_8*16+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_8,VX[ChOffset+SBC_SUB_BANDS_8*3+6],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_2_9,VX[ChOffset+SBC_SUB_BANDS_8*19+2],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_2_9,VX[ChOffset+6],s32Hi2;\
    MVN s32Hi,s32Hi,asr 10;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
    MVN s32Hi2,s32Hi2,asr 10;\
    CMP s32Hi2,ThresholdP;\
    MOVPL s32Hi2,ThresholdP;\
    CMP s32Hi2,ThresholdN;\
    MOVMI s32Hi2,ThresholdN;\
    }\
    pcmBuffer[ChSbOffset+2*s32NumOfChannels]=(SINT16)s32Hi;\
    pcmBuffer[ChSbOffset+6*s32NumOfChannels]=(SINT16)s32Hi2;\
}
#define SBC_WINDOW_CALC_8_3_5                                                       \
{                                                                               \
    __asm\
    {\
    MUL s32Hi,WIND_8_SUBBANDS_3_0,VX[ChOffset+3];\
    MUL s32Hi2,WIND_8_SUBBANDS_3_0,VX[ChOffset+SBC_SUB_BANDS_8*19+5];\
    MLA s32Hi,WIND_8_SUBBANDS_3_1,VX[ChOffset+SBC_SUB_BANDS_8*3+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_1,VX[ChOffset+SBC_SUB_BANDS_8*16+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_2,VX[ChOffset+SBC_SUB_BANDS_8*4+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_2,VX[ChOffset+SBC_SUB_BANDS_8*15+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_3,VX[ChOffset+SBC_SUB_BANDS_8*7+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_3,VX[ChOffset+SBC_SUB_BANDS_8*12+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_4,VX[ChOffset+SBC_SUB_BANDS_8*8+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_4,VX[ChOffset+SBC_SUB_BANDS_8*11+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_5,VX[ChOffset+SBC_SUB_BANDS_8*11+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_5,VX[ChOffset+SBC_SUB_BANDS_8*8+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_6,VX[ChOffset+SBC_SUB_BANDS_8*12+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_6,VX[ChOffset+SBC_SUB_BANDS_8*7+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_7,VX[ChOffset+SBC_SUB_BANDS_8*15+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_7,VX[ChOffset+SBC_SUB_BANDS_8*4+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_8,VX[ChOffset+SBC_SUB_BANDS_8*16+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_8,VX[ChOffset+SBC_SUB_BANDS_8*3+5],s32Hi2;\
    MLA s32Hi,WIND_8_SUBBANDS_3_9,VX[ChOffset+SBC_SUB_BANDS_8*19+3],s32Hi;\
    MLA s32Hi2,WIND_8_SUBBANDS_3_9,VX[ChOffset+5],s32Hi2;\
    MVN s32Hi,s32Hi,asr 10;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
    MVN s32Hi2,s32Hi2,asr 10;\
    CMP s32Hi2,ThresholdP;\
    MOVPL s32Hi2,ThresholdP;\
    CMP s32Hi2,ThresholdN;\
    MOVMI s32Hi2,ThresholdN;\
    }\
    pcmBuffer[ChSbOffset+3*s32NumOfChannels]=(SINT16)s32Hi;\
    pcmBuffer[ChSbOffset+5*s32NumOfChannels]=(SINT16)s32Hi2;\
}


#define SBC_WINDOW_CALC_8_4                                                     \
{                                                                               \
    __asm\
    {\
    MUL s32Hi,WIND_8_SUBBANDS_4_0,(VX[ChOffset+4]+VX[ChOffset+SBC_SUB_BANDS_8*19+4]);\
    MLA s32Hi,WIND_8_SUBBANDS_4_1,(VX[ChOffset+SBC_SUB_BANDS_8*3+4]+VX[ChOffset+SBC_SUB_BANDS_8*16+4]),s32Hi;\
    MLA s32Hi,WIND_8_SUBBANDS_4_2,(VX[ChOffset+SBC_SUB_BANDS_8*4+4]+VX[ChOffset+SBC_SUB_BANDS_8*15+4]),s32Hi;\
    MLA s32Hi,WIND_8_SUBBANDS_4_3,(VX[ChOffset+SBC_SUB_BANDS_8*7+4]+VX[ChOffset+SBC_SUB_BANDS_8*12+4]),s32Hi;\
    MLA s32Hi,WIND_8_SUBBANDS_4_4,(VX[ChOffset+SBC_SUB_BANDS_8*8+4]+VX[ChOffset+SBC_SUB_BANDS_8*11+4]),s32Hi;\
    MVN s32Hi,s32Hi,asr 10;\
    CMP s32Hi,ThresholdP;\
    MOVPL s32Hi,ThresholdP;\
    CMP s32Hi,ThresholdN;\
    MOVMI s32Hi,ThresholdN;\
    }\
    pcmBuffer[ChSbOffset+4*s32NumOfChannels]=(SINT16)s32Hi;\
}

#else
#if (SBC_IPAQ_OPT==TRUE)
#define SBC_WINDOW_CALC_4_0                                                     \
{                                                                               \
    sSumValue = 0;                                                            \
    WINDOW_ACCU(WIND_4_SUBBANDS_0_1, (VX[ChOffset+SBC_SUB_BANDS_4*3]+VX[ChOffset+SBC_SUB_BANDS_4*19]), sSumValue);\
    /*printf("%ld %ld %ld\n",WIND_4_SUBBANDS_0_1, VX[ChOffset+SBC_SUB_BANDS_4*3], sSumValue);*/\
    WINDOW_ACCU(WIND_4_SUBBANDS_0_2, (VX[ChOffset+SBC_SUB_BANDS_4*4]-VX[ChOffset+SBC_SUB_BANDS_4*16]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_0_3, (VX[ChOffset+SBC_SUB_BANDS_4*7]+VX[ChOffset+SBC_SUB_BANDS_4*15]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_0_4, (VX[ChOffset+SBC_SUB_BANDS_4*8]-VX[ChOffset+SBC_SUB_BANDS_4*12]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_0_5, VX[ChOffset+SBC_SUB_BANDS_4*11], sSumValue);\
    sSumValue >>= 11;                                                        \
    sSumValue=-sSumValue;                  \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
}
#define SBC_WINDOW_CALC_4_1_3                                                   \
{                                                                               \
    sSumValue = 0;                                                            \
    sSumValue2=0;                                                             \
    \
    WINDOW_ACCU(WIND_4_SUBBANDS_1_0, VX[ChOffset+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_0, VX[ChOffset+SBC_SUB_BANDS_4*19+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_1, VX[ChOffset+SBC_SUB_BANDS_4*3+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_1, VX[ChOffset+SBC_SUB_BANDS_4*16+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_2, VX[ChOffset+SBC_SUB_BANDS_4*4+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_2, VX[ChOffset+SBC_SUB_BANDS_4*15+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_3, VX[ChOffset+SBC_SUB_BANDS_4*7+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_3, VX[ChOffset+SBC_SUB_BANDS_4*12+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_4, VX[ChOffset+SBC_SUB_BANDS_4*8+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_4, VX[ChOffset+SBC_SUB_BANDS_4*11+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_5, VX[ChOffset+SBC_SUB_BANDS_4*11+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_5, VX[ChOffset+SBC_SUB_BANDS_4*8+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_6, VX[ChOffset+SBC_SUB_BANDS_4*12+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_6, VX[ChOffset+SBC_SUB_BANDS_4*7+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_7, VX[ChOffset+SBC_SUB_BANDS_4*15+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_7, VX[ChOffset+SBC_SUB_BANDS_4*4+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_8, VX[ChOffset+SBC_SUB_BANDS_4*16+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_8, VX[ChOffset+SBC_SUB_BANDS_4*3+3], sSumValue2);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_9, VX[ChOffset+SBC_SUB_BANDS_4*19+1], sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_1_9, VX[ChOffset+3], sSumValue2);\
    sSumValue >>= 11;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+1*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+1*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset+1*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
    sSumValue2 >>= 11;                                                         \
    sSumValue2=-sSumValue2;                                                   \
    if (sSumValue2>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+3*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue2<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+3*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset+3*s32NumOfChannels] = (SINT16)sSumValue2; \
        }                                                                           \
    }                                                                               \
}
#define SBC_WINDOW_CALC_4_2                                              \
{                                                                               \
    sSumValue = 0;                                                            \
    \
    WINDOW_ACCU(WIND_4_SUBBANDS_2_0, (VX[ChOffset+2]+VX[ChOffset+SBC_SUB_BANDS_4*19+2]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_2_1, (VX[ChOffset+SBC_SUB_BANDS_4*3+2]+VX[ChOffset+SBC_SUB_BANDS_4*16+2]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_2_2, (VX[ChOffset+SBC_SUB_BANDS_4*4+2]+VX[ChOffset+SBC_SUB_BANDS_4*15+2]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_2_3, (VX[ChOffset+SBC_SUB_BANDS_4*7+2]+VX[ChOffset+SBC_SUB_BANDS_4*12+2]), sSumValue);\
    WINDOW_ACCU(WIND_4_SUBBANDS_2_4, (VX[ChOffset+SBC_SUB_BANDS_4*8+2]+VX[ChOffset+SBC_SUB_BANDS_4*11+2]), sSumValue);\
    sSumValue >>= 11;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+2*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+2*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset+2*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
}

#define SBC_WINDOW_CALC_8_0                                                     \
{                                                                               \
    sSumValue = 0;                                                            \
    WINDOW_ACCU(WIND_8_SUBBANDS_0_1, (VX[ChOffset+SBC_SUB_BANDS_8*3]+VX[ChOffset+SBC_SUB_BANDS_8*19]), sSumValue);/*+VX[ChOffset+SBC_SUB_BANDS_8*19]*/\
    /*printf("%ld %ld\n",VX[ChOffset+SBC_SUB_BANDS_8*3],sSumValue)*/;\
    WINDOW_ACCU(WIND_8_SUBBANDS_0_2, (VX[ChOffset+SBC_SUB_BANDS_8*4]-VX[ChOffset+SBC_SUB_BANDS_8*16]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_0_3, (VX[ChOffset+SBC_SUB_BANDS_8*7]+VX[ChOffset+SBC_SUB_BANDS_8*15]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_0_4, (VX[ChOffset+SBC_SUB_BANDS_8*8]-VX[ChOffset+SBC_SUB_BANDS_8*12]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_0_5, VX[ChOffset+SBC_SUB_BANDS_8*11], sSumValue);\
    sSumValue >>=10;                                                         \
    sSumValue=-sSumValue;                                                   \
    /*printf("%ld\n",sSumValue);*/\
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset] = ((SINT16)sSumValue); \
        }                                                                           \
    }                                                                                 \
}
#define SBC_WINDOW_CALC_8_1_7                                              \
{                                                                               \
    sSumValue = 0;                                                            \
    sSumValue2=0;                                                             \
    \
    WINDOW_ACCU(WIND_8_SUBBANDS_1_0, VX[ChOffset+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_0, VX[ChOffset+SBC_SUB_BANDS_8*19+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_1, VX[ChOffset+SBC_SUB_BANDS_8*3+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_1, VX[ChOffset+SBC_SUB_BANDS_8*16+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_2, VX[ChOffset+SBC_SUB_BANDS_8*4+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_2, VX[ChOffset+SBC_SUB_BANDS_8*15+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_3, VX[ChOffset+SBC_SUB_BANDS_8*7+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_3, VX[ChOffset+SBC_SUB_BANDS_8*12+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_4, VX[ChOffset+SBC_SUB_BANDS_8*8+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_4, VX[ChOffset+SBC_SUB_BANDS_8*11+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_5, VX[ChOffset+SBC_SUB_BANDS_8*11+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_5, VX[ChOffset+SBC_SUB_BANDS_8*8+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_6, VX[ChOffset+SBC_SUB_BANDS_8*12+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_6, VX[ChOffset+SBC_SUB_BANDS_8*7+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_7, VX[ChOffset+SBC_SUB_BANDS_8*15+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_7, VX[ChOffset+SBC_SUB_BANDS_8*4+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_8, VX[ChOffset+SBC_SUB_BANDS_8*16+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_8, VX[ChOffset+SBC_SUB_BANDS_8*3+7], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_9, VX[ChOffset+SBC_SUB_BANDS_8*19+1], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_1_9, VX[ChOffset+7], sSumValue2);\
    sSumValue >>= 10;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+1*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+1*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+1*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
    sSumValue2 >>= 10;                                                         \
    sSumValue2=-sSumValue2;                                                   \
    if (sSumValue2>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+7*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue2<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+7*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+7*s32NumOfChannels] = (SINT16)sSumValue2; \
        }                                                                           \
    }                                                                               \
}
#define SBC_WINDOW_CALC_8_2_6                                              \
{                                                                               \
    sSumValue = 0;                                                            \
    sSumValue2=0;                                                             \
    \
    WINDOW_ACCU(WIND_8_SUBBANDS_2_0, VX[ChOffset+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_0, VX[ChOffset+SBC_SUB_BANDS_8*19+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_1, VX[ChOffset+SBC_SUB_BANDS_8*3+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_1, VX[ChOffset+SBC_SUB_BANDS_8*16+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_2, VX[ChOffset+SBC_SUB_BANDS_8*4+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_2, VX[ChOffset+SBC_SUB_BANDS_8*15+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_3, VX[ChOffset+SBC_SUB_BANDS_8*7+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_3, VX[ChOffset+SBC_SUB_BANDS_8*12+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_4, VX[ChOffset+SBC_SUB_BANDS_8*8+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_4, VX[ChOffset+SBC_SUB_BANDS_8*11+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_5, VX[ChOffset+SBC_SUB_BANDS_8*11+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_5, VX[ChOffset+SBC_SUB_BANDS_8*8+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_6, VX[ChOffset+SBC_SUB_BANDS_8*12+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_6, VX[ChOffset+SBC_SUB_BANDS_8*7+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_7, VX[ChOffset+SBC_SUB_BANDS_8*15+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_7, VX[ChOffset+SBC_SUB_BANDS_8*4+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_8, VX[ChOffset+SBC_SUB_BANDS_8*16+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_8, VX[ChOffset+SBC_SUB_BANDS_8*3+6], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_9, VX[ChOffset+SBC_SUB_BANDS_8*19+2], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_2_9, VX[ChOffset+6], sSumValue2);\
    sSumValue >>= 10;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+2*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+2*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+2*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
    sSumValue2 >>= 10;                                                         \
    sSumValue2=-sSumValue2;                                                   \
    if (sSumValue2>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+6*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue2<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+6*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+6*s32NumOfChannels] = (SINT16)sSumValue2; \
        }                                                                           \
    }                                                                              \
}
#define SBC_WINDOW_CALC_8_3_5                                              \
{                                                                               \
    sSumValue = 0;                                                            \
    sSumValue2=0;                                                             \
    \
    WINDOW_ACCU(WIND_8_SUBBANDS_3_0, VX[ChOffset+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_0, VX[ChOffset+SBC_SUB_BANDS_8*19+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_1, VX[ChOffset+SBC_SUB_BANDS_8*3+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_1, VX[ChOffset+SBC_SUB_BANDS_8*16+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_2, VX[ChOffset+SBC_SUB_BANDS_8*4+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_2, VX[ChOffset+SBC_SUB_BANDS_8*15+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_3, VX[ChOffset+SBC_SUB_BANDS_8*7+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_3, VX[ChOffset+SBC_SUB_BANDS_8*12+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_4, VX[ChOffset+SBC_SUB_BANDS_8*8+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_4, VX[ChOffset+SBC_SUB_BANDS_8*11+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_5, VX[ChOffset+SBC_SUB_BANDS_8*11+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_5, VX[ChOffset+SBC_SUB_BANDS_8*8+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_6, VX[ChOffset+SBC_SUB_BANDS_8*12+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_6, VX[ChOffset+SBC_SUB_BANDS_8*7+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_7, VX[ChOffset+SBC_SUB_BANDS_8*15+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_7, VX[ChOffset+SBC_SUB_BANDS_8*4+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_8, VX[ChOffset+SBC_SUB_BANDS_8*16+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_8, VX[ChOffset+SBC_SUB_BANDS_8*3+5], sSumValue2);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_9, VX[ChOffset+SBC_SUB_BANDS_8*19+3], sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_3_9, VX[ChOffset+5], sSumValue2);\
    sSumValue >>= 10;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+3*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+3*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+3*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
    sSumValue2 >>= 10;                                                         \
    sSumValue2=-sSumValue2;                                                   \
    if (sSumValue2>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+5*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue2<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+5*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+5*s32NumOfChannels] = (SINT16)sSumValue2; \
        }                                                                           \
    }                                                                              \
}
#define SBC_WINDOW_CALC_8_4                                                     \
{                                                                               \
    sSumValue = 0;                                                            \
    WINDOW_ACCU(WIND_8_SUBBANDS_4_0, (VX[ChOffset+4]+VX[ChOffset+SBC_SUB_BANDS_8*19+4]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_4_1, (VX[ChOffset+SBC_SUB_BANDS_8*3+4]+VX[ChOffset+SBC_SUB_BANDS_8*16+4]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_4_2, (VX[ChOffset+SBC_SUB_BANDS_8*4+4]+VX[ChOffset+SBC_SUB_BANDS_8*15+4]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_4_3, (VX[ChOffset+SBC_SUB_BANDS_8*7+4]+VX[ChOffset+SBC_SUB_BANDS_8*12+4]), sSumValue);\
    WINDOW_ACCU(WIND_8_SUBBANDS_4_4, (VX[ChOffset+SBC_SUB_BANDS_8*8+4]+VX[ChOffset+SBC_SUB_BANDS_8*11+4]), sSumValue);\
    sSumValue >>=10;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+4*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+4*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset+4*s32NumOfChannels] = ((SINT16)sSumValue); \
        }                                                                           \
    }                                                                                 \
}
#else
#define SBC_WINDOW_CALC_4_1(jIndex)                                               \
{                                                                               \
    sSumValue = 0;                                                            \
    \
    WINDOW_ACCU(D[jIndex], VX[ChOffset+jIndex], s32Low);\
    sSumValue += s32Low;                                         \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*1], VX[ChOffset+SBC_SUB_BANDS_4*3+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*2], VX[ChOffset+SBC_SUB_BANDS_4*4+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*3], VX[ChOffset+SBC_SUB_BANDS_4*7+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*4], VX[ChOffset+SBC_SUB_BANDS_4*8+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*5], VX[ChOffset+SBC_SUB_BANDS_4*11+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*6], VX[ChOffset+SBC_SUB_BANDS_4*12+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*7], VX[ChOffset+SBC_SUB_BANDS_4*15+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*8], VX[ChOffset+SBC_SUB_BANDS_4*16+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_4*9], VX[ChOffset+SBC_SUB_BANDS_4*19+jIndex], s32Low);\
    sSumValue += s32Low; \
    sSumValue >>= 11;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+jIndex*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+jIndex*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset+jIndex*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
}
#define SBC_WINDOW_CALC_4_0                                                     \
{                                                                               \
    sSumValue = 0;                                                            \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*1], VX[ChOffset+SBC_SUB_BANDS_4*3], s32Low);\
    /*printf("%ld %ld %ld\n",D[SBC_SUB_BANDS_4*1], VX[ChOffset+SBC_SUB_BANDS_4*3], s32Low);*/\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*2], VX[ChOffset+SBC_SUB_BANDS_4*4], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*3], VX[ChOffset+SBC_SUB_BANDS_4*7], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*4], VX[ChOffset+SBC_SUB_BANDS_4*8], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*5], VX[ChOffset+SBC_SUB_BANDS_4*11], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*6], VX[ChOffset+SBC_SUB_BANDS_4*12], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*7], VX[ChOffset+SBC_SUB_BANDS_4*15], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*8], VX[ChOffset+SBC_SUB_BANDS_4*16], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_4*9], VX[ChOffset+SBC_SUB_BANDS_4*19], s32Low);\
    sSumValue += s32Low; \
    sSumValue >>= 11;                                                        \
    sSumValue=-sSumValue;                  \
    /*printf("%ld\n",sSumValue);*/\
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
}
#define SBC_WINDOW_CALC_8(iIndex,jIndex) \
{                                                                   \
    SBC_MULT_32_32_SIMPLIFIED(D[jIndex+SBC_SUB_BANDS_8*iIndex], U[jIndex+SBC_SUB_BANDS_8*iIndex], s32Low);\
    sSumValue += s32Low;                                         \
}
#define SBC_WINDOW_CALC_8_1(jIndex)                                               \
{                                                                               \
    sSumValue = 0;                                                            \
    \
    WINDOW_ACCU(D[jIndex], VX[ChOffset+jIndex], s32Low);\
    sSumValue += s32Low;                                         \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*1], VX[ChOffset+SBC_SUB_BANDS_8*3+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*2], VX[ChOffset+SBC_SUB_BANDS_8*4+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*3], VX[ChOffset+SBC_SUB_BANDS_8*7+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*4], VX[ChOffset+SBC_SUB_BANDS_8*8+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*5], VX[ChOffset+SBC_SUB_BANDS_8*11+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*6], VX[ChOffset+SBC_SUB_BANDS_8*12+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*7], VX[ChOffset+SBC_SUB_BANDS_8*15+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*8], VX[ChOffset+SBC_SUB_BANDS_8*16+jIndex], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[jIndex+SBC_SUB_BANDS_8*9], VX[ChOffset+SBC_SUB_BANDS_8*19+jIndex], s32Low);\
    sSumValue += s32Low; \
    sSumValue >>= 10;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset+jIndex*s32NumOfChannels] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset+jIndex*s32NumOfChannels] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                             \
        pcmBuffer[ChSbOffset+jIndex*s32NumOfChannels] = (SINT16)sSumValue; \
        }                                                                           \
    }                                                                               \
}
#define SBC_WINDOW_CALC_8_0                                                     \
{                                                                               \
    sSumValue = 0;                                                            \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*1], VX[ChOffset+SBC_SUB_BANDS_8*3], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*2], VX[ChOffset+SBC_SUB_BANDS_8*4], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*3], VX[ChOffset+SBC_SUB_BANDS_8*7], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*4], VX[ChOffset+SBC_SUB_BANDS_8*8], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*5], VX[ChOffset+SBC_SUB_BANDS_8*11], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*6], VX[ChOffset+SBC_SUB_BANDS_8*12], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*7], VX[ChOffset+SBC_SUB_BANDS_8*15], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*8], VX[ChOffset+SBC_SUB_BANDS_8*16], s32Low);\
    sSumValue += s32Low; \
    WINDOW_ACCU(D[SBC_SUB_BANDS_8*9], VX[ChOffset+SBC_SUB_BANDS_8*19], s32Low);\
    sSumValue += s32Low; \
    sSumValue >>=10;                                                         \
    sSumValue=-sSumValue;                                                   \
    if (sSumValue>SBC_MAX_SINT16)                                                      \
    {                                                                              \
    pcmBuffer[ChSbOffset] =SBC_MAX_SINT16;                \
    }                                                                           \
    else                                                                        \
    {                                                                             \
    if (sSumValue<SBC_MIN_SINT16)                                                      \
        {                                                                              \
        pcmBuffer[ChSbOffset] =SBC_MIN_SINT16;                \
        }                                                                           \
        else                                                                        \
        {                                                                            \
        pcmBuffer[ChSbOffset] = ((SINT16)sSumValue); \
        }                                                                           \
    }                                                                                 \
}
#endif
#endif

#else
#define SBC_WINDOW_CALC(iIndex,jIndex) \
{                                                                   \
    SBC_MULT_64(U[jIndex+strDecParams->numOfSubBands*iIndex] ,D[jIndex+strDecParams->numOfSubBands*iIndex] , s32Low, s32Hi);\
    s64Temp = (s32Hi<<2) | ((s32Low>>30)&3);                       \
    s64Temp *= (strDecParams->numOfSubBands * -1);                  \
    sSumValue += s64Temp;                                         \
}
#define SBC_WINDOW_CALC_1(jIndex)                                               \
{                                                                               \
    sSumValue = 0;                                                            \
    \
    for (i=0;i<10;i++)                                                          \
    {                                                                           \
    SBC_WINDOW_CALC(i,jIndex);                                              \
    }                                                                           \
    sSumValue >>= 15;                                                         \
    /*printf("sSumValue %08d \n", sSumValue);                               */\
    pcmBuffer[s16Blk*BLK+ch*CHL+jIndex] = (SINT16)((sSumValue+1)); \
}

#endif

#if (SBC_OPTIMIZATION == FALSE && SBC_ARM_OPT==FALSE)
#define SBC_BUILD_UVECT(iIndex, jIndex) \
{                                           \
    U[iIndex*(strDecParams->numOfSubBands*2)+jIndex] = strDecParams->VX[ChOffset+iIndex*(strDecParams->numOfSubBands*4)+jIndex];\
    U[iIndex*(strDecParams->numOfSubBands*2)+strDecParams->numOfSubBands+jIndex] =                        \
    strDecParams->VX[ChOffset+iIndex*(strDecParams->numOfSubBands*4)+(strDecParams->numOfSubBands*3)+jIndex];\
}
/*   printf("iIndex %d, jIndex %d, U1 0x%4X, U2 0x%4X pVectX 0x%4X VX 0x%4X ch %d\n",iIndex, jIndex, U[iIndex*(strDecParams->numOfSubBands*2)+jIndex], \
U[iIndex*(strDecParams->numOfSubBands*2)+strDecParams->numOfSubBands+jIndex], strDecParams->pVectX[ch], strDecParams->VX, ch);\*/
#define SBC_BUILD_UVECT_1(jIndex)                   \
{                                                   \
    SBC_BUILD_UVECT(0,jIndex);                      \
    SBC_BUILD_UVECT(1,jIndex);                      \
    SBC_BUILD_UVECT(2,jIndex);                      \
    SBC_BUILD_UVECT(3,jIndex);                      \
    SBC_BUILD_UVECT(4,jIndex);                      \
}
#endif


/****************************************************************************
* SbcAnalysisFilter - performs Synthesis on the encoded audio
*
* RETURNS : N/A
*/
#if (SBC_DEC_4_SUBD_INCLUDED == TRUE)
void SbcSynthesisFilter4sb(SBC_DEC_PARAMS *strDecParams)
{
    register SINT32 ch, s16Blk;
    SINT32 ChOffset, i, ChSbOffset, s32NumOfChannels = strDecParams->numOfChannels, s32NumOfBlocks = strDecParams->numOfBlocks;
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    register SINT16 *VX = strDecParams->VX + DecMaxShiftCounter;
#else
    register SINT32 *VX = strDecParams->VX + DecMaxShiftCounter;
#endif
    register SINT32 *pVX1, *pVX2;
    register SINT16 *pcmBuffer = strDecParams->pcmBuffer;
#if (SBC_ARM_OPT==TRUE)
    SINT16 *SbBuffer = strDecParams->sbBuffer;
#else
    SINT32 *SbBuffer = strDecParams->sbBuffer;
#endif

#if (SBC_C5402_OPT==TRUE)
    register SINT32 sSumValue;
    SINT32 s32Low;
    const SINT32 *D = coeffFor4SBs;
#else
#if (SBC_ARM_OPT==TRUE)
    SINT32 ThresholdP, ThresholdN;
    SINT32 s32Hi, s32Hi2;
#else

#if (SBC_IPAQ_OPT==TRUE)
#if (SBC_IS_64_BITS_IN_WINDOWING==TRUE)
    register SINT64 sSumValue, sSumValue2;
#else
    register SINT32 sSumValue, sSumValue2;
#endif
#else
#if (SBC_IS_64_BITS_IN_WINDOWING==TRUE)
#error SBC_IS_64_BITS_IN_WINDOWING=TRUE and SBC_IPAQ_OPT=FALSE is not a possible combination
    register SINT64 sSumValue;
#else
    register SINT32 sSumValue;
    SINT32 s32Temp, s32HiTemp, s32In1Temp, s32In2Temp, s32Carry;
#endif
#if (SBC_OPTIMIZATION==TRUE)
    register SINT32 s32Low;
    const SINT32 *D = coeffFor4SBs;
#else
    SINT16 j, k;
    SINT32 *U = (SINT32 *)strDecParams->scartchMemForFilter;
    SINT32 s32In1Temp, s32In2Temp, s32Hi, s32Low, s32TempVal1, s32TempVal2, s32Carry, s64Temp;
    const SINT32 *D = coeffFor4SBs;
#endif
#endif
#endif
#endif

    for (s16Blk = 0; s16Blk < s32NumOfBlocks; s16Blk++)
    {
        ChSbOffset = s16Blk * s32NumOfChannels * SBC_SUB_BANDS_4;
        ChOffset = 0;
        for (ch = 0; ch < s32NumOfChannels; ch++)
        {
            ChOffset = ch * (CHL * 20 + DecMaxShiftCounter);
            ChSbOffset += ch;
            strDecParams->ShiftCounter[ch] += SBC_SUB_BANDS_4 * 2;
            /*shifting*/
            if (strDecParams->ShiftCounter[ch] > DecMaxShiftCounter)
            {
                pVX1 = (SINT32 *)(VX + ChOffset - 2 + (SBC_SUB_BANDS_4 * 20));
                pVX2 = (SINT32 *)(VX + ChOffset - 2 + (SBC_SUB_BANDS_4 * 18) - DecMaxShiftCounter);
                for (i = 8; i >= 0; i--)
                {
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
#if (SBC_VECTOR_VX_ON_16BITS==FALSE)
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
#endif
                }
                strDecParams->ShiftCounter[ch] = 0;
            }

            ChOffset -= strDecParams->ShiftCounter[ch];

            /*memset(&(VX[ChOffset]), 0, (SBC_SUB_BANDS_4*2)*sizeof(SINT32));*/
#if (SBC_OPTIMIZATION == TRUE || SBC_ARM_OPT==TRUE)
            /*matrixing */
            /*printf("%ld %ld %ld %ld\n",*(SbBuffer),*(SbBuffer+1),*(SbBuffer+2),*(SbBuffer+3));*/
            /*fprintf(pfile, "SbBuffer %x, VX %x,  ChOffset %d sum : %x ShiftCounter %x;%x\r\n",SbBuffer,  VX, ChOffset, VX+ChOffset, strDecParams->ShiftCounter[0], strDecParams->ShiftCounter[1]);*/
            SBC_FastDCT4(SbBuffer, VX + ChOffset);

#if (SBC_ARM_OPT == TRUE)
            ThresholdP = (SINT32)0x7FFF;
            ThresholdN = -ThresholdP;
            SBC_WINDOW_CALC_4_0;
            SBC_WINDOW_CALC_4_1_3;
            SBC_WINDOW_CALC_4_2;
#else
            /*Window by coeffs*/
#if (SBC_IPAQ_OPT==TRUE)
            SBC_WINDOW_CALC_4_0
            SBC_WINDOW_CALC_4_1_3;
            SBC_WINDOW_CALC_4_2;
#else
            SBC_WINDOW_CALC_4_0
            SBC_WINDOW_CALC_4_1(1);
            SBC_WINDOW_CALC_4_1(2);
            SBC_WINDOW_CALC_4_1(3);
#endif
#endif
#else
            SBC_FastDCT4(SbBuffer, VX + ChOffset);
            /*Build vector U*/
            for (i = 0; i < 5; i++)
            {
                for (k = 0; k < SBC_SUB_BANDS_4; k++)
                {
                    U[i * (SBC_SUB_BANDS_4 * 2) + k] = VX[ChOffset + i * (SBC_SUB_BANDS_4 * 4) + k];
                    U[i * (SBC_SUB_BANDS_4 * 2) + SBC_SUB_BANDS_4 + k] =
                        VX[ChOffset + i * (SBC_SUB_BANDS_4 * 4) + (SBC_SUB_BANDS_4 * 3) + k];
                }
            }

            /*Window by coeffs*/
            /*calculate audio samples*/

            for (j = 0; j < SBC_SUB_BANDS_4; j++)
            {
                SBC_WINDOW_CALC_1(j)
            }
#endif
            SbBuffer += SBC_SUB_BANDS_4;
        }
    }
}
#endif

/****************************************************************************
* SbcAnalysisFilter - performs Synthesis on the encoded audio
*
* RETURNS : N/A
*/
#if (SBC_DEC_8_SUBD_INCLUDED == TRUE)
void SbcSynthesisFilter8sb(SBC_DEC_PARAMS *strDecParams)
{

    register SINT32 s16Blk;
    SINT32 s32NumOfBlocks = strDecParams->numOfBlocks;
    SINT32 ch, i;
    SINT32 ChOffset;
    SINT32 ChSbOffset, s32NumOfChannels = strDecParams->numOfChannels;
#if (SBC_VECTOR_VX_ON_16BITS==TRUE)
    register SINT16 *VX = strDecParams->VX + DecMaxShiftCounter;
#else
    register SINT32 *VX = strDecParams->VX + DecMaxShiftCounter;
#endif
    SINT32 *pVX1, *pVX2;
    SINT16 *pcmBuffer = strDecParams->pcmBuffer;
#if (SBC_ARM_OPT==TRUE)
    SINT16 *SbBuffer = strDecParams->sbBuffer;
#else
    SINT32 *SbBuffer = strDecParams->sbBuffer;
#endif

#if (SBC_C5402_OPT==TRUE)
    register SINT32 sSumValue;
    SINT32 s32Low;
    const SINT32 *D = coeffFor4SBs;
#else
#if (SBC_ARM_OPT==TRUE)
    SINT32 ThresholdP, ThresholdN;
    register SINT32 s32Hi, s32Hi2;;
#else

#if (SBC_IPAQ_OPT==TRUE)
#if (SBC_IS_64_BITS_IN_WINDOWING==TRUE)
    register SINT64 sSumValue, sSumValue2;
#else
    register SINT32 sSumValue, sSumValue2;
#endif
#else
#if (SBC_IS_64_BITS_IN_WINDOWING==TRUE)
    register SINT64 sSumValue;
#else
    register SINT32 sSumValue;
    SINT32 s32Temp, s32HiTemp, s32In1Temp, s32In2Temp, s32Carry;
#endif
#if (SBC_OPTIMIZATION==TRUE)
    SINT32 s32Low;
    const SINT32 *D = coeffFor8SBs;
#else
    SINT16 j;
    SINT32 *U = (SINT32 *)strDecParams->scartchMemForFilter;
    SINT32 s32In1Temp, s32In2Temp, s32Hi, s32Low, s32TempVal1, s32TempVal2, s32Carry, s64Temp;
    const SINT32 *D = coeffFor8SBs;
#endif
#endif
#endif
#endif

    for (s16Blk = 0; s16Blk < s32NumOfBlocks; s16Blk++)
    {
        ChSbOffset = s16Blk * s32NumOfChannels * SBC_SUB_BANDS_8;
        for (ch = 0; ch < s32NumOfChannels; ch++)
        {
            ChOffset = ch * (CHL * 20 + DecMaxShiftCounter);
            ChSbOffset += ch;
            strDecParams->ShiftCounter[ch] += SBC_SUB_BANDS_8 * 2;
            /*shifting*/
            if (strDecParams->ShiftCounter[ch] > DecMaxShiftCounter)
            {
                pVX1 = (SINT32 *)(VX + ChOffset - 2 + (SBC_SUB_BANDS_8 * 20));
                pVX2 = (SINT32 *)(VX + ChOffset - 2 + (SBC_SUB_BANDS_8 * 18) - DecMaxShiftCounter);
                //pVX1=VX+ChOffset+(SBC_SUB_BANDS_8*20)-1;
                //pVX2=pVX1-(SBC_SUB_BANDS_8*2)-DecMaxShiftCounter;
                for (i = 8; i >= 0; i--)
                {
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
#if (SBC_VECTOR_VX_ON_16BITS==FALSE)
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
                    *(pVX1--) = *(pVX2--);
#endif
                }
                strDecParams->ShiftCounter[ch] = 0;
            }
            ChOffset -= strDecParams->ShiftCounter[ch];
            /*matrixing  (VX) = (N) * (sbBuffer) */
            /*memset(&(VX[ChOffset]), 0, (SBC_SUB_BANDS_8*2)*sizeof(SINT32));*/
#if (SBC_OPTIMIZATION == TRUE || SBC_ARM_OPT==TRUE)
            /*fprintf(pfile, "SbBuffer %x, VX %x,  ChOffset %d sum : %x ShiftCounter %x;%x\r\n",SbBuffer,  VX, ChOffset, VX+ChOffset, strDecParams->ShiftCounter[0], strDecParams->ShiftCounter[1]);*/
            SBC_FastDCT8(SbBuffer, VX + ChOffset);
            /*Window by coeffs*/
            /*calculate audio samples*/
#if (SBC_ARM_OPT == TRUE)
            ThresholdP = (SINT32)0x7FFF;
            ThresholdN = -ThresholdP;
            SBC_WINDOW_CALC_8_0
            SBC_WINDOW_CALC_8_1_7
            SBC_WINDOW_CALC_8_2_6
            SBC_WINDOW_CALC_8_3_5
            SBC_WINDOW_CALC_8_4
#else
#if (SBC_IPAQ_OPT==TRUE)
            SBC_WINDOW_CALC_8_0
            SBC_WINDOW_CALC_8_1_7
            SBC_WINDOW_CALC_8_2_6
            SBC_WINDOW_CALC_8_3_5
            SBC_WINDOW_CALC_8_4
#else
            SBC_WINDOW_CALC_8_0
            for (i = 1; i < SBC_SUB_BANDS_8; i++)
            {
                SBC_WINDOW_CALC_8_1(i)
            }
#endif
#endif
#else
            SBC_FastDCT8(SbBuffer, &(VX[ChOffset]));
            /*Build vector U*/
            for (i = 0; i < 5; i++)
            {
                for (j = 0; j < SBC_SUB_BANDS_8; j++)
                {
                    U[i * (SBC_SUB_BANDS_8 * 2) + j] = VX[ChOffset + i * (SBC_SUB_BANDS_8 * 4) + j];
                    U[i * (SBC_SUB_BANDS_8 * 2) + SBC_SUB_BANDS_8 + j] =
                        VX[ChOffset + i * (SBC_SUB_BANDS_8 * 4) + (SBC_SUB_BANDS_8 * 3) + j];
                }
            }
            /*Window by coeffs*/
            /*calculate audio samples*/
            for (j = 0; j < SBC_SUB_BANDS_8; j++)
            {
                SBC_WINDOW_CALC_1(j)
            }
#endif
            SbBuffer += SBC_SUB_BANDS_8;
            /*printf("%d %d %d %d\n", pcmBuffer[ChSbOffset+ch],pcmBuffer[ChSbOffset+s32NumOfChannels+ch],pcmBuffer[ChSbOffset+2*s32NumOfChannels+ch],pcmBuffer[ChSbOffset+3*s32NumOfChannels+ch]);*/
        }
    }
}
#endif
/****************************************************************************
* SbcAnalysisFilter - performs Synthesis on the encoded audio
*
* RETURNS : N/A
*/
void SbcSynthesisFilter(SBC_DEC_PARAMS *strDecParams)
{
#if (SBC_DEC_STEREO_INCLUDED == TRUE)
    SINT32 sb, Temp, s16Blk, s32NumOfSubBands = strDecParams->numOfSubBands;
#if (SBC_ARM_OPT==TRUE)
    SINT16 *SbBuffer = strDecParams->sbBuffer, *s32SbPtrR, *s32SbPtrL;
#else
    SINT32 *SbBuffer = strDecParams->sbBuffer, *s32SbPtrR, *s32SbPtrL;
#endif
#endif

#if (SBC_DEC_STEREO_INCLUDED == TRUE)
    if (strDecParams->channelMode == SBC_JOINT_STEREO)
    {
        for (sb = 0; sb < s32NumOfSubBands; sb++)
        {
#if (SBC_ARM_OPT==TRUE)
            s32SbPtrL = (SINT16 *)(SbBuffer + sb);
            s32SbPtrR = (SINT16 *)(SbBuffer + s32NumOfSubBands + sb);
#else
            s32SbPtrL = (SINT32 *)(SbBuffer + sb);
            s32SbPtrR = (SINT32 *)(SbBuffer + s32NumOfSubBands + sb);
#endif
            if (strDecParams->join[sb] != 0)
            {
                for (s16Blk = strDecParams->numOfBlocks - 1; s16Blk >= 0 ; s16Blk--)
                {

                    Temp = *s32SbPtrL + *s32SbPtrR;
                    *s32SbPtrR = *s32SbPtrL - *s32SbPtrR;
                    *s32SbPtrL = Temp;
                    s32SbPtrL += s32NumOfSubBands << 1;
                    s32SbPtrR += s32NumOfSubBands << 1;
                }
            }
        }
    }
#endif

#if (SBC_DEC_4_SUBD_INCLUDED == TRUE) && (SBC_DEC_8_SUBD_INCLUDED == TRUE)
    if (strDecParams->numOfSubBands == SBC_SUB_BANDS_8)
#endif
#if (SBC_DEC_8_SUBD_INCLUDED == TRUE)
    {
        /*        printf("SBC_SUB_BANDS_8\n");*/

        SbcSynthesisFilter8sb(strDecParams);

        return;
    }
#endif

#if (SBC_DEC_4_SUBD_INCLUDED == TRUE)

    /*   printf("SBC_SUB_BANDS_4\n"); */
    SbcSynthesisFilter4sb(strDecParams);
#endif

}

