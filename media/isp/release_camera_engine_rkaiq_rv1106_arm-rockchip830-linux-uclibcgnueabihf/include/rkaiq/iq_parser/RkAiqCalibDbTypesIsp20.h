#ifndef _RK_AIQ_CALIB_TYPES_ISP20_H_
#define _RK_AIQ_CALIB_TYPES_ISP20_H_

#pragma pack(4)

#define CALIBDB_MAX_ISO_LEVEL 13
#define CALIBDB_NR_SHARP_MAX_ISO_LEVEL CALIBDB_MAX_ISO_LEVEL
#define CALIBDB_DPCC_MAX_ISO_LEVEL CALIBDB_MAX_ISO_LEVEL
#define CALIBDB_BLC_MAX_ISO_LEVEL CALIBDB_MAX_ISO_LEVEL
#define CALIBDB_NR_SHARP_SETTING_LEVEL 6
#define CALIBDB_MAX_MODE_NUM 5
#define CALIBDB_MAX_MODE_NAME_LENGTH (20)
//TODO: define max size as 256 temporarily
//#define CALIBDB_ZOOM_FOCUS_TBL_SIZE 10240
#define CALIBDB_ZOOM_FOCUS_TBL_SIZE 256
#define CALIBDB_DEGAMMA_CRUVE_KNOTS 17

#define CALIBDB_MAX_SCENE_NAME_LENGTH (10)
/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC Algo Params
 */
/*****************************************************************************/

#define AEC_ECM_SCHEME_NAME         ( 20U )
typedef char                        AecEcmSchemeName_t[AEC_ECM_SCHEME_NAME];

#define AEC_ECM_PROFILE_NAME        ( 20U )
typedef char                        AecEcmProfileName_t[AEC_ECM_PROFILE_NAME];

#define AEC_DYNAMIC_SETPOINT_NAME   ( 20U )
typedef char                        AecDynamicSetpointName_t[AEC_DYNAMIC_SETPOINT_NAME];

#define AEC_EXP_SEPARATE_NAME       ( 20U )
typedef char                        AecExpSeparateName_t[AEC_EXP_SEPARATE_NAME];

#define AEC_RAWAEBIG_WIN_NUM (15*15)
#define AEC_RAWAELITE_WIN_NUM (5*5)

typedef enum _CalibDb_HdrAeRatioType_e {
    RKAIQ_HDRAE_RATIOTYPE_MODE_INVALID    = 0,
    RKAIQ_HDRAE_RATIOTYPE_MODE_AUTO   = 1,
    RKAIQ_HDRAE_RATIOTYPE_MODE_FIX    = 2,
    RKAIQ_HDRAE_RATIOTYPE_MODE_MAX
} CalibDb_HdrAeRatioType_t;

typedef enum _CalibDb_AeStrategyMode_e {
    RKAIQ_AEC_STRATEGY_MODE_AUTO    = 0,
    RKAIQ_AEC_STRATEGY_MODE_LOWLIGHT_PRIOR     = 1,
    RKAIQ_AEC_STRATEGY_MODE_HIGHLIGHT_PRIOR    = 2,
    RKAIQ_AEC_STRATEGY_MODE_MAX
} CalibDb_AeStrategyMode_t;

typedef enum _CalibDb_AeHdrLongFrmMode_e {
    RKAIQ_AEC_HDR_LONGFRMMODE_NORMAL             = 0,
    RKAIQ_AEC_HDR_LONGFRMMODE_AUTO_LONG_FRAME    = 1,
    RKAIQ_AEC_HDR_LONGFRMMODE_LONG_FRAME         = 2,
} CalibDb_AeHdrLongFrmMode_t;

typedef enum _CalibDb_AecDayNightMode_e {
    AEC_DNMODE_MIN = -1,
    AEC_DNMODE_DAY = 0,
    AEC_DNMODE_NIGHT = 1, /*night mode or IR night-vision mode*/
    AEC_DNMODE_MAX = 2,
} CalibDb_AecDayNightMode_t;

/*****************************************************************************/
/**
 *          AecSemMode_t
 *
 * @brief   mode type of AEC Scene Evaluation
 *
 */
/*****************************************************************************/
typedef enum _CalibDb_AecSemMode_e {
    AEC_SCENE_EVALUATION_INVALID    = 0,        /* invalid (only used for initialization) */
    AEC_SCENE_EVALUATION_DISABLED   = 1,        /* Scene Evaluation disabled (fix setpoint) */
    AEC_SCENE_EVALUATION_FIX        = 2,        /* Scene Evaluation fix (static ROI) */
    AEC_SCENE_EVALUATION_ADAPTIVE   = 3,        /* Scene Evaluation adaptive (ROI caluclated by Scene Evaluation */
    AEC_SCENE_EVALUATION_MAX
} CalibDb_AecSemMode_t;
/*****************************************************************************/
/**
 *          AecBackLitMode_t
 *
 * @brief   mode type of AEC-BackLit
 *
 */
/*****************************************************************************/
typedef enum _CalibDb_AecMeasAreaMode_e {
    AEC_MEASURE_AREA_AUTO    = 0,
    AEC_MEASURE_AREA_UP,
    AEC_MEASURE_AREA_BOTTOM,
    AEC_MEASURE_AREA_LEFT,
    AEC_MEASURE_AREA_RIGHT,
    AEC_MEASURE_AREA_CENTER,
} CalibDb_AecMeasAreaMode_t;

/*****************************************************************************/
typedef enum _CalibDb_AecECMMode_e {
    AEC_ECM_MODE_INVALID     = 0,    /**< invalid exposure conversion module */
    AEC_ECM_MODE_1   = 1,  /* the original exposure conversion module ,   minimize the value of gain   */
    AEC_ECM_MODE_2    = 2,    /* the newl exposure conversion module ,imcrease the frame rate */
    AEC_ECM_MODE_MAX,
} CalibDb_AecECMMode_t;

typedef enum _CalibDb_FlickerFreq_e {
    AEC_FLICKER_FREQUENCY_OFF   = 0,
    AEC_FLICKER_FREQUENCY_50HZ = 1,
    AEC_FLICKER_FREQUENCY_60HZ = 2,
} CalibDb_FlickerFreq_t;

typedef enum _CalibDb_AntiFlickerMode_e {
    AEC_ANTIFLICKER_NORMAL_MODE = 0,
    AEC_ANTIFLICKER_AUTO_MODE = 1,
} CalibDb_AntiFlickerMode_t;

typedef enum _CalibDb_IrisType_e {
    IRIS_DC_TYPE = 0,
    IRIS_P_TYPE = 1,
    IRIS_INVALID_TYPE,
} CalibDb_IrisType_t;

/*****************************************************************************/
/**
 * @brief   Enumeration type to configure CamerIC ISP exposure measuring mode.
 *
 *****************************************************************************/
typedef enum _CalibDb_CamYRangeMode_e {
    CAM_YRANGE_MODE_INVALID    = 0,    /**< invalid y range mode   */
    CAM_YRANGE_MODE_FULL          = 1,    /**< Y = 0.299 * R + 0.587 * G + 0.114 * B,full range BT601*/
    CAM_YRANGE_MODE_LIMITED          = 2,    /**< Y = 16 + 0.25R + 0.5G + 0.1094B,limited range*/
    CAM_YRANGE_MODE_MAX,
} CalibDb_CamYRangeMode_t;

typedef enum _CalibDb_CamRawStatsMode_e {
    CAM_RAWSTATS_MODE_INVALID       = 0,    /**< lower border (only for an internal evaluation) */
    CAM_RAWSTATS_MODE_R             = 1,    /**< R channel */
    CAM_RAWSTATS_MODE_G             = 2,    /**< G channel */
    CAM_RAWSTATS_MODE_B             = 3,    /**< B channel */
    CAM_RAWSTATS_MODE_Y             = 4,    /**< luminance channel */
    CAM_RAWSTATS_MODE_MAX,                                  /**< upper border (only for an internal evaluation) */
} CalibDb_CamRawStatsMode_t;

typedef enum _CalibDb_CamHistStatsMode_e {
    CAM_HIST_MODE_INVALID       = 0,    /**< lower border (only for an internal evaluation) */
    CAM_HIST_MODE_RGB_COMBINED  = 1,    /**< RGB combined histogram, only available for SiHist */
    CAM_HIST_MODE_R             = 2,    /**< R histogram */
    CAM_HIST_MODE_G             = 3,    /**< G histogram */
    CAM_HIST_MODE_B             = 4,    /**< B histogram */
    CAM_HIST_MODE_Y             = 5,    /**< Y luminance histogram */
    CAM_HIST_MODE_MAX,                  /**< upper border (only for an internal evaluation) */
} CalibDb_CamHistStatsMode_t;

typedef struct CalibDb_Aec_Win_s {
    unsigned short h_offs;
    unsigned short v_offs;
    unsigned short h_size;
    unsigned short v_size;
} CalibDb_Aec_Win_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC API Exp-Route Params
 */
/*****************************************************************************/
#define AEC_ROUTE_MAX_NODES (10)
typedef struct CalibDb_LinAeRoute_Attr_s {
    AecExpSeparateName_t     name;                       /**name */
    float                    TimeDot[AEC_ROUTE_MAX_NODES];
    float                    GainDot[AEC_ROUTE_MAX_NODES];
    float                    IspgainDot[AEC_ROUTE_MAX_NODES];
    int                      PIrisGainDot[AEC_ROUTE_MAX_NODES];
    int                      array_size;
} CalibDb_LinAeRoute_Attr_t;

typedef struct CalibDb_HdrAeRoute_Attr_s {
    AecExpSeparateName_t     name;                       /**name */
    float                    HdrTimeDot[3][AEC_ROUTE_MAX_NODES];
    float                    HdrGainDot[3][AEC_ROUTE_MAX_NODES];
    float                    HdrIspDGainDot[3][AEC_ROUTE_MAX_NODES];
    int                      PIrisGainDot[AEC_ROUTE_MAX_NODES];
    int                      array_size;
} CalibDb_HdrAeRoute_Attr_t;

typedef struct CalibDb_AeRoute_Attr_s {
    CalibDb_LinAeRoute_Attr_t LinAeSeperate[AEC_DNMODE_MAX];
    CalibDb_HdrAeRoute_Attr_t HdrAeSeperate[AEC_DNMODE_MAX];
} CalibDb_AeRoute_Attr_t;

typedef struct CalibDb_AeEnvLvCalib_s {
    float CalibFN;
    float RealFN;
    Cam2x1FloatMatrix_t Curve;
} CalibDb_AeEnvLvCalib_t;


//2). Auto exposure
typedef struct CalibDb_AeSpeed_s {
    bool                    SmoothEn;
    bool                    DyDampEn;
    float                   DampOver;
    float                   DampUnder;
    float                   DampDark2Bright;
    float                   DampBright2Dark;
} CalibDb_AeSpeed_t;

typedef struct CalibDb_AeRange_s {
    float                   Min;
    float                   Max;
} CalibDb_AeRange_t;

typedef struct CalibDb_LinAeRange_s {
    CalibDb_AeRange_t      stExpTimeRange;
    CalibDb_AeRange_t      stGainRange;
    CalibDb_AeRange_t      stIspDGainRange;
    CalibDb_AeRange_t      stPIrisRange;
} CalibDb_LinAeRange_t;

typedef struct CalibDb_HdrAeRange_s {
    CalibDb_AeRange_t      stExpTimeRange[3];
    CalibDb_AeRange_t      stGainRange[3];
    CalibDb_AeRange_t      stIspDGainRange[3];
    CalibDb_AeRange_t      stPIrisRange;
} CalibDb_HdrAeRange_t;

typedef struct CalibDb_AeFrmRateAttr_s {
    bool             isFpsFix;
    float            FpsValue;
} CalibDb_AeFrmRateAttr_t;

typedef struct CalibDb_AntiFlickerAttr_s {
    bool                           enable;
    CalibDb_FlickerFreq_t          Frequency;
    CalibDb_AntiFlickerMode_t      Mode;
} CalibDb_AntiFlickerAttr_t;

//3.) Init expsore
typedef struct CalibDb_LinExpInitExp_s {
    float                   InitTimeValue;
    float                   InitGainValue;
    float                   InitIspDGainValue;
    int                     InitPIrisGainValue;
    int                     InitDCIrisDutyValue;
    int                     array_size;
} CalibDb_LinExpInitExp_t;

typedef struct CalibDb_HdrExpInitExp_s {
    Cam3x1FloatMatrix_t     InitTimeValue;
    Cam3x1FloatMatrix_t     InitGainValue;
    Cam3x1FloatMatrix_t     InitIspDGainValue;
    int                     InitPIrisGainValue;
    int                     InitDCIrisDutyValue;
    int                     array_size;
} CalibDb_HdrExpInitExp_t;

typedef struct CalibDb_ExpInitExp_s {
    CalibDb_LinExpInitExp_t         stLinExpInitExp;
    CalibDb_HdrExpInitExp_t         stHdrExpInitExp;
} CalibDb_ExpInitExp_t;

typedef struct CalibDb_AeAttr_s {

    CalibDb_AeSpeed_t        stAeSpeed;
    //DelayFrmNum
    uint8_t                  BlackDelayFrame;
    uint8_t                  WhiteDelayFrame;
    //AeRange
    bool                     SetAeRangeEn;
    CalibDb_LinAeRange_t     stLinAeRange;
    CalibDb_HdrAeRange_t     stHdrAeRange;
    //Auto/Fixed fps
    CalibDb_AeFrmRateAttr_t  stFrmRate;

} CalibDb_AeAttr_t;
//manual exposure
typedef struct CalibDb_LinMeAttr_s {
    bool                 ManualTimeEn;
    bool                 ManualGainEn;
    bool                 ManualIspDgainEn;
    bool                 ManualIrisEn;
    float                TimeValue;
    float                GainValue;
    float                IspDGainValue;
    int                  PIrisGainValue;
    int                  DCIrisValue;
} CalibDb_LinMeAttr_t;

typedef struct CalibDb_HdrMeAttr_s {
    bool                    ManualTimeEn;
    bool                    ManualGainEn;
    bool                    ManualIspDgainEn;
    bool                    ManualIrisEn;
    Cam3x1FloatMatrix_t     TimeValue;
    Cam3x1FloatMatrix_t     GainValue;
    Cam3x1FloatMatrix_t     IspDGainValue;
    int                     PIrisGainValue;
    int                     DCIrisValue;
} CalibDb_HdrMeAttr_t;

typedef struct CalibDb_MeAttr_s {
    CalibDb_LinMeAttr_t     stLinMe;
    CalibDb_HdrMeAttr_t     stHdrMe;
} CalibDb_MeAttr_t;

// DayorNight Switch
typedef struct CalibDb_IRNightMode_s {
    uint8_t                 enable;
    float                   IRRgain;
    float                   IRBgain;
    float                   MaxWbDis;
    uint8_t                 Night2DayFrmCnt;
    float                   Night2DayFacTh;
    float                   VbPercent;
} CalibDb_IRNightMode_t;

typedef struct CalibDb_VBNightMode_s {
    uint8_t                 enable;
    float                   Night2DayFacTh;
    uint8_t                 Night2DayFrmCnt;
} CalibDb_VBNightMode_t;

typedef struct CalibDb_DNSwitch_Attr_s {
    uint8_t                     DNTrigger;
    CalibDb_AecDayNightMode_t   DNMode;
    uint8_t                     FillLightMode;
    float                       Day2NightFacTh;
    uint8_t                     Day2NightFrmCnt;
    CalibDb_VBNightMode_t       stVBNightMode;
    CalibDb_IRNightMode_t       stIRNightMode;
} CalibDb_DNSwitch_Attr_t;

//Aec-Sync-Test used for debug
#define AEC_ALTER_EXP_MAX_NUM (10)

typedef struct CalibDb_LinAlterExp_s {
    float                    TimeValue[AEC_ALTER_EXP_MAX_NUM];
    float                    GainValue[AEC_ALTER_EXP_MAX_NUM];
    float                    IspgainValue[AEC_ALTER_EXP_MAX_NUM];
    int                      PIrisGainValue[AEC_ALTER_EXP_MAX_NUM];
    int                      DcgMode[AEC_ALTER_EXP_MAX_NUM];
    int                      array_size;
} CalibDb_LinAlterExp_t;

typedef struct CalibDb_HdrAlterExp_s {
    float                    TimeValue[AEC_ALTER_EXP_MAX_NUM][3];
    float                    GainValue[AEC_ALTER_EXP_MAX_NUM][3];
    float                    IspDGainValue[AEC_ALTER_EXP_MAX_NUM][3];
    int                      PIrisGainValue[AEC_ALTER_EXP_MAX_NUM];
    int                      DcgMode[AEC_ALTER_EXP_MAX_NUM][3];
    int                      array_size;
} CalibDb_HdrAlterExp_t;

typedef struct CalibDb_AeSyncTest_s {
    uint8_t                     enable;
    int                         IntervalFrm;
    CalibDb_LinAlterExp_t       LinAlterExp;
    CalibDb_HdrAlterExp_t       HdrAlterExp;
} CalibDb_AeSyncTest_t;

//AecIrisCtrl
#define AEC_PIRIS_STAP_TABLE_MAX (1024)
typedef struct CalibDb_PIris_Attr_s {
    uint16_t              TotalStep;
    uint16_t              EffcStep;
    bool                  ZeroIsMax;
    uint16_t              StepTable[AEC_PIRIS_STAP_TABLE_MAX];
} CalibDb_PIris_Attr_t;

typedef struct CalibDb_DCIris_Attr_s {
    float       Kp;
    float       Ki;
    float       Kd;
    int         MinPwmDuty;
    int         MaxPwmDuty;
    int         OpenPwmDuty;
    int         ClosePwmDuty;
} CalibDb_DCIris_Attr_t;

typedef struct CalibDb_AecIrisCtrl_s {
    uint8_t                     enable;
    CalibDb_IrisType_t          IrisType;
    CalibDb_PIris_Attr_t        PIrisAttr;
    CalibDb_DCIris_Attr_t       DCIrisAttr;
} CalibDb_AecIrisCtrl_t;

typedef struct CalibDb_AecCommon_Attr_s {
    uint8_t                          enable;
    uint8_t                          AecRunInterval;
    RKAiqOPMode_t                    AecOpType;
    CalibDb_CamRawStatsMode_t        RawStatsMode;
    CalibDb_CamHistStatsMode_t       HistStatsMode;
    CalibDb_CamYRangeMode_t          YRangeMode;
    //GridWeight
    Cam15x15UCharMatrix_t            DayGridWeights;
    int                              DayWeightNum;
    Cam15x15UCharMatrix_t            NightGridWeights;
    int                              NightWeightNum;

    CalibDb_AecIrisCtrl_t            stIris;

    CalibDb_AeSyncTest_t             stSyncTest;
    //envlv calibration
    CalibDb_AeEnvLvCalib_t           stEnvLvCalib;
    //antiflicker
    CalibDb_AntiFlickerAttr_t        stAntiFlicker;
    //initial exp
    CalibDb_ExpInitExp_t             stInitExp;
    //DayOrNight switch
    CalibDb_DNSwitch_Attr_t          stDNSwitch;
    //ExpSeperate
    CalibDb_AeRoute_Attr_t           stAeRoute;
    //manual expvalue
    CalibDb_MeAttr_t                 stManual;
    CalibDb_AeAttr_t                 stAuto;
} CalibDb_AecCommon_Attr_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC API algo Config Params(private)
 */
/*****************************************************************************/

#ifndef AEC_SETPOINT_MAX_NODES
#define AEC_SETPOINT_MAX_NODES 10
#endif
typedef struct CalibDb_AecDynamicSetpoint_s {
    AecDynamicSetpointName_t      name;                       /**name */
    float ExpValue[AEC_SETPOINT_MAX_NODES];
    float DySetpoint[AEC_SETPOINT_MAX_NODES];
    int   array_size;
} CalibDb_AecDynamicSetpoint_t;


typedef struct CalibDb_AecBacklight_s {
    uint8_t                     enable;
    float                       StrBias;
    CalibDb_AecMeasAreaMode_t   MeasArea;
    float                       OEROILowTh;
    float                       LumaDistTh;
    float                       LvLowTh;
    float                       LvHightTh;
    Cam1x6FloatMatrix_t         ExpLevel;
    Cam1x6FloatMatrix_t         NonOEPdfTh;
    Cam1x6FloatMatrix_t         LowLightPdfTh;
    Cam1x6FloatMatrix_t         TargetLLLuma;
} CalibDb_AecBacklight_t;

typedef struct CalibDb_AecOverExpCtrl_s {
    uint8_t                     enable;
    float                       StrBias;
    float                       MaxWeight;
    float                       HighLightTh;
    float                       LowLightTh;
    Cam1x6FloatMatrix_t         OEpdf;
    Cam1x6FloatMatrix_t         LowLightWeight;
    Cam1x6FloatMatrix_t         HighLightWeight;

} CalibDb_AecOverExpCtrl_t;


typedef struct CalibDb_LinearAE_Attr_s {
    uint8_t                 RawStatsEn;
    float                   SetPoint;                   /**< set point to hit by the ae control system */
    float                   NightSetPoint;
    float                   ToleranceIn;
    float                   ToleranceOut;
    float                   Evbias;
    CalibDb_AeStrategyMode_t        StrategyMode;
    bool                    DySetPointEn;
    CalibDb_AecDynamicSetpoint_t DySetpoint[AEC_DNMODE_MAX];

    CalibDb_AecBacklight_t  BackLightConf;
    CalibDb_AecOverExpCtrl_t OverExpCtrl;
} CalibDb_LinearAE_Attr_t;

typedef struct CalibDb_LFrameCtrl_s
{
    //LframeCtrl
    float                   OEROILowTh;
    float                   LvLowTh;
    float                   LvHighTh;
    Cam1x6FloatMatrix_t     LExpLevel;
    Cam1x6FloatMatrix_t     LSetPoint;
    Cam1x6FloatMatrix_t     TargetLLLuma;
    Cam1x6FloatMatrix_t     NonOEPdfTh;
    Cam1x6FloatMatrix_t     LowLightPdfTh;
} CalibDb_LFrameCtrl_t;

typedef struct CalibDb_MFrameCtrl_s
{
    //MframeCtrl
    Cam1x6FloatMatrix_t     MExpLevel;
    Cam1x6FloatMatrix_t     MSetPoint;
} CalibDb_MFrameCtrl_t;

typedef struct CalibDb_SFrameCtrl_s
{
    //SframeCtrl
    float                   HLLumaTolerance;
    Cam1x6FloatMatrix_t     SExpLevel;
    Cam1x6FloatMatrix_t     TargetHLLuma;
    Cam1x6FloatMatrix_t     SSetPoint;
    bool                    HLROIExpandEn;
} CalibDb_SFrameCtrl_t;

typedef struct CalibDb_HdrAE_Attr_s {
    float                           ToleranceIn;
    float                           ToleranceOut;
    CalibDb_AeHdrLongFrmMode_t      LongfrmMode;
    uint16_t                        SfrmMinLine;
    float                           LfrmModeExpTh;
    CalibDb_AeStrategyMode_t        StrategyMode;
    float                           Evbias;
    //ExpRatioCtrl
    CalibDb_HdrAeRatioType_t        ExpRatioType;
    Cam1x6FloatMatrix_t             RatioExpDot;
    Cam1x6FloatMatrix_t             M2SRatioFix;
    Cam1x6FloatMatrix_t             L2MRatioFix;
    Cam1x6FloatMatrix_t             M2SRatioMax;
    Cam1x6FloatMatrix_t             L2MRatioMax;

    float                           LumaDistTh; //used for area-growing
    CalibDb_LFrameCtrl_t            LframeCtrl;
    CalibDb_MFrameCtrl_t            MframeCtrl;
    CalibDb_SFrameCtrl_t            SframeCtrl;

} CalibDb_HdrAE_Attr_t;

/*****************************************************************************/
/**
 * @brief   Global AEC calibration structure of isp2.0
 */
/*****************************************************************************/
typedef struct CalibDb_Aec_Para_s {
    CalibDb_AecCommon_Attr_t      CommCtrl;
    CalibDb_LinearAE_Attr_t       LinearAeCtrl;
    CalibDb_HdrAE_Attr_t          HdrAeCtrl;
} CalibDb_Aec_Para_t;

#define CALD_AEC_GAIN_RANGE_MAX_LEN  350
typedef enum _CalibDb_ExpGainMode_e {
    RKAIQ_EXPGAIN_MODE_NONLINEAR_DB   = 0,
    RKAIQ_EXPGAIN_MODE_LINEAR         = 1,
    RKAIQ_EXPGAIN_MODE_MAX            = 2
} CalibDb_ExpGainMode_t;

typedef struct CalibDb_AecGainRange_s {
    bool    IsLinear;
    int     array_size;
    float   pGainRange[CALD_AEC_GAIN_RANGE_MAX_LEN];
    CalibDb_ExpGainMode_t     GainMode;
} CalibDb_AecGainRange_t;

typedef struct CalibDb_Sensor_Para_s {
    //Exp RealValue 2 RegValue
    CalibDb_AecGainRange_t  GainRange;
    float                   TimeFactor[4];
    //ExpSeperateCtrl
    Cam2x1FloatMatrix_t     CISHdrTimeRegSumFac;
    Cam2x1FloatMatrix_t     CISLinTimeRegMaxFac;
    Cam2x1FloatMatrix_t     CISTimeRegOdevity;
    Cam2x1FloatMatrix_t     CISHdrTimeRegOdevity;
    uint16_t                CISTimeRegMin;
    uint16_t                CISHdrTimeRegMin;
    Cam1x3ShortMatrix_t     CISHdrTimeRegMax;
    uint8_t                 CISTimeRegUnEqualEn;
    float                   CISMinFps;
    CalibDb_AeRange_t       CISAgainRange; //sensor Again or LCG range
    CalibDb_AeRange_t       CISExtraAgainRange; //add for HDR-DCG MODE, HCG range
    CalibDb_AeRange_t       CISDgainRange; //sensor Dgain
    CalibDb_AeRange_t       CISIspDgainRange; //Isp Dgain

    //bit 0: for hdr gain should use the same value;
    //bit 1: support use different gain value
    uint8_t                 CISHdrGainIndSetEn; //only used for hdr-stagger mode
    // bit 0 : mirror
    // bit 1 : flip
    uint8_t                 flip; // this will change the sensor output image orientation
} CalibDb_Sensor_Para_t;

typedef struct CalibDb_Module_Info_s {
    float FNumber;
    float EFL;
    float LensT;
    float IRCutT;
} CalibDb_Module_Info_t;

#define CALD_AWB_LS_NUM_MAX 14
#define CALD_AWB_WINDOW_NUM_MAX 8
#define CALD_AWB_EXCRANGE_NUM_MAX 7
#define CALD_AWB_RRES_NUM_MAX 5
#define CALD_AWB_LV_NUM_MAX 16
#define CALD_AWB_SGC_NUM_MAX 16
#define CALD_AWB_ILLUMINATION_NAME       ( 20U )
#define CALD_AWB_RES_NAME       ( 20U )
#define CALD_AWB_TEMPORAL_GAIN_SIZE_MAX 4
#define CALD_AWB_XY_TYPE_MAX_V201 2
#define CALD_AWB_GRID_NUM_TOTAL 225
#define CALD_AWB_CT_GRID_NUM_MAX 10
#define CALD_AWB_CRI_GRID_NUM_MAX 10
#define CALD_AWB_CT_LV_NUM_MAX 5
#define CALD_AWB_CT_CLIP_GRID_NUM_MAX 14
#define CALD_AWB_LV_NUM_FOR_TOLERANCE 4
#define CALD_AWB_LV_NUM_FOR_RUNINTERVAL 8

typedef struct _CalibDb_ExcRange_s {
    unsigned char domain;/*0uv domain,1 xy domain*/
    unsigned char mode;
    int xu[2];//left right
    int yv[2];//top bottom
} CalibDb_ExcRange_t;

typedef struct _CalibDb_Rgb2TcsParam_s {
    float pseudoLuminanceWeight[3];
    float rotationMat[9];
} CalibDb_Rgb2TcsParam_t;

typedef struct CalibDb_Yuv3D_Range_Ill_s {
    int b_uv;           //17+0 s
    int slope_inv_neg_uv; //9+10s //-1/k
    int slope_factor_uv; //9+10s // 1/(-1/k+k)
    int slope_ydis;     //9+10s //k
    int b_ydis;         //17+0s
    unsigned char ref_u;          //8u
    unsigned char ref_v;            //8u
    unsigned short  dis[6];//8+4 u
    unsigned char th[6]; //8u
} CalibDb_Yuv3D_Range_Ill_t;

typedef struct CalibDb_Yuv3D_2_Range_Ill_s {
    float thcurve_u[6];
    float thcure_th[6];
    float line[6];
    //float lineP1[3];
    //float lineP2[3];
} CalibDb_Yuv3D_2_Range_Ill_t;

typedef struct CalibDb_UV_Range_Ill_s {
    float  pu_region[5];
    float  pv_region[5];
} CalibDb_UV_Range_Ill_t;

typedef struct CalibDb_Tcs_Range_Ill_s {
    float xleft;
    float xright;
    float ytop;
    float ybottom;
} CalibDb_Tcs_Range_Ill_t;

typedef struct CalibDb_Awb_Light_Info_s {
    char light_name[16];
    CalibDb_UV_Range_Ill_t uvRange_param;
    CalibDb_UV_Range_Ill_t uvRange_param_small;
    CalibDb_Tcs_Range_Ill_t normalRange;
    CalibDb_Tcs_Range_Ill_t bigRange;
    CalibDb_Tcs_Range_Ill_t smallRange;
    CalibDb_Yuv3D_Range_Ill_t   yuv3DRange_param;
    CalibDb_Yuv3D_2_Range_Ill_t yuv3D2Range_param;
} CalibDb_Awb_Light_Info_t;

typedef struct CalibDb_Awb_Light_Info2_s {
    char light_name[CALD_AWB_ILLUMINATION_NAME];
    unsigned char staWeight[CALD_AWB_LV_NUM_MAX];
    unsigned int spatialGain_LV_THH;//threshold for  spatial gain calculation in different illuminant
    unsigned int spatialGain_LV_THL;//
    unsigned char xyType2Enable;
    float weightCurve_ratio[4];
    float weightCurve_weight[4];
} CalibDb_Awb_Light_Info2_t;

typedef struct CalibDb_StatWindow_s {
    int mode;
    int  resNum;
    char resName[CALD_AWB_RRES_NUM_MAX][CALD_AWB_RES_NAME];
    uint16_t window[CALD_AWB_RRES_NUM_MAX][4];
} CalibDb_StatWindow_t;
typedef struct CalibDb_tcs_range_ill_s {
    float NorrangeX[2];//left right
    float NorrangeY[2];//top bottom
    float SperangeX[2];
    float SperangeY[2];
    float SmalrangeX[2];
    float SmalrangeY[2];
} CalibDb_tcs_range_ill_t;

typedef struct CalibDb_Awb_Cct_Clip_Cfg_s {
    float outdoor_cct_min;
    int grid_num;
    float cct[CALD_AWB_CT_CLIP_GRID_NUM_MAX];
    float cri_bound_up[CALD_AWB_CT_CLIP_GRID_NUM_MAX];
    float cri_bound_low[CALD_AWB_CT_CLIP_GRID_NUM_MAX];
}  CalibDb_Awb_Cct_Clip_Cfg_t;
typedef struct CalibDb_limit_range_s {
    int lumaNum;
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    unsigned short      maxR[CALD_AWB_LV_NUM_MAX];
    unsigned short      minR[CALD_AWB_LV_NUM_MAX];
    unsigned short      maxG[CALD_AWB_LV_NUM_MAX];
    unsigned short      minG[CALD_AWB_LV_NUM_MAX];
    unsigned short      maxB[CALD_AWB_LV_NUM_MAX];
    unsigned short      minB[CALD_AWB_LV_NUM_MAX];
    unsigned short      maxY[CALD_AWB_LV_NUM_MAX];
    unsigned short      minY[CALD_AWB_LV_NUM_MAX];
} CalibDb_limit_range_t;

typedef struct CalibDb_Awb_Calib_Para_V200_s {
    struct list_head           listHead;
    char                scene[CALIBDB_MAX_SCENE_NAME_LENGTH];
    unsigned char       hdrFrameChooseMode;
    unsigned char       hdrFrameChoose;
    bool                lscBypEnable;
    bool                uvDetectionEnable;
    bool                xyDetectionEnable;
    bool                yuvDetectionEnable;
    bool                wpDiffWeiEnable;
    bool                wbGainClipEn;
    bool                wbGainDaylightClipEn;
    bool                blkWeightEnable;//the different weight in WP sum
    bool                blkStatisticsEnable;
    int                 lsUsedForYuvDetNum;
    char                lsUsedForYuvDet[CALD_AWB_LS_NUM_MAX][CALD_AWB_ILLUMINATION_NAME];
    unsigned char       dsMode;
    unsigned char       blkMeasureMode;
    CalibDb_StatWindow_t measeureWindow;
    bool                multiwindow_en;
    unsigned char       lightNum;
    char                lightName[CALD_AWB_LS_NUM_MAX][CALD_AWB_ILLUMINATION_NAME];
    unsigned char       doorType[CALD_AWB_LS_NUM_MAX];
    float               standardGainValue[CALD_AWB_LS_NUM_MAX][4];//rggb
    CalibDb_limit_range_t limitRange;
    //uv_range_ill_t        uvRange_param[7];//big range
    //uv_range_ill_t        uvRange_param_small[7];//small range
    CalibDb_Rgb2TcsParam_t     rgb2tcs_param;
    //tcs_range_ill_t     xyRangeLight[7];//HZ A TL84 CWF D50 D65 D75
    //yuv3d_range_ill_t yuv3DRange_param[7];
    //yuv3d_2_range_ill_t yuv3D2Range_param[7];
    //several window in pixel domain
    //CalibDb_Awb_Light_Info_t    awb_light_info[CALD_AWB_LS_NUM_MAX];
    CalibDb_UV_Range_Ill_t uvRange_param[CALD_AWB_LS_NUM_MAX];
    CalibDb_UV_Range_Ill_t uvRange_param_small[CALD_AWB_LS_NUM_MAX];
    CalibDb_tcs_range_ill_t xyRangeLight[CALD_AWB_LS_NUM_MAX];
    CalibDb_Yuv3D_Range_Ill_t   yuv3DRange_param[CALD_AWB_LS_NUM_MAX];


    unsigned short multiwindow[CALD_AWB_WINDOW_NUM_MAX][4];//8  windows in pixel domain ,hOffset,vOffser,hSize,vSize;
    //several winow in uv or xy domain
    CalibDb_ExcRange_t excludeWpRange[CALD_AWB_EXCRANGE_NUM_MAX];
    float spatialGain_H[4];//spatial gain
    float spatialGain_L[4];
    float temporalDefaultGain[4];
    float ca_targetGain[4];
    //single color
    unsigned short      sSelColorNUM;
    unsigned short      sIndSelColor[CALD_AWB_SGC_NUM_MAX];
    float               sMeanCh[2][CALD_AWB_SGC_NUM_MAX];
    float               srGain[CALD_AWB_LS_NUM_MAX];
    float               sbGain[CALD_AWB_LS_NUM_MAX];
    unsigned short      sIllEstNum;
    char                sNameIllEst[CALD_AWB_LS_NUM_MAX][CALD_AWB_ILLUMINATION_NAME];
    float               sAlpha;
    float lineRgBg[3];
    float lineRgProjCCT[3];
    //wbgain clip
    CalibDb_Awb_Cct_Clip_Cfg_t cct_clip_cfg;
} CalibDb_Awb_Calib_Para_V200_t;


typedef struct CalibDb_Awb_line_s {
    float a;
    float b;
    float c;
} CalibDb_Awb_line_t;

typedef struct CalibDb_Awb_Cct_Lut_Cfg_Lv_s {
    float lv;
    int ct_grid_num;
    int cri_grid_num;
    float ct_range[2];//min,max, equal distance sapmle
    float cri_range[2];//min,max
    float ct_lut_out[CALD_AWB_CT_GRID_NUM_MAX * CALD_AWB_CRI_GRID_NUM_MAX];
    float cri_lut_out[CALD_AWB_CT_GRID_NUM_MAX * CALD_AWB_CRI_GRID_NUM_MAX];
}  CalibDb_Awb_Cct_Lut_Cfg_Lv_t;


typedef struct CalibDb_Awb_gain_offset_cfg_s {
    bool enable;
    float offset[4];
} CalibDb_Awb_gain_offset_cfg_t;

typedef struct rk_aiq_wb_awb_runinterval_s {
    int num;
    float LV[CALD_AWB_LV_NUM_FOR_RUNINTERVAL];
    float value[CALD_AWB_LV_NUM_FOR_RUNINTERVAL];
}  CalibDb_Awb_runinterval_t;

typedef struct rk_aiq_wb_awb_tolerance_s {
    int num;
    float LV[CALD_AWB_LV_NUM_FOR_RUNINTERVAL];
    float value[CALD_AWB_LV_NUM_FOR_RUNINTERVAL];
}  CalibDb_Awb_tolerance_t;

typedef struct CalibDb_Awb_Remosaic_Para_s
{
    bool enable;
    float sensor_awb_gain[4];
} CalibDb_Awb_Remosaic_Para_t;

typedef struct CalibDb_Awb_Wp_Th_s {
    int num;
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    float WP_THH[CALD_AWB_LV_NUM_MAX];
    float WP_THL[CALD_AWB_LV_NUM_MAX];
}  CalibDb_Awb_Wp_Th_t;

typedef struct CalibDb_Awb_XyReionSelect_WpNumTh_s {
    int num;
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    float wpNumThForBigType[CALD_AWB_LV_NUM_MAX];
    float wpNumThForExtraType[CALD_AWB_LV_NUM_MAX];

}  CalibDb_Awb_XyReionSelect_WpNumTh_t;


typedef struct CalibDb_Awb_Adjust_Para_s {
    struct list_head           listHead;
    char                scene[CALIBDB_MAX_SCENE_NAME_LENGTH];
    bool                wbBypass;
    bool                awbEnable;
    unsigned char lightNum;
    bool ca_enable;
    bool wbGainAdjustEn;
    char lsForFirstFrame[CALD_AWB_ILLUMINATION_NAME];
    //multiwindow
    unsigned char multiwindowMode;
    bool uvRange_small_enable;
    CalibDb_Awb_tolerance_t tolerance;//wb gain diff th for awb gain update, set 0 to disable this function
    CalibDb_Awb_runinterval_t runInterval;
    //wbgain damp
    float dFStep;
    float dFMin;
    float dFMax;
    int LvIIRsize;
    float LvVarTh;

    int LV_NUM;
    unsigned int LVMatrix[CALD_AWB_LV_NUM_MAX];
    unsigned int LV_THL;
    unsigned int LV_THL2;
    unsigned int LV_THH;
    unsigned int LV_THH2;
    CalibDb_Awb_Wp_Th_t WP_TH;
    float proDis_THL; //threshold for distance probality calculation
    float proDis_THH;
    unsigned int proLV_Indoor_THL;// threshold for luminance probality calculation
    unsigned int proLV_Indoor_THH;
    unsigned int proLV_Outdoor_THL;
    unsigned int proLV_Outdoor_THH;
    unsigned char temporalCalGainSetSize;
    unsigned char temporalGainSetWeight[CALD_AWB_TEMPORAL_GAIN_SIZE_MAX];//ËÄÖ¡µÄ±ÈÀý0-100×ÜºÍÎª100//gainPer[0]Îª-1Ö¡£¬gainPer[1]is -2,gainPer[2] is -3 gainPer[3] is -4
    float  wpNumPercTh;//ÎÞÐ§°×µããÐÖµ£¬°×µãÊýÁ¿ÉÙÊ±ÈÏÎª¸Ã°×µãÎÞÐ§
    unsigned char tempWeight[CALD_AWB_LV_NUM_MAX];


    //color adaptation
    float ca_LACalcFactor;


    //wb gain shift 2 //to do  from xml
    int cct_lut_cfg_num;
    CalibDb_Awb_Cct_Lut_Cfg_Lv_t cct_lut_cfg[CALD_AWB_CT_LV_NUM_MAX];
    CalibDb_Awb_gain_offset_cfg_t wbGainOffset;


    float convergedVarTh;



    //make  xyTypeSelect stable
    int xyTypeListSize;// xyTypeListSize ==0 will disable this function
    float varianceLumaTh;
    CalibDb_Awb_XyReionSelect_WpNumTh_t wpNumTh;
    CalibDb_Awb_Light_Info2_t    awb_light_info[CALD_AWB_LS_NUM_MAX];

    bool xyType2ForColBalEnable;// to do for awb2.1

    CalibDb_Awb_Remosaic_Para_t remosaic_cfg;
} CalibDb_Awb_Adjust_Para_t;


typedef struct CalibDb_Awb_Para_s
{
    CalibDb_Awb_Calib_Para_V200_t calib_para_v200;
    CalibDb_Awb_Adjust_Para_t adjust_para;
} CalibDb_Awb_Para_t;

typedef struct GlobalLuma_s
{
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float GlobalLumaMode;
    float envLevel[13];
    float ISO[13];
    float Tolerance;
    float globalLuma[13];
} GlobalLuma_t;

typedef struct DetailsHighLight_s
{
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float DetailsHighLightMode;
    float OEPdf[13];
    float EnvLv[13];
    float Tolerance;
    float detailsHighLight[13];
} DetailsHighLight_t;

typedef struct DetailsLowLight_s
{
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float DetailsLowLightMode;
    float FocusLuma[13];
    float DarkPdf[13];
    float ISO[13];
    float Tolerance;
    float detailsLowLight[13];
} DetailsLowLight_t;

typedef struct LocalTMO_s
{
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float LocalTMOMode;
    float DynamicRange[13];
    float EnvLv[13];
    float Tolerance;
    float Strength[13];
} LocalTMO_t;

typedef struct GlobaTMO_s
{
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float en;
    float iir;
    float mode;
    float DynamicRange[13];
    float EnvLv[13];
    float Tolerance;
    float Strength[13];
} GlobaTMO_t;

typedef struct TMO_en_s
{
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float en;
} TMO_en_t;

typedef struct CalibDb_Amerge_Para_s {
    float envLevel[13];
    float oeCurve_smooth[13];
    float oeCurve_offset[13];
    float moveCoef[13];
    float mdCurveLm_smooth[13];
    float mdCurveLm_offset[13];
    float mdCurveMs_smooth[13];
    float mdCurveMs_offset[13];
    float oeCurve_damp;
    float mdCurveLm_damp;
    float mdCurveMs_damp;
} CalibDb_Amerge_Para_t;

typedef struct CalibDb_Atmo_Para_s {
    TMO_en_t en[CALIBDB_MAX_MODE_NUM];
    GlobalLuma_t luma[CALIBDB_MAX_MODE_NUM];
    DetailsHighLight_t HighLight[CALIBDB_MAX_MODE_NUM];
    DetailsLowLight_t LowLight[CALIBDB_MAX_MODE_NUM];
    LocalTMO_t LocalTMO[CALIBDB_MAX_MODE_NUM];
    GlobaTMO_t GlobaTMO[CALIBDB_MAX_MODE_NUM];
    float damp;
} CalibDb_Atmo_Para_t;



typedef struct CalibDb_Blc_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float iso[CALIBDB_BLC_MAX_ISO_LEVEL];
    float level[4][CALIBDB_BLC_MAX_ISO_LEVEL];
} CalibDb_Blc_ModeCell_t;

typedef struct CalibDb_Blc_s {
    int enable;
    CalibDb_Blc_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_Blc_t;

typedef struct CalibDb_Dpcc_set_RK_s {
    unsigned char rb_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_sw_mindis[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_sw_mindis[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char sw_dis_scale_min[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char sw_dis_scale_max[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_set_RK_t;

typedef struct CalibDb_Dpcc_set_LC_s {
    unsigned char rb_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_line_thr[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_line_thr[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_line_mad_fac[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_line_mad_fac[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_set_LC_t;

typedef struct CalibDb_Dpcc_set_PG_s {
    unsigned char rb_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_pg_fac[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_pg_fac[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_set_PG_t;

typedef struct CalibDb_Dpcc_set_RND_s {
    unsigned char rb_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_rnd_thr[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_rnd_thr[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_rnd_offs[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_rnd_offs[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_set_RND_t;

typedef struct CalibDb_Dpcc_set_RG_s {
    unsigned char rb_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_rg_fac[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_rg_fac[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_set_RG_t;

typedef struct CalibDb_Dpcc_set_RO_s {
    unsigned char rb_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char rb_ro_lim[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char g_ro_lim[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_set_RO_t;

typedef struct CalibDb_Dpcc_set_s {
    CalibDb_Dpcc_set_RK_t rk;
    CalibDb_Dpcc_set_LC_t lc;
    CalibDb_Dpcc_set_PG_t pg;
    CalibDb_Dpcc_set_RND_t rnd;
    CalibDb_Dpcc_set_RG_t rg;
    CalibDb_Dpcc_set_RO_t ro;
} CalibDb_Dpcc_set_t;

typedef struct CalibDb_Dpcc_Fast_Mode_s {
    int fast_mode_en;
    int ISO[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_single_en;
    int fast_mode_single_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_double_en;
    int fast_mode_double_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_triple_en;
    int fast_mode_triple_level[CALIBDB_DPCC_MAX_ISO_LEVEL];

} CalibDb_Dpcc_Fast_Mode_t;

typedef struct CalibDb_Dpcc_Pdaf_s {
    unsigned char en;
    unsigned char point_en[16];
    unsigned short int offsetx;
    unsigned short int offsety;
    unsigned char wrapx;
    unsigned char wrapy;
    unsigned short int wrapx_num;
    unsigned short int wrapy_num;
    unsigned char point_x[16];
    unsigned char point_y[16];
    unsigned char forward_med;
} CalibDb_Dpcc_Pdaf_t;

typedef struct CalibDb_Dpcc_Expert_Mode_s {
    float iso[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_Enable[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char grayscale_mode;
    unsigned char rk_out_sel[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char dpcc_out_sel[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_rb_3x3[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_g_3x3[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_inc_rb_center[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_inc_g_center[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_use_fix_set[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_use_set3[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_use_set2[CALIBDB_DPCC_MAX_ISO_LEVEL];
    unsigned char stage1_use_set1[CALIBDB_DPCC_MAX_ISO_LEVEL];
    CalibDb_Dpcc_set_t set[3];
} CalibDb_Dpcc_Expert_Mode_t;

typedef struct CalibDb_Dpcc_Sensor_s {
    float en;
    float max_level;
    float iso[CALIBDB_DPCC_MAX_ISO_LEVEL];
    float level_single[CALIBDB_DPCC_MAX_ISO_LEVEL];
    float level_multiple[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_Sensor_t;


typedef struct CalibDb_Dpcc_s {
    int enable;
    char version[64];
    CalibDb_Dpcc_Fast_Mode_t fast;
    CalibDb_Dpcc_Expert_Mode_t expert;
    CalibDb_Dpcc_Pdaf_t pdaf;
    CalibDb_Dpcc_Sensor_t sensor_dpcc;
} CalibDb_Dpcc_t;

#define CALIBDB_NR_SHARP_NAME_LENGTH 64
#define CALIBDB_NR_SHARP_MODE_LENGTH 64

typedef struct CalibDb_BayerNR_Params_s {
    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];
    float iso[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float filtPara[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float luLevel[8];
    float luLevelVal[8];
    float luRatio[8][CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float fixW[4][CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float lamda;
    unsigned char gauss_en;
    float RGainOff;
    float RGainFilp;
    float BGainOff;
    float BGainFilp;
    float edgeSoftness;
    float gaussWeight0;
    float gaussWeight1;
    float bilEdgeFilter;
    float bilFilterStreng[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float bilEdgeSoft;
    float bilEdgeSoftRatio;
    float bilRegWgt;
} CalibDb_BayerNR_Params_t;

typedef struct CalibDb_BayerNr_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    CalibDb_BayerNR_Params_t setting[CALIBDB_NR_SHARP_SETTING_LEVEL];
} CalibDb_BayerNr_ModeCell_t;

typedef struct CalibDb_BayerNr_s {
    int enable;
    char version[64];
    CalibDb_BayerNr_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_BayerNr_t;

typedef struct CalibDb_BayerNr_2_s {
    int enable;
    char version[64];
    CalibDb_BayerNr_ModeCell_t *mode_cell;
    int mode_num;
} CalibDb_BayerNr_2_t;


#define CIFISP_LSC_DATA_TBL_SIZE_ISP20  289
#define CIFISP_LSC_GRAD_TBL_SIZE_ISP20  8
#define CIFISP_LSC_SIZE_TBL_SIZE_ISP20  8
#define LSC_GRAD_TBL_SIZE_ISP20         8
#define LSC_ILLUMINATION_MAX            10
typedef enum  CalibDb_Used_For_Case_e {
    USED_FOR_CASE_NORMAL = 0,
    USED_FOR_CASE_FLASH,
    USED_FOR_CASE_GRAY,
    USED_FOR_CASE_3,
    USED_FOR_CASE_MAX
} CalibDb_Used_For_Case_t;

#define LSC_RESOLUTION_NAME         ( 15U )
typedef char                        CalibDb_ResolutionName_t[LSC_RESOLUTION_NAME];
#define LSC_PROFILE_NAME            ( 25U )
typedef char                        CalibDb_Lsc_ProfileName_t[LSC_PROFILE_NAME];
#define LSC_ILLUMINATION_NAME       ( 20U )
typedef char                        CalibDb_IlluminationName_t[LSC_ILLUMINATION_NAME];
#ifndef LSC_PROFILES_NUM_MAX
#define LSC_PROFILES_NUM_MAX         ( 5 )
#endif
#ifndef LSC_RESOLUTIONS_NUM_MAX
#define LSC_RESOLUTIONS_NUM_MAX      ( 4 )
#endif
typedef enum Cam4ChColorComponent_e
{
    CAM_4CH_COLOR_COMPONENT_RED     = 0,
    CAM_4CH_COLOR_COMPONENT_GREENR  = 1,
    CAM_4CH_COLOR_COMPONENT_GREENB  = 2,
    CAM_4CH_COLOR_COMPONENT_BLUE    = 3,
    CAM_4CH_COLOR_COMPONENT_MAX
} Cam4ChColorComponent_t;

typedef struct CamLscMatrix_s
{
    Cam17x17UShortMatrix_t  LscMatrix[CAM_4CH_COLOR_COMPONENT_MAX];
} CamLscMatrix_t;

typedef struct CamVignettingCurve_s {
    uint16_t    arraySize;
    float      pSensorGain[4];//change to pointer
    float      pVignetting[4];
} CamVignettingCurve_t;

typedef struct CalibDb_AlscCof_ill_s {
    char illuName[25];
    float awbGain[2];
    int tableUsedNO;
    CalibDb_Lsc_ProfileName_t tableUsed[LSC_PROFILES_NUM_MAX];
    CamVignettingCurve_t vignettingCurve;
} CalibDb_AlscCof_ill_t;

typedef struct CalibDb_AlscCof_s {
    int   lscResNum;
    CalibDb_ResolutionName_t  lscResName[LSC_RESOLUTIONS_NUM_MAX];// type CalibDb_ResolutionName_t
    int   illuNum[USED_FOR_CASE_MAX];
    CalibDb_AlscCof_ill_t illAll[USED_FOR_CASE_MAX][LSC_ILLUMINATION_MAX];
    int usedForCaseAll[USED_FOR_CASE_MAX * LSC_ILLUMINATION_MAX]; //for write xml
} CalibDb_AlscCof_t;


typedef struct CalibDb_LscTableProfile_s {
    CalibDb_Lsc_ProfileName_t     name;                                   /**< profile name */
    CalibDb_ResolutionName_t     resolution;                             /**< resolution link */
    CalibDb_IlluminationName_t   illumination;                           /**< illumination link */
    float                   vignetting;                             /**< vignetting value */

    uint16_t                LscSectors;
    uint16_t                LscNo;
    uint16_t                LscXo;
    uint16_t                LscYo;

    uint16_t                LscXGradTbl[LSC_GRAD_TBL_SIZE_ISP20];
    uint16_t                LscYGradTbl[LSC_GRAD_TBL_SIZE_ISP20];
    uint16_t                LscXSizeTbl[CIFISP_LSC_SIZE_TBL_SIZE_ISP20];
    uint16_t                LscYSizeTbl[CIFISP_LSC_SIZE_TBL_SIZE_ISP20];

    Cam17x17UShortMatrix_t  LscMatrix[CAM_4CH_COLOR_COMPONENT_MAX];     /**< matrix for different color channels */
} CalibDb_LscTableProfile_t;

typedef struct CalibDb_Lsc_s {
    bool enable;
    bool damp_enable;
    CalibDb_AlscCof_t aLscCof;
    int tableAllNum;
    CalibDb_LscTableProfile_t *tableAll; //type  CalibDb_LscTableProfile_t;
} CalibDb_Lsc_t;


typedef struct CalibDb_RKDM_s {
    unsigned char debayer_en;
    signed char debayer_filter1[5];
    signed char debayer_filter2[5];
    unsigned char debayer_gain_offset;
    int ISO[9];
    unsigned char sharp_strength[9];
    unsigned short debayer_hf_offset[9];
    unsigned char debayer_offset;
    unsigned char debayer_clip_en;
    unsigned char debayer_filter_g_en;
    unsigned char debayer_filter_c_en;
    unsigned char debayer_thed0;
    unsigned char debayer_thed1;
    unsigned char debayer_dist_scale;
    unsigned char debayer_cnr_strength;
    unsigned char debayer_shift_num;
} CalibDb_RKDM_t;

#define CCM_ILLUMINATION_MAX               7
#define CCM_PROFILE_NAME            ( 25U )
typedef char                        CalibDb_Ccm_ProfileName_t[CCM_PROFILE_NAME];
#define CCM_ILLUMINATION_NAME       ( 20U )
typedef char                        CalibDb_IlluminationName_t[CCM_ILLUMINATION_NAME];
#define CCM_PROFILES_NUM_MAX         ( 5 )
#define CCM_RESOLUTIONS_NUM_MAX      ( 4 )
#define CALIBDB_ISO_NUM              ( 9 )
typedef enum Cam3ChColorComponent_e
{
    CAM_3CH_COLOR_COMPONENT_RED     = 0,
    CAM_3CH_COLOR_COMPONENT_GREEN   = 1,
    CAM_3CH_COLOR_COMPONENT_BLUE    = 2,
    CAM_3CH_COLOR_COMPONENT_MAX
} Cam3ChColorComponent_t;

typedef struct CamSaturationCurve_s {
    uint16_t    arraySize;
    float      pSensorGain[4];//change to pointer
    float      pSaturation[4];
} CamSaturationCurve_t;

typedef struct CalibDb_AccmCof_ill_s {
    char illuName[25];
    float awbGain[2];
    int matrixUsedNO;
    CalibDb_Ccm_ProfileName_t matrixUsed[CCM_PROFILES_NUM_MAX];
    CamSaturationCurve_t saturationCurve;
} CalibDb_AccmCof_ill_t;

typedef struct CalibDb_AccmCof_s {
    int   illuNum;
    CalibDb_AccmCof_ill_t illAll[CCM_ILLUMINATION_MAX];
} CalibDb_AccmCof_t;

typedef struct CalibDb_CcmMatrixProfile_s
{
    CalibDb_Ccm_ProfileName_t  name;                   /**< profile name */
    CalibDb_IlluminationName_t illumination;
    float                   saturation;             /**< saturation value */
    Cam3x3FloatMatrix_t     CrossTalkCoeff;         /**< CrossTalk matrix coefficients */
    Cam1x3FloatMatrix_t     CrossTalkOffset;        /**< CrossTalk offsets */
} CalibDb_CcmMatrixProfile_t;

typedef struct CalibDb_LUMA_CCM_s {
    float rgb2y_para[3];
    float low_bound_pos_bit;
    float y_alpha_curve[17];
    int gain_scale_cure_size;
    float alpha_gain[9];//change to pointer
    float alpha_scale[9];
} CalibDb_LUMA_CCM_t;

typedef struct CalibDb_Ccm_ModeCell_s {
    bool valid;
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    bool  damp_enable;
    CalibDb_AccmCof_t aCcmCof;
    CalibDb_LUMA_CCM_t   luma_ccm;
    int matrixAllNum;
    CalibDb_CcmMatrixProfile_t matrixAll[CCM_RESOLUTIONS_NUM_MAX * CCM_ILLUMINATION_MAX * CCM_PROFILES_NUM_MAX]; //type  CalibDb_CcmMatrixProfile_t;
} CalibDb_Ccm_ModeCell_t;

typedef enum  CalibDb_CcmHdrNormalMode_e {
    CCM_FOR_MODE_NORMAL = 0,
    CCM_FOR_MODE_HDR,
    CCM_FOR_MODE_MAX,
} CalibDb_CcmHdrNormalMode_t;


typedef struct CalibDb_Ccm_s {
    int modecellNum;
    bool enable;
    CalibDb_Ccm_ModeCell_t mode_cell[CCM_FOR_MODE_MAX];
} CalibDb_Ccm_t;

typedef struct CalibDb_UVNR_Params_s {
    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];
    float ISO[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step0_uvgrad_ratio[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step0_uvgrad_offset[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_nonMed1[4];
    float step1_nonBf1[4];
    float step1_downSample_w[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_downSample_h[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_downSample_meansize[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_median_ratio[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_median_size[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_median_IIR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_sigmaR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_uvgain[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_ratio[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_size[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_sigmaD[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_isRowIIR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step1_bf_isYcopy[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_nonExt_block[4];
    float step2_nonMed[4];
    float step2_nonBf[4];
    float step2_downSample_w[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_downSample_h[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_downSample_meansize[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_median_ratio[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_median_size[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_median_IIR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_sigmaR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_uvgain[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_ratio[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_size[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_sigmaD[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_isRowIIR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step2_bf_isYcopy[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_nonBf3[4];
    float step3_bf_sigmaR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_bf_uvgain[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_bf_ratio[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_bf_size[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_bf_sigmaD[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_bf_isRowIIR[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float step3_bf_isYcopy[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float kernel_3x3[3];
    float kernel_5x5[5];
    float kernel_9x9[8];
    float kernel_9x9_num;
    float sigma_adj_luma[9];
    float sigma_adj_ratio[9];
    float threshold_adj_luma[9];
    float threshold_adj_thre[9];
} CalibDb_UVNR_Params_t;

typedef struct CalibDb_UVNR_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    CalibDb_UVNR_Params_t setting[CALIBDB_NR_SHARP_SETTING_LEVEL];
} CalibDb_UVNR_ModeCell_t;

typedef struct CalibDb_UVNR_s {
    int enable;
    char version[64];
    CalibDb_UVNR_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_UVNR_t;

typedef struct CalibDb_UVNR_2_s {
    int enable;
    char version[64];
    CalibDb_UVNR_ModeCell_t *mode_cell;
    int mode_num;
} CalibDb_UVNR_2_t;


typedef struct CalibDb_Gamma_s {
    unsigned char gamma_en;
    unsigned char gamma_out_segnum;
    unsigned char gamma_out_offset;
    float curve_normal[45];
    float curve_hdr[45];
    float curve_night[45];
} CalibDb_Gamma_t;

typedef struct CalibDb_Degamma_para_s {
    unsigned char degamma_scene_en;
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float X_axis[CALIBDB_DEGAMMA_CRUVE_KNOTS];
    float curve_R[CALIBDB_DEGAMMA_CRUVE_KNOTS];
    float curve_G[CALIBDB_DEGAMMA_CRUVE_KNOTS];
    float curve_B[CALIBDB_DEGAMMA_CRUVE_KNOTS];
} CalibDb_Degamma_para_t;

typedef struct CalibDb_Degamma_s {
    unsigned char degamma_en;
    CalibDb_Degamma_para_t mode[3];
} CalibDb_Degamma_t;

typedef struct CalibDb_YNR_ISO_s {
    float iso;
    double sigma_curve[5];
    float ynr_lci[4];
    float ynr_lhci[4];
    float ynr_hlci[4];
    float ynr_hhci[4];
    float lo_lumaPoint[6];
    float lo_lumaRatio[6];
    float lo_directionStrength;
    float lo_bfScale[4];
    float imerge_ratio;
    float imerge_bound;
    float denoise_weight[4];
    float hi_lumaPoint[6];
    float hi_lumaRatio[6];
    float hi_bfScale[4];
    float hwith_d[4];
    float hi_denoiseStrength;
    float hi_detailMinAdjDnW;
    float hi_denoiseWeight[4];
    float y_luma_point[6];
    float hgrad_y_level1[6];
    float hgrad_y_level2[6];
    float hgrad_y_level3[6];
    float hgrad_y_level4[6];
    float hi_soft_thresh_scale[4];
} CalibDb_YNR_ISO_t;

typedef struct CalibDb_YNR_Setting_s {
    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];
    CalibDb_YNR_ISO_t ynr_iso[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
} CalibDb_YNR_Setting_t;

typedef struct CalibDb_YNR_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    CalibDb_YNR_Setting_t setting[CALIBDB_NR_SHARP_SETTING_LEVEL];
} CalibDb_YNR_ModeCell_t;

typedef struct CalibDb_YNR_s {
    int enable;
    char version[64];
    CalibDb_YNR_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_YNR_t;

typedef struct CalibDb_YNR_2_s {
    int enable;
    char version[64];
    CalibDb_YNR_ModeCell_t *mode_cell;
    int mode_num;
} CalibDb_YNR_2_t;


typedef struct CalibDb_Gic_setting_v20_s {
    float iso[CALIBDB_ISO_NUM];
    float min_busy_thre[CALIBDB_ISO_NUM];
    float min_grad_thr1[CALIBDB_ISO_NUM];
    float min_grad_thr2[CALIBDB_ISO_NUM] ;
    float k_grad1[CALIBDB_ISO_NUM];
    float k_grad2[CALIBDB_ISO_NUM] ;
    float gb_thre[CALIBDB_ISO_NUM] ;
    float maxCorV[CALIBDB_ISO_NUM] ;
    float maxCorVboth[CALIBDB_ISO_NUM];
    float dark_thre[CALIBDB_ISO_NUM];
    float dark_threHi[CALIBDB_ISO_NUM];
    float k_grad1_dark[CALIBDB_ISO_NUM];
    float k_grad2_dark[CALIBDB_ISO_NUM] ;
    float min_grad_thr_dark1[CALIBDB_ISO_NUM];
    float min_grad_thr_dark2[CALIBDB_ISO_NUM] ;
    float noiseCurve_0[CALIBDB_ISO_NUM];
    float noiseCurve_1[CALIBDB_ISO_NUM];
    float GValueLimitLo[CALIBDB_ISO_NUM] ;
    float GValueLimitHi[CALIBDB_ISO_NUM];
    float textureStrength[CALIBDB_ISO_NUM] ;
    float ScaleLo[CALIBDB_ISO_NUM] ;
    float ScaleHi[CALIBDB_ISO_NUM] ;
    float globalStrength[CALIBDB_ISO_NUM];
    float noise_coea[CALIBDB_ISO_NUM] ;
    float noise_coeb[CALIBDB_ISO_NUM];
    float diff_clip[CALIBDB_ISO_NUM];
} CalibDb_Gic_setting_v20_t;

typedef struct CalibDb_Gic_ModeCell_v20_s {
    char scene[CALIBDB_MAX_MODE_NAME_LENGTH];
    unsigned char gic_en;
    unsigned char edge_en;
    unsigned char gr_ration;
    unsigned char noise_cut_en;
    CalibDb_Gic_setting_v20_t setting;
} CalibDb_Gic_ModeCell_v20_t;

typedef struct CalibDb_Gic_s {
    CalibDb_Gic_ModeCell_v20_t calib_v20[CALIBDB_MAX_MODE_NUM];
    CalibDb_Gic_ModeCell_v20_t tuning_v20[CALIBDB_MAX_MODE_NUM];
} CalibDb_Gic_t;

struct CalibDb_awb_uv_ratio_s {
    char illum[9];
    float ratio[2];
};

struct CalibDb_MFNR_ISO_s {
    float iso;
    float weight_limit_y[4];
    float weight_limit_uv[3];
    float ratio_frq[4];
    float luma_w_in_chroma[3];
    double noise_curve[5];
    double noise_curve_x00;
    float y_lo_noiseprofile[4];
    float y_hi_noiseprofile[4];
    float y_lo_denoiseweight[4];
    float y_hi_denoiseweight[4];
    float y_lo_bfscale[4];
    float y_hi_bfscale[4];
    float y_lumanrpoint[6];
    float y_lumanrcurve[6];
    float y_denoisestrength;
    float y_lo_lvl0_gfdelta[6];
    float y_hi_lvl0_gfdelta[6];
    float y_lo_lvl1_gfdelta[3];
    float y_hi_lvl1_gfdelta[3];
    float y_lo_lvl2_gfdelta[3];
    float y_hi_lvl2_gfdelta[3];
    float y_lo_lvl3_gfdelta[3];
    float y_hi_lvl3_gfdelta[3];
    float uv_lo_noiseprofile[3];
    float uv_hi_noiseprofile[3];
    float uv_lo_denoiseweight[3];
    float uv_hi_denoiseweight[3];
    float uv_lo_bfscale[3];
    float uv_hi_bfscale[3];
    float uv_lumanrpoint[6];
    float uv_lumanrcurve[6];
    float uv_denoisestrength;
    float uv_lo_lvl0_gfdelta[6];
    float uv_hi_lvl0_gfdelta[6];
    float uv_lo_lvl1_gfdelta[3];
    float uv_hi_lvl1_gfdelta[3];
    float uv_lo_lvl2_gfdelta[3];
    float uv_hi_lvl2_gfdelta[3];
    float lvl0_gfsigma[6];
    float lvl1_gfsigma[3];
    float lvl2_gfsigma[3];
    float lvl3_gfsigma[3];
};

typedef struct CalibDb_MFNR_Setting_s {
    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];
    struct CalibDb_MFNR_ISO_s mfnr_iso[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
} CalibDb_MFNR_Setting_t;

typedef struct CalibDb_MFNR_Dynamic_s {
    int enable;
    float lowth_iso;
    float lowth_time;
    float highth_iso;
    float highth_time;
} CalibDb_MFNR_Dynamic_t;

typedef struct CalibDb_MFNR_Motion_s {
    int enable;
    float iso[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float sigmaHScale[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float sigmaLScale[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float lightClp[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float uvWeight[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float mfnrSigmaScale[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float yuvnrGainScale0[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float yuvnrGainScale1[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float yuvnrGainScale2[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved0[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved1[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved2[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved3[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved4[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved5[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved6[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float reserved7[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float frame_limit_uv[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
    float frame_limit_y[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
} CalibDb_MFNR_Motion_t;

typedef struct CalibDb_MFNR_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    CalibDb_MFNR_Setting_t setting[CALIBDB_NR_SHARP_SETTING_LEVEL];
    CalibDb_MFNR_Dynamic_t dynamic;
    CalibDb_MFNR_Motion_t  motion;
} CalibDb_MFNR_ModeCell_t;

typedef struct CalibDb_MFNR_s {
    int enable;
    char version[64];
    unsigned char local_gain_en;
    unsigned char motion_detect_en;
    unsigned char mode_3to1;
    unsigned char max_level;
    unsigned char max_level_uv;
    unsigned char back_ref_num;
    struct CalibDb_awb_uv_ratio_s uv_ratio[4];
    CalibDb_MFNR_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_MFNR_t;

typedef struct CalibDb_MFNR_2_s {
    int enable;
    char version[64];
    unsigned char local_gain_en;
    unsigned char motion_detect_en;
    unsigned char mode_3to1;
    unsigned char max_level;
    unsigned char max_level_uv;
    unsigned char back_ref_num;
    struct CalibDb_awb_uv_ratio_s uv_ratio[4];
    CalibDb_MFNR_ModeCell_t *mode_cell;
    int mode_num;
} CalibDb_MFNR_2_t;


struct CalibDb_Sharp_ISO_s {
    float iso;
    float hratio;
    float lratio;
    float mf_sharp_ratio;
    float hf_sharp_ratio;
    float luma_sigma[8];
    float pbf_gain;
    float pbf_ratio;
    float pbf_add;
    float mf_clip_pos[8];
    float mf_clip_neg[8];
    float hf_clip[8];
    float mbf_gain;
    float hbf_gain;
    float hbf_ratio;
    float mbf_add;
    float hbf_add;
    float local_sharp_strength;
    float pbf_coeff_percent;
    float rf_m_coeff_percent;
    float rf_h_coeff_percent;
    float hbf_coeff_percent;
};

typedef struct CalibDb_Sharp_Setting_s {
    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];
    struct CalibDb_Sharp_ISO_s sharp_iso[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
} CalibDb_Sharp_Setting_t;

typedef struct CalibDb_Sharp_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float gauss_luma_coeff[9];
    float mbf_coeff[221];
#if 0
    float pbf_coeff[9];
    float rf_m_coeff[25];
    float rf_h_coeff[25];
    float hbf_coeff[9];
#else
    //v2
    float pbf_coeff_l[9];
    float pbf_coeff_h[9];
    float rf_m_coeff_l[25];
    float rf_m_coeff_h[25];
    float rf_h_coeff_l[25];
    float rf_h_coeff_h[25];
    float hbf_coeff_l[9];
    float hbf_coeff_h[9];
#endif
    CalibDb_Sharp_Setting_t setting[CALIBDB_NR_SHARP_SETTING_LEVEL];
} CalibDb_Sharp_ModeCell_t;

typedef struct CalibDb_Sharp_s {
    int enable;
    char version[64];
    float luma_point[8];
    CalibDb_Sharp_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_Sharp_t;

typedef struct CalibDb_Sharp_2_s {
    int enable;
    char version[64];
    float luma_point[8];
    CalibDb_Sharp_ModeCell_t* mode_cell;
    int mode_num;
} CalibDb_Sharp_2_t;


struct CalibDb_EdgeFilter_ISO_s {
    float iso;
    float edge_thed;
    float src_wgt;
    unsigned char alpha_adp_en;
    float local_alpha;
    float global_alpha;
    float noise_clip[8];
    float dog_clip_pos[8];
    float dog_clip_neg[8];
    float dog_alpha[8];
    float direct_filter_coeff[5];
    float dog_kernel_row0[5];
    float dog_kernel_row1[5];
    float dog_kernel_row2[5];
    float dog_kernel_row3[5];
    float dog_kernel_row4[5];
    float dog_kernel_percent;
};

typedef struct CalibDb_EdgeFilter_Setting_s {
    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];
    struct CalibDb_EdgeFilter_ISO_s edgeFilter_iso[CALIBDB_NR_SHARP_MAX_ISO_LEVEL];
} CalibDb_EdgeFilter_Setting_t;

typedef struct CalibDb_EdgeFilter_ModeCell_s {
    char name[CALIBDB_MAX_MODE_NAME_LENGTH];
    float dog_kernel_l[25];
    float dog_kernel_h[25];
    CalibDb_EdgeFilter_Setting_t setting[CALIBDB_NR_SHARP_SETTING_LEVEL];
} CalibDb_EdgeFilter_ModeCell_t;

typedef struct CalibDb_EdgeFilter_s {
    int enable;
    char version[64];
    float luma_point[8];
    CalibDb_EdgeFilter_ModeCell_t mode_cell[CALIBDB_MAX_MODE_NUM];
} CalibDb_EdgeFilter_t;

typedef struct CalibDb_EdgeFilter_2_s {
    int enable;
    char version[64];
    float luma_point[8];
    CalibDb_EdgeFilter_ModeCell_t *mode_cell;
    int mode_num;
} CalibDb_EdgeFilter_2_t;

typedef struct CalibDb_IIR_setting_v20_s {
    float stab_fnum;
    float sigma;
    float wt_sigma;
    float air_sigma;
    float tmax_sigma;
} CalibDb_IIR_setting_v20_t;

typedef struct CalibDb_Dehaze_Setting_v20_s {
    float en;
    float iso[9];
    float dc_min_th[9];
    float dc_max_th[9];
    float yhist_th[9];
    float yblk_th[9];
    float dark_th[9];
    float bright_min[9];
    float bright_max[9];
    float wt_max[9];
    float air_min[9];
    float air_max[9];
    float tmax_base[9];
    float tmax_off[9];
    float tmax_max[9];
    float cfg_wt[9];
    float cfg_air[9];
    float cfg_tmax[9];
    float dc_thed[9];
    float dc_weitcur[9];
    float air_thed[9];
    float air_weitcur[9];
    CalibDb_IIR_setting_v20_t IIR_setting;
} CalibDb_Dehaze_Setting_v20_t;

typedef struct CalibDb_Enhance_Setting_v20_s {
    float en;
    float iso[9];
    float enhance_value[9];
} CalibDb_Enhance_Setting_v20_t;

typedef struct CalibDb_Hist_setting_v20_s {
    float en;
    float iso[9];
    unsigned char  hist_channel[9];
    unsigned char  hist_para_en[9];
    float hist_gratio[9];
    float hist_th_off[9];
    float hist_k[9];
    float hist_min[9];
    float hist_scale[9];
    float cfg_gratio[9];
} CalibDb_Hist_setting_v20_t;

typedef struct CalibDb_Dehaze_ModeCell_v20_s {
    char scene[CALIBDB_MAX_MODE_NAME_LENGTH];
    float en;
    float cfg_alpha;
    CalibDb_Dehaze_Setting_v20_t dehaze_setting;
    CalibDb_Enhance_Setting_v20_t enhance_setting;
    CalibDb_Hist_setting_v20_t hist_setting;
} CalibDb_Dehaze_ModeCell_v20_t;

typedef struct CalibDb_Dehaze_s {
    CalibDb_Dehaze_ModeCell_v20_t calib_v20[CALIBDB_MAX_MODE_NUM];
    CalibDb_Dehaze_ModeCell_v20_t tuning_v20[CALIBDB_MAX_MODE_NUM];

} CalibDb_Dehaze_t;

typedef enum CalibDb_Af_SearchDir_s {
    CAM_AFM_POSITIVE_SEARCH     = 0,
    CAM_AFM_NEGATIVE_SEARCH     = 1,
    CAM_AFM_ADAPTIVE_SEARCH     = 2
} CalibDb_Af_SearchDir_t;

typedef enum CalibDb_Af_SS_s {
    CAM_AFM_FSS_INVALID         = 0,
    CAM_AFM_FSS_FULLRANGE       = 1,    /**< scan the full focus range to find the point of best focus */
    CAM_AFM_FSS_HILLCLIMBING    = 2,    /**< use hillclimbing search */
    CAM_AFM_FSS_ADAPTIVE_RANGE  = 3,    /**< similar to full range search, but with multiple subsequent scans
                                         with decreasing range and step size will be performed. */
    CAM_AFM_FSS_MUTIWINDOW    = 4,    /**< search by muti-window statistics */
    CAM_AFM_FSS_MAX
} CalibDb_Af_SS_t;

typedef struct CalibDb_Af_Contrast_s {
    unsigned char           enable;
    CalibDb_Af_SS_t         Afss;                         /**< enumeration type for search strategy */
    CalibDb_Af_SearchDir_t  FullDir;
    unsigned char           FullSteps;
    unsigned short          FullRangeTbl[65];                 /**< full range search table*/
    CalibDb_Af_SearchDir_t  AdaptiveDir;
    unsigned char           AdaptiveSteps;
    unsigned short          AdaptRangeTbl[65];                /**< adaptive range search table*/
    float                   TrigThers;                    /**< AF trigger threshold */
    float                   LumaTrigThers;

    float                   StableThers;                  /**< AF stable threshold */
    unsigned short          StableFrames;                 /**< AF stable  status must hold frames */
    unsigned short          StableTime;                   /**< AF stable status must hold time */

    unsigned char           SceneDiffEnable;
    float                   SceneDiffThers;
    unsigned short          SceneDiffBlkThers;
    float                   CenterSceneDiffThers;

    float                   ValidMaxMinRatio;
    float                   ValidValueThers;

    float                   OutFocusValue;                /**< out of focus vlaue*/
    unsigned short          OutFocusPos;                  /**< out of focus position*/

    unsigned char           WeightEnable;
    unsigned short          Weight[225];                  /**< weight */

    unsigned char           SearchPauseLumaEnable;
    float                   SearchPauseLumaThers;
    unsigned short          SearchLumaStableFrames;
    float                   SearchLumaStableThers;

    float                   FlatValue;
} CalibDb_Af_Contrast_t;

typedef struct CalibDb_Af_Laser_s {
    unsigned char enable;
    float vcmDot[7];
    float distanceDot[7];
} CalibDb_Af_Laser_t;

typedef struct CalibDb_Af_Pdaf_s {
    unsigned char enable;
} CalibDb_Af_Pdaf_t;

typedef struct CalibDb_Af_VcmCfg_s {
    int start_current;
    int rated_current;
    int step_mode;
    int extra_delay;
} CalibDb_Af_VcmCfg_t;

typedef struct CalibDb_Af_MeasIsoCfg_s {
    int iso;
    unsigned short afmThres;
    unsigned short gammaY[17];
    unsigned char gaussWeight[3];
} CalibDb_Af_MeasIsoCfg_t;

typedef struct CalibDb_Af_DefCode_s {
    unsigned char code;
} CalibDb_Af_DefCode_t;

typedef struct CalibDb_Af_ZoomFocusTbl_s {
    int tbl_len;
    float focal_length[CALIBDB_ZOOM_FOCUS_TBL_SIZE];
    int zoom_pos[CALIBDB_ZOOM_FOCUS_TBL_SIZE];
    int focus_infpos[CALIBDB_ZOOM_FOCUS_TBL_SIZE];
    int focus_macropos[CALIBDB_ZOOM_FOCUS_TBL_SIZE];
} CalibDb_Af_ZoomFocusTbl_t;

typedef struct CalibDb_AF_s {
    signed char af_mode;
    unsigned short win_h_offs;
    unsigned short win_v_offs;
    unsigned short win_h_size;
    unsigned short win_v_size;
    CalibDb_Af_DefCode_t fixed_mode;
    CalibDb_Af_DefCode_t macro_mode;
    CalibDb_Af_DefCode_t infinity_mode;
    CalibDb_Af_Contrast_t contrast_af;
    CalibDb_Af_Laser_t laser_af;
    CalibDb_Af_Pdaf_t pdaf;
    CalibDb_Af_VcmCfg_t vcmcfg;
    CalibDb_Af_MeasIsoCfg_t measiso_cfg[CALIBDB_MAX_ISO_LEVEL];
    CalibDb_Af_ZoomFocusTbl_t zoomfocus_tbl;
} CalibDb_AF_t;

typedef struct CalibDb_ORB_s {
    unsigned char orb_en;
} CalibDb_ORB_t;

typedef struct CalibDb_LUMA_DETECT_s {
    unsigned char luma_detect_en;
    int fixed_times;
    float mutation_threshold;
    float mutation_threshold_level2;
} CalibDb_LUMA_DETECT_t;

typedef struct CalibDb_FEC_s {
    unsigned char fec_en;
    char meshfile[256];
    int correct_level;
    double light_center[2]; // light center
    double coefficient[4]; // the distortion coefficient of the fisheye lens
} CalibDb_FEC_t;

typedef struct CalibDb_EIS_s {
    unsigned char eis_en;
    // TODO(Cody): Add EIS specific calib data
} CalibDb_EIS_t;

typedef struct CalibDb_LDCH_s {
    unsigned char ldch_en;
    char meshfile[256];
    int correct_level;
    int correct_level_max;
    double light_center[2]; // light center
    double coefficient[4]; // the distortion coefficient of the fisheye lens
} CalibDb_LDCH_t;
typedef struct {
    bool enable;
    unsigned short look_up_table_r[729];
    unsigned short look_up_table_g[729];
    unsigned short look_up_table_b[729];
} CalibDb_Lut3d_t;

typedef struct {
    bool          support_en;
    RKAiqOPMode_t dcg_optype;
    Cam1x3IntMatrix_t dcg_mode;
    float         dcg_ratio;
    bool          gainCtrl_en;
    bool          envCtrl_en;
    bool          sync_switch;
    float         lcg2hcg_gain_th;
    float         lcg2hcg_env_th;
    float         hcg2lcg_gain_th;
    float         hcg2lcg_env_th;
} CalibDb_Dcg_Params_t;

typedef struct {
    CalibDb_Dcg_Params_t Hdr;
    CalibDb_Dcg_Params_t Normal;
} CalibDb_Dcg_t;

typedef struct {
    uint8_t       support_en;
    /* default mode
     * 0: auto
     * 1: manual
     */
    int32_t      mode;
    /* force gray if cpsl on */
    uint8_t        gray;
    /* default cpsl source
     * 0: led
     * 1: ir
     * 2: mix
     */
    int32_t      lght_src;
    /* auto mode default params */
    float         ajust_sens;
    uint32_t      sw_interval;
    uint32_t      on2off_th;
    uint32_t      off2on_th;
    /* manual mode default params */
    uint8_t       cpsl_on;
    float         strength;
} CalibDb_Cpsl_t;

typedef struct {
    int time_delay;
    int gain_delay;
    int dcg_delay;
} CalibDb_ExpDelay_t;

typedef struct {
    CalibDb_ExpDelay_t Hdr;
    CalibDb_ExpDelay_t Normal;
} CalibDb_ExpDelay_comb_t;

typedef struct {
#define HDR_MODE_2_FRAME_STR        "MODE_2_FRAME"
#define HDR_MODE_2_LINE_STR         "MODE_2_LINE"
#define HDR_MODE_3_FRAME_STR        "MODE_3_FRAME"
#define HDR_MODE_3_LINE_STR         "MODE_3_LINE"
#define HDR_LINE_MODE_DCG_STR       "DCG"
#define HDR_LINE_MODE_STAGGER_STR   "STAGGER"

    unsigned char hdr_en;
    rk_aiq_isp_hdr_mode_t hdr_mode;
    rk_aiq_sensor_hdr_line_mode_t line_mode;
    CalibDb_Dcg_t dcg;
    CalibDb_ExpDelay_comb_t exp_delay;
} CalibDb_System_t;

typedef struct {
    char parse_version[16];
    char date[16];
    char author[32];
    char sensor_name[32];
    char sample_name[64];
    char gen_verion[16];
    uint32_t magic_code;
} CalibDb_Header_t;

typedef struct CalibDb_ColorAsGrey_s {
    int enable;
} CalibDb_ColorAsGrey_t;


typedef struct CalibDb_cProc_s {
    uint8_t enable;
    uint8_t brightness;
    uint8_t contrast;
    uint8_t saturation;
    uint8_t hue;
} CalibDb_cProc_t;

typedef struct CalibDb_IE_s {
    int enable;
    int mode;
} CalibDb_IE_t;

typedef struct CamCalibDbContextIsp20_s {
    CalibDb_Header_t header;
    struct list_head awb_calib_para_v200;
    struct list_head awb_adjust_para;
    CalibDb_Lut3d_t lut3d;
    CalibDb_Aec_Para_t aec;
    CalibDb_AF_t af;
    CalibDb_Amerge_Para_t amerge;
    CalibDb_Atmo_Para_t atmo;
    CalibDb_Blc_t blc;
    CalibDb_Dpcc_t dpcc;
    CalibDb_BayerNr_2_t bayerNr;
    CalibDb_Lsc_t lsc;
    CalibDb_RKDM_t dm;
    CalibDb_Ccm_t ccm;
    CalibDb_UVNR_2_t uvnr;
    CalibDb_Gamma_t gamma;
    CalibDb_Degamma_t degamma;
    CalibDb_YNR_2_t ynr;
    CalibDb_Gic_t gic;
    CalibDb_MFNR_2_t mfnr;
    CalibDb_Sharp_2_t sharp;
    CalibDb_EdgeFilter_2_t edgeFilter;
    CalibDb_Dehaze_t dehaze;
    CalibDb_FEC_t afec;
    CalibDb_EIS_t aeis;
    CalibDb_LDCH_t aldch;
    CalibDb_LUMA_DETECT_t lumaDetect;
    CalibDb_ORB_t orb;
    CalibDb_Sensor_Para_t sensor;
    CalibDb_Module_Info_t module;
    CalibDb_Cpsl_t cpsl;
    CalibDb_ColorAsGrey_t colorAsGrey;
    CalibDb_cProc_t cProc;
    CalibDb_IE_t    ie;
    CalibDb_System_t  sysContrl;
} CamCalibDbContextIsp20_t;

#pragma pack()

#endif
