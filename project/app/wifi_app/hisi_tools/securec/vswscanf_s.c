/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: vswscanf_s  function
 * Author: lishunda
 * Create: 2014-02-25
 */

#define SECUREC_INLINE_INIT_FILE_STREAM_STR 1
#include "secinput.h"

SECUREC_INLINE size_t SecWcslen(const wchar_t *s)
{
    const wchar_t *end = s;
    while (*end != L'\0') {
        ++end;
    }
    return ((size_t)((end - s)));
}

/*
 * <FUNCTION DESCRIPTION>
 *    The  vswscanf_s  function  is  the  wide-character  equivalent  of the vsscanf_s function
 *    The vsscanf_s function reads data from buffer into the location given by
 *    each argument. Every argument must be a pointer to a variable with a type
 *    that corresponds to a type specifier in format.
 *    The format argument controls the interpretation of the input fields and
 *    has the same form and function as the format argument for the scanf function.
 *    If copying takes place between strings that overlap, the behavior is undefined.
 *
 * <INPUT PARAMETERS>
 *    buffer                Stored data
 *    format                Format control string, see Format Specifications.
 *    argList               pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    argList               the converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Each of these functions returns the number of fields successfully converted
 *    and assigned; the return value does not include fields that were read but
 *    not assigned. A return value of 0 indicates that no fields were assigned.
 *    return -1 if an error occurs.
 */
int vswscanf_s(const wchar_t *buffer, const wchar_t *format, va_list argList)
{
    size_t count; /* If initialization causes  e838 */
    SecFileStream fStr;
    int retVal;

    /* Validation section */
    if (buffer == NULL || format == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("vswscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    count = SecWcslen(buffer);
    if (count == 0 || count > SECUREC_WCHAR_STRING_MAX_LEN) {
        SecClearDestBufW(buffer, format, argList);
        SECUREC_ERROR_INVALID_PARAMTER("vswscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    SecInitFileStreamFromString(&fStr, (const char *)buffer, (int)count * ((int)sizeof(wchar_t)));
    retVal = SecInputSW(&fStr, format, argList);
    if (retVal < 0) {
        SECUREC_ERROR_INVALID_PARAMTER("vswscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    return retVal;
}


