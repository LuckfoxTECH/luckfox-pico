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
#include "TXSDKCommonDef.h"
#include "TXDeviceSDK.h"
#include "TXOTA.h"
#include "TXFileTransfer.h"
#include "TXIPCAM.h"
extern bool test_start_camera();
extern bool test_stop_camera();
extern bool test_set_bitrate(int bit_rate);
extern void test_recv_audiodata(tx_audio_encode_param *param, unsigned char *pcEncData, int nEncDataLen);
extern bool test_start_mic();
extern bool test_stop_mic();
extern void cb_on_transfer_progress(unsigned long long transfer_cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress);
extern void cb_on_recv_file(unsigned long long transfer_cookie, const tx_ccmsg_inst_info * inst_info, const tx_file_transfer_info * tran_info);
extern void cb_on_transfer_complete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info* tran_info);
extern void test_send_pic_alarm();
extern void test_send_audio_alarm();
extern int cb_on_new_pkg_come(unsigned long long from, unsigned long long pkg_size, const char * title, const char * desc, unsigned int target_version);
extern void cb_on_download_progress(unsigned long long download_size, unsigned long long total_size);
extern void cb_on_download_complete(int ret_code);
extern void cb_on_update_confirm();
extern int cb_on_set_definition(int definition, char *cur_definition, int cur_definition_length);
extern int cb_on_control_rotate(int rotate_direction, int rotate_degree);
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
        avcallback.on_recv_audiodata = test_recv_audiodata;
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
void on_binder_list_change(int error_code, tx_binder_info * pBinderList, int nCount)
{
    if (err_null != error_code)
    {
        printf("on_binder_list_change failed, errcode:%d\n", error_code);
        return;
    }
    printf("on_binder_list_change, %d binder: \n", nCount);
    int i = 0;
    for (i = 0; i < nCount; ++i )
    {
        printf("binder uin[%llu], nick_name[%s]\n", pBinderList[i].uin, pBinderList[i].nick_name);
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
    unsigned char svrPubkey[256] = {0};
    int nPubkeySize = 0;
    if (!readBufferFromFile("./1700001460.pem", svrPubkey, sizeof(svrPubkey), &nPubkeySize))
    {
        printf("[error]get svrPubkey from file failed...\n");
        return NULL;
    }
    tx_device_info info = {0};
    info.os_platform = "Linux";
    info.device_name = "demo1";
    info.device_serial_number = (char*)guid;
    info.device_license = (char*)license;
    info.product_version = 1;
    info.network_type = network_type_wifi;
    info.product_id = 1700001460;
    info.server_pub_key = svrPubkey;
    tx_device_notify notify = {0};
    notify.on_login_complete = on_login_complete;
    notify.on_online_status = on_online_status;
    notify.on_binder_list_change = on_binder_list_change;
    tx_init_path init_path = {0};
    init_path.system_path = "./";
    init_path.system_path_capicity = 100 * 1024;
    init_path.app_path = "./";
    init_path.app_path_capicity = 1024 * 1024;
    init_path.temp_path = "./";
    init_path.temp_path_capicity = 10 * 1024;
    tx_set_log_func(log_func);
 int ret = tx_init_device(&info, &notify, &init_path);
 if (err_null != ret) {
        printf(" >>> tx_init_device failed [%d]\n", ret);
        return false;
 }
    printf(" >>> tx_init_device success\n");
    tx_ipcamera_notify ipcamera_notify = {0};
    ipcamera_notify.on_control_rotate = cb_on_control_rotate;
    ipcamera_notify.on_set_definition = cb_on_set_definition;
    tx_ipcamera_set_callback(&ipcamera_notify);
    tx_file_transfer_notify fileTransferNotify = {0};
    fileTransferNotify.on_transfer_complete = cb_on_transfer_complete;
    fileTransferNotify.on_transfer_progress = cb_on_transfer_progress;
    fileTransferNotify.on_file_in_come = cb_on_recv_file;
    tx_init_file_transfer(fileTransferNotify, "/tmp/ramdisk/");
    tx_ota_notify ota_notify = {0};
    ota_notify.on_new_pkg_come = cb_on_new_pkg_come;
    ota_notify.on_download_progress = cb_on_download_progress;
    ota_notify.on_download_complete = cb_on_download_complete;
    ota_notify.on_update_confirm = cb_on_update_confirm;
    tx_init_ota(&ota_notify, 10*60, "/tmp/update_pkg.tar");
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
        else if ( !strcmp(input, "sendpicalarm") ) {
            test_send_pic_alarm();
        }
        else if ( !strcmp(input, "sendaudioalarm") ) {
            test_send_audio_alarm();
        }
  sleep(1);
 }
 return 0;
}
