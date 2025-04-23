
#include<rk_common.h>


extern int g_video_run_;
extern int enable_ivs, enable_jpeg, enable_venc_0, enable_venc_1, enable_rtsp, enable_rtmp;
extern int pipe_id_;
extern int cycle_snapshot_flag;
extern int send_jpeg_cnt;
extern int get_jpeg_cnt;
extern VO_DEV VoLayer;
#if HAS_VO
void *saix_send_cam_input_to_display(void *arg);

#endif

void *saix_rtsp_stream_from_venc0(void *arg);
void *saix_vi_snapshot_jpeg(void *arg);
void *saix_vi_overlay_to_venc(void *arg);
void *saix_venc1_dispatch(void *arg);
void *saix_process_jpeg(void *arg);
void *saix_cycle_snapshot(void *arg);
void *saix_push_vi2_to_iva(void *arg);
void *saix_get_vpss_bgr_frame(void *arg);
void *saix_fetch_ivs_results(void *arg);
void *saix_push_vi2_to_iva(void *arg);
