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

#ifndef _DRV_COMM_H_
#define _DRV_COMM_H_

#define PHY_INFO_TBL1_SIZE          39
#define PHY_INFO_TBL2_SIZE          39
#define PHY_INFO_TBL3_SIZE          8

/* The maximal number of STAs */
#define SSV_NUM_STA     8

/* The maximal number of VIFs */
#define SSV_NUM_VIF     2

/**
* ssv6xxx hardware security engine data structure.
*/
#define SECURITY_KEY_LEN              (32)

struct ssv6xxx_hw_key {
    u8          key[SECURITY_KEY_LEN];
	u32			tx_pn_l;
    u32         tx_pn_h;
	u32        	rx_pn_l;
    u32         rx_pn_h;
}__attribute__((packed));

struct ssv6xxx_hw_sta_key {
	u8         	pair_key_idx:4;		/* 0: pairwise key, 1-3: group key */
	u8         	group_key_idx:4;	/* 0: pairwise key, 1-3: group key */
    
    u8          valid;              /* 0: invalid entry, 1: valid entry asic hw don't check this field*/
	u8			reserve[2];
	
	struct ssv6xxx_hw_key	pair;
}__attribute__((packed));

struct ssv6xxx_hw_sec {
    struct ssv6xxx_hw_key group_key[3];	//mapping to protocol group key 1-3
    struct ssv6xxx_hw_sta_key sta_key[8];
}__attribute__((packed));

#endif /* _DRV_COMM_H_ */



