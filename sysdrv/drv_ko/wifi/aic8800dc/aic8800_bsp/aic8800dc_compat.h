#ifndef _AIC8800DC_COMPAT_H_
#define _AIC8800DC_COMPAT_H_

#include "aicsdio.h"
typedef u32 (*array2_tbl_t)[2];
typedef u32 (*array3_tbl_t)[3];

typedef uint8_t u8_l;
typedef int8_t s8_l;
typedef bool bool_l;
typedef uint16_t u16_l;
typedef int16_t s16_l;
typedef uint32_t u32_l;
typedef int32_t s32_l;
typedef uint64_t u64_l;

extern u8 chip_sub_id;
extern u8 chip_mcu_id;
#define FW_PATH_MAX_LEN 200

void aicwf_patch_config_8800dc(struct          aic_sdio_dev *rwnx_hw);
void system_config_8800dc(struct aic_sdio_dev *rwnx_hw);
int aicwf_misc_ram_init_8800dc(struct aic_sdio_dev *sdiodev);

#ifdef CONFIG_DPD
int aicwf_dpd_calib_8800dc(struct aic_sdio_dev *sdiodev, rf_misc_ram_lite_t *dpd_res);
int aicwf_dpd_result_apply_8800dc(struct aic_sdio_dev *sdiodev, rf_misc_ram_lite_t *dpd_res);
#ifndef CONFIG_FORCE_DPD_CALIB
int aicwf_dpd_result_load_8800dc(struct aic_sdio_dev *sdiodev, rf_misc_ram_lite_t *dpd_res);
int aicwf_dpd_result_write_8800dc(void *buf, int buf_len);
#endif/* !CONFIG_FORCE_DPD_CALIB */
#endif

#endif



