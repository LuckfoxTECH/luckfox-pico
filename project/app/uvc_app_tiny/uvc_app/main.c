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

#define LOG_TAG "rk_mpi_uvc"
#define UVC_VERSION "SDK V2.01"
enum { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };
int rkuvc_log_level = LOG_DEBUG;
char *rkuvc_ini_path_ = NULL;
char *rkuvc_iq_file_path_ = NULL;
// int enable_minilog    = 0;
// int rk_mpi_uvc_log_level = LOG_LEVEL_INFO;
// int app_quit;

static const char short_options[] = "c:a:l:";
static const struct option long_options[] = {
    {"config", required_argument, NULL, 'c'},
    {"aiq_file", no_argument, NULL, 'a'},
    {"log_level", no_argument, NULL, 'l'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}};

static void usage_tip(FILE *fp, int argc, char **argv) {
  fprintf(fp, "Usage: %s [options]\n"
              "Version %s\n"
              "Options:\n"
              "-c | --config      rkuvc ini file, default is "
              "/userdata/rkuvc.ini, need to be writable\n"
              "-a | --aiq_file    aiq file dir path, default is /etc/iqfiles\n"
              "-l | --log_level   log_level [0/1/2/3], default is 2\n"
              "-h | --help        for help \n\n"
              "\n",
          argv[0], "V1.0");
}

#ifdef COMPILE_FOR_UVC_UAC

void *uac_app_thread(void *arg) {
  prctl(PR_SET_NAME, "uac_pthread", 0, 0, 0);

  LOG_INFO("uac uevent version = 1.0\n");
  // create uac control
  char *ch;
  int type = 0; // UAC_API_MPI;
  // rkuac_get_opt(argc, argv);

  int result = uac_control_create(type);
  if (result < 0) {
    LOG_ERROR("uac_control_create fail\n");
    goto err_monitor;
  }

  // register uevent monitor
  uac_uevent_monitor_run();

  while (1) {
    usleep(100000);
  }

err_monitor:
  pthread_detach(pthread_self());
  pthread_exit(NULL);
}
int uac_app_run() {
  pthread_t tid;

  return pthread_create(&tid, NULL, uac_app_thread, NULL);
}
#endif

void rkuvc_get_opt(int argc, char *argv[]) {
  for (;;) {
    int idx;
    int c;
    c = getopt_long(argc, argv, short_options, long_options, &idx);
    if (-1 == c)
      break;
    switch (c) {
    case 0: /* getopt_long() flag */
      break;
    case 'c':
      rkuvc_ini_path_ = optarg;
      break;
    case 'a':
      rkuvc_iq_file_path_ = optarg;
      break;
    case 'l':
      rkuvc_log_level = atoi(optarg);
      break;
    case 'h':
      usage_tip(stdout, argc, argv);
      exit(EXIT_SUCCESS);
    default:
      usage_tip(stderr, argc, argv);
      exit(EXIT_FAILURE);
    }
  }
}
void sigterm_handler(int sig) {
  LOG_INFO("signal %d\n", sig);
  app_quit = sig;
}

int main(int argc, char *argv[]) {
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
  rkuvc_get_opt(argc, argv);
  LOG_INFO("rkuvc_ini_path_ is %s, rkuvc_iq_file_path_ is %s, rkuvc_log_level "
           "is %d\n",
           rkuvc_ini_path_, rkuvc_iq_file_path_, rkuvc_log_level);
  rk_param_init(rkuvc_ini_path_);
  camera_control_init();
  s32Ret = RK_MPI_SYS_Init();
  if (s32Ret != RK_SUCCESS) {
    LOG_ERROR("RK_MPI_SYS_Init %x\n", s32Ret);
    return -1;
  }
  uvc_control_start_setcallback(camera_control_start);
  uvc_control_stop_setcallback(camera_control_stop);
  uevent_monitor_run(UVC_CONTROL_CAMERA);
  uvc_control_run(UVC_CONTROL_CAMERA);

#ifdef COMPILE_FOR_UVC_UAC
  uac_app_log_level = LOG_DEBUG;
  char *uac_log_level = getenv("uac_app_log_level");
  if (uac_log_level) {
    uac_app_log_level = atoi(uac_log_level);
  }
  if (rk_param_get_int("video.source:enable_uac", 0))
    uac_app_run();
#endif

  while (1) {
    if (0 == uvc_control_loop())
      break;
    usleep(100000);
  }
  uvc_video_id_exit_all();
  camera_control_deinit();
#ifdef COMPILE_FOR_UVC_UAC
  if (rk_param_get_int("video.source:enable_uac", 0)) {
    LOG_INFO("uac control destory...\n");
    uac_control_destory();
  }
#endif
  RK_MPI_SYS_Exit();
  rk_param_deinit();
  LOG_INFO("uvc_app exit.\n");
  return 0;
}
