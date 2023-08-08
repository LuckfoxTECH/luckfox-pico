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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <linux/genetlink.h>
#include "ssv_smartlink.h"
#define SSV_SMARTLINK_VERSION "v1.0"
#define GLOBAL_NL_ID 999
enum {
    SSV_CTL_ATTR_UNSPEC,
    SSV_CTL_ATTR_ENABLE,
    SSV_CTL_ATTR_SUCCESS,
    SSV_CTL_ATTR_CHANNEL,
    SSV_CTL_ATTR_PROMISC,
    SSV_CTL_ATTR_RXFRAME,
    SSV_CTL_ATTR_SI_CMD,
    SSV_CTL_ATTR_SI_STATUS,
    SSV_CTL_ATTR_SI_SSID,
    SSV_CTL_ATTR_SI_PASS,
    SSV_CTL_ATTR_RAWDATA,
    __SSV_CTL_ATTR_MAX,
};
#define SSV_CTL_ATTR_MAX (__SSV_CTL_ATTR_MAX - 1)
enum {
    SSV_CTL_CMD_UNSPEC,
    SSV_CTL_CMD_SMARTLINK,
    SSV_CTL_CMD_SET_CHANNEL,
    SSV_CTL_CMD_GET_CHANNEL,
    SSV_CTL_CMD_SET_PROMISC,
    SSV_CTL_CMD_GET_PROMISC,
    SSV_CTL_CMD_RX_FRAME,
    SSV_CTL_CMD_SMARTICOMM,
    SSV_CTL_CMD_SET_SI_CMD,
    SSV_CTL_CMD_GET_SI_STATUS,
    SSV_CTL_CMD_GET_SI_SSID,
    SSV_CTL_CMD_GET_SI_PASS,
    SSV_CTL_CMD_SEND_RAWDATA,
    __SSV_CTL_CMD_MAX,
};
#define SSV_CTL_CMD_MAX (__SSV_CTL_CMD_MAX - 1)
struct netlink_msg {
    struct nlmsghdr n;
    struct genlmsghdr g;
    char buf[MAX_PAYLOAD];
};
static int gnl_fd=-1;
static int gnl_id;
//static int gnl_id=29;

/* Generic macros for dealing with netlink sockets. */
#define GENLMSG_DATA(glh) ((void *)(NLMSG_DATA(glh) + GENL_HDRLEN))
#define GENLMSG_PAYLOAD(glh) (NLMSG_PAYLOAD(glh, 0) - GENL_HDRLEN)
#define NLA_DATA(na) ((void *)((char*)(na) + NLA_HDRLEN))

//#define SSV_SMARTLINK_DEBUG 


int genl_send_msg(int sock_fd, u_int16_t family_id, u_int32_t nlmsg_pid,
            u_int8_t genl_cmd, u_int8_t genl_version, u_int16_t nla_type,
            void *nla_data, int nla_len)
{
    struct nlattr *na;
    struct sockaddr_nl dst_addr;
    int r, buflen;
    char *buf;
    struct netlink_msg msg;
    
    if (family_id == 0) {
            return 0;
        }
    
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = family_id;

    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = nlmsg_pid;

    msg.g.cmd = genl_cmd;
    msg.g.version = genl_version;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = nla_type;
    na->nla_len = nla_len + 1 + NLA_HDRLEN;
    memcpy(NLA_DATA(na), nla_data, nla_len);
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    buf = (char *) &msg;
    buflen = msg.n.nlmsg_len ;

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.nl_family = AF_NETLINK;
    dst_addr.nl_pid = 0; 
    dst_addr.nl_groups = 0; 
    while ((r = sendto(sock_fd, buf, buflen, 0, (struct sockaddr *) &dst_addr
            , sizeof(dst_addr))) < buflen) {
    if (r > 0) {
            buf += r;
            buflen -= r;
    } else if (errno != EAGAIN) {
            return -1;
        }
    }
    return 0;
}



static int genl_get_family_id(int sock_fd, char *family_name)
{
    struct netlink_msg ans;
    int id, rc;
    struct nlattr *na;
    int rep_len;
    
    rc = genl_send_msg(sock_fd, GENL_ID_CTRL, 0, CTRL_CMD_GETFAMILY, 1,
                      CTRL_ATTR_FAMILY_NAME, (void *)family_name,
                      strlen(family_name)+1);
    
    rep_len = recv(sock_fd, &ans, sizeof(ans), 0);
    if (rep_len < 0) {
        return 1;
    }
    if (ans.n.nlmsg_type == NLMSG_ERROR || !NLMSG_OK((&ans.n), rep_len))
    {
            return 1;
    }
    na = (struct nlattr *) GENLMSG_DATA(&ans);
    na = (struct nlattr *) ((char *) na + NLA_ALIGN(na->nla_len));
    if (na->nla_type == CTRL_ATTR_FAMILY_ID) {
        id = *(__u16 *) NLA_DATA(na);
    } else {
        id = 0;
    }
    return id;
}


static int _ssv_trigger_smartlink(unsigned int enable)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_SMARTLINK\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_SMARTLINK;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_ENABLE;
    mlength = sizeof(unsigned int);
    na->nla_len = mlength+NLA_HDRLEN;
    memcpy(NLA_DATA(na), &enable, mlength);
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    return retval;
}
static int _ssv_set_channel(unsigned int ch)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_SET_CHANNEL channel[%d]\n",ch);
#endif
    msg.g.cmd = SSV_CTL_CMD_SET_CHANNEL;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_CHANNEL;
    mlength = sizeof(unsigned int);
    na->nla_len = mlength+NLA_HDRLEN;
    memcpy(NLA_DATA(na), &ch, mlength);
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    return retval;
}
int ssv_smartlink_set_channel(unsigned int ch)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_START_SMARTLINK;
    }
    else
    {
        return _ssv_set_channel(ch);
    }
}
static int _ssv_get_channel(unsigned int *pch)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    int len;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_GET_CHANNEL\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_GET_CHANNEL;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_UNSPEC;
    mlength = 0;
    na->nla_len = mlength+NLA_HDRLEN;
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    len = recv(gnl_fd, &msg, sizeof(msg), 0);
    if (len > 0)
    {
        if (msg.n.nlmsg_type == NLMSG_ERROR)
        {
            printf("Error, receive NACK\n");
            return -1;
        }
        if (!NLMSG_OK((&msg.n), len))
        {
             printf("Invalid reply message received via Netlink\n");
             return -1;
        }
#ifdef SSV_SMARTLINK_DEBUG
        printf("Receive CMD GET CHANNEL\n");
#endif
        na = (struct nlattr *) GENLMSG_DATA(&msg);
        if (na->nla_type != SSV_CTL_ATTR_CHANNEL)
        {
            printf("%s Receive nla_type ERROR\n",__FUNCTION__);
            return -1;
        }
        *pch = *(unsigned int *)NLA_DATA(na);
    }
    return -1;
}
int ssv_smartlink_get_channel(unsigned int *pch)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_START_SMARTLINK;
    }
    else
    {
        return _ssv_get_channel(pch);
    }
}
static int _ssv_set_promisc(unsigned int promisc)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_SET_PROMISC\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_SET_PROMISC;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_PROMISC;
    mlength = sizeof(unsigned int);
    na->nla_len = mlength+NLA_HDRLEN;
    memcpy(NLA_DATA(na), &promisc, mlength);
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    return retval;
}
int ssv_smartlink_set_promisc(unsigned int promisc)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_START_SMARTLINK;
    }
    else
    {
        return _ssv_set_promisc(promisc);
    }
}
static int _ssv_get_promisc(int sock_fd, unsigned int *promisc)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    int len;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_GET_PROMISC\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_GET_PROMISC;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_UNSPEC;
    mlength = 0;
    na->nla_len = mlength+NLA_HDRLEN;
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    len = recv(gnl_fd, &msg, sizeof(msg), 0);
    if (len > 0)
    {
        if (msg.n.nlmsg_type == NLMSG_ERROR)
        {
            printf("Error, receive NACK\n");
            return -1;
        }
        if (!NLMSG_OK((&msg.n), len))
        {
             printf("Invalid reply message received via Netlink\n");
             return -1;
        }
#ifdef SSV_SMARTLINK_DEBUG
        printf("Receive CMD GET PROMISC\n");
#endif
        na = (struct nlattr *) GENLMSG_DATA(&msg);
        if (na->nla_type != SSV_CTL_ATTR_PROMISC)
        {
            printf("%s Receive nla_type ERROR\n",__FUNCTION__);
            return -1;
        }
        *promisc = *(unsigned int *)NLA_DATA(na);
    }
    return -1;
}
int ssv_smartlink_get_promisc(unsigned int *paccept)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_START_SMARTLINK;
    }
    else
    {
        return _ssv_get_promisc(gnl_fd, paccept);
    }
}
static int _ssv_set_si_cmd(int sock_fd, unsigned int command)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_SET_SI_CMD\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_SET_SI_CMD;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_SI_CMD;
    mlength = sizeof(unsigned int);
    na->nla_len = mlength+NLA_HDRLEN;
    memcpy(NLA_DATA(na), &command, mlength);
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
     return retval;
}
int smarticomm_set_si_cmd(unsigned int command)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_SET_SI_CMD;
    }
    else
    {
        return _ssv_set_si_cmd(gnl_fd, command);
    }
}
static int _ssv_get_si_status(uint8_t *status)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    int len;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_GET_SI_STATUS\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_GET_SI_STATUS;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_UNSPEC;
    mlength = 0;
    na->nla_len = mlength+NLA_HDRLEN;
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    memset((void *)&msg,0x00,sizeof(struct netlink_msg));
    len = recv(gnl_fd, &msg, sizeof(msg), 0);
    if (len > 0)
    {
        if (msg.n.nlmsg_type == NLMSG_ERROR)
        {
            printf("Error, receive NACK\n");
            return -1;
        }
        if (!NLMSG_OK((&msg.n), len))
        {
             printf("Invalid reply message received via Netlink\n");
             return -1;
        }
#ifdef SSV_SMARTLINK_DEBUG
        printf("Receive CMD SET SI_STATUS\n");
#endif
        na = (struct nlattr *) GENLMSG_DATA(&msg);
        if (na->nla_type != SSV_CTL_ATTR_SI_STATUS)
        {
            printf("%s Receive nla_type ERROR\n",__FUNCTION__);
            return -1;
        }
        strcpy(status,(char *)NLA_DATA(na));
    }
    return -1;
}
int smarticomm_get_si_status(uint8_t *status)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_GET_SI_STATUS;
    }
    else
    {
        return _ssv_get_si_status(status);
    }
}
static int _ssv_get_si_ssid(uint8_t *ssid)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    int len;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_GET_SI_SSID\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_GET_SI_SSID;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_UNSPEC;
    mlength = 0;
    na->nla_len = mlength+NLA_HDRLEN;
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    len = recv(gnl_fd, &msg, sizeof(msg), 0);
    if (len > 0)
    {
        if (msg.n.nlmsg_type == NLMSG_ERROR)
        {
            printf("Error, receive NACK\n");
            return -1;
        }
        if (!NLMSG_OK((&msg.n), len))
        {
             printf("Invalid reply message received via Netlink\n");
             return -1;
        }
#ifdef SSV_SMARTLINK_DEBUG
        printf("Receive CMD SET SI_SSID\n");
#endif
        na = (struct nlattr *) GENLMSG_DATA(&msg);
        if (na->nla_type != SSV_CTL_ATTR_SI_SSID)
        {
            printf("%s Receive nla_type ERROR\n",__FUNCTION__);
            return -1;
        }
        strcpy(ssid,(char *)NLA_DATA(na));
    }
    return -1;
}
int smarticomm_get_si_ssid(uint8_t *ssid)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_GET_SI_SSID;
    }
    else
    {
        return _ssv_get_si_ssid(ssid);
    }
}
static int _ssv_get_si_pass(uint8_t *pass)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    int len;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_SI_PASS\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_GET_SI_PASS;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_UNSPEC;
    mlength = 0;
    na->nla_len = mlength+NLA_HDRLEN;
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
    {
        printf("Fail to send message to kernel\n");
        return retval;
    }
    len = recv(gnl_fd, &msg, sizeof(msg), 0);
    if (len > 0)
    {
        if (msg.n.nlmsg_type == NLMSG_ERROR)
        {
            printf("Error, receive NACK\n");
            return -1;
        }
        if (!NLMSG_OK((&msg.n), len))
        {
             printf("Invalid reply message received via Netlink\n");
             return -1;
        }
#ifdef SSV_SMARTLINK_DEBUG
        printf("Receive CMD SET SI_PASS\n");
#endif
        na = (struct nlattr *) GENLMSG_DATA(&msg);
        if (na->nla_type != SSV_CTL_ATTR_SI_PASS)
        {
            printf("%s Receive nla_type ERROR\n",__FUNCTION__);
            return -1;
        }
        strcpy(pass,(char *)NLA_DATA(na));
    }
    return -1;
}
int smarticomm_get_si_pass(uint8_t *pass)
{
    if (gnl_fd < 0)
    {
        return SSV_ERR_GET_SI_PASS;
    }
    else
    {
        return _ssv_get_si_pass(pass);
    }
}
static int _ssv_netlink_init(void)
{
    int fd;
    struct sockaddr_nl local;
    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (fd < 0)
    {
        printf("fail to create netlink socket\n");
        return -1;
    }
    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_groups = 0;
    if (bind(fd, (struct sockaddr *) &local, sizeof(local)) < 0)
        goto error;

    gnl_id = genl_get_family_id(fd ,"SSVCTL");
    //printf("family_id:%d\n",gnl_id);
    
    return fd;
error:
    close(fd);
    return -1;
}
static int _ssv_netlink_close(int *psock_fd)
{
    if (psock_fd)
    {
        if (*psock_fd > 0)
        {
            close(*psock_fd);
        }
        *psock_fd = -1;
    }
    return 0;
}
int ssv_smartlink_start(void)
{
    int ret=-1;
    unsigned int ch=0;
    unsigned int accept=0;
    gnl_fd = _ssv_netlink_init();
    if (gnl_fd < 0)
    {
        ret = gnl_fd;
        goto out;
    }
    ret = _ssv_trigger_smartlink(1);
    if (ret < 0)
    {
        goto out;
    }
    ret = 0;
out:
    return ret;
}
int ssv_smartlink_stop(void)
{
    (void)_ssv_trigger_smartlink(0);
    (void)_ssv_netlink_close(&gnl_fd);
    return 0;
}
void hexdump(unsigned char *buf, int len)
{
    int i;
    printf("\n-----------------------------\n");
    printf("hexdump(len=%d):\n", len);
    for (i = 0; i < len; i++)
    {
        printf(" %02x", buf[i]);
        if ((i+1)%40 == 0)
            printf("\n");
    }
    printf("\n-----------------------------\n");
}
int ssv_smartlink_recv_packet(uint8_t *pOutBuf, unsigned int *pOutBufLen)
{
    int len;
    struct netlink_msg msg;
    struct nlattr *na;
    struct ssv_wireless_register *reg;
    len = recv(gnl_fd, &msg, sizeof(msg), 0);
    if (len > 0)
    {
        if (msg.n.nlmsg_type == NLMSG_ERROR)
        {
            printf("Error, receive NACK\n");
            return -1;
        }
        if (!NLMSG_OK((&msg.n), len))
        {
             printf("Invalid reply message received via Netlink\n");
             return -1;
        }
        na = (struct nlattr *) GENLMSG_DATA(&msg);
        if (na->nla_type != SSV_CTL_ATTR_RXFRAME)
        {
            printf("%s Receive nla_type ERROR\n",__FUNCTION__);
            return -1;
        }
        *pOutBufLen = na->nla_len - NLA_HDRLEN;
        memcpy(pOutBuf,(unsigned char *)NLA_DATA(na),*pOutBufLen);
#ifdef SSV_SMARTLINK_DEBUG
        printf("Receive RX FRAME pOutBufLen[%d] nla_len[%d]\n",*pOutBufLen,na->nla_len);
#endif
        return 1;
    }
    return -1;
}
static int _ssv_trigger_smarticomm(unsigned int enable)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int mlength, retval;
    struct sockaddr_nl nladdr;
    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
#ifdef SSV_SMARTLINK_DEBUG
    printf("SSV_CTL_CMD_SMARTICOMM\n");
#endif
    msg.g.cmd = SSV_CTL_CMD_SMARTICOMM;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_ENABLE;
    mlength = (sizeof(unsigned int));
    na->nla_len = mlength+NLA_HDRLEN;
    memcpy(NLA_DATA(na), &enable, mlength);
    msg.n.nlmsg_len += NLMSG_ALIGN(na->nla_len);
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    retval = sendto(gnl_fd, (char *)&msg, msg.n.nlmsg_len, 0,
                    (struct sockaddr *) &nladdr, sizeof(nladdr));
    if (retval < 0)
        printf("Fail to send message to kernel\n");
}
int smaricomm_start(void)
{
    int ret=-1;
    unsigned int ch=0;
    unsigned int accept=0;
    gnl_fd = _ssv_netlink_init();
    if (gnl_fd < 0)
    {
        ret = gnl_fd;
        goto out;
    }
    ret = _ssv_trigger_smarticomm(START_SMART_ICOMM);
    if (ret < 0)
    {
        goto out;
    }
    ret = 0;
out:
    return ret;
}
int smarticomm_stop(void)
{
    (void)_ssv_trigger_smarticomm(STOP_SMART_ICOMM);
    (void)_ssv_netlink_close(&gnl_fd);
    return 0;
}
static char version[64]="SSV SmartLink Verison: " SSV_SMARTLINK_VERSION;
char *ssv_smartlink_version(void)
{
    return version;
}
