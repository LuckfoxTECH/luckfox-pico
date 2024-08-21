#ifndef _SSV_PHY_RF_H_
#define _SSV_PHY_RF_H_


#include "ssvdevice/dev.h"


#ifndef SSV_PACKED_STRUCT
//SSV PACK Definitionf
#define SSV_PACKED_STRUCT_BEGIN
#define SSV_PACKED_STRUCT               //__attribute__ ((packed))
#define SSV_PACKED_STRUCT_END           //__attribute__((packed))
#define SSV_PACKED_STRUCT_STRUCT        __attribute__ ((packed))
#define SSV_PACKED_STRUCT_FIELD(x)      x
#endif


int ssv_update_rf_conf_table(struct ssv_hw *sh);
void ssv_chg_xtal_freq_offset(struct ssv_hw *sh, u8 xtal);
void ssv_chg_band_gain(struct ssv_hw *sh, u8 power_ch_1_7, u8 power_ch_8_14);
void ssv_chg_gain_b_mode(struct ssv_hw *sh, u8 gain);
void ssv_chg_gain_g_mode(struct ssv_hw *sh, u8 gain);
void ssv_chg_gain_n_mode(struct ssv_hw *sh, u8 bw20_gain,  u8 bw40_gain);
void ssv_chg_gain_su_mode(struct ssv_hw *sh, u8 bw20_gain,  u8 bw40_gain);
void ssv_chg_gain_tb_mode(struct ssv_hw *sh, u8 bw20_gain,  u8 bw40_gain);
void ssv_chg_ble_power(struct ssv_hw *sh, u32 ble_power);
void ssv_chg_thermal_boundary(struct ssv_hw *sh, int *boundary);
void ssv_chg_ref_power(struct ssv_hw *sh, u8 ref_power);
int ssv_send_priv_msg_rf_calib_result_backup(struct ssv_softc *sc);
#if 0
int ssv_send_priv_msg_agc_reg_table(struct ssv_softc *sc);
int ssv_send_priv_msg_rfphy_reg_talbe(struct ssv_softc *sc);
#endif


int ssv_custom_modify_rf_conf_table(struct ssv_hw *sh);
int ssv_kdump_rf_conf_table(struct ssv_hw *sh);
int ssv_dump_rf_conf_table(struct ssv_hw *sh);
int ssv_set_rf_conf_table(struct ssv_hw *sh);
int ssv_set_pll_phy_rf(struct ssv_hw *sh);
void ssv_6030_rf(struct ssv_hw *sh, int argc, char *argv[]);
void ssv_6030_rfble(struct ssv_hw *sh, int argc, char *argv[]);
int ssv_res_rf_update_to_cfg(void* param);
#endif
