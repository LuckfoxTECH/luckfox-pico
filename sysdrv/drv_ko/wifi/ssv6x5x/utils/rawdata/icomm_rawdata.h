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

#ifndef _ICOMM_RAWDATA_H_
#define _ICOMM_RAWDATA_H_

#include <linux/genetlink.h>

typedef struct ssv_cli_cmd {
    const char        *cmd;
    void (*cmd_handle) (void *, int, char **);
    const char        *cmd_usage;
} ssv_cli_cmd_st;

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

#define MAX_PAYLOAD (4096)
struct netlink_msg {
    struct nlmsghdr n;
    struct genlmsghdr g;
    char buf[MAX_PAYLOAD];
};

/* Generic macros for dealing with netlink sockets. */
#define GENLMSG_DATA(glh) ((void *)(NLMSG_DATA(glh) + GENL_HDRLEN))
#define GENLMSG_PAYLOAD(glh) (NLMSG_PAYLOAD(glh, 0) - GENL_HDRLEN)
#define NLA_DATA(na) ((void *)((char*)(na) + NLA_HDRLEN))
//#define NLA_PAYLOAD(len) (len - NLA_HDRLEN)

#endif
