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

#ifndef _SSV6006_MAC_H_
#define _SSV6006_MAC_H_

#define FPGA_PHY_5     "5."     // for fpga version 5.xx  
#define FPGA_PHY_4     ((u64)0x2015123100171848)
#define RF_GEMINA      0x2B9
#define RF_TURISMOA    0x06579597
#define ADR_GEMINA_TRX_VER	0xCBB0E008
#define ADR_TURISMO_TRX_VER 0xCBC0E008
#define CHIP_TYPE_CHIP 0xA0
#define CHIP_TYPE_FPGA 0x0F


#define SSV6006_NUM_HW_STA  8

#define SSV6006_NUM_HW_BSSID        2

#define SSV6006_RX_BA_MAX_SESSIONS	255 // set to 255 since there is no limit for Turismo

#define PBUF_BASE_ADDR	            0x80000000
#define PBUF_ADDR_SHIFT	            16

#define PBUF_MapPkttoID(_pkt)		(((u32)_pkt&0x0FFF0000)>>PBUF_ADDR_SHIFT)	
#define PBUF_MapIDtoPkt(_id)		(PBUF_BASE_ADDR|((_id)<<PBUF_ADDR_SHIFT))

/* turismo rate control word defintion
*
*    Encoding Rate table in one byte as below:
*
*    bit[7:6]: phy_mode<2>  
*    bit[5]  : ht40<1>      
*    bit[4]  : Long/short preamble/GI <1>
*    bit[3]  : mf<1> (mix mode / Green field)       
*    bit[2:0]: RateIdx<3>
*/
struct ssv6006_rc_idx
{
    u8  rate_idx:3;
    u8  mf:1;
    u8  long_short:1;
    u8  ht40:1;
    u8  phy_mode:2;
};
#define SSV6006RC_MAX_RATE_RETRY   10
#define SSV6006RC_MAX_RATE_SERIES   4
// Phy Mode Encoding
#define SSV6006RC_PHY_MODE_MSK      0xc0
#define SSV6006RC_PHY_MODE_SFT         6
#define SSV6006RC_B_MODE    0
#define SSV6006RC_G_MODE    2
#define SSV6006RC_N_MODE    3	
// ht40
#define SSV6006RC_20_40_MSK         0x20
#define SSV6006RC_20_40_SFT          5
#define SSV6006RC_HT20      0
#define SSV6006RC_HT40      1
// long/short preamble/GI
#define SSV6006RC_LONG_SHORT_MSK    0x10
#define SSV6006RC_LONG_SHORT_SFT     4
#define SSV6006RC_LONG      0
#define SSV6006RC_SHORT     1
// mf Field Encoding
#define SSV6006RC_MF_MSK            0x08
#define SSV6006RC_MF_SFT            3
#define SSV6006RC_MIX       0
#define SSV6006RC_GREEN     1
//Rate Encoding
#define SSV6006RC_B_RATE_MSK        0x3
#define SSV6006RC_RATE_MSK          0x7
#define SSV6006RC_RATE_SFT          0
#define SSV6006RC_B_1M      0
#define SSV6006RC_B_2M      1
#define SSV6006RC_B_5_5M    2
#define SSV6006RC_B_11M     3
#define SSV6006RC_B_MAX_RATE  4

#define DOT11_G_RATE_IDX_OFFSET     4       // g mode rate idx is from 4.
#define SSV6006RC_G_6M      0
#define SSV6006RC_G_9M      1
#define SSV6006RC_G_12M     2
#define SSV6006RC_G_18M     3
#define SSV6006RC_G_24M     4
#define SSV6006RC_G_36M     5
#define SSV6006RC_G_48M     6
#define SSV6006RC_G_54M     7

#define SSV6006RC_N_MCS0    0
#define SSV6006RC_N_MCS1    1
#define SSV6006RC_N_MCS2    2
#define SSV6006RC_N_MCS3    3
#define SSV6006RC_N_MCS4    4
#define SSV6006RC_N_MCS5    5
#define SSV6006RC_N_MCS6    6
#define SSV6006RC_N_MCS7    7
#define SSV6006RC_MAX_RATE  8

#define SSV6006_EDCCA_AVG_T_6US			0x0
#define SSV6006_EDCCA_AVG_T_12US		0x1
#define SSV6006_EDCCA_AVG_T_25US		0x2
#define SSV6006_EDCCA_AVG_T_51US		0x3
#define SSV6006_EDCCA_AVG_T_102US		0x4
#define SSV6006_EDCCA_AVG_T_204US		0x5
#define SSV6006_EDCCA_AVG_T_409US		0x6
#define SSV6006_EDCCA_AVG_T_819US		0x7

/* Flash Config */
#define DEFAULT_CFG_BIN_NAME "/tmp/flash.bin"
#define SEC_CFG_BIN_NAME "/system/etc/wifi/ssv6x5x/flash.bin"

/**
* SSV6006 HW Security Table: 2 BSSID entry + 8 WSID entry (in packet buffer): 
*/
#define SSV6006_HW_SEC_TABLE_SIZE        sizeof(struct ssv6006_hw_sec)  //2*BSSID(140 bytes) + 8*WSID(44 bytes)
#define SSV6006_HW_KEY_SIZE              32  //one hw key size, refer to ssv6006_hw_key
#define SSV6006_PAIRWISE_KEY_OFFSET      12  //refer to ssv6006_hw_sta_key
#define SSV6006_GROUP_KEY_OFFSET         12  //refer to ssv6006_bss

/**
* struct ssv6006_hw_sec - ssv6006 hardware security engine data structure.
* This descriptor is shared with ssv6006 hardware and driver.
*/
#define SSV6006_SECURITY_KEY_LEN              (32)

struct ssv6006_hw_key {
    u8          key[SSV6006_SECURITY_KEY_LEN];
}__attribute__((packed));

/**
* Key index for each security
* - WEP security: key indexes 0~3 are all stored in pair_key_idx.
* - others security: pairwise key index is stored in pair_key_idx,
*                    group key indexes are stored in group_key_idx.
*/    
struct ssv6006_hw_sta_key {
    u8         	pair_key_idx;       /* 0: pairwise key, 1-3: group key */
    u8         	pair_cipher_type;   //WEP,TKIP,CCMP
    u8          valid;              /* 0: invalid entry, 1: valid entry asic hw don't check this field*/
    u8          reserve[1];
    u32         tx_pn_l;
    u32         tx_pn_h;

    struct ssv6006_hw_key pair;
}__attribute__((packed));

struct ssv6006_bss {
    u8          group_key_idx;
    u8          group_cipher_type;  //WEP,TKIP,CCMP
    u8          reserve[2];
    u32         tx_pn_l;
    u32         tx_pn_h;

	struct ssv6006_hw_key group_key[4]; //mapping to protocol group key 0-3
}__attribute__((packed));

struct ssv6006_hw_sec {
    struct ssv6006_bss bss_group[2];       /* The maximal number of VIFs: 2 */
    struct ssv6006_hw_sta_key sta_key[8];  /* The maximal number of STAs: 8 */
}__attribute__((packed));

struct ssv6006_flash_layout_table 
{
    // 0 ~ 3 word
    u16             ic;
    u16             sid;
    u32             date;
    u32             version:24;
    u32             RSVD0:8;
    u8              dcdc;
    u8              padpd;
    u16             RSVD1;
    // 4 ~ 7 word
    u16             xtal_offset_rt_config;  
    u16             xtal_offset_lt_config; 
    u16             xtal_offset_ht_config;
    u16             RSVD2;
    u8              xtal_offset_low_boundary;
    u8              xtal_offset_high_boundary;
    u8              band_gain_low_boundary;
    u8              band_gain_high_boundary;
    u32             RSVD3;
    // 8 ~ 11 word
    u8              g_band_gain_rt[7];//0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14
    u8              RSVD4;
    u32             g_band_pa_bias0;    
    u32             g_band_pa_bias1;   
    // 12 ~ 15 word
    u8              g_band_gain_lt[7];//0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14
    u8              RSVD5;
    u8              g_band_gain_ht[7];//0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14
    u8              RSVD6;
    // 16 ~ 19 word
    u8              rate_delta[13];
    u8              RSVD7;
    u16             RSVD8;
    // 20 ~ 23 word
    u8              a_band_gain_rt[4];//0:band0, 1:band1, ..., 3:band3
    u32             RSVD9;
    u32             a_band_pa_bias0;
    u32             a_band_pa_bias1;
    // 24 ~ 27 word
    u8              a_band_gain_lt[4];
    u8              a_band_gain_ht[4];
    u32             RSVD10;
    u32             RSVD11;
} __attribute__((packed));

void ssv6006c_cmd_hwinfo_phy_cali(struct ssv_hw *sh);

#endif
