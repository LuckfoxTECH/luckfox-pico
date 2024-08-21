/*
*  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _RKDEF_H
#define _RKDEF_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef     signed   int                    sint;
typedef     unsigned int                    uint;
typedef     signed char                     int8;
typedef     unsigned char                   INT8U;
typedef     unsigned char                   uchar;
typedef     signed short                    int16;
typedef     signed long                     int32;
typedef     signed long long                int64;

typedef     unsigned char                   uint8;
typedef     unsigned short                  uint16;
typedef     unsigned long                   uint32;
typedef     unsigned long long              uint64;

typedef     volatile signed char            vint8;
typedef     volatile signed short           vint16;
typedef     volatile signed long            vint32;

typedef     volatile unsigned char          vuint8;
typedef     volatile unsigned short         vuint16;
typedef     volatile unsigned long          vuint32;

typedef     signed const char               intc8;              /* Read Only */
typedef     signed const short              intc16;             /* Read Only */
typedef     signed const long               intc32;             /* Read Only */

typedef     unsigned const char             uintc8;             /* Read Only */
typedef     unsigned const short            uintc16;            /* Read Only */
typedef     unsigned const long             uintc32;            /* Read Only */

typedef     signed   int                    INT;
typedef     unsigned int                    UINT;

typedef     signed char                     INT8;
typedef     signed short                    INT16;
typedef     signed long                     INT32;

typedef     unsigned char                   UINT8;
typedef     unsigned short                  UINT16;
typedef     unsigned long                   UINT32;
typedef     unsigned long long              UINT64;

typedef     volatile signed char            VINT8;
typedef     volatile signed short           VINT16;
typedef     volatile signed long            VINT32;

typedef     volatile unsigned char          VUINT8;
typedef     volatile unsigned short         VUINT16;
typedef     volatile unsigned long          VUINT32;

typedef     signed const char               INTC8;              /* Read Only */
typedef     signed const short              INTC16;             /* Read Only */
typedef     signed const long               INTC32;             /* Read Only */

typedef     unsigned const char             UINTC8;             /* Read Only */
typedef     unsigned const short            UINTC16;            /* Read Only */
typedef     unsigned const long             UINTC32;            /* Read Only */

typedef     void (*pFunc)(void);


#endif