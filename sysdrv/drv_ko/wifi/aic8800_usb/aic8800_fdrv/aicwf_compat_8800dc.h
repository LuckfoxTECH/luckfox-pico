#include <linux/types.h>

#ifdef CONFIG_DPD
typedef struct {
    uint32_t bit_mask[3];
    uint32_t reserved;
    uint32_t dpd_high[96];
    uint32_t dpd_11b[96];
    uint32_t dpd_low[96];
    uint32_t idac_11b[48];
    uint32_t idac_high[48];
    uint32_t idac_low[48];
    uint32_t loft_res[18];
    uint32_t rx_iqim_res[16];
} rf_misc_ram_t;

typedef struct {
    uint32_t bit_mask[4];
    uint32_t dpd_high[96];
    uint32_t loft_res[18];
} rf_misc_ram_lite_t;

#define MEMBER_SIZE(type, member)   sizeof(((type *)0)->member)
#define DPD_RESULT_SIZE_8800DC      sizeof(rf_misc_ram_lite_t)

extern rf_misc_ram_lite_t dpd_res;
#endif

int aicwf_patch_table_load(struct rwnx_hw *rwnx_hw, char *filename);
void aicwf_patch_config_8800dc(struct rwnx_hw *rwnx_hw);
int aicwf_set_rf_config_8800dc(struct rwnx_hw *rwnx_hw, struct mm_set_rf_calib_cfm *cfm);
int aicwf_misc_ram_init_8800dc(struct rwnx_hw *rwnx_hw);
#ifdef CONFIG_DPD
int aicwf_misc_ram_valid_check_8800dc(struct rwnx_hw *rwnx_hw, int *valid_out);
int aicwf_plat_calib_load_8800dc(struct rwnx_hw *rwnx_hw);
int aicwf_dpd_calib_8800dc(struct rwnx_hw *rwnx_hw, rf_misc_ram_lite_t *dpd_res);
int aicwf_dpd_result_apply_8800dc(struct rwnx_hw *rwnx_hw, rf_misc_ram_lite_t *dpd_res);
#ifndef CONFIG_FORCE_DPD_CALIB
int aicwf_dpd_result_load_8800dc(struct rwnx_hw *rwnx_hw, rf_misc_ram_lite_t *dpd_res);
int aicwf_dpd_result_write_8800dc(void *buf, int buf_len);
#endif
#endif
int aicwf_plat_patch_load_8800dc(struct rwnx_hw *rwnx_hw);
int aicwf_plat_rftest_load_8800dc(struct rwnx_hw *rwnx_hw);
int	rwnx_plat_userconfig_load_8800dc(struct rwnx_hw *rwnx_hw);
int	rwnx_plat_userconfig_load_8800dw(struct rwnx_hw *rwnx_hw);
void system_config_8800dc(struct rwnx_hw *rwnx_hw);
