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

#ifndef _SSV_RFTOOL_H_
#define _SSV_RFTOOL_H_


/**
* struct ssv_nimble_softc - hold the whole nimble driver data structure.
*
*/
struct ssv_rftool_softc {
    struct device               *dev;
    void                        *hci_priv;
    struct ssv6xxx_hci_ops      *hci_ops;
    struct completion ssv_rftool_cmd_done;
};

struct ssv_rftool_cfg {

    u8      efuse_mac[6];

    //bool    greentx_en;
    //u8     disable_fw_thermal;

    //efuse item
    u8 psk[32];
    u32 skuid;
    u32 ble_power;
    u8 xtal;
    u8 power_ch_1_7;
    u8 power_ch_8_14;
    u8 gain_b;
    u8 gain_n40;
    u8 gain_g;
    u8 gain_n20;
    u8 hetb_cali_rate;
    u8 hetb_cali_power;
    u8 gain_su20;
    u8 gain_su40;
    u8 gain_tb20;
    u8 gain_tb40;
    u8 free_item;
    u8 psk_len;
    u8 reserved;
    u16 usb_pid;
    u16 usb_vid;
    s8 temperature;
    u16 ble_rx_count;
    u16 ble_rx_err_count;
};

#endif
