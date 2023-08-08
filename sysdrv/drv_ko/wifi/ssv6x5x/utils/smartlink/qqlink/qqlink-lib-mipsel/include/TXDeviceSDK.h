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

#ifndef __TX_DEVICE_SDK_H__
#define __TX_DEVICE_SDK_H__ 
#include "TXAudioVideo.h"
#include "TXMsg.h"
#include "TXDataPoint.h"
#include "TXFileTransfer.h"
CXX_EXTERN_BEGIN
enum tx_test_mode_enum
{
    test_mode_default = 0,
    test_mode_test_env = 1,
};
enum tx_network_type
{
 network_type_none = 0,
 network_type_wifi = 1,
 network_type_mobile = 2,
 network_type_unicom = 3,
 network_type_telecom = 4,
 network_type_hongkong = 5,
};
typedef struct _tx_device_info
{
    char * os_platform;
    int network_type;
    char * device_name;
    char * device_serial_number;
    char * device_license;
    int product_version;
    int product_id;
    char * server_pub_key;
    unsigned int test_mode;
} tx_device_info;
typedef struct tag_tx_binder_info
{
    int type;
    unsigned long long tinyid;
    unsigned long long uin;
    char nick_name[128];
    int gender;
    char head_url[1024];
}tx_binder_info;
typedef struct _tx_device_notify
{
    void (*on_login_complete)(int error_code);
    void (*on_online_status)(int old_status, int new_status);
    void (*on_binder_list_change)(int error_code, tx_binder_info * pBinderList, int nCount);
} tx_device_notify;
typedef struct _tx_init_path
{
    char * system_path;
    unsigned int system_path_capicity;
    char * app_path;
    unsigned int app_path_capicity;
    char * temp_path;
    unsigned int temp_path_capicity;
} tx_init_path;
enum tx_binder_type
{
 binder_type_unknown = 0,
 binder_type_owner = 1,
 binder_type_sharer = 2,
};
enum tx_binder_gender
{
 binder_gender_unknown = -1,
 binder_gender_male = 0,
 binder_gender_female = 1,
};
SDK_API int tx_init_device(tx_device_info *info, tx_device_notify *notify, tx_init_path* init_path);
SDK_API int tx_ack_app(unsigned int ip, unsigned int port);
SDK_API int tx_exit_device();
SDK_API int tx_get_sdk_version(unsigned int *main_ver, unsigned int *sub_ver, unsigned int *build_no);
typedef void (*tx_log_func)(int level, const char* module, int line, const char* message);
SDK_API void tx_set_log_func(tx_log_func log_func);
typedef void (*on_get_binder_list_result)(tx_binder_info * pBinderList, int nCount);
SDK_API int tx_get_binder_list(tx_binder_info * pBinderList, int* nCount, on_get_binder_list_result callback);
SDK_API unsigned long long tx_get_self_din();
SDK_API int tx_device_relogin();
typedef void (*on_erase_all_binders)(int error_code);
SDK_API int tx_erase_all_binders(on_erase_all_binders callback);
SDK_API int tx_get_server_time();
CXX_EXTERN_END
#endif
