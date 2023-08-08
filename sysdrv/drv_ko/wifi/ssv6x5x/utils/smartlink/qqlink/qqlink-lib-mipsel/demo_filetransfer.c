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
void on_login_complete(int errcode) {
    printf("on_login_complete | code[%d]\n", errcode);
}
void on_online_status(int old, int new) {
 printf("online status: %s\n", 11 == new ? "true" : "false");
}
void ontransfercomplete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info tran_info) {
    printf("================ontransfer complete=====transfer_cookie == %lld ====================\n", transfer_cookie);
    printf("errcode %d, bussiness_name [%s], file path [%s]\n", err_code, tran_info.bussiness_name, tran_info.file_path);
    printf("===============================================================================\n");
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
        pBuffer[uLen -1] = '\0';
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
    tx_file_transfer_notify fileTransferNotify = {0};
    fileTransferNotify.on_transfer_complete = ontransfercomplete;
    tx_init_file_transfer(fileTransferNotify, "./recv/");
 return true;
}
int main(int argc, char* argv[]) {
 if ( !initDevice() ) {
  return -1;
 }
 char input[100];
 while (scanf("%s", input)) {
  if ( !strcmp(input, "quit") ) {
   tx_exit_device();
   break;
  }
  sleep(1);
 }
 return 0;
}
