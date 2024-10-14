#ifndef WC_ASSERT_H
#define WC_ASSERT_H
/*******************************************************************************
**
**    File Name:   wcassert.h
**
**    Purpose:     Defines the macro WC_ASSERT(), which is used during
**                 development / debugging to do precondition and postcondition
**                 checks for routines.
**
**                 The macros are enabled when _DEBUG is defined.
**    Note:
**        None.
**
**    Revision History:
**        10/23/99    Stephen Lee       Create
**
**    Copyright (c) 1999-2004 WIDCOMM Inc., All Rights Reserved.
**
*******************************************************************************/


#ifdef _DEBUG

#include "bt_target.h"



/* debug settings*/
#ifndef WC_DEBUG_LEVEL
#define WC_DEBUG_LEVEL 0
#endif

#if WC_DEBUG_LEVEL == 0

#include "stdio.h"  /* for printf()*/

#ifdef __cplusplus
extern "C" wc_assert(char *message, char *file, UINT32 line);
#else
void wc_assert(char *message, char *file, UINT32 line);
#endif

#define WC_ASSERT(_x) if ( !(_x) ) wc_assert("ASSERT at %s line %d\n", __FILE__, __LINE__);
#define WC_ASSERT_ALWAYS() wc_assert("ASSERT! at %s line %d\n", __FILE__, __LINE__);

#elif WC_DEBUG_LEVEL == 1

#include "assert.h"

#define WC_ASSERT(_x)        assert(_x);
#define WC_ASSERT_ALWAYS()   assert(0);
#endif  /* WC_DEBUG_LEVEL*/

#else /* _DEBUG*/

#ifndef WC_ASSERT
#define WC_ASSERT(_x)         ;
#endif

#ifndef WC_ASSERT_ALWAYS
#define WC_ASSERT_ALWAYS()    ;
#endif

#endif /* _DEBUG*/
#endif /* WC_ASSERT_H*/
