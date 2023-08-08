#ifndef _RF_TABLE_H_
#define _RF_TABLE_H_

#ifndef SSV_PACKED_STRUCT
//SSV PACK Definition
#define SSV_PACKED_STRUCT_BEGIN
#define SSV_PACKED_STRUCT               //__attribute__ ((packed))
#define SSV_PACKED_STRUCT_END           //__attribute__((packed))
#define SSV_PACKED_STRUCT_STRUCT        __attribute__ ((packed))
#define SSV_PACKED_STRUCT_FIELD(x)      x
#endif

SSV_PACKED_STRUCT_BEGIN
struct st_tempe_table
{
    uint8_t band_gain[7];   //0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14

    uint8_t freq_xi;
    uint8_t freq_xo;

    uint8_t ldo_rxafe;
    uint8_t ldo_dcdcv;
    uint8_t ldo_dldov;

    uint8_t pa_vcas1;
    uint8_t pa_vcas2;
    uint8_t pa_vcas3;
    uint8_t pa_bias;
    uint8_t pa_cap;

    uint8_t padpd_cali;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct st_rate_gain
{
    uint8_t rate1;
    uint8_t rate2;
    uint8_t rate3;
    uint8_t rate4;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct st_tempe_5g_table
{
    uint8_t bbscale_band0;   //Band0 in 0xccb0ada8(31-24)
    uint8_t bbscale_band1;   //Band1 in 0xccb0ada8(23-16)
    uint8_t bbscale_band2;   //Band2 in 0xccb0ada8(15- 8)
    uint8_t bbscale_band3;   //Band3 in 0xccb0ada8( 7- 0)
    uint32_t bias1;                  // Band0 in 0xccb0a62c(15-0), Band1 in 0xccb0a62c(31-16)
    uint32_t bias2;                  // Band2 in 0xccb0a630(15-0), Band3 in 0xccb0a630(31-16)
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct st_extpa_table
{
    uint8_t extpa_en;
    uint8_t lna_trigger_2G;
    uint8_t lna_trigger_5G;
    uint8_t rssi_lna_on_offset;
    uint8_t rssi_lna_off_offset;
    uint8_t rssi_5g_lna_on_offset;
    uint8_t rssi_5g_lna_off_offset;
    uint8_t reserved;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct st_rf_table
{
    struct st_tempe_table rt_config;
    struct st_tempe_table ht_config;
    struct st_tempe_table lt_config;

    uint8_t rf_gain;
    uint8_t rate_gain_b;

    struct st_rate_gain rate_config_g;

    struct st_rate_gain rate_config_20n;

    struct st_rate_gain rate_config_40n;
    int8_t low_boundary;
    int8_t high_boundary;
    /*0xFF: EN_FIRST_BOOT, 0:EN_NOT_FIST_BOOT*/
    uint8_t boot_flag;
    /*0:EN_WORK_NOMAL, 1:EN_WORK_ENGINEER*/
    uint8_t work_mode;
    struct st_tempe_5g_table rt_5g_config;
    struct st_tempe_5g_table ht_5g_config;
    struct st_tempe_5g_table lt_5g_config;
    uint16_t band_f0_threshold;
    uint16_t band_f1_threshold;
    uint16_t band_f2_threshold;
    /* Extend structure members. */
    uint8_t signature[4]; // For check whether the structure exists in flash.
    uint32_t version; // Store structure version, it should be incremented after each structure change.
    uint32_t dcdc_flag; // V.01: DCDC enable flag, 0: Disable 1: Enable
    struct st_extpa_table extpa_tbl;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

enum{
    EN_FIRST_BOOT=0xFF,
    EN_NOT_FIRST_BOOT=0
};

enum{
    EN_WORK_NOMAL=0,
    EN_WORK_ENGINEER
};

#define RF_API_SIGNATURE            "SSV"
#define RF_API_TABLE_VERSION        (0x0002) // It should be incremented after structure st_rf_table change.

#endif
