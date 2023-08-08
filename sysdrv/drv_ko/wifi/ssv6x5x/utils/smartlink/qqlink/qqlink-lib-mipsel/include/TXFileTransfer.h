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

#ifndef __TX_FILE_TRANSFER_H__
#define __TX_FILE_TRANSFER_H__ 
#include "TXSDKCommonDef.h"
#include "TXOldInf.h"
CXX_EXTERN_BEGIN
#define BUSINESS_NAME_IMAGE_MSG "ImgMsg"
#define BUSINESS_NAME_AUDIO_MSG "AudioMsg"
#define BUSINESS_NAME_VIDEO_MSG "VideoMsg"
#define BUSINESS_NAME_NAS_DEVPUSHFILE "7000-NASDevPushFile"
#define BUSINESS_NAME_NAS_DEVPUSHTHUMB "7001-NASDevPushThumb"
enum tx_file_transfer_type
{
    transfet_type_none = 0,
    transfer_type_upload = 1,
    transfer_type_download = 2,
    transfer_type_c2c_in = 3,
    transfer_type_c2c_out = 4,
};
enum tx_file_transfer_filetype
{
    transfer_filetype_image = 1,
    transfer_filetype_video = 2,
    transfer_filetype_audio = 3,
    transfer_filetype_other = 4,
};
enum tx_file_transfer_channeltype
{
    transfer_channeltype_FTN = 1,
    transfer_channeltype_MINI = 2,
};
typedef struct tag_tx_file_transfer_info
{
    char file_path[1024];
    char file_key[512];
    int key_length;
    char * buffer_raw;
    unsigned long long buffer_raw_len;
    char buffer_key[512];
    int buffer_key_len;
    char * buff_with_file;
    int buff_length;
    char bussiness_name[64];
    unsigned long long file_size;
    int channel_type;
    int file_type;
    int transfer_type;
}tx_file_transfer_info;
typedef struct tag_tx_file_transfer_notify
{
    void (*on_transfer_progress)(unsigned long long transfer_cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress);
    void (*on_transfer_complete)(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info* tran_info);
    void (*on_file_in_come)(unsigned long long transfer_cookie, const tx_ccmsg_inst_info * inst_info, const tx_file_transfer_info * tran_info);
}tx_file_transfer_notify;
SDK_API int tx_init_file_transfer(tx_file_transfer_notify notify, char * path_recv_file);
SDK_API int tx_upload_file(int channeltype, int filetype, char * file_path, unsigned long long * transfer_cookie);
SDK_API int tx_download_file(int channeltype, int filetype, char * file_key, int key_length, unsigned long long * transfer_cookie);
SDK_API int tx_send_file_to(unsigned long long target_id, char * file_path, unsigned long long * transfer_cookie, char * buff_with_file, int buff_length, char * bussiness_name);
SDK_API int tx_cancel_transfer(unsigned long long transfer_cookie);
SDK_API int tx_reg_file_transfer_filter(char * bussiness_name, tx_file_transfer_notify notify);
SDK_API int tx_get_minidownload_url(char* fileId, int fileType, char* downloadUrl);
CXX_EXTERN_END
#endif
