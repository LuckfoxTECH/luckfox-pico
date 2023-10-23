/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: Define macro, data struct, and declare function prototype,
 *              which is used by input.inl, secureinput_a.c and secureinput_w.c.
 * Author: lishunda
 * Create: 2014-02-25
 */

#ifndef SEC_INPUT_H_E950DA2C_902F_4B15_BECD_948E99090D9C
#define SEC_INPUT_H_E950DA2C_902F_4B15_BECD_948E99090D9C
#include "securecutil.h"

#define SECUREC_SCANF_EINVAL             (-1)
#define SECUREC_SCANF_ERROR_PARA         (-2)

/* For internal stream flag */
#define SECUREC_MEM_STR_FLAG             0X01
#define SECUREC_FILE_STREAM_FLAG         0X02
#define SECUREC_PIPE_STREAM_FLAG         0X04
#define SECUREC_LOAD_FILE_TO_MEM_FLAG    0X08

#define SECUREC_BOM_HEADER_SIZE          2
#define SECUREC_BOM_HEADER_BE_1ST        0xFEU
#define SECUREC_BOM_HEADER_BE_2ST        0xFFU
#define SECUREC_BOM_HEADER_LE_1ST        0xFFU
#define SECUREC_BOM_HEADER_LE_2ST        0xFEU
#define SECUREC_UTF8_BOM_HEADER_SIZE     3
#define SECUREC_UTF8_BOM_HEADER_1ST      0xEFU
#define SECUREC_UTF8_BOM_HEADER_2ND      0xBBU
#define SECUREC_UTF8_BOM_HEADER_3RD      0xBFU
#define SECUREC_UTF8_LEAD_1ST            0xE0
#define SECUREC_UTF8_LEAD_2ND            0x80

typedef struct {
    unsigned int flag;          /* Mark the properties of input stream */
    int count;                  /* The size of buffered string in bytes */
    const char *cur;            /* The pointer to next read position */
    char *base;                 /* The pointer to the header of buffered string */
#if SECUREC_ENABLE_SCANF_FILE
    FILE *pf;                   /* The file pointer */
    long oriFilePos;            /* The original position of file offset when fscanf is called */
    int fileRealRead;
#ifdef SECUREC_NO_STD_UNGETC
    unsigned int lastChar;      /* The char code of last input */
    int fUnGet;                 /* The boolean flag of pushing a char back to read stream */
#endif
#endif
} SecFileStream;

#ifdef SECUREC_INLINE_INIT_FILE_STREAM_STR
/*
 * This initialization for eliminating redundant initialization.
 */
SECUREC_INLINE void SecInitFileStreamFromString(SecFileStream *stream, const char *cur, int count)
{
    stream->flag = SECUREC_MEM_STR_FLAG;
    stream->count = count;
    stream->cur = cur;
    stream->base = NULL;
#if SECUREC_ENABLE_SCANF_FILE
    stream->pf = NULL;
    stream->oriFilePos = 0;
    stream->fileRealRead = 0;
#ifdef SECUREC_NO_STD_UNGETC
    stream->lastChar = 0;
    stream->fUnGet = 0;
#endif
#endif
}
#endif

#ifdef SECUREC_INLINE_INIT_FILE_STREAM_STDIN
/*
 * This initialization for eliminating redundant initialization.
 */
SECUREC_INLINE void SecInitFileStreamFromStdin(SecFileStream *stream)
{
    stream->flag = SECUREC_PIPE_STREAM_FLAG;
    stream->count = 0;
    stream->cur = NULL;
    stream->base = NULL;
#if SECUREC_ENABLE_SCANF_FILE
    stream->pf = SECUREC_STREAM_STDIN;
    stream->oriFilePos = 0;
    stream->fileRealRead = 0;
#ifdef SECUREC_NO_STD_UNGETC
    stream->lastChar = 0;
    stream->fUnGet = 0;
#endif
#endif
}
#endif


#ifdef SECUREC_INLINE_INIT_FILE_STREAM_FILE
/*
 * This initialization for eliminating redundant initialization.
 * Compared with the previous version initialization 0,
 * the current code causes the binary size to increase by some bytes
 */
SECUREC_INLINE void SecInitFileStreamFromFile(SecFileStream *stream, FILE *file)
{
    stream->flag = SECUREC_FILE_STREAM_FLAG;
    stream->count = 0;
    stream->cur = NULL;
    stream->base = NULL;
#if SECUREC_ENABLE_SCANF_FILE
    stream->pf = file;
    stream->oriFilePos = 0;
    stream->fileRealRead = 0;
#ifdef SECUREC_NO_STD_UNGETC
    stream->lastChar = 0;
    stream->fUnGet = 0;
#endif
#endif
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

    extern int SecInputS(SecFileStream *stream, const char *cFormat, va_list argList);
    extern void SecClearDestBuf(const char *buffer, const char *format, va_list argList);
#if SECUREC_IN_KERNEL == 0
    extern int SecInputSW(SecFileStream *stream, const wchar_t *cFormat, va_list argList);
    extern void SecClearDestBufW(const wchar_t *buffer, const wchar_t *format, va_list argList);
#endif
/* 20150105 For software and hardware decoupling,such as UMG */
#if defined(SECUREC_SYSAPI4VXWORKS)
#ifdef feof
#undef feof
#endif
    extern int feof(FILE *stream);
#endif

#if defined(SECUREC_SYSAPI4VXWORKS) || defined(SECUREC_CTYPE_MACRO_ADAPT)
#ifndef isspace
#define isspace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\r') || ((c) == '\n'))
#endif
#ifndef iswspace
#define iswspace(c) (((c) == L' ') || ((c) == L'\t') || ((c) == L'\r') || ((c) == L'\n'))
#endif
#ifndef isascii
#define isascii(c) (((unsigned char)(c)) <= 0x7f)
#endif
#ifndef isupper
#define isupper(c) ((c) >= 'A' && (c) <= 'Z')
#endif
#ifndef islower
#define islower(c) ((c) >= 'a' && (c) <= 'z')
#endif
#ifndef isalpha
#define isalpha(c) (isupper(c) || (islower(c)))
#endif
#ifndef isdigit
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#endif
#ifndef isxupper
#define isxupper(c) ((c) >= 'A' && (c) <= 'F')
#endif
#ifndef isxlower
#define isxlower(c) ((c) >= 'a' && (c) <= 'f')
#endif
#ifndef isxdigit
#define isxdigit(c) (isdigit(c) || isxupper(c) || isxlower(c))
#endif
#endif

#ifdef __cplusplus
}
#endif
/* Reserved file operation macro interface */
#define SECUREC_LOCK_FILE(s)
#define SECUREC_UNLOCK_FILE(s)
#define SECUREC_LOCK_STDIN(i, s)
#define SECUREC_UNLOCK_STDIN(i, s)
#endif


