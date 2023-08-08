/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/errno.h>

#include "icomm_rawdata.h"
#include "icomm_rawdata_packet.h"

int ssv_genl_send_msg(int sock_fd, u_int16_t family_id, u_int32_t nlmsg_pid,
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

static int ssv_genl_get_family_id(int sock_fd, char *family_name)
{
    struct netlink_msg ans;
    int id, rc;
    struct nlattr *na;
    int rep_len;
    
    rc = ssv_genl_send_msg(sock_fd, GENL_ID_CTRL, 0, CTRL_CMD_GETFAMILY, 1,
                      CTRL_ATTR_FAMILY_NAME, (void *)family_name,
                      strlen(family_name)+1);
    if (rc) {
        printf("Fail to ssv_genl_send_msg, rc=%d\n", rc);
        return 1;
    }
    
    rep_len = recv(sock_fd, &ans, sizeof(ans), 0);
    if (rep_len < 0) {
        return 1;
    }

    if (ans.n.nlmsg_type == NLMSG_ERROR || !NLMSG_OK((&ans.n), rep_len)) {
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

static int ssv_netlink_init(int *gnl_fd, int *gnl_id)
{
    int fd;
    struct sockaddr_nl local;
    
    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (fd < 0) {
        printf("fail to create netlink socket\n");
        return -1;
    }
    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_groups = 0;
    if (bind(fd, (struct sockaddr *) &local, sizeof(local)) < 0)
        goto error;

    *gnl_id = ssv_genl_get_family_id(fd ,"SSVCTL");
    *gnl_fd = fd; 
    return 0;

error:
    close(fd);
    return -1;
}

static int ssv_netlink_close(int *psock_fd)
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

static int ssv_send_rawdata(int gnl_fd, int gnl_id, unsigned char *data, int len)
{
    struct netlink_msg msg;
    struct nlattr *na;
    int retval;
    struct sockaddr_nl nladdr;

    msg.n.nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    msg.n.nlmsg_type = gnl_id;;
    msg.n.nlmsg_flags = NLM_F_REQUEST;
    msg.n.nlmsg_seq = 0;
    msg.n.nlmsg_pid = getpid();
    
    // set rawdata context
    msg.g.cmd = SSV_CTL_CMD_SEND_RAWDATA;
    na = (struct nlattr *) GENLMSG_DATA(&msg);
    na->nla_type = SSV_CTL_ATTR_RAWDATA;
    na->nla_len = len+NLA_HDRLEN;
	memcpy((unsigned char *)(((unsigned char *)na) + NLA_HDRLEN), data, len);
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

static void cmd_send_rawdata_frame(void *sc, int argc, char *argv[])
{
    int gnl_fd = 0, gnl_id = 0;
    int retval = 0;
    
    retval = ssv_netlink_init(&gnl_fd, &gnl_id);
    if (0 == retval) {
        ssv_send_rawdata(gnl_fd, gnl_id, raw_data_packet, sizeof(raw_data_packet));
        ssv_netlink_close(&gnl_fd);
    } else {
        printf("Fail to create ssv_wifi_ctrl\n");
    }
}

const ssv_cli_cmd_st g_cli_cmd_tbl[] =
{
    { "rawdata",           cmd_send_rawdata_frame,                  ""                            },
    { NULL, NULL, NULL },
};

static void cli_cmd_show(void) {

    const ssv_cli_cmd_st *cmd_ptr;

    printf("util:\n");
    for(cmd_ptr=g_cli_cmd_tbl; cmd_ptr->cmd; cmd_ptr++) {
        printf("\t%-15s %s\n", cmd_ptr->cmd, cmd_ptr->cmd_usage);
    }
}



int main(int argc, char *argv[])
{
    const ssv_cli_cmd_st *cmd_ptr;
    struct icomm_osif_hal *icomm_hal_sc = NULL;

    if (argc < 2) {
        cli_cmd_show();
        return 0;
    }
    
    for (cmd_ptr=g_cli_cmd_tbl; cmd_ptr->cmd; cmd_ptr++) {
        if(!strcmp(argv[1], cmd_ptr->cmd)) {
            fflush(stdout);
            cmd_ptr->cmd_handle(icomm_hal_sc, argc, argv);
            break;
        }
    }
    
    if (cmd_ptr->cmd == NULL) {
        printf("\ncmd not found\n");
        cli_cmd_show();
    }

    return 0;
}
