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

#ifndef __TX_SDK_COMMON_DEF_H__
#define __TX_SDK_COMMON_DEF_H__ 
#define SDK_API __attribute__((visibility("default")))
#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#define CXX_EXTERN_BEGIN 
#define CXX_EXTERN_END 
#define C_EXTERN extern
#else
#define _Bool bool
#define CXX_EXTERN_BEGIN extern "C" {
#define CXX_EXTERN_END }
#define C_EXTERN 
#endif
CXX_EXTERN_BEGIN
enum error_code
{
    err_null = 0x00000000,
    err_failed = 0x00000001,
    err_unknown = 0x00000002,
    err_invalid_param = 0x00000003,
    err_buffer_notenough = 0x00000004,
    err_mem_alloc = 0x00000005,
    err_internal = 0x00000006,
    err_device_inited = 0x00000007,
    err_av_service_started = 0x00000008,
    err_invalid_device_info = 0x00000009,
    err_invalid_serial_number = 0x0000000A,
    err_invalid_fs_handler = 0x0000000B,
    err_invalid_device_notify = 0x0000000C,
    err_invalid_av_callback = 0x0000000D,
    err_invalid_system_path = 0x0000000E,
    err_invalid_app_path = 0x0000000F,
    err_invalid_temp_path = 0x00000010,
    err_not_impl = 0x00000011,
    err_fetching = 0x00000012,
    err_fetching_buff_not_enough = 0x00000013,
    err_off_line = 0x00000014,
    err_invalid_device_name = 0x00000015,
    err_invalid_os_platform = 0x00000016,
    err_invalid_license = 0x00000017,
    err_invalid_server_pub_key = 0x00000018,
    err_invalid_product_version = 0x00000019,
    err_invalid_product_id = 0x0000001A,
    err_connect_failed = 0x0000001B,
    err_call_too_frequently = 0x0000001C,
    err_sys_path_access_permission = 0x0000001D,
    err_invalid_network_type = 0x0000001E,
    err_login_failed = 0x00010001,
    err_login_invalid_deviceinfo = 0x00010002,
    err_login_connect_failed = 0x00010003,
    err_login_timeout = 0x00010004,
    err_login_eraseinfo = 0x00010005,
    err_login_servererror = 0x00010006,
    err_msg_sendfailed = 0x00020001,
    err_msg_sendtimeout = 0x00020002,
    err_av_unlogin = 0x00030001,
    err_ft_transfer_failed = 0x00040001,
    err_ft_file_too_large = 0x00040002,
};
CXX_EXTERN_END
#endif
