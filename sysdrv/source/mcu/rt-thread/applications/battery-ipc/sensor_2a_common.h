#ifndef __SENSOR_2A_COMMON_H__
#define __SENSOR_2A_COMMON_H__

#define ADC_CALIB_NO          4
#define ADC_CALIB_TBL_LEN     15
#define AE_START_EXP_NO       3
#define AE_RESERVE_NO         4
#define AWB_RESERVE_NO        60
#define FIX2INT16(x)          (int)(x * (1 << 16))
#define AE_GRID_STATS         113

struct ae_init_cfg {
    uint32_t black_lvl; //blc: 12bit
    uint32_t adc_calib_type; //0: Formula 1: LUT(look up table)
    uint32_t adc_range_type; //0: dual 1: single
    uint32_t adc_calib[ADC_CALIB_NO]; //adc_calib[0]=lowadc_lux0, adc_calib[1]=lowadc_lux100, adc_calib[2]=highadc_lux0, adc_calib[3]=highadc_lux1000, 10bit
    uint32_t adc_table_len;
    uint32_t adc_table_low[ADC_CALIB_TBL_LEN][2];
    uint32_t adc_table_high[ADC_CALIB_TBL_LEN][2];
    uint32_t start_exp[AE_START_EXP_NO]; //typical: start_exp[0]=1lux, start_exp[1]=10lux, start_exp[2]=100lux
};

struct awb_init_cfg {
    uint32_t rg_gain_base; //used for soft light sensor, need float => FIX2INT16
    uint32_t bg_gain_base;
    uint32_t awb_gain_rad;
    uint32_t awb_gain_dis;
    //int reserve[AWB_RESERVE_NO];
};

struct rtt_2a_res {
    uint32_t day_or_night; // 0: invalid l:day 2:night
    uint32_t awb_gain; //0~-15bit: rgain 16~31bit: bgain
    uint32_t exp;
    uint32_t isp_dgain;
    uint32_t reg_time;
    uint32_t reg_gain;
    uint32_t dcg_mode;
    uint32_t ae_stats[AE_GRID_STATS]; // only G channel (l6bit)
    uint32_t ae_diff_th;//unit:%
    uint32_t nr_diff_th;//unit:%
    uint32_t last_lodif_0;
    uint32_t last_lodif_1;
    uint32_t last_hidif_0;
    uint32_t last_hidif_1;
};

struct ae_awb_init_cfg {
    uint32_t head;
    int32_t len;
    uint32_t crc32;
    struct ae_init_cfg ae;
    struct awb_init_cfg awb;
    struct rtt_2a_res rtt_res;
};
#endif

