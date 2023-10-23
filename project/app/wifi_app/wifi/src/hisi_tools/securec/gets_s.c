/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: gets_s function
 * Author: lishunda
 * Create: 2014-02-25
 */

#include "securecutil.h"

SECUREC_INLINE void SecTrimCRLF(char *buffer, size_t len)
{
    int i;
    /* No need to determine whether integer overflow exists */
    for (i = (int)(len - 1); i >= 0 && (buffer[i] == '\r' || buffer[i] == '\n'); --i) {
        buffer[i] = '\0';
    }
}

/*
 * <FUNCTION DESCRIPTION>
 *    The gets_s function reads at most one less than the number of characters
 *    specified by destMax from the std input stream, into the array pointed to by buffer
 *    The line consists of all characters up to and including
 *    the first newline character ('\n'). gets_s then replaces the newline
 *    character with a null character ('\0') before returning the line.
 *    If the first character read is the end-of-file character, a null character
 *    is stored at the beginning of buffer and NULL is returned.
 *
 * <INPUT PARAMETERS>
 *    buffer                         Storage location for input string.
 *    numberOfElements       The size of the buffer.
 *
 * <OUTPUT PARAMETERS>
 *    buffer                         is updated
 *
 * <RETURN VALUE>
 *    buffer                         Successful operation
 *    NULL                           Improper parameter or read fail
 */
char *gets_s(char *buffer, size_t numberOfElements)
{
    size_t len;
#ifdef SECUREC_COMPATIBLE_WIN_FORMAT
    size_t bufferSize = ((numberOfElements == (size_t)-1) ? SECUREC_STRING_MAX_LEN : numberOfElements);
#else
    size_t bufferSize = numberOfElements;
#endif

    if (buffer == NULL || bufferSize == 0 || bufferSize > SECUREC_STRING_MAX_LEN) {
        SECUREC_ERROR_INVALID_PARAMTER("gets_s");
        return NULL;
    }

    if (fgets(buffer, (int)bufferSize, SECUREC_STREAM_STDIN) == NULL) {
        return NULL;
    }

    len = strlen(buffer);
    if (len > 0 && len < bufferSize) {
        SecTrimCRLF(buffer, len);
    }

    return buffer;
}

