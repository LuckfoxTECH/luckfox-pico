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

#ifndef __TX_MSG_H__
#define __TX_MSG_H__ 
#include "TXSDKCommonDef.h"
CXX_EXTERN_BEGIN
typedef void (*on_send_text_msg_ret)(unsigned int cookie, int err_code);
SDK_API void tx_send_text_msg(int msg_id, char *text, on_send_text_msg_ret ret_callback, unsigned int *cookie, unsigned long long* to_targetids, unsigned int to_targetids_count);
typedef struct tag_structuring_msg
{
 int msg_id;
    char* file_path;
    char* thumb_path;
    char* title;
    char* digest;
    char* guide_words;
 unsigned int duration;
 unsigned long long* to_targetids;
 unsigned int to_targetids_count;
} structuring_msg;
typedef struct _tx_send_msg_notify
{
    void (*on_file_transfer_progress)(const unsigned int cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress);
    void (*on_send_structuring_msg_ret)(const unsigned int cookie, int err_code);
} tx_send_msg_notify;
SDK_API void tx_send_structuring_msg(const structuring_msg *msg, tx_send_msg_notify *notify, unsigned int *cookie);
typedef void (*on_send_notify_msg_ret)(unsigned int cookie, int err_code);
SDK_API void tx_send_notify_msg(int msg_id, char* digest, on_send_notify_msg_ret ret_callback, unsigned int *cookie, unsigned long long* to_targetids, unsigned int to_targetids_count);
CXX_EXTERN_END
#endif
