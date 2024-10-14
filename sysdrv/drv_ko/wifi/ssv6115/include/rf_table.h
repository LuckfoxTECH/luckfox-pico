#ifndef _RF_TABLE_H_
#define _RF_TABLE_H_


#define RF_API_SIGNATURE            "SSV"
#define RF_API_TABLE_VERSION        (0x0001) // It should be incremented after structure st_rf_table change.


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
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t reserved4;
    uint8_t reserved5;
    uint8_t reserved6;
    uint8_t reserved7;
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

#define BLE_CHANNEL_PART_COUNT 2 /*Part0 : channel 0 ~ 19,  Part1 channel: 20 ~ 39*/
#define BLE_CHANNEL_BOUNDARY 19

SSV_PACKED_STRUCT_BEGIN
struct st_ble_tempe_table
{
    uint8_t band_gain[BLE_CHANNEL_PART_COUNT];
    uint8_t reserved[2];
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

SSV_PACKED_STRUCT_BEGIN
struct st_rate_gain
{
    uint8_t gain[33];
    uint8_t reserved[7];
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

//rf_conf_table
#define RF_THREMAL_TABLE_NUM                  (5)
//#define RF_TABLE_EXTPADPD_NUM                (58)

SSV_PACKED_STRUCT_BEGIN
struct st_rf_table
{
    uint8_t signature[4];
    uint32_t version;

    uint8_t boot_flag;          /*0xFF: EN_FIRST_BOOT, 0:EN_NOT_FIST_BOOT*/
    uint8_t work_mode;       /*0:EN_WORK_NOMAL, 1:EN_WORK_ENGINEER*/
    uint8_t reserved8_1;
    uint8_t reserved8_2;

    struct st_tempe_table rf_config[RF_THREMAL_TABLE_NUM];
    struct st_ble_tempe_table ble_rf_config[RF_THREMAL_TABLE_NUM];
    int8_t temperature_boundary[RF_THREMAL_TABLE_NUM-1];
    struct st_rate_gain rate_gain_table;

    uint8_t   abs_power;
    uint8_t ble_dtm_enable;
    uint8_t dcdc_flag; // V.01: DCDC enable flag, 0: Disable 1: Enable
    uint8_t padpd_flag;
    uint32_t  resverd32_1;
    uint32_t  resverd32_2;
    uint32_t  resverd32_3;
    uint32_t  resverd32_4;
    //struct st_extpa_table extpa_tbl;
    //uint8_t rtc32k_xtal_setting; // 32768 source, 0: internal_RC, 1: XTAL, 2: external
    //uint8_t tx_lowpower_flag; // 0 tx_lowpower_mode
    //uint8_t extpadpd_flag; //dadpd_enable flag, 0:Disable 1:Enable
    //uint16_t extpadpd_setting[RF_TABLE_EXTPADPD_NUM];
}SSV_PACKED_STRUCT_STRUCT;
SSV_PACKED_STRUCT_END

enum{
    EN_FIRST_BOOT=0xFF,
    EN_NOT_FIRST_BOOT=0,
};

enum{
    EN_WORK_NOMAL=0,
    EN_WORK_ENGINEER
};

#endif
