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

#ifndef _SSV6200_COMMON_H_
#define _SSV6200_COMMON_H_

#include <ssv6xxx_common.h>
#define FW_VERSION_REG   ADR_TX_SEG
/*
    Reference with firmware
*/

/* Hardware Offload Engine ID */
#define M_ENG_CPU                       0x00
#define M_ENG_HWHCI                     0x01
//#define M_ENG_FRAG                    0x02
#define M_ENG_EMPTY                     0x02
#define M_ENG_ENCRYPT                   0x03
#define M_ENG_MACRX                     0x04  
#define M_ENG_MIC                       0x05
#define M_ENG_TX_EDCA0                  0x06
#define M_ENG_TX_EDCA1                  0x07
#define M_ENG_TX_EDCA2                  0x08
#define M_ENG_TX_EDCA3                  0x09
#define M_ENG_TX_MNG                    0x0A
#define M_ENG_ENCRYPT_SEC               0x0B
#define M_ENG_MIC_SEC                   0x0C
#define M_ENG_RESERVED_1                0x0D
#define M_ENG_RESERVED_2                0x0E
#define M_ENG_TRASH_CAN                 0x0F
#define M_ENG_MAX                      (M_ENG_TRASH_CAN+1)


/* Software Engine ID: */
#define M_CPU_HWENG                     0x00
#define M_CPU_TXL34CS                   0x01
#define M_CPU_RXL34CS                   0x02
#define M_CPU_DEFRAG                    0x03
#define M_CPU_EDCATX                    0x04
#define M_CPU_RXDATA                    0x05
#define M_CPU_RXMGMT                    0x06
#define M_CPU_RXCTRL                    0x07
#define M_CPU_FRAG                      0x08
#define M_CPU_TXTPUT                    0x09

#ifndef ID_TRAP_SW_TXTPUT
#define ID_TRAP_SW_TXTPUT               50 //(ID_TRAP_SW_START + M_CPU_TXTPUT - 1)
#endif //ID_TRAP_SW_TXTPUT

#define TXPB_OFFSET		80
#define RXPB_OFFSET		80

//TX_PKT_RSVD(3) * unit(16)
#define SSV6200_PKT_HEADROOM_RSVD       (80)
#define SSV6200_TX_PKT_RSVD_SETTING     (0x3)
#define SSV6200_TX_PKT_RSVD             (SSV6200_TX_PKT_RSVD_SETTING*16)
#define SSV6200_ALLOC_RSVD              (TXPB_OFFSET+SSV6200_TX_PKT_RSVD+SSV6200_PKT_HEADROOM_RSVD)


/* The maximal number of hardware offload STAs */
#define SSV_NUM_HW_STA  2


typedef enum{
//===========================================================================    
    SSV6XXX_RC_COUNTER_CLEAR                = 1                                                     ,
    SSV6XXX_RC_REPORT                                                                            ,
    
//===========================================================================    
} ssv6xxx_host_rate_control_event;


struct ssv62xx_tx_rate {
    s8 data_rate;
    u8 count;
} __attribute__((packed));

struct ampdu_ba_notify_data {
    //u16 retry_count;
    u8  wsid;
    struct ssv62xx_tx_rate tried_rates[SSV62XX_TX_MAX_RATES];
    u16 seq_no[MAX_AGGR_NUM];    
} __attribute__((packed));

struct firmware_rate_control_report_data{
    u8 wsid;
    struct ssv62xx_tx_rate rates[SSV62XX_TX_MAX_RATES];
    u16 ampdu_len;
    u16 ampdu_ack_len;
    int ack_signal;
    /* 15 bytes free */
} __attribute__((packed));

#define RC_RETRY_PARAM_OFFSET  ((sizeof(struct fw_rc_retry_params))*SSV62XX_TX_MAX_RATES)
#define SSV_RC_RATE_MAX                     39


#endif /* _SSV6200_COMMON_H_ */
