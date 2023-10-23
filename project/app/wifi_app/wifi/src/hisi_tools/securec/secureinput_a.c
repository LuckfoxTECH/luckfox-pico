/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: By defining data type for ANSI string and including "input.inl",
 *              this file generates real underlying function used by scanf family API.
 * Author: lishunda
 * Create: 2014-02-25
 */
#define SECUREC_FORMAT_OUTPUT_INPUT 1
#ifdef SECUREC_FOR_WCHAR
#undef SECUREC_FOR_WCHAR
#endif

#include "secinput.h"

#include "input.inl"

SECUREC_INLINE int SecIsDigit(SecInt ch)
{
    /* SecInt to unsigned char clear  571 */
    return isdigit((unsigned char)(ch) & 0x00ff);
}
SECUREC_INLINE int SecIsXdigit(SecInt ch)
{
    return isxdigit((unsigned char)(ch) & 0x00ff);
}
SECUREC_INLINE int SecIsSpace(SecInt ch)
{
    return isspace((unsigned char)(ch) & 0x00ff);
}

