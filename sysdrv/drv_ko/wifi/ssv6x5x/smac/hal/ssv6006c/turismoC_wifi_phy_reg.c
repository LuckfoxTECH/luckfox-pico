/*
 * Copyright (c) 2015 iComm Semiconductor Ltd.
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
 
#define SSV6006_TURISMOC_PHY_TABLE_VER "26.00"

ssv_cabrio_reg ssv6006_turismoC_phy_setting[]={
    // turn off phy
    //{0xccb0e004,0x00000000},

    // Tx packet generator
    {0xccb0e010,0x00000FFF},
    {0xccb0e014,0x00807f03},
    {0xccb0e018,0x0055003C},
    {0xccb0e01C,0x00000064},
    {0xccb0e020,0x00000000},

    // AGC
    {0xccb0e02C,0x7004606C},
    {0xccb0e030,0x7004606C},
    {0xccb0e034,0x1A040400},
    {0xccb0e038,0x630F36D0},
    {0xccb0e03C,0x100c0003},

    // DAGC 11b
    {0xccb0e040,0x11600800},
    {0xccb0e044,0x00080868},

    // DAGC 11gn HT20
    {0xccb0e048,0xFF001160},
    {0xccb0e04C,0x00100040},

    // agc bandwidth
    {0xccb0e060,0x11501150},

    // DAGC 11gn HT40
    {0xccb0e12C,0x00001160},
    {0xccb0e130,0x00100040},

    // DAGC 11gn init gain
    {0xccb0e134,0x00080010},

    // baseband power control
    {0xccb0e180,0x00010060},//barker_cck
    {0xccb0e184,0xB5A19080},//Legacy bpsk,qpsk,16qam,64qam
    {0xccb0e188,0xB5A19080},//ht20 bpsk,qpsk,16qam,64qam
    {0xccb0e18c,0xB5A19080},//ht40 bpsk,qpsk,16qam,64qam

    // rf power control
    {0xccb0e190,0x00010006},//barker_cck
    {0xccb0e194,0x06060606},//Legacy bpsk,qpsk,16qam,64qam
    {0xccb0e198,0x06060606},//ht20 bpsk,qpsk,16qam,64qam
    {0xccb0e19c,0x06060606},//ht40 bpsk,qpsk,16qam,64qam

    // RSSI
    {0xccb0e080,0x0110000F},

    // AGC relock
    {0xccb0e098,0x00102000}, // {rg_agc_relock_11b[31], rg_agc_relock_11gn[30], rg_agc_relock_en[28],
                             //  rg_agc_relock_sel[25:24], rg_agc_relock_cnt_th[21:16], rg_agc_relock_pwr_th[13:0]};
    {0xccb0e09C,0x00100018}, // {rg_agc_relock_cnt_diffdb_th[21:16], rg_agc_relock_pwr_diffdb_th[6:0]}

    // Tx 11b ramp setting
    {0xccb0e4b4,0x00002001},

    // Tx 11gn ramp setting
    {0xccb0ecA4,0x00009001},

    // Tx 11gn ifft gain setting
    {0xccb0ecB8,0x000C50CC},

    // Tx IQ swap
    {0xccb0fc44,0x00028080},

    // RX 11gn setting
    {0xccb0f008,0x00004775},
    {0xccb0f00c,0x10000075},
    {0xccb0f010,0x3F304905},
    {0xccb0f014,0x40182000},
    {0xccb0f018,0x20600000},
    {0xccb0f01C,0x0c010080},
    {0xccb0f03C,0x0000005a},

    {0xccb0f020,0x20202020},
    {0xccb0f024,0x20000000},
    {0xccb0f028,0x50505050},
    {0xccb0f02c,0x20202020},
    {0xccb0f030,0x20000000},
    {0xccb0f034,0x00002424},

    {0xccb0f09c,0x000030A0},
    {0xccb0f0C0,0x0f0003c0},
    {0xccb0f0C4,0x30023003},

    {0xccb0f0CC,0x00000120},
    {0xccb0f0D0,0x00000020},

    {0xccb0f130,0x40000000},
    {0xccb0f164,0x000e0090},
    {0xccb0f188,0x82000000},
    {0xccb0f190,0x00000020},
    // new pilot average
    {0xccb0f194,0x09370001}, // turn on new pilot average when HT_MF
    {0xccb0f3F8,0x00100001},
    {0xccb0f3FC,0x00010425},

    // 11b Rx
    {0xccb0e804,0x00020000},
    {0xccb0e808,0x20280060},
    {0xccb0e80c,0x00003467},
    {0xccb0e810,0x00430000},
    {0xccb0e814,0x30000015},
    {0xccb0e818,0x00390005},
    {0xccb0e81C,0x05050005},
    {0xccb0e820,0x00570057},
    {0xccb0e824,0x00570057},
    {0xccb0e828,0x00236700},
    {0xccb0e82c,0x000d1746},
    {0xccb0e830,0x05051787},
    {0xccb0e834,0x07800000},

    {0xccb0e89c,0x009000B0},
    {0xccb0e8A0,0x00000000}, 
    {0xccb0ebF8,0x00100000},
    {0xccb0ebFC,0x00000001},

    //{0xccb0e000,0x80000000}, // ht20
    //{0xccb0e000,0x8000c000}, // ht40 high primary channel
    //{0xccb0e000,0x80008000}, // ht40 low primary channel

    // Tx DAC
    //{0xccb0a810,0x01000000},

    // Rx ADC
    //{0xccb0a808,0x08000000},
};

ssv_cabrio_reg ssv6006_turismoC_phy_hp_patch[]={
    // baseband power control
//    {0xccb0e180,0x00010060},//barker_cck
//    {0xccb0e184,0x90908888},//Legacy bpsk,qpsk,16qam,64qam
//    {0xccb0e188,0xB5A19885},//ht20 bpsk,qpsk,16qam,64qam
//    {0xccb0e18c,0xB5A19880},//ht40 bpsk,qpsk,16qam,64qam
    {0xccb0e180,0x00010072},//barker_cck
    {0xccb0e184,0x90807966},//Legacy bpsk,qpsk,16qam,64qam
    {0xccb0e188,0x90807966},//ht20 bpsk,qpsk,16qam,64qam
    {0xccb0e18c,0x90807966},//ht40 bpsk,qpsk,16qam,64qam
};

#define PAPDP_GAIN_SETTING      0x0
#define PAPDP_GAIN_SETTING_F2   0x0
#define PAPDP_GAIN_SETTING_2G   0x6

#define EDCCA_LOW       0x200
#define EDCCA_LOW_HT40  0x200
#define EDCCA_HIGH      0x800
#define EDCCA_HIGH_HT40 0x800
