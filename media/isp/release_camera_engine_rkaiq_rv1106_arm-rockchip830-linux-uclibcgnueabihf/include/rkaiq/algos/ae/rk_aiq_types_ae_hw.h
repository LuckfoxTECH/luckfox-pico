
#ifndef __RK_AIQ_TYPES_AE_HW_H__
#define __RK_AIQ_TYPES_AE_HW_H__

#define RAWAEBIG_SUBWIN_NUM 4
#define RAWAEBIG_WIN_NUM    225
#define RAWAELITE_WIN_NUM   25
#define YUVAE_SUBWIN_NUM    4
#define YUVAE_WIN_NUM       225

#define RAWHISTBIG_WIN_NUM  225
#define RAWHISTLITE_WIN_NUM 25
#define SIHIST_WINCFG_NUM   1
#define SIHIST_WIN_NUM      225

#define RAWHIST_BIN_N_MAX   256
#define SIHIST_BIN_N_MAX    32

#define MAX_AEC_EFFECT_FNUM 5

typedef enum _RAWAE_WND_NUM {
    RAWAE_WND_1X1 = 0,
    RAWAE_WND_5X5 = 1,
    RAWAE_WND_15X15 = 2,
} RAWAE_WND_NUM;

typedef enum _RAWHIST_WND_NUM {
    RAWHIST_WND_5X5 = 0,
    RAWHIST_WND_15X15 = 2,
} RAWHISST_WND_NUM;

typedef enum _AE_MODE {
    RAWAE_MODE_S_LITE    = 0,
    RAWAE_MODE_M_LITE    = 1,
    RAWAE_MODE_L_LITE    = 2
} AE_MODE;

typedef enum WinSplitMode_s {
    LEFT_AND_RIGHT_MODE = 0,
    LEFT_MODE,
    RIGHT_MODE,
} WinSplitMode;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC HW-Meas Config Params
 */
/*****************************************************************************/
#pragma pack(1)
typedef struct window {
    uint16_t h_offs;
    uint16_t v_offs;
    uint16_t h_size;
    uint16_t v_size;
} window_t;
#pragma pack()


#pragma pack(1)
typedef struct rawaebig_meas_cfg {
    unsigned char rawae_sel;
    unsigned char wnd_num;
    unsigned char subwin_en[RAWAEBIG_SUBWIN_NUM];
    window_t win;
    window_t subwin[RAWAEBIG_SUBWIN_NUM];
} rawaebig_meas_cfg_t;
#pragma pack()

#pragma pack(1)
typedef struct rawaelite_meas_cfg {
    unsigned char rawae_sel;
    unsigned char wnd_num;
    window_t win;
} rawaelite_meas_cfg_t;
#pragma pack()

#pragma pack(1)
typedef struct yuvae_meas_cfg {
    unsigned char ysel;
    unsigned char wnd_num;
    unsigned char subwin_en[YUVAE_SUBWIN_NUM];
    window_t win;
    window_t subwin[YUVAE_SUBWIN_NUM];
} yuvae_meas_cfg_t;
#pragma pack()


#pragma pack(1)
typedef struct rawhistbig_cfg {
    unsigned char wnd_num;
    unsigned char data_sel;
    unsigned char waterline;
    unsigned char mode;
    unsigned char stepsize;
    unsigned char off;
    unsigned char bcc;
    unsigned char gcc;
    unsigned char rcc;
    window_t win;
    unsigned char weight[RAWHISTBIG_WIN_NUM];
} rawhistbig_cfg_t;
#pragma pack()

#pragma pack(1)
typedef struct rawhistlite_cfg {
    unsigned char data_sel;
    unsigned char waterline;
    unsigned char mode;
    unsigned char stepsize;
    unsigned char off;
    unsigned char bcc;
    unsigned char gcc;
    unsigned char rcc;
    window_t win;
    unsigned char weight[RAWHISTLITE_WIN_NUM];
} rawhistlite_cfg_t;
#pragma pack()

#pragma pack(1)
typedef struct sihst_win_cfg {
    unsigned char  data_sel;
    unsigned char  waterline;
    unsigned char  auto_stop;
    unsigned char  mode;
    unsigned char  stepsize;
    window_t win;
} sihst_win_cfg_t;
#pragma pack()

#pragma pack(1)
typedef struct sihst_cfg {
    unsigned char wnd_num;
    sihst_win_cfg_t win_cfg[SIHIST_WINCFG_NUM];
    unsigned char hist_weight[SIHIST_WIN_NUM];
} sihst_cfg_t;
#pragma pack()

/*NOTE: name of rawae/rawhist channel has been renamed!
   RawAE0 = RawAE lite,  addr=0x4500  <=> RawHIST0
   RawAE1 = RawAE big2, addr=0x4600 <=> RawHIST1
   RawAE2 = RawAE big3, addr=0x4700 <=> RawHIST2
   RawAE3 = RawAE big1, addr=0x4400, extra aebig <=> RawHIST3
*/
typedef struct rk_aiq_ae_meas_params_s {
    bool   ae_meas_en;
    bool   ae_meas_update;
    rawaelite_meas_cfg_t rawae0;
    rawaebig_meas_cfg_t rawae1;
    rawaebig_meas_cfg_t rawae2;
    rawaebig_meas_cfg_t rawae3;
    yuvae_meas_cfg_t yuvae;
} rk_aiq_ae_meas_params_t;

typedef struct rk_aiq_hist_meas_params_s {
    bool   hist_meas_en;
    bool   hist_meas_update;
    unsigned char ae_swap; // used to choose LITE & BIG
    unsigned char ae_sel; // used for rawae3 & rawhist3
    rawhistlite_cfg_t rawhist0;
    rawhistbig_cfg_t rawhist1;
    rawhistbig_cfg_t rawhist2;
    rawhistbig_cfg_t rawhist3;
    sihst_cfg_t sihist;
} rk_aiq_hist_meas_params_t;
/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC AEC HW-Meas Res Params
 */
/*****************************************************************************/

typedef struct rawhist_stat {
    unsigned int bins[RAWHIST_BIN_N_MAX];
} rawhist_stat_t;

typedef struct sihist_stat_t {
    unsigned int bins[SIHIST_BIN_N_MAX];
} sihist_stat_t;

typedef struct rawaebig_stat {
    uint16_t channelr_xy[RAWAEBIG_WIN_NUM];
    uint16_t channelg_xy[RAWAEBIG_WIN_NUM];
    uint16_t channelb_xy[RAWAEBIG_WIN_NUM];
    uint16_t channely_xy[RAWAEBIG_WIN_NUM]; //not HW!
    uint16_t wndx_channelr[RAWAEBIG_SUBWIN_NUM]; //not HW!
    uint16_t wndx_channelg[RAWAEBIG_SUBWIN_NUM]; //not HW!
    uint16_t wndx_channelb[RAWAEBIG_SUBWIN_NUM]; //not HW!
    uint16_t wndx_channely[RAWAEBIG_SUBWIN_NUM]; //not HW!
    uint64_t wndx_sumr[RAWAEBIG_SUBWIN_NUM];
    uint64_t wndx_sumg[RAWAEBIG_SUBWIN_NUM];
    uint64_t wndx_sumb[RAWAEBIG_SUBWIN_NUM];
} rawaebig_stat_t;

typedef struct rawaelite_stat {
    uint16_t channelr_xy[RAWAELITE_WIN_NUM];
    uint16_t channelg_xy[RAWAELITE_WIN_NUM];
    uint16_t channelb_xy[RAWAELITE_WIN_NUM];
    uint16_t channely_xy[RAWAELITE_WIN_NUM]; //not HW!
} rawaelite_stat_t;

typedef struct yuvae_stat {
    unsigned char mean[YUVAE_WIN_NUM];
    uint64_t ro_yuvae_sumy[YUVAE_SUBWIN_NUM];
} yuvae_stat_t;

typedef struct Aec_Stat_Res_s {
    //rawae
    rawaebig_stat_t rawae_big;
    rawaelite_stat_t rawae_lite;
    //rawhist
    rawhist_stat_t rawhist_big;
    rawhist_stat_t rawhist_lite;
} Aec_Stat_Res_t;

typedef struct RkAiqAecHwConfig_s {
    rk_aiq_ae_meas_params_t ae_meas;
    rk_aiq_hist_meas_params_t hist_meas;
    int ae_swap;
    int ae_sel;
} RkAiqAecHwConfig_t;

typedef struct RkAiqAecHwStatsRes_s {
    Aec_Stat_Res_t chn[3];
    Aec_Stat_Res_t extra;
    yuvae_stat_t yuvae;
    sihist_stat_t sihist;
} RkAiqAecHwStatsRes_t;


/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC Exposure Params
 */
/*****************************************************************************/
typedef struct CISFeature_s {
    // M4_NUMBER_DESC("SNR", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(1))
    uint8_t SNR;
    // M4_NUMBER_DESC("DR", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(1))
    uint8_t DR;
    // M4_NUMBER_DESC("Sat", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(1))
    uint8_t Sat;
    // M4_NUMBER_DESC("SEN", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(1))
    uint8_t SEN;
} CISFeature_t;

typedef struct RkAiqExpRealParam_s {

    // M4_NUMBER_DESC("CISTime", "f32", M4_RANGE(0,1), "0", M4_DIGIT(6))
    float integration_time;

    // M4_NUMBER_DESC("CISGain", "f32", M4_RANGE(0,4096), "0", M4_DIGIT(3))
    float analog_gain;

    // M4_NUMBER_DESC("digital_gain", "f32", M4_RANGE(0,4096), "0", M4_DIGIT(3),M4_HIDE(1))
    float digital_gain;

    // M4_NUMBER_DESC("isp_dgain", "f32", M4_RANGE(0,256), "0", M4_DIGIT(3),M4_HIDE(1))
    float isp_dgain;

    // M4_NUMBER_DESC("iso", "s32", M4_RANGE(0,524288), "0", M4_DIGIT(0),M4_HIDE(1))
    int   iso;

    // M4_NUMBER_DESC("DcgMode", "s32", M4_RANGE(-1,1), "0", M4_DIGIT(0))
    int   dcg_mode;

    // M4_NUMBER_DESC("longfrm_mode", "s32", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(1))
    int   longfrm_mode;
} RkAiqExpRealParam_t;

typedef struct RkAiqExpSensorParam_s {

    // M4_NUMBER_DESC("fine_integration_time", "u32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned int fine_integration_time;

    // M4_NUMBER_DESC("coarse_integration_time", "u32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned int coarse_integration_time;

    // M4_NUMBER_DESC("analog_gain_code_global", "u32", M4_RANGE(0,524288), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned int analog_gain_code_global;

    // M4_NUMBER_DESC("digital_gain_global", "u32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned int digital_gain_global;

    // M4_NUMBER_DESC("isp_digital_gain", "u32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned int isp_digital_gain;
} RkAiqExpSensorParam_t;

#define MAX_I2CDATA_LEN 64
typedef struct RKAiqExpI2cParam_s {
    // M4_BOOL_DESC("bValid", "0",M4_HIDE(1))
    bool           bValid;

    // M4_NUMBER_DESC("nNumRegs", "u32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned int   nNumRegs;

    // M4_ARRAY_DESC("RegAddr", "u32", M4_SIZE(1,64), M4_RANGE(0,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0),M4_HIDE(1))
    unsigned int   RegAddr[MAX_I2CDATA_LEN];

    // M4_ARRAY_DESC("AddrByteNum", "u32", M4_SIZE(1,64), M4_RANGE(0,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0),M4_HIDE(1))
    unsigned int   AddrByteNum[MAX_I2CDATA_LEN];

    // M4_ARRAY_DESC("RegValue", "u32", M4_SIZE(1,64), M4_RANGE(0,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0),M4_HIDE(1))
    unsigned int   RegValue[MAX_I2CDATA_LEN];

    // M4_ARRAY_DESC("ValueByteNum", "u32", M4_SIZE(1,64), M4_RANGE(0,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0),M4_HIDE(1))
    unsigned int   ValueByteNum[MAX_I2CDATA_LEN];

    // M4_ARRAY_DESC("DelayFrames", "u32", M4_SIZE(1,64), M4_RANGE(0,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0),M4_HIDE(1))
    unsigned int   DelayFrames[MAX_I2CDATA_LEN];

} RKAiqExpI2cParam_t;

typedef struct {
    // M4_STRUCT_DESC("RealPara", "normal_ui_style")
    RkAiqExpRealParam_t exp_real_params; //real value

    // M4_STRUCT_DESC("RegPara", "normal_ui_style",M4_HIDE(1))
    RkAiqExpSensorParam_t exp_sensor_params;//reg value
} RkAiqExpParamComb_t;

typedef struct {
    // M4_BOOL_DESC("update", "0",M4_HIDE(1))
    bool           update;
    // M4_NUMBER_DESC("step", "s32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    int            step;
    // M4_NUMBER_DESC("gain", "s32", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(1))
    int            gain;
} RkAiqPIrisParam_t;

typedef struct {
    // M4_BOOL_DESC("update", "0",M4_HIDE(1))
    bool       update;
    // M4_NUMBER_DESC("pwmDuty", "s32", M4_RANGE(0,100), "0", M4_DIGIT(0),M4_HIDE(1))
    int        pwmDuty; //percent value,range = 0-100
} RkAiqDCIrisParam_t;

typedef struct {
    // M4_STRUCT_DESC("PIris", "normal_ui_style",M4_HIDE(1))
    RkAiqPIrisParam_t   PIris;
    // M4_STRUCT_DESC("DCIris", "normal_ui_style",M4_HIDE(1))
    RkAiqDCIrisParam_t  DCIris;
} RkAiqIrisParamComb_t;

typedef struct RKAiqAecExpInfo_s {

    // M4_STRUCT_DESC("LinearExp", "normal_ui_style")
    RkAiqExpParamComb_t LinearExp;

    // M4_STRUCT_LIST_DESC("HdrExp", M4_SIZE(1,3), "normal_ui_style")
    RkAiqExpParamComb_t HdrExp[3];

    // M4_STRUCT_DESC("Iris", "normal_ui_style",M4_HIDE(1))
    RkAiqIrisParamComb_t Iris;

    // M4_NUMBER_DESC("LineLengthPixels(hts)", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0))
    uint16_t line_length_pixels;

    // M4_NUMBER_DESC("FrameLengthLines(vts)", "u32", M4_RANGE(0,65535), "0", M4_DIGIT(0))
    uint32_t frame_length_lines;

    // M4_NUMBER_DESC("PixelClockFreqMhz", "f32", M4_RANGE(0,65535), "0", M4_DIGIT(2))
    float pixel_clock_freq_mhz;

    // M4_STRUCT_DESC("CISFeature_t", "normal_ui_style",M4_HIDE(1))
    CISFeature_t CISFeature;
} RKAiqAecExpInfo_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC transfer Params(from demo to rkaiq)
 */
/*****************************************************************************/

typedef struct RKAiqAecStats_s {
    RkAiqAecHwStatsRes_t ae_data;  //AeHwMeas_Res
    RKAiqAecExpInfo_t ae_exp; //AeExp_Info
} RKAiqAecStats_t;

typedef RkAiqAecHwConfig_t html_aec_para_t;

#endif /*__RK_AIQ_TYPES_AE_HW_H__*/
