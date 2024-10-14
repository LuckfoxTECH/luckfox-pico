/*
********************************************************************
* THIS INFORMATION IS PROPRIETARY TO
* BROADCOM CORP.
*-------------------------------------------------------------------
*
*           Copyright (c) 2006 Broadcom Corp.
*                      ALL RIGHTS RESERVED
*
********************************************************************

 ********************************************************************
 *    File Name: multprecision.h
 *
 *    Abstract: simple pairing algorithms
 *
 *    Functions:
 *            --
 *
 *    $History:$
 *
 ********************************************************************
*/
#ifndef P_256_MULTPRECISION_H
#define P_256_MULTPRECISION_H

#include "p_256_types.h"

/* Type definitions */

#define DWORD_BITS      32
#define DWORD_BYTES     4
#define DWORD_BITS_SHIFT 5

#define KEY_LENGTH_DWORDS_P192 6
#define KEY_LENGTH_DWORDS_P256 8
/* Arithmetic Operations */


#ifdef __cplusplus
extern "C" {
#endif

int MP_CMP(DWORD *a, DWORD *b, UINT32 keyLength);
int MP_isZero(DWORD *a, UINT32 keyLength);
void MP_Init(DWORD *c, UINT32 keyLength);
void MP_Copy(DWORD *c, DWORD *a, UINT32 keyLength);
UINT32 MP_DWORDBits(DWORD a);
UINT32 MP_MostSignDWORDs(DWORD *a, UINT32 keyLength);
UINT32 MP_MostSignBits(DWORD *a, UINT32 keyLength);
void MP_InvMod(DWORD *aminus, DWORD *a, UINT32 keyLength);

DWORD MP_Add(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);           // c=a+b
void MP_AddMod(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);
DWORD MP_Sub(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);           // c=a-b
void MP_SubMod(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);
void MP_RShift(DWORD *c, DWORD *a, UINT32 keyLength);                   // c=a>>1, return carrier
void MP_LShiftMod(DWORD *c, DWORD *a, UINT32 keyLength);                // c=a<<b, return carrier
DWORD MP_LShift(DWORD *c, DWORD *a, UINT32 keyLength);                  // c=a<<b, return carrier
void MP_Mult(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);           // c=a*b
void MP_MersennsMultMod(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);
void MP_MersennsSquaMod(DWORD *c, DWORD *a, UINT32 keyLength);
DWORD MP_LShift(DWORD *c, DWORD *a, UINT32 keyLength);
void MP_Mult(DWORD *c, DWORD *a, DWORD *b, UINT32 keyLength);
void MP_FastMod(DWORD *c, DWORD *a);
void MP_FastMod_P256(DWORD *c, DWORD *a);

#ifdef __cplusplus
}
#endif



#endif /* P_256_MULTPRECISION_H */




