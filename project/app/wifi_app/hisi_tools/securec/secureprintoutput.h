/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: Define macro, enum, data struct, and declare internal used function
 *              prototype, which is used by output.inl, secureprintoutput_w.c and
 *              secureprintoutput_a.c.
 * Author: lishunda
 * Create: 2014-02-25
 */

#ifndef SECUREPRINTOUTPUT_H_E950DA2C_902F_4B15_BECD_948E99090D9C
#define SECUREPRINTOUTPUT_H_E950DA2C_902F_4B15_BECD_948E99090D9C
#include "securecutil.h"

/*
 * Flag definitions.
 * Using macros instead of enumerations is because some of the enumerated types under the compiler are 16bit.
 */
#define SECUREC_FLAG_SIGN           0x00001U
#define SECUREC_FLAG_SIGN_SPACE     0x00002U
#define SECUREC_FLAG_LEFT           0x00004U
#define SECUREC_FLAG_LEADZERO       0x00008U
#define SECUREC_FLAG_LONG           0x00010U
#define SECUREC_FLAG_SHORT          0x00020U
#define SECUREC_FLAG_SIGNED         0x00040U
#define SECUREC_FLAG_ALTERNATE      0x00080U
#define SECUREC_FLAG_NEGATIVE       0x00100U
#define SECUREC_FLAG_FORCE_OCTAL    0x00200U
#define SECUREC_FLAG_LONG_DOUBLE    0x00400U
#define SECUREC_FLAG_WIDECHAR       0x00800U
#define SECUREC_FLAG_LONGLONG       0x01000U
#define SECUREC_FLAG_CHAR           0x02000U
#define SECUREC_FLAG_POINTER        0x04000U
#define SECUREC_FLAG_I64            0x08000U
#define SECUREC_FLAG_PTRDIFF        0x10000U
#define SECUREC_FLAG_SIZE           0x20000U
#ifdef  SECUREC_COMPATIBLE_LINUX_FORMAT
#define SECUREC_FLAG_INTMAX         0x40000U
#endif

/* State definitions. Identify the status of the current format */
typedef enum {
    STAT_NORMAL,
    STAT_PERCENT,
    STAT_FLAG,
    STAT_WIDTH,
    STAT_DOT,
    STAT_PRECIS,
    STAT_SIZE,
    STAT_TYPE,
    STAT_INVALID
} SecFmtState;

/* Format output buffer pointer and available size */
typedef struct {
    int count;
    char *cur;
} SecPrintfStream;


#ifndef SECUREC_BUFFER_SIZE
#if SECUREC_IN_KERNEL
#define SECUREC_BUFFER_SIZE    32
#elif defined(SECUREC_STACK_SIZE_LESS_THAN_1K)
/*
 * SECUREC BUFFER SIZE Can not be less than 23
 * The length of the octal representation of 64-bit integers with zero lead
 */
#define SECUREC_BUFFER_SIZE    256
#else
#define SECUREC_BUFFER_SIZE    512
#endif
#endif
#if SECUREC_BUFFER_SIZE < 23
#error SECUREC_BUFFER_SIZE Can not be less than 23
#endif
/* Buffer size for wchar, use 4 to make the compiler aligns as 8 bytes as possible */
#define SECUREC_WCHAR_BUFFER_SIZE 4


#define SECUREC_MAX_PRECISION  SECUREC_BUFFER_SIZE
/* Max. # bytes in multibyte char  ,see MB_LEN_MAX */
#define SECUREC_MB_LEN 16
/* The return value of the internal function, which is returned when truncated */
#define SECUREC_PRINTF_TRUNCATE (-2)

#define SECUREC_VSPRINTF_PARAM_ERROR(format, strDest, destMax, maxLimit) \
    ((format) == NULL || (strDest) == NULL || (destMax) == 0 || (destMax) > (maxLimit))

#define SECUREC_VSPRINTF_CLEAR_DEST(strDest, destMax, maxLimit) do { \
    if ((strDest) != NULL && (destMax) > 0 && (destMax) <= (maxLimit)) { \
        *(strDest) = '\0'; \
    } \
} SECUREC_WHILE_ZERO

#ifdef SECUREC_COMPATIBLE_WIN_FORMAT
#define SECUREC_VSNPRINTF_PARAM_ERROR(format, strDest, destMax, count, maxLimit) \
    (((format) == NULL || (strDest) == NULL || (destMax) == 0 || (destMax) > (maxLimit)) || \
    ((count) > (SECUREC_STRING_MAX_LEN - 1) && (count) != (size_t)(-1)))

#else
#define SECUREC_VSNPRINTF_PARAM_ERROR(format, strDest, destMax, count, maxLimit) \
    (((format) == NULL || (strDest) == NULL || (destMax) == 0 || (destMax) > (maxLimit)) || \
    ((count) > (SECUREC_STRING_MAX_LEN - 1)))
#endif


#ifdef __cplusplus
extern "C" {
#endif
    extern int SecVsnprintfImpl(char *string, size_t count, const char *format, va_list argList);
#if SECUREC_IN_KERNEL == 0
    extern int SecVswprintfImpl(wchar_t *string, size_t sizeInWchar, const wchar_t *format, va_list argList);
#endif
#ifdef __cplusplus
}
#endif

#endif


