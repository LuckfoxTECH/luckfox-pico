/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_DEBUG_H_
#define INCLUDE_RT_MPI_RK_DEBUG_H_
#ifndef __BUILD_KO__
#include <stdarg.h>
#include <stdio.h>
#endif
#include "rk_common.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define RK_DBG_FATAL 1   /* fatal error                          */
#define RK_DBG_ERR 2     /* error conditions                     */
#define RK_DBG_WARN 3    /* warning conditions                   */
#define RK_DBG_INFO 4    /* informational                        */
#define RK_DBG_DEBUG 5   /* debug-level messages                 */
#define RK_DBG_VERBOSE 6 /* verbose                              */

typedef struct rkLOG_LEVEL_CONF_S {
  MOD_ID_E enModId;
  RK_S32 s32Level;
  RK_CHAR cModName[16];
} LOG_LEVEL_CONF_S;

/******************************************************************************
** For User Mode : RK_PRINT, RK_ASSERT, RK_TRACE
******************************************************************************/

#define RK_PRINT printf

#define CONFIG_RK_LOG_TRACE_SUPPORT 1
#define DBG_MOD_COMB1(MOD1)                                                    \
  ((RK_ID_BUTT << 24) | (RK_ID_BUTT << 16) | (RK_ID_BUTT << 8) | MOD1)
#define DBG_MOD_COMB2(MOD1, MOD2)                                              \
  ((RK_ID_BUTT << 24) | (RK_ID_BUTT << 16) | (MOD2 << 8) | MOD1)
#define DBG_MOD_COMB3(MOD1, MOD2, MOD3)                                        \
  ((RK_ID_BUTT << 24) | (MOD3 << 16) | (MOD2 << 8) | MOD1)
#define DBG_MOD_COMB4(MOD1, MOD2, MOD3, MOD4)                                  \
  ((MOD1 << 24) | (MOD2 << 16) | (MOD3 << 8) | MOD4)
#ifndef DBG_MOD_ID
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_CMPI)
#endif

/* #ifdef RK_DEBUG */
#ifdef CONFIG_RK_LOG_TRACE_SUPPORT
/* Using samples:   RK_ASSERT(x>y); */
#define RK_ASSERT(expr)                                                        \
  do {                                                                         \
    if (!(expr)) {                                                             \
      printf("\nASSERT at:\n"                                                  \
             "  >Function : %s\n"                                              \
             "  >Line No. : %d\n"                                              \
             "  >Condition: %s\n",                                             \
             __FUNCTION__, __LINE__, #expr);                                   \
      _exit(-1);                                                               \
    }                                                                          \
  } while (0)

/* Using samples:
 * RK_TRACE(RK_DBG_DEBUG, "Test %d, %s\n", 12, "Test");
 */
#define RK_TRACE(level, fmt, ...)                                              \
  do {                                                                         \
    RK_LOG(level, DBG_MOD_ID, fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);     \
  } while (0)
#else
#define RK_ASSERT(expr)
#define RK_TRACE(level, fmt, ...)
#endif

#define RK_LOGE(fmt, ...) RK_TRACE(RK_DBG_ERR, fmt, ##__VA_ARGS__)
#define RK_LOGW(fmt, ...) RK_TRACE(RK_DBG_WARN, fmt, ##__VA_ARGS__)
#define RK_LOGI(fmt, ...) RK_TRACE(RK_DBG_INFO, fmt, ##__VA_ARGS__)
#define RK_LOGD(fmt, ...) RK_TRACE(RK_DBG_DEBUG, fmt, ##__VA_ARGS__)
#define RK_LOGV(fmt, ...) RK_TRACE(RK_DBG_VERBOSE, fmt, ##__VA_ARGS__)

void RK_LOG(RK_S32 level, RK_S32 modId, const char *fmt, const char *fname,
            const RK_U32 row, ...);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_RK_DEBUG_H_
