/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_ERRNO_H_
#define INCLUDE_RT_MPI_RK_ERRNO_H_
#ifndef __BUILD_KO__
#include <stdint.h>
#endif
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum rkERR_LEVEL_E {
  RK_ERR_LEVEL_DEBUG = 0, /* debug-level                                  */
  RK_ERR_LEVEL_INFO,      /* informational                                */
  RK_ERR_LEVEL_NOTICE,    /* normal but significant condition             */
  RK_ERR_LEVEL_WARNING,   /* warning conditions                           */
  RK_ERR_LEVEL_ERROR,     /* error conditions                             */
  RK_ERR_LEVEL_CRIT,      /* critical conditions                          */
  RK_ERR_LEVEL_ALERT,     /* action must be taken immediately             */
  RK_ERR_LEVEL_FATAL,     /* just for compatibility with previous version */
  RK_ERR_LEVEL_BUTT
} ERR_LEVEL_E;

#define RK_ERR_APPID (0x80000000L + 0x20000000L)

/******************************************************************************
|----------------------------------------------------------------|
| 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            |
|----------------------------------------------------------------|
|<--><--7bits----><----8bits---><--3bits---><------13bits------->|
******************************************************************************/

#define RK_DEF_ERR(module, level, errid)                                       \
  ((RK_S32)((RK_ERR_APPID) | ((module) << 16) | ((level) << 13) | (errid)))

/* NOTE! the following defined all common error code,
** all module must reserved 0~63 for their common error code
*/
typedef enum rkEN_ERR_CODE_E {
  // invlalid device ID
  RK_ERR_INVALID_DEVID = 1,
  // invlalid channel ID
  RK_ERR_INVALID_CHNID = 2,
  /*
   * at lease one parameter is illagal
   * eg, an illegal enumeration value
   */
  RK_ERR_ILLEGAL_PARAM = 3,
  // resource exists
  RK_ERR_EXIST = 4,
  // resource unexists
  RK_ERR_UNEXIST = 5,
  // using a NULL point
  RK_ERR_NULL_PTR = 6,
  /*
   * try to enable or initialize system, device
   * or channel, before configing attribute
   */
  RK_ERR_NOT_CONFIG = 7,
  // operation or type is not supported by NOW
  RK_ERR_NOT_SUPPORT = 8,
  /*
   * operation is not permitted
   * eg, try to change static attribute
   */
  RK_ERR_NOT_PERM = 9,
  // invlalid pipe ID
  RK_ERR_INVALID_PIPEID = 10,
  // invlalid stitch group ID
  RK_ERR_INVALID_STITCHGRPID = 11,
  // failure caused by malloc memory
  RK_ERR_NOMEM = 12,
  // failure caused by malloc buffer
  RK_ERR_NOBUF = 13,
  // no data in buffer
  RK_ERR_BUF_EMPTY = 14,
  // no buffer for new data
  RK_ERR_BUF_FULL = 15,
  /*
   * System is not ready,maybe not initialed or
   * loaded. Returning the error code when opening
   * a device file failed.
   */
  RK_ERR_NOTREADY = 16,
  /*
   * bad address,
   * eg. used for copy_from_user & copy_to_user
   */
  RK_ERR_BADADDR = 17,
  /*
   * resource is busy,
   * eg. destroy a venc chn without unregister it
   */
  RK_ERR_BUSY = 18,
  // buffer size is smaller than the actual size required
  RK_ERR_SIZE_NOT_ENOUGH = 19,
  /*
   * maxium code, private error code of all modules
   * must be greater than it
   */
  RK_ERR_BUTT = 63,
} RK_ERR_CODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif // INCLUDE_RT_MPI_RK_ERRNO_H_
