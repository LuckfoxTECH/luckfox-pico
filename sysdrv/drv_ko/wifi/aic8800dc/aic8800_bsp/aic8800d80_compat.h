#ifndef _AIC8800D80_COMPAT_H_
#define _AIC8800D80_COMPAT_H_

#include "aicsdio.h"
/*typedef u32 (*array2_tbl_t)[2];

typedef uint8_t u8_l;
typedef int8_t s8_l;
typedef bool bool_l;
typedef uint16_t u16_l;
typedef int16_t s16_l;
typedef uint32_t u32_l;
typedef int32_t s32_l;
typedef uint64_t u64_l;*/

int aicbsp_system_config_8800d80(struct aic_sdio_dev *sdiodev);
int aicwifi_sys_config_8800d80(struct aic_sdio_dev *sdiodev);
int aicwifi_patch_config_8800d80(struct aic_sdio_dev *sdiodev);


#endif



