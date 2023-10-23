/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: By defining data type for UNICODE string and including "input.inl",
 *             this file generates real underlying function used by scanf family API.
 * Author: lishunda
 * Create: 2014-02-25
 */

/* If some platforms don't have wchar.h, dont't include it */
#if !(defined(SECUREC_VXWORKS_PLATFORM))
/* If there is no macro below, it will cause vs2010 compiling alarm */
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#ifndef __STDC_WANT_SECURE_LIB__
/* The order of adjustment is to eliminate alarm of Duplicate Block */
#define __STDC_WANT_SECURE_LIB__ 0
#endif
#ifndef _CRTIMP_ALTERNATIVE
#define _CRTIMP_ALTERNATIVE     /* Comment microsoft *_s function */
#endif
#endif
#include <wchar.h>
#endif

/* Disable wchar func to clear vs warning */
#define SECUREC_ENABLE_WCHAR_FUNC       0
#define SECUREC_FORMAT_OUTPUT_INPUT     1

#ifndef SECUREC_FOR_WCHAR
#define SECUREC_FOR_WCHAR
#endif

#include "secinput.h"

#include "input.inl"

SECUREC_INLINE int SecIsDigit(SecInt ch)
{
    /* Convert int to unsigned int clear 571 */
    return (!((unsigned int)(int)(ch) & 0xff00) && isdigit(((unsigned int)(int)(ch) & 0x00ff)));
}
SECUREC_INLINE int SecIsXdigit(SecInt ch)
{
    return (!((unsigned int)(int)(ch) & 0xff00) && isxdigit(((unsigned int)(int)(ch) & 0x00ff)));
}
SECUREC_INLINE int SecIsSpace(SecInt ch)
{
    return iswspace((wint_t)(int)(ch));
}



