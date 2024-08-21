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
 *    File Name: ecc_pp.h
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

#ifndef P_256_ECC_PP_H
#define P_256_ECC_PP_H

#include "p_256_multprecision.h"

//#define width 4

struct _point
{
    DWORD x[KEY_LENGTH_DWORDS_P256];
    DWORD y[KEY_LENGTH_DWORDS_P256];
    DWORD z[KEY_LENGTH_DWORDS_P256];
};
typedef struct _point Point;

typedef struct
{
    // curve's coefficients
    DWORD a[KEY_LENGTH_DWORDS_P256];
    DWORD b[KEY_LENGTH_DWORDS_P256];

    //whether a is -3
    int a_minus3;

    // prime modulus
    DWORD p[KEY_LENGTH_DWORDS_P256];

    // Omega, p = 2^m -omega
    DWORD omega[KEY_LENGTH_DWORDS_P256];

    // base point, a point on E of order r
    Point G;

} EC;

#ifdef __cplusplus
extern "C" {
#endif

extern EC curve;
extern EC curve_p256;

void ECC_PM_B_NAF(Point *q, Point *p, DWORD *n, UINT32 keyLength);

#define ECC_PM(q, p, n, keyLength)  ECC_PM_B_NAF(q, p, n, keyLength)

void p_256_init_curve(UINT32 keyLength);
void InitPoint(Point *q);

#ifdef __cplusplus
}
#endif

#endif /* P_256_ECC_PP_H */
