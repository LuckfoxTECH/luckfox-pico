/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: By defining corresponding macro for ANSI string and including "output.inl",
 *              this file generates real underlying function used by printf family API.
 * Author: Hisilicon
 * Create: 2014-02-25
 */

#define SECUREC_FORMAT_OUTPUT_INPUT 1

#ifdef SECUREC_FOR_WCHAR
#undef SECUREC_FOR_WCHAR
#endif

#include "secureprintoutput.h"
#if SECUREC_WARP_OUTPUT
#define SECUREC_FORMAT_FLAG_TABLE_SIZE 128
static const unsigned char g_flagTable[SECUREC_FORMAT_FLAG_TABLE_SIZE] = {
    /*
     * Known flag is  "0123456789 +-#hlLwZzjqt*I"
     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

SECUREC_INLINE const char *SecSkipKnownFlags(const char *format)
{
    const char *fmt = format;
    while (*fmt != '\0') {
        char fmtChar = *fmt;
        if ((unsigned char)fmtChar > 0x7f) { /* 0x7f is upper limit of format char value */
            break;
        }
        if (g_flagTable[(unsigned char)fmtChar] == 0) {
            break;
        }
        ++fmt;
    }
    return fmt;
}

SECUREC_INLINE int SecFormatContainN(const char *format)
{
    const char *fmt = format;
    while (*fmt != '\0') {
        ++fmt;
        /* Skip normal char */
        if (*(fmt - 1) != '%') {
            continue;
        }
        /* Meet %% */
        if (*fmt == '%') {
            ++fmt; /* Point to  the character after the %. Correct handling %%xx */
            continue;
        }
        /* Now parse %..., fmt point to the character after the % */
        fmt = SecSkipKnownFlags(fmt);
        if (*fmt == 'n') {
            return 1;
        }
    }
    return 0;
}
/*
 * Multi character formatted output implementation, the count include \0 character, must be greater than zero
 */
int SecVsnprintfImpl(char *string, size_t count, const char *format, va_list argList)
{
    int retVal;
    if (SecFormatContainN(format)) {
        string[0] = '\0';
        return -1;
    }
    retVal = vsnprintf(string, count, format, argList);
    if (retVal >= (int)count) { /* The size_t to int is ok, count max is SECUREC_STRING_MAX_LEN */
        /* The buffer was too small; we return truncation */
        string[count - 1] = '\0';
        return SECUREC_PRINTF_TRUNCATE;
    } else if (retVal < 0) {
        string[0] = '\0'; /* Empty the dest strDest */
        return -1;
    }
    return retVal;
}
#else
#if SECUREC_IN_KERNEL
#include <linux/ctype.h>
#endif

#define SECUREC_CHAR(x) x
#define SECUREC_WRITE_MULTI_CHAR  SecWriteMultiChar
#define SECUREC_WRITE_STRING      SecWriteString

#ifndef EOF
#define EOF (-1)
#endif

SECUREC_INLINE void SecWriteMultiChar(char ch, int num, SecPrintfStream *f, int *pnumwritten);
SECUREC_INLINE void SecWriteString(const char *string, int len, SecPrintfStream *f, int *pnumwritten);

#ifndef HAVE_PCLINT_CHECK
#include "output.inl"
#endif

/*
 * Multi character formatted output implementation
 */
int SecVsnprintfImpl(char *string, size_t count, const char *format, va_list argList)
{
    SecPrintfStream str;
    int retVal;

    str.count = (int)count; /* The count include \0 character, must be greater than zero */
    str.cur = string;

    retVal = SecOutputS(&str, format, argList);
    if (retVal >= 0 && SecPutZeroChar(&str) == 0) {
        return retVal;
    } else if (str.count < 0) {
        /* The buffer was too small; we return truncation */
        string[count - 1] = '\0';
        return SECUREC_PRINTF_TRUNCATE;
    }
    string[0] = '\0'; /* Empty the dest strDest */
    return -1;
}

/*
 * Write a wide character
 */
SECUREC_INLINE void SecWriteMultiChar(char ch, int num, SecPrintfStream *f, int *pnumwritten)
{
    int count = num;
    while (count-- > 0 && --(f->count) >= 0) {
        *(f->cur) = ch;
        ++(f->cur);
        *pnumwritten = *pnumwritten + 1;
    }
    if (f->count < 0) {
        *pnumwritten = -1;
    }
}

/*
 * Write string function, where this function is called, make sure that len is greater than 0
 */
SECUREC_INLINE void SecWriteString(const char *string, int len, SecPrintfStream *f, int *pnumwritten)
{
    const char *str = string;
    int count = len;
    while (count-- > 0 && --(f->count) >= 0) {
        *(f->cur) = *str;
        ++(f->cur);
        ++str;
    }
    *pnumwritten = *pnumwritten + (int)(size_t)(str - string);
    if (f->count < 0) {
        *pnumwritten = -1;
    }
}
#endif

