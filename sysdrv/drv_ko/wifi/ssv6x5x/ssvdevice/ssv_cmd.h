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

#ifndef _SSV_CMD_H_
#define _SSV_CMD_H_

#define CLI_ARG_SIZE                10

#define PROC_DIR_ENTRY              "ssv"
#define PROC_SSV_CMD_ENTRY          "ssv_cmd"
#define PROC_SSV_DBG_ENTRY          "ssv_dbg_fs"
#define PROC_SSV_FREQ_ENTRY         "freq"
#define PROC_SSV_P2P_ENTRY          "p2p"

#define MAX_CHARS_PER_LINE          512 //256

#ifdef CONFIG_SMART_ICOMM
#define PROC_SI_ENTRY               "smart_config"
#define PROC_SI_SSID_ENTRY          "si_ssid"
#define PROC_SI_PASS_ENTRY          "si_pass"
#endif //CONFIG_SMART_ICOMM

struct ssv_softc;

struct ssv_cmd_table {
    const char *cmd;
    int (*cmd_func_ptr)(struct ssv_softc *sc, int, char **);
    const char *usage;
    const int  result_buffer_size;
};

struct ssv6xxx_cfg_cmd_table {
    u8 *cfg_cmd;
    void *var;
    u32 arg;
    int (*translate_func)(u8 *, void *, u32);
    u8  *def_val;
};

#define SSV_REG_READ1(ops, reg, val) \
        (ops)->ifops->readreg((ops)->dev, reg, val)
#define SSV_REG_WRITE1(ops, reg, val) \
        (ops)->ifops->writereg((ops)->dev, reg, val)
#define SSV_REG_SET_BITS1(ops, reg, set, clr) \
    {                                           \
        u32 reg_val;                            \
        SSV_REG_READ(ops, reg, &reg_val);        \
        reg_val &= ~(clr);                      \
        reg_val |= (set);                       \
        SSV_REG_WRITE(ops, reg, reg_val);        \
    }

struct ssv_cmd_data;
int ssv_cmd_submit(struct ssv_cmd_data *cmd_data, char *cmd);
void snprintf_res(struct ssv_cmd_data *cmd_data, const char *fmt, ... );
struct sk_buff *ssvdevice_skb_alloc(s32 len);
void ssvdevice_skb_free(struct sk_buff *skb);

#define SSV_DUMP_WSID(_sh)                      HAL_DUMP_WSID(_sh)
#define SSV_DUMP_DECISION(_sh)                  HAL_DUMP_DECISION(_sh)
#define SSV_READ_FFOUT_CNT(_sh, _value, _value1, _value2)                   \
                HAL_READ_FFOUT_CNT(_sh, _value, _value1, _value2)
#define SSV_READ_IN_FFCNT(_sh, _value, _value1) HAL_READ_IN_FFCNT(_sh, _value, _value1)
#define SSV_READ_ID_LEN_THRESHOLD(_sh, _tx_len, _rx_len)  					\
				HAL_READ_ID_LEN_THRESHOLD(_sh, _tx_len, _rx_len)    
#define SSV_READ_ALLID_MAP(_sh, _id0, _id1, _id2, _id3)  	HAL_READ_ALLID_MAP(_sh, _id0, _id1, _id2, _id3)
#define SSV_READ_TXID_MAP(_sh, _id0, _id1, _id2, _id3)  	HAL_READ_TXID_MAP(_sh, _id0, _id1, _id2, _id3)
#define SSV_READ_RXID_MAP(_sh, _id0, _id1, _id2, _id3)  	HAL_READ_RXID_MAP(_sh, _id0, _id1, _id2, _id3)
#define SSV_READ_TAG_STATUS(_sh, _ava_status)  	HAL_READ_TAG_STATUS(_sh, _ava_status)
#define SSV_CMD_MIB(_sc, _argc, _argv)          HAL_CMD_MIB(_sc, _argc, _argv)
#define SSV_CMD_POWER_SAVING(_sc, _argc, _argv) HAL_CMD_POWER_SAVING(_sc, _argc, _argv)
#define SSV_GET_FW_VERSION(_sh, _regval)        HAL_GET_FW_VERSION(_sh, _regval)
#define SSV_TXTPUT_SET_DESC(_sh, _skb)          HAL_TXTPUT_SET_DESC(_sh, _skb)
#define SSV_READRG_TXQ_INFO2(_ifops, _sh , _txq_info2)                      \
                HAL_READRG_TXQ_INFO2(_sh , _txq_info2)
#define SSV_GET_RD_ID_ADR(_sh, _id_base_addr)   HAL_GET_RD_ID_ADR(_sh, _id_base_addr)
#define SSV_GET_FFOUT_CNT(_sh, _value, _tag)	HAL_GET_FFOUT_CNT(_sh, _value, _tag)	    		
#define SSV_GET_IN_FFCNT(_sh, _value, _tag)		HAL_GET_IN_FFCNT(_sh, _value, _tag)	    		
#define SSV_AUTO_GEN_NULLPKT(_sh, _hwq)	        HAL_AUTO_GEN_NULLPKT(_sh, _hwq)	    		

#endif /* _SSV_CMD_H_ */


