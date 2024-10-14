/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "rkadk_common.h"
#include "rkadk_media_comm.h"
#include "rkadk_log.h"
#include "rkadk_param.h"
#include "rkadk_record.h"
#include "rkadk_osd.h"
#include "isp/sample_isp.h"
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/rtnetlink.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include <sys/time.h>
#ifdef ENABLE_AOV
#include "rkadk_aov.h"
#endif

extern int optind;
extern char *optarg;

static RKADK_CHAR optstr[] = "a:I:p:m:S:c:t:o:x:y:W:H:kdh";

#define MAX_LINE_SIZE 256
#define MAX_NL_BUF_SIZE (1024 * 16)
#define MAX_SELECT_TIMEOUT (2 * 1000 * 1000)

#define MOUNT_DEV_1 "/dev/mmcblk1p1"
#define MOUNT_DEV_2 "/dev/mmcblk1"

#define SDCARD_DEVICE "ffaa0000.mmc"
#define SDCARD_BIND_DONE "bind@/devices/platform/ffaa0000.mmc/mmc_host/mmc1/mmc1"
#define SDCARD_UNBIND_DONE "unbind@/devices/platform/ffaa0000.mmc"
#define SDCARD_DRIVER "/sys/bus/platform/drivers/dwmmc_rockchip"

static bool is_quit = false;
#define IQ_FILE_PATH "/etc/iqfiles"

static void print_usage(const RKADK_CHAR *name) {
  printf("usage example:\n");
  printf("\t%s [-a /etc/iqfiles] [-I 0]\n", name);
  printf("\t-a: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, Default /etc/iqfiles,"
         "without this option aiq should run in other application\n");
  printf("\t-I: Camera id, Default:0\n");
  printf("\t-p: param ini directory path, Default:/data/rkadk\n");
  printf("\t-k: key frame fragment, Default: disable\n");
  printf("\t-m: multiple sensors, Default:0, options: 1(all isp sensors), 2(isp+ahd sensors)\n");
  printf("\t-c: loop switch normal record and aov lapse record count, Default: 0\n");
  printf("\t-t: loop switch once duration(second), Default: 30s\n");
  printf("\t-d: enable debug, Default: disable\n");
  printf("\t-S: aov suspend time, Default: 1000ms\n");
  printf("\t-o: osd file, ARGB8888 fmt, Default:NULL\n");
  printf("\t-x: osd x-coordinate, Default: 0\n");
  printf("\t-y: osd y-coordinate, Default: 0\n");
  printf("\t-W: osd width, Default: 777\n");
  printf("\t-H: osd height, Default: 46\n");
}

static RKADK_S32
GetRecordFileName(RKADK_MW_PTR pRecorder, RKADK_U32 u32FileCnt,
                  RKADK_CHAR (*paszFilename)[RKADK_MAX_FILE_PATH_LEN]) {
  static RKADK_U32 u32FileIdx = 0;

  RKADK_LOGD("u32FileCnt:%d, pRecorder:%p", u32FileCnt, pRecorder);

  if (u32FileIdx >= 50)
    u32FileIdx = 0;

  for (RKADK_U32 i = 0; i < u32FileCnt; i++) {
    sprintf(paszFilename[i], "/mnt/sdcard/RecordTest_%u.mp4", u32FileIdx);
    u32FileIdx++;
  }

  return 0;
}

static RKADK_VOID
RecordEventCallback(RKADK_MW_PTR pRecorder,
                    const RKADK_MUXER_EVENT_INFO_S *pstEventInfo) {
  switch (pstEventInfo->enEvent) {
  case RKADK_MUXER_EVENT_STREAM_START:
    printf("+++++ RKADK_MUXER_EVENT_STREAM_START +++++\n");
    break;
  case RKADK_MUXER_EVENT_STREAM_STOP:
    printf("+++++ RKADK_MUXER_EVENT_STREAM_STOP +++++\n");
    break;
  case RKADK_MUXER_EVENT_FILE_BEGIN:
    printf("+++++ RKADK_MUXER_EVENT_FILE_BEGIN +++++\n");
    printf("\tstFileInfo: %s\n",
           pstEventInfo->unEventInfo.stFileInfo.asFileName);
    printf("\tu32Duration: %d\n",
           pstEventInfo->unEventInfo.stFileInfo.u32Duration);
    break;
  case RKADK_MUXER_EVENT_FILE_END:
    printf("+++++ RKADK_MUXER_EVENT_FILE_END +++++\n");
    printf("\tstFileInfo: %s\n",
           pstEventInfo->unEventInfo.stFileInfo.asFileName);
    printf("\tu32Duration: %d\n",
           pstEventInfo->unEventInfo.stFileInfo.u32Duration);
    break;
  case RKADK_MUXER_EVENT_MANUAL_SPLIT_END:
    printf("+++++ RKADK_MUXER_EVENT_MANUAL_SPLIT_END +++++\n");
    printf("\tstFileInfo: %s\n",
           pstEventInfo->unEventInfo.stFileInfo.asFileName);
    printf("\tu32Duration: %d\n",
           pstEventInfo->unEventInfo.stFileInfo.u32Duration);
    break;
  case RKADK_MUXER_EVENT_ERR_CREATE_FILE_FAIL:
    printf("+++++ RKADK_MUXER_EVENT_ERR_CREATE_FILE_FAIL[%d, %s] +++++\n",
            pstEventInfo->unEventInfo.stErrorInfo.s32ErrorCode,
            strerror(pstEventInfo->unEventInfo.stErrorInfo.s32ErrorCode));
    break;
  case RKADK_MUXER_EVENT_ERR_WRITE_FILE_FAIL:
    printf("+++++ RKADK_MUXER_EVENT_ERR_WRITE_FILE_FAIL[%d, %s] +++++\n",
            pstEventInfo->unEventInfo.stErrorInfo.s32ErrorCode,
            strerror(pstEventInfo->unEventInfo.stErrorInfo.s32ErrorCode));
    break;
  case RKADK_MUXER_EVENT_FILE_WRITING_SLOW:
    printf("+++++ RKADK_MUXER_EVENT_FILE_WRITING_SLOW +++++\n");
    break;
  case RKADK_MUXER_EVENT_ERR_CARD_NONEXIST:
    printf("+++++ RKADK_MUXER_EVENT_ERR_CARD_NONEXIST +++++\n");
    break;
  default:
    printf("+++++ Unknown event(%d) +++++\n", pstEventInfo->enEvent);
    break;
  }
}

#ifdef RKAIQ
static int IspProcess(RKADK_S32 u32CamId) {
  int ret;
  bool mirror = false, flip = false;

  // set mirror flip
  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_MIRROR, &mirror);
  if (ret)
    RKADK_LOGE("RKADK_PARAM_GetCamParam mirror failed");

  ret = RKADK_PARAM_GetCamParam(u32CamId, RKADK_PARAM_TYPE_FLIP, &flip);
  if (ret)
    RKADK_LOGE("RKADK_PARAM_GetCamParam flip failed");

  if (mirror || flip) {
    ret = SAMPLE_ISP_SET_MirrorFlip(u32CamId, mirror, flip);
    if (ret)
      RKADK_LOGE("SAMPLE_ISP_SET_MirrorFlip failed");
  }

#ifdef RKADK_DUMP_ISP_RESULT
  // mirror flip
  ret = SAMPLE_ISP_GET_MirrorFlip(u32CamId, &mirror, &flip);
  if (ret)
    RKADK_LOGE("SAMPLE_ISP_GET_MirrorFlip failed");
  else
    RKADK_LOGD("GET mirror = %d, flip = %d", mirror, flip);
#endif

  return 0;
}
#endif

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  is_quit = true;
}

static bool DeviceDriverIsBound(const char *device, const char *driver) {
  char path[256] = {'\0'};
  snprintf(path, 256, "%s/%s", driver, device);
  return (access(path, F_OK) == 0);
}

static int DeviceAttachDriver(const char *device, const char *driver) {
  char path[256] = {'\0'};
  int fd = -1;
  int ret = 0;

  snprintf(path, sizeof(path), "%s/bind", driver);
  fd = open(path, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
  if (fd < 0) {
    RKADK_LOGE("can't open %s, errno = %s", driver, strerror(errno));
    return RKADK_FAILURE;
  }
  RKADK_LOGD("start bind %s to %s", device, driver);

  ret = write(fd, device, strlen(device));
  if (ret < 0) {
    RKADK_LOGE("bind %s to %s failed, errno = %s", device, driver, strerror(errno));
    close(fd);
    return RKADK_FAILURE;
  }

  close(fd);
  return RKADK_SUCCESS;
}

static int DeviceDetachDriver(const char *device, const char *driver) {
  char path[256] = {'\0'};
  int fd = -1;
  int ret = 0;

  snprintf(path, sizeof(path), "%s/unbind", driver);
  fd = open(path, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
  if (fd < 0) {
    RKADK_LOGD("can't open %s, errno = %s", path, strerror(errno));
    return RKADK_FAILURE;
  }

  RKADK_LOGD("start unbind %s from %s", device, driver);
  ret = write(fd, device, strlen(device));
  if (ret < 0) {
    RKADK_LOGE("unbind %s from %s failed, errno = %s", device, driver, strerror(errno));
    close(fd);
    return RKADK_FAILURE;
  }

  close(fd);
  return RKADK_SUCCESS;
}

static int BindSdcard() {
  int ret = 0;
  int fd = -1;
  char buf[MAX_NL_BUF_SIZE] = {'\0'};
  fd_set read_set;
  struct timeval timeout;
  struct sockaddr_nl addr;

  if (DeviceDriverIsBound(SDCARD_DEVICE, SDCARD_DRIVER)) {
    RKADK_LOGE("sdcard device already bind");
    return RKADK_SUCCESS;
  }

  memset(&addr, 0, sizeof(addr));
  addr.nl_family = AF_NETLINK;
  addr.nl_groups = NETLINK_KOBJECT_UEVENT;
  addr.nl_pid = 0;

  fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
  if (fd < 0) {
    RKADK_LOGE("Failed to open sdcard netlink, errno = %s", strerror(errno));
    return RKADK_FAILURE;
  } else if (bind(fd, (struct sockaddr *)(&addr), sizeof(addr)) != 0) {
    RKADK_LOGE("bind sdcard netlink addr failed, errno = %s", strerror(errno));
    goto __FAILED;
  }

  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeout.tv_sec = 0;
  timeout.tv_usec = MAX_SELECT_TIMEOUT;

  // bind sdcard
  if (DeviceAttachDriver(SDCARD_DEVICE, SDCARD_DRIVER) != RKADK_SUCCESS)
    goto __FAILED;

  // wait for bind success
__RETRY:
  ret = select(fd + 1, &read_set, NULL, NULL, &timeout);
  if (ret > 0) {
    memset(&buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf));
    buf[MAX_NL_BUF_SIZE - 1] = '\0';
    // printf("[%s()] bind msg: %s\n", __func__, buf);
    if (strncmp(buf, SDCARD_BIND_DONE, strlen(SDCARD_BIND_DONE)) == 0) {
      RKADK_LOGD("Bind success: %s", buf);
      goto __SUCCESS;
    }
    goto __RETRY; // drop all message
  } else {
    RKADK_LOGE("select error = %s", strerror(errno));
    goto __FAILED;
  }

__SUCCESS:
  close(fd);
  return RKADK_SUCCESS;

__FAILED:
  close(fd);
  return RKADK_FAILURE;
}

static int UnbindSdcard() {
  int ret = 0;
  int fd = -1;
  char buf[MAX_NL_BUF_SIZE] = {'\0'};
  fd_set read_set;
  struct timeval timeout;
  struct sockaddr_nl addr;

  if (!DeviceDriverIsBound(SDCARD_DEVICE, SDCARD_DRIVER)) {
    RKADK_LOGE("sdcard device already unbind!");
    return RKADK_SUCCESS;
  }

  memset(&addr, 0, sizeof(addr));
  addr.nl_family = AF_NETLINK;
  addr.nl_groups = NETLINK_KOBJECT_UEVENT;
  addr.nl_pid = 0;

  fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
  if (fd < 0) {
    RKADK_LOGE("Failed to open sdcard netlink, errno = %s", strerror(errno));
    return RKADK_FAILURE;
  } else if (bind(fd, (struct sockaddr *)(&addr), sizeof(addr)) != 0) {
    RKADK_LOGE("bind sdcard netlink addr failed, errno = %s", strerror(errno));
    goto __FAILED;
  }

  memset(&buf, 0, sizeof(buf));
  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeout.tv_sec = 0;
  timeout.tv_usec = MAX_SELECT_TIMEOUT;

  // unbind sdcard
  if (DeviceDetachDriver(SDCARD_DEVICE, SDCARD_DRIVER) != RKADK_SUCCESS)
    goto __FAILED;

  // wait for unbind success
__RETRY:
  ret = select(fd + 1, &read_set, NULL, NULL, &timeout);
  if (ret > 0) {
    memset(&buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf));
    buf[MAX_NL_BUF_SIZE - 1] = '\0';
    // printf("[%s()] unbind msg: %s\n", __func__, buf);
    if (strcmp(buf, SDCARD_UNBIND_DONE) == 0) {
      RKADK_LOGD("Unbind success: %s", buf);
      goto __SUCCESS;
    }
    goto __RETRY; // drop all message
  } else {
    RKADK_LOGE("select error %s", strerror(errno));
    goto __FAILED;
  }

__SUCCESS:
  close(fd);
  return RKADK_SUCCESS;

__FAILED:
  close(fd);
  return RKADK_FAILURE;
}

static int CheckSDcardMount(void) {
  int fd = -1, ret = -1, pos = 0;
  char line[MAX_LINE_SIZE];
  ssize_t bytesRead;

  fd = open("/proc/mounts", O_RDONLY);
  if (fd == -1) {
    RKADK_LOGE("Error opening /proc/mounts");
    return ret;
  }

  while ((bytesRead = read(fd, &line[pos], 1)) > 0) {
    if (line[pos] == '\n') {
      line[pos] = '\0'; // Null-terminate the string
      if (strstr(line, "/mnt/sdcard")) {
        RKADK_LOGD("Found '/mnt/sdcard' in line: %s", line);
        ret = 0;
        break; // No need to continue searching
      }
      pos = 0; // Reset position for next line
    } else {
      pos++;
      if (pos >= MAX_LINE_SIZE - 1) {
        // Line exceeds buffer size, discard it
        pos = 0;
      }
    }
  }

  close(fd);
  return ret;
}

static int MountSdcard() {
  RKADK_LOGD("Enter mount");

  int ret = 0;

  BindSdcard();

  if (CheckSDcardMount() == 0) {
    RKADK_LOGD("sdcard already mount");
    return 0;
  }

  // mount sd
  if (access("/mnt/sdcard", F_OK) == 0) {
#if 0
    system("mount -t vfat /dev/mmcblk1p1 /mnt/sdcard/");
#else
    ret = mount(MOUNT_DEV_1, "/mnt/sdcard", "vfat", 0, NULL);
    if (ret != 0)
      RKADK_LOGE("mount failed, errno = %s", strerror(errno));
    else
      RKADK_LOGD("mount success");
#endif
  } else if (access(MOUNT_DEV_2, F_OK) == 0) {
#if 0
    system("mount -t vfat /dev/mmcblk1 /mnt/sdcard/");
#else
    ret = mount(MOUNT_DEV_2, "/mnt/sdcard", "vfat", 0, NULL);
    if (ret != 0)
      RKADK_LOGE("mount failed, errno = %s", strerror(errno));
    else
      RKADK_LOGD("mount success");
#endif
  } else {
    RKADK_LOGE("bad mount path!");
  }

  if (0 != CheckSDcardMount()) {
    RKADK_LOGE("Not found mount sdcard on /mnt/sdcard");
    UnbindSdcard();
    ret = -1;
  }

  RKADK_LOGD("Exit mount");
  return ret;
}

static int UmountSdcard() {
  int ret = 0;

  RKADK_LOGD("Enter umount");
  umount2("/mnt/sdcard", MNT_DETACH);
  ret = UnbindSdcard();
  RKADK_LOGD("Exit umount");
  return ret;
}

static void AovNotifyCallback(RKADK_AOV_EVENT_E enEvent, void *msg) {
  switch(enEvent) {
  case RKADK_AOV_ENTER_SLEEP:
    RKADK_LOGD("+++++ RKADK_AOV_ENTER_SLEEP +++++");

    RKADK_AOV_WakeupLock();
    RKADK_LOGD("fs sdcard lock");
    UmountSdcard();
    RKADK_AOV_EnterSleep();
    RKADK_AOV_WakeupUnlock();
    RKADK_LOGD("fs sdcard unlock");
    break;

  default:
    RKADK_LOGD("Unknown event: %d", enEvent);
    break;
  }
}

int main(int argc, char *argv[]) {
  int c, ret;
  RKADK_RECORD_ATTR_S stRecAttr;
  RKADK_MW_PTR pRecorder = NULL, pRecorder1 = NULL;
  RK_BOOL bMultiSensor = RK_FALSE;
  const char *iniPath = NULL;
  RKADK_REC_TYPE_E enRecType;
  RKADK_PARAM_RES_E type;
  RKADK_PARAM_FPS_S stFps;
  RKADK_PARAM_CODEC_CFG_S stCodecType;
  char path[RKADK_PATH_LEN];
  char sensorPath[RKADK_MAX_SENSOR_CNT][RKADK_PATH_LEN];
  RKADK_S32 s32CamId = 0;
  FILE_CACHE_ARG stFileCacheAttr;
  int loopCount = -1, loopDuration = 30;
  bool bDebug = false;

  //osd
  char *osdfile = NULL;
  RKADK_U32 u32OsdX = 0, u32OsdY = 0;
  RKADK_U32 u32OsdWidth = 777, u32OsdHeight = 46;
  RKADK_OSD_ATTR_S OsdAttr;
  RKADK_OSD_STREAM_ATTR_S OsdStreamAttr;
  RKADK_U32 u32OsdId = 0;

#ifdef ENABLE_AOV
  RKADK_S32 s32SuspendTime = 1000;
  RKADK_AOV_ARG_S stAovArg;
#endif

#ifdef RKAIQ
  int inCmd = 0;
  RK_BOOL bMultiCam = RK_FALSE;
  const char *tmp_optarg = optarg;
  SAMPLE_ISP_PARAM stIspParam;

  memset(&stIspParam, 0, sizeof(SAMPLE_ISP_PARAM));
  stIspParam.iqFileDir = IQ_FILE_PATH;
#endif

  memset(&stRecAttr, 0, sizeof(RKADK_RECORD_ATTR_S));

  while ((c = getopt(argc, argv, optstr)) != -1) {
    switch (c) {
#ifdef RKAIQ
    case 'a':
      if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
        tmp_optarg = argv[optind++];
      }

      if (tmp_optarg)
        stIspParam.iqFileDir = (char *)tmp_optarg;
      break;
    case 'm':
      inCmd = atoi(optarg);
      if (inCmd == 1) {
        bMultiCam = RKADK_TRUE;
        bMultiSensor = RKADK_TRUE;
      } else if (inCmd == 2)
        bMultiSensor = RKADK_TRUE;
      break;
#endif
    case 'I':
      s32CamId = atoi(optarg);
      break;
    case 'p':
      iniPath = optarg;
      RKADK_LOGD("iniPath: %s", iniPath);
      break;
    case 'k':
      stRecAttr.u32FragKeyFrame = 1;
      break;
#ifdef ENABLE_AOV
    case 'S':
      s32SuspendTime = atoi(optarg);
      break;
#endif
    case 'c':
      loopCount = atoi(optarg);
      break;
    case 't':
      loopDuration = atoi(optarg);
      break;
    case 'd':
      bDebug = true;
      break;
    case 'o':
      osdfile = optarg;
      break;
    case 'x':
      u32OsdX = atoi(optarg);
      break;
    case 'y':
      u32OsdY = atoi(optarg);
      break;
    case 'W':
      u32OsdWidth = atoi(optarg);
      break;
    case 'H':
      u32OsdHeight = atoi(optarg);
      break;
    case 'h':
    default:
      print_usage(argv[0]);
      optind = 0;
      return -1;
    }
  }
  optind = 0;

#ifdef ENABLE_AOV
  memset(&stAovArg, 0, sizeof(RKADK_AOV_ARG_S));
  stAovArg.pfnNotifyCallback = AovNotifyCallback;
  RKADK_AOV_Init(&stAovArg);

  RKADK_LOGD("s32SuspendTime: %d", s32SuspendTime);
  RKADK_AOV_SetSuspendTime(s32SuspendTime);
#endif

  if (bMultiSensor)
    s32CamId = 0;

  RKADK_MPI_SYS_Init();

  if (iniPath) {
    memset(path, 0, RKADK_PATH_LEN);
    memset(sensorPath, 0, RKADK_MAX_SENSOR_CNT * RKADK_PATH_LEN);
    sprintf(path, "%s/rkadk_setting.ini", iniPath);
    for (int i = 0; i < RKADK_MAX_SENSOR_CNT; i++)
      sprintf(sensorPath[i], "%s/rkadk_setting_sensor_%d.ini", iniPath, i);

    /*
    lg:
      char *sPath[] = {"/data/rkadk/rkadk_setting_sensor_0.ini",
      "/data/rkadk/rkadk_setting_sensor_1.ini", NULL};
    */
    char *sPath[] = {sensorPath[0], sensorPath[1], NULL};

    RKADK_PARAM_Init(path, sPath);
  } else {
    RKADK_PARAM_Init(NULL, NULL);
  }

record:
#ifdef RKAIQ
  stFps.enStreamType = RKADK_STREAM_TYPE_SENSOR;
  ret = RKADK_PARAM_GetCamParam(s32CamId, RKADK_PARAM_TYPE_FPS, &stFps);
  if (ret) {
    RKADK_LOGE("RKADK_PARAM_GetCamParam u32CamId[%d] fps failed", s32CamId);
    return -1;
  }

  stIspParam.WDRMode = RK_AIQ_WORKING_MODE_NORMAL;
  stIspParam.bMultiCam = bMultiCam;
  stIspParam.fps = stFps.u32Framerate;
  SAMPLE_ISP_Start(s32CamId, stIspParam);
  //IspProcess(s32CamId);

  if (bMultiCam) {
    ret = RKADK_PARAM_GetCamParam(1, RKADK_PARAM_TYPE_FPS, &stFps);
    if (ret) {
      RKADK_LOGE("RKADK_PARAM_GetCamParam u32CamId[1] fps failed");
      SAMPLE_ISP_Stop(s32CamId);
      return -1;
    }

    SAMPLE_ISP_Start(1, stIspParam);
    //IspProcess(1);
  }
#endif

  //enable file cache
  ret = putenv("file_cache_env=1");
  if (ret)
    RKADK_LOGE("putenv file_cache_env failed");

  if (bDebug) {
    ret = putenv("file_cache_log=6");
      RKADK_LOGE("putenv file_cache_log failed");
  }

  memset(&stFileCacheAttr, 0, sizeof(FILE_CACHE_ARG));
  stFileCacheAttr.sdcard_path = "/dev/mmcblk1p1";
  stFileCacheAttr.total_cache = 7 * 1024 * 1024; // 7M
  stFileCacheAttr.write_cache = 256 * 1024; //1024 * 1024; // 1M
  stFileCacheAttr.write_thread_arg.sched_policy = FILE_SCHED_FIFO;
  stFileCacheAttr.write_thread_arg.priority = 99;
  stFileCacheAttr.sdcard_arg.mount_sdcard = MountSdcard;
  stFileCacheAttr.sdcard_arg.umount_sdcard = UmountSdcard;

#ifdef ENABLE_AOV
  stFileCacheAttr.sdcard_arg.lock = RKADK_AOV_WakeupLock;
  stFileCacheAttr.sdcard_arg.unlock = RKADK_AOV_WakeupUnlock;
#endif
  RKADK_RECORD_FileCacheInit(&stFileCacheAttr);

  stRecAttr.s32CamID = s32CamId;
  stRecAttr.pfnRequestFileNames = GetRecordFileName;
  stRecAttr.pfnEventCallback = RecordEventCallback;

#ifdef ENABLE_AOV
  stRecAttr.stAovAttr.pfnSingleFrame = SAMPLE_ISP_SingleFrame;
  stRecAttr.stAovAttr.pfnMultiFrame = SAMPLE_ISP_MultiFrame;
#endif

  if (RKADK_RECORD_Create(&stRecAttr, &pRecorder)) {
    RKADK_LOGE("s32CamId[%d] Create recorder failed", s32CamId);
#ifdef RKAIQ
    SAMPLE_ISP_Stop(s32CamId);
    if (bMultiCam)
      SAMPLE_ISP_Stop(1);
#endif
    return -1;
  }

  if (osdfile) {
    memset(&OsdAttr, 0, sizeof(RKADK_OSD_ATTR_S));
    memset(&OsdStreamAttr, 0, sizeof(RKADK_OSD_STREAM_ATTR_S));
    OsdAttr.Format = RKADK_FMT_ARGB8888;
    OsdAttr.Width = u32OsdWidth;
    OsdAttr.Height = u32OsdHeight;
    OsdAttr.pData = malloc(OsdAttr.Height * OsdAttr.Width * 4);

#ifdef RV1106_1103
    OsdAttr.enOsdType = RKADK_OSD_TYPE_NORMAL;
#else
    OsdAttr.enOsdType = RKADK_OSD_TYPE_EXTRA;
#endif

    OsdStreamAttr.Origin_X = u32OsdX;
    OsdStreamAttr.Origin_Y = u32OsdY;
    OsdStreamAttr.bEnableShow = RKADK_TRUE;
    OsdStreamAttr.enOsdType = OsdAttr.enOsdType;

    RKADK_OSD_Init(u32OsdId, &OsdAttr);
    RKADK_OSD_AttachToStream(u32OsdId, s32CamId, RKADK_STREAM_TYPE_VIDEO_MAIN, &OsdStreamAttr);

    FILE *fp = fopen(osdfile, "rw");
    if (!fp) {
      RKADK_LOGD("open osd file fail");
    } else {
      RKADK_LOGD("open osd file success");
      fread((RKADK_U8 *)OsdAttr.pData, OsdAttr.Width * OsdAttr.Height * 4, 1, fp);
      fclose(fp);
      RKADK_OSD_UpdateBitMap(u32OsdId, &OsdAttr);
    }
  }

  RKADK_RECORD_Start(pRecorder);

  if (bMultiSensor) {
    stRecAttr.s32CamID = 1;
    if (RKADK_RECORD_Create(&stRecAttr, &pRecorder1)) {
      RKADK_LOGE("s32CamId[1] Create recorder failed");
#ifdef RKAIQ
      SAMPLE_ISP_Stop(s32CamId);
      if (bMultiCam)
        SAMPLE_ISP_Stop(1);
#endif
      return -1;
    }

    RKADK_RECORD_Start(pRecorder1);
  }

  RKADK_LOGD("initial finish\n");

  signal(SIGINT, sigterm_handler);
  char cmd[64];
  printf("\n#Usage: input 'quit' to exit programe!\n"
         "peress any other key to quit\n");

  while (!is_quit) {
    if (loopCount >= 0) {
      sleep(loopDuration);
      if (loopCount == 0) {
        RKADK_LOGD("loop switch end!");
        is_quit = true;
        goto __EXIT;
      }

      RKADK_PARAM_GetCamParam(s32CamId, RKADK_PARAM_TYPE_RECORD_TYPE, &enRecType);
      if (enRecType == RKADK_REC_TYPE_NORMAL) {
        enRecType = RKADK_REC_TYPE_AOV_LAPSE;
        printf("\n\n\n----- switch aov lapse record[%d] -----\n", loopCount);
      } else {
        printf("\n\n\n----- switch normal record[%d] -----\n", loopCount);
        enRecType = RKADK_REC_TYPE_NORMAL;
      }

      RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_RECORD_TYPE, &enRecType);
      RKADK_RECORD_Reset(&pRecorder);
      RKADK_RECORD_FileCacheSetMode(enRecType);
      RKADK_RECORD_Start(pRecorder);

      loopCount--;
    } else {
      fgets(cmd, sizeof(cmd), stdin);
      if (strstr(cmd, "quit") || is_quit) {
        RKADK_LOGD("#Get 'quit' cmd!");
        break;
      } else if (strstr(cmd, "LR")) { //Lapse Record
        enRecType = RKADK_REC_TYPE_LAPSE;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_RECORD_TYPE, &enRecType);
        RKADK_RECORD_Reset(&pRecorder);
        RKADK_RECORD_FileCacheSetMode(enRecType);
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "NR")) { //Normal Record
        enRecType = RKADK_REC_TYPE_NORMAL;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_RECORD_TYPE, &enRecType);
        RKADK_RECORD_Reset(&pRecorder);
        RKADK_RECORD_FileCacheSetMode(enRecType);
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "720")) {
        type = RKADK_RES_720P;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_RES, &type);
        ret = RKADK_RECORD_Reset(&pRecorder);
        if (ret < 0) {
#ifndef RV1106_1103
          RKADK_RECORD_Stop(pRecorder);
          RKADK_RECORD_Destroy(pRecorder);
          pRecorder = NULL;
#ifdef RKAIQ
          SAMPLE_ISP_Stop(stRecAttr.s32CamID);
#endif
          goto record;
#endif
        }
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "1080")) {
        type = RKADK_RES_1080P;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_RES, &type);
        ret = RKADK_RECORD_Reset(&pRecorder);
        if (ret < 0) {
#ifndef RV1106_1103
          RKADK_RECORD_Stop(pRecorder);
          RKADK_RECORD_Destroy(pRecorder);
          pRecorder = NULL;
#ifdef RKAIQ
          SAMPLE_ISP_Stop(stRecAttr.s32CamID);
#endif
          goto record;
#endif
        }
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "264")) {
        stCodecType.enCodecType = RKADK_CODEC_TYPE_H264;
        stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
        stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_SUB;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
        ret = RKADK_RECORD_Reset(&pRecorder);
        if (ret < 0) {
#ifndef RV1106_1103
          RKADK_RECORD_Stop(pRecorder);
          RKADK_RECORD_Destroy(pRecorder);
          pRecorder = NULL;
#ifdef RKAIQ
          SAMPLE_ISP_Stop(stRecAttr.s32CamID);
#endif
          goto record;
#endif
        }
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "265")) {
        stCodecType.enCodecType = RKADK_CODEC_TYPE_H265;
        stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_MAIN;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
        stCodecType.enStreamType = RKADK_STREAM_TYPE_VIDEO_SUB;
        RKADK_PARAM_SetCamParam(s32CamId, RKADK_PARAM_TYPE_CODEC_TYPE, &stCodecType);
        ret = RKADK_RECORD_Reset(&pRecorder);
        if (ret < 0) {
#ifndef RV1106_1103
          RKADK_RECORD_Stop(pRecorder);
          RKADK_RECORD_Destroy(pRecorder);
          pRecorder = NULL;
#ifdef RKAIQ
          SAMPLE_ISP_Stop(stRecAttr.s32CamID);
#endif
          goto record;
#endif
        }
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "start")) {
        RKADK_RECORD_Start(pRecorder);
      } else if (strstr(cmd, "stop")) {
        RKADK_RECORD_Stop(pRecorder);
      }

      usleep(500000);
    }
  }

__EXIT:
  if (osdfile) {
    RKADK_OSD_DettachFromStream(u32OsdId, s32CamId, RKADK_STREAM_TYPE_VIDEO_MAIN);
    RKADK_OSD_Deinit(u32OsdId);

    if (OsdAttr.pData)
      free(OsdAttr.pData);
  }

  RKADK_RECORD_Stop(pRecorder);
  RKADK_RECORD_Destroy(pRecorder);

#ifdef RKAIQ
  SAMPLE_ISP_Stop(s32CamId);
#endif

  if (bMultiSensor) {
    RKADK_RECORD_Stop(pRecorder1);
    RKADK_RECORD_Destroy(pRecorder1);

#ifdef RKAIQ
    if (bMultiCam)
      SAMPLE_ISP_Stop(1);
#endif
  }

  RKADK_RECORD_FileCacheDeInit();
  RKADK_MPI_SYS_Exit();

#ifdef ENABLE_AOV
    RKADK_AOV_DeInit();
#endif

  RKADK_LOGD("exit!");
  return 0;
}
