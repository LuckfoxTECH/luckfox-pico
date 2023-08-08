
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

#define MAX_AEC_EFFECT_FNUM 10

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

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC HW-Meas Config Params
 */
/*****************************************************************************/
#pragma pack(1)
struct window {
    unsigned short h_offs;
    unsigned short v_offs;
    unsigned short h_size;
    unsigned short v_size;
};
#pragma pack()


#pragma pack(1)
struct rawaebig_meas_cfg {
    unsigned char rawae_sel;
    unsigned char wnd_num;
    unsigned char subwin_en[RAWAEBIG_SUBWIN_NUM];
    struct window win;
    struct window subwin[RAWAEBIG_SUBWIN_NUM];
};
#pragma pack()

#pragma pack(1)
struct rawaelite_meas_cfg {
    unsigned char rawae_sel;
    unsigned char wnd_num;
    struct window win;
};
#pragma pack()

#pragma pack(1)
struct yuvae_meas_cfg {
    unsigned char ysel;
    unsigned char wnd_num;
    unsigned char subwin_en[YUVAE_SUBWIN_NUM];
    struct window win;
    struct window subwin[YUVAE_SUBWIN_NUM];
};
#pragma pack()


#pragma pack(1)
struct rawhistbig_cfg {
    unsigned char wnd_num;
    unsigned char data_sel;
    unsigned char waterline;
    unsigned char mode;
    unsigned char stepsize;
    unsigned char off;
    unsigned char bcc;
    unsigned char gcc;
    unsigned char rcc;
    struct window win;
    unsigned char weight[RAWHISTBIG_WIN_NUM];
};
#pragma pack()

#pragma pack(1)
struct rawhistlite_cfg {
    unsigned char data_sel;
    unsigned char waterline;
    unsigned char mode;
    unsigned char stepsize;
    unsigned char off;
    unsigned char bcc;
    unsigned char gcc;
    unsigned char rcc;
    struct window win;
    unsigned char weight[RAWHISTLITE_WIN_NUM];
};
#pragma pack()

#pragma pack(1)
struct sihst_win_cfg {
    unsigned char  data_sel;
    unsigned char  waterline;
    unsigned char  auto_stop;
    unsigned char  mode;
    unsigned char  stepsize;
    struct window win;
};
#pragma pack()

#pragma pack(1)
struct sihst_cfg {
    unsigned char wnd_num;
    struct sihst_win_cfg win_cfg[SIHIST_WINCFG_NUM];
    unsigned char hist_weight[SIHIST_WIN_NUM];
};
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
    struct rawaelite_meas_cfg rawae0;
    struct rawaebig_meas_cfg rawae1;
    struct rawaebig_meas_cfg rawae2;
    struct rawaebig_meas_cfg rawae3;
    struct yuvae_meas_cfg yuvae;
} rk_aiq_ae_meas_params_t;

typedef struct rk_aiq_hist_meas_params_s {
    bool   hist_meas_en;
    bool   hist_meas_update;
    struct rawhistlite_cfg rawhist0;
    struct rawhistbig_cfg rawhist1;
    struct rawhistbig_cfg rawhist2;
    struct rawhistbig_cfg rawhist3;
    struct sihst_cfg sihist;
} rk_aiq_hist_meas_params_t;
/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC AEC HW-Meas Res Params
 */
/*****************************************************************************/

struct rawhist_stat {
    unsigned int bins[RAWHIST_BIN_N_MAX];
};
struct sihist_stat {
    unsigned int bins[SIHIST_BIN_N_MAX];
};

struct rawaebig_stat {
    unsigned short channelr_xy[RAWAEBIG_WIN_NUM];
    unsigned short channelg_xy[RAWAEBIG_WIN_NUM];
    unsigned short channelb_xy[RAWAEBIG_WIN_NUM];
    unsigned short channely_xy[RAWAEBIG_WIN_NUM]; //not HW!
    unsigned long int wndx_sumr[RAWAEBIG_SUBWIN_NUM];
    unsigned long int wndx_sumg[RAWAEBIG_SUBWIN_NUM];
    unsigned long int wndx_sumb[RAWAEBIG_SUBWIN_NUM];
    unsigned short wndx_channelr[RAWAEBIG_SUBWIN_NUM]; //not HW!
    unsigned short wndx_channelg[RAWAEBIG_SUBWIN_NUM]; //not HW!
    unsigned short wndx_channelb[RAWAEBIG_SUBWIN_NUM]; //not HW!
    unsigned short wndx_channely[RAWAEBIG_SUBWIN_NUM]; //not HW!
};
struct rawaelite_stat {
    unsigned short channelr_xy[RAWAELITE_WIN_NUM];
    unsigned short channelg_xy[RAWAELITE_WIN_NUM];
    unsigned short channelb_xy[RAWAELITE_WIN_NUM];
    unsigned short channely_xy[RAWAELITE_WIN_NUM]; //not HW!
};

struct yuvae_stat {
    unsigned long int ro_yuvae_sumy[YUVAE_SUBWIN_NUM];
    unsigned char mean[YUVAE_WIN_NUM];
};

typedef struct Aec_Stat_Res_s {
    //rawae
    struct rawaebig_stat rawae_big;
    struct rawaelite_stat rawae_lite;
    //rawhist
    struct rawhist_stat rawhist_big;
    struct rawhist_stat rawhist_lite;
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
    struct yuvae_stat yuvae;
    struct sihist_stat sihist;
} RkAiqAecHwStatsRes_t;


/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC Exposure Params
 */
/*****************************************************************************/
typedef struct CISFeature_s {
    uint8_t SNR;
    uint8_t DR;
    uint8_t Sat;
    uint8_t SEN;
} CISFeature_t;

typedef struct RkAiqExpRealParam_s {
    float integration_time;
    float analog_gain;
    float digital_gain;
    float isp_dgain;
    int   iso;
    int   dcg_mode;
} RkAiqExpRealParam_t;

typedef struct RkAiqExpSensorParam_s {
    unsigned int fine_integration_time;
    unsigned int coarse_integration_time;
    unsigned int analog_gain_code_global;
    unsigned int digital_gain_global;
    unsigned int isp_digital_gain;
} RkAiqExpSensorParam_t;

typedef struct {
    RkAiqExpRealParam_t exp_real_params; //real value
    RkAiqExpSensorParam_t exp_sensor_params;//reg value
} RkAiqExpParamComb_t;

typedef struct {
    int            step;
    int            gain;
    bool           update;
} RkAiqPIrisParam_t;

typedef struct {
    int        pwmDuty; //percent value,range = 0-100
    bool       update;
} RkAiqDCIrisParam_t;

typedef struct {
    RkAiqPIrisParam_t   PIris;
    RkAiqDCIrisParam_t  DCIris;
} RkAiqIrisParamComb_t;

typedef struct RKAiqAecExpInfo_s {
    RkAiqExpParamComb_t LinearExp;
    RkAiqExpParamComb_t HdrExp[3];
    RkAiqIrisParamComb_t Iris;
    unsigned short line_length_pixels;
    unsigned short frame_length_lines;
    float pixel_clock_freq_mhz;
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
