/*******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO BROADCOM CORP
*
* ------------------------------------------------------------------------------
*
* Copyright (c) 2003 Broadcom Corp.
*
*          ALL RIGHTS RESERVED
*
********************************************************************************
*
* File Name: bt_rtos.h
*
* Abstract:  This file provides bt_rtos thread related definition.
*
*
* Functions:
*
*******************************************************************************/
#ifndef _BT_RTOS_H_
#define _BT_RTOS_H_

#include <types.h>


// need it for memcpy family of functions.
#include <string.h>





#define BT_MEMCPY(a,b,c)  memcpy( (a), (b), (c) )

#define BT_MEMSET(a,b,c)  memset( (a), (b), (c) )

#define BT_STRLEN(a)      strlen( (a) )

#define BT_MEMCMP(a,b,c)  memcmp( (a), (b), (c) )

#define BT_MEMCMP_OK    0  // means memcmp succeed



/*
*******************************************************************************
*
* $HISTORY:$
*
*******************************************************************************
*/
#endif // end of #ifndef _BT_RTOS_H_
