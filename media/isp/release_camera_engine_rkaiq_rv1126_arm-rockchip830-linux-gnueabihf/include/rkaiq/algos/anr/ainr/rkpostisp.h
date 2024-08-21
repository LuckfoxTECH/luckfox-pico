#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/*-------------------------------------------
                  Struct
-------------------------------------------*/
#define     RK_AINR_TAB_LEN     13
#define     RK_AINR_LUMA_LEN     6
typedef struct rk_ainr_param_t
{
    int   enable;
    float gain;
    float RawMeanluma;
    float YuvMeanluma;

    int   gain_tab_len;
    int   gain_max;
    int   tuning_visual_flag;   //  0~1:0
    float gain_tab[RK_AINR_TAB_LEN];
    float sigma_tab[RK_AINR_TAB_LEN];           // 0.0 ~ 32.0
    float shade_tab[RK_AINR_TAB_LEN];           // -4.0 ~ 4.0
    float sharp_tab[RK_AINR_TAB_LEN];           // 0.0 ~ 4.0
    float min_luma_tab[RK_AINR_TAB_LEN];        // 0.0 ~ 128.0
    float sat_scale_tab[RK_AINR_TAB_LEN];       // 1.0 ~ 3.0
    float dark_contrast_tab[RK_AINR_TAB_LEN];   // 0.0 ~ 32.0
    float ai_ratio_tab[RK_AINR_TAB_LEN];        // 0.0 ~ 1.0
    float mot_thresh_tab[RK_AINR_TAB_LEN];      //  0.0~32.0
    float static_thresh_tab[RK_AINR_TAB_LEN];   //  1~32
    float mot_nr_stren_tab[RK_AINR_TAB_LEN];    //  0.0~2.0

    float luma_curve_tab[RK_AINR_LUMA_LEN];     //  0~255
    float sigma_curve_tab[RK_AINR_LUMA_LEN][RK_AINR_TAB_LEN];   //  0.0~32.0
}rk_ainr_param;

typedef struct module_debug_info_t
{
    uint64_t avgCostUs;
    uint64_t maxCostUs;
    uint64_t nowCostUs;
    uint64_t nowIntervalUs;
    uint64_t avgIntervalUs;
    uint64_t maxIntervalUs;
    uint32_t runningCnt;
    uint32_t runSuccCnt;
    uint32_t runFailCnt;
} rkpostisp_module_debug_info;

typedef struct debug_info_t
{
    rkpostisp_module_debug_info run_proc;
    rkpostisp_module_debug_info npu_proc;
    rkpostisp_module_debug_info post_proc;
} rkpostisp_debug_info;

typedef struct rk_aiisp_frame_t
{
    int src_fd;
    int src_phy_addr;
    void *src_vir_addr;
    int dst_fd;
    int dst_phy_addr;
    void *dst_vir_addr;
    int width;
    int height;
    int vir_width;
    int vir_height;
    void *userdata; // 用户自定义的数据指针，需要传入回调中，可用于通知回调去释放这个MB BLK
    rkpostisp_debug_info debug;
} rk_aiisp_frame;

/*-------------------------------------------
                  Function
-------------------------------------------*/
typedef int (*rkPostIsp_over_callback)(rk_aiisp_frame *);
int rkPostIspInit(rkPostIsp_over_callback callback_ptr, int imgHgt, int imgWid, int imgStd);
int rkPostIspDeinit();
int rkPostIspProc(rk_aiisp_frame *frame, rk_ainr_param *param);
int rkPostIspGetVersion(char *version);

#ifdef __cplusplus
}
#endif