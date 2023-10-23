/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: snprintf_s  function
 * Author: lishunda
 * Create: 2014-02-25
 */

#include "securec.h"

#if SECUREC_ENABLE_SNPRINTF
/*
 * <FUNCTION DESCRIPTION>
 *    The snprintf_s function is equivalent to the snprintf function
 *    except for the parameter destMax/count and the explicit runtime-constraints violation
 *    The snprintf_s function formats and stores count or fewer characters in
 *    strDest and appends a terminating null. Each argument (if any) is converted
 *    and output according to the corresponding format specification in format.
 *    The formatting is consistent with the printf family of functions; If copying
 *    occurs between strings that overlap, the behavior is undefined.
 *
 * <INPUT PARAMETERS>
 *    strDest                 Storage location for the output.
 *    destMax                 The size of the storage location for output. Size
 *                                 in bytes for snprintf_s or size in words for snwprintf_s.
 *    count                    Maximum number of character to store.
 *    format                  Format-control string.
 *    ...                        Optional arguments.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                 is updated
 *
 * <RETURN VALUE>
 *    return  the number of characters written, not including the terminating null
 *    return -1 if an  error occurs.
 *    return -1 if count < destMax and the output string  has been truncated
 *
 * If there is a runtime-constraint violation, strDest[0] will be set to the '\0' when strDest and destMax valid
 *
 */
int snprintf_s(char *strDest, size_t destMax, size_t count, const char *format, ...)
{
    int ret;                    /* If initialization causes  e838 */
    va_list argList;

    va_start(argList, format);
    ret = vsnprintf_s(strDest, destMax, count, format, argList);
    va_end(argList);
    (void)argList;              /* To clear e438 last value assigned not used , the compiler will optimize this code */

    return ret;
}
#if SECUREC_IN_KERNEL
EXPORT_SYMBOL(snprintf_s);
#endif
#endif

#if SECUREC_SNPRINTF_TRUNCATED
/*
 * <FUNCTION DESCRIPTION>
 *    The snprintf_truncated_s function is equivalent to the snprintf function
 *    except for the parameter destMax/count and the explicit runtime-constraints violation
 *    The snprintf_truncated_s function formats and stores count or fewer characters in
 *    strDest and appends a terminating null. Each argument (if any) is converted
 *    and output according to the corresponding format specification in format.
 *    The formatting is consistent with the printf family of functions; If copying
 *    occurs between strings that overlap, the behavior is undefined.
 *
 * <INPUT PARAMETERS>
 *    strDest                 Storage location for the output.
 *    destMax                 The size of the storage location for output. Size
 *                                 in bytes for snprintf_truncated_s or size in words for snwprintf_s.
 *    format                  Format-control string.
 *    ...                        Optional arguments.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                 is updated
 *
 * <RETURN VALUE>
 *    return  the number of characters written, not including the terminating null
 *    return -1 if an  error occurs.
 *    return destMax-1 if output string  has been truncated
 *
 * If there is a runtime-constraint violation, strDest[0] will be set to the '\0' when strDest and destMax valid
 *
 */
int snprintf_truncated_s(char *strDest, size_t destMax, const char *format, ...)
{
    int ret;                    /* If initialization causes  e838 */
    va_list argList;

    va_start(argList, format);
    ret = vsnprintf_truncated_s(strDest, destMax, format, argList);
    va_end(argList);
    (void)argList;              /* To clear e438 last value assigned not used , the compiler will optimize this code */

    return ret;
}
#if SECUREC_IN_KERNEL
EXPORT_SYMBOL(snprintf_truncated_s);
#endif

#endif


