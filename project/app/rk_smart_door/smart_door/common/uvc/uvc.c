#include "camera_control.h"
#include "isp.h"
#include "param.h"
#include "rk_mpi_sys.h"
#include "uevent.h"
#include "uevent.h"
#include "uvc_control.h"
#include "uvc_log.h"
#include "uvc_video.h"
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#ifdef COMPILE_FOR_UVC_UAC
#include "uac_control.h"
#include "uac_log.h"
#include "uac_uevent.h"
#include <pthread.h>
#include <sys/prctl.h>
#endif


#define UVC_VERSION "SDK V2.01"
enum { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };
int rkuvc_log_level = LOG_DEBUG;
char *rkuvc_iq_file_path_ = NULL;
// int enable_minilog    = 0;
// int rk_mpi_uvc_log_level = LOG_LEVEL_INFO;
// int app_quit;


void sigterm_handler(int sig) {
  LOG_INFO("signal %d\n", sig);
  app_quit = sig;
}

int uvc_start() {
  int fd;
  int ret;
  unsigned int handle;
  char *buffer;
  int handle_fd;
  size_t size;
  int i = 0;
  int width, height;
  int y, uv;
  int extra_cnt = 0;
  uint32_t flags = 0;
  int media_set = 0;
  RK_S32 s32Ret = RK_FAILURE;
  rk_mpi_uvc_log_level = LOG_LEVEL_INFO;
  app_quit = 0;
  signal(SIGQUIT, sigterm_handler);
  signal(SIGTERM, sigterm_handler);
  char *log_level = getenv("rk_mpi_uvc_log_level");
  if (log_level) {
    LOG_INFO("rk_mpi_uvc_log_level=%d", atoi(log_level));
    rk_mpi_uvc_log_level = atoi(log_level);
  }
  LOG_INFO("VERSION:%s %s %s \n", UVC_VERSION, __DATE__, __TIME__);

  camera_control_init();
  uvc_control_start_setcallback(camera_control_start);
  uvc_control_stop_setcallback((void *)camera_control_stop);
  uevent_monitor_run(UVC_CONTROL_CAMERA);
  uvc_control_run(UVC_CONTROL_CAMERA);


  while (1) {
    if (0 == uvc_control_loop())
      break;
    usleep(100000);
  }
  uvc_video_id_exit_all();
  camera_control_deinit();

  LOG_INFO("uvc_app exit.\n");
  return 0;
}

int uvc_stop(void) {
  int video_id = uvc_video_id_get(0);
  uvc_set_user_run_state(false, video_id);
}
