/* * Copyright (c) 2015 iComm-semi Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int u32;

typedef struct ssv_cabrio_reg_st {
    u32 address;
    u32 data;
} ssv_cabrio_reg;


#include "../../../smac/hal/ssv6006c/turismoC_wifi_phy_reg.c"
#include "../../../include/ssv6006c/ssv6006C_reg.h"
#include "../../../include/ssv6006c/ssv6006C_aux.h"

#define FLASH_BIN_FILE_NAME     "flash.bin"

struct ssv6006_flash_layout_table 
{
    // 0 ~ 3 word
    short               ic;                         /* chip id */
    short               sid;                        /* sw version */
    int                 date;                       /* date */
    int                 version:24;                 /* version */
    int                 RSVD0:8;            
    char                dcdc;                       /* DCDC on/off, default: 1 */
    char                padpd;                      /* PADPD on/off, default: 1 */
    short               RSVD1;
    // 4 ~ 7 word
    short               xtal_offset_rt_config;      /* rt xtal frequency offset */ 
    short               xtal_offset_lt_config;      /* lt xtal frequency offset */
    short               xtal_offset_ht_config;      /* ht xtal frequency offset */
    short               RSVD2;
    char                xtal_offset_low_boundary;   /* xtal frequency offset temperature low boundary */
    char                xtal_offset_high_boundary;  /* xtal frequency offset temperature high boundary */
    char                band_gain_low_boundary;     /* band gain temperature low boundary */
    char                band_gain_high_boundary;    /* band gain temperature high boundary */
    int                 RSVD3;
    // 8 ~ 11 word
    char                g_band_gain_rt[7];          /* rt 2g band gain, 0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14 */
    char                RSVD4;
    int                 g_band_pa_bias0;            /* 2g PABIAS */ 
    int                 g_band_pa_bias1;            /* 2g PABIAS */
    // 12 ~ 15 word
    char                g_band_gain_lt[7];          /* lt 2g band gain, 0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14 */
    char                RSVD5;
    char                g_band_gain_ht[7];          /* ht 2g band gain, 0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14 */          
    char                RSVD6;
    // 16 ~ 19 word
    char                rate_delta[13];             /* rate gain */
    char                RSVD7;
    short               RSVD8;
    // 20 ~ 23 word
    char                a_band_gain_rt[4];          /* rt 5g band gain 0:band0, 1:band1, ..., 3:band3 */
    int                 RSVD9;
    int                 a_band_pa_bias0;            /* 5g PABIAS */
    int                 a_band_pa_bias1;            /* 5G PABIAS */
    // 24 ~ 27 word
    char                a_band_gain_lt[4];          /* lt 5g band gain 0:band0, 1:band1, ..., 3:band3 */
    char                a_band_gain_ht[4];          /* ht 5g band gain 0:band0, 1:band1, ..., 3:band3 */
    int                 RSVD10;
    int                 RSVD11;
} __attribute__((packed));

#define     FACTORY_DEFAULT  -1

int main()
{
    FILE *ptr;
    struct ssv6006_flash_layout_table table;
    int i;
    ssv_cabrio_reg *raw;
    int b_rate_gain = 0;
    int g_dpsk_rate_gain = 0, g_qpsk_rate_gain = 0, g_16qam_rate_gain = 0, g_64qam_rate_gain = 0;
    int ht20_dpsk_rate_gain = 0, ht20_qpsk_rate_gain = 0, ht20_16qam_rate_gain = 0, ht20_64qam_rate_gain = 0;
    int ht40_dpsk_rate_gain = 0, ht40_qpsk_rate_gain = 0, ht40_16qam_rate_gain = 0, ht40_64qam_rate_gain = 0;
    
    printf("flash_bin_util v1.1:  Copyright (c) 2019 iComm-semi Ltd.\n");
    memset(&table, 0, sizeof(struct ssv6006_flash_layout_table));
    
    table.ic = 0x6155;
    table.sid = 0x3009;
    table.date = 0x20170503;
    table.version = 0x00300a;
    table.dcdc = 0x01;                          /* DCDC on/off, default: 1 */
    table.padpd = 0x01;                         /* PADPD on/off, default: 1 */
    
    table.xtal_offset_rt_config = 0x4242;       /* rt xtal frequency offset */ 
    table.xtal_offset_lt_config = 0x4242;       /* lt xtal frequency offset */
    table.xtal_offset_ht_config = 0x4242;       /* ht xtal frequency offset */
    table.xtal_offset_low_boundary = 0x0;      /* xtal frequency offset temperature low boundary */
    table.xtal_offset_high_boundary = 0xff;     /* xtal frequency offset temperature high boundary */
    table.band_gain_low_boundary = 0x0;        /* band gain temperature low boundary */
    table.band_gain_high_boundary = 0xff;       /* band gain temperature high boundary */
    
    /* rt 2g band gain, 0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14 */
    printf("2.4G band_gain factory default: 0x%x\n", DEFAULT_DPD_BBSCALE_2500);
    table.g_band_gain_rt[0] = (char) FACTORY_DEFAULT;
    table.g_band_gain_rt[1] = (char) FACTORY_DEFAULT;
    table.g_band_gain_rt[2] = (char) FACTORY_DEFAULT;
    table.g_band_gain_rt[3] = (char) FACTORY_DEFAULT;
    table.g_band_gain_rt[4] = (char) FACTORY_DEFAULT;
    table.g_band_gain_rt[5] = (char) FACTORY_DEFAULT;
    table.g_band_gain_rt[6] = (char) FACTORY_DEFAULT;

    for (i = 0; i < 7; i++){
        if (table.g_band_gain_rt[i] == FACTORY_DEFAULT) {
            table.g_band_gain_rt[i] = DEFAULT_DPD_BBSCALE_2500;
        }
    }
    /* 2g PABIAS */
    table.g_band_pa_bias0 = 0x0033e73f;    
    table.g_band_pa_bias1 = 0x57444497;   
    
    /* lt 2g band gain, 0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14 */
    table.g_band_gain_lt[0] = FACTORY_DEFAULT;
    table.g_band_gain_lt[1] = FACTORY_DEFAULT;
    table.g_band_gain_lt[2] = FACTORY_DEFAULT;
    table.g_band_gain_lt[3] = FACTORY_DEFAULT;
    table.g_band_gain_lt[4] = FACTORY_DEFAULT;
    table.g_band_gain_lt[5] = FACTORY_DEFAULT;
    table.g_band_gain_lt[6] = FACTORY_DEFAULT;
    
    for (i = 0; i < 7; i++){
        if (table.g_band_gain_lt[i] == FACTORY_DEFAULT) {
            table.g_band_gain_lt[i] = DEFAULT_DPD_BBSCALE_2500;
        }
    }    
    /* ht 2g band gain, 0:ch1~ch2, 1:ch2~ch3, ..., 6:ch13~ch14 */
    table.g_band_gain_ht[0] = FACTORY_DEFAULT;
    table.g_band_gain_ht[1] = FACTORY_DEFAULT;
    table.g_band_gain_ht[2] = FACTORY_DEFAULT;
    table.g_band_gain_ht[3] = FACTORY_DEFAULT;
    table.g_band_gain_ht[4] = FACTORY_DEFAULT;
    table.g_band_gain_ht[5] = FACTORY_DEFAULT;
    table.g_band_gain_ht[6] = FACTORY_DEFAULT;
    for (i = 0; i < 7; i++){
        if (table.g_band_gain_ht[i] == FACTORY_DEFAULT) {
            table.g_band_gain_ht[i] = DEFAULT_DPD_BBSCALE_2500;
        }
    }
    
    raw = ssv6006_turismoC_phy_setting;
    /* rate gain */
    for (i = 0; i < (sizeof(ssv6006_turismoC_phy_setting) / sizeof(ssv_cabrio_reg));  i ++, raw++){
        if (raw->address== ADR_WIFI_PHY_COMMON_BB_SCALE_REG_0){
            b_rate_gain = ((raw->data & RG_BB_SCALE_BARKER_CCK_MSK) >> RG_BB_SCALE_BARKER_CCK_SFT);
        }
        if (raw->address== ADR_WIFI_PHY_COMMON_BB_SCALE_REG_1){
            g_dpsk_rate_gain = ((raw->data & RG_BB_SCALE_LEGACY_BPSK_MSK) >> RG_BB_SCALE_LEGACY_BPSK_SFT);
            g_qpsk_rate_gain = ((raw->data & RG_BB_SCALE_LEGACY_QPSK_MSK) >> RG_BB_SCALE_LEGACY_QPSK_SFT);
            g_16qam_rate_gain = ((raw->data & RG_BB_SCALE_LEGACY_16QAM_MSK) >> RG_BB_SCALE_LEGACY_16QAM_SFT);
            g_64qam_rate_gain = ((raw->data & RG_BB_SCALE_LEGACY_64QAM_MSK) >> RG_BB_SCALE_LEGACY_64QAM_SFT);
        }
        if (raw->address== ADR_WIFI_PHY_COMMON_BB_SCALE_REG_2){
            ht20_dpsk_rate_gain = ((raw->data & RG_BB_SCALE_HT20_BPSK_MSK) >> RG_BB_SCALE_HT20_BPSK_SFT);
            ht20_qpsk_rate_gain = ((raw->data & RG_BB_SCALE_HT20_QPSK_MSK) >> RG_BB_SCALE_HT20_QPSK_SFT);
            ht20_16qam_rate_gain = ((raw->data & RG_BB_SCALE_HT20_16QAM_MSK) >> RG_BB_SCALE_HT20_16QAM_SFT);
            ht20_64qam_rate_gain = ((raw->data & RG_BB_SCALE_HT20_64QAM_MSK) >> RG_BB_SCALE_HT20_64QAM_SFT);
        }
        if (raw->address== ADR_WIFI_PHY_COMMON_BB_SCALE_REG_3){
            ht40_dpsk_rate_gain = ((raw->data & RG_BB_SCALE_HT40_BPSK_MSK) >> RG_BB_SCALE_HT40_BPSK_SFT);
            ht40_qpsk_rate_gain = ((raw->data & RG_BB_SCALE_HT40_QPSK_MSK) >> RG_BB_SCALE_HT40_QPSK_SFT);
            ht40_16qam_rate_gain = ((raw->data & RG_BB_SCALE_HT40_16QAM_MSK) >> RG_BB_SCALE_HT40_16QAM_SFT);
            ht40_64qam_rate_gain = ((raw->data & RG_BB_SCALE_HT40_64QAM_MSK) >> RG_BB_SCALE_HT40_64QAM_SFT);
        }                        
    }
    if (b_rate_gain != 0) {
        printf("B mode rate gain default: 0x%x\n", b_rate_gain);
    } else {
        printf("B mode rate gain no default value, set it to 0x80\n");
        b_rate_gain = 0x80;
    }

    if (g_dpsk_rate_gain != 0) {
        printf("G mode dpsk rate gain default: 0x%x\n", g_dpsk_rate_gain);
    } else {
        printf("G mode dpsk rate gain no default value, set it to 0x80\n");
        g_dpsk_rate_gain = 0x80;
    }
    if (g_qpsk_rate_gain != 0) {
        printf("G mode qpsk rate gain default: 0x%x\n", g_qpsk_rate_gain);
    } else {
        printf("G mode qpsk rate gain no default value, set it to 0x80\n");
        g_qpsk_rate_gain = 0x80;
    }
    if (g_16qam_rate_gain != 0) {
        printf("G mode 16QAM rate gain default: 0x%x\n", g_16qam_rate_gain);
    } else {
        printf("G mode 16QAM rate gain no default value, set it to 0x80\n");
        g_16qam_rate_gain = 0x80;
    }
    if (g_64qam_rate_gain != 0) {
        printf("G mode 64QAM rate gain default: 0x%x\n", g_64qam_rate_gain);
    } else {
        printf("G mode 64QAM rate gain no default value, set it to 0x80\n");
        g_64qam_rate_gain = 0x80;
    }
    
    if (ht20_dpsk_rate_gain != 0) {
        printf("HT20 dpsk rate gain default: 0x%x\n", ht20_dpsk_rate_gain);
    } else {
        printf("HT20 dpsk rate gain no default value, set it to 0x80\n");
        ht20_dpsk_rate_gain = 0x80;
    }
    if (ht20_qpsk_rate_gain != 0) {
        printf("HT20 qpsk rate gain default: 0x%x\n", ht20_qpsk_rate_gain);
    } else {
        printf("HT20 qpsk rate gain no default value, set it to 0x80\n");
        ht20_qpsk_rate_gain = 0x80;
    }
    if (ht20_16qam_rate_gain != 0) {
        printf("HT20 16QAM rate gain default: 0x%x\n", ht20_16qam_rate_gain);
    } else {
        printf("HT20 16QAM rate gain no default value, set it to 0x80\n");
        ht20_16qam_rate_gain = 0x80;
    }
    if (ht20_64qam_rate_gain != 0) {
        printf("HT20 64QAM rate gain default: 0x%x\n", ht20_64qam_rate_gain);
    } else {
        printf("HT20 64QAM rate gain no default value, set it to 0x80\n");
        ht20_64qam_rate_gain = 0x80;
    }
    
    if (ht40_dpsk_rate_gain != 0) {
        printf("HT40 dpsk rate gain default: 0x%x\n", ht40_dpsk_rate_gain);
    } else {
        printf("HT40 dpsk rate gain no default value, set it to 0x80\n");
        ht40_dpsk_rate_gain = 0x80;
    }
    if (ht40_qpsk_rate_gain != 0) {
        printf("HT40 qpsk rate gain default: 0x%x\n", ht40_qpsk_rate_gain);
    } else {
        printf("HT40 qpsk rate gain no default value, set it to 0x80\n");
        ht40_qpsk_rate_gain = 0x80;
    }
    if (ht40_16qam_rate_gain != 0) {
        printf("HT40 16QAM rate gain default: 0x%x\n", ht40_16qam_rate_gain);
    } else {
        printf("HT40 16QAM rate gain no default value, set it to 0x80\n");
        ht40_16qam_rate_gain = 0x80;
    }
    if (ht40_64qam_rate_gain != 0) {
        printf("HT40 64QAM rate gain default: 0x%x\n", ht40_64qam_rate_gain);
    } else {
        printf("HT40 64QAM rate gain no default value, set it to 0x80\n");
        ht40_64qam_rate_gain = 0x80;
    }         
    // rate gain 
    table.rate_delta[0] = FACTORY_DEFAULT;
    table.rate_delta[1] = FACTORY_DEFAULT;
    table.rate_delta[2] = FACTORY_DEFAULT;
    table.rate_delta[3] = FACTORY_DEFAULT;
    table.rate_delta[4] = FACTORY_DEFAULT;
    table.rate_delta[5] = FACTORY_DEFAULT;
    table.rate_delta[6] = FACTORY_DEFAULT;
    table.rate_delta[7] = FACTORY_DEFAULT;
    table.rate_delta[8] = FACTORY_DEFAULT;
    table.rate_delta[9] = FACTORY_DEFAULT;
    table.rate_delta[10] = FACTORY_DEFAULT;
    table.rate_delta[11] = FACTORY_DEFAULT;
    table.rate_delta[12] = FACTORY_DEFAULT;
    
    if (table.rate_delta[0] == FACTORY_DEFAULT){
        table.rate_delta[0] = b_rate_gain;
    }

    if (table.rate_delta[1] == FACTORY_DEFAULT){
        table.rate_delta[1] = g_dpsk_rate_gain;
    }
    if (table.rate_delta[2] == FACTORY_DEFAULT){
        table.rate_delta[2] = g_qpsk_rate_gain;
    }
    if (table.rate_delta[3] == FACTORY_DEFAULT){
        table.rate_delta[3] = g_16qam_rate_gain;
    }
    if (table.rate_delta[4] == FACTORY_DEFAULT){
        table.rate_delta[4] = g_64qam_rate_gain;
    }

    if (table.rate_delta[5] == FACTORY_DEFAULT){
        table.rate_delta[5] = ht20_dpsk_rate_gain;
    }
    if (table.rate_delta[6] == FACTORY_DEFAULT){
        table.rate_delta[6] = ht20_qpsk_rate_gain;
    }
    if (table.rate_delta[7] == FACTORY_DEFAULT){
        table.rate_delta[7] = ht20_16qam_rate_gain;
    }
    if (table.rate_delta[8] == FACTORY_DEFAULT){
        table.rate_delta[8] = ht20_64qam_rate_gain;
    }
    
    if (table.rate_delta[9] == FACTORY_DEFAULT){
        table.rate_delta[9] = ht40_dpsk_rate_gain;
    }
    if (table.rate_delta[10] == FACTORY_DEFAULT){
        table.rate_delta[10] = ht40_qpsk_rate_gain;
    }
    if (table.rate_delta[11] == FACTORY_DEFAULT){
        table.rate_delta[11] = ht40_16qam_rate_gain;
    }
    if (table.rate_delta[12] == FACTORY_DEFAULT){
        table.rate_delta[12] = ht40_64qam_rate_gain;
    }
    
    /* rt 5g band gain */
    printf("5 band 0 band_gain factory default: 0x%x\n", DEFAULT_DPD_BBSCALE_5100);
    printf("5 band 1 band_gain factory default: 0x%x\n", DEFAULT_DPD_BBSCALE_5500);
    printf("5 band 2 band_gain factory default: 0x%x\n", DEFAULT_DPD_BBSCALE_5700);
    printf("5 band 3 band_gain factory default: 0x%x\n", DEFAULT_DPD_BBSCALE_5900);
    table.a_band_gain_rt[0] = FACTORY_DEFAULT;
    table.a_band_gain_rt[1] = FACTORY_DEFAULT;
    table.a_band_gain_rt[2] = FACTORY_DEFAULT;
    table.a_band_gain_rt[3] = FACTORY_DEFAULT;
    
    if (table.a_band_gain_rt[0] == FACTORY_DEFAULT) {
        table.a_band_gain_rt[0] = DEFAULT_DPD_BBSCALE_5100;
    }
    if (table.a_band_gain_rt[1] == FACTORY_DEFAULT) {
        table.a_band_gain_rt[1] = DEFAULT_DPD_BBSCALE_5500;
    }
    if (table.a_band_gain_rt[2] == FACTORY_DEFAULT) {
        table.a_band_gain_rt[2] = DEFAULT_DPD_BBSCALE_5700;
    }
    if (table.a_band_gain_rt[3] == FACTORY_DEFAULT) {
        table.a_band_gain_rt[3] = DEFAULT_DPD_BBSCALE_5900;
    }
     
    /* 5g PABIAS */
    table.a_band_pa_bias0 = 0x924d924a;
    table.a_band_pa_bias1 = 0xb6cab6cc;
    
    /* lt 5g band gain */
    table.a_band_gain_lt[0] = FACTORY_DEFAULT;
    table.a_band_gain_lt[1] = FACTORY_DEFAULT;
    table.a_band_gain_lt[2] = FACTORY_DEFAULT;
    table.a_band_gain_lt[3] = FACTORY_DEFAULT;

    if (table.a_band_gain_lt[0] == FACTORY_DEFAULT) {
        table.a_band_gain_lt[0] = DEFAULT_DPD_BBSCALE_5100;
    }
    if (table.a_band_gain_lt[1] == FACTORY_DEFAULT) {
        table.a_band_gain_lt[1] = DEFAULT_DPD_BBSCALE_5500;
    }
    if (table.a_band_gain_lt[2] == FACTORY_DEFAULT) {
        table.a_band_gain_lt[2] = DEFAULT_DPD_BBSCALE_5700;
    }
    if (table.a_band_gain_lt[3] == FACTORY_DEFAULT) {
        table.a_band_gain_lt[3] = DEFAULT_DPD_BBSCALE_5900;
    } 
     
    /* ht 5g band gain */
    table.a_band_gain_ht[0] = FACTORY_DEFAULT;
    table.a_band_gain_ht[1] = FACTORY_DEFAULT;
    table.a_band_gain_ht[2] = FACTORY_DEFAULT;
    table.a_band_gain_ht[3] = FACTORY_DEFAULT;

    if (table.a_band_gain_ht[0] == FACTORY_DEFAULT) {
        table.a_band_gain_ht[0] = DEFAULT_DPD_BBSCALE_5100;
    }

    if (table.a_band_gain_ht[1] == FACTORY_DEFAULT) {
        table.a_band_gain_ht[1] = DEFAULT_DPD_BBSCALE_5500;
    } 
    if (table.a_band_gain_ht[2] == FACTORY_DEFAULT) {
        table.a_band_gain_ht[2] = DEFAULT_DPD_BBSCALE_5700;
    }
    if (table.a_band_gain_ht[3] == FACTORY_DEFAULT) {
        table.a_band_gain_ht[3] = DEFAULT_DPD_BBSCALE_5900;
    } 

    ptr = fopen(FLASH_BIN_FILE_NAME, "w");
    if (ptr) {
        fwrite(&table, sizeof(struct ssv6006_flash_layout_table), 1, ptr);
        fclose(ptr);
    }
        
    return 0;
}
