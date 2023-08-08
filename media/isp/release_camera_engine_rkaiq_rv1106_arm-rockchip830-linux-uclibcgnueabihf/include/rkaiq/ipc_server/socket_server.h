#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "MessageParser.hpp"
#include "include/algos/adehaze/rk_aiq_types_adehaze_algo_int.h"
#include "include/algos/adpcc/rk_aiq_types_adpcc_algo_int.h"
#include "include/algos/ae/rk_aiq_uapi_ae_int_types_v1.h"
#include "include/algos/ae/rk_aiq_uapi_ae_int_types_v2.h"
#include "include/algos/agamma/rk_aiq_types_agamma_algo_int.h"
#include "include/algos/amerge/rk_aiq_types_amerge_algo_int.h"
#include "include/algos/anr/rk_aiq_types_anr_algo_int.h"
#include "include/algos/atmo/rk_aiq_types_atmo_algo_int.h"
#include "include/uAPI2/rk_aiq_user_api2_adpcc.h"
#include "include/uAPI2/rk_aiq_user_api2_helper.h"
#include "include/uAPI2/rk_aiq_user_api2_sysctl.h"
#include "include/uAPI2/rk_aiq_user_api2_awb.h"
#include "rk_aiq_user_api2_ae.h"
#include "rk_aiq_user_api_ae.h"
#include "rkaiq_tool_ae.h"
#include "rkaiq_tool_anr.h"
#include "rkaiq_tool_imgproc.h"
#include "rkaiq_tool_sharp.h"
#include "rkaiq_tool_sysctl.h"
#include "xcam_log.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <mutex>
#include <poll.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

#define MAXPACKETSIZE 18192
#define MAX_CLIENT 1000

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

using RecvCallBack =
    add_pointer<void(int sockfd, char *buffer, int size)>::type;

class SocketServer {
public:
  SocketServer();
  virtual ~SocketServer();

  int Send(int cilent_socket, char *buff, int size);
  int Process(rk_aiq_sys_ctx_t *aiq_ctx);

  void RegisterRecvCallBack(RecvCallBack cb) { callback_ = cb; }
  void UnRegisterRecvCallBack() { callback_ = nullptr; }
  void SaveEixt();
  void Deinit();

  bool is_connected() { return tool_mode_on; }

  void tool_mode_set(bool status) { tool_mode_on = status; }
  int packetHandle(void *packet, MessageType type);

  static std::mutex send_mutex;

private:
  void Accepted();
  int Recvieve();
  int Recvieve(int sync);
  int poll_event(int timeout_msec, int fds[]);
#ifdef __ANDROID__
  int getAndroidLocalSocket();
#endif

private:
  bool tool_mode_on;
  int sockfd;
  int client_socket;
  int quit_;
  struct sockaddr_un serverAddress;
  struct sockaddr_un clientAddress;
  rk_aiq_sys_ctx_t *aiq_ctx;
  std::unique_ptr<std::thread> accept_threads_;
  std::unique_ptr<RkMSG::MessageParser> msg_parser;
  std::shared_ptr<std::thread> tunning_thread;
  RecvCallBack callback_;
  int _stop_fds[2];
};

void hexdump2(char *buf, const int num);
int ProcessCommand(rk_aiq_sys_ctx_t *ctx, RkAiqSocketPacket *dataRecv,
                   RkAiqSocketPacket *dataReply);

#define CHECK_PARAM_SIZE(parmaStruct, dataSize)                                \
  LOG_INFO("real struct size : 0x%x, data size 0x%x\n", sizeof(parmaStruct),   \
           dataSize);                                                          \
  if (sizeof(parmaStruct) != dataSize) {                                       \
    LOG_INFO("The parameter does not match and will not take effect\n");       \
    return -1;                                                                 \
  }

enum {
  ENUM_ID_AE_START = 0x1100,
  ENUM_ID_AE_SETEXPSWATTR,
  ENUM_ID_AE_GETEXPSWATTR,
  ENUM_ID_AE_SETLINAEDAYROUTEATTR,
  ENUM_ID_AE_GETLINAEDAYROUTEATTR,
  ENUM_ID_AE_SETLINAENIGHTROUTEATTR,
  ENUM_ID_AE_GETLINAENIGHTROUTEATTR,
  ENUM_ID_AE_SETHDRAEDAYROUTEATTR,
  ENUM_ID_AE_GETHDRAEDAYROUTEATTR,
  ENUM_ID_AE_SETHDRAENIGHTROUTEATTR,
  ENUM_ID_AE_GETHDRAENIGHTROUTEATTR,
  ENUM_ID_AE_QUERYEXPRESINFO,
  ENUM_ID_AE_SETLINEXPATTR,
  ENUM_ID_AE_GETLINEXPATTR,
  ENUM_ID_AE_SETHDREXPATTR,
  ENUM_ID_AE_GETHDREXPATTR,
  ENUM_ID_AE_END,
};

enum {
  ENUM_ID_IMGPROC_START = 0x1200,
  ENUM_ID_IMGPROC_SETEXPMODE,
  ENUM_ID_IMGPROC_GETEXPMODE,
  ENUM_ID_IMGPROC_SETAEMODE,
  ENUM_ID_IMGPROC_GETAEMODE,
  ENUM_ID_IMGPROC_SETEXPGAINRANGE,
  ENUM_ID_IMGPROC_GETEXPGAINRANGE,
  ENUM_ID_IMGPROC_SETEXPTIMERANGE,
  ENUM_ID_IMGPROC_GETEXPTIMERANGE,
  ENUM_ID_IMGPROC_SETBLCMODE,
  ENUM_ID_IMGPROC_SETHLCMODE,
  ENUM_ID_IMGPROC_SETLEXPMODE,
  ENUM_ID_IMGPROC_GETLEXPMODE,
  ENUM_ID_IMGPROC_SETMLEXP,
  ENUM_ID_IMGPROC_GETMLEXP,
  ENUM_ID_IMGPROC_SETANTIFLICKERMODE,
  ENUM_ID_IMGPROC_GETANTIFLICKERMODE,
  ENUM_ID_IMGPROC_SETEXPPWRLINEFREQMODE,
  ENUM_ID_IMGPROC_GETEXPPWRLINEFREQMODE,
  ENUM_ID_IMGPROC_SETDAYNSWMODE,
  ENUM_ID_IMGPROC_GETDAYNSWMODE,
  ENUM_ID_IMGPROC_SETMDNSCENE,
  ENUM_ID_IMGPROC_GETMDNSCENE,
  ENUM_ID_IMGPROC_SETADNSENS,
  ENUM_ID_IMGPROC_GETADNSENS,
  ENUM_ID_IMGPROC_SETFLIGHTMODE,
  ENUM_ID_IMGPROC_GETFLIGHTMODE,
  ENUM_ID_IMGPROC_SETMFLIGHT,
  ENUM_ID_IMGPROC_GETMFLIGHT,
  ENUM_ID_IMGPROC_SETWBMODE,
  ENUM_ID_IMGPROC_GETWBMODE,
  ENUM_ID_IMGPROC_LOCKAWB,
  ENUM_ID_IMGPROC_UNLOCKAWB,
  ENUM_ID_IMGPROC_SETAWBRANGE,
  ENUM_ID_IMGPROC_GETAWBRANGE,
  ENUM_ID_IMGPROC_SETMWBSCENE,
  ENUM_ID_IMGPROC_GETMWBSCENE,
  ENUM_ID_IMGPROC_SETMWBGAIN,
  ENUM_ID_IMGPROC_GETMWBGAIN,
  ENUM_ID_IMGPROC_SETMWBCT,
  ENUM_ID_IMGPROC_GETMWBCT,
  ENUM_ID_IMGPROC_SETCRSUPPSN,
  ENUM_ID_IMGPROC_GETCRSUPPSN,
  ENUM_ID_IMGPROC_SETFOCUSMODE,
  ENUM_ID_IMGPROC_GETFOCUSMODE,
  ENUM_ID_IMGPROC_SETFOCUSWIN,
  ENUM_ID_IMGPROC_GETFOCUSWIN,
  ENUM_ID_IMGPROC_SETFIXEDMODECODE,
  ENUM_ID_IMGPROC_GETFIXEDMODECODE,
  ENUM_ID_IMGPROC_SETMINFOCUSDIS,
  ENUM_ID_IMGPROC_GETMINFOCUSDIS,
  ENUM_ID_IMGPROC_SETOPZOOMRANGE,
  ENUM_ID_IMGPROC_GETOPZOOMRANGE,
  ENUM_ID_IMGPROC_SETOPZOOMSPEED,
  ENUM_ID_IMGPROC_GETOPZOOMSPEED,
  ENUM_ID_IMGPROC_SETHDRMODE,
  ENUM_ID_IMGPROC_GETHDRMODE,
  ENUM_ID_IMGPROC_SETMHDRSTRTH,
  ENUM_ID_IMGPROC_GETMHDRSTRTH,
  ENUM_ID_IMGPROC_SETNRMODE,
  ENUM_ID_IMGPROC_GETNRMODE,
  ENUM_ID_IMGPROC_SETANRSTRTH,
  ENUM_ID_IMGPROC_GETANRSTRTH,
  ENUM_ID_IMGPROC_SETMSPANRSTRTH,
  ENUM_ID_IMGPROC_GETMSPANRSTRTH,
  ENUM_ID_IMGPROC_SETMTNRSTRTH,
  ENUM_ID_IMGPROC_GETMTNRSTRTH,
  ENUM_ID_IMGPROC_SETDHZMODE,
  ENUM_ID_IMGPROC_GETDHZMODE,
  ENUM_ID_IMGPROC_SETMDHZSTRTH,
  ENUM_ID_IMGPROC_GETMDHZSTRTH,
  ENUM_ID_IMGPROC_SETCONTRAST,
  ENUM_ID_IMGPROC_GETCONTRAST,
  ENUM_ID_IMGPROC_SETBRIGHTNESS,
  ENUM_ID_IMGPROC_GETBRIGHTNESS,
  ENUM_ID_IMGPROC_SETSATURATION,
  ENUM_ID_IMGPROC_GETSATURATION,
  ENUM_ID_IMGPROC_SETSHARPNESS,
  ENUM_ID_IMGPROC_GETSHARPNESS,
  ENUM_ID_IMGPROC_SETGAMMACOEF,
  ENUM_ID_IMGPROC_SETGRAYMODE,
  ENUM_ID_IMGPROC_GETGRAYMODE,
  ENUM_ID_IMGPROC_END,
};

enum {
  ENUM_ID_ANR_START = 0x1300,
  ENUM_ID_ANR_SETBAYERNRATTR,
  ENUM_ID_ANR_GETBAYERNRATTR,
  ENUM_ID_ANR_SETMFNRATTR,
  ENUM_ID_ANR_GETMFNRATTR,
  ENUM_ID_ANR_SETUVNRATTR,
  ENUM_ID_ANR_GETUVNRATTR,
  ENUM_ID_ANR_SETYNRATTR,
  ENUM_ID_ANR_GETYNRATTR,

  ENUM_ID_ANR_SETATTRIB,
  ENUM_ID_ANR_GETATTRIB,
  ENUM_ID_ANR_SETLUMASFSTRENGTH,
  ENUM_ID_ANR_SETLUMATFSTRENGTH,
  ENUM_ID_ANR_GETLUMASFSTRENGTH,
  ENUM_ID_ANR_GETLUMATFSTRENGTH,
  ENUM_ID_ANR_SETCHROMASFSTRENGTH,
  ENUM_ID_ANR_SETCHROMATFSTRENGTH,
  ENUM_ID_ANR_GETCHROMASFSTRENGTH,
  ENUM_ID_ANR_GETCHROMATFSTRENGTH,
  ENUM_ID_ANR_SETRAWNRSFSTRENGTH,
  ENUM_ID_ANR_GETRAWNRSFSTRENGTH,
  ENUM_ID_ANR_END,
};

enum {
  ENUM_ID_SHARP_START = 0X1400,
  ENUM_ID_SHARP_SET_ATTR,
  ENUM_ID_SHARP_GET_ATTR,
  ENUM_ID_SHARP_SET_IQPARA,
  ENUM_ID_SHARP_GET_IQPARA,
  ENUM_ID_SHARP_SET_EF_IQPARA,
  ENUM_ID_SHARP_GET_EF_IQPARA,
  ENUM_ID_SHARP_SET_STRENGTH,
  ENUM_ID_SHARP_GET_STRENGTH,
  ENUM_ID_SHARP_END,
};

enum {
  ENUM_ID_SYSCTL_START = 0X1500,
  ENUM_ID_SYSCTL_GETVERSIONINFO,
  ENUM_ID_SYSCTL_SETCPSLTCFG,
  ENUM_ID_SYSCTL_GETCPSLTINFO,
  ENUM_ID_SYSCTL_QUERYCPSLTCAP,
  ENUM_ID_SYSCTL_SETWORKINGMODE,
  ENUM_ID_SYSCTL_ENQUEUERKRAWFILE,
  ENUM_ID_SYSCTL_END,
};

enum {
  ENUM_ID_AMERGE_START = 0X1600,
  ENUM_ID_AMERGE_SETATTRIB,
  ENUM_ID_AMERGE_GETATTRIB,
  ENUM_ID_AMERGE_END,
};

enum {
  ENUM_ID_AGAMMA_START = 0X1700,
  ENUM_ID_AGAMMA_SETATTRIB,
  ENUM_ID_AGAMMA_GETATTRIB,
  ENUM_ID_AGAMMA_END,
};

enum {
  ENUM_ID_ADPCC_START = 0X1800,
  ENUM_ID_ADPCC_SETATTRIB,
  ENUM_ID_ADPCC_GETATTRIB,
  ENUM_ID_ADPCC_END,
};

enum {
  ENUM_ID_DEHAZE_START = 0X1900,
  ENUM_ID_DEHAZE_SETATTRIB,
  ENUM_ID_DEHAZE_GETATTRIB,
  ENUM_ID_DEHAZE_END,
};

enum {
  ENUM_ID_ACCM_START = 0X1B00,
  ENUM_ID_ACCM_SETATTRIB,
  ENUM_ID_ACCM_GETATTRIB,
  ENUM_ID_ACCM_QUERYCCMINFO,
  ENUM_ID_ACCM_END,
};

enum {
  ENUM_ID_AWB_START = 0X1C00,
  ENUM_ID_AWB_SETATTRIB,
  ENUM_ID_AWB_GETATTRIB,
  ENUM_ID_AWB_QUERYWBINFO,
  ENUM_ID_AWB_END,
};

enum {
  ENUM_ID_CPROC_START = 0X1D00,
  ENUM_ID_CPROC_SETATTRIB,
  ENUM_ID_CPROC_GETATTRIB,
  ENUM_ID_CPROC_END,
};

enum {
  ENUM_ID_ATMO_START = 0X1E00,
  ENUM_ID_ATMO_SETATTRIB,
  ENUM_ID_ATMO_GETATTRIB,
  ENUM_ID_ATMO_END,
};

enum
{
  ENUM_ID_AIQ_UAPI_START = 0X1F00,
  ENUM_ID_AIQ_UAPI_SYSCTL_GET3ASTATS,
  ENUM_ID_AIQ_UAPI_SYSCTL_GET3ASTATSBLK,
  ENUM_ID_AIQ_UAPI2_AWB_WRITEAWBIN,
  ENUM_ID_AIQ_UAPI_END,
};

#endif
