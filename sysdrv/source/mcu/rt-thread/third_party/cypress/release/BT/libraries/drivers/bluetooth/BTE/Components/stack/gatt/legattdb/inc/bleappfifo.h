#ifndef _BLEAPPFIFO_H_
#define _BLEAPPFIFO_H_
/*******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO BROADCOM CORP
*
* ------------------------------------------------------------------------------
*
* Copyright (c) 2011 Broadcom Corp.
*
*          ALL RIGHTS RESERVED
*
********************************************************************************
*
* File Name: bleappfifo.h
*
* Abstract: This file provides a simple fifo. It does not grow dynamically.
*
* Functions:
*
*******************************************************************************/


#include "types.h" // basic types.

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <pshpack1.h>
#endif
// GHS syntax.
#pragma pack(1)
typedef unsigned char            BOOL8;
typedef PACKED struct
{
    UINT8 size;    // total entry of the fifo.
    UINT8 headIdx; // this is the head.
    UINT8 tailIdx; // this is the tail.
    BOOL8 full;    // This is flag to indicate fifo full.
} BLEAPPFIFO_HDR;



//////////////////////////////////////////////////////////////////////////////
//                      public interface
// This will initialize a piece of memory that is provided by called.
BLEAPPFIFO_HDR *bleappfifo_init(UINT8 *buffer, int size);


void *bleappfifo_pop(BLEAPPFIFO_HDR *ptr);

int bleappfifo_append(BLEAPPFIFO_HDR *ptr, void *);


#ifdef _WIN32
#include <poppack.h>
#endif
// GHS syntax.
#pragma pack()

#ifdef __cplusplus
}
#endif
#endif // end of #ifndef _BLEAPPFIFO_H_
