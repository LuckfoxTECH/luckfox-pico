/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "TXSDKCommonDef.h"
#include "TXDeviceSDK.h"
static FILE *fstream = NULL;
static char* s_cData = NULL;
static bool s_bstart = false;
static pthread_t thread_enc_id = 0;
static unsigned long s_dwTotalFrameIndex = 0;
unsigned long _GetTickCount() {
    struct timeval current = {0};
 gettimeofday(&current, NULL);
 return (current.tv_sec*1000 + current.tv_usec/1000);
}
static void* EncoderThread(void* pThreadData) {
 while(s_bstart) {
  if(s_bstart) {
   if(fstream &&!feof(fstream)) {
    int iLen = 0;
    int iType = -1;
    fread(&iLen, 1, 4, fstream);
    fread(&iType, 1, 4, fstream);
    if (iLen > 0) {
     fread(s_cData, 1, iLen, fstream);
     static int s_nFrameIndex = 0;
     static int s_gopIndex = -1;
     unsigned char * pcEncData = (unsigned char *)s_cData;
     int nEncDataLen = iLen;
     int nFrameType = 1;
     if(iType == 0) {
      s_nFrameIndex = 0;
      s_gopIndex++;
      nFrameType = 0;
     }
     else {
      s_nFrameIndex++;
      nFrameType = 1;
     }
     if(s_gopIndex == -1) {
      printf("No I Frame s_gopIndex == -1\n");
     }
     else {
      if(nEncDataLen != 0) {
       tx_set_video_data(pcEncData, nEncDataLen, nFrameType, _GetTickCount(), s_gopIndex, s_nFrameIndex, s_dwTotalFrameIndex++, 40);
      }
     }
    }
   }
   else if(fstream && feof(fstream)) {
    fclose(fstream);
    fstream = fopen("test.264", "rb");
   }
  }
  usleep(90000);
 }
    return 0;
}
bool test_start_camera() {
 printf("###### test_start_camera ###################################### \n");
 if (!fstream) {
  fstream = fopen("test.264", "rb");
 }
 if (!s_cData) {
  s_cData = malloc(1280*720);
 }
 if (!fstream || !s_cData) {
  return false;
 }
 s_bstart = true;
 int ret = pthread_create(&thread_enc_id, NULL, EncoderThread, NULL);
 if (ret || !thread_enc_id) {
  s_bstart = false;
  return false;
 }
    return true;
}
bool test_stop_camera() {
 printf("###### test_stop_camera ###################################### \n");
 s_bstart = false;
 if(fstream) {
  fclose(fstream);
  fstream = NULL;
 }
 if(s_cData) {
  free(s_cData);
  s_cData = NULL;
 }
 if(thread_enc_id !=0) {
        pthread_join(thread_enc_id,NULL);
        thread_enc_id = 0;
    }
    return true;
}
bool test_set_bitrate(int bit_rate) {
    printf("###### test_set_bitrate  ##################################### %d \n", bit_rate);
 return true;
}
bool test_restart_gop() {
 printf("###### test_restart_gop ###################################### \n");
    return true;
}
bool test_start_mic() {
 printf("###### test_start_mic ###################################### \n");
 return true;
}
bool test_stop_mic() {
 printf("###### test_stop_mic ######################################\n");
 return true;
}
static bool g_start_av_service = false;
void on_login_complete(int errcode) {
    printf("on_login_complete | code[%d]\n", errcode);
}
void on_online_status(int old, int new) {
 printf("online status: %s\n", 11 == new ? "true" : "false");
    if(11 == new && !g_start_av_service) {
     tx_av_callback avcallback = {0};
  avcallback.on_start_camera = test_start_camera;
  avcallback.on_stop_camera = test_stop_camera;
  avcallback.on_set_bitrate = test_set_bitrate;
  avcallback.on_start_mic = test_start_mic;
  avcallback.on_stop_mic = test_stop_mic;
     int ret = tx_start_av_service(&avcallback);
     if (err_null == ret) {
      printf(" >>> tx_start_av_service successed\n");
     }
     else {
      printf(" >>> tx_start_av_service failed [%d]\n", ret);
     }
     g_start_av_service = true;
    }
}
bool readBufferFromFile(char *pPath, unsigned char *pBuffer, int nInSize, int *pSizeUsed) {
 if (!pPath || !pBuffer) {
  return false;
 }
 int uLen = 0;
 FILE * file = fopen(pPath, "rb");
 if (!file) {
     return false;
 }
 fseek(file, 0L, SEEK_END);
 uLen = ftell(file);
 fseek(file, 0L, SEEK_SET);
 if (0 == uLen || nInSize < uLen) {
  printf("invalide file or buffer size is too small...\n");
  return false;
 }
 *pSizeUsed = fread(pBuffer, 1, uLen, file);
 if (pBuffer[uLen-1] == 0x0a)
 {
  *pSizeUsed = uLen - 1;
  pBuffer[uLen - 1] = '\0';
 }
 printf("len:%d, ulen:%d\n",uLen, *pSizeUsed);
 fclose(file);
 return true;
}
void log_func(int level, const char* module, int line, const char* message)
{
 printf("%s\n", message);
}
bool initDevice() {
    unsigned char license[256] = {0};
    int nLicenseSize = 0;
    if (!readBufferFromFile("./licence.sign.file.txt", license, sizeof(license), &nLicenseSize)) {
        printf("[error]get license from file failed...\n");
        return false;
    }
    unsigned char guid[32] = {0};
    int nGUIDSize = 0;
    if(!readBufferFromFile("./GUID_file.txt", guid, sizeof(guid), &nGUIDSize)) {
        printf("[error]get guid from file failed...\n");
        return false;
    }
    char svrPubkey[256] = {0};
    int nPubkeySize = 0;
    if (!readBufferFromFile("./1000000004.pem", svrPubkey, sizeof(svrPubkey), &nPubkeySize))
    {
        printf("[error]get svrPubkey from file failed...\n");
        return NULL;
    }
    tx_device_info info = {0};
    info.os_platform = "Linux";
    info.device_name = "demo1";
    info.device_serial_number = guid;
    info.device_license = license;
    info.product_version = 1;
    info.network_type = network_type_wifi;
    info.product_id = 1000000004;
    info.server_pub_key = svrPubkey;
    tx_device_notify notify = {0};
    notify.on_login_complete = on_login_complete;
    notify.on_online_status = on_online_status;
    notify.on_binder_list_change = NULL;
    tx_init_path init_path = {0};
    init_path.system_path = "./";
    init_path.system_path_capicity = 100 * 1024;
    init_path.app_path = "./";
    init_path.app_path_capicity = 1024 * 1024;
    init_path.temp_path = "./";
    init_path.temp_path_capicity = 10 * 1024;
    tx_set_log_func(log_func);
 int ret = tx_init_device(&info, &notify, &init_path);
 if (err_null == ret) {
  printf(" >>> tx_init_device success\n");
 }
 else {
  printf(" >>> tx_init_device failed [%d]\n", ret);
  return false;
 }
 return true;
}
int main(int argc, char* argv[]) {
 if ( !initDevice() ) {
  return -1;
 }
 char input[100];
 while (scanf("%s", input)) {
  if ( !strcmp(input, "quit") ) {
   if (g_start_av_service) {
    tx_stop_av_service();
   }
   tx_exit_device();
   break;
  }
  sleep(1);
 }
 return 0;
}
