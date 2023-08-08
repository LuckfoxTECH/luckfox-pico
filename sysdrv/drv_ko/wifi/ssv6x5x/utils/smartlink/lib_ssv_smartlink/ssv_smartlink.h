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

#ifndef _SSV_SMARTLINK_H
#define _SSV_SMARTLINK_H 
#define MAX_PAYLOAD (4096)
enum
{
    SSV_ERR_BASE = -255,
    SSV_ERR_PARAMETER,
    SSV_ERR_NETLINK_SOCKET,
    SSV_ERR_NETLINK_BIND,
    SSV_ERR_NETLINK_SENDMSG,
    SSV_ERR_NETLINK_RECVMSG,
    SSV_ERR_NETLINK_CORRUPTED,
    SSV_ERR_START_SMARTLINK = -248,
    SSV_ERR_STOP_SMARTLINK,
    SSV_ERR_SET_CHANNEL,
    SSV_ERR_GET_CHANNEL,
    SSV_ERR_SET_PROMISC,
    SSV_ERR_GET_PROMISC,
    SSV_ERR_SET_SI_CMD,
    SSV_ERR_GET_SI_STATUS,
    SSV_ERR_GET_SI_SSID,
    SSV_ERR_GET_SI_PASS,
};
enum ssv_smart_icomm_cmd
{
    STOP_SMART_ICOMM,
    START_SMART_ICOMM,
    RESET_SMART_ICOMM,
    MAX_SMART_ICOMM
};
#define SSV_MIN_CHANNEL (1)
#define SSV_MAX_CHANNEL (14)
#define SSV_PROMISC_DISABLED (0)
#define SSV_PROMISC_ENABLED (1)
int ssv_smartlink_start(void);
int ssv_smartlink_set_channel(uint32_t ch);
int ssv_smartlink_get_channel(uint32_t *pch);
int ssv_smartlink_set_promisc(uint32_t promisc);
int ssv_smartlink_get_promisc(uint32_t *promisc);
int ssv_smartlink_recv_packet(uint8_t *pOutBuf, uint32_t *pOutBufLen);
int ssv_smartlink_stop(void);
char *ssv_smartlink_version(void);
int smaricomm_start(void);
int smarticomm_set_si_cmd(uint32_t command);
int smarticomm_get_si_status(uint8_t *status);
int smarticomm_get_si_ssid(uint8_t *ssid);
int smarticomm_get_si_pass(uint8_t *pass);
int smarticomm_stop(void);
#endif
