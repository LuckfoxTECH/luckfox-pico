#include "sensor_2a_common.h"

struct ae_awb_init_cfg g_2a_init_info = {
    .head = 0,
    .len = 0,
    .crc32 = 0,
    .ae = {
        .black_lvl = 256,
        .adc_calib_type = 0,
        .adc_range_type = 0,
        .adc_calib = {701, 337, 909, 753},
        .start_exp = {1, 10, 100},
    },
    .awb = {
        .rg_gain_base = FIX2INT16(1.0),
        .bg_gain_base = FIX2INT16(1.0),
        .awb_gain_rad = FIX2INT16(1.0),
        .awb_gain_dis = FIX2INT16(1.0),
        //.reserve = {0x0}
    },
    .rtt_res = {
        .day_or_night = 0, // 0: invalid 1:day 2:night
        .awb_gain = 0, //0~-15bit: rgain 16~31bit: bgain
        .exp = 0,
        .isp_dgain = 0,
        .reg_time = 0,
        .reg_gain = 0,
        .dcg_mode = 0,
        .ae_stats = {0x0}, // only G channel (l6bit)
        .ae_diff_th = 20,
        .nr_diff_th = 25,
        .last_lodif_0 = 0,
        .last_lodif_1 = 0,
        .last_hidif_0 = 0,
        .last_hidif_1 = 0,
    }
};
