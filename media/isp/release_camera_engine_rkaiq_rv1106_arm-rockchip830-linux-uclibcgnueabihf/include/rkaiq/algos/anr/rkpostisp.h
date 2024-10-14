#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
// typedef signed char   int8;
// typedef unsigned char uint8;
// typedef short         int16;

#define RK_POSTISP_SIMULATE                     0
#define RK_POSTISP_MAX_HGT                      1520
#define RK_POSTISP_MAX_WID                      2688

/*-------------------------------------------
                  Struct
-------------------------------------------*/
#define     RK_AINR_TAB_LEN      13
#define     RK_AINR_LUMA_LEN     6

typedef struct rk_ainr_param_t
{
    int   enable;
    float gain;
    float RawMeanluma;
    float YuvMeanluma;

    int     gain_tab_len;
    int     gain_max;
    int     tuning_visual_flag;   //  0~1:0
    float gain_tab[RK_AINR_TAB_LEN];
    float sigma_tab[RK_AINR_TAB_LEN];   //  0.0~32.0
    float shade_tab[RK_AINR_TAB_LEN];   //  -4.0~4.0
    float sharp_tab[RK_AINR_TAB_LEN];   //  0.0~4.0
    float min_luma_tab[RK_AINR_TAB_LEN];    //  0.0~256.0
    float sat_scale_tab[RK_AINR_TAB_LEN];   //  1.0~3.0
    float dark_contrast_tab[RK_AINR_TAB_LEN];   //  0.0~32.0
    float ai_ratio_tab[RK_AINR_TAB_LEN];    //  0.0~1.0
    float mot_thresh_tab[RK_AINR_TAB_LEN];  //  0.0~32.0
    float static_thresh_tab[RK_AINR_TAB_LEN];   //  1~32
    float mot_nr_stren_tab[RK_AINR_TAB_LEN];    //  0.0~2.0

    float luma_curve_tab[RK_AINR_LUMA_LEN]; //  0~255
    float sigma_curve_tab[RK_AINR_LUMA_LEN][RK_AINR_TAB_LEN];   //  0.0~32.0
}rk_ainr_param;

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
} rk_aiisp_frame;

/*-------------------------------------------
                  Function
-------------------------------------------*/
typedef int (*rkPostIsp_over_callback)(rk_aiisp_frame *);
int rkPostIspInit(rkPostIsp_over_callback callback_ptr, int imgHgt, int imgWid, int imgStd);

int rkPostIspDeinit();

int rkPostIspProc(rk_aiisp_frame *frame, rk_ainr_param *param);

#if RK_POSTISP_SIMULATE
int rkSimulatorInit(int gain, int YuvMeanluma, int RawMeanluma);

int rkSimulatorDeinit();

int rkSimulatorProc(rk_aiisp_frame *frame, int imgHgt, int imgWid, int imgStd);
#endif

#ifdef __cplusplus
}
#endif
