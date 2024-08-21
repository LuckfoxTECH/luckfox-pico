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

#ifndef _DEV_H_
#define _DEV_H_

#include "utils/debugfs.h"
#include "rf_table.h"

#define SSV_DRVER_NAME "SSV WLAN driver"
#define HOUSE_KEEPING_TIMEOUT       5

struct ssv_timer_list {
	struct timer_list timer;
	void (*function)(unsigned long argv);
	void *arg;
};

struct ssv_house_keeping {
    // house keeping
    struct ssv_timer_list time;
};

struct ssv_hw {
    struct ssv_softc            *sc;
    struct ssv_nimble_softc     *snc;
    struct ssv_rftool_softc       *srfc;
    struct ssv_ble_softc        *sbsc;
    struct device               *dev;
    struct ssv6xxx_hwif_ops     *hwif_ops;
    void                        *hci_priv;
    struct ssv6xxx_hci_ops      *hci_ops;
    struct ssv_cmd_data          cmd_data;
    struct ssv_house_keeping house_keeping;
    /* hanlde house keeping work queue*/
    unsigned long reorder_work_timestamp;
    unsigned long probe_sta_work_timestamp;
#ifdef SEND_KEEP_ALIVE
    unsigned long send_keep_alive_timestamp;
#endif
    unsigned long check_fw_status_timestamp;
    //user space record
    struct net *usernet;
    u16 userport;
    /* for sc configuration */
    struct mutex mutex;

    struct st_rf_table rf_conf_table;
    u8      maddr[2][6];
};
#endif
