/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: Used by secureprintoutput_a.c and secureprintoutput_w.c to include.
 *              This file provides a template function for ANSI and UNICODE compiling
 *              by different type definition. The functions of SecOutputS or
 *              SecOutputSW  provides internal implementation for printf family API, such as sprintf, swprintf_s.
 * Author: lishunda
 * Create: 2014-02-25
 * Notes: see www.cplusplus.com/reference/cstdio/printf/
 * History: 2019-04-03 zhaozhijian Code base quality improvement
 */
/*
 * [Standardize-exceptions] Use unsafe function: Portability
 * [reason] Use unsafe function to implement security function to maintain platform compatibility.
 *          And sufficient input validation is performed before calling
 */

#ifndef OUTPUT_INL_2B263E9C_43D8_44BB_B17A_6D2033DECEE5
#define OUTPUT_INL_2B263E9C_43D8_44BB_B17A_6D2033DECEE5

#define SECUREC_NULL_STRING_SIZE            8
#define SECUREC_STATE_TABLE_SIZE              337
#define SECUREC_OFFSET_BITS_WORD            16
#define SECUREC_OFFSET_BITS_DWORD           32

#define SECUREC_OFFSET_DIV_OCTAL            3
#define SECUREC_OFFSET_DIV_HEX              4
#define SECUREC_RADIX_OCTAL                 8
#define SECUREC_RADIX_DECIMAL               10
#define SECUREC_RADIX_HEX                   16
#define SECUREC_PREFIX_LEN                  2
/* Size include '+' and '\0' */
#define SECUREC_FLOAT_BUF_EXT               2

typedef union {
    /* Integer formatting refers to the end of the buffer, plus 1 to prevent tool alarms */
    char str[SECUREC_BUFFER_SIZE + 1];
#if SECUREC_HAVE_WCHART
    wchar_t wStr[SECUREC_WCHAR_BUFFER_SIZE]; /* Just for %lc */
#endif
} SecBuffer;

typedef union {
    char *str;                  /* Not a null terminated  string */
#if SECUREC_HAVE_WCHART
    wchar_t *wStr;
#endif
} SecFormatBuf;

typedef struct {
    const char *digits;                 /* Point to the hexadecimal subset */
    SecFormatBuf text;                  /* Point to formated string */
    int textLen;                        /* Length of the text */
    int textIsWide;                     /* Flag for text is wide chars ; 0 is not wide char */
    unsigned int radix;                 /* Use for output number , default set to 10 */
    unsigned int flags;
    int fldWidth;
    int precision;
    int dynWidth;                       /* %*   1 width from variable parameter ;0 not */
    int dynPrecision;                   /* %.*  1 precision from variable parameter ;0 not */
    int padding;                        /* Padding len */
    int prefixLen;                      /* Length of prefix, 0 or 1 or 2 */
    SecChar prefix[SECUREC_PREFIX_LEN]; /* Prefix is  0 or 0x */
    SecBuffer buffer;
} SecFormatAttr;

#if SECUREC_ENABLE_SPRINTF_FLOAT
#ifdef SECUREC_STACK_SIZE_LESS_THAN_1K
#define SECUREC_FMT_STR_LEN                 8
#else
#define SECUREC_FMT_STR_LEN                 16
#endif
typedef struct {
    char buffer[SECUREC_FMT_STR_LEN];
    char *fmtStr;                     /* Initialization must point to buffer */
    char *allocatedFmtStr;            /* Initialization must be NULL  to store alloced point */
    char *floatBuffer;                /* Use heap memory if the SecFormatAttr.buffer is not enough */
    int bufferSize;                   /* The size of floatBuffer */
} SecFloatAdapt;
#endif




static const char *g_itoaUpperDigits = "0123456789ABCDEFX";
static const char *g_itoaLowerDigits = "0123456789abcdefx";
static const unsigned char g_stateTable[SECUREC_STATE_TABLE_SIZE] = {
    /*
     * Type
     * 0:    nospecial meanin;
     * 1:    '%'
     * 2:    '.'
     * 3:    '*'
     * 4:    '0'
     * 5:    '1' ... '9'
     * 6:    ' ', '+', '-', '#'
     * 7:    'h', 'l', 'L', 'w' , 'N', 'z', 'q', 't', 'j'
     * 8:    'd', 'o', 'u', 'i', 'x', 'X', 'e', 'f', 'g', 'E', 'F', 'G', 's', 'c', '[', 'p'
     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x06, 0x02, 0x00,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x00, 0x07, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x07, 0x08, 0x07, 0x00, 0x07, 0x00, 0x00, 0x08,
    0x08, 0x07, 0x00, 0x08, 0x07, 0x08, 0x00, 0x07, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* Fill zero  for normal char 128 byte for 0x80 - 0xff */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*
     * State
     * 0: normal
     * 1: percent
     * 2: flag
     * 3: width
     * 4: dot
     * 5: precis
     * 6: size
     * 7: type
     * 8: invalid
     */
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x01, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x01, 0x00, 0x00, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x03, 0x03, 0x08, 0x05,
    0x08, 0x08, 0x00, 0x00, 0x00, 0x02, 0x02, 0x03, 0x05, 0x05, 0x08, 0x00, 0x00, 0x00, 0x03, 0x03,
    0x03, 0x05, 0x05, 0x08, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00,
    0x00
};

#if SECUREC_ENABLE_SPRINTF_FLOAT
/* Call system sprintf to format float value */
SECUREC_INLINE int SecFormatFloat(char *strDest, const char *format, ...)
{
    int ret;                    /* If initialization causes  e838 */
    va_list argList;

    va_start(argList, format);
    SECUREC_MASK_MSVC_CRT_WARNING
    ret = vsprintf(strDest, format, argList);
    SECUREC_END_MASK_MSVC_CRT_WARNING
    va_end(argList);
    (void)argList; /* To clear e438 last value assigned not used , the compiler will optimize this code */

    return ret;
}

#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
/* Out put long double value to dest */
SECUREC_INLINE void SecFormatLongDboule(SecFormatAttr *attr, const SecFloatAdapt *floatAdapt, long double ldValue)
{
    int fldWidth = ((attr->flags & SECUREC_FLAG_LEFT) ? (-attr->fldWidth) : attr->fldWidth);
    if (attr->dynWidth && attr->dynPrecision) {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, fldWidth, attr->precision, ldValue);
    } else if (attr->dynWidth) {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, fldWidth, ldValue);
    } else if (attr->dynPrecision) {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, attr->precision, ldValue);
    } else {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, ldValue);
    }
    if (attr->textLen < 0 || attr->textLen >= floatAdapt->bufferSize) {
        attr->textLen = 0;
    }
}
#endif

/* Out put double value to dest */
SECUREC_INLINE void SecFormatDboule(SecFormatAttr *attr, const SecFloatAdapt *floatAdapt, double dValue)
{
    int fldWidth = ((attr->flags & SECUREC_FLAG_LEFT) ? (-attr->fldWidth) : attr->fldWidth);
    if (attr->dynWidth && attr->dynPrecision) {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, fldWidth, attr->precision, dValue);
    } else if (attr->dynWidth) {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, fldWidth, dValue);
    } else if (attr->dynPrecision) {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, attr->precision, dValue);
    } else {
        attr->textLen = SecFormatFloat(attr->text.str, floatAdapt->fmtStr, dValue);
    }
    if (attr->textLen < 0 || attr->textLen >= floatAdapt->bufferSize) {
        attr->textLen = 0;
    }
}
#endif

#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
/* To clear e506 warning */
SECUREC_INLINE int SecIsSameSize(size_t sizeA, size_t sizeB)
{
    return sizeA == sizeB;
}
#endif


#ifndef SECUREC_ON_64BITS
/*
 * Compiler Optimized Division 8.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber32ToOctalString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val32 % SECUREC_RADIX_OCTAL];
    } while ((val32 /= SECUREC_RADIX_OCTAL) != 0);
}

#ifdef _AIX
/*
 * Compiler Optimized Division 10.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber32ToDecString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val32 % SECUREC_RADIX_DECIMAL];
    } while ((val32 /= SECUREC_RADIX_DECIMAL) != 0);
}
#endif
/*
 * Compiler Optimized Division 16.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber32ToHexString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        --attr->text.str;
        *(attr->text.str) = attr->digits[val32 % SECUREC_RADIX_HEX];
    } while ((val32 /= SECUREC_RADIX_HEX) != 0);
}

#ifndef _AIX
/* Use fast div 10 */
SECUREC_INLINE void SecNumber32ToDecStringFast(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        SecUnsignedInt32 quotient;
        SecUnsignedInt32 remain;
        --attr->text.str;
        *(attr->text.str) = g_itoaLowerDigits[val32 % SECUREC_RADIX_DECIMAL];
        quotient = (val32 >> 1) + (val32 >> 2); /* Fast div  magic 2 */
        quotient = quotient + (quotient >> 4); /* Fast div  magic 4 */
        quotient = quotient + (quotient >> 8); /* Fast div  magic 8 */
        quotient = quotient + (quotient >> 16); /* Fast div  magic 16 */
        quotient = quotient >> 3; /* Fast div  magic 3 */
        remain = val32 - SECUREC_MUL_TEN(quotient);
        val32 = (remain > 9) ? (quotient + 1) : quotient; /* Fast div  magic 9 */
    } while (val32 != 0);
}
#endif

SECUREC_INLINE void SecNumber32ToString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    switch (attr->radix) {
        case SECUREC_RADIX_HEX:
            SecNumber32ToHexString(number, attr);
            break;
        case SECUREC_RADIX_OCTAL:
            SecNumber32ToOctalString(number, attr);
            break;
        case SECUREC_RADIX_DECIMAL:
#ifdef _AIX
            /* The compiler will optimize div 10 */
            SecNumber32ToDecString(number, attr);
#else
            SecNumber32ToDecStringFast(number, attr);
#endif
            break;
        default:
            break;
    }
}
#endif

#if defined(SECUREC_USE_SPECIAL_DIV64) || (defined(SECUREC_VXWORKS_VERSION_5_4) && !defined(SECUREC_ON_64BITS))
/*
 * This function just to clear warning, on sume vxworks compiler shift 32 bit make warnigs
 */
SECUREC_INLINE SecUnsignedInt64 SecU64Shr32(SecUnsignedInt64 number)
{
    return (((number) >> 16) >> 16); /* Two shifts of 16 bits to realize shifts of 32 bits */
}
/*
 * Fast divide by 10 algorithm.
 * Calculation divisor multiply  0xcccccccccccccccdULL, resultHi64 >> 3 as quotient
 */
SECUREC_INLINE void SecU64Div10(SecUnsignedInt64 divisor, SecUnsignedInt64 *quotient, SecUnsignedInt32 *remainder)
{
    SecUnsignedInt64 mask = 0xffffffffULL; /* Use 0xffffffffULL as 32 bit mask */
    SecUnsignedInt64 magicHi = 0xccccccccULL; /* Fast divide 10 magic numbers high 32bit 0xccccccccULL */
    SecUnsignedInt64 magicLow = 0xcccccccdULL; /* Fast divide 10 magic numbers low 32bit  0xcccccccdULL */
    SecUnsignedInt64 divisorHi = (SecUnsignedInt64)(SecU64Shr32(divisor)); /* High 32 bit use  */
    SecUnsignedInt64 divisorLow = (SecUnsignedInt64)(divisor & mask); /* Low 32 bit mask */
    SecUnsignedInt64 factorHi = divisorHi * magicHi;
    SecUnsignedInt64 factorLow1 = divisorHi * magicLow;
    SecUnsignedInt64 factorLow2 = divisorLow * magicHi;
    SecUnsignedInt64 factorLow3 = divisorLow * magicLow;
    SecUnsignedInt64 carry = (factorLow1 & mask) + (factorLow2 & mask) + SecU64Shr32(factorLow3);
    SecUnsignedInt64 resultHi64 = factorHi + SecU64Shr32(factorLow1) + SecU64Shr32(factorLow2) + SecU64Shr32(carry);

    *quotient = resultHi64 >> 3; /* Fast divide 10 magic numbers 3 */
    *remainder = (SecUnsignedInt32)(divisor - ((*quotient) * 10)); /* Quotient mul 10 */
    return;
}
#if defined(SECUREC_VXWORKS_VERSION_5_4) && !defined(SECUREC_ON_64BITS)
/*
 * Divide function for VXWORKS
 */
SECUREC_INLINE int SecU64Div32(SecUnsignedInt64 divisor, SecUnsignedInt32 radix,
    SecUnsignedInt64 *quotient, SecUnsignedInt32 *remainder)
{
    switch (radix) {
        case SECUREC_RADIX_DECIMAL:
            SecU64Div10(divisor, quotient, remainder);
            break;
        case SECUREC_RADIX_HEX:
            *quotient = (divisor >> SECUREC_OFFSET_DIV_HEX);
            *remainder = (SecUnsignedInt32)(divisor & 0xfULL); /* Mask one hex number by 0xfULL */
            break;
        case SECUREC_RADIX_OCTAL:
            *quotient = (divisor >> SECUREC_OFFSET_DIV_OCTAL);
            *remainder = (SecUnsignedInt32)(divisor & 0x7ULL); /* Mask one hex number by 0x7ULL */
            break;
        default:
            return -1; /* This does not happen in the current file */
    }
    return 0;
}
SECUREC_INLINE void SecNumber64ToStringSpecial(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        SecUnsignedInt32 digit = 0; /* Ascii value of digit */
        SecUnsignedInt64 quotient = 0;
        if (SecU64Div32(val64, (SecUnsignedInt32)attr->radix, &quotient, &digit) != 0) {
            /* Just break, when enter this function, no error is returned */
            break;
        }
        --attr->text.str;
        *(attr->text.str) = attr->digits[digit];
        val64 = quotient;
    } while (val64 != 0);
}
#endif
#endif

#if defined(SECUREC_ON_64BITS) || !defined(SECUREC_VXWORKS_VERSION_5_4)
#if defined(SECUREC_USE_SPECIAL_DIV64)
/* The compiler does not provide 64 bit division problems */
SECUREC_INLINE void SecNumber64ToDecString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        SecUnsignedInt64 quotient = 0;
        SecUnsignedInt32 digit = 0;
        SecU64Div10(val64, &quotient, &digit);
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[digit];
        val64 = quotient;
    } while (val64 != 0);
}
#else
/*
 * Compiler Optimized Division 10.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber64ToDecString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val64 % SECUREC_RADIX_DECIMAL];
    } while ((val64 /= SECUREC_RADIX_DECIMAL) != 0);
}
#endif

/*
 * Compiler Optimized Division 8.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber64ToOctalString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val64 % SECUREC_RADIX_OCTAL];
    } while ((val64 /= SECUREC_RADIX_OCTAL) != 0);
}
/*
 * Compiler Optimized Division 16.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber64ToHexString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        --attr->text.str;
        *(attr->text.str) = attr->digits[val64 % SECUREC_RADIX_HEX];
    } while ((val64 /= SECUREC_RADIX_HEX) != 0);
}


SECUREC_INLINE void SecNumber64ToString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    switch (attr->radix) {
        /* The compiler will optimize div 10 */
        case SECUREC_RADIX_DECIMAL:
            SecNumber64ToDecString(number, attr);
            break;
        case SECUREC_RADIX_OCTAL:
            SecNumber64ToOctalString(number, attr);
            break;
        case SECUREC_RADIX_HEX:
            SecNumber64ToHexString(number, attr);
            break;
        default:
            break;
    }
}
#endif

/*
 * Converting integers to string
 */
SECUREC_INLINE void SecNumberToString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
#ifdef SECUREC_ON_64BITS
        SecNumber64ToString(number, attr);
#else /* For 32 bits system */
        if (number <= 0xFFFFFFFFUL) {
            /* In most case, the value to be converted is small value */
            SecUnsignedInt32 n32Tmp = (SecUnsignedInt32)number;
            SecNumber32ToString(n32Tmp, attr);
        } else {
            /* The value to be converted is greater than 4G */
#if defined(SECUREC_VXWORKS_VERSION_5_4)
            SecNumber64ToStringSpecial(number, attr);
#else
            SecNumber64ToString(number, attr);
#endif
        }
#endif

}

SECUREC_INLINE int SecIsNumberNeedTo32Bit(const SecFormatAttr *attr)
{
    return (((attr->flags & SECUREC_FLAG_I64) == 0) &&
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
            ((attr->flags & SECUREC_FLAG_INTMAX) == 0) &&
#endif
#ifdef SECUREC_ON_64BITS
            ((attr->flags & SECUREC_FLAG_PTRDIFF) == 0) &&
            ((attr->flags & SECUREC_FLAG_SIZE) == 0) &&
#if !defined(SECUREC_COMPATIBLE_WIN_FORMAT)  /* on window 64 system sizeof long is 32bit */
            ((attr->flags & SECUREC_FLAG_LONG) == 0) &&
#endif
#endif
            ((attr->flags & SECUREC_FLAG_LONGLONG) == 0));
}

SECUREC_INLINE void SecNumberToBuffer(SecFormatAttr *attr, SecInt64 num64)
{
    SecUnsignedInt64 number;
    /* Check for negative; copy into number */
    if ((attr->flags & SECUREC_FLAG_SIGNED) && num64 < 0) {
        number = (SecUnsignedInt64)(-num64);
        attr->flags |= SECUREC_FLAG_NEGATIVE;
    } else {
        number = (SecUnsignedInt64)num64;
    }
    if (SecIsNumberNeedTo32Bit(attr)) {
        number = (number & (SecUnsignedInt64)0xffffffffUL);  /* Use 0xffffffff as 32 bit mask */
    }

    /* The text.str must be point to buffer.str, this pointer is used outside the function */
    attr->text.str = &attr->buffer.str[SECUREC_BUFFER_SIZE];

    if (number == 0) {
        /* Turn off hex prefix default, and textLen is zero */
        attr->prefixLen = 0;
        attr->textLen = 0;
        return;
    }

    /* Convert integer to string. It must be invoked when number > 0, otherwise the following logic is incorrect */
    SecNumberToString(number, attr);
    /* Compute length of number,  text.str must be in buffer.str */
    attr->textLen = (int)(size_t)((char *)&attr->buffer.str[SECUREC_BUFFER_SIZE] - attr->text.str);
}

/* Use loop copy char or wchar_t string */
SECUREC_INLINE void SecWriteStringToStreamOpt(SecPrintfStream *stream, const SecChar *str, int len)
{
    int i;
    const SecChar *tmp = str;
    for (i = 0; i < len; ++i) {
        *((SecChar *)(void *)(stream->cur)) = *(const SecChar *)(tmp);
        stream->cur += sizeof(SecChar);
        tmp = tmp + 1;
    }
    stream->count -= len * (int)(sizeof(SecChar));
}

SECUREC_INLINE void SecWriteStringToStream(SecPrintfStream *stream, const SecChar *str, int len)
{
    if (len < 12) { /* Performance optimization for mobile number length 12 */
        SecWriteStringToStreamOpt(stream, str, len);
    } else {
        size_t count = (size_t)(unsigned int)len * (sizeof(SecChar));
        SECUREC_MEMCPY_WARP_OPT(stream->cur, str, count);
        stream->cur += (size_t)((size_t)(unsigned int)len * (sizeof(SecChar)));
        stream->count -= len * (int)(sizeof(SecChar));
    }
}

/*
 * Return if buffer length is enough
 * The count variable can be reduced to 0, and the external function complements the \0 terminator.
 */
SECUREC_INLINE int SecIsStreamBufEnough(const SecPrintfStream *stream, int needLen)
{
    return ((int)(stream->count - (needLen * (int)(sizeof(SecChar)))) >= 0);
}

/* Write left padding */
SECUREC_INLINE void SecWriteLeftPadding(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if (!(attr->flags & (SECUREC_FLAG_LEFT | SECUREC_FLAG_LEADZERO)) && attr->padding > 0) {
        /* Pad on left with blanks */
        SECUREC_WRITE_MULTI_CHAR(SECUREC_CHAR(' '), attr->padding, stream, charsOut);
    }
}

/* Write prefix */
SECUREC_INLINE void SecWritePrefix(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if (attr->prefixLen > 0) {
        if (SecIsStreamBufEnough(stream, attr->prefixLen)) {
            /* Max prefix len is 2, use loop copy */
            SecWriteStringToStreamOpt(stream, attr->prefix, attr->prefixLen);
            *charsOut += attr->prefixLen;
        } else {
            SECUREC_WRITE_STRING(attr->prefix, attr->prefixLen, stream, charsOut);
        }
    }
}

/* Write leading zeros */
SECUREC_INLINE void SecWriteLeadingZero(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if ((attr->flags & SECUREC_FLAG_LEADZERO) && !(attr->flags & SECUREC_FLAG_LEFT) &&
        attr->padding > 0) {
        SECUREC_WRITE_MULTI_CHAR(SECUREC_CHAR('0'), attr->padding, stream, charsOut);
    }
}

/* Write right padding */
SECUREC_INLINE void SecWriteRightPadding(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if (*charsOut >= 0 && (attr->flags & SECUREC_FLAG_LEFT) && attr->padding > 0) {
        /* Pad on right with blanks */
        SECUREC_WRITE_MULTI_CHAR(SECUREC_CHAR(' '), attr->padding, stream, charsOut);
    }
}

/* Write text string */
SECUREC_INLINE void SecWriteStringChk(SecPrintfStream *stream, const SecChar *str, int len, int *charsOut)
{
    if (SecIsStreamBufEnough(stream, len)) {
        SecWriteStringToStream(stream, str, len);
        *charsOut += len;
    } else {
        SECUREC_WRITE_STRING(str, len, stream, charsOut);
    }
}

#ifdef SECUREC_FOR_WCHAR
#if SECUREC_HAVE_MBTOWC
SECUREC_INLINE void SecWriteTextAfterMbtowc(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    char *p = attr->text.str;
    int count = attr->textLen;
    while (count > 0) {
        wchar_t wChar = L'\0';
        int retVal = mbtowc(&wChar, p, (size_t)MB_CUR_MAX);
        if (retVal <= 0) {
            *charsOut = -1;
            break;
        }
        SecWriteCharW(wChar, stream, charsOut);
        if (*charsOut == -1) {
            break;
        }
        p += retVal;
        count -= retVal;
    }
}
#endif
#else  /* Not SECUREC_FOR_WCHAR */
#if SECUREC_HAVE_WCTOMB
SECUREC_INLINE void SecWriteTextAfterWctomb(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    wchar_t *p = attr->text.wStr;
    int count = attr->textLen;
    while (count > 0) {
        char tmpBuf[SECUREC_MB_LEN + 1];
        SECUREC_MASK_MSVC_CRT_WARNING
        int retVal = wctomb(tmpBuf, *p);
        SECUREC_END_MASK_MSVC_CRT_WARNING
        if (retVal <= 0) {
            *charsOut = -1;
            break;
        }
        SecWriteString(tmpBuf, retVal, stream, charsOut);
        if (*charsOut == -1) {
            break;
        }
        --count;
        ++p;
    }
}
#endif
#endif

#if SECUREC_ENABLE_SPRINTF_FLOAT
/*
 * Write text of float
 * Using independent functions to optimize the expansion of inline functions by the compiler
 */
SECUREC_INLINE void SecWriteFloatText(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
#ifdef SECUREC_FOR_WCHAR
#if SECUREC_HAVE_MBTOWC
    SecWriteTextAfterMbtowc(stream, attr, charsOut);
#else
    *charsOut = -1;
    (void)stream; /* To clear e438 last value assigned not used , the compiler will optimize this code */
    (void)attr;   /* To clear e438 last value assigned not used , the compiler will optimize this code */
#endif
#else /* Not SECUREC_FOR_WCHAR */
    SecWriteString(attr->text.str, attr->textLen, stream, charsOut);
#endif
}
#endif

/* Write text of integer or string ... */
SECUREC_INLINE void SecWriteText(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
#ifdef SECUREC_FOR_WCHAR
    if (attr->textIsWide != 0) {
        SecWriteStringChk(stream, attr->text.wStr, attr->textLen, charsOut);
    } else {
#if SECUREC_HAVE_MBTOWC
        SecWriteTextAfterMbtowc(stream, attr, charsOut);
#else
        *charsOut = -1;
#endif
    }

#else /* Not SECUREC_FOR_WCHAR */
    if (attr->textIsWide != 0) {
#if SECUREC_HAVE_WCTOMB
        SecWriteTextAfterWctomb(stream, attr, charsOut);
#else
        *charsOut = -1;
#endif
    } else {
        SecWriteStringChk(stream, attr->text.str, attr->textLen, charsOut);
    }
#endif
}

#define SECUREC_FMT_STATE_OFFSET  256
SECUREC_INLINE SecFmtState SecDecodeState(SecChar ch, SecFmtState lastState)
{
#ifdef SECUREC_FOR_WCHAR
    /* Convert to unsigned char to clear gcc 4.3.4 warning */
    unsigned char fmtType = (unsigned char)((((unsigned int)(int)(ch)) <= (unsigned int)(int)(L'~')) ? \
        (g_stateTable[(unsigned char)(ch)]) : 0);
    return (SecFmtState)(g_stateTable[fmtType * ((unsigned char)STAT_INVALID + 1) +
        (unsigned char)(lastState) + SECUREC_FMT_STATE_OFFSET]);
#else
    unsigned char fmtType = g_stateTable[(unsigned char)(ch)];
    return (SecFmtState)(g_stateTable[fmtType * ((unsigned char)STAT_INVALID + 1) +
        (unsigned char)(lastState) + SECUREC_FMT_STATE_OFFSET]);
#endif
}

SECUREC_INLINE void SecDecodeFlags(SecChar ch, SecFormatAttr *attr)
{
    switch (ch) {
        case SECUREC_CHAR(' '):
            attr->flags |= SECUREC_FLAG_SIGN_SPACE;
            break;
        case SECUREC_CHAR('+'):
            attr->flags |= SECUREC_FLAG_SIGN;
            break;
        case SECUREC_CHAR('-'):
            attr->flags |= SECUREC_FLAG_LEFT;
            break;
        case SECUREC_CHAR('0'):
            attr->flags |= SECUREC_FLAG_LEADZERO;   /* Add zero th the front */
            break;
        case SECUREC_CHAR('#'):
            attr->flags |= SECUREC_FLAG_ALTERNATE;  /* Output %x with 0x */
            break;
        default:
            break;
    }
    return;
}


/*
 * Decoded size identifier in format string to Reduce the number of lines of function code
 */
SECUREC_INLINE int SecDecodeSizeI(SecFormatAttr *attr, const SecChar **format)
{
#ifdef SECUREC_ON_64BITS
    attr->flags |= SECUREC_FLAG_I64;    /* %I  to  INT64 */
#endif
    if ((**format == SECUREC_CHAR('6')) && (*((*format) + 1) == SECUREC_CHAR('4'))) {
        (*format) += 2; /* Add 2 to skip I64 */
        attr->flags |= SECUREC_FLAG_I64;    /* %I64  to  INT64 */
    } else if ((**format == SECUREC_CHAR('3')) && (*((*format) + 1) == SECUREC_CHAR('2'))) {
        (*format) += 2; /* Add 2 to skip I32 */
        attr->flags &= ~SECUREC_FLAG_I64;   /* %I64  to  INT32 */
    } else if ((**format == SECUREC_CHAR('d')) || (**format == SECUREC_CHAR('i')) ||
        (**format == SECUREC_CHAR('o')) || (**format == SECUREC_CHAR('u')) ||
        (**format == SECUREC_CHAR('x')) || (**format == SECUREC_CHAR('X'))) {
        /* Do nothing */
    } else {
        /* Compatibility  code for "%I" just print I */
        return -1;
    }
    return 0;
}
/*
 * Decoded size identifier in format string, and skip format to next charater
 */
SECUREC_INLINE int SecDecodeSize(SecChar ch, SecFormatAttr *attr, const SecChar **format)
{
    switch (ch) {
        case SECUREC_CHAR('l'):
            if (**format == SECUREC_CHAR('l')) {
                *format = *format + 1;
                attr->flags |= SECUREC_FLAG_LONGLONG; /* For long long */
            } else {
                attr->flags |= SECUREC_FLAG_LONG;     /* For long int or wchar_t */
            }
            break;
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
        case SECUREC_CHAR('z'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('Z'):
            attr->flags |= SECUREC_FLAG_SIZE;
            break;
        case SECUREC_CHAR('j'):
            attr->flags |= SECUREC_FLAG_INTMAX;
            break;
#endif
        case SECUREC_CHAR('t'):
            attr->flags |= SECUREC_FLAG_PTRDIFF;
            break;
        case SECUREC_CHAR('q'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('L'):
            attr->flags |= (SECUREC_FLAG_LONGLONG | SECUREC_FLAG_LONG_DOUBLE);
            break;
        case SECUREC_CHAR('I'):
            if (SecDecodeSizeI(attr, format) != 0) {
                /* Compatibility  code for "%I" just print I */
                return -1;
            }
            break;
        case SECUREC_CHAR('h'):
            if (**format == SECUREC_CHAR('h')) {
                *format = *format + 1;
                attr->flags |= SECUREC_FLAG_CHAR;   /* For char */
            } else {
                attr->flags |= SECUREC_FLAG_SHORT;  /* For short int */
            }
            break;
        case SECUREC_CHAR('w'):
            attr->flags |= SECUREC_FLAG_WIDECHAR;   /* For wide char */
            break;
        default:
            break;
    }
    return 0;
}


/*
 * Decoded char type identifier
 */
SECUREC_INLINE void SecDecodeTypeC(SecFormatAttr *attr, unsigned int c)
{
    attr->textLen = 1; /* Only 1 wide character */

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT)) && !(defined(__hpux)) && !(defined(SECUREC_ON_SOLARIS))
    attr->flags &= ~SECUREC_FLAG_LEADZERO;
#endif

#ifdef SECUREC_FOR_WCHAR
    if (attr->flags & SECUREC_FLAG_SHORT) {
        /* Get  multibyte character from argument */
        attr->buffer.str[0] = (char)c;
        attr->text.str = attr->buffer.str;
        attr->textIsWide = 0;
    } else {
        attr->buffer.wStr[0] = (wchar_t)c;
        attr->text.wStr = attr->buffer.wStr;
        attr->textIsWide = 1;
    }
#else /* Not SECUREC_FOR_WCHAR */
    if (attr->flags & (SECUREC_FLAG_LONG | SECUREC_FLAG_WIDECHAR)) {
#if SECUREC_HAVE_WCHART
        attr->buffer.wStr[0] = (wchar_t)c;
        attr->text.wStr = attr->buffer.wStr;
        attr->textIsWide = 1;
#else
        attr->textLen = 0; /* Ignore unsupported characters */
        attr->fldWidth = 0; /* No paddings  */
#endif
    } else {
        /* Get  multibyte character from argument */
        attr->buffer.str[0] = (char)c;
        attr->text.str = attr->buffer.str;
        attr->textIsWide = 0;
    }
#endif
}

SECUREC_INLINE void SecDecodeTypeSchar(SecFormatAttr *attr)
{
    if (attr->text.str == NULL) {
        /*
         * Literal string to print null ptr, define it as array rather than const text area
         * To avoid gcc warning with pointing const text with variable
         */
        static char strNullString[SECUREC_NULL_STRING_SIZE] = "(null)";
        attr->text.str = strNullString;
    }
    if (attr->precision == -1) {
        /* Precision NOT assigned */
        /* The strlen performance is high when the string length is greater than 32 */
        attr->textLen = (int)strlen(attr->text.str);
    } else {
        /* Precision assigned */
        size_t textLen;
        SECUREC_CALC_STR_LEN(attr->text.str, (size_t)(unsigned int)attr->precision, &textLen);
        attr->textLen = (int)textLen;
    }
}

SECUREC_INLINE void SecDecodeTypeSwchar(SecFormatAttr *attr)
{
#if SECUREC_HAVE_WCHART
    size_t textLen;
    attr->textIsWide = 1;
    if (attr->text.wStr == NULL) {
        /*
         * Literal string to print null ptr, define it as array rather than const text area
         * To avoid gcc warning with pointing const text with variable
         */
        static wchar_t wStrNullString[SECUREC_NULL_STRING_SIZE] = { L'(', L'n', L'u', L'l', L'l', L')', L'\0', L'\0' };
        attr->text.wStr = wStrNullString;
    }
    /* The textLen in wchar_t,when precision is -1, it is unlimited  */
    SECUREC_CALC_WSTR_LEN(attr->text.wStr, (size_t)(unsigned int)attr->precision, &textLen);
    attr->textLen = (int)textLen;
#else
    attr->textLen = 0;
#endif
}


/*
 * Decoded string identifier
 */
SECUREC_INLINE void SecDecodeTypeS(SecFormatAttr *attr, char *argPtr)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT)) && (!defined(SECUREC_ON_UNIX))
    attr->flags &= ~SECUREC_FLAG_LEADZERO;
#endif
    attr->text.str = argPtr;
#ifdef SECUREC_FOR_WCHAR
#if defined(SECUREC_COMPATIBLE_LINUX_FORMAT)
    if (!(attr->flags & SECUREC_FLAG_LONG)) {
        attr->flags |= SECUREC_FLAG_SHORT;
    }
#endif
    if (attr->flags & SECUREC_FLAG_SHORT) {
        /* The textLen now contains length in multibyte chars */
        SecDecodeTypeSchar(attr);
    } else {
        /* The textLen now contains length in wide chars */
        SecDecodeTypeSwchar(attr);
    }
#else /* SECUREC_FOR_WCHAR */
    if (attr->flags & (SECUREC_FLAG_LONG | SECUREC_FLAG_WIDECHAR)) {
        /* The textLen now contains length in wide chars */
        SecDecodeTypeSwchar(attr);
    } else {
        /* The textLen now contains length in multibyte chars */
        SecDecodeTypeSchar(attr);
    }
#endif /* SECUREC_FOR_WCHAR */
    if (attr->textLen < 0) {
        attr->textLen = 0;
    }
}

/*
 * Write one character to dest buffer
 */
SECUREC_INLINE void SecOutputOneChar(SecChar ch, SecPrintfStream *stream, int *counter)
{
    /* Count must be reduced first, In order to identify insufficient length */
    if ((stream->count -= (int)(sizeof(SecChar))) >= 0) {
        *((SecChar *)(void *)(stream->cur)) = (SecChar)ch;
        stream->cur += sizeof(SecChar);
        *counter = *(counter) + 1;
        return;
    }
    /* No enough length */
    *counter = -1;
}

/*
 * Check precison in format
 */
SECUREC_INLINE int SecDecodePrecision(SecChar ch, SecFormatAttr *attr)
{
    if (attr->dynPrecision == 0) {
        /* Add digit to current precision */
        if (SECUREC_MUL_TEN_ADD_BEYOND_MAX(attr->precision)) {
            return -1;
        }
        attr->precision = (int)SECUREC_MUL_TEN((unsigned int)attr->precision) +
            (unsigned char)(ch - SECUREC_CHAR('0'));
    } else {
        if (attr->precision < 0) {
            attr->precision = -1;
        }
        if (attr->precision > SECUREC_MAX_WIDTH_LEN) {
            return -1;
        }
    }
    return 0;
}


/*
 * Check width in format
 */
SECUREC_INLINE int SecDecodeWidth(SecChar ch, SecFormatAttr *attr, SecFmtState lastState)
{
    if (attr->dynWidth == 0) {
        if (lastState != STAT_WIDTH) {
            attr->fldWidth = 0;
        }
        if (SECUREC_MUL_TEN_ADD_BEYOND_MAX(attr->fldWidth)) {
            return -1;
        }
        attr->fldWidth = (int)SECUREC_MUL_TEN((unsigned int)attr->fldWidth) +
            (unsigned char)(ch - SECUREC_CHAR('0'));
    } else {
        if (attr->fldWidth < 0) {
            attr->flags |= SECUREC_FLAG_LEFT;
            attr->fldWidth = (-attr->fldWidth);
            if (attr->fldWidth > SECUREC_MAX_WIDTH_LEN) {
                return -1;
            }
        }
    }
    return 0;
}


/*
 * The sprintf_s function processes the wide character as a parameter for %C
 * The swprintf_s function processes the multiple character as a parameter for %C
 */
SECUREC_INLINE void SecUpdateWcharFlags(SecFormatAttr *attr)
{
    if (!(attr->flags & (SECUREC_FLAG_SHORT | SECUREC_FLAG_LONG | SECUREC_FLAG_WIDECHAR))) {
#ifdef SECUREC_FOR_WCHAR
        attr->flags |= SECUREC_FLAG_SHORT;
#else
        attr->flags |= SECUREC_FLAG_WIDECHAR;
#endif
    }
}
/*
 * When encountering %S, current just same as %C
 */
SECUREC_INLINE void SecUpdateWstringFlags(SecFormatAttr *attr)
{
    SecUpdateWcharFlags(attr);
}

#if SECUREC_IN_KERNEL
SECUREC_INLINE void SecUpdatePointFlagsForKernel(SecFormatAttr *attr)
{
    /* Width is not set */
    if (attr->fldWidth <= 0) {
        attr->flags |= SECUREC_FLAG_LEADZERO;
        attr->fldWidth = 2 * sizeof(void *);  /* 2 x byte number is the length of hex */
    }
    if (attr->flags & SECUREC_FLAG_ALTERNATE) {
        /* Alternate form means '0x' prefix */
        attr->prefix[0] = SECUREC_CHAR('0');
        attr->prefix[1] = SECUREC_CHAR('x');
        attr->prefixLen = SECUREC_PREFIX_LEN;
    }
    attr->flags |= SECUREC_FLAG_LONG;  /* Converting a long */
}
#endif

SECUREC_INLINE void SecUpdatePointFlags(SecFormatAttr *attr)
{
    attr->flags |= SECUREC_FLAG_POINTER;
#if SECUREC_IN_KERNEL
    SecUpdatePointFlagsForKernel(attr);
#else
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) || defined(SECUREC_VXWORKS_PLATFORM)) && (!defined(SECUREC_ON_UNIX))
#if defined(SECUREC_VXWORKS_PLATFORM)
    attr->precision = 1;
#else
    attr->precision = 0;
#endif
    attr->flags |= SECUREC_FLAG_ALTERNATE; /* "0x" is not default prefix in UNIX */
    attr->digits = g_itoaLowerDigits;
#else /* On unix or win */
#if defined(_AIX) || defined(SECUREC_ON_SOLARIS)
    attr->precision = 1;
#else
    attr->precision = 2 * sizeof(void *);  /* 2 x byte number is the length of hex */
#endif
#if defined(SECUREC_ON_UNIX)
    attr->digits = g_itoaLowerDigits;
#else
    attr->digits = g_itoaUpperDigits;
#endif
#endif

#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    attr->flags &= ~SECUREC_FLAG_LEADZERO;
#endif

#ifdef SECUREC_ON_64BITS
    attr->flags |= SECUREC_FLAG_I64;   /* Converting an int64 */
#else
    attr->flags |= SECUREC_FLAG_LONG;  /* Converting a long */
#endif
    /* Set up for %#p on different system */
    if (attr->flags & SECUREC_FLAG_ALTERNATE) {
        /* Alternate form means '0x' prefix */
        attr->prefix[0] = SECUREC_CHAR('0');
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) || defined(SECUREC_VXWORKS_PLATFORM))
        attr->prefix[1] = SECUREC_CHAR('x');
#else
        attr->prefix[1] = (SecChar)(attr->digits[16]); /* 16 for 'x' or 'X' */
#endif
#if defined(_AIX) || defined(SECUREC_ON_SOLARIS)
        attr->prefixLen = 0;
#else
        attr->prefixLen = SECUREC_PREFIX_LEN;
#endif
    }
#endif
}

SECUREC_INLINE void SecUpdateXpxFlags(SecFormatAttr *attr, SecChar ch)
{
    /* Use unsigned lower hex output for 'x' */
    attr->digits = g_itoaLowerDigits;
    attr->radix = SECUREC_RADIX_HEX;
    switch (ch) {
        case SECUREC_CHAR('p'):
            /* Print a pointer */
            SecUpdatePointFlags(attr);
            break;
        case SECUREC_CHAR('X'): /* fall-through */ /* FALLTHRU */
            /* Unsigned upper hex output */
            attr->digits = g_itoaUpperDigits;
            /* fall-through */ /* FALLTHRU */
        default:
            /* For %#x or %#X */
            if (attr->flags & SECUREC_FLAG_ALTERNATE) {
                /* Alternate form means '0x' prefix */
                attr->prefix[0] = SECUREC_CHAR('0');
                attr->prefix[1] = (SecChar)(attr->digits[16]); /* 16 for 'x' or 'X' */
                attr->prefixLen = SECUREC_PREFIX_LEN;
            }
            break;
    }
}
SECUREC_INLINE void SecUpdateOudiFlags(SecFormatAttr *attr, SecChar ch)
{
    /* Do not set digits here */
    switch (ch) {
        case SECUREC_CHAR('i'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('d'): /* fall-through */ /* FALLTHRU */
            /* For signed decimal output */
            attr->flags |= SECUREC_FLAG_SIGNED;
            /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('u'):
            attr->radix = SECUREC_RADIX_DECIMAL;
            attr->digits = g_itoaLowerDigits;
            break;
        case SECUREC_CHAR('o'):
            /* For unsigned octal output */
            attr->radix = SECUREC_RADIX_OCTAL;
            attr->digits = g_itoaLowerDigits;
            if (attr->flags & SECUREC_FLAG_ALTERNATE) {
                /* Alternate form means force a leading 0 */
                attr->flags |= SECUREC_FLAG_FORCE_OCTAL;
            }
            break;
        default:
            break;
    }
}

#if SECUREC_ENABLE_SPRINTF_FLOAT
SECUREC_INLINE void SecFreeFloatBuffer(SecFloatAdapt *floatAdapt)
{
    if (floatAdapt->floatBuffer != NULL) {
        SECUREC_FREE(floatAdapt->floatBuffer);
    }
    if (floatAdapt->allocatedFmtStr != NULL) {
        SECUREC_FREE(floatAdapt->allocatedFmtStr);
    }
    floatAdapt->floatBuffer = NULL;
    floatAdapt->allocatedFmtStr = NULL;
    floatAdapt->fmtStr = NULL;
    floatAdapt->bufferSize = 0;
}

SECUREC_INLINE void SecSeekToFrontPercent(const SecChar **format)
{
    const SecChar *fmt = *format;
    while (*fmt != SECUREC_CHAR('%')) { /* Must meet '%' */
        --fmt;
    }
    *format = fmt;
}

/* Init float format, return 0 is OK */
SECUREC_INLINE int SecInitFloatFmt(SecFloatAdapt *floatFmt, const SecChar *format)
{
    const SecChar *fmt = format - 2;  /* Sub 2 to the position before 'f' or 'g' */
    int fmtStrLen;
    int i;

    SecSeekToFrontPercent(&fmt);
    /* Now fmt point to '%' */
    fmtStrLen = (int)(size_t)(format - fmt) + 1;   /* With ending terminator */

    if (fmtStrLen > (int)sizeof(floatFmt->buffer)) {
        /* When buffer is NOT enough, alloc a new buffer */
        floatFmt->allocatedFmtStr = (char *)SECUREC_MALLOC((size_t)((unsigned int)fmtStrLen));
        if (floatFmt->allocatedFmtStr == NULL) {
            return -1;
        }
        floatFmt->fmtStr = floatFmt->allocatedFmtStr;
    } else {
        floatFmt->fmtStr = floatFmt->buffer;
        floatFmt->allocatedFmtStr = NULL; /* Must set to NULL, later code free memory based on this identity */
    }

    for (i = 0; i < fmtStrLen - 1; ++i) {
        /* Convert wchar to char */
        floatFmt->fmtStr[i] = (char)(fmt[i]);  /* Copy the format string */
    }
    floatFmt->fmtStr[fmtStrLen - 1] = '\0';

    return 0;

}

/* Init float buffer and format, return 0 is OK */
SECUREC_INLINE int SecInitFloatBuffer(SecFloatAdapt *floatAdapt, const SecChar *format, SecFormatAttr *attr)
{
    floatAdapt->allocatedFmtStr = NULL;
    floatAdapt->fmtStr = NULL;
    floatAdapt->floatBuffer = NULL;
    /* Compute the precision value */
    if (attr->precision < 0) {
        attr->precision = SECUREC_FLOAT_DEFAULT_PRECISION;
    }
    /*
     * Calc buffer size to store double value
     * The maximum length of SECUREC_MAX_WIDTH_LEN is enough
     */
    if (attr->flags & SECUREC_FLAG_LONG_DOUBLE) {
        if (attr->precision > (SECUREC_MAX_WIDTH_LEN - SECUREC_FLOAT_BUFSIZE_LB)) {
            return -1;
        }
        /* Long double needs to meet the basic print length */
        floatAdapt->bufferSize = SECUREC_FLOAT_BUFSIZE_LB + attr->precision + SECUREC_FLOAT_BUF_EXT;
    } else {
        if (attr->precision > (SECUREC_MAX_WIDTH_LEN - SECUREC_FLOAT_BUFSIZE)) {
            return -1;
        }
        /* Double needs to meet the basic print length */
        floatAdapt->bufferSize = SECUREC_FLOAT_BUFSIZE + attr->precision + SECUREC_FLOAT_BUF_EXT;
    }
    if (attr->fldWidth > floatAdapt->bufferSize) {
        floatAdapt->bufferSize = attr->fldWidth + SECUREC_FLOAT_BUF_EXT;
    }

    if (floatAdapt->bufferSize > SECUREC_BUFFER_SIZE) {
        /* The current vlaue of SECUREC_BUFFER_SIZE could NOT store the formatted float string */
        floatAdapt->floatBuffer = (char *)SECUREC_MALLOC(((size_t)(unsigned int)floatAdapt->bufferSize));
        if (floatAdapt->floatBuffer == NULL) {
            return -1;
        }
        attr->text.str = floatAdapt->floatBuffer;
    } else {
        attr->text.str = attr->buffer.str; /* Output buffer for float string with default size */
    }

    if (SecInitFloatFmt(floatAdapt, format) != 0) {
        if (floatAdapt->floatBuffer != NULL) {
            SECUREC_FREE(floatAdapt->floatBuffer);
            floatAdapt->floatBuffer = NULL;
        }
        return -1;
    }
    return 0;
}
#endif

SECUREC_INLINE SecInt64 SecUpdateNegativeChar(SecFormatAttr *attr, char ch)
{
    SecInt64 num64 = ch; /* Sign extend */
    if (num64 >= 128) { /* 128 on some platform, char is always unsigned */
        unsigned char tmp = (unsigned char)(~((unsigned char)ch));
        num64 = tmp + 1;
        attr->flags |= SECUREC_FLAG_NEGATIVE;
    }
    return num64;
}

/*
 * If the precision is not satisfied, zero is added before the string
 */
SECUREC_INLINE void SecNumberSatisfyPrecision(SecFormatAttr *attr)
{
    int precision;
    if (attr->precision < 0) {
        precision = 1; /* Default precision 1 */
    } else {
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
        attr->flags &= ~SECUREC_FLAG_LEADZERO;
#else
        if (!(attr->flags & SECUREC_FLAG_POINTER)) {
            attr->flags &= ~SECUREC_FLAG_LEADZERO;
        }
#endif
        if (attr->precision > SECUREC_MAX_PRECISION) {
            attr->precision = SECUREC_MAX_PRECISION;
        }
        precision = attr->precision;
    }
    while (attr->textLen < precision) {
        ++attr->textLen;
        *(--attr->text.str) = '0';
    }
}

/*
 * Add leading zero for %#o
 */
SECUREC_INLINE void SecNumberForceOctal(SecFormatAttr *attr)
{
    /* Force a leading zero if FORCEOCTAL flag set */
    if ((attr->flags & SECUREC_FLAG_FORCE_OCTAL) &&
        (attr->textLen == 0 || attr->text.str[0] != '0')) {
        *(--attr->text.str) = '0';
        ++attr->textLen;
    }
}

SECUREC_INLINE void SecUpdateSignedNumberPrefix(SecFormatAttr *attr)
{
    if (attr->flags & SECUREC_FLAG_SIGNED) {
        if (attr->flags & SECUREC_FLAG_NEGATIVE) {
            /* Prefix is '-' */
            attr->prefix[0] = SECUREC_CHAR('-');
            attr->prefixLen = 1;
        } else if (attr->flags & SECUREC_FLAG_SIGN) {
            /* Prefix is '+' */
            attr->prefix[0] = SECUREC_CHAR('+');
            attr->prefixLen = 1;
        } else if (attr->flags & SECUREC_FLAG_SIGN_SPACE) {
            /* Prefix is ' ' */
            attr->prefix[0] = SECUREC_CHAR(' ');
            attr->prefixLen = 1;
        }
    }
}

SECUREC_INLINE void SecNumberCompatZero(SecFormatAttr *attr)
{
#if SECUREC_IN_KERNEL
    if (attr->flags & SECUREC_FLAG_POINTER) {
        static char strNullPointer[SECUREC_NULL_STRING_SIZE] = "(null)";
        attr->text.str = strNullPointer;
        attr->textLen = 6; /* Length of (null) is 6 */
        attr->flags &= ~SECUREC_FLAG_LEADZERO;
        attr->prefixLen = 0;
        if (attr->precision >= 0 && attr->precision < attr->textLen) {
            attr->textLen = attr->precision;
        }
    }
    if (!(attr->flags & SECUREC_FLAG_POINTER) && attr->radix == SECUREC_RADIX_HEX &&
        (attr->flags & SECUREC_FLAG_ALTERNATE)) {
        /* Add 0x prefix for %x or %X, the prefix string has been set before */
        attr->prefixLen = SECUREC_PREFIX_LEN;
    }
#elif defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && (!defined(SECUREC_ON_UNIX))
    if (attr->flags & SECUREC_FLAG_POINTER) {
        static char strNullPointer[SECUREC_NULL_STRING_SIZE] = "(nil)";
        attr->text.str = strNullPointer;
        attr->textLen = 5; /* Length of (nil) is 5 */
        attr->flags &= ~SECUREC_FLAG_LEADZERO;
    }
#elif defined(SECUREC_VXWORKS_PLATFORM) || defined(__hpux)
    if ((attr->flags & SECUREC_FLAG_POINTER) && (attr->flags & SECUREC_FLAG_ALTERNATE)) {
        /* Add 0x prefix for %p, the prefix string has been set before */
        attr->prefixLen = SECUREC_PREFIX_LEN;
    }
#endif
    (void)attr; /* To clear e438 last value assigned not used , the compiler will optimize this code */
}

#ifdef SECUREC_FOR_WCHAR
/*
 * Formatting output core functions for wchar version.Called by a function such as vswprintf_s
 * The argList must not be declare as const
 */
SECUREC_INLINE int SecOutputSW(SecPrintfStream *stream, const wchar_t *cFormat, va_list argList)
#else
/*
 * Formatting output core functions for char version.Called by a function such as vsnprintf_s
 */
SECUREC_INLINE int SecOutputS(SecPrintfStream *stream, const char *cFormat, va_list argList)
#endif
{
    const SecChar *format = cFormat;
    int charsOut;               /* Characters written */
    int noOutput = 0; /* Must be initialized or compiler alerts */
    SecFmtState state;
    SecChar ch;                 /* Currently read character */
    SecFormatAttr formatAttr;

    formatAttr.flags = 0;
    formatAttr.textIsWide = 0;    /* Flag for buffer contains wide chars */
    formatAttr.fldWidth = 0;
    formatAttr.precision = 0;
    formatAttr.dynWidth = 0;
    formatAttr.dynPrecision = 0;
    formatAttr.digits = g_itoaUpperDigits;
    formatAttr.radix = SECUREC_RADIX_DECIMAL;
    formatAttr.padding = 0;
    formatAttr.textLen = 0;
    formatAttr.text.str = NULL;
    formatAttr.prefixLen = 0;
    formatAttr.prefix[0] = SECUREC_CHAR('\0');
    formatAttr.prefix[1] = SECUREC_CHAR('\0');
    charsOut = 0;
    state = STAT_NORMAL;        /* Starting state */

    /* Loop each format character */
    while (*format != SECUREC_CHAR('\0') && charsOut >= 0) {
        SecFmtState lastState = state;
        ch = *(format++);
        state = SecDecodeState(ch, lastState);
        switch (state) {
            case STAT_NORMAL:
                SecOutputOneChar(ch, stream, &charsOut);
                continue;
            case STAT_PERCENT:
                /* Set default values */
                noOutput = 0;
                formatAttr.prefixLen = 0;
                formatAttr.textLen = 0;
                formatAttr.flags = 0;
                formatAttr.fldWidth = 0;
                formatAttr.precision = -1;
                formatAttr.textIsWide = 0;
                formatAttr.dynWidth = 0;
                formatAttr.dynPrecision = 0;
                break;
            case STAT_FLAG:
                /* Set flag based on which flag character */
                SecDecodeFlags(ch, &formatAttr);
                break;
            case STAT_WIDTH:
                /* Update width value */
                formatAttr.dynWidth = 0;
                if (ch == SECUREC_CHAR('*')) {
                    /* get width from arg list */
                    formatAttr.fldWidth = (int)va_arg(argList, int);
                    formatAttr.dynWidth = 1;
                }
                if (SecDecodeWidth(ch, &formatAttr, lastState) != 0) {
                    return -1;
                }
                break;
            case STAT_DOT:
                formatAttr.precision = 0;
                break;
            case STAT_PRECIS:
                /* Update precison value */
                formatAttr.dynPrecision = 0;
                if (ch == SECUREC_CHAR('*')) {
                    /* Get precision from arg list */
                    formatAttr.precision = (int)va_arg(argList, int);
                    formatAttr.dynPrecision = 1;
                }
                if (SecDecodePrecision(ch, &formatAttr) != 0) {
                    return -1;
                }
                break;
            case STAT_SIZE:
                /* Read a size specifier, set the formatAttr.flags based on it, and skip format to next charater */
                if (SecDecodeSize(ch, &formatAttr, &format) != 0) {
                    /* Compatibility  code for "%I" just print I */
                    SecOutputOneChar(ch, stream, &charsOut);
                    state = STAT_NORMAL;
                    continue;
                }
                break;
            case STAT_TYPE:
                switch (ch) {
                    case SECUREC_CHAR('C'): /* Wide char */
                        SecUpdateWcharFlags(&formatAttr);
                        /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('c'): {
                        unsigned int cValue = (unsigned int)va_arg(argList, int);
                        SecDecodeTypeC(&formatAttr, cValue);
                        break;
                    }
                    case SECUREC_CHAR('S'):    /* Wide char string */
                        SecUpdateWstringFlags(&formatAttr);
                        /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('s'): {
                        char *argPtr = (char *)va_arg(argList, char *);
                        SecDecodeTypeS(&formatAttr, argPtr);
                        break;
                    }
                    case SECUREC_CHAR('G'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('g'): /* fall-through */ /* FALLTHRU */
                        /* Default precision is 1 for g or G */
                        if (formatAttr.precision == 0) {
                            formatAttr.precision = 1;
                        }
                        /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('E'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('F'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('e'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('f'): {
#if SECUREC_ENABLE_SPRINTF_FLOAT
                        /* Add following code to call system sprintf API for float number */
                        SecFloatAdapt floatAdapt;
                        noOutput = 1; /* It's no more data needs to be written */

                        /* Now format is pointer to the next character of 'f' */
                        if (SecInitFloatBuffer(&floatAdapt, format, &formatAttr) != 0) {
                            break;
                        }

                        if (formatAttr.flags & SECUREC_FLAG_LONG_DOUBLE) {
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
                            long double tmp = (long double)va_arg(argList, long double);
                            SecFormatLongDboule(&formatAttr, &floatAdapt, tmp);
#else
                            double tmp = (double)va_arg(argList, double);
                            SecFormatDboule(&formatAttr, &floatAdapt, tmp);
#endif
                        } else {
                            double tmp = (double)va_arg(argList, double);
                            SecFormatDboule(&formatAttr, &floatAdapt, tmp);
                        }

                        /* Only need write formated float string */
                        SecWriteFloatText(stream, &formatAttr, &charsOut);
                        SecFreeFloatBuffer(&floatAdapt);
                        break;
#else
                        return -1;
#endif
                    }

                    case SECUREC_CHAR('X'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('p'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('x'): /* fall-through */ /* FALLTHRU */
                        SecUpdateXpxFlags(&formatAttr, ch);
                        /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('i'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('d'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('u'): /* fall-through */ /* FALLTHRU */
                    case SECUREC_CHAR('o'): {
                        SecInt64 num64;
                        SecUpdateOudiFlags(&formatAttr, ch);
                        /* Read argument into variable num64 */
                        if ((formatAttr.flags & SECUREC_FLAG_I64) || (formatAttr.flags & SECUREC_FLAG_LONGLONG)) {
                            num64 = (SecInt64)va_arg(argList, SecInt64); /* Maximum Bit Width sign bit unchanged */
                        } else if (formatAttr.flags & SECUREC_FLAG_LONG) {
                            if (formatAttr.flags & SECUREC_FLAG_SIGNED) {
                                num64 = (long)va_arg(argList, long); /* Sign extend */
                            } else {
                                num64 = (SecInt64)(unsigned long)va_arg(argList, long); /* Zero-extend */
                            }
                        } else if (formatAttr.flags & SECUREC_FLAG_CHAR) {
                            if (formatAttr.flags & SECUREC_FLAG_SIGNED) {
                                char tmp = (char)va_arg(argList, int); /* Sign extend */
                                num64 = SecUpdateNegativeChar(&formatAttr, tmp);
                            } else {
                                num64 = (SecInt64)(unsigned char)va_arg(argList, int);    /* Zero-extend */
                            }
                        } else if (formatAttr.flags & SECUREC_FLAG_SHORT) {
                            if (formatAttr.flags & SECUREC_FLAG_SIGNED) {
                                num64 = (short)va_arg(argList, int);    /* Sign extend */
                            } else {
                                num64 = (SecInt64)(unsigned short)va_arg(argList, int);   /* Zero-extend */
                            }
                        }
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
                        else if (formatAttr.flags & SECUREC_FLAG_PTRDIFF) {
                            num64 = (ptrdiff_t)va_arg(argList, ptrdiff_t);  /* Sign extend */
                        } else if (formatAttr.flags & SECUREC_FLAG_SIZE) {
                            if (formatAttr.flags & SECUREC_FLAG_SIGNED) {
                                /* No suitable macros were found to handle the branch */
                                if (SecIsSameSize(sizeof(size_t), sizeof(long))) {
                                    num64 = va_arg(argList, long);  /* Sign extend */
                                } else if (SecIsSameSize(sizeof(size_t), sizeof(long long))) {
                                    num64 = va_arg(argList, long long); /* Sign extend */
                                } else {
                                    num64 = va_arg(argList, int);   /* Sign extend */
                                }
                            } else {
                                num64 = (SecInt64)(size_t)va_arg(argList, size_t);  /* Zero-extend */
                            }
                        } else if (formatAttr.flags & SECUREC_FLAG_INTMAX) {
                            num64 = (SecInt64)va_arg(argList, SecInt64);
                        }
#endif
                        else {
                            if (formatAttr.flags & SECUREC_FLAG_SIGNED) {
                                num64 = va_arg(argList, int);   /* Sign extend */
                            } else {
                                num64 = (SecInt64)(unsigned int)va_arg(argList, int); /* Zero-extend */
                            }
                        }

                        /* The order of the following calls must be correct */
                        SecNumberToBuffer(&formatAttr, num64);
                        SecNumberSatisfyPrecision(&formatAttr);
                        SecNumberForceOctal(&formatAttr);
                        SecUpdateSignedNumberPrefix(&formatAttr);
                        if (num64 == 0) {
                            SecNumberCompatZero(&formatAttr);
                        }
                        break;
                    }
                    default:
                        break;
                }

                if (noOutput == 0) {
                    /* Calculate amount of padding */
                    formatAttr.padding = (formatAttr.fldWidth - formatAttr.textLen) - formatAttr.prefixLen;

                    /* Put out the padding, prefix, and text, in the correct order */
                    SecWriteLeftPadding(stream, &formatAttr, &charsOut);
                    SecWritePrefix(stream, &formatAttr, &charsOut);
                    SecWriteLeadingZero(stream, &formatAttr, &charsOut);
                    SecWriteText(stream, &formatAttr, &charsOut);
                    SecWriteRightPadding(stream, &formatAttr, &charsOut);
                }
                break;
            case STAT_INVALID: /* fall-through */ /* FALLTHRU */
            default:
                return -1;  /* Input format is wrong(STAT_INVALID), directly return */
        }
    }

    if (state != STAT_NORMAL && state != STAT_TYPE) {
        return -1;
    }

    return charsOut;            /* The number of characters written */
}

/*
 * Output one zero character zero into the SecPrintfStream structure
 * If there is not enough space, make sure f->count is less than 0
 */
SECUREC_INLINE int SecPutZeroChar(SecPrintfStream *str)
{
    if (--(str->count) >= 0) {
        *(str->cur) = '\0';
        str->cur = str->cur + 1;
        return 0;
    }
    return -1;
}

#endif /* OUTPUT_INL_2B263E9C_43D8_44BB_B17A_6D2033DECEE5 */

