#ifndef __RK_IIO_COMMON_
#define __RK_IIO_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

#define _CAPI __attribute__((visibility("default")))

typedef unsigned char RK_U8;
typedef unsigned short RK_U16;
typedef unsigned int RK_U32;

typedef signed char RK_S8;
typedef short RK_S16;
typedef int RK_S32;

typedef unsigned long RK_UL;
typedef signed long RK_SL;

typedef float RK_FLOAT;
typedef double RK_DOUBLE;

#ifndef _M_IX86
typedef unsigned long long RK_U64;
typedef long long RK_S64;
#else
typedef unsigned __int64 RK_U64;
typedef __int64 RK_S64;
#endif

typedef char RK_CHAR;
// #define RK_VOID void

typedef enum {
  RK_FALSE = 0,
  RK_TRUE = 1,
} RK_BOOL;

#ifndef NULL
#define NULL 0L
#endif

enum RKIIO_RETURN_E {
    RKIIO_ERR_NO_ERR = 0,
    RKIIO_ERR_NULL_PTR,
    RKIIO_ERR_ILLEGAL_PARAM,
    RKIIO_ERR_MISMATCH_MODE,
    RKIIO_ERR_NO_SCAN_ELEMENT,
    RKIIO_ERR_MEM_FULL,
    RKIIO_ERR_DEV_BUSY,
    RKIIO_NO_CTX,
    RKIIO_RESERVED_FUNC,
    RKIIO_NO_MATCH_DEV,
    RKIIO_NOT_READY,
    RKIIO_ERR_NO_DEV,
    RKIIO_ERR_UNSUPPORT,

    // for RKIIO_BUFFER
    RKIIO_ERR_BUFFER_FULL,
    RKIIO_ERR_NO_BUFFER,
    RKIIO_ERR_BUFFER_NO_DATA,
};

#ifdef __cplusplus
}
#endif

#endif // #ifndef __RK_IIO_COMMON_