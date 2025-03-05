#ifndef __UVC_IPC_EXT_H__
#define __UVC_IPC_EXT_H__

#ifdef __cplusplus
extern "C" {
#endif
#if DBUG
#define UVC_DYNAMIC_DEBUG_USE_TIME 1 // release version can set to 0
#define UVC_DYNAMIC_DEBUG_USE_TIME_CHECK "/tmp/uvc_use_time"
#else
#define UVC_DYNAMIC_DEBUG_USE_TIME 0
#endif

enum UVC_IPC_EVENT {
  UVC_IPC_EVENT_START = 1,
  UVC_IPC_EVENT_STOP = 2,
  UVC_IPC_EVENT_ENABLE_ETPTZ = 3,
  UVC_IPC_EVENT_SET_ZOOM = 4,
  UVC_IPC_EVENT_RET_TRANSPORT_BUF = 5,
  UVC_IPC_EVENT_CONFIG_CAMERA = 6,
  UVC_IPC_EVENT_SET_EPTZ_PAN = 7,
  UVC_IPC_EVENT_SET_EPTZ_TILT = 8,
  UVC_IPC_EVENT_ENABLE_BYPASS = 9
};

enum UVC_IPC_ENC_TYPE {
  UVC_IPC_ENC_YUV = 0,
  UVC_IPC_ENC_MJPEG_NORMAL,
  UVC_IPC_ENC_MJPEG_LOW_LATENCY,
  UVC_IPC_ENC_H264,
  UVC_IPC_ENC_H265
};

struct CAMERA_INFO {
  int width;
  int height;
  int vir_width;
  int vir_height;
  int buf_size;
  int range;
  enum UVC_IPC_ENC_TYPE encode_type;
  int uvc_fps_set;
};

extern void uvc_ipc_event(enum UVC_IPC_EVENT event, void *data);
extern void uvc_ipc_reconnect(void);

#ifdef __cplusplus
}
#endif

#endif
