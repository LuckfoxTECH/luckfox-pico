#ifndef __APP_PARAM_INFO_H__
#define __APP_PARAM_INFO_H__

typedef enum {
	RK_VENC_TYPE_H264 = 1,
	RK_VENC_TYPE_H265,
	RK_VENC_TYPE_MJPEG,
	RK_VENC_TYPE_INVALID
} eVENC_TYPE;

struct app_param_info
{
	uint32_t head;
	int32_t len;
	int32_t cam_mirror_flip;
	int32_t cam_fps;
	int32_t cam2_fps;

	int32_t night_mode;
	int32_t led_value;
	int32_t venc_w;
	int32_t venc_h;
	int32_t venc_type;
	int32_t venc_bitrate;

	int32_t cam_hdr;
	int32_t cam2_hdr;
	int32_t color_mode;
	int32_t cam_index;

	int32_t fastae_max_frame;

	int32_t cam_num;

	int32_t venc2_w;
	int32_t venc2_h;
	int32_t venc2_type;
	int32_t venc2_bitrate;

	int32_t cam2_mirror_flip;

	uint32_t crc32;
};
#define RK_APP_PARAM_OFFSET   "rk_app_param_offset"
#define RK_CAM_W              "rk_cam_w"
#define RK_CAM_H              "rk_cam_h"
#define RK_VENC_W             "rk_venc_w"
#define RK_VENC_H             "rk_venc_h"
#define RK_VENC_TYPE          "rk_venc_type"
#define RK_VENC_BITRATE       "rk_venc_bitrate"

#define RK_CAM2_W              "rk_cam2_w"
#define RK_CAM2_H              "rk_cam2_h"
#define RK_VENC2_W             "rk_venc2_w"
#define RK_VENC2_H             "rk_venc2_h"
#define RK_VENC2_TYPE          "rk_venc2_type"
#define RK_VENC2_BITRATE       "rk_venc2_bitrate"

#define RK_CAM_MIRROR_FLIP    "rk_cam_mirror_flip"
#define RK_CAM_FPS            "rk_cam_fps"
#define RK_CAM2_FPS            "rk_cam2_fps"
#define RK_CAM_HDR            "rk_cam_hdr"
#define RK_CAM2_HDR            "rk_cam2_hdr"
#define RK_CAM_INDEX          "rk_cam_index"
#define RK_LED_VALUE          "rk_led_value"
#define RK_NIGHT_MODE         "rk_night_mode"
#define RK_IQ_BIN_ADDR_STRINGS "rk_iqbin_addr"
#define RK_IQ_BIN_SIZE_STRINGS "rk_iqbin_size"
#define RK_IQ_BIN_SECONDARY_ADDR_STRINGS "rk_iqbin_secondary_addr"
#define RK_IQ_BIN_SECONDARY_SIZE_STRINGS "rk_iqbin_secondary_size"
#define RK_FULLCOLOR_GRAY_STRINGS "rk_color_mode"
#define RK_FASTAE_MAX_FRAME "rk_fastae_max_frame"
#define RK_CAM_SENSOR_NUM "rk_cam_num"

#define RK_CAM2_MIRROR_FLIP    "rk_cam2_mirror_flip"

#endif//  __APP_PARAM_INFO_H__
