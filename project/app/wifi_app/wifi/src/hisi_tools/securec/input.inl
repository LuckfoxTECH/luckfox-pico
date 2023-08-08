/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: Used by secureinput_a.c and secureinput_w.c to include.
 *              This file provides a template function for ANSI and UNICODE compiling by
 *              different type definition. The functions of SecInputS or
 *              SecInputSW provides internal implementation for scanf family API, such as sscanf_s, fscanf_s.
 * Author: lishunda
 * Create: 2014-02-25
 * Notes: The formatted input processing results of integers on different platforms are different.
 * History: 2019-04-03 zhaozhijian Code base quality improvement
 */
/*
 * [Standardize-exceptions] Use unsafe function: Performance-sensitive
 * [reason] Always used in the performance critical path,
 *          and sufficient input validation is performed before calling
 */

#ifndef INPUT_INL_5D13A042_DC3F_4ED9_A8D1_882811274C27
#define INPUT_INL_5D13A042_DC3F_4ED9_A8D1_882811274C27

#if SECUREC_IN_KERNEL
#if !defined(SECUREC_CTYPE_MACRO_ADAPT)
#include <linux/ctype.h>
#endif
#ifndef EOF
#define EOF  (-1)
#endif
#else
#if !defined(SECUREC_SYSAPI4VXWORKS) && !defined(SECUREC_CTYPE_MACRO_ADAPT)
#include <ctype.h>
#ifdef SECUREC_FOR_WCHAR
#include <wctype.h>             /* For iswspace */
#endif
#endif
#endif

#define SECUREC_NUM_WIDTH_SHORT                 0
#define SECUREC_NUM_WIDTH_INT                   1
#define SECUREC_NUM_WIDTH_LONG                  2
#define SECUREC_NUM_WIDTH_LONG_LONG             3 /* Also long double */

#define SECUREC_BUFFERED_BLOK_SIZE              1024

#if defined(SECUREC_VXWORKS_PLATFORM) && !defined(va_copy) && !defined(__va_copy)
/* The name is the same as system macro. */
#define __va_copy(d, s) do { \
    size_t size_of_d = (size_t)sizeof(d); \
    size_t size_of_s = (size_t)sizeof(s); \
    if (size_of_d != size_of_s) { \
        (void)memcpy((d), (s), sizeof(va_list)); \
    } else { \
        (void)memcpy(&(d), &(s), sizeof(va_list)); \
    } \
} SECUREC_WHILE_ZERO
#endif


#define SECUREC_MULTI_BYTE_MAX_LEN              6

/* Compatibility macro name cannot be modifie */
#ifndef UNALIGNED
#if !(defined(_M_IA64)) && !(defined(_M_AMD64))
#define UNALIGNED
#else
#define UNALIGNED __unaligned
#endif
#endif

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
/* Max 64bit value is 0xffffffffffffffff */
#define SECUREC_MAX_64BITS_VALUE                18446744073709551615ULL
#define SECUREC_MAX_64BITS_VALUE_DIV_TEN        1844674407370955161ULL
#define SECUREC_MAX_64BITS_VALUE_CUT_LAST_DIGIT 18446744073709551610ULL
#define SECUREC_MIN_64BITS_NEG_VALUE            9223372036854775808ULL
#define SECUREC_MAX_64BITS_POS_VALUE            9223372036854775807ULL
#define SECUREC_MIN_32BITS_NEG_VALUE            2147483648ULL
#define SECUREC_MAX_32BITS_POS_VALUE            2147483647ULL
#define SECUREC_MAX_32BITS_VALUE                4294967295ULL
#define SECUREC_MAX_32BITS_VALUE_INC            4294967296ULL
#define SECUREC_MAX_32BITS_VALUE_DIV_TEN        429496729ULL
#define SECUREC_LONG_BIT_NUM                    ((unsigned int)(sizeof(long) << 3U))
/* Use ULL to clean up cl6x compilation alerts */
#define SECUREC_MAX_LONG_POS_VALUE              ((unsigned long)(1ULL << (SECUREC_LONG_BIT_NUM - 1)) - 1)
#define SECUREC_MIN_LONG_NEG_VALUE              ((unsigned long)(1ULL << (SECUREC_LONG_BIT_NUM - 1)))

/* Covert to long long to clean up cl6x compilation alerts */
#define SECUREC_LONG_HEX_BEYOND_MAX(number)     (((unsigned long long)(number) >> (SECUREC_LONG_BIT_NUM - 4U)) > 0)
#define SECUREC_LONG_OCTAL_BEYOND_MAX(number)   (((unsigned long long)(number) >> (SECUREC_LONG_BIT_NUM - 3U)) > 0)

#define SECUREC_QWORD_HEX_BEYOND_MAX(number)    (((number) >> (64U - 4U)) > 0)
#define SECUREC_QWORD_OCTAL_BEYOND_MAX(number)  (((number) >> (64U - 3U)) > 0)

#define SECUREC_LP64_BIT_WIDTH                  64
#define SECUREC_LP32_BIT_WIDTH                  32
#endif

#define SECUREC_CHAR(x)                         (x)
#define SECUREC_BRACE                           '{'     /* [ to { */

#ifdef SECUREC_FOR_WCHAR
/* Bits for all wchar, size is 65536/8, only supports wide characters with a maximum length of two bytes */
#define SECUREC_BRACKET_TABLE_SIZE    8192
#define SECUREC_EOF WEOF
#define SECUREC_MB_LEN 16       /* Max. # bytes in multibyte char  ,see MB_LEN_MAX */
#else
/* Bits for all char, size is 256/8 */
#define SECUREC_BRACKET_TABLE_SIZE    32
#define SECUREC_EOF EOF
#endif

#if SECUREC_HAVE_WCHART
#define SECUREC_ARRAY_WIDTH_IS_WRONG(spec) ((spec).arrayWidth == 0 || \
    ((spec).isWCharOrLong <= 0 && (spec).arrayWidth > SECUREC_STRING_MAX_LEN) || \
    ((spec).isWCharOrLong > 0 && (spec).arrayWidth > SECUREC_WCHAR_STRING_MAX_LEN))
#else
#define SECUREC_ARRAY_WIDTH_IS_WRONG(spec) ((spec).arrayWidth == 0 || \
    ((spec).isWCharOrLong <= 0 && (spec).arrayWidth > SECUREC_STRING_MAX_LEN))
#endif

/* For next %n */
#define SECUREC_MEET_EOF_BEFORE_NEXT_N(ch, format) (((ch) == SECUREC_EOF) && \
    ((*(format) != SECUREC_CHAR('%')) || (*((format) + 1) != SECUREC_CHAR('n'))))

typedef struct {
#ifdef SECUREC_FOR_WCHAR
    unsigned char *table; /* Default NULL */
#else
    unsigned char table[SECUREC_BRACKET_TABLE_SIZE]; /* Array length is large enough in application scenarios */
#endif
    unsigned char mask; /* Default 0 */
} SecBracketTable;

#ifdef SECUREC_FOR_WCHAR
#define SECUREC_INIT_BRACKET_TABLE { NULL, 0 }
#else
#define SECUREC_INIT_BRACKET_TABLE { {0}, 0 }
#endif

#if SECUREC_ENABLE_SCANF_FLOAT
typedef struct {
    size_t floatStrTotalLen;       /* Initialization must be length of buffer in charater */
    size_t floatStrUsedLen;        /* Store float string len */
    SecChar buffer[SECUREC_FLOAT_BUFSIZE + 1];
    SecChar *floatStr;             /* Initialization must point to buffer */
    SecChar *allocatedFloatStr;    /* Initialization must be NULL  to store alloced point */
} SecFloatSpec;
#endif

typedef struct {
    SecInt ch;            /* Char read from input */
    int charCount;        /* Number of characters processed */
    SecUnsignedInt64 number64;
    unsigned long number;
    int numberWidth;      /* 0 = SHORT, 1 = int, > 1  long or L_DOUBLE */
    int isInt64Arg;       /* 1 for 64-bit integer, 0 otherwise */
    int negative;         /* 0 is positive */
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    int beyondMax;        /* Non-zero means beyond */
#endif
    void *argPtr;         /* Variable parameter pointer */
    size_t arrayWidth;    /* Length of pointer Variable parameter, in charaters */
    int width;            /* Width number in format */
    int widthSet;         /* 0 is not set width in format */
    int convChr;          /* Lowercase format conversion characters */
    int oriConvChr;       /* Store  original format conversion, convChr may change when parsing integers */
    signed char isWCharOrLong;  /* -1/0 not wchar or long, 1 for wchar or long */
    char suppress;              /* 0 is not have %* in format */
} SecScanSpec;

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
#define SECUREC_INIT_NUMBER_SPEC { 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0 }
#else
#define SECUREC_INIT_NUMBER_SPEC { 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0 }
#endif

#ifdef SECUREC_FOR_WCHAR
#define SECUREC_GETC fgetwc
#define SECUREC_UN_GETC ungetwc
#define SECUREC_CHAR_MASK 0xffff
#else
#define SECUREC_GETC fgetc
#define SECUREC_UN_GETC ungetc
#define SECUREC_CHAR_MASK 0xff
#endif

/* Record a flag for each bit */
#define SECUREC_BRACKET_INDEX(x)  ((unsigned int)(x) >> 3)
#define SECUREC_BRACKET_VALUE(x)  ((unsigned char)(1 << ((unsigned int)(x) & 7)))

/*
 * Set char in %[xxx] into table, only supports  wide characters with a maximum length of two bytes
 */
SECUREC_INLINE void SecBracketSetBit(unsigned char *table, SecUnsignedChar ch)
{
    unsigned int tableIndex = SECUREC_BRACKET_INDEX(((unsigned int)(int)(ch) & SECUREC_CHAR_MASK));
    unsigned int tableValue = SECUREC_BRACKET_VALUE(((unsigned int)(int)(ch) & SECUREC_CHAR_MASK));
    /* Do not use   |=   optimize this code, it will cause compiling warning */
    table[tableIndex] = (unsigned char)(table[tableIndex] | tableValue);
}

/*
 * Determine whether the expression can be satisfied
 */
SECUREC_INLINE int SecCanInputForBracket(int convChr, SecInt ch, const SecBracketTable *bracketTable)
{
    unsigned int tableIndex = SECUREC_BRACKET_INDEX(((unsigned int)(int)(ch) & SECUREC_CHAR_MASK));
    unsigned int tableValue = SECUREC_BRACKET_VALUE(((unsigned int)(int)(ch) & SECUREC_CHAR_MASK));
#ifdef SECUREC_FOR_WCHAR
    return (convChr == SECUREC_BRACE && bracketTable->table != NULL &&
        ((bracketTable->table[tableIndex] ^ bracketTable->mask) & tableValue));
#else
    return (convChr == SECUREC_BRACE && ((bracketTable->table[tableIndex] ^ bracketTable->mask) & tableValue));
#endif
}

/*
 * String input ends when blank character is encountered
 */
SECUREC_INLINE int SecCanInputString(int convChr, SecInt ch)
{
    return ((convChr) == SECUREC_CHAR('s') &&
        (!((ch) >= SECUREC_CHAR('\t') && (ch) <= SECUREC_CHAR('\r')) && (ch) != SECUREC_CHAR(' ')));
}

/*
 * Can input a character when format is %c
 */
SECUREC_INLINE int SecCanInputCharacter(int convChr)
{
    return (convChr == SECUREC_CHAR('c'));
}

/*
 * Determine if it is a 64-bit pointer  function
 * Return 0 is not ,1 is 64bit pointer
 */
SECUREC_INLINE int SecIs64BitPtr(size_t sizeOfVoidStar)
{
    /* Point size is 4 or 8 , Under the 64 bit system, the value not 0 */
    /* To clear e778 */
    if ((sizeOfVoidStar & sizeof(SecInt64)) != 0) {
        return 1;
    }
    return 0;
}
SECUREC_INLINE int SecIsDigit(SecInt ch);
SECUREC_INLINE int SecIsXdigit(SecInt ch);
SECUREC_INLINE int SecIsSpace(SecInt ch);
SECUREC_INLINE SecInt SecSkipSpaceChar(SecFileStream *stream, int *counter);
SECUREC_INLINE SecInt SecGetChar(SecFileStream *stream, int *counter);
SECUREC_INLINE void SecUnGetChar(SecInt ch, SecFileStream *stream, int *counter);

#if SECUREC_ENABLE_SCANF_FLOAT

/*
 * Convert a floating point string to a floating point number
 */
SECUREC_INLINE void SecAssignFloat(const char *floatStr, int numberWidth, void *argPtr)
{
    char *endPtr = NULL;
    double d;
#if SECUREC_SUPPORT_STRTOLD
    if (numberWidth == SECUREC_NUM_WIDTH_LONG_LONG) {
        long double d2 = strtold(floatStr, &endPtr);
        *(long double UNALIGNED *)(argPtr) = d2;
        return;
    }
#endif
    d = strtod(floatStr, &endPtr);
    if (numberWidth > SECUREC_NUM_WIDTH_INT) {
        *(double UNALIGNED *)(argPtr) = (double)d;
    } else {
        *(float UNALIGNED *)(argPtr) = (float)d;
    }
}

#ifdef SECUREC_FOR_WCHAR
/*
 * Convert a floating point wchar string to a floating point number
 * Success  ret 0
 */
SECUREC_INLINE int SecAssignFloatW(const SecFloatSpec *floatSpec, const  SecScanSpec *spec)
{
    /* Convert float string */
    size_t mbsLen;
    size_t tempFloatStrLen = (size_t)(floatSpec->floatStrTotalLen + 1) * sizeof(wchar_t);
    char *tempFloatStr = (char *)SECUREC_MALLOC(tempFloatStrLen);

    if (tempFloatStr == NULL) {
        return -1;
    }
    tempFloatStr[0] = '\0';
    SECUREC_MASK_MSVC_CRT_WARNING
    mbsLen = wcstombs(tempFloatStr, floatSpec->floatStr, tempFloatStrLen - 1);
    SECUREC_END_MASK_MSVC_CRT_WARNING
    /* This condition must satisfy mbsLen is not -1 */
    if (mbsLen < tempFloatStrLen) {
        tempFloatStr[mbsLen] = '\0';
        SecAssignFloat(tempFloatStr, spec->numberWidth, spec->argPtr);
    } else {
        SECUREC_FREE(tempFloatStr);
        return -1;
    }
    SECUREC_FREE(tempFloatStr);
    return 0;
}
#endif

/*
 * Init SecFloatSpec befor parse format
 */
SECUREC_INLINE void SecInitFloatSpec(SecFloatSpec *floatSpec)
{
    floatSpec->allocatedFloatStr = NULL;
    floatSpec->floatStrTotalLen = sizeof(floatSpec->buffer) / sizeof(floatSpec->buffer[0]);
    floatSpec->floatStr = floatSpec->buffer;
    floatSpec->floatStrUsedLen = 0;
}

SECUREC_INLINE void SecFreeFloatSpec(SecFloatSpec *floatSpec, int *doneCount)
{
     /* LSD 2014.3.6 add, clear the stack data */
    if (memset_s(floatSpec->buffer, sizeof(floatSpec->buffer), 0, sizeof(floatSpec->buffer)) != EOK) {
        *doneCount = 0; /* This code just to meet the coding requirements */
    }
    /* The pFloatStr can be alloced in SecUpdateFloatString function, clear and free it */
    if (floatSpec->allocatedFloatStr != NULL) {
        size_t bufferSize = floatSpec->floatStrTotalLen * sizeof(SecChar);
        if (memset_s(floatSpec->allocatedFloatStr, bufferSize, 0, bufferSize) != EOK) {
            *doneCount = 0; /* This code just to meet the coding requirements */
        }
        SECUREC_FREE(floatSpec->allocatedFloatStr);
        floatSpec->allocatedFloatStr = NULL;
        floatSpec->floatStr = NULL;
    }
}

/*
 * Splice floating point string
 * Return 0 OK
 */
SECUREC_INLINE int SecUpdateFloatString(SecChar ch, SecFloatSpec *floatSpec)
{
    floatSpec->floatStr[floatSpec->floatStrUsedLen++] = ch;
    if (floatSpec->floatStrUsedLen >= floatSpec->floatStrTotalLen) {
        /* Buffer size is len x sizeof(SecChar) */
        size_t oriSize = floatSpec->floatStrTotalLen * sizeof(SecChar);
        /* Add one character to clear tool warning */
        size_t nextSize = (oriSize * 2) + sizeof(SecChar); /* Multiply 2 to extend buffer size */

        /* Prevents integer overflow, the maximum length of SECUREC_MAX_WIDTH_LEN is enough */
        if (nextSize <= SECUREC_MAX_WIDTH_LEN) {
            void *nextBuffer = (void *)SECUREC_MALLOC(nextSize);
            if (nextBuffer == NULL) {
                return -1;
            }
            if (memcpy_s(nextBuffer, nextSize, floatSpec->floatStr, oriSize) != EOK) {
                SECUREC_FREE(nextBuffer);   /* This is a dead code, just to meet the coding requirements */
                return -1;
            }
            /* Clear old buffer memory */
            if (memset_s(floatSpec->floatStr, oriSize, 0, oriSize) != EOK) {
                SECUREC_FREE(nextBuffer);   /* This is a dead code, just to meet the coding requirements */
                return -1;
            }
            /* Free old allocated buffer */
            if (floatSpec->allocatedFloatStr != NULL) {
                SECUREC_FREE(floatSpec->allocatedFloatStr);
            }
            floatSpec->allocatedFloatStr = (SecChar *)(nextBuffer);    /* Use to clear free on stack warning */
            floatSpec->floatStr = (SecChar *)(nextBuffer);
            floatSpec->floatStrTotalLen = nextSize / sizeof(SecChar); /* Get buffer total len in character */
            return 0;
        }
        return -1; /* Next size is beyond max */
    }
    return 0;
}


/* Do not use localeconv()->decimal_pointif  onlay support  '.' */
SECUREC_INLINE int SecIsFloatDecimal(SecChar ch)
{
    return ((ch) == SECUREC_CHAR('.'));
}

/*
 * Scan value of exponent.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputFloatE(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    spec->ch = SecGetChar(stream, &(spec->charCount));
    if (spec->ch == SECUREC_CHAR('+') || spec->ch == SECUREC_CHAR('-')) {
        if (spec->ch == SECUREC_CHAR('-') && SecUpdateFloatString((SecChar)'-', floatSpec) != 0) {
            return -1;
        }
        if (spec->width != 0) {
            spec->ch = SecGetChar(stream, &(spec->charCount));
            --spec->width;
        }
    }

    while (SecIsDigit(spec->ch) && spec->width-- != 0) {
        if (SecUpdateFloatString((SecChar)spec->ch, floatSpec) != 0) {
            return -1;
        }
        spec->ch = SecGetChar(stream, &(spec->charCount));
    }
    return 0;
}

/*
 * Scan %f.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputFloat(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    int started = -1;
    spec->ch = SecGetChar(stream, &(spec->charCount));

    floatSpec->floatStrUsedLen = 0;
    if (spec->ch == SECUREC_CHAR('-')) {
        floatSpec->floatStr[floatSpec->floatStrUsedLen++] = SECUREC_CHAR('-');
        --spec->width;
        spec->ch = SecGetChar(stream, &(spec->charCount));
    } else if (spec->ch == SECUREC_CHAR('+')) {
        --spec->width;
        spec->ch = SecGetChar(stream, &(spec->charCount));
    }

    if (spec->widthSet == 0) {    /* Must care width */
        spec->width = -1; /* -1 is unlimited */
    }

    /* Now get integral part */
    while (SecIsDigit(spec->ch) && spec->width-- != 0) {
        started = 0;
        /* The ch must be '0' - '9' */
        if (SecUpdateFloatString((SecChar)spec->ch, floatSpec) != 0) {
            return -1;
        }
        spec->ch = SecGetChar(stream, &(spec->charCount));
    }

    /* Now get fractional part */
    if (SecIsFloatDecimal((SecChar)spec->ch) && spec->width-- != 0) {
        /* Now check for decimal */
        if (SecUpdateFloatString((SecChar)spec->ch, floatSpec) != 0) {
            return -1;
        }
        spec->ch = SecGetChar(stream, &(spec->charCount));
        while (SecIsDigit(spec->ch) && spec->width-- != 0) {
            started = 0;
            if (SecUpdateFloatString((SecChar)spec->ch, floatSpec) != 0) {
                return -1;
            }
            spec->ch = SecGetChar(stream, &(spec->charCount));
        }
    }

    /* Now get exponent part */
    if (started == 0 && (spec->ch == SECUREC_CHAR('e') || spec->ch == SECUREC_CHAR('E')) && spec->width-- != 0) {
        if (SecUpdateFloatString((SecChar)'e', floatSpec) != 0) {
            return -1;
        }
        if (SecInputFloatE(stream, spec, floatSpec) != 0) {
            return -1;
        }
    }
    /* Un set the last character that is not a floating point number */
    SecUnGetChar(spec->ch, stream, &(spec->charCount));
    /* Make sure  have a string terminator, buffer is large enough */
    floatSpec->floatStr[floatSpec->floatStrUsedLen] = SECUREC_CHAR('\0');
    return started;
}
#endif

#if (!defined(SECUREC_FOR_WCHAR) && SECUREC_HAVE_WCHART && SECUREC_HAVE_MBTOWC) || \
    (!defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_VERSION))
/* LSD only multi-bytes string need isleadbyte() function */
SECUREC_INLINE int SecIsLeadByte(SecInt ch)
{
    unsigned int c = (unsigned int)ch;
#if !(defined(_MSC_VER) || defined(_INC_WCTYPE))
    return (int)(c & 0x80);
#else
    return (int)isleadbyte((int)(c & 0xff));
#endif
}
#endif

/*
 * Parsing whether it is a wide character
 */
SECUREC_INLINE void SecUpdateWcharFlagByType(SecUnsignedChar ch, SecScanSpec *spec)
{
    if (spec->isWCharOrLong != 0) {
        /* Wide character identifiers have been explicitly set by l or h flag */
        return;
    }

    /* Set default flag */
#if defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    spec->isWCharOrLong = 1;  /* On windows wide char version %c %s %[ is wide char */
#else
    spec->isWCharOrLong = -1; /* On linux all version %c %s %[ is multi char */
#endif

    if (ch == SECUREC_CHAR('C') || ch == SECUREC_CHAR('S')) {
#if defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_WIN_FORMAT)
        spec->isWCharOrLong = -1; /* On windows wide char version %C %S is multi char */
#else
        spec->isWCharOrLong = 1;  /* On linux all version %C %S is wide char */
#endif
    }

    return;
}
/*
 * Decode  %l %ll
 */
SECUREC_INLINE void SecDecodeScanQualifierL(const SecUnsignedChar **format, SecScanSpec *spec)
{
    const SecUnsignedChar *fmt = *format;
    if (*(fmt + 1) == SECUREC_CHAR('l')) {
        spec->isInt64Arg = 1;
        spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;
        ++fmt;
    } else {
        spec->numberWidth = SECUREC_NUM_WIDTH_LONG;
#if defined(SECUREC_ON_64BITS) && !(defined(SECUREC_COMPATIBLE_WIN_FORMAT))
        /* On window 64 system sizeof long is 32bit */
        spec->isInt64Arg = 1;
#endif
        spec->isWCharOrLong = 1;
    }
    *format = fmt;
}

/*
 * Decode  %I %I43 %I64 %Id %Ii %Io ...
 * Set finishFlag to  1  finish Flag
 */
SECUREC_INLINE void SecDecodeScanQualifierI(const SecUnsignedChar **format, SecScanSpec *spec, int *finishFlag)
{
    const SecUnsignedChar *fmt = *format;
    if ((*(fmt + 1) == SECUREC_CHAR('6')) &&
        (*(fmt + 2) == SECUREC_CHAR('4'))) { /* Offset 2 for I64 */
        spec->isInt64Arg = 1;
        *format = *format + 2; /* Add 2 to skip I64 point to '4' next loop will inc */
    } else if ((*(fmt + 1) == SECUREC_CHAR('3')) &&
                (*(fmt + 2) == SECUREC_CHAR('2'))) { /* Offset 2 for I32 */
        *format = *format + 2; /* Add 2 to skip I32 point to '2' next loop will inc */
    } else if ((*(fmt + 1) == SECUREC_CHAR('d')) ||
                (*(fmt + 1) == SECUREC_CHAR('i')) ||
                (*(fmt + 1) == SECUREC_CHAR('o')) ||
                (*(fmt + 1) == SECUREC_CHAR('x')) ||
                (*(fmt + 1) == SECUREC_CHAR('X'))) {
        spec->isInt64Arg = SecIs64BitPtr(sizeof(void *));
    } else {
        /* For %I */
        spec->isInt64Arg = SecIs64BitPtr(sizeof(void *));
        *finishFlag = 1;
    }
}

SECUREC_INLINE int SecDecodeScanWidth(const SecUnsignedChar **format, SecScanSpec *spec)
{
    const SecUnsignedChar *fmt = *format;
    while (SecIsDigit((SecInt)(int)(*fmt))) {
        spec->widthSet = 1;
        if (SECUREC_MUL_TEN_ADD_BEYOND_MAX(spec->width)) {
            return -1;
        }
        spec->width = (int)SECUREC_MUL_TEN((unsigned int)spec->width) + (unsigned char)(*fmt - SECUREC_CHAR('0'));
        ++fmt;
    }
    *format = fmt;
    return 0;
}

/*
 * Init default flags for each format. do not init ch this variable is context-dependent
 */
SECUREC_INLINE void SecSetDefaultScanSpec(SecScanSpec *spec)
{
    /* The ch and charCount member variables cannot be initialized here */
    spec->number64 = 0;
    spec->number = 0;
    spec->numberWidth = SECUREC_NUM_WIDTH_INT;    /* 0 = SHORT, 1 = int, > 1  long or L_DOUBLE */
    spec->isInt64Arg = 0;                         /* 1 for 64-bit integer, 0 otherwise */
    spec->negative = 0;
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    spec->beyondMax = 0;
#endif
    spec->argPtr = NULL;
    spec->arrayWidth = 0;
    spec->width = 0;
    spec->widthSet = 0;
    spec->convChr = 0;
    spec->oriConvChr = 0;
    spec->isWCharOrLong = 0;
    spec->suppress = 0;
}

/*
 * Decode qualifier %I %L %h ...
 * Set finishFlag to  1  finish Flag
 */
SECUREC_INLINE void  SecDecodeScanQualifier(const SecUnsignedChar **format, SecScanSpec *spec, int *finishFlag)
{
    switch ((int)(unsigned char)(**(format))) {
        case SECUREC_CHAR('F'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('N'):
            break;
        case SECUREC_CHAR('h'):
            --spec->numberWidth; /* The h for SHORT , hh for CHAR */
            spec->isWCharOrLong = -1;
            break;
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
        case SECUREC_CHAR('j'):
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;  /* For intmax_t or uintmax_t */
            spec->isInt64Arg = 1;
            break;
        case SECUREC_CHAR('t'): /* fall-through */ /* FALLTHRU */
#endif
#if SECUREC_IN_KERNEL
        case SECUREC_CHAR('Z'): /* fall-through */ /* FALLTHRU */
#endif
        case SECUREC_CHAR('z'):
#ifdef SECUREC_ON_64BITS
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;
            spec->isInt64Arg = 1;
#else
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG;
#endif
            break;
        case SECUREC_CHAR('L'):    /* For long double */ /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('q'):
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;
            spec->isInt64Arg = 1;
            break;
        case SECUREC_CHAR('l'):
            SecDecodeScanQualifierL(format, spec);
            break;
        case SECUREC_CHAR('w'):
            spec->isWCharOrLong = 1;
            break;
        case SECUREC_CHAR('*'):
            spec->suppress = 1;
            break;
        case SECUREC_CHAR('I'):
            SecDecodeScanQualifierI(format, spec, finishFlag);
            break;
        default:
            *finishFlag = 1;
            break;
    }

}
/*
 * Decode width and qualifier in format
 */
SECUREC_INLINE int SecDecodeScanFlag(const SecUnsignedChar **format, SecScanSpec *spec)
{
    const SecUnsignedChar *fmt = *format;
    int finishFlag = 0;

    do {
        ++fmt; /*  First skip % , next  seek fmt */
        /* May %*6d , so put it inside the loop */
        if (SecDecodeScanWidth(&fmt, spec) != 0) {
            return -1;
        }
        SecDecodeScanQualifier(&fmt, spec, &finishFlag);
    } while (finishFlag == 0);
    *format = fmt;
    return 0;
}

/*
 * Judging whether a zeroing buffer is needed according to different formats
 */
SECUREC_INLINE int SecDecodeClearFormat(const SecUnsignedChar *format, int *convChr)
{
    const SecUnsignedChar *fmt = format;
    /* To lowercase */
    int ch = (unsigned char)(*fmt) | (SECUREC_CHAR('a') - SECUREC_CHAR('A'));
    if (!(ch == SECUREC_CHAR('c') || ch == SECUREC_CHAR('s') || ch == SECUREC_BRACE)) {
        return -1;     /* First argument is not a string type */
    }
    if (ch == SECUREC_BRACE) {
#if !(defined(SECUREC_COMPATIBLE_WIN_FORMAT))
        if (*fmt == SECUREC_CHAR('{')) {
            return -1;
        }
#endif
        ++fmt;
        if (*fmt == SECUREC_CHAR('^')) {
            ++fmt;
        }
        if (*fmt == SECUREC_CHAR(']')) {
            ++fmt;
        }
        while (*fmt != SECUREC_CHAR('\0') && *fmt != SECUREC_CHAR(']')) {
            ++fmt;
        }
        if (*fmt == SECUREC_CHAR('\0')) {
            return -1; /* Trunc'd format string */
        }
    }
    *convChr = ch;
    return 0;
}

/*
 * Add L'\0' for wchar string , add '\0' for char string
 */
SECUREC_INLINE void SecAddEndingZero(void *ptr, const SecScanSpec *spec)
{
    if (spec->suppress == 0) {
        *(char *)ptr = '\0';
#if SECUREC_HAVE_WCHART
        if (spec->isWCharOrLong > 0) {
            *(wchar_t UNALIGNED *)ptr = L'\0';
        }
#endif
    }
}

#ifdef SECUREC_FOR_WCHAR
/*
 *  Clean up the first %s %c buffer to zero for wchar version
 */
void SecClearDestBufW(const wchar_t *buffer, const wchar_t *format, va_list argList)
#else
/*
 *  Clean up the first %s %c buffer to zero for char version
 */
void SecClearDestBuf(const char *buffer, const char *format, va_list argList)
#endif
{

    va_list argListSave;        /* Backup for argList value, this variable don't need initialized */
    SecScanSpec spec;
    int convChr = 0;
    const SecUnsignedChar *fmt = (const SecUnsignedChar *)format;
    if (fmt == NULL) {
        return;
    }

    /* Find first % */
    while (*fmt != SECUREC_CHAR('\0') && *fmt != SECUREC_CHAR('%')) {
        ++fmt;
    }
    if (*fmt == SECUREC_CHAR('\0')) {
        return;
    }

    SecSetDefaultScanSpec(&spec);
    if (SecDecodeScanFlag(&fmt, &spec) != 0) {
        return;
    }

    /* Update wchar flag for %S %C */
    SecUpdateWcharFlagByType(*fmt, &spec);

    if (spec.suppress != 0 || SecDecodeClearFormat(fmt, &convChr) != 0) {
        return;
    }

    if (buffer != NULL && *buffer != SECUREC_CHAR('\0') && convChr != SECUREC_CHAR('s')) {
        /*
         * When buffer not empty just clear %s.
         * Example call sscanf by  argment of (" \n", "%s", s, sizeof(s))
         */
        return;
    }
    (void)memset(&argListSave, 0, sizeof(va_list)); /* To clear e530 argListSave not initialized */
#if defined(va_copy)
    va_copy(argListSave, argList);
#elif defined(__va_copy)        /* For vxworks */
    __va_copy(argListSave, argList);
#else
    argListSave = argList;
#endif
    do {
        void *argPtr = (void *)va_arg(argListSave, void *);
        /* Get the next argument - size of the array in characters */
        size_t arrayWidth = ((size_t)(va_arg(argListSave, size_t))) & 0xFFFFFFFFUL;
        va_end(argListSave);
        /* To clear e438 last value assigned not used , the compiler will optimize this code */
        (void)argListSave;
        /* There is no need to judge the upper limit */
        if (arrayWidth == 0 || argPtr == NULL) {
            return;
        }

        /* Clear one char */
        SecAddEndingZero(argPtr, &spec);
    } SECUREC_WHILE_ZERO;
    return;

}

/*
 *  Assign number  to output buffer
 */
SECUREC_INLINE void SecAssignNumber(const SecScanSpec *spec)
{
    void *argPtr = spec->argPtr;
    if (spec->isInt64Arg != 0) {
#if defined(SECUREC_VXWORKS_PLATFORM)
#if defined(SECUREC_VXWORKS_PLATFORM_COMP)
        *(SecInt64 UNALIGNED *)argPtr = (SecInt64)(spec->number64);
#else
        /* Take number64 as unsigned number unsigned to int clear Compile warning */
        *(SecInt64 UNALIGNED *)argPtr = *(SecUnsignedInt64 *)(&(spec->number64));
#endif
#else
        /* Take number64 as unsigned number */
        *(SecInt64 UNALIGNED *)argPtr = (SecInt64)(spec->number64);
#endif
        return;
    }
    if (spec->numberWidth > SECUREC_NUM_WIDTH_INT) {
        /* Take number as unsigned number */
        *(long UNALIGNED *)argPtr = (long)(spec->number);
    } else if (spec->numberWidth == SECUREC_NUM_WIDTH_INT) {
        *(int UNALIGNED *)argPtr = (int)(spec->number);
    } else if (spec->numberWidth == SECUREC_NUM_WIDTH_SHORT) {
        /* Take number as unsigned number */
        *(short UNALIGNED *)argPtr = (short)(spec->number);
    } else {  /* < 0 for hh format modifier */
        /* Take number as unsigned number */
        *(char UNALIGNED *)argPtr = (char)(spec->number);
    }
}

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
/*
 *  Judge the long bit width
 */
SECUREC_INLINE int SecIsLongBitEqual(int bitNum)
{
    return (unsigned int)bitNum == SECUREC_LONG_BIT_NUM;
}
#endif

/*
 * Convert hexadecimal characters to decimal value
 */
SECUREC_INLINE int SecHexValueOfChar(SecInt ch)
{
    /* Use isdigt Causing tool false alarms */
    return (int)((ch >= '0' && ch <= '9') ? ((unsigned char)ch - '0') :
            ((((unsigned char)ch | (unsigned char)('a' - 'A')) - ('a')) + 10)); /* Adding 10 is to hex value */
}

/*
 * Parse decimal character to integer for 32bit .
 */
static void SecDecodeNumberDecimal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    unsigned long decimalEdge = SECUREC_MAX_32BITS_VALUE_DIV_TEN;
#ifdef SECUREC_ON_64BITS
    if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH)) {
        decimalEdge = (unsigned long)SECUREC_MAX_64BITS_VALUE_DIV_TEN;
    }
#endif
    if (spec->number > decimalEdge) {
        spec->beyondMax = 1;
    }
#endif
    spec->number = SECUREC_MUL_TEN(spec->number);
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->number == SECUREC_MUL_TEN(decimalEdge)) {
        SecUnsignedInt64 number64As = (unsigned long)SECUREC_MAX_64BITS_VALUE - spec->number;
        if (number64As < (SecUnsignedInt64)((SecUnsignedInt)spec->ch - SECUREC_CHAR('0'))) {
            spec->beyondMax = 1;
        }
    }
#endif
    spec->number += (unsigned long)((SecUnsignedInt)spec->ch - SECUREC_CHAR('0'));

}


/*
 * Parse Hex character to integer for 32bit .
 */
static void SecDecodeNumberHex(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_LONG_HEX_BEYOND_MAX(spec->number)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number = SECUREC_MUL_SIXTEEN(spec->number);
    spec->number += (unsigned long)(unsigned int)SecHexValueOfChar(spec->ch);
}


/*
 * Parse Octal character to integer for 32bit .
 */
static void SecDecodeNumberOctal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_LONG_OCTAL_BEYOND_MAX(spec->number)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number = SECUREC_MUL_EIGHT(spec->number);
    spec->number += (unsigned long)((SecUnsignedInt)spec->ch - SECUREC_CHAR('0'));
}


#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
/* Compatible with integer negative values other than int */
SECUREC_INLINE void SecFinishNumberNegativeOther(SecScanSpec *spec)
{
    if (spec->oriConvChr == SECUREC_CHAR('d') || spec->oriConvChr == SECUREC_CHAR('i')) {
        if (spec->number > SECUREC_MIN_LONG_NEG_VALUE) {
            spec->number = SECUREC_MIN_LONG_NEG_VALUE;
        } else {
            spec->number = (unsigned long)(-(long)spec->number);
        }
        if (spec->beyondMax != 0) {
            if (spec->numberWidth < SECUREC_NUM_WIDTH_INT) {
                spec->number = 0;
            } else if (spec->numberWidth == SECUREC_NUM_WIDTH_LONG) {
                spec->number = SECUREC_MIN_LONG_NEG_VALUE;
            }
        }
    } else { /* For o, u, x, X, p */
        spec->number = (unsigned long)(-(long)spec->number);
        if (spec->beyondMax != 0) {
            spec->number |= (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
    }
}
/* Compatible processing of integer negative numbers */
SECUREC_INLINE void SecFinishNumberNegativeInt(SecScanSpec *spec)
{
    if (spec->oriConvChr == SECUREC_CHAR('d') || spec->oriConvChr == SECUREC_CHAR('i')) {
#ifdef SECUREC_ON_64BITS
        if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH)) {
            if ((spec->number > SECUREC_MIN_64BITS_NEG_VALUE)) {
                spec->number = 0;
            } else {
                spec->number = (unsigned int)(-(int)spec->number);
            }
        }
#else
        if (SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH)) {
            if ((spec->number > SECUREC_MIN_32BITS_NEG_VALUE)) {
                spec->number = SECUREC_MIN_32BITS_NEG_VALUE;
            } else {
                spec->number = (unsigned int)(-(int)spec->number);
            }
        }
#endif
        if (spec->beyondMax != 0) {
#ifdef SECUREC_ON_64BITS
            if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH)) {
                spec->number = 0;
            }
#else
            if (SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH)) {
                spec->number = SECUREC_MIN_32BITS_NEG_VALUE;
            }
#endif
        }
    } else {            /* For o, u, x, X ,p */
#ifdef SECUREC_ON_64BITS
        if (spec->number > SECUREC_MAX_32BITS_VALUE_INC) {
            spec->number = SECUREC_MAX_32BITS_VALUE;
        } else {
            spec->number = (unsigned int)(-(int)spec->number);
        }
#else
        spec->number = (unsigned int)(-(int)spec->number);
#endif
        if (spec->beyondMax != 0) {
            spec->number |= (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
    }
}

/* Compatible with integer positive values other than int */
SECUREC_INLINE void SecFinishNumberPositiveOther(SecScanSpec *spec)
{
    if (spec->oriConvChr == SECUREC_CHAR('d') || spec->oriConvChr == SECUREC_CHAR('i')) {
        if (spec->number > SECUREC_MAX_LONG_POS_VALUE) {
            spec->number = SECUREC_MAX_LONG_POS_VALUE;
        }
        if ((spec->beyondMax != 0 && spec->numberWidth < SECUREC_NUM_WIDTH_INT)) {
            spec->number |= (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
        if (spec->beyondMax != 0 && spec->numberWidth == SECUREC_NUM_WIDTH_LONG) {
            spec->number = SECUREC_MAX_LONG_POS_VALUE;
        }
    } else {
        if (spec->beyondMax != 0) {
            spec->number |= (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
    }
}

/* Compatible processing of integer positive numbers */
SECUREC_INLINE void SecFinishNumberPositiveInt(SecScanSpec *spec)
{
    if (spec->oriConvChr == SECUREC_CHAR('d') || spec->oriConvChr == SECUREC_CHAR('i')) {
#ifdef SECUREC_ON_64BITS
        if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH)) {
            if (spec->number > SECUREC_MAX_64BITS_POS_VALUE) {
                spec->number |= (unsigned long)SECUREC_MAX_64BITS_VALUE;
            }
        }
        if (spec->beyondMax != 0 && SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH)) {
            spec->number |= (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
#else
        if (SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH)) {
            if (spec->number > SECUREC_MAX_32BITS_POS_VALUE) {
                spec->number = SECUREC_MAX_32BITS_POS_VALUE;
            }
        }
        if (spec->beyondMax != 0 && SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH)) {
            spec->number = SECUREC_MAX_32BITS_POS_VALUE;
        }
#endif
    } else {            /* For o,u,x,X,p */
        if (spec->beyondMax != 0) {
            spec->number = SECUREC_MAX_32BITS_VALUE;
        }
    }
}

#endif

/*
 * Parse decimal character to integer for 64bit .
 */
static void SecDecodeNumber64Decimal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->number64 > SECUREC_MAX_64BITS_VALUE_DIV_TEN) {
        spec->beyondMax = 1;
    }
#endif
    spec->number64 = SECUREC_MUL_TEN(spec->number64);
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->number64 == SECUREC_MAX_64BITS_VALUE_CUT_LAST_DIGIT) {
        SecUnsignedInt64 number64As = (SecUnsignedInt64)SECUREC_MAX_64BITS_VALUE - spec->number64;
        if (number64As < (SecUnsignedInt64)((SecUnsignedInt)spec->ch - SECUREC_CHAR('0'))) {
            spec->beyondMax = 1;
        }
    }
#endif
    spec->number64 += (SecUnsignedInt64)((SecUnsignedInt)spec->ch - SECUREC_CHAR('0'));
}

/*
 * Parse Hex character to integer for 64bit .
 */
static void SecDecodeNumber64Hex(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_QWORD_HEX_BEYOND_MAX(spec->number64)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number64 = SECUREC_MUL_SIXTEEN(spec->number64);
    spec->number64 += (SecUnsignedInt64)(unsigned int)SecHexValueOfChar(spec->ch);

}

/*
 * Parse Octal character to integer for 64bit .
 */
static void SecDecodeNumber64Octal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_QWORD_OCTAL_BEYOND_MAX(spec->number64)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number64 = SECUREC_MUL_EIGHT(spec->number64);
    spec->number64 += (SecUnsignedInt64)((SecUnsignedInt)spec->ch - SECUREC_CHAR('0'));
}

#define SECUREC_DECODE_NUMBER_FUNC_NUM 2
/* Function name cannot add address symbol, causing 546 alarm */
static void (*g_secDecodeNumberHex[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = \
    { SecDecodeNumberHex, SecDecodeNumber64Hex };
static void (*g_secDecodeNumberOctal[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = \
    { SecDecodeNumberOctal, SecDecodeNumber64Octal };
static void (*g_secDecodeNumberDecimal[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = \
    { SecDecodeNumberDecimal, SecDecodeNumber64Decimal };

/*
 * Parse 64-bit integer formatted input, return 0 when ch is a number.
 */
SECUREC_INLINE int SecDecodeNumber(SecScanSpec *spec)
{
    if (spec->convChr == SECUREC_CHAR('x') || spec->convChr == SECUREC_CHAR('p')) {
        if (SecIsXdigit(spec->ch)) {
            (*g_secDecodeNumberHex[spec->isInt64Arg])(spec);
        } else {
            return -1;
        }
        return 0;
    }
    if (!(SecIsDigit(spec->ch))) {
        return -1;
    }
    if (spec->convChr == SECUREC_CHAR('o')) {
        if (spec->ch < SECUREC_CHAR('8')) {
            (*g_secDecodeNumberOctal[spec->isInt64Arg])(spec);
        } else {
            return -1;
        }
    } else { /* The convChr is 'd' */
        (*g_secDecodeNumberDecimal[spec->isInt64Arg])(spec);
    }
    return 0;
}


/*
 * Complete the final 32-bit integer formatted input
 */
static void SecFinishNumber(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->negative != 0) {
        if (spec->numberWidth == SECUREC_NUM_WIDTH_INT) {
            SecFinishNumberNegativeInt(spec);
        } else {
            SecFinishNumberNegativeOther(spec);
        }
    } else {
        if (spec->numberWidth == SECUREC_NUM_WIDTH_INT) {
            SecFinishNumberPositiveInt(spec);
        } else {
            SecFinishNumberPositiveOther(spec);
        }
    }
#else
    if (spec->negative != 0) {
#if defined(__hpux)
        if (spec->oriConvChr != SECUREC_CHAR('p')) {
            spec->number = (unsigned long)(-(long)spec->number);
        }
#else
        spec->number = (unsigned long)(-(long)spec->number);
#endif
    }
#endif
    return;
}

/*
 * Complete the final 64-bit integer formatted input
 */
static void SecFinishNumber64(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->negative != 0) {
        if (spec->oriConvChr == SECUREC_CHAR('d') || spec->oriConvChr == SECUREC_CHAR('i')) {
            if (spec->number64 > SECUREC_MIN_64BITS_NEG_VALUE) {
                spec->number64 = SECUREC_MIN_64BITS_NEG_VALUE;
            } else {
                spec->number64 = (SecUnsignedInt64)(-(SecInt64)spec->number64);
            }
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MIN_64BITS_NEG_VALUE;
            }
        } else {                /* For o, u, x, X, p */
            spec->number64 = (SecUnsignedInt64)(-(SecInt64)spec->number64);
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MAX_64BITS_VALUE;
            }
        }
    } else {
        if (spec->oriConvChr == SECUREC_CHAR('d') || spec->oriConvChr == SECUREC_CHAR('i')) {
            if (spec->number64 > SECUREC_MAX_64BITS_POS_VALUE) {
                spec->number64 = SECUREC_MAX_64BITS_POS_VALUE;
            }
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MAX_64BITS_POS_VALUE;
            }
        } else {
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MAX_64BITS_VALUE;
            }
        }
    }
#else
    if (spec->negative != 0) {
#if defined(__hpux)
        if (spec->oriConvChr != SECUREC_CHAR('p')) {
            spec->number64 = (SecUnsignedInt64)(-(SecInt64)spec->number64);
        }
#else
        spec->number64 = (SecUnsignedInt64)(-(SecInt64)spec->number64);
#endif
    }
#endif
    return;
}
static void (*g_secFinishNumber[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = \
    { SecFinishNumber, SecFinishNumber64 };

#if SECUREC_ENABLE_SCANF_FILE

/*
 *  Adjust the pointer position of the file stream
 */
SECUREC_INLINE void SecSeekStream(SecFileStream *stream)
{
    if ((stream->count <= 0) && feof(stream->pf)) {
        /* File pointer at the end of file, don't need to seek back */
        stream->base[0] = '\0';
        return;
    }
    /* LSD seek to original position, bug fix 2014 1 21 */
    if (fseek(stream->pf, stream->oriFilePos, SEEK_SET)) {
        /* Seek failed, ignore it */
        stream->oriFilePos = 0;
        return;
    }

    if (stream->fileRealRead > 0) { /* LSD bug fix. when file reach to EOF, don't seek back */
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
        int loops;
        for (loops = 0; loops < (stream->fileRealRead / SECUREC_BUFFERED_BLOK_SIZE); ++loops) {
            if (fread(stream->base, (size_t)1, (size_t)SECUREC_BUFFERED_BLOK_SIZE,
                stream->pf) != SECUREC_BUFFERED_BLOK_SIZE) {
                break;
            }
        }
        if ((stream->fileRealRead % SECUREC_BUFFERED_BLOK_SIZE) != 0) {
            size_t len = fread(stream->base, (size_t)((unsigned int)stream->fileRealRead % SECUREC_BUFFERED_BLOK_SIZE),
                               (size_t)1, stream->pf);
            if ((len == 1 || len == 0) && (ftell(stream->pf) < stream->oriFilePos + stream->fileRealRead)) {
                (void)fseek(stream->pf, stream->oriFilePos + stream->fileRealRead, SEEK_SET);
            }
        }

#else
        /* On linux like system */
        if (fseek(stream->pf, stream->oriFilePos + stream->fileRealRead, SEEK_SET)) {
            /* Seek failed, ignore it */
            stream->oriFilePos = 0;
        }
#endif
    }

    return;
}

/*
 *  Adjust the pointer position of the file stream and free memory
 */
SECUREC_INLINE void SecAdjustStream(SecFileStream *stream)
{
    if (stream != NULL && (stream->flag & SECUREC_FILE_STREAM_FLAG) && stream->base != NULL) {
        SecSeekStream(stream);
        SECUREC_FREE(stream->base);
        stream->base = NULL;
    }
    return;
}
#endif

SECUREC_INLINE void SecSkipSpaceFormat(const SecUnsignedChar **format)
{
    const SecUnsignedChar *fmt = *format;
    while (SecIsSpace((SecInt)(int)(*fmt))) {
        ++fmt;
    }
    *format = fmt;
}

#if !defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_VERSION)
/*
 * Handling multi-character characters
 */
SECUREC_INLINE int SecDecodeLeadByte(SecScanSpec *spec,
    const SecUnsignedChar **format, SecFileStream *stream, int *counter)
{
#if SECUREC_HAVE_MBTOWC
    const SecUnsignedChar *fmt = *format;
    int ch1 = (int)spec->ch;
    int ch2 = SecGetChar(stream, counter);
    spec->ch = (SecInt)ch2;
    if (*fmt == SECUREC_CHAR('\0') || (int)(*fmt) != ch2) {
        /* LSD in console mode, ungetc twice may cause problem */
        SecUnGetChar(ch2, stream, counter);
        SecUnGetChar(ch1, stream, counter);
        return -1;
    }
    ++fmt;
    if (MB_CUR_MAX >= SECUREC_UTF8_BOM_HEADER_SIZE &&
        (((unsigned char)ch1 & SECUREC_UTF8_LEAD_1ST) == SECUREC_UTF8_LEAD_1ST) &&
        (((unsigned char)ch2 & SECUREC_UTF8_LEAD_2ND) == SECUREC_UTF8_LEAD_2ND)) {
        /* This char is very likely to be a UTF-8 char */
        wchar_t tempWChar;
        char temp[SECUREC_MULTI_BYTE_MAX_LEN];
        int ch3 = (int)SecGetChar(stream, counter);
        spec->ch = (SecInt)ch3;
        temp[0] = (char)ch1;
        temp[1] = (char)ch2; /* 1 index of second character */
        temp[2] = (char)ch3; /* 2 index of third character */
        temp[3] = '\0';      /* 3 of string terminator position */

        if (mbtowc(&tempWChar, temp, sizeof(temp)) > 0) {
            /* Succeed */
            if (*fmt == SECUREC_CHAR('\0') || (int)(*fmt) != ch3) {
                SecUnGetChar(ch3, stream, counter);
                return -1;
            }
            ++fmt;
            *counter = *counter - 1;
        } else {
            SecUnGetChar(ch3, stream, counter);
        }
    }
    *counter = *counter - 1;    /* Only count as one character read */
    *format = fmt;
    return 0;
#else
    SecUnGetChar(spec->ch, stream, counter);
    (void)format; /* To clear e438 last value assigned not used , the compiler will optimize this code */
    return -1;
#endif
}
#endif

/*
 *  Resolving sequence of characters from %[ format, format wile point to ']'
 */
SECUREC_INLINE int SecSetupBracketTable(const SecUnsignedChar **format, SecBracketTable *bracketTable)
{
    const SecUnsignedChar *fmt = *format;
    SecUnsignedChar prevChar = 0;
    SecUnsignedChar last = 0;
#if !(defined(SECUREC_COMPATIBLE_WIN_FORMAT))
    if (*fmt == SECUREC_CHAR('{')) {
        return -1;
    }
#endif
    /* For building "table" data */
    ++fmt; /* Skip [ */
    bracketTable->mask = 0;
    if (*fmt == SECUREC_CHAR('^')) {
        ++fmt;
        bracketTable->mask = (unsigned char)0xff;
    }
    if (*fmt == SECUREC_CHAR(']')) {
        prevChar = SECUREC_CHAR(']');
        ++fmt;
        SecBracketSetBit(bracketTable->table, SECUREC_CHAR(']'));
    }
    while (*fmt != SECUREC_CHAR('\0') && *fmt != SECUREC_CHAR(']')) {
        SecUnsignedChar expCh = *(fmt++);
        if (expCh != SECUREC_CHAR('-') || prevChar == 0 || *fmt == SECUREC_CHAR(']')) {
            /* Normal character */
            prevChar = expCh;
            SecBracketSetBit(bracketTable->table, expCh);
        } else {
            /* For %[a-z] */
            expCh = *(fmt++);   /* Get end of range */
            if (prevChar < expCh) { /* %[a-z] */
                last = expCh;
            } else {
                prevChar = expCh;
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
                /* For %[z-a] */
                last = prevChar;
#else
                SecBracketSetBit(bracketTable->table, SECUREC_CHAR('-'));
                SecBracketSetBit(bracketTable->table, expCh);
                continue;
#endif
            }
            /* Format %[a-\xff] last is 0xFF, condition (rnch <= last) cause dead loop */
            for (expCh = prevChar; expCh < last; ++expCh) {
                SecBracketSetBit(bracketTable->table, expCh);
            }
            SecBracketSetBit(bracketTable->table, last);
            prevChar = 0;
        }
    }
    *format = fmt;
    return 0;
}

#ifdef SECUREC_FOR_WCHAR
SECUREC_INLINE int SecInputForWchar(SecScanSpec *spec)
{
    void *endPtr = spec->argPtr;
    if (spec->isWCharOrLong > 0) {
        *(wchar_t UNALIGNED *)endPtr = (wchar_t)spec->ch;
        endPtr = (wchar_t *)endPtr + 1;
        --spec->arrayWidth;
    } else {
#if SECUREC_HAVE_WCTOMB
        int temp;
        char tmpBuf[SECUREC_MB_LEN + 1];
        SECUREC_MASK_MSVC_CRT_WARNING temp = wctomb(tmpBuf, (wchar_t)spec->ch);
        SECUREC_END_MASK_MSVC_CRT_WARNING
        if (temp <= 0 || (size_t)(unsigned int)temp > sizeof(tmpBuf)) {
            /* If wctomb  error, then ignore character */
            return 0;
        }
        if (((size_t)(unsigned int)temp) > spec->arrayWidth) {
            return -1;
        }
        if (memcpy_s(endPtr, spec->arrayWidth, tmpBuf, (size_t)(unsigned int)temp) != EOK) {
            return -1;
        }
        endPtr = (char *)endPtr + temp;
        spec->arrayWidth -= (size_t)(unsigned int)temp;
#else
        return -1;
#endif
    }
    spec->argPtr = endPtr;
    return 0;
}
#endif

#ifndef SECUREC_FOR_WCHAR
#if SECUREC_HAVE_WCHART
SECUREC_INLINE wchar_t SecConvertInputCharToWchar(SecScanSpec *spec, SecFileStream *stream)
{
    wchar_t tempWChar = L'?';   /* Set default char is ? */
#if SECUREC_HAVE_MBTOWC
    char temp[SECUREC_MULTI_BYTE_MAX_LEN + 1];
    temp[0] = (char)spec->ch;
    temp[1] = '\0';
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    if (SecIsLeadByte(spec->ch)) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        temp[1] = (char)spec->ch;
        temp[2] = '\0'; /* 2 of string terminator position */
    }
    if (mbtowc(&tempWChar, temp, sizeof(temp)) <= 0) {
        /* No string termination error for tool */
        tempWChar = L'?';
    }
#else
    if (SecIsLeadByte(spec->ch)) {
        int convRes = 0;
        int di = 1;
        /* On Linux like system, the string is encoded in UTF-8 */
        while (convRes <= 0 && di < (int)MB_CUR_MAX && di < SECUREC_MULTI_BYTE_MAX_LEN) {
            spec->ch = SecGetChar(stream, &(spec->charCount));
            temp[di++] = (char)spec->ch;
            temp[di] = '\0';
            convRes = mbtowc(&tempWChar, temp, sizeof(temp));
        }
        if (convRes <= 0) {
            tempWChar = L'?';
        }
    } else {
        if (mbtowc(&tempWChar, temp, sizeof(temp)) <= 0) {
            tempWChar = L'?';
        }
    }
#endif
#else
    (void)spec;      /* To clear e438 last value assigned not used , the compiler will optimize this code */
    (void)stream;    /* To clear e438 last value assigned not used , the compiler will optimize this code */
#endif /* SECUREC_HAVE_MBTOWC */

    return tempWChar;
}
#endif /* SECUREC_HAVE_WCHART */


SECUREC_INLINE int SecInputForChar(SecScanSpec *spec, SecFileStream *stream)
{
    void *endPtr = spec->argPtr;
    if (spec->isWCharOrLong > 0) {
#if SECUREC_HAVE_WCHART
        *(wchar_t UNALIGNED *)endPtr = SecConvertInputCharToWchar(spec, stream);
        endPtr = (wchar_t *)endPtr + 1;
        --spec->arrayWidth;
#else
    (void)stream;    /* To clear e438 last value assigned not used , the compiler will optimize this code */
    return -1;
#endif
    } else {
        *(char *)endPtr = (char)spec->ch;
        endPtr = (char *)endPtr + 1;
        --spec->arrayWidth;
    }
    spec->argPtr = endPtr;
    return 0;
}
#endif

/*
 * Scan digital part of %d %i %o %u %x %p.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputNumberDigital(SecFileStream *stream, SecScanSpec *spec)
{
    int loopFlag = 0;
    int started = -1;
    while (loopFlag == 0) {
        /* Decode ch to number */
        loopFlag = SecDecodeNumber(spec);
        if (loopFlag == 0) {
            started = 0;
            if (spec->widthSet != 0 && --spec->width == 0) {
                loopFlag = 1;
            } else {
                spec->ch = SecGetChar(stream, &(spec->charCount));
            }
        } else {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
        }
    }
    /* Handling integer negative numbers and beyond max */
    (*g_secFinishNumber[spec->isInt64Arg])(spec);
    return started;

}

/*
 * Scan %d %i %o %u %x %p.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputNumber(SecFileStream *stream, SecScanSpec *spec)
{
    spec->ch = SecGetChar(stream, &(spec->charCount));

    if (spec->ch == SECUREC_CHAR('+') || spec->ch == SECUREC_CHAR('-')) {
        if (spec->ch == SECUREC_CHAR('-')) {
            spec->negative = 1;
#if SECUREC_IN_KERNEL
            if (spec->convChr == SECUREC_CHAR('x') ||
                spec->convChr == SECUREC_CHAR('o') ||
                spec->convChr == SECUREC_CHAR('u')) {
                /* In kernel Refuse to enter negative number */
                return -1;
            }
#endif
        }
        if (spec->widthSet != 0 && --spec->width == 0) {
            return -1;
        } else {
            spec->ch = SecGetChar(stream, &(spec->charCount));
        }
    }

    if (spec->oriConvChr == SECUREC_CHAR('i')) {
        /* The i could be d, o, or x, use d as default */
        spec->convChr = SECUREC_CHAR('d');
    }

    if (spec->oriConvChr == SECUREC_CHAR('x') || spec->oriConvChr == SECUREC_CHAR('i')) {
        if (spec->ch != SECUREC_CHAR('0')) {
            /* Scan number */
            return SecInputNumberDigital(stream, spec);
        }
        /* Now input string may be 0x123 or 0X123 or just 0 */
        /* Get next char */
        spec->ch = SecGetChar(stream, &(spec->charCount));
        if ((SecChar)spec->ch == SECUREC_CHAR('x') || (SecChar)spec->ch == SECUREC_CHAR('X')) {
            spec->convChr = SECUREC_CHAR('x');
            spec->ch = SecGetChar(stream, &(spec->charCount));
            /* Length of 0x is 2 */
            if (spec->widthSet != 0 && spec->width <= (1 + 1)) {
                /* Length not enough for "0x" */
                return -1;
            }
            spec->width -= 2; /* Subtract 2 for the length of "0x" */
        } else {
            if (spec->oriConvChr != SECUREC_CHAR('x')) {
                spec->convChr = SECUREC_CHAR('o');
            }
            /* Unset the character after 0 back to stream, input only '0' result is OK */
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            spec->ch = SECUREC_CHAR('0');
        }
    }
    /* Scan number */
    return SecInputNumberDigital(stream, spec);
}

/*
 * Scan %c %s %[
 * Return 0 OK
 */
SECUREC_INLINE int SecInputString(SecFileStream *stream, SecScanSpec *spec,
    const SecBracketTable *bracketTable, int *doneCount)
{
    void *startPtr = spec->argPtr;
    int suppressed = 0;
    int errNoMem = 0;

    while (spec->widthSet == 0 || spec->width-- != 0) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        /*
         * The char condition or string condition and bracket condition.
         * Only supports wide characters with a maximum length of two bytes
         */
        if (spec->ch != SECUREC_EOF && (SecCanInputCharacter(spec->convChr) ||
            SecCanInputString(spec->convChr, spec->ch) ||
            SecCanInputForBracket(spec->convChr, spec->ch, bracketTable))) {
            if (spec->suppress != 0) {
                /* Used to identify processed data for %*, use argPtr to identify will cause 613, so use suppressed */
                suppressed = 1;
                continue;
            }
            /* Now suppress is not set */
            if (spec->arrayWidth == 0) {
                errNoMem = 1; /* We have exhausted the user's buffer */
                break;
            }
#ifdef SECUREC_FOR_WCHAR
            errNoMem = SecInputForWchar(spec);
#else
            errNoMem = SecInputForChar(spec, stream);
#endif
            if (errNoMem != 0) {
                break;
            }
        } else {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            break;
        }
    }

    if (errNoMem != 0) {
        /* In case of error, blank out the input buffer */
        SecAddEndingZero(startPtr, spec);
        return -1;
    }

    /* No input was scanned */
    if ((spec->suppress != 0 && suppressed == 0) ||
        (spec->suppress == 0 && startPtr == spec->argPtr)) {
        return -1;
    }

    if (spec->convChr != 'c') {
        /* Add null-terminate for strings */
        SecAddEndingZero(spec->argPtr, spec);
    }
    if (spec->suppress == 0) {
        *doneCount = *doneCount + 1;
    }
    return 0;
}

#ifdef SECUREC_FOR_WCHAR
/*
 * Alloce buffer for wchar version of %[.
 * Return 0 OK
 */
SECUREC_INLINE int SecAllocBracketTable(SecBracketTable *bracketTable)
{
    if (bracketTable->table == NULL) {
        /* Table should be freed after use */
        bracketTable->table = (unsigned char *)SECUREC_MALLOC(SECUREC_BRACKET_TABLE_SIZE);
        if (bracketTable->table == NULL) {
            return -1;
        }
    }
    return 0;
}

/*
 * Free buffer for wchar version of %[
 */
SECUREC_INLINE void SecFreeBracketTable(SecBracketTable *bracketTable)
{
    if (bracketTable->table != NULL) {
        SECUREC_FREE(bracketTable->table);
        bracketTable->table = NULL;
    }
}
#endif

#ifdef SECUREC_FOR_WCHAR
/*
 *  Formatting input core functions for wchar version.Called by a function such as vswscanf_s
 */
int SecInputSW(SecFileStream *stream, const wchar_t *cFormat, va_list argList)
#else
/*
 * Formatting input core functions for char version.Called by a function such as vsscanf_s
 */
int SecInputS(SecFileStream *stream, const char *cFormat, va_list argList)
#endif
{
    const SecUnsignedChar *format = (const SecUnsignedChar *)cFormat;
    SecBracketTable bracketTable = SECUREC_INIT_BRACKET_TABLE;
    SecScanSpec spec;
    int doneCount = 0;
    int formatError = 0;
    int paraIsNull = 0;
    int match = 0; /* When % is found , inc this value */
    int errRet = 0;
#if SECUREC_ENABLE_SCANF_FLOAT
    SecFloatSpec floatSpec;
    SecInitFloatSpec(&floatSpec);
#endif
    spec.ch = 0;
    spec.charCount = 0;

    /* Format must not NULL, use err < 1 to claer 845 */
    while (errRet < 1 && *format != SECUREC_CHAR('\0')) {
        /* Skip space in format and space in input */
        if (SecIsSpace((SecInt)(int)(*format))) {
            /* Read first no space char */
            spec.ch = SecSkipSpaceChar(stream, &(spec.charCount));
            if (spec.ch == SECUREC_EOF) {
                break;
            }
            /* Put fist no space char backup */
            SecUnGetChar(spec.ch, stream, &(spec.charCount));
            SecSkipSpaceFormat(&format);
            continue;
        }

        if (*format != SECUREC_CHAR('%')) {
            spec.ch = SecGetChar(stream, &(spec.charCount));
            if ((int)(*(format++)) != (int)(spec.ch)) {
                SecUnGetChar(spec.ch, stream, &(spec.charCount));
                break;
            }
#if !defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_VERSION)
            if (SecIsLeadByte(spec.ch) && SecDecodeLeadByte(&spec, &format, stream) != 0) {
                break;
            }
#endif

            if (SECUREC_MEET_EOF_BEFORE_NEXT_N(spec.ch, format)) {
                break;
            }
            continue;
        }

        /* Now *format is % */
        /* Set default value for each % */
        SecSetDefaultScanSpec(&spec);
        if (SecDecodeScanFlag(&format, &spec) != 0) {
            formatError = 1;
            ++errRet;
            continue;
        }
        /* Update wchar flag for %S %C */
        SecUpdateWcharFlagByType(*format, &spec);

        if (spec.widthSet != 0 && spec.width == 0) {
            /* 0 width in format */
            ++errRet;
            continue;
        }

        spec.convChr = (unsigned char)(*format) | (SECUREC_CHAR('a') - SECUREC_CHAR('A')); /* To lowercase */
        spec.oriConvChr = spec.convChr;

        if (spec.convChr != SECUREC_CHAR('n')) {
            if (spec.convChr != SECUREC_CHAR('c') && spec.convChr != SECUREC_BRACE) {
                spec.ch = SecSkipSpaceChar(stream, &(spec.charCount));
            } else {
                spec.ch = SecGetChar(stream, &(spec.charCount));
            }
            if (spec.ch == SECUREC_EOF) {
                ++errRet;
                continue;
            }
        }

        /* Now no 0 width in format and get one char from input */
        switch (spec.convChr) {
            case SECUREC_CHAR('c'): /* Also 'C' */
                /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('s'): /* Also 'S': */
                /* fall-through */ /* FALLTHRU */
            case SECUREC_BRACE:
                /* Check dest buffer and size */
                if (spec.suppress == 0) {
                    spec.argPtr = (void *)va_arg(argList, void *);
                    if (spec.argPtr == NULL) {
                        paraIsNull = 1;
                        ++errRet;
                        continue;
                    }
                    /* Get the next argument - size of the array in characters */
#ifdef SECUREC_ON_64BITS
                    spec.arrayWidth = ((size_t)(va_arg(argList, size_t))) & 0xFFFFFFFFUL;
#else /* !SECUREC_ON_64BITS */
                    spec.arrayWidth = (size_t)va_arg(argList, size_t);
#endif
                    if (SECUREC_ARRAY_WIDTH_IS_WRONG(spec)) {
                        /* Do not clear buffer just go error */
                        ++errRet;
                        continue;
                    }
                    /* One element is needed for '\0' for %s and %[ */
                    if (spec.convChr != SECUREC_CHAR('c')) {
                        --spec.arrayWidth;
                    }
                } else {
                    /* Set argPtr to  NULL  is necessary, in supress mode we don't use argPtr to store data */
                    spec.argPtr = NULL;
                }

                if (spec.convChr == 'c') {
                    if (spec.widthSet == 0) {
                        spec.widthSet = 1;
                        spec.width = 1;
                    }
                } else if (spec.convChr == SECUREC_BRACE) {
                    /* Malloc when first %[ is meet  for wchar version */
#ifdef SECUREC_FOR_WCHAR
                    if (SecAllocBracketTable(&bracketTable) != 0) {
                        ++errRet;
                        continue;
                    }
#endif
                    (void)memset(bracketTable.table, 0, (size_t)SECUREC_BRACKET_TABLE_SIZE);
                    if (SecSetupBracketTable(&format, &bracketTable) != 0) {
                        ++errRet;
                        continue;
                    }

                    if (*format == SECUREC_CHAR('\0')) {
                        /* Default add string terminator */
                        SecAddEndingZero(spec.argPtr, &spec);
                        ++errRet;
                        /* Truncated format */
                        continue;
                    }
                }
                /* Unset last char to stream */
                SecUnGetChar(spec.ch, stream, &(spec.charCount));
                /* Set completed.  Now read string */
                if (SecInputString(stream, &spec, &bracketTable, &doneCount) != 0) {
                    ++errRet;
                    continue;
                }
                break;
            case SECUREC_CHAR('p'):
                /* Make %hp same as %p */
                spec.numberWidth = SECUREC_NUM_WIDTH_INT;
#ifdef SECUREC_ON_64BITS
                spec.isInt64Arg = 1;
#endif
                /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('o'):    /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('u'):    /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('d'):    /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('i'):    /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('x'):
                /* Unset last char to stream */
                SecUnGetChar(spec.ch, stream, &(spec.charCount));
                if (SecInputNumber(stream, &spec) != 0) {
                    ++errRet;
                    continue;
                }
                if (spec.suppress == 0) {
                    spec.argPtr = (void *)va_arg(argList, void *);
                    if (spec.argPtr == NULL) {
                        paraIsNull = 1;
                        ++errRet;
                        continue;
                    }
                    SecAssignNumber(&spec);
                    ++doneCount;
                }
                break;
            case SECUREC_CHAR('n'):    /* Char count */
                if (spec.suppress == 0) {
                    spec.argPtr = (void *)va_arg(argList, void *);
                    if (spec.argPtr == NULL) {
                        paraIsNull = 1;
                        ++errRet;
                        continue;
                    }
                    spec.number = (unsigned long)(unsigned int)(spec.charCount);
                    spec.isInt64Arg = 0;
                    SecAssignNumber(&spec);
                }
                break;
            case SECUREC_CHAR('e'):    /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('f'):    /* fall-through */ /* FALLTHRU */
            case SECUREC_CHAR('g'):    /* Scan a float */
#if SECUREC_ENABLE_SCANF_FLOAT
                /* Unset last char to stream */
                SecUnGetChar(spec.ch, stream, &(spec.charCount));
                if (SecInputFloat(stream, &spec, &floatSpec) != 0) {
                    ++errRet;
                    continue;
                }
                if (spec.suppress == 0) {
                    spec.argPtr = (void *)va_arg(argList, void *);
                    if (spec.argPtr == NULL) {
                        ++errRet;
                        paraIsNull = 1;
                        continue;
                    }
#ifdef SECUREC_FOR_WCHAR
                    if (SecAssignFloatW(&floatSpec, &spec) != 0) {
                        ++errRet;
                        continue;
                    }
#else
                    SecAssignFloat(floatSpec.floatStr, spec.numberWidth, spec.argPtr);
#endif
                    ++doneCount;
                }

                break;
#else /* SECUREC_ENABLE_SCANF_FLOAT */
                ++errRet;
                continue;
#endif
            default:
                if ((int)(*format) != (int)spec.ch) {
                    SecUnGetChar(spec.ch, stream, &(spec.charCount));
                    formatError = 1;
                    ++errRet;
                    continue;
                } else {
                    --match; /*  Compensate for the self-increment of the following code */
                }
        }

        ++match;
        ++format;

        if (SECUREC_MEET_EOF_BEFORE_NEXT_N(spec.ch, format)) {
            break;
        }
    }

#ifdef SECUREC_FOR_WCHAR
    SecFreeBracketTable(&bracketTable);
#endif

#if SECUREC_ENABLE_SCANF_FLOAT
    SecFreeFloatSpec(&floatSpec, &doneCount);
#endif

#if SECUREC_ENABLE_SCANF_FILE
    SecAdjustStream(stream);
#endif

    if (spec.ch == SECUREC_EOF) {
        return ((doneCount || match) ? doneCount : SECUREC_SCANF_EINVAL);
    } else if (formatError != 0 || paraIsNull != 0) {
        /* Invalid Input Format or parameter */
        return SECUREC_SCANF_ERROR_PARA;
    }

    return doneCount;
}

#if SECUREC_ENABLE_SCANF_FILE

#if defined(SECUREC_NO_STD_UNGETC)
/*
 *  Get char  from stream or buffer
 */
SECUREC_INLINE SecInt SecGetCharFromStream(SecFileStream *stream)
{
    SecInt ch;
    if (stream->fUnGet == 1) {
        ch = (SecInt) stream->lastChar;
        stream->fUnGet = 0;
    } else {
        ch = SECUREC_GETC(stream->pf);
        stream->lastChar = (unsigned int)ch;
    }
    return ch;
}
#else
/*
 *  Get char from stream use std function
 */
SECUREC_INLINE SecInt SecGetCharFromStream(const SecFileStream *stream)
{
    SecInt ch;
    ch = SECUREC_GETC(stream->pf);
    return ch;
}
#endif

/*
 * Make data is aligned to SecChar size
 */
SECUREC_INLINE void SecMakeDataIsAligned(SecFileStream *stream)
{
    int remainder = stream->count % (int)sizeof(SecChar);
    if (remainder != 0) {
        int needLen = (int)sizeof(SecChar) - remainder;
        int len = (int)fread(stream->base + stream->count, (size_t)1, (size_t)(unsigned int)needLen, stream->pf);
        if (len > 0 && len <= needLen) {
            /* When encountering the end of a file, the read length is less than needLen */
            stream->count += len;
        }
    }
}

/*
 * Try to read the BOM header, when meet a BOM head, discard it
 */
SECUREC_INLINE void SecReadAndSkipBomHeader(SecFileStream *stream)
{
    int bomHeadSize;
#ifdef SECUREC_FOR_WCHAR
    bomHeadSize = SECUREC_BOM_HEADER_SIZE;
#else
    bomHeadSize = SECUREC_UTF8_BOM_HEADER_SIZE;
#endif
    stream->count = (int)fread(stream->base, (size_t)1, (size_t)(unsigned int)bomHeadSize, stream->pf);
    if (stream->count < 0 || stream->count > bomHeadSize) {
        stream->count = 0;
    }
#ifdef SECUREC_FOR_WCHAR
    if (stream->count >= SECUREC_BOM_HEADER_SIZE &&
        (((unsigned char)(stream->base[0]) == SECUREC_BOM_HEADER_LE_1ST &&
        (unsigned char)(stream->base[1]) == SECUREC_BOM_HEADER_LE_2ST) ||
        ((unsigned char)(stream->base[0]) == SECUREC_BOM_HEADER_BE_1ST &&
        (unsigned char)(stream->base[1]) == SECUREC_BOM_HEADER_BE_2ST))) {
        /* It's BOM header, discard it */
        stream->count = 0;
    }
#else
    if (stream->count >= SECUREC_UTF8_BOM_HEADER_SIZE &&
        (unsigned char)(stream->base[0]) == SECUREC_UTF8_BOM_HEADER_1ST &&
        (unsigned char)(stream->base[1]) == SECUREC_UTF8_BOM_HEADER_2ND &&
        (unsigned char)(stream->base[2]) == SECUREC_UTF8_BOM_HEADER_3RD) { /* 2 offset of third head character */
        /* It's BOM header, discard it */
        stream->count = 0;
    }
#endif
    SecMakeDataIsAligned(stream);
}

/*
 *  Get char  from file stream or buffer
 */
SECUREC_INLINE SecInt SecGetCharFromFile(SecFileStream *stream)
{
    SecInt ch;
    if (stream->count == 0) {
        int len;
        /* Load file to buffer */
        if (stream->base == NULL) {
            stream->oriFilePos = ftell(stream->pf);   /* Save original file read position */
            if (stream->oriFilePos == -1) {
                /* It may be a pipe stream */
                stream->flag = SECUREC_PIPE_STREAM_FLAG;
                return SecGetCharFromStream(stream);
            }
            /* Reserve the length of BOM head */
            stream->base = (char *)SECUREC_MALLOC(SECUREC_BUFFERED_BLOK_SIZE +
                SECUREC_BOM_HEADER_SIZE + SECUREC_UTF8_BOM_HEADER_SIZE);
            if (stream->base == NULL) {
                return SECUREC_EOF;
            }
            /* First read file */
            if (stream->oriFilePos == 0) {
                SecReadAndSkipBomHeader(stream);
            }
        }
        /* SecReadAndSkipBomHeader has read part of the data, so add offset */
        len = (int)fread(stream->base + stream->count,
            (size_t)1, (size_t)SECUREC_BUFFERED_BLOK_SIZE, stream->pf);
        if (len < 0 || len > SECUREC_BUFFERED_BLOK_SIZE) {
            len = 0;
        }
        stream->count += len;
        stream->cur = stream->base;
        stream->flag |= SECUREC_LOAD_FILE_TO_MEM_FLAG;
        stream->base[stream->count] = '\0';   /* For tool Warning string null */
    }
    /* According wchar_t has two bytes */
    stream->count -= (int)sizeof(SecChar);
    ch = (SecInt)((stream->count) >= 0 ? \
                  (SecInt)(SECUREC_CHAR_MASK & \
                  (unsigned int)(int)(*((const SecChar *)(const void *)stream->cur))) : SECUREC_EOF);
    stream->cur += sizeof(SecChar); /* Pointer may be out of bounds, but overread does not occur */

    if (ch != SECUREC_EOF && stream->base != NULL) {
        stream->fileRealRead += (int)sizeof(SecChar);
    }
    return ch;
}
#endif

/*
 *  Get char  for wchar version
 */
SECUREC_INLINE SecInt SecGetChar(SecFileStream *stream, int *counter)
{
    SecInt ch = SECUREC_EOF;
#if SECUREC_ENABLE_SCANF_FILE
    if ((stream->flag & SECUREC_PIPE_STREAM_FLAG) > 0) {
        ch = SecGetCharFromStream(stream);
    } else if ((stream->flag & SECUREC_FILE_STREAM_FLAG) > 0) {
        ch = SecGetCharFromFile(stream);
    }
#endif
    if ((stream->flag & SECUREC_MEM_STR_FLAG) > 0) {
        /* According wchar_t has two bytes */
        stream->count -= (int)sizeof(SecChar);
        ch = (SecInt)((stream->count >= 0) ? \
                      (SecInt)(SECUREC_CHAR_MASK & \
                      (unsigned int)(int)(*((const SecChar *)(const void *)stream->cur))) : SECUREC_EOF);
        stream->cur += sizeof(SecChar);
    }
    *counter = *counter + 1;
    return ch;
}

/*
 *  Unget Public realizatio char for wchar and char version
 */
SECUREC_INLINE void SecUnGetCharImpl(SecInt ch, SecFileStream *stream)
{
    if ((stream->flag & SECUREC_PIPE_STREAM_FLAG) > 0) {
#if SECUREC_ENABLE_SCANF_FILE
#if defined(SECUREC_NO_STD_UNGETC)
        stream->lastChar = (unsigned int)ch;
        stream->fUnGet = 1;
#else
        (void)SECUREC_UN_GETC(ch, stream->pf);
#endif
#else
        (void)ch; /* To clear e438 last value assigned not used , the compiler will optimize this code */
#endif
    } else if ((stream->flag & SECUREC_MEM_STR_FLAG) || (stream->flag & SECUREC_LOAD_FILE_TO_MEM_FLAG) > 0) {
        if (stream->cur > stream->base) {
            stream->cur -= sizeof(SecChar);
            stream->count += (int)sizeof(SecChar);
        }
    }
#if SECUREC_ENABLE_SCANF_FILE
    if ((stream->flag & SECUREC_FILE_STREAM_FLAG) > 0 && stream->base) {
        stream->fileRealRead -= (int)sizeof(SecChar);
    }
#endif
}

/*
 *  Unget char  for char version
 */
SECUREC_INLINE void SecUnGetChar(SecInt ch, SecFileStream *stream, int *counter)
{
    if (ch != SECUREC_EOF) {
        SecUnGetCharImpl(ch, stream);
    }
    *counter = *counter - 1;
}

/*
 *  Skip space char by isspace
 */
SECUREC_INLINE SecInt SecSkipSpaceChar(SecFileStream *stream, int *counter)
{
    SecInt ch;
    do {
        ch = SecGetChar(stream, counter);
    } while (ch != SECUREC_EOF && SecIsSpace(ch));
    return ch;
}
#endif /* __INPUT_INL__5D13A042_DC3F_4ED9_A8D1_882811274C27 */

