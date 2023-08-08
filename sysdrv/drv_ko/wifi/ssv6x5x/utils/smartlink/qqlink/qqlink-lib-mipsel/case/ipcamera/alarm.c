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
#include "TXMsg.h"
void on_send_alarm_file_progress(const unsigned int cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress)
{
    printf("on_send_alarm_file_progress, cookie[%u]\n", cookie);
}
void on_send_alarm_msg_ret(const unsigned int cookie, int err_code)
{
    printf("on_send_alarm_msg_ret, cookie[%u] ret[%d]\n", cookie, err_code);
}
void test_send_pic_alarm()
{
    structuring_msg msg = {0};
    msg.msg_id = 1;
    msg.file_path = "./alarm.png";
    msg.thumb_path = "./thumb.png";
    msg.title = "发现异常";
    msg.digest = "客厅发现异常";
    msg.guide_words = "点击查看";
    tx_send_msg_notify notify = {0};
    notify.on_file_transfer_progress = on_send_alarm_file_progress;
    notify.on_send_structuring_msg_ret = on_send_alarm_msg_ret;
    tx_send_structuring_msg(&msg, &notify, 0);
}
void test_send_audio_alarm()
{
    structuring_msg msg = {0};
    msg.msg_id = 2;
    msg.file_path = "./test.mp3";
    msg.title = "语音报警";
    msg.digest = "收到语音报警";
    msg.guide_words = "点击查看";
    tx_send_msg_notify notify = {0};
    notify.on_file_transfer_progress = on_send_alarm_file_progress;
    notify.on_send_structuring_msg_ret = on_send_alarm_msg_ret;
    tx_send_structuring_msg(&msg, &notify, 0);
}
