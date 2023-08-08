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

#include <linux/version.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#ifdef CONFIG_SSV_SUPPORT_ANDROID
#include <linux/wakelock.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif //CONFIG_HAS_EARLYSUSPEND
#endif
#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
#include "drv_comm.h"
#include <linux/kthread.h>
#include <linux/completion.h>

#define SSV_DRVER_NAME "SSV WLAN driver"

// define RF mode
#define RF_MODE_SHUTDOWN	0
#define RF_MODE_STANDBY   1
#define RF_MODE_TRX_EN    2

// define mrx mode
#define MRX_MODE_PROMISCUOUS        0x2
#define MRX_MODE_NORMAL             0x3

// define CCI switch
#define CCI_CTL     0x1
#define CCI_DBG     0x2
#define CCI_P1      0x4
#define CCI_P2      0x8
#define CCI_SMART   0x10
#define MAX_CCI_LEVEL 128

// define for green Tx
#define GT_PWR_START_MASK   0xFF
#define GT_ENABLE           0x100
#define GT_DBG              0x200

// define for auto SGI
#define AUTOSGI_CTL 0x1
#define AUTOSGI_DBG 0x2

#define CCMP_MIC_LEN 8

#define MAX_AVAIL_CHANNEL           (39)

enum ssv6xxx_rc_phy_type {
    WLAN_RC_PHY_CCK,
    WLAN_RC_PHY_OFDM,
    WLAN_RC_PHY_HT_20_SS_LGI,
    WLAN_RC_PHY_HT_20_SS_SGI,
    WLAN_RC_PHY_HT_20_SS_GF,
};

/* rx flow*/
enum ssv_rx_flow{
    RX_DATA_FLOW,
    RX_MGMT_FLOW,
    RX_CTRL_FLOW,
};

typedef enum __PBuf_Type_E {
    NOTYPE_BUF  = 0,
    TX_BUF      = 1,
    RX_BUF      = 2
} PBuf_Type_E;

typedef struct ssv_cabrio_reg_st {
    u32 address;
    u32 data;
} __attribute__((packed)) ssv_cabrio_reg;

struct ssv_dbg_log {
	int size;
	int totalsize;
	char *data;
	char *top;
	char *tail;
	char *end;
};

struct ssv_cmd_data{
    char *ssv6xxx_result_buf;
    u32  rsbuf_len;
    u32  rsbuf_size;
    bool cmd_in_proc;
	bool log_to_ram;
	struct ssv_dbg_log dbg_log;
	struct proc_dir_entry *proc_dev_entry;
	atomic_t    cli_count;
};

// debug print
void dbgprint(struct ssv_cmd_data *cmd_data, u32 log_ctrl, u32 log_id, const char *fmt,...); 
void ssv6xxx_hci_dbgprint(void *argc, u32 log_id, const char *fmt,...);

#define SSV_TEMPERATURE_NORMAL     0
#define SSV_TEMPERATURE_HIGH       1
#define SSV_TEMPERATURE_LOW        2

#define RX_HCI                  M_ENG_MACRX|(M_ENG_HWHCI<<4)
#define RX_CIPHER_HCI           M_ENG_MACRX|(M_ENG_ENCRYPT_SEC<<4)|(M_ENG_HWHCI<<8)
#define RX_CPU_HCI              M_ENG_MACRX|(M_ENG_CPU<<4)|(M_ENG_HWHCI<<8)
#define RX_TRASH                M_ENG_MACRX|(M_ENG_TRASH_CAN<<4)
#define RX_CPU_TRASH            M_ENG_MACRX|(M_ENG_CPU<<4)|(M_ENG_TRASH_CAN<<8)
#define RX_CIPHER_MIC_HCI       M_ENG_MACRX|(M_ENG_ENCRYPT_SEC<<4)|(M_ENG_MIC_SEC<<8)|(M_ENG_HWHCI<<12)
#define RX_CIPHER_MIC_CPU_HCI   M_ENG_MACRX|(M_ENG_ENCRYPT_SEC<<4)|(M_ENG_MIC_SEC<<8)|(M_ENG_CPU<<12)|(M_ENG_HWHCI<<16)
#define RX_CIPHER_MIC_CPU_TRASH M_ENG_MACRX|(M_ENG_ENCRYPT_SEC<<4)|(M_ENG_MIC_SEC<<8)|(M_ENG_CPU<<12)|(M_ENG_TRASH_CAN<<16)

#define SSV6200_MAX_HW_MAC_ADDR             2
#define SSV6200_MAX_VIF                     2

#define SSV6200_RX_BA_MAX_SESSIONS			1

enum SSV6XXX_OPMODE {
    SSV6XXX_OPMODE_STA   = 0,
    SSV6XXX_OPMODE_AP    = 1,
    SSV6XXX_OPMODE_IBSS  = 2,
    SSV6XXX_OPMODE_WDS   = 3
};

#define IS_EQUAL(a, b)                  ( (a) == (b) )
#define SET_BIT(v, b)					( (v) |= (0x01<<b) )
#define CLEAR_BIT(v, b)			    	( (v) &= ~(0x01<<b) )
#define IS_BIT_SET(v, b)				( (v) & (0x01<<(b) ) )

#define SSV6200_USE_HW_WSID(_sta_idx)    ((_sta_idx == 0) || (_sta_idx == 1))

#define HW_MAX_RATE_TRIES   7

/**
* Define the number of entries of mac decision table.
*
* @ MAC_DECITBL1_SIZE: The decision filtering table size
* @ MAC_DECITBL2_SIZE: The decision mask table size
*/
#define MAC_DECITBL1_SIZE               16
#define MAC_DECITBL2_SIZE               9

#ifndef USE_GENERIC_DECI_TBL
extern u16 ap_deci_tbl[];
extern u16 sta_deci_tbl[];
#else
extern u16 generic_deci_tbl[];
#define ap_deci_tbl     generic_deci_tbl
#define sta_deci_tbl    generic_deci_tbl
#endif // USE_GENERIC_DECI_TBL

/**
 *
 */
#define HT_SIGNAL_EXT				    6		//For 2.4G
#define HT_SIFS_TIME        		    10
#define BITS_PER_BYTE           8
#define HT_RC_2_STREAMS(_rc)    ((((_rc) & 0x78) >> 3) + 1)
#define ACK_LEN				    (14)	//include fcs
#define BA_LEN                  (32)
#define RTS_LEN					(20)	//include fcs
#define CTS_LEN					(14)	//include fcs
#define L_STF                   8
#define L_LTF                   8
#define L_SIG                   4
#define HT_SIG                  8
#define HT_STF                  4
#define HT_LTF(_ns)             (4 * (_ns))
#define SYMBOL_TIME(_ns)        ((_ns) << 2) /* ns * 4 us */
#define SYMBOL_TIME_HALFGI(_ns) (((_ns) * 18 + 4) / 5)  /* ns * 3.6 us */

#define CCK_SIFS_TIME        10
#define CCK_PREAMBLE_BITS   144
#define CCK_PLCP_BITS        48

#define OFDM_SIFS_TIME        16
#define OFDM_PREAMBLE_TIME    20
#define OFDM_PLCP_BITS        22
#define OFDM_SYMBOL_TIME      4

#define HOUSE_KEEPING_TIMEOUT       100
#define MAX_RX_IDLE_INTERVAL  3
#define HOUSE_KEEPING_1_SEC			10
#define HOUSE_KEEPING_10_SEC	    100

/* These must match mac80211 skb queue mapping numbers */
#define WMM_AC_VO       0
#define WMM_AC_VI       1
#define WMM_AC_BE       2
#define WMM_AC_BK       3
#define WMM_NUM_AC      4

/* The maximal number of support TID */
#define WMM_TID_NUM     8

/**
*
*/
#define TXQ_EDCA_0      0x01
#define TXQ_EDCA_1      0x02
#define TXQ_EDCA_2      0x04
#define TXQ_EDCA_3      0x08
#define TXQ_MGMT        0x10

// used to adjust rssi
#define RSSI_SMOOTHING_SHIFT        5
#define RSSI_DECIMAL_POINT_SHIFT    6

// use for EDCA caculation
#define SSV_EDCA_SCALE  10
#define SSV_EDCA_FRAC(_val, _div) (((_val) << SSV_EDCA_SCALE) / _div)
#define SSV_EDCA_TRUNC(_val) ((_val) >> SSV_EDCA_SCALE)
#define GET_PRIMARY_EDCA(_sc)   SSV_EDCA_TRUNC(_sc->primary_edca_mib * 100)
#define GET_SECONDARY_EDCA(_sc) SSV_EDCA_TRUNC(_sc->secondary_edca_mib * 100)

#define HCI_START(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_start(__sh->hci.hci_ctrl); \
        })
#define HCI_STOP(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_stop(__sh->hci.hci_ctrl); \
        })
#define HCI_HCMD_START(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_hcmd_start(__sh->hci.hci_ctrl); \
        })
#define HCI_BLE_START(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_ble_start(__sh->hci.hci_ctrl); \
        })
#define HCI_BLE_STOP(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_ble_stop(__sh->hci.hci_ctrl); \
        })
#ifdef CONFIG_PM
#define HCI_SUSPEND(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_suspend(__sh->hci.hci_ctrl); \
        })
#define HCI_RESUME(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_resume(__sh->hci.hci_ctrl); \
        })
#endif
#define HCI_SEND(_sh, _sk, _q, _force_trigger) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_tx(__sh->hci.hci_ctrl, _sk, _q, _force_trigger, 0); \
        })
#define HCI_PAUSE_HWSWQ(_sh, _mk) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_tx_pause(__sh->hci.hci_ctrl, _mk); \
        })
#define HCI_RESUME_HWSWQ(_sh, _mk) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_tx_resume(__sh->hci.hci_ctrl, _mk); \
        })
#define HCI_TX_PAUSE_BY_STA(_sh, _txqid) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_tx_pause_by_sta(__sh->hci.hci_ctrl, _txqid); \
        })
#define HCI_TX_RESUME_BY_STA(_sh, _txqid) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_tx_resume_by_sta(__sh->hci.hci_ctrl, _txqid); \
        })
#define HCI_TXQ_FLUSH(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_txq_flush(__sh->hci.hci_ctrl); \
        })
#define HCI_BLE_TXQ_FLUSH(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_ble_txq_flush(__sh->hci.hci_ctrl); \
        })
#define HCI_HCMD_TXQ_FLUSH(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_hcmd_txq_flush(__sh->hci.hci_ctrl); \
        })
#define HCI_TXQ_FLUSH_BY_STA(_sh, _aid) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_txq_flush_by_sta(__sh->hci.hci_ctrl, _aid); \
        })
#define HCI_TXQ_LOCK_BY_STA(_sh,_txqid) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_txq_lock_by_sta(__sh->hci.hci_ctrl, _txqid); \
        })

#define HCI_TXQ_UNLOCK_BY_STA(_sh,_txqid) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_txq_unlock_by_sta(__sh->hci.hci_ctrl, _txqid); \
        })
#define HCI_TXQ_LEN(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_txq_len(__sh->hci.hci_ctrl); \
        })
#define HCI_TXQ_EMPTY(_sh, _txqid) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_txq_empty(__sh->hci.hci_ctrl, _txqid); \
        })
#define HCI_WAKEUP_PMU(_sh) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_pmu_wakeup(__sh->hci.hci_ctrl); \
        })
#define HCI_CMD_DONE(_sh, _data, _len) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_cmd_done(__sh->hci.hci_ctrl, _data, _len); \
        })
#define HCI_SEND_CMD(_sh, _sk) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_send_cmd(__sh->hci.hci_ctrl, _sk); \
        })
#define HCI_IGNORE_CMD(_sh, _val) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_ignore_cmd(__sh->hci.hci_ctrl, _val); \
        })
#define HCI_SET_CAP(_sh, _cap, _enable) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_set_cap(__sh->hci.hci_ctrl, _cap, _enable); \
        })
#define HCI_SET_TRIGGER_CONF(_sh, _en, _qlen, _pkt_size, _timeout) \
        ({ \
            typeof(_sh) __sh = _sh; \
            __sh->hci.hci_ops->hci_set_trigger_conf(__sh->hci.hci_ctrl, _en, _qlen, _pkt_size, _timeout); \
        })

#define SSV6XXX_SET_HW_TABLE(sh_, tbl_)                                 \
({                                                                      \
    int ret = 0;                                                        \
    u32 i=0;                                                            \
    for(; i<sizeof(tbl_)/sizeof(ssv_cabrio_reg); i++) {       \
        ret = SMAC_REG_WRITE(sh_, tbl_[i].address, tbl_[i].data);       \
        if (ret) break;                                                 \
    }                                                                   \
    ret;                                                                \
})

// define macro for STA/VIF using HW/SW cipher check

#define SSV6XXX_USE_HW_DECRYPT(_priv)       (_priv->has_hw_decrypt)    
#define SSV6XXX_USE_SW_DECRYPT(_priv)       (SSV6XXX_USE_LOCAL_SW_DECRYPT(_priv) || SSV6XXX_USE_MAC80211_DECRYPT(_priv)) 
#define SSV6XXX_USE_LOCAL_SW_DECRYPT(_priv)  (_priv->need_sw_decrypt)
#define SSV6XXX_USE_MAC80211_DECRYPT(_priv)  (_priv->use_mac80211_decrypt)

struct ssv_softc;

#define SSV6200_RX_HIGHEST_RATE             72


enum PWRSV_STATUS{
    PWRSV_DISABLE,
    PWRSV_ENABLE,
    PWRSV_PREPARE,
};

struct ssv6xxx_calib_table {
    u16 channel_id;
    u32 rf_ctrl_N;
    u32 rf_ctrl_F;
    u16 rf_precision_default;
};

struct ssv_hw;
struct ssv_sta_priv_data;
struct ssv_vif_priv_data;
struct ssv_sta_info;
struct ssv_vif_info;

int hw_update_watch_wsid(struct ssv_softc *sc, struct ieee80211_sta *sta, 
        struct ssv_sta_info *sta_info, int sta_idx, int rx_hw_sec, int ops);

void _set_wep_sw_crypto_key(struct ssv_softc *sc, struct ssv_vif_info *vif_info,
        struct ssv_sta_info *sta_info, void *param);

typedef enum
{
	CLK_SRC_OSC,
    CLK_SRC_RTC,
    CLK_SRC_SYNTH_80M,
    CLK_SRC_SYNTH_40M,    
    CLK_SRC_MAX,
} SSV_CLK_SRC;
/**
* struct ssv_hal - the structure for ssv6xxx HAL function.
*
* This structure is used to register HAL relation function
* for different chip.
*/
struct ssv_hal_ops {
    
    // MAC
    // initial rountine ----
    //adjust pre-set config
	void (*adj_config)(struct ssv_hw *sh);
	// need sw cipher?
	bool (*need_sw_cipher)(struct ssv_hw *sh);
    // init MAC
    int  (*init_mac)(struct ssv_hw *sh);
    // reset plf
    void  (*reset_sysplf)(struct ssv_hw *sh);
    // allocate HW
    struct ssv_hw * (*alloc_hw)(void);
    // init hw PHY and security table
    int  (*init_hw_sec_phy_table)(struct ssv_softc *sc);
    // write mac ini table value into HW
    int  (*write_mac_ini)(struct ssv_hw *sh);
    // check if ccmp/tkip use HW encryption
    bool (*use_hw_encrypt)(int cipher, struct ssv_softc *sc,
        struct ssv_sta_priv_data *sta_priv, struct ssv_vif_priv_data *vif_priv);
    // set rx flow
    int  (*set_rx_flow)(struct ssv_hw *sh, enum ssv_rx_flow type, u32 rxflow);
    // set rx ctrl flow
    int  (*set_rx_ctrl_flow)(struct ssv_hw *sh);
    // set mac addr
    int  (*set_macaddr)(struct ssv_hw *sh, int vif_idx);
    int  (*set_macaddr_2)(struct ssv_hw *sh, int vif_idx, u8 *macaddr);
    // set bssid
    int  (*set_bssid)(struct ssv_hw *sh, u8 *bssid, int vif_idx);
    // get chip time tag
    u64 (*get_ic_time_tag)(struct ssv_hw *sh);
    // get chip chip id
    void (*get_chip_id)(struct ssv_hw *sh);   
    // if need to check mac address 2
    bool (*if_chk_mac2)(struct ssv_hw *sh);
    // save hw status.
    void (*save_hw_status)(struct ssv_softc *sc);
    // restore hw config.
    void (*restore_hw_config)(struct ssv_softc *sc);
    // pll check
    void (*pll_chk)(struct ssv_hw *sh);
    // init gpio cfg
    void (*init_gpio_cfg)(struct ssv_hw *sh);
	// security-related-----
    // init wsid and get wsid. return wsid.
    int (*get_wsid)(struct ssv_softc *sc, struct ieee80211_vif *vif,
        struct ieee80211_sta *sta);
    // set mac to hw wsid
    void (*set_hw_wsid)(struct ssv_softc *sc, struct ieee80211_vif *vif,
        struct ieee80211_sta *sta, int wsid);
    // del hw wsid
    void (*del_hw_wsid)(struct ssv_softc *sc, int hw_wsid);
    // add fw wsid
    void (*add_fw_wsid)(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv,
        struct ieee80211_sta *sta, struct ssv_sta_info *sta_info);
    // del fw wsid
    void (*del_fw_wsid)(struct ssv_softc *sc, struct ieee80211_sta *sta,
        struct ssv_sta_info *sta_info);
    // enable fw wsid
    void (*enable_fw_wsid)(struct ssv_softc *sc, struct ieee80211_sta *sta,
        struct ssv_sta_info *sta_info, enum SSV6XXX_WSID_SEC key_type);
    // disable fw wsid
    void (*disable_fw_wsid)(struct ssv_softc *sc, int key_idx,
        struct ssv_sta_priv_data *sta_priv, struct ssv_vif_priv_data *vif_priv);
    // set hw wsid security type(hw or sw) to fw    
    void (*set_fw_hwwsid_sec_type)(struct ssv_softc *sc, struct ieee80211_sta *sta,
        struct ssv_sta_info *sta_info, struct ssv_vif_priv_data *vif_priv);
    // check if wep use hw cipher    
    bool (*wep_use_hw_cipher)(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv);    
    // check if wpa (ccmp/tkip) use hw cipher for pairwise   
    bool (*pairwise_wpa_use_hw_cipher)(struct ssv_softc *sc, 
        struct ssv_vif_priv_data *vif_priv, enum sec_type_en cipher,
        struct ssv_sta_priv_data *sta_priv);
    // check if wpa (ccmp/tkip) use hw cipher for group key   
    bool (*group_wpa_use_hw_cipher)(struct ssv_softc *sc, 
        struct ssv_vif_priv_data *vif_priv, enum sec_type_en cipher);
    // write aes tkip hw crypto group key
    void (*set_aes_tkip_hw_crypto_group_key) (struct ssv_softc *sc, struct ssv_vif_info *vif_info,
        struct ssv_sta_info *sta_info, void *param);                                               
    // write pairwise key to hw.
    int (*write_pairwise_key_to_hw) (struct ssv_softc *sc, int index, u8 algorithm, const u8 *key, int key_len, 
        struct ieee80211_key_conf *keyconf, struct ssv_vif_priv_data *vif_priv, struct ssv_sta_priv_data *sta_priv);        
    // write group key to hw.
    int (*write_group_key_to_hw) (struct ssv_softc *sc, int index, u8 algorithm, const u8 *key, int key_len, 
            struct ieee80211_key_conf *keyconf, struct ssv_vif_priv_data *vif_priv, struct ssv_sta_priv_data *sta_priv);
    // write pairwise key index to hw
    void (*write_pairwise_keyidx_to_hw)(struct ssv_hw *sh, int key_idx, int wsid);
    // write group key index to hw
    void (*write_group_keyidx_to_hw)(struct ssv_hw *sh, struct ssv_vif_priv_data *vif_priv, int key_idx);
    // write security key to hw.
    void (*write_key_to_hw)(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv, 
        void *data_ptr, int wsid, int key_idx, enum SSV6XXX_WSID_SEC key_type);
    // set group cipher type
    void (*set_group_cipher_type)(struct ssv_hw *sh, struct ssv_vif_priv_data *vif_priv, u8 cipher);
    // set pairwise cipher type
    void (*set_pairwise_cipher_type)(struct ssv_hw *sh, u8 cipher, u8 wsid);
    // check this interface if support hw bssid
    bool (*chk_if_support_hw_bssid)(struct ssv_softc *sc, int vif_idx);
    // change rx flow to cpu if dual i/f use both hw sw cipher for group key
    void (*chk_dual_vif_chg_rx_flow)(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv);
    // restore rx flow for group key delete
    void (*restore_rx_flow)(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv, struct ieee80211_sta *sta);
   // set wep hw crypto key
    void (*set_wep_hw_crypto_key)(struct ssv_softc *sc, struct ssv_sta_priv_data *sta_priv, struct ssv_vif_priv_data *vif_priv);    
	//set wep key    
    void (*set_wep_key) (struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv, struct ssv_sta_priv_data *sta_priv, 
        enum sec_type_en cipher, struct ieee80211_key_conf *key);
    // reserve skb for hw ccmp encrypt
    bool (*put_mic_space_for_hw_ccmp_encrypt) (struct ssv_softc *sc, struct sk_buff *skb);
    bool (*use_turismo_hw_encrpt)(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv);
#ifdef CONFIG_PM
	// save/clear trap reason
	void (*save_clear_trap_reason)(struct ssv_softc *sc);
    // restore trap reason
	void (*restore_trap_reason)(struct ssv_softc *sc);
	// save/reset rx flow
	void (*ps_save_reset_rx_flow)(struct ssv_softc *sc);
    // restore rx flow
	void (*ps_restore_rx_flow)(struct ssv_softc *sc);
    // pmu awake
	void (*pmu_awake)(struct ssv_softc *sc);
    // hold on3 power domain
	void (*ps_hold_on3)(struct ssv_softc *sc, int value);
#endif
    // misc related----
    // set replay ignore
    void (*set_replay_ignore)(struct ssv_hw *sh, u8 ignore);
    // update_decision_table #6 to val
    void (*update_decision_table_6)(struct ssv_hw *sh, u32 val);
    // update_decision_table
    int  (*update_decision_table)(struct ssv_softc *sc);
    // get fw version to regval
    void (*get_fw_version)(struct ssv_hw *sh, u32 *regval);
    // set mrx mode
    void (*set_mrx_mode)(struct ssv_hw *sh, u32 regval);
    // get mrx mode
    void (*get_mrx_mode)(struct ssv_hw *sh, u32 *regval);
    // set OP_MODE (STA/AP/IBSS/WDS)
    void (*set_op_mode)(struct ssv_hw *sh, u32 opmode, int vif_idx);
    // set The duration of burst SIFS (in us) of G mode 
    void (*set_dur_burst_sifs_g)(struct ssv_hw *sh, u32 val);
    // set The duration of slot time (in us) of G mode 
    void (*set_dur_slot)(struct ssv_hw *sh, u32 val);
    // set SIFS according to band
    void (*set_sifs)(struct ssv_hw *sh, int band);
    // set QOS on/off
    void (*set_qos_enable)(struct ssv_hw *sh, bool val);
    // set wmm parameters (aifs/cwin/txop
    void (*set_wmm_param)(struct ssv_softc *sc, 
        const struct ieee80211_tx_queue_params *params, u16 queue);
    void (*update_page_id)(struct ssv_hw *sh);
    // init tx cfg
    void (*init_tx_cfg)(struct ssv_hw *sh);
    // init rx cfg
    void (*init_rx_cfg)(struct ssv_hw *sh);
    // allocate pbuf
    u32 (*alloc_pbuf)(struct ssv_softc *sc, int size, int type);
    // free pbuf
    bool (*free_pbuf)(struct ssv_softc *sc, u32 pbuf_addr);
    // enable ampdu auto crc32
    void (*ampdu_auto_crc_en)(struct ssv_hw *sh);
    // set RX BA session
    void (*set_rx_ba)(struct ssv_hw *sh, bool on, u8 *ta,
        u16 tid, u16 ssn, u8 buf_size);
    // read efuse
    u8   (*read_efuse)(struct ssv_hw *sh, u8 *pbuf);
    // write efuse
    void (*write_efuse)(struct ssv_hw *sh, u8 *data, u8 data_length);
    // change clock source
    int (*chg_clk_src)(struct ssv_hw *sh);       
    // update rf table 
    void (*update_rf_table)(struct ssv_hw *sh);
#ifdef CONFIG_ENABLE_HOST_THERMAL    
    // do temperature compensation
    void (*do_temperature_compensation)(struct ssv_hw *sh);
#endif    
	// beacon related ----
	// beacon loss enable
	void (*beacon_loss_enable)(struct ssv_hw *sh);
	// beacon loss disable
	void (*beacon_loss_disable)(struct ssv_hw *sh);
	// beacon loss config
	void (*beacon_loss_config)(struct ssv_hw *sh, u16 beacon_interval, const u8 *bssid);
    // hci related ----
    // write txq_mask into hw
    void (*update_txq_mask)(struct ssv_hw *sh, u32 txq_mask);
    // read hci_inq_info from register
    void (*readrg_hci_inq_info)(struct ssv_hw *sh, int *hci_used_id, int *tx_use_page);
    // read txq_info from register
    bool (*readrg_txq_info)(struct ssv_hw *sh, u32 *txq_info, int *hci_used_id);
    // ssv_cmd related ----
    // dump wsid to console.
    bool (*dump_wsid)(struct ssv_hw *sh);
    // dump decision to console.
    bool (*dump_decision)(struct ssv_hw *sh);
	// get mbox output queue status
	u32 (*get_ffout_cnt)(u32 value, int tag);
	// get mbox input queue status
	u32 (*get_in_ffcnt)(u32 value, int tag);
    // read mbox output queue fifo count to value/value1/value2
    void (*read_ffout_cnt)(struct ssv_hw *sh, u32 *value, u32 *value1, u32 *value2);
    // read mbox input queue fifo count to value/value1
    void (*read_in_ffcnt)(struct ssv_hw *sh, u32 *value, u32 *value1);
    // read in id length threshold to value
    void (*read_id_len_threshold)(struct ssv_hw *sh, u32 *tx_len, u32 *rx_len);
    // read all id map
    void (*read_allid_map)(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3);
    // read tx id map
    void (*read_txid_map)(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3);
    // read rx id map
    void (*read_rxid_map)(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3);
    // read tag status to value
    void (*read_tag_status)(struct ssv_hw *sh, u32 *ava_status);
    // cli cmd mib
    void (*cmd_mib)(struct ssv_softc *sc, int argc, char *argv[]);
    // cli cmd power saving     
    void (*cmd_power_saving)(struct ssv_softc *sc, int argc, char *argv[]);
    // cmd for debug information.
    void (*cmd_hwinfo)(struct ssv_hw *sh, int argc, char *argv[]);
    // cmd for tx_gen packet
    void (*cmd_txgen)(struct ssv_hw *sh, u8 drate);
    // cmd for changing parameters for rf tools.
    void (*cmd_rf)(struct ssv_hw *sh, int argc, char *argv[]);  
    // cmd for efuse.
    void (*cmd_efuse)(struct ssv_hw *sh, int argc, char *argv[]);
      
    // cmd to change software limit for hardware queue.
    void (*cmd_hwq_limit)(struct ssv_hw *sh, int argc, char *argv[]);    
    // get RD_IDx address value array
    void (*get_rd_id_adr)(u32 *id_base_address);
    // burst(mutiple) read registers
    int (*burst_read_reg)(struct ssv_hw *sh, u32 *addr, u32 *buf, u8 reg_amount);
    // burst(mutiple) write registers
    int (*burst_write_reg)(struct ssv_hw *sh, u32 *addr, u32 *buf, u8 reg_amount);
    // auto_gen_nullpkt
    int (*auto_gen_nullpkt)(struct ssv_hw *sh, int hwq);
    // exec ssv_cmd cali
    void (*cmd_cali)(struct ssv_hw *sh, int argc, char *argv[]);
	//mcu enable
	void (*load_fw_enable_mcu)(struct ssv_hw *sh);
	//mcu disable
	int (*load_fw_disable_mcu)(struct ssv_hw *sh);
	//set fw status
	int (*load_fw_set_status)(struct ssv_hw *sh, u32 status);
	//get fw status
	int (*load_fw_get_status)(struct ssv_hw *sh, u32 *status);
	//pre config hw interfce
	void (*load_fw_pre_config_device)(struct ssv_hw *sh);
	//post config hw interfce
	void (*load_fw_post_config_device)(struct ssv_hw *sh);
	//reset cpu
	int (*reset_cpu)(struct ssv_hw *sh);
	//set sram mode
	void (*set_sram_mode)(struct ssv_hw *sh, enum SSV_SRAM_MODE mode);
	//get sram mode
	int (*get_sram_mode)(struct ssv_hw *sh);
    // enable usb accelerator
    void (*enable_usb_acc)(struct ssv_softc *sc, u8 epnum);
    // disable usb accelerator
    void (*disable_usb_acc)(struct ssv_softc *sc, u8 epnum);
    // set usb lpm enable/disable to hw
    void (*set_usb_lpm)(struct ssv_softc *sc, u8 enable);    
    // jump to rom
    int (*jump_to_rom)(struct ssv_softc *sc);
    // get default fw name
    void (*get_fw_name)(u8 *fw_name);
    //load fw
    int (*load_fw)(struct ssv_hw *sh, u8 *firmware_name, u8 openfile);
    // send tx poll to fw
    void (*send_tx_poll_cmd)(struct ssv_hw *sh, u32 type);
    // flash_read_all_map
    void (*flash_read_all_map)(struct ssv_hw *sh);
    // init hci rx aggr
    int (*init_hci_rx_aggr)(struct ssv_hw *sh);
    // reset hw mac
    int (*reset_hw_mac)(struct ssv_hw *sh);
    // xtal clk setting
    void (*set_crystal_clk)(struct ssv_hw *sh);
    // wait usb rom code ready
    void (*wait_usb_rom_ready)(struct ssv_hw *sh);
    // detach usb hci
    void (*detach_usb_hci)(struct ssv_hw *sh);
    
    // PHY
    // tx descriptor related ----
    // add_txinfo :add a new tx descriptor
    void (*add_txinfo) (struct ssv_softc *sc, struct sk_buff *skb);
    // update_txinfo : setup tx descriptor
    void (*update_txinfo)(struct ssv_softc *sc, struct sk_buff *skb);
    // add txinfo for ampdu
    void (*update_ampdu_txinfo)(struct ssv_softc *sc, struct sk_buff *ampdu_skb);
    // add txinfo for ampdu
    void (*add_ampdu_txinfo)(struct ssv_softc *sc, struct sk_buff *ampdu_skb);
    // update null func txinfo
    int (*update_null_func_txinfo)(struct ssv_softc *sc, struct ieee80211_sta *sta, struct sk_buff *skb);
    // return tx desciptor size
    int (*get_tx_desc_size)(struct ssv_hw *sh);
    // get tx_desc->c_type from tx descriptor
    int (*get_tx_desc_ctype)(struct sk_buff *skb );
    // get tx_desc->reason from tx descriptor
    int (*get_tx_desc_reason)(struct sk_buff *skb );
    // get tx_desc->wsid from tx descriptor
    int (*get_tx_desc_wsid)(struct sk_buff *skb );
    // get tx_desc->txq_idx from tx descriptor
    int (*get_tx_desc_txq_idx)(struct sk_buff *skb );    
    // txtput_m2_set_tx_desc
    void (*txtput_set_desc)(struct ssv_hw *sh, struct sk_buff *skb ); 
    // fill tx desc for beacon
    void (*fill_beacon_tx_desc)(struct ssv_softc *sc, struct sk_buff* beacon_skb);  
    // rx descriptor related ----
    // get rx_desc->reason from rx descriptor
    int (*get_tkip_mmic_err)(struct sk_buff *skb);    
    // get rx desciptor size (include rx phy info)    
    int (*get_rx_desc_size)(struct ssv_hw *sh);
    // get rx desc length (without rx phy info)
    int (*get_rx_desc_length)(struct ssv_hw *sh);
    // get rxdesc wsid
    u32 (*get_rx_desc_wsid)(struct sk_buff *skb);
    // get rxdesc rate index
    u32 (*get_rx_desc_rate_idx)(struct sk_buff *skb);
    // get rxdesc mng_used
    u32 (*get_rx_desc_mng_used)(struct sk_buff *skb);
    // get rxdesc rx report
    bool (*is_rx_aggr)(struct sk_buff *skb);
    // get rx packet type = rxdesc->c_type;
    u32 (*get_rx_desc_ctype)(struct sk_buff *skb); 
    // get rx packet type = rxdesc->hdr_offset;
    int (*get_rx_desc_hdr_offset)(struct sk_buff *skb); 
    // get rx desc info without skb: packet_len, ctype, tx_pktno
    void (*get_rx_desc_info_hdr)(unsigned char *desc, u32 *packet_len, u32 *ctype,
        u32 *_tx_pkt_run_no);
    // get rex desc phy rssi
    u32 (*get_rx_desc_phy_rssi)(struct sk_buff *skb);
    // rx frame filter
    bool (*nullfun_frame_filter)(struct sk_buff *skb);
    // get txdesc's sw_ack_ctl 
    u8 (*get_sw_ack_ctl)(struct ssv_hw *sh, struct sk_buff *skb);
    // get txdesc's sw_ack_seq 
    u8 (*get_sw_ack_seq)(struct ssv_hw *sh, struct sk_buff *skb);
    // common phy ---
    // set default phy mode without enable when on , clear when off
    void (*set_phy_mode)(struct ssv_hw *sh, bool val);
    // enable/disable phy
    void (*phy_enable)(struct ssv_hw *sh, bool val);
	// set rx secondary edcca cfg
	void (*edca_enable)(struct ssv_hw *sh, bool val);
	// get rx secondary edcca stat
	void (*edca_stat)(struct ssv_hw *sh);
#ifdef	CONFIG_ENABLE_ACS_FUNC
	// update survey information from rx secondary edcca stat
	void (*edca_update_survey)(struct ssv_hw *sh);
#endif	
	// enable/disable rx fixed gain
	void (*rx_fixed_gain_enable)(struct ssv_hw *sh, bool enable);
    // ssv_cmd related ----
    // reset mib phy part
    void (*reset_mib_phy)(struct ssv_hw *sh);
    // dump rx_mib phy part
    void (*dump_mib_rx_phy)(struct ssv_hw *sh);
    // rate control related---
	// set rate control algorithm
	void (*rc_algorithm)(struct ssv_softc *sc);
	// set max rate and rate retry 
	void (*set_80211_hw_rate_config)(struct ssv_softc *sc);
	// legacy bitrate to hw rate desc
	void (*rc_legacy_bitrate_to_rate_desc)(int bitrate, u8 *drate);
	// rate control rx data
	void (*rc_rx_data_handler)(struct ssv_softc *sc, struct sk_buff *skb, u32 rate_index);
	// ps event handler	
	void (*ps_evt_handler)(struct ssv_softc *sc, struct sk_buff *skb);
    // fill mac80211 chan info
    void (*update_mac80211_chan_info)(struct ssv_softc *sc, 
            struct sk_buff *skb, struct ieee80211_rx_status *rxs);
    // fill mac80211 rx status
    void (*rc_mac80211_rate_idx)(struct ssv_softc *sc, 
            int hw_rate_idx, struct ieee80211_rx_status *rxs);
    // fill mac80211 tx info
    void (*rc_mac80211_tx_rate_idx)(struct ssv_softc *sc,
            int hw_rate_idx, struct ieee80211_tx_info *tx_info);
    // Update rx status(RSSI, rx rate statistics)
    int  (*update_rxstatus)(struct ssv_softc *sc, 
            struct sk_buff *rx_skb, struct ieee80211_rx_status *rxs);
    // cmd for change fix rate parameters.
    void (*cmd_rc)(struct ssv_hw *sh, int argc, char *argv[]);
    //AMPDU
    // ampdu rx start
    int (*ampdu_rx_start)(struct ieee80211_hw *hw, struct ieee80211_vif *vif, struct ieee80211_sta *sta, 
            u16 tid, u16 *ssn, u8 buf_size);

    // BB RF		
    // load PHY ini table pointer
    void (*load_phy_table)(ssv_cabrio_reg **phy_table);
    // get phy table size
    u32 (*get_phy_table_size)(struct ssv_hw *sh);
    // load RF ini table pointer
    void (*load_rf_table)(ssv_cabrio_reg **rf_table);
    // get RF table size
    u32 (*get_rf_table_size)(struct ssv_hw *sh);
    // init PLL
    void (*init_pll)(struct ssv_hw *sh);
    // set channel freq to RF
    int (*set_channel)(struct ssv_softc *sc, struct ieee80211_channel *channel, enum nl80211_channel_type, bool offchan);
    // write pht rf ini table to HW and initial pll.
    int  (*set_pll_phy_rf)(struct ssv_hw *sh, ssv_cabrio_reg *rf_tbl, ssv_cabrio_reg *phy_tbl);
    // set RF on/off
    bool (*set_rf_enable)(struct ssv_hw *sh, bool val);
    // ssv_command related
    // dump phy_register to console.
    bool (*dump_phy_reg)(struct ssv_hw *sh);
    // dump rf_register to console.
    bool (*dump_rf_reg)(struct ssv_hw *sh);
    // return true/false for support iqk cmd or not
    bool (*support_iqk_cmd)(struct ssv_hw *sh);
    // set on3 on/off
    void (*set_on3_enable)(struct ssv_hw *sh, bool val);
#ifdef CONFIG_ENABLE_HOST_THERMAL
    // get chip temprature
    int (*get_temp)(struct ssv_hw *sh, int *val);
#endif

#ifdef CONFIG_STA_BCN_FILTER
    void (*set_mrx_filter)(struct ssv_hw *sh, u8 ID, bool enable, u16 rule_bitmap);
#endif
};

struct ssv_hw_cfg {
    u32 addr;
    u32 value;
    struct list_head list;
};

//define for turismo PADPD use
#define     PADPDBAND   5
#define     MAX_PADPD_TONE  26

struct ssv6006dpd{
    u32     am[MAX_PADPD_TONE/2];
    u32     pm[MAX_PADPD_TONE/2];
};

#define NORMAL_PWR      0
#define ENHANCE_PWR     1
#define GREEN_PWR       1

struct ssv6006_padpd{
    bool    dpd_done[PADPDBAND];
    bool    dpd_disable[PADPDBAND];    
    bool    pwr_mode;           /* 0: normal mode, 1: enhance_mode*/
    bool    spur_patched;
    u8      current_band;
    struct  ssv6006dpd val[PADPDBAND];
    u8      bbscale[PADPDBAND];
};

struct ssv6006_cal_result{
    bool cal_done;
    bool cal_iq_done[PADPDBAND];
    u32 rxdc_2g[21];
    u8  rxrc_bw20;
    u8  rxrc_bw40;
    u8  txdc_i_2g;
    u8  txdc_q_2g;
    u32 rxdc_5g[21];
    u8  rxiq_alpha[PADPDBAND];
    u8  rxiq_theta[PADPDBAND];
    u8  txdc_i_5g;
    u8  txdc_q_5g;
    u8  txiq_alpha[PADPDBAND];
    u8  txiq_theta[PADPDBAND];       
};

struct ssv_tempe_table {
    u8  g_band_gain[7];
    u8  a_band_gain[4];
    u16 xtal_offset;
};

struct ssv_flash_config {
    bool exist; 
    struct ssv_tempe_table rt_config;
    struct ssv_tempe_table ht_config;
    struct ssv_tempe_table lt_config;
    // xtal offset temperature boundary
    u8  xtal_offset_tempe_state;
    u8  xtal_offset_low_boundary;
    u8  xtal_offset_high_boundary;
    // band gain temperature boundary
    u8  band_gain_tempe_state;
    u8  band_gain_low_boundary;
    u8  band_gain_high_boundary;
    // chan
    int chan;
    // dcdc on/off
    u16 dcdc;
    // padpd on/off
    u16 padpd;
    // 2.4G PA BIAS
    u32 g_band_pa_bias0;
    u32 g_band_pa_bias1;
    // 5G PA BIAS
    u32 a_band_pa_bias0;
    u32 a_band_pa_bias1;
    // rate delta
    u8 rate_delta[13]; // 0 => 11b, 1 => ofdm 6/9m .... 12 => ht40 mcs6/7
};

#ifdef CONFIG_ENABLE_HOST_THERMAL
struct hw_temp_compensation_tem_item{
    int temp;
    u32 reg_count;
    u32 reserved;
};

struct hw_temp_compensation_reg_item{
    u32 reg;
    u32 vaule;
    u32 reserved;
};

#define MAX_LENTH_OF_TABLE_COMPENSATION 8
#define MAX_REG_COUNT_OF_TABLE_COMPENSATION 8

struct hw_temp_compensation_config {
     u8 hw_temp_boundary_levels;
     struct hw_temp_compensation_tem_item hw_temp_items[MAX_LENTH_OF_TABLE_COMPENSATION + 1];
     struct hw_temp_compensation_reg_item hw_tem_reg_items[MAX_LENTH_OF_TABLE_COMPENSATION + 1][MAX_REG_COUNT_OF_TABLE_COMPENSATION];
     u8 hw_temp_state;
     bool exist;
};
#endif

/**
* struct ssv_hw - the structure for ssv6200 hardware information.
*
* This structure is shared between ssv6200 hw/sw mac & HCI/SDIO
* drivers. hw/sw mac registers this structure to HCI/SDIO.
*/
struct ssv_hw {
    struct ssv_softc *sc; /* back point to ssv_softc */
    struct ssv6xxx_platform_data *priv;
    struct ssv6xxx_hci_info hci;

    char   chip_id[SSV6XXX_CHIP_ID_LENGTH];
    u64    chip_tag;

    struct ssv_hal_ops hal_ops;

//    u32 hw_caps;
//    u8 mac_addr[6];
//    int default_channel;

    // efuse category bitmap
    u32 efuse_bitmap;

    /* parameter settings for ssv6200 mac */
    u32 tx_desc_len; /* include tx_phy_info length = 0 */
    u32 rx_desc_len; /* include rx_phy_info length  */ 
    u32 rx_pinfo_pad; /* after the payload */

    u32 tx_page_available;
    u32 total_tx_rx_page;
    u32 ampdu_divider;

	struct ssv6xxx_tx_hw_info tx_info;
	struct ssv6xxx_rx_hw_info rx_info;
    
    //Store the data of secure table.
    u8 hw_sec_key_data[1024]; //must greater than 2*BSSID(140 bytes) + 8*WSID(44 bytes)

    /**
     * ssv6200 hardware configuration from external module,
     * such as flash/eeprom...,etc.
     */
    struct ssv6xxx_cfg cfg;
    struct ssv_flash_config flash_config;
#ifdef	CONFIG_ENABLE_HOST_THERMAL
    struct hw_temp_compensation_config htc_config;
#endif	
    struct st_rf_table rf_table;

    /* # of MAC addresses this device supports */
    u32    n_addresses;
    /* all mac addresses that this device support */
    struct mac_address maddr[SSV6200_MAX_HW_MAC_ADDR];

    //For restore channel 1-12 default value
    struct ssv6xxx_ch_cfg *p_ch_cfg;
    u32 ch_cfg_size;

	//for rx
    int rx_mode;

    u8  *page_count; /* Pages allocated by host. Index by packet ID */

    // default tx _power
    u8  default_txgain[PADPDBAND];
};


/**
* struct ssv_tx - tx queue for outgoing frames through interface.
* Each AC queue uniquely associates with a hardware tx queue.
*/
struct ssv_tx {
    u16 seq_no;
    int hw_txqid[WMM_NUM_AC];
    int ac_txqid[WMM_NUM_AC];

    /* statistical counters: */
    u32 tx_pkt[SSV_HW_TXQ_NUM];
    u32 tx_frag[SSV_HW_TXQ_NUM];

    u32 tx_count;
    u32 mgmt_count;
    u32 data_count;
    u32 auth_count;
    u32 deauth_count;
    u32 assoc_req_count;
    u32 assoc_resp_count;
    u32 probe_req_count;
    u32 probe_resp_count;
};



/**
* struct ssv_rx, mib count 
*/
struct ssv_rx {
    u32 rx_count;
    u32 rx_data_count;
    u32 rx_mgmt_count;
    u32 rx_bcn_count;
    u32 rx_proberesp_count;
    u32 rx_probereq_count;
    u32 rx_assoc_req_count;
    u32 rx_assoc_resp_count;
    u32 rx_auth_count;
    u32 rx_disassoc_count;
    u32 rx_deauth_count;
};


/* Macros for struct ssv_sta_info */
#define SSV6XXX_GET_STA_INFO(_sc, _s) \
    &(_sc)->sta_info[((struct ssv_sta_priv_data *)((_s)->drv_priv))->sta_idx]



/**
* Constatnt value defined for s_flag which indicates a station's 
* current capabilities.
*/
#define STA_FLAG_VALID                  0x00001
#define STA_FLAG_QOS                    0x00002
#define STA_FLAG_AMPDU                  0x00004
#define STA_FLAG_ENCRYPT                0x00008
#define STA_FLAG_AMPDU_RX               0x00010


/**
* struct ssv_sta_info - structure to hold station info.
*/
struct ssv_sta_info {
    u16 aid;
    u16 s_flags;

    int hw_wsid;    /* -1: only software */
    
    struct ieee80211_sta *sta;
    struct ieee80211_vif *vif;

    bool tim_set;
    #ifdef CONFIG_SSV6XXX_DEBUGFS
    struct dentry *debugfs_dir;
	#endif // CONFIG_SSV6XXX_DEBUGFS
    s8 signal;
};


/**
* struct ssv_vif_info - structure to hold vif info.
*/
struct ssv_vif_info {
    struct ieee80211_vif      *vif;
    struct ssv_vif_priv_data  *vif_priv;
    enum                       nl80211_iftype if_type;
//    u8                         bssid[6];

    struct ssv6xxx_hw_sec      sramKey;

    #ifdef CONFIG_SSV6XXX_DEBUGFS
    struct dentry             *debugfs_dir;
    #endif // CONFIG_SSV6XXX_DEBUGFS
};

struct rx_stats {
    u64 n_pkts[8];
    u64 g_pkts[8];
    u64 cck_pkts[4];
    u8  phy_mode;
    u8  ht40;
};

/* struct ssv_sta_priv_data - private data structure for ieee80211_*/

#define SSV_AMPDU_1_3_TX_STOP                   0x0
#define SSV_AMPDU_1_3_TX_PROGRESS               0x1
#define SSV_AMPDU_1_3_TX_START                  0x2

struct ssv_sta_priv_data {
    int                                sta_idx;
    struct ssv_sta_info               *sta_info;

    struct list_head                   list; // Hook to VIF's STA list.
    
    spinlock_t                         ampdu_ctrl_lock;              // for AMPDU 1.3
    int                                ampdu_tid_state[WMM_TID_NUM]; // for AMPDU 1.3
    bool                               has_hw_encrypt;    // Pairwise frame has hardware encryptor
    bool                               need_sw_encrypt;   // Pairwise frame needs software for encription
    bool                               has_hw_decrypt;    // Pairwise frame has hardware decryptor
    bool                               need_sw_decrypt;   // Pairwise frame needs software for decription
    bool                               use_mac80211_decrypt;  // use mac80211 to decrypt to decrypt group frame
    bool                               unicast_key_changed;
    u8                                 group_key_idx;
    bool                               wep_key_update;

    u32  beacon_rssi;
    struct rx_stats                     rxstats;
    u64 last_pn;
};

struct wep_hw_key {
    u8 keylen;
    u8 key[SECURITY_KEY_LEN];
};

/**
 * struct ssv_vif_priv_data - data per Virtual Interface, it is a MAC context
 * @id: between 0 and 1
 *
 */
struct ssv_vif_priv_data {
    int                      vif_idx;

    //u16                      id;
    // struct ssv_sta_info      sta_info[SSV_NUM_STA];
    struct list_head         sta_list; // STA list added to this VIF.

    u32                      sta_asleep_mask;

    u32                      pair_cipher;  // Security type for unicast data of this VIF.
    u32                      group_cipher; // Security type for multicast data of this VIF.

    bool                     is_security_valid; // For VIF-wide security.
    bool                     has_hw_encrypt;    // Group frame has hardware encryptor
    bool                     need_sw_encrypt;   // Group frame needs software for encription
    bool                     has_hw_decrypt;    // Group frame has hardware decryptor
    bool                     need_sw_decrypt;   // Group frame needs software for decription
    bool                     use_mac80211_decrypt;  // use mac80211 to decrypt to decrypt group frame
    bool                     force_sw_encrypt; // Freddie ToDo: temporary solution for AP mode.
    bool                     group_key_changed;
    u8                       group_key_idx;

    s8                       wep_idx;         // Current wep key index, it is used for HW or SW encryption (one of them)
    int                      wep_cipher;      // Current wep cipher
    u64                      last_pn_mcast;

}; // struct ssv_vif_priv_data

/**
* Constant values defined for sc_flags which indicates the 
* current status of WiFi driver.
*
* @ SC_OP_DEV_READY: This flag should be set to indicate device is ready to use
* @ SC_OP_HW_RESET
*
*/
#define SC_OP_DEV_READY             BIT(0)
#define SC_OP_HW_RESET              BIT(1)
#define SC_OP_OFFCHAN               BIT(2)
#define SC_OP_FIXED_RATE            BIT(3)
#define SC_OP_SHORT_PREAMBLE        BIT(4)
#define SC_OP_CTS_PROT              BIT(5)
#define SC_OP_DIRECTLY_ACK          BIT(6)
#define SC_OP_BLOCK_CNTL            BIT(7)
#define SC_OP_CHAN_FIXED            BIT(8)
#define SC_OP_IF_UP                 BIT(9)
#define SC_OP_FLOWCTL               BIT(10)
#define SC_OP_STA_DEAUTH            BIT(11)
#define SC_OP_MONITOR               BIT(12)

#define IS_ALLOW_SCAN                    (sc->p2p_status)
#define IS_NON_AP_MODE                   (sc->ap_vif == NULL)
#define IS_NONE_STA_CONNECTED_IN_AP_MODE (list_empty(&((struct ssv_vif_priv_data *)sc->ap_vif->drv_priv)->sta_list))
#define IS_MGMT_AND_BLOCK_CNTL(_sc, _hdr) ((sc->sc_flags & SC_OP_BLOCK_CNTL) &&  (ieee80211_is_mgmt(_hdr->frame_control)))
#define IS_P2P_PS(_vif, _hdr) \
            (_vif->p2p && ieee80211_is_nullfunc(_hdr->frame_control) && ieee80211_has_pm(_hdr->frame_control))

#define SSV6200_MAX_BCAST_QUEUE_LEN 16
struct ssv6xxx_bcast_txq {	
    spinlock_t txq_lock;
    struct sk_buff_head qhead;       
    int cur_qsize;         
};

struct _ssv6xxx_txtput{
    struct task_struct *txtput_tsk;
    struct sk_buff *skb;
    u32 size_per_frame;
    u32 loop_times;
    u32 occupied_tx_pages;
};

struct ssv_timer_list {
	struct timer_list timer;
	void (*function)(unsigned long argv);
	void *arg;
};

/**
* struct ssv_softc - hold the whole wifi driver data structure.
*
*/
struct ssv_softc {
    struct ieee80211_hw    *hw;
    struct device          *dev;
    struct platform_device *platform_dev;

    //Force disable directly ack tx frame
    bool force_disable_directly_ack_tx;

    //Force reset
    u32 restart_counter;
    bool force_triger_reset;
    struct work_struct hw_restart_work;

    unsigned long   sdio_throughput_timestamp;
    unsigned long   sdio_rx_evt_size;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0)
    struct ieee80211_supported_band sbands[NUM_NL80211_BANDS];
#else
    struct ieee80211_supported_band sbands[IEEE80211_NUM_BANDS];
#endif
    struct ieee80211_channel *cur_channel;
    u16 hw_chan;
    u16 hw_chan_type;

#ifdef CONFIG_ENABLE_ACS_FUNC
	/* for survey info */
	struct survey_info survey[MAX_AVAIL_CHANNEL];
	u16 survey_cur_chan;
#endif

    /* for sc configuration */
    struct mutex mutex;
    
    /* hardware configuration: */
    struct ssv_hw *sh;

    struct ssv_tx tx;
    struct ssv_rx rx;

    //struct ieee80211_vif *vif_ptr[SSV_NUM_VIF];
    struct ssv_vif_info vif_info[SSV_NUM_VIF];

    struct ssv_sta_info sta_info[SSV_NUM_STA];

    /*just allow one AP vif in the same time */
    struct ieee80211_vif *ap_vif;
    //struct ieee80211_vif *vif[SSV6200_MAX_VIF];
    u8     nvif;

    u32 sc_flags;

    /* mac80211 state*/
    bool mac80211_dev_started;  // device is started. It corresponds to to parameter "started" in struct ieee80211_local

    /**
     * For fixed rate, max_rate_idx is the fixed rate to use.
     * For auto rate, max_rate_idx is the auto rate.
     */
    int max_rate_idx;


    /*  security algorithm  the variable just use only for wep mode now. 
     *  we just keep the algorithm of pairwise key or group key.
     */
    //u8 algorithm;
    //u8 group_key_idx;
    
    //bool bSecurity_wapi;

    /* ssv6200 mac decision table */
    u16 *mac_deci_tbl;

    /* hanlde mac80211 interface work queue*/
    struct workqueue_struct *config_wq;

//---------------------------------------------------------
    /* Beacon related function*/
    u16 beacon_interval;
    u8 beacon_dtim_cnt;			//Maxium DTIM counter number, not DTIM
    //cache latest beacon
    struct sk_buff *beacon_buf; 
    // beacon miss handler
    struct work_struct	beacon_miss_work;

    /* Broadcast related */
    struct work_struct	bcast_start_work;
    struct delayed_work	bcast_stop_work;
    struct delayed_work	bcast_tx_work;

    bool aid0_bit_set;
    u8 hw_mng_used;
    // u32 sta_asleep_mask;
    struct ssv6xxx_bcast_txq bcast_txq;
    int bcast_interval;

    /*=================================================================*/
    /*  <= Hardware related resource end*/
    /*=================================================================*/
    u8 bssid[2][6];

    /* for asic pbuf allocate/release*/
    struct mutex mem_mutex;
	
    /* Protect power save state */
    spinlock_t ps_state_lock; 			
	
    /* Station mode related*/
    //u8 sta_count;
    u8 hw_wsid_bit;

    /* CCMP header source select*/
    bool ccmp_h_sel;

    #if 0
	// CCMP encryption in SSV6xxx driver.
    struct work_struct ampdu_tx_encry_work;
    bool ampdu_encry_work_scheduled;
    bool ampdu_ccmp_encrypt;
    // Sync key to HW encrption engine when mixing software and hardware encryption in one session.
    struct work_struct sync_hwkey_work;
    bool sync_hwkey_write;
    struct ssv_sta_info *key_sync_sta_info;

    AMPDU_REKEY_PAUSE_STATE  ampdu_rekey_pause;
    #endif // 0
    /* ampdu rx related*/
    int rx_ba_session_count;
    struct ieee80211_sta *rx_ba_sta;
    u8  rx_ba_bitmap;
    u8 	ba_ra_addr[ETH_ALEN];
    u16 ba_tid;
    u16 ba_ssn;
    struct work_struct set_ampdu_rx_add_work;
	struct work_struct set_ampdu_rx_del_work;


    bool isAssoc;
    u16 channel_center_freq;		//The channel we use in station mode    
    /*
    * p2p_status:
    * special flag for RK 3036 DLNA mode. 
    * 1: indicate softap is started and allow to scan
    */    
    u8 p2p_status;
    bool bScanning;
    bool boffchan;
    
    /*power saving*/
    int ps_status;
    u16 ps_aid; 
    u8  ps_event_cnt;
#ifdef CONFIG_SSV_SUPPORT_ANDROID
#ifdef CONFIG_HAS_EARLYSUSPEND
    struct early_suspend early_suspend;
#endif //CONFIG_HAS_EARLYSUSPEND
#ifdef CONFIG_HAS_WAKELOCK
    struct wake_lock ssv_wake_lock_;
#endif /* WAKELOCK */

#endif

    // TX handling thread
    struct sk_buff_head tx_done_q;
    struct sk_buff_head tx_ack_ctl_q;

    // RX handling thread
    wait_queue_head_t   rx_wait_q;
    struct sk_buff_head rx_skb_q;
    struct task_struct *rx_task;

    /**
        * MAC debug counters:
        */
    bool dbg_rx_frame;
    bool dbg_tx_frame;

#ifdef CONFIG_SSV6XXX_DEBUGFS
    /*
     * DebugFS
     */
    struct dentry *debugfs_dir;
    // struct dentry *vif_debugfs_dir;
#endif // CONFIG_SSV6XXX_DEBUGFS

    /* 
     * Debug Desc
     */
    u32 log_ctrl;

    // for ssv_cmd.
    struct ssv_cmd_data cmd_data;

    struct ssv6006_padpd dpd;
    
    // for tx desc
    u8 tx_pkt_run_no;
    spinlock_t tx_pkt_run_no_lock;

    // Wait for firmware ready
    wait_queue_head_t   fw_wait_q;
    u32                 iq_cali_done; 

    // sta_info protection
    struct rw_semaphore sta_info_sem;

#ifdef CONFIG_SSV_CTL
    u32 ssv_smartlink_status;
    u32 ssv_usr_pid;
#endif

    u8  ack_counter;
    // house keeping
    struct ssv_timer_list house_keeping;
    /* hanlde house keeping work queue*/
    struct workqueue_struct *house_keeping_wq;

    //check fw status
    struct work_struct check_fw_status_work;
    int fw_cur_status_idle_time;
    u32 fw_cur_status_cnt;
    u32 fw_pre_status_cnt;

    //edca mib
    struct work_struct mib_edca_work;
    int primary_edca_mib;
    int secondary_edca_mib;
    u32 default_edcca;

    // tx poll 
    struct work_struct tx_poll_work;
        
    // flow control 
    struct work_struct flowctl_work;
	u32 flowctl_frame_cnt;
	u32 flowctl_hci_cnt;

    //txtput
    struct _ssv6xxx_txtput ssv_txtput;
    // directack tx frame counter    
    atomic_t tx_frame;

    bool rx_data_exist;

    //WoW
    struct completion	wakeup_done;
    struct completion	hold_on3;

    // handle tx done 
    struct workqueue_struct *tx_done_wq;
    //tx done process
    struct work_struct tx_done_work;
    //only for ampdu stop
    struct mutex ampdu_stop_mutex;

#ifdef CONFIG_STA_BCN_FILTER
    bool sta_bcn_filter;
#endif

    u32 hw_cur_rate[SSV_NUM_STA];

    /* hw scan */   
    bool hw_scan_start;
    bool hw_scan_cancel;
    bool hw_scan_done;
    struct ieee80211_channel *hw_scan_restore_channel;
    u16 hw_scan_restore_chan_type;
    struct ssv_scan_param scan_param;
    struct cfg80211_scan_request *scan_req;
    struct ieee80211_vif *scan_vif;
    struct workqueue_struct *scan_wq;
    struct delayed_work scan_work;
    struct delayed_work scan_ignore_work;
    bool scan_is_passive;
    u8 scan_wsid;
    u8 next_hw_scan_state;
    u8 scan_channel_idx;
    unsigned long scan_time;
    unsigned long scan_wait_period;
    u8 scan_ie_band2g[512];
    u16 scan_ie_len_band2g;
    u8 scan_ie_band5g[512];
    u16 scan_ie_len_band5g;

#ifdef	CONFIG_ENABLE_HOST_THERMAL
    bool thermal_monitor_enable;
    int thermal_monitor_counter;
    struct work_struct thermal_monitor_work;
#endif
    bool bAdapt;
};

enum {
    IQ_CALI_RUNNING,
    IQ_CALI_OK,
    IQ_CALI_FAILED
};
//typedef struct cfg_host_event HDR_HostEvent;

void ssv6xxx_cancel_work_sync(struct ssv_softc *sc);

void ssv6xxx_txbuf_free_skb(struct sk_buff *skb , void *args);

void ssv6200_rx_process(struct work_struct *work);
int ssv6200_rx(struct sk_buff *rx_skb, void *args);
int ssv6200_is_rx_q_full(void *args);
void ssv6xxx_post_tx_cb(struct sk_buff_head *skb_head, void *args);
void ssv6200_tx_flow_control(struct ssv_softc *sc, bool fc_en, bool force_stop);
void ssv6xxx_hci_update_flowctl_cb(void *args);

int ssv6xxx_get_channel(struct ssv_softc *sc, int *pch);
int ssv6xxx_set_promisc(struct ssv_softc *sc, int accept);
int ssv6xxx_get_promisc(struct ssv_softc *sc, int *paccept);

int ssv6xxx_rx_task (void *data);
void ssv6xxx_tx_done_process(struct work_struct *work);

/* house keeping & related work */
void ssv6xxx_house_keeping(unsigned long argv);
void ssv6xxx_check_fw_status_process(struct work_struct *work);
void ssv6xxx_mib_edca_process(struct work_struct *work);
void ssv6xxx_tx_poll_process(struct work_struct *work);
void ssv6xxx_flowctl_process(struct work_struct *work);
void ssv6xxx_ps_callback_func(unsigned long data);
void ssv6xxx_enable_ps(struct ssv_softc *sc);
void ssv6xxx_disable_ps(struct ssv_softc *sc);
int ssv6xxx_skb_encrypt(struct sk_buff *mpdu,struct ssv_softc *sc);
int ssv6xxx_skb_decrypt(struct sk_buff *mpdu, struct ieee80211_sta *sta,struct ssv_softc *sc);
void ssv6200_sync_hw_key_sequence(struct ssv_softc *sc, struct ssv_sta_info* sta_info, bool bWrite);
struct ieee80211_sta *ssv6xxx_find_sta_by_rx_skb (struct ssv_softc *sc, struct sk_buff *skb);
struct ieee80211_sta *ssv6xxx_find_sta_by_addr (struct ssv_softc *sc, u8 addr[6]);
void ssv6xxx_foreach_sta (struct ssv_softc *sc,
                          void (*sta_func)(struct ssv_softc *,
                                           struct ssv_sta_info *,
                                           void *),
                          void *param);
void ssv6xxx_foreach_vif_sta (struct ssv_softc *sc,
                              struct ssv_vif_info *vif_info,
                              void (*sta_func)(struct ssv_softc *,
                                               struct ssv_vif_info *,
                                               struct ssv_sta_info *,
                                               void *),
                              void *param);

#ifdef CONFIG_SSV_SUPPORT_ANDROID
#ifdef CONFIG_HAS_EARLYSUSPEND
void ssv6xxx_early_suspend(struct early_suspend *h);
void ssv6xxx_late_resume(struct early_suspend *h);
#endif //CONFIG_HAS_EARLYSUSPEND
#endif

#ifdef CONFIG_SSV6XXX_DEBUGFS
ssize_t ssv6xxx_tx_queue_status_dump (struct ssv_softc *sc, char *status_buf,
                                      ssize_t buf_size);
#endif // CONFIG_SSV6XXX_DEBUGFS

#define SSV_UPDATE_PAGE_ID(_sh)                 HAL_UPDATE_PAGE_ID(_sh)
#define SSV_RESET_SYSPLF(_sh)                   HAL_RESET_SYSPLF(_sh)
#define SSV_SAVE_HW_STATUS(_sc)                 HAL_SAVE_HW_STATUS(_sc)
#define SSV_RESTORE_HW_CONFIG(_sc)              HAL_RESTORE_HW_CONFIG(_sc)
#define SSV_IF_CHK_MAC2(_sh)					HAL_IF_CHK_MAC2(_sh)
#define SSV_GET_IC_TIME_TAG(_sh)                HAL_GET_IC_TIME_TAG(_sh)
#define SSV_RC_ALGORITHM(_sc)					HAL_RC_ALGORITHM(_sc)
#define SSV_SET_80211HW_RATE_CONFIG(_sc)		HAL_SET_80211HW_RATE_CONFIG(_sc)
#define SSV_RC_LEGACY_BITRATE_TO_RATE_DESC(_sc, _bitrate, _drate)		    \
			HAL_RC_LEGACY_BITRATE_TO_RATE_DESC(_sc, _bitrate, _drate)
#define SSV_DISABLE_FW_WSID(_sc, _index, _sta_priv, _vif_priv)                  \
            HAL_DISABLE_FW_WSID(_sc, _index, _sta_priv, _vif_priv)
#define SSV_ENABLE_FW_WSID(_sc, _sta, _sta_info, _type)                         \
            HAL_ENABLE_FW_WSID(_sc, _sta, _sta_info, _type)
#define SSV_WEP_USE_HW_CIPHER(_sc, _vif_priv)   HAL_WEP_USE_HW_CIPHER(_sc, _vif_priv)
#define SSV_PAIRWISE_WPA_USE_HW_CIPHER( _sc, _vif_priv, _cipher, _sta_priv)     \
            HAL_PAIRWISE_WPA_USE_HW_CIPHER( _sc, _vif_priv, _cipher, _sta_priv)
#define SSV_USE_HW_ENCRYPT(_cipher, _sc, _sta_priv, _vif_priv)                  \
            HAL_USE_HW_ENCRYPT(_cipher, _sc, _sta_priv, _vif_priv)
#define SSV_GROUP_WPA_USE_HW_CIPHER( _sc, _vif_priv, _cipher)                   \
            HAL_GROUP_WPA_USE_HW_CIPHER( _sc, _vif_priv, _cipher)
#define SSV_SET_AES_TKIP_HW_CRYPTO_GROUP_KEY(_sc, _vif_info, _sta_info, _param) \
            HAL_SET_AES_TKIP_HW_CRYPTO_GROUP_KEY(_sc, _vif_info, _sta_info, _param)
#define SSV_WRITE_PAIRWISE_KEYIDX_TO_HW(_sh, _key_idx, _wsid)                   \
            HAL_WRITE_PAIRWISE_KEYIDX_TO_HW(_sh, _key_idx, _wsid)
#define SSV_WRITE_GROUP_KEYIDX_TO_HW(_sh, _key_idx, _wsid)                      \
            HAL_WRITE_GROUP_KEYIDX_TO_HW(_sh, _key_idx, _wsid)
#define SSV_WRITE_PAIRWISE_KEY_TO_HW(_sc, _key_idx, _alg, _key, _key_len, _keyconf, _vif_priv, _sta_priv)   \
            HAL_WRITE_PAIRWISE_KEY_TO_HW(_sc, _key_idx, _alg, _key, _key_len, _keyconf, _vif_priv, _sta_priv)
#define SSV_WRITE_GROUP_KEY_TO_HW(_sc, _key_idx, _alg, _key, _key_len, _keyconf, _vif_priv, _sta_priv)      \
            HAL_WRITE_GROUP_KEY_TO_HW(_sc, _key_idx, _alg, _key, _key_len, _keyconf, _vif_priv, _sta_priv)
#define SSV_WRITE_KEY_TO_HW(_sc, _vif_priv, _sram_ptr, _wsid, _key_idx,  _key_type)   \
            HAL_WRITE_KEY_TO_HW(_sc, _vif_priv, _sram_ptr, _wsid, _key_idx,  _key_type)
#define SSV_SET_PAIRWISE_CIPHER_TYPE( _sh, _cipher, _wsid)                      \
            HAL_SET_PAIRWISE_CIPHER_TYPE( _sh, _cipher, _wsid) 
#define SSV_SET_GROUP_CIPHER_TYPE( _sh, _vif_priv, _cipher)                     \
            HAL_SET_GROUP_CIPHER_TYPE( _sh, _vif_priv, _cipher)
#define SSV_CHK_IF_SUPPORT_HW_BSSID( _sc, _vif_idx)                             \
            HAL_CHK_IF_SUPPORT_HW_BSSID( _sc, _vif_idx)
#define SSV_CHK_DUAL_VIF_CHG_RX_FLOW( _sc, _vif_priv)                           \
            HAL_CHK_DUAL_VIF_CHG_RX_FLOW( _sc, _vif_priv)
#define SSV_SET_FW_HWWSID_SEC_TYPE( _sc, _sta, _sta_info, _vif_priv)            \
            HAL_SET_FW_HWWSID_SEC_TYPE( _sc, _sta, _sta_info, _vif_priv)
#define SSV_SET_RX_CTRL_FLOW(_sh)              HAL_SET_RX_CTRL_FLOW(_sh)
#define SSV_RESTORE_RX_FLOW( _sc, _vif_priv, _sta)                              \
            HAL_RESTORE_RX_FLOW( _sc, _vif_priv, _sta)
#ifdef CONFIG_PM
#define SSV_SAVE_CLEAR_TRAP_REASON(_sc)			HAL_SAVE_CLEAR_TRAP_REASON(_sc)
#define SSV_RESTORE_TRAP_REASON(_sc)			HAL_RESTORE_TRAP_REASON(_sc);
#define SSV_PS_SAVE_RESET_RX_FLOW(_sc)			HAL_PS_SAVE_RESET_RX_FLOW(_sc)
#define SSV_PS_RESTORE_RX_FLOW(_sc)			    HAL_PS_RESTORE_RX_FLOW(_sc)
#define SSV_PMU_AWAKE(_sc)						HAL_PMU_AWAKE(_sc)
#define SSV_PS_HOLD_ON3(_sc, _value)		    HAL_PS_HOLD_ON3(_sc, _value)
#endif
#define SSV_ENABLE_USB_ACC(_sc, _epnum)         HAL_ENABLE_USB_ACC(_sc, _epnum)
#define SSV_DISABLE_USB_ACC(_sc, _epnum)        HAL_DISABLE_USB_ACC(_sc, _epnum)
#define SSV_SET_USB_LPM( _sc, _enable)          HAL_SET_USB_LPM( _sc, _enable)
#define SSV_JUMP_TO_ROM(_sc)                    HAL_JUMP_TO_ROM(_sc)

#define SSV_SET_WEP_KEY( _sc, _vif_priv, _sta_priv, _cipher, _key)            \
            HAL_SET_WEP_KEY( _sc, _vif_priv, _sta_priv,_cipher, _key)
#define SSV_PUT_MIC_SPACE_FOR_HW_CCMP_ENCRYPT( _sc, _skb)                       \
            HAL_PUT_MIC_SPACE_FOR_HW_CCMP_ENCRYPT( _sc, _skb)
#define SSV_USE_TURISMO_HW_ENCRYPT(_sc, _vif_priv)                              \
            HAL_USE_TURISMO_HW_ENCRYPT( _sc, _vif_priv)

#define SSV_GET_TX_DESC_CTYPE(_sh, _skb)        HAL_GET_TX_DESC_CTYPE(_sh, _skb) 
#define SSV_GET_TX_DESC_SIZE( _sh)              HAL_GET_TX_DESC_SIZE( _sh)
#define SSV_UPDATE_NULL_FUNC_TXINFO(_sc, _sta, _skb)                            \
                                                HAL_UPDATE_NULL_FUNC_TXINFO(_sc, _sta, _skb)
#define SSV_ADD_TXINFO(_sc, _skb)               HAL_ADD_TXINFO(_sc, _skb)
#define SSV_GET_TX_DESC_TXQ_IDX( _sh, _skb)     HAL_GET_TX_DESC_TXQ_IDX( _sh, _skb)
#define SSV_SET_MACADDR( _sh, _vif_idx)         HAL_SET_MACADDR( _sh, _vif_idx)
#define SSV_SET_MACADDR_2( _sh, _vif_idx, macaddr)       HAL_SET_MACADDR_2( _sh, _vif_idx, macaddr)
#define SSV_SET_BSSID( _sh, _bssid, _vif_idx)   HAL_SET_BSSID( _sh, _bssid, _vif_idx)
#define SSV_SET_OP_MODE( _sh, _opmode, _vif_idx) HAL_SET_OP_MODE( _sh, _opmode, _vif_idx)
#define SSV_HALT_MNGQ_UNTIL_DTIM( _sh)          HAL_HALT_MNGQ_UNTIL_DTIM( _sh, true)
#define SSV_UNHALT_MNGQ_UNTIL_DTIM( _sh)        HAL_HALT_MNGQ_UNTIL_DTIM( _sh, false)
#define SSV_SET_DUR_BURST_SIFS_G( _sh, _val)    HAL_SET_DUR_BURST_SIFS_G( _sh, _val)
#define SSV_SET_DUR_SLOT( _sh, _val)            HAL_SET_DUR_SLOT( _sh, _val)
#define SSV_SET_HW_WSID( _sc, _vif, _sta, _s)   HAL_SET_HW_WSID( _sc, _vif, _sta, _s)
#define SSV_ADD_FW_WSID( _sc, _vif_priv, _sta, _sta_info)                       \
            HAL_ADD_FW_WSID( _sc, _vif_priv, _sta, _sta_info)
#define SSV_DEL_HW_WSID( _sc, _hw_wsid)         HAL_DEL_HW_WSID( _sc, _hw_wsid)
#define SSV_SET_QOS_ENABLE( _sh, _qos)          HAL_SET_QOS_ENABLE( _sh, _qos)
#define SSV_SET_WMM_PARAM(_sc, _params, _queue) HAL_SET_WMM_PARAM( _sc, _params, _queue)
#define SSV_UPDATE_MAC80211_CHAN_INFO( _sc, _skb, _rxs)                         \
            HAL_UPDATE_MAC80211_CHAN_INFO( _sc, _skb, _rxs)
#define SSV_RC_MAC80211_RATE_IDX( _sc, _rate_idx, _rxs)                         \
            HAL_RC_MAC80211_RATE_IDX( _sc, _rate_idx, _rxs)
#define SSV_RC_MAC80211_TX_RATE_IDX( _sc, _rate_idx, _tx_info)                  \
            HAL_RC_MAC80211_TX_RATE_IDX( _sc, _rate_idx, _tx_info)
#define SSV_INIT_TX_CFG(_sh)         		    HAL_INIT_TX_CFG(_sh)
#define SSV_INIT_RX_CFG(_sh)         		    HAL_INIT_RX_CFG(_sh)
#define SSV_FREE_PBUF(_sc, _hw_buf_ptr)         HAL_FREE_PBUF(_sc, _hw_buf_ptr)
#define SSV_GET_TKIP_MMIC_ERR(_sh, _rx_skb)                                     \
            HAL_GET_TKIP_MMIC_ERR(_sh, _rx_skb)
#define SSV_GET_RX_DESC_INFO_HDR( _sh, _desc, _packet_len, _c_type, _tx_pkt_run_no)   \
            HAL_GET_RX_DESC_INFO_HDR( _sh, _desc, _packet_len, _c_type, _tx_pkt_run_no)
#define SSV_GET_RX_DESC_HDR_OFFSET(_sh, _skb)                                              \
            HAL_GET_RX_DESC_HDR_OFFSET(_sh, _skb)
#define SSV_GET_RX_DESC_PHY_RSSI(_sh, _skb)     HAL_GET_RX_DESC_PHY_RSSI(_sh, _skb)
#define SSV_NULLFUN_FRAME_FILTER(_sh, _skb)     HAL_NULLFUN_FRAME_FILTER(_sh, _skb)
#define SSV_GET_SW_ACK_CTL(_sh, _skb)           HAL_GET_SW_ACK_CTL(_sh, _skb)
#define SSV_GET_SW_ACK_SEQ(_sh, _skb)           HAL_GET_SW_ACK_SEQ(_sh, _skb)
#define SSV_PS_EVT_HANDLER(_sc, _skb) 			HAL_PS_EVT_HANDLER(_sc, _skb)
#define SSV_ADJ_CONFIG(_sh)                     HAL_ADJ_CONFIG(_sh)
#define SSV_BEACON_LOSS_ENABLE(_sh) 			HAL_BEACON_LOSS_ENABLE(_sh)
#define SSV_BEACON_LOSS_DISABLE(_sh) 			HAL_BEACON_LOSS_DISABLE(_sh)
#define SSV_BEACON_LOSS_CONFIG(_sh, _beacon_int, _bssid)						\
			HAL_BEACON_LOSS_CONFIG(_sh, _beacon_int, _bssid)
#define SSV_PHY_ENABLE(_sh, _val)               HAL_PHY_ENABLE(_sh, _val)               
#define SSV_EDCA_ENABLE(_sh, _val)  		    HAL_EDCA_ENABLE(_sh, _val)
#define SSV_EDCA_STAT(_sh)          			HAL_EDCA_STAT(_sh)
#ifdef CONFIG_ENABLE_ACS_FUNC
#define SSV_EDCA_UPDATE_SURVEY(_sh)             HAL_EDCA_UPDATE_SURVEY(_sh)
#define SSV_RX_FIXED_GAIN_ENABLE(_sh, _val)     HAL_RX_FIXED_GAIN_ENABLE(_sh, _val)
#endif
#define SSV_SEND_TX_POLL_CMD(_sh, _type)        HAL_SEND_TX_POLL_CMD(_sh, _type)
#define SSV_UPDATE_RF_TABLE(_sh)                HAL_UPDATE_RF_TABLE(_sh)
#ifdef CONFIG_ENABLE_HOST_THERMAL
#define SSV_DO_TEMPERATURE_COMPENSATION(_sh)                HAL_DO_TEMPERATURE_COMPENSATION(_sh)
#endif
#define SSV_PLL_CHK(_sh)                        HAL_PLL_CHK(_sh)               
#define SSV_AMPDU_AUTO_CRC_EN(_sh)              HAL_AMPDU_AUTO_CRC_EN(_sh)
#define SSV_SET_RX_BA(_sh, _bool, _ba_ra_addr, _ba_tid, _ba_ssn, _val)      \
            HAL_SET_RX_BA(_sh, _bool, _ba_ra_addr, _ba_tid, _ba_ssn, _val)
#define SSV_AMPDU_RX_START(_sc, _hw, _vif, _sta, _tid, _ssn, _buf_size)          \
            HAL_AMPDU_RX_START(_sc, _hw, _vif, _sta, _tid, _ssn, _buf_size)
//function header.
struct SKB_info_st;
struct ampdu_ba_notify_data;
int  ssv6xxx_frame_hdrlen(struct ieee80211_hdr *hdr, bool is_ht);
u32  ssv6xxx_ht_txtime(u8 rix, int pktlen, int width, int half_gi, bool is_gf);
u32  ssv6xxx_non_ht_txtime(u8 phy, int kbps, u32 frameLen, bool shortPreamble);
int  ssv6200_dump_BA_notification (char *buf, struct ampdu_ba_notify_data *ba_notification);
int  ssv6xxx_get_real_index(struct ssv_softc *sc, struct sk_buff *skb);
bool ssv6xxx_ssn_to_bit_idx (u32 start_ssn, u32 mpdu_ssn, u32 *word_idx,
         u32 *bit_idx);
void ssv6xxx_mark_skb_retry (struct ssv_softc *sc, struct SKB_info_st *skb_info, struct sk_buff *skb);
bool ssv6xxx_inc_bit_idx (struct ssv_softc *sc, u32 ssn_1st, u32 ssn_next, u32 *word_idx,
         u32 *bit_idx);   
void ssv6xxx_find_txpktrun_no_from_BA(struct ssv_softc *sc, u32 start_ssn, u32 sn_bit_map[2]
    , struct ssv_sta_priv_data *ssv_sta_priv);
void ssv6xxx_find_txpktrun_no_from_ssn(struct ssv_softc *sc, u32 ssn,
    struct ssv_sta_priv_data *ssv_sta_priv);
void _ssv6xxx_hexdump(const char *title, const u8 *buf, size_t len);

void ssv6xxx_enable_usb_acc(void *param, u8 epnum);
void ssv6xxx_disable_usb_acc(void *param, u8 epnum);
void ssv6xxx_jump_to_rom(void *param);
int ssv6xxx_rx_mode(void *param);
void ssv6xxx_beacon_miss_work(struct work_struct *work);
bool ssv6200_not_dual_intf_on_line(struct ssv_softc *sc);
int ssvxxx_get_sta_assco_cnt(struct ssv_softc *sc);
void ssv6xxx_set_ampdu_rx_add_work (struct work_struct *work);
void ssv6xxx_set_ampdu_rx_del_work(struct work_struct *work);
void ssv6xxx_scan_opertaion(struct ssv_softc *sc, bool start);
#ifdef CONFIG_ENABLE_HOST_THERMAL
void ssv6xxx_thermal_monitor_process(struct work_struct *work);
#endif

static inline unsigned ssv6xxx_compare_ether_addr(const u8 *addr1, const u8 *addr2)
{
    const u16 *a = (const u16 *) addr1;
    const u16 *b = (const u16 *) addr2;
    return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | (a[2] ^ b[2])) != 0;
}
#endif /* _DEV_H_ */

