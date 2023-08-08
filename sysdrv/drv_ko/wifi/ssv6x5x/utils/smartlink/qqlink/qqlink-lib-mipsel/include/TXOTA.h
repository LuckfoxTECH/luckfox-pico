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

#ifndef __OTA_H__
#define __OTA_H__ 
#include "TXSDKCommonDef.h"
CXX_EXTERN_BEGIN
typedef struct _tx_ota_notify
{
 int (*on_new_pkg_come)(unsigned long long from, unsigned long long pkg_size, const char * title, const char * desc, unsigned int target_version);
 void (*on_download_progress)(unsigned long long download_size, unsigned long long total_size);
 void (*on_download_complete)(int ret_code);
 void (*on_update_confirm)();
} tx_ota_notify;
SDK_API int tx_init_ota(tx_ota_notify * notify, int replace_timeout , char* target_pathname);
SDK_API void tx_ack_ota_result(int ret_code, char* err_msg);
CXX_EXTERN_END
#endif
