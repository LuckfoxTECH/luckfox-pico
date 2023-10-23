/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: vscanf_s  function
 * Author: lishunda
 * Create: 2014-02-25
 */

#define SECUREC_INLINE_INIT_FILE_STREAM_STDIN 1
#include "secinput.h"

/*
 * <FUNCTION DESCRIPTION>
 *     The vscanf_s function is equivalent to scanf_s, with the variable argument list replaced by argList,
 *     The vscanf_s function reads data from the standard input stream stdin and
 *    writes the data into the location that's given by argument. Each argument
 *    must be a pointer to a variable of a type that corresponds to a type specifier
 *    in format. If copying occurs between strings that overlap, the behavior is
 *    undefined.
 *
 * <INPUT PARAMETERS>
 *    format                Format control string.
 *    argList               pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    argList               the converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Returns the number of fields successfully converted and assigned;
 *    the return value does not include fields that were read but not assigned.
 *    A return value of 0 indicates that no fields were assigned.
 *    return -1 if an error occurs.
 */
int vscanf_s(const char *format, va_list argList)
{
    int retVal;                 /* If initialization causes  e838 */
    SecFileStream fStr;
    SecInitFileStreamFromStdin(&fStr);
    /*
     * The "va_list" has different definition on different platform, so we can't use argList == NULL
     * To determine it's invalid. If you has fixed platform, you can check some fields to validate it,
     * such as "argList == NULL" or argList.xxx != NULL or *(size_t *)&argList != 0.
     */
    if (format == NULL || fStr.pf == NULL) {
        SECUREC_ERROR_INVALID_PARAMTER("vscanf_s");
        return SECUREC_SCANF_EINVAL;
    }

    SECUREC_LOCK_STDIN(0, fStr.pf);

    retVal = SecInputS(&fStr, format, argList);

    SECUREC_UNLOCK_STDIN(0, fStr.pf);
    if (retVal < 0) {
        SECUREC_ERROR_INVALID_PARAMTER("vscanf_s");
        return SECUREC_SCANF_EINVAL;
    }
    return retVal;
}


