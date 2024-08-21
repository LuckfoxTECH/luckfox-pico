/*
 * Copyright (c) 2021 iComm-semi Ltd.
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

/*
 * Common defines and declarations for host driver and firmware for all
 * platforms.
 */


#ifndef __FMAC_CMDS_H__
#define __FMAC_CMDS_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/spinlock.h>
#include <linux/completion.h>
#include "lmac_msg.h"
#include "ipc_shared.h"


/*******************************************************************************
 *         Defines
 ******************************************************************************/
#define SSV_80211_CMD_TIMEOUT_MS            (2000)

#define SSV_CMD_FLAG_NONBLOCK               BIT(0)
#define SSV_CMD_FLAG_REQ_CFM                BIT(1)
#define SSV_CMD_FLAG_WAIT_PUSH              BIT(2)
#define SSV_CMD_FLAG_WAIT_ACK               BIT(3)
#define SSV_CMD_FLAG_WAIT_CFM               BIT(4)
#define SSV_CMD_FLAG_DONE                   BIT(5)
/* ATM IPC design makes it possible to get the CFM before the ACK,
 * otherwise this could have simply been a state enum */
#define SSV_CMD_WAIT_COMPLETE(flags) \
    (!(flags & (SSV_CMD_FLAG_WAIT_ACK | SSV_CMD_FLAG_WAIT_CFM)))

#define SSV_CMD_MAX_QUEUED                  (16)

struct ssv_softc;
struct ssv_cmd;
typedef int (*msg_cb_fct)(struct ssv_softc *sc, struct ssv_cmd *cmd,
                          struct ipc_e2a_msg *msg);


/*******************************************************************************
 *         Enumerations
 ******************************************************************************/
enum ssv_cmd_mgr_state {
    SSV_CMD_MGR_STATE_DEINIT,
    SSV_CMD_MGR_STATE_INITED,
    SSV_CMD_MGR_STATE_CRASHED,
};


/*******************************************************************************
 *         Structures
 ******************************************************************************/
struct ssv_cmd {
    struct list_head list;
    lmac_msg_id_t id;
    lmac_msg_id_t reqid;
    struct lmac_msg *a2e_msg;
    char            *e2a_msg;
    u32 tkn;
    u16 flags;

    struct completion complete;
    u32 result;
};

struct ssv_cmd_mgr {
    enum ssv_cmd_mgr_state state;
    struct mutex lock;
    u32 next_tkn;
    u32 queue_sz;
    u32 max_queue_sz;

    struct list_head cmds;

    int  (*queue)(struct ssv_cmd_mgr *, struct ssv_cmd *);
    int  (*llind)(struct ssv_cmd_mgr *, struct ssv_cmd *);
    int  (*msgind)(struct ssv_cmd_mgr *, struct ipc_e2a_msg *, msg_cb_fct);
    void (*print)(struct ssv_cmd_mgr *);
    void (*drain)(struct ssv_cmd_mgr *);
};


/*******************************************************************************
 *         Variables
 ******************************************************************************/



/*******************************************************************************
 *         Functions
 ******************************************************************************/
void ssv_cmd_dump(const struct ssv_cmd *cmd);
void ssv_cmd_mgr_init(struct ssv_cmd_mgr *cmd_mgr);
void ssv_cmd_mgr_deinit(struct ssv_cmd_mgr *cmd_mgr);
void ssv_msg_to_hci(struct ssv_softc *sc, void *msg_params);


#endif /* __FMAC_CMDS_H__ */

