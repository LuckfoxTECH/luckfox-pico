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
#include "TXFileTransfer.h"
void cb_on_transfer_progress(unsigned long long transfer_cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress)
{
    printf("========> on file progress %f%%\n", transfer_progress * 100.0 / max_transfer_progress);
}
void cb_on_recv_file(unsigned long long transfer_cookie, const tx_ccmsg_inst_info * inst_info, const tx_file_transfer_info * tran_info)
{
}
void cb_on_transfer_complete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info* tran_info)
{
    printf("================ontransfer complete=====transfer_cookie == %lld ====================\n", transfer_cookie);
    printf("errcode %d, bussiness_name [%s], file path [%s]\n", err_code, tran_info->bussiness_name, tran_info->file_path);
    printf("===============================================================================\n");
    if(err_code == 0)
    {
        if(strcmp(tran_info->bussiness_name, BUSINESS_NAME_AUDIO_MSG) == 0)
        {
        }
        else
        {
        }
    }
}
