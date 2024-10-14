/****************************************************************************/
/*                                                                          */
/*  Name:       smp_int.h                                                   */
/*                                                                          */
/*  Function    this file contains internally used SMP definitions          */
/*                                                                          */
/*  Copyright (c) 1999-2008, Broadcom Corp., All Rights Reserved.           */
/*  WIDCOMM Bluetooth Core. Proprietary and confidential.                   */
/*                                                                          */
/*****************************************************************************/
#ifndef  SMP_INT_H
#define  SMP_INT_H

#include "btu.h"
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
#include "btm_ble_api.h"
#endif
#include "smp_api.h"

#define SMP_MODEL_ENC_ONLY      0   /* Legacy mode, Just Works model */
#define SMP_MODEL_PASSKEY       1   /* Legacy mode, Passkey Entry model, this side inputs the key */
#define SMP_MODEL_OOB           2   /* Legacy mode, OOB model */
#define SMP_MODEL_KEY_NOTIF     3   /* Legacy mode, Passkey Entry model, this side displays the key */
#if BLE_INCLUDED == FALSE || SMP_INCLUDED == FALSE || SMP_LE_SC_INCLUDED == FALSE
#define SMP_MODEL_MAX       4       /* SMP_MODEL_OUT_OF_RANGE is used instead for LE SC */
#else
#define SMP_MODEL_SC_JW         4   /* Secure Connections mode, Just Works model */
#define SMP_MODEL_SC_NC         5   /* Secure Connections mode, Numeric Comparison model */
#define SMP_MODEL_SC_PE         6   /* Secure Connections mode, Passkey Entry model, this side inputs the key */
#define SMP_MODEL_SC_KN         7   /* Secure Connections mode, Passkey Entry model, this side displays the key */
#define SMP_MODEL_SC_OOB        8   /* Secure Connections mode, OOB model */
#define SMP_MODEL_OUT_OF_RANGE  9
#endif
typedef UINT8   tSMP_ASSO_MODEL;


#ifndef SMP_MAX_CONN
#define SMP_MAX_CONN    2
#endif

#define SMP_WAIT_FOR_RSP_TOUT           30

#if BLE_INCLUDED == FALSE || SMP_INCLUDED == FALSE || SMP_LE_SC_INCLUDED == FALSE
#define SMP_OPCODE_INIT                   0x04
#endif

/* SMP events */
#define SMP_PAIRING_REQ_EVT             SMP_OPCODE_PAIRING_REQ
#define SMP_PAIRING_RSP_EVT             SMP_OPCODE_PAIRING_RSP
#define SMP_CONFIRM_EVT                 SMP_OPCODE_CONFIRM
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
#define SMP_RAND_EVT                    SMP_OPCODE_RAND
#else
#define SMP_RAND_EVT                    SMP_OPCODE_INIT
#endif
#define SMP_PAIRING_FAILED_EVT          SMP_OPCODE_PAIRING_FAILED
#define SMP_ENCRPTION_INFO_EVT          SMP_OPCODE_ENCRYPT_INFO
#define SMP_MASTER_ID_EVT               SMP_OPCODE_MASTER_ID
#define SMP_ID_INFO_EVT                 SMP_OPCODE_IDENTITY_INFO
#define SMP_ID_ADDR_EVT                 SMP_OPCODE_ID_ADDR
#define SMP_SIGN_INFO_EVT               SMP_OPCODE_SIGN_INFO
#define SMP_SECURITY_REQ_EVT            SMP_OPCODE_SEC_REQ

#if BLE_INCLUDED == FALSE || SMP_INCLUDED == FALSE || SMP_LE_SC_INCLUDED == FALSE
#define SMP_SELF_DEF_EVT                SMP_SECURITY_REQ_EVT
#define SMP_KEY_READY_EVT               (SMP_SELF_DEF_EVT + 1)
#define SMP_ENCRYPTED_EVT               (SMP_SELF_DEF_EVT + 2)
#define SMP_L2CAP_CONN_EVT              (SMP_SELF_DEF_EVT + 3)
#define SMP_L2CAP_DISCONN_EVT           (SMP_SELF_DEF_EVT + 4)
#define SMP_IO_RSP_EVT                  (SMP_SELF_DEF_EVT + 5)
#define SMP_API_SEC_GRANT_EVT           (SMP_SELF_DEF_EVT + 6)
#define SMP_TK_REQ_EVT                  (SMP_SELF_DEF_EVT + 7)
#define SMP_AUTH_CMPL_EVT               (SMP_SELF_DEF_EVT + 8)
#define SMP_ENC_REQ_EVT                 (SMP_SELF_DEF_EVT + 9)
#define SMP_BOND_REQ_EVT                (SMP_SELF_DEF_EVT + 10)
#define SMP_DISCARD_SEC_REQ_EVT         (SMP_SELF_DEF_EVT + 11)
#define SMP_MAX_EVT                     SMP_DISCARD_SEC_REQ_EVT + 1
#else
#define SMP_PAIR_PUBLIC_KEY_EVT         SMP_OPCODE_PAIR_PUBLIC_KEY
#define SMP_PAIR_DHKEY_CHCK_EVT         SMP_OPCODE_PAIR_DHKEY_CHECK
#define SMP_PAIR_KEYPR_NOTIF_EVT        SMP_OPCODE_PAIR_KEYPR_NOTIF

#define SMP_PAIR_COMMITM_EVT            SMP_OPCODE_PAIR_COMMITM

/* LVE TBD #define SMP_SELF_DEF_EVT                (SMP_PAIR_KEYPR_NOTIF_EVT + 1) */
#define SMP_SELF_DEF_EVT                (SMP_PAIR_COMMITM_EVT + 1)
#define SMP_KEY_READY_EVT               (SMP_SELF_DEF_EVT)
#define SMP_ENCRYPTED_EVT               (SMP_SELF_DEF_EVT + 1)
#define SMP_L2CAP_CONN_EVT              (SMP_SELF_DEF_EVT + 2)
#define SMP_L2CAP_DISCONN_EVT           (SMP_SELF_DEF_EVT + 3)
#define SMP_IO_RSP_EVT                  (SMP_SELF_DEF_EVT + 4)
#define SMP_API_SEC_GRANT_EVT           (SMP_SELF_DEF_EVT + 5)
#define SMP_TK_REQ_EVT                  (SMP_SELF_DEF_EVT + 6)
#define SMP_AUTH_CMPL_EVT               (SMP_SELF_DEF_EVT + 7)
#define SMP_ENC_REQ_EVT                 (SMP_SELF_DEF_EVT + 8)
#define SMP_BOND_REQ_EVT                (SMP_SELF_DEF_EVT + 9)
#define SMP_DISCARD_SEC_REQ_EVT         (SMP_SELF_DEF_EVT + 10)
#define SMP_PUBL_KEY_EXCH_REQ_EVT       (SMP_SELF_DEF_EVT + 11)     /* request to start public key exchange */
#define SMP_LOC_PUBL_KEY_CRTD_EVT       (SMP_SELF_DEF_EVT + 12)     /* local public key created */
#define SMP_BOTH_PUBL_KEYS_RCVD_EVT     (SMP_SELF_DEF_EVT + 13)     /* both local and peer public keys are saved in cb */
#define SMP_SC_DHKEY_CMPLT_EVT          (SMP_SELF_DEF_EVT + 14)     /* DHKey computation is completed, time to start SC phase1 */
#define SMP_HAVE_LOC_NONCE_EVT          (SMP_SELF_DEF_EVT + 15)     /* new local nonce is generated and saved in p_cb->rand */
#define SMP_SC_PHASE1_CMPLT_EVT         (SMP_SELF_DEF_EVT + 16)     /* time to start SC phase2 */
#define SMP_SC_CALC_NC_EVT              (SMP_SELF_DEF_EVT + 17)     /* request to calculate number for user check. Used only in the numeric compare protocol */
#define SMP_SC_DSPL_NC_EVT              (SMP_SELF_DEF_EVT + 18)     /* request to display the number for user check to the user. Used only in the numeric compare protocol */
#define SMP_SC_NC_OK_EVT                (SMP_SELF_DEF_EVT + 19)     /* user confirms 'OK' numeric comparison request */
#define SMP_SC_2_DHCK_CHKS_PRES_EVT     (SMP_SELF_DEF_EVT + 20)     /* both local and peer DHKey Checks are already present - it is used on slave to prevent race condition */
#define SMP_SC_KEY_READY_EVT            (SMP_SELF_DEF_EVT + 21)     /* same meaning as SMP_KEY_READY_EVT to separate between SC and legacy actions */
#define SMP_KEYPR_NOTIF_EVT             (SMP_SELF_DEF_EVT + 22)
#define SMP_SC_OOB_DATA_EVT             (SMP_SELF_DEF_EVT + 23)     /* SC OOB data from some repository is provided */
#define SMP_CR_LOC_SC_OOB_DATA_EVT      (SMP_SELF_DEF_EVT + 24)     /* request to create local SC OOB data (TBD to add SMP_LE_SC_OOB_INCLUDED == TRUE) */
/* LVE TBD #define SMP_MAX_EVT                     SMP_SC_OOB_DATA_EVT */
#define SMP_MAX_EVT                     SMP_CR_LOC_SC_OOB_DATA_EVT  /* (TBD to add SMP_LE_SC_OOB_INCLUDED == TRUE) */
#endif
typedef UINT8 tSMP_EVENT;

/* auumption it's only using the low 8 bits, if bigger than that, need to expand it to be 16 bits */
#define SMP_SEC_KEY_MASK                    0x00ff

/* SMP pairing state */
enum
{
    SMP_ST_IDLE,
    SMP_ST_WAIT_APP_RSP,
    SMP_ST_SEC_REQ_PENDING,
    SMP_ST_PAIR_REQ_RSP,
    SMP_ST_WAIT_CONFIRM,
    SMP_ST_CONFIRM,
    SMP_ST_RAND,
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    SMP_ST_PUBL_KEY_EXCH,
    SMP_ST_SC_PHS1_STRT,
    SMP_ST_WAIT_COMMITM,
    SMP_ST_WAIT_NONCE,
    SMP_ST_SC_PHS2_STRT,
    SMP_ST_WAIT_DHK_CHCK,
    SMP_ST_DHK_CHCK,
#endif
    SMP_ST_ENC_PENDING,
    SMP_ST_BOND_PENDING,
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE /* TBD to add SMP_LE_SC_OOB_INCLUDED == TRUE */
    SMP_ST_CR_LOC_SC_OOB_DATA,
#endif
    SMP_ST_MAX
};
typedef UINT8 tSMP_STATE;

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
/* SMP over BR/EDR events */
#define SMP_BR_PAIRING_REQ_EVT              SMP_OPCODE_PAIRING_REQ
#define SMP_BR_PAIRING_RSP_EVT              SMP_OPCODE_PAIRING_RSP
#define SMP_BR_CONFIRM_EVT                  SMP_OPCODE_CONFIRM          /* not expected over BR/EDR */
#define SMP_BR_RAND_EVT                     SMP_OPCODE_RAND             /* not expected over BR/EDR */
#define SMP_BR_PAIRING_FAILED_EVT           SMP_OPCODE_PAIRING_FAILED
#define SMP_BR_ENCRPTION_INFO_EVT           SMP_OPCODE_ENCRYPT_INFO     /* not expected over BR/EDR */
#define SMP_BR_MASTER_ID_EVT                SMP_OPCODE_MASTER_ID        /* not expected over BR/EDR */
#define SMP_BR_ID_INFO_EVT                  SMP_OPCODE_IDENTITY_INFO
#define SMP_BR_ID_ADDR_EVT                  SMP_OPCODE_ID_ADDR
#define SMP_BR_SIGN_INFO_EVT                SMP_OPCODE_SIGN_INFO
#define SMP_BR_SECURITY_REQ_EVT             SMP_OPCODE_SEC_REQ          /* not expected over BR/EDR */
#define SMP_BR_PAIR_PUBLIC_KEY_EVT          SMP_OPCODE_PAIR_PUBLIC_KEY  /* not expected over BR/EDR */
#define SMP_BR_PAIR_DHKEY_CHCK_EVT          SMP_OPCODE_PAIR_DHKEY_CHECK /* not expected over BR/EDR */
#define SMP_BR_PAIR_KEYPR_NOTIF_EVT         SMP_OPCODE_PAIR_KEYPR_NOTIF /* not expected over BR/EDR */
#define SMP_BR_SELF_DEF_EVT                 SMP_BR_PAIR_KEYPR_NOTIF_EVT
#define SMP_BR_KEY_READY_EVT                (SMP_BR_SELF_DEF_EVT + 1)
#define SMP_BR_ENCRYPTED_EVT                (SMP_BR_SELF_DEF_EVT + 2)
#define SMP_BR_L2CAP_CONN_EVT               (SMP_BR_SELF_DEF_EVT + 3)
#define SMP_BR_L2CAP_DISCONN_EVT            (SMP_BR_SELF_DEF_EVT + 4)
#define SMP_BR_KEYS_RSP_EVT                 (SMP_BR_SELF_DEF_EVT + 5)
#define SMP_BR_API_SEC_GRANT_EVT            (SMP_BR_SELF_DEF_EVT + 6)
#define SMP_BR_TK_REQ_EVT                   (SMP_BR_SELF_DEF_EVT + 7)
#define SMP_BR_AUTH_CMPL_EVT                (SMP_BR_SELF_DEF_EVT + 8)
#define SMP_BR_ENC_REQ_EVT                  (SMP_BR_SELF_DEF_EVT + 9)
#define SMP_BR_BOND_REQ_EVT                 (SMP_BR_SELF_DEF_EVT + 10)
#define SMP_BR_DISCARD_SEC_REQ_EVT          (SMP_BR_SELF_DEF_EVT + 11)
#define SMP_BR_MAX_EVT                      (SMP_BR_SELF_DEF_EVT + 12)
typedef UINT8 tSMP_BR_EVENT;

/* SMP over BR/EDR pairing states */
enum
{
    SMP_BR_ST_IDLE = SMP_ST_IDLE,
    SMP_BR_ST_WAIT_APP_RSP,
    SMP_BR_ST_PAIR_REQ_RSP,
    SMP_BR_ST_BOND_PENDING,
    SMP_BR_ST_MAX
};
typedef UINT8 tSMP_BR_STATE;
#endif /* BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE */

/* random and encrption activity state */
enum
{
    SMP_GEN_COMPARE = 1,
    SMP_GEN_CONFIRM,

    SMP_GEN_DIV_LTK,
    SMP_GEN_DIV_CSRK,
    SMP_GEN_RAND_V,
    SMP_GEN_TK,
    SMP_GEN_SRAND_MRAND,
    SMP_GEN_SRAND_MRAND_CONT
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    , SMP_GEN_PRIV_KEY_0_7,
    SMP_GEN_PRIV_KEY_8_15,
    SMP_GEN_PRIV_KEY_16_23,
    SMP_GEN_PRIV_KEY_24_31,
    SMP_GEN_NONCE_0_7,
    SMP_GEN_NONCE_8_15
#endif
};

enum
{
    SMP_KEY_TYPE_TK,
    SMP_KEY_TYPE_CFM,
    SMP_KEY_TYPE_CMP,
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    SMP_KEY_TYPE_PEER_DHK_CHCK,
#endif
    SMP_KEY_TYPE_STK,
    SMP_KEY_TYPE_LTK
};
typedef struct
{
    UINT8   key_type;
    UINT8  *p_data;
} tSMP_KEY;

typedef union
{
    UINT8       *p_data;    /* UINT8 type data pointer */
    tSMP_KEY    key;
    UINT16      reason;
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    UINT32      passkey;
#if SMP_LE_SC_OOB_INCLUDED == TRUE
    tSMP_OOB_DATA_TYPE  req_oob_type;
#endif
#endif
} tSMP_INT_DATA;

/* internal status mask */
#define SMP_PAIR_FLAGS_WE_STARTED_DD           (1)
#define SMP_PAIR_FLAGS_PEER_STARTED_DD         (1 << 1)
#define SMP_PAIR_FLAGS_CMD_CONFIRM             (1 << SMP_OPCODE_CONFIRM) /* 1 << 3 */
#define SMP_PAIR_FLAG_ENC_AFTER_PAIR           (1 << 4)
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
#define SMP_PAIR_FLAG_HAVE_PEER_DHK_CHK        (1 << 5) /* it is used on slave to resolve race condition */
#define SMP_PAIR_FLAG_HAVE_PEER_PUBL_KEY       (1 << 6) /* it is used on slave to resolve race condition */
#define SMP_PAIR_FLAG_HAVE_PEER_COMM           (1 << 7) /* it is used to resolve race condition */
#define SMP_PAIR_FLAG_HAVE_LOCAL_PUBL_KEY      (1 << 8) /* it is used on slave to resolve race condition */
#endif

/* check if authentication requirement need MITM protection */
#define SMP_NO_MITM_REQUIRED(x)  (((x) & SMP_AUTH_YN_BIT) == 0)

#define SMP_ENCRYPT_KEY_SIZE        16
#define SMP_ENCRYPT_DATA_SIZE       16

typedef struct
{
    BD_ADDR bd_addr;
    BT_HDR *p_copy;
} tSMP_REQ_Q_ENTRY;

/* SMP control block */
typedef struct
{
    tSMP_CALLBACK   *p_callback;
    TIMER_LIST_ENT  rsp_timer_ent;
    UINT8           trace_level;

    BD_ADDR         pairing_bda;

    tSMP_STATE      state;
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
    BOOLEAN         derive_lk;
    BOOLEAN         id_addr_rcvd;
    tBLE_ADDR_TYPE  id_addr_type;
    BD_ADDR         id_addr;
    BOOLEAN         smp_over_br;
    tSMP_BR_STATE   br_state;           /* if SMP over BR/ERD has priority over SMP */
    BT_OCTET16      ltk_to_derive_lk;   /* ltk can be less then 16 bytes, lk has to be
                                        derived from all 16 bytes */
#endif
    UINT8           failure;
    UINT8           status;
    UINT8           role;
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    UINT16          flags;
#else
    UINT8           flags;
#endif
    UINT8           cb_evt;

    tSMP_SEC_LEVEL  sec_level;
    BOOLEAN         connect_initialized;
    BT_OCTET16      confirm;
    BT_OCTET16      rconfirm;
    BT_OCTET16      rrand;                      /* for SC this is peer nonce */
    BT_OCTET16      rand;                       /* for SC this is local nonce */
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    BT_OCTET32      private_key;
    BT_OCTET32      dhkey;
    BT_OCTET16      commitm;
    BT_OCTET16      rcommitm;
    BT_OCTET16      loc_random;                 /* local randomizer - passkey or OOB randomizer */
    BT_OCTET16      peer_random;                /* peer randomizer - passkey or OOB randomizer */
    BT_OCTET16      dhkey_check;
    BT_OCTET16      rdhkey_check;
    tSMP_PUBLIC_KEY loc_publ_key;
    tSMP_PUBLIC_KEY peer_publ_key;
#if SMP_LE_SC_OOB_INCLUDED == TRUE
    tSMP_OOB_DATA_TYPE  req_oob_type;
    tSMP_SC_OOB_DATA    sc_oob_data;
#endif
#endif
    tSMP_IO_CAP     peer_io_caps;
    tSMP_IO_CAP     loc_io_caps;
    tSMP_OOB_FLAG   peer_oob_flag;
    tSMP_OOB_FLAG   loc_oob_flag;
    tSMP_AUTH_REQ   peer_auth_req;
    tSMP_AUTH_REQ   loc_auth_req;
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
    BOOLEAN         sc_only_mode_required;  /* TRUE if locally SM is required to operate either in SC mode or not at all */
    tSMP_ASSO_MODEL assoc_model_selected;
    BOOLEAN         le_sc_mode_is_used;
    BOOLEAN         le_sc_kp_notif_is_used;
    tSMP_SC_KEY_TYPE    loc_keypr_notif;
    tSMP_SC_KEY_TYPE    peer_keypr_notif;
    UINT8           round;                  /* authentication stage 1 round for passkey association model */
    UINT32          num_to_displ;
    BT_OCTET16      mac_key;
#endif
    UINT8           peer_enc_size;
    UINT8           loc_enc_size;
    UINT8           peer_i_key;
    UINT8           peer_r_key;
    UINT8           loc_i_key;
    UINT8           loc_r_key;

    BT_OCTET16      tk;
    BT_OCTET16      ltk;
    UINT16          div;
    BT_OCTET16      csrk;  /* storage for local CSRK */
    UINT16          ediv;
    BT_OCTET8       enc_rand;

    UINT8           rand_enc_proc;
    UINT8           addr_type;
    BD_ADDR         local_bda;
    BOOLEAN         is_pair_cancel;
    BOOLEAN         discard_sec_req;
    UINT8           rcvd_cmd_code;
    UINT8           rcvd_cmd_len;
    UINT16          total_tx_unacked;
    BOOLEAN         wait_4_auth_cmpl;

#if SMP_CONFORMANCE_TESTING == TRUE
    BOOLEAN         enable_test_confirm_val;
    BT_OCTET16      test_confirm;
    BOOLEAN         enable_test_rand_val;
    BT_OCTET16      test_rand;
    BOOLEAN         enable_test_pair_fail;
    UINT8           pair_fail_status;
    BOOLEAN         remove_fixed_channel_disable;
    BOOLEAN         skip_test_compare_check;
    BOOLEAN         use_min_encr_key_len_param;
    UINT8           min_encr_key_len_param;
#if SMP_LE_SC_INCLUDED == TRUE
    BOOLEAN         enable_test_nc_fail;
    BOOLEAN         enable_test_jw_conf_fail_master;
    BOOLEAN         enable_test_passk_entry_fail;
    BOOLEAN         enable_test_oob_fail;
    BOOLEAN         enable_test_peer_sc_notif;
#endif
#endif

} tSMP_CB;

/* Server Action functions are of this type */
typedef void (*tSMP_ACT)(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);


#ifdef __cplusplus
extern "C"
{
#endif

#if SMP_DYNAMIC_MEMORY == FALSE
SMP_API extern tSMP_CB  smp_cb;
#else
SMP_API extern tSMP_CB *smp_cb_ptr;
#define smp_cb (*smp_cb_ptr)
#endif

#ifdef __cplusplus
}
#endif

/* Functions provided by att_main.c */
SMP_API extern void smp_init(void);

#if SMP_CONFORMANCE_TESTING == TRUE
/* Used only for conformance testing */
SMP_API extern void  smp_set_test_confirm_value(BOOLEAN enable, UINT8 *p_c_value);
SMP_API extern void  smp_set_test_rand_value(BOOLEAN enable, UINT8 *p_c_value);
SMP_API extern void  smp_set_test_pair_fail_status(BOOLEAN enable, UINT8 status);
SMP_API extern void  smp_remove_fixed_channel_disable(BOOLEAN disable);
SMP_API extern void  smp_skip_compare_check(BOOLEAN enable);
SMP_API extern void  smp_set_min_encr_key_len(UINT8 min_encr_key_len);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
SMP_API extern void  smp_set_fail_nc(BOOLEAN enable);
SMP_API extern void  smp_set_fail_conf(BOOLEAN enable);
SMP_API extern void  smp_set_passk_entry_fail(BOOLEAN enable);
SMP_API extern void  smp_set_oob_fail(BOOLEAN enable);
SMP_API extern void  smp_set_peer_sc_notif(BOOLEAN enable);
SMP_API extern void smp_aes_cmac_rfc4493_chk(UINT8 *key, UINT8 *msg, UINT8 msg_len,
        UINT8 mac_len, UINT8 *mac);
SMP_API extern void smp_f4_calc_chk(UINT8 *U, UINT8 *V, UINT8 *X, UINT8 *Z, UINT8 *mac);
SMP_API extern void smp_g2_calc_chk(UINT8 *U, UINT8 *V, UINT8 *X, UINT8 *Y);
SMP_API extern void smp_h6_calc_chk(UINT8 *key, UINT8 *key_id, UINT8 *mac);
SMP_API extern void smp_f5_key_calc_chk(UINT8 *w, UINT8 *mac);
SMP_API extern void smp_f5_mackey_or_ltk_calc_chk(UINT8 *t, UINT8 *counter,
        UINT8 *key_id, UINT8 *n1,
        UINT8 *n2, UINT8 *a1, UINT8 *a2,
        UINT8 *length, UINT8 *mac);
SMP_API extern void smp_f5_calc_chk(UINT8 *w, UINT8 *n1, UINT8 *n2, UINT8 *a1, UINT8 *a2,
                                    UINT8 *mac_key, UINT8 *ltk);
SMP_API extern void smp_f6_calc_chk(UINT8 *w, UINT8 *n1, UINT8 *n2, UINT8 *r,
                                    UINT8 *iocap, UINT8 *a1, UINT8 *a2, UINT8 *mac);
#endif
#endif /* SMP_CONFORMANCE_TESTING == TRUE */
/* smp_main */
extern void         smp_sm_event(tSMP_CB *p_cb, tSMP_EVENT event, void *p_data);
extern tSMP_STATE   smp_get_state(void);
extern void         smp_set_state(tSMP_STATE state);

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE && BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
/* smp_br_main */
extern void             smp_br_sm_event(tSMP_CB *p_cb, tSMP_BR_EVENT event, void *p_data);
extern tSMP_BR_STATE    smp_get_br_state(void);
extern void             smp_set_br_state(tSMP_BR_STATE state);

#endif

/* smp_act.c */
extern void smp_send_pair_req(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_confirm(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_pair_fail(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
extern void smp_send_rand(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_pair_publ_key(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_commitm(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_dhkey_chck(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_keypr_notif(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#else
extern void smp_send_init(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#endif
extern void smp_proc_pair_fail(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_confirm(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
extern void smp_proc_rand(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_pair_publ_key(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#else
extern void smp_proc_init(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#endif
extern void smp_proc_enc_info(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_master_id(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_id_info(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_id_addr(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_sec_grant(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_sec_req(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_sl_key(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_start_enc(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_enc_cmpl(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_discard(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_pairing_cmpl(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_decide_asso_model(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_app_cback(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_compare(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_check_auth_req(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_io_rsp(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_id_info(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_enc_info(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_csrk_info(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_ltk_reply(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_pair_cmd(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_pair_terminate(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_idle_terminate(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_send_pair_rsp(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_key_distribution(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_srk_info(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_generate_csrk(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_fast_conn_param(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_key_pick_key(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
extern void smp_have_both_publ_keys(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_start_sc_phase1(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_loc_nonce(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_commitm(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_peer_nonce(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_dhkey_chck(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_match_dhkey_chcks(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_keypr_notif(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_move_to_phase2(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_2_dhkey_chks_pres(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_wait_both_publ_keys(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_start_passk_verif(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_sc_oob_data(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_proc_sc_ltk(void);
extern void smp_set_loc_oob_keys(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);         /* TBD to add SMP_LE_SC_OOB_INCLUDED == TRUE */
extern void smp_set_loc_oob_rand_comm(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);    /* TBD to add SMP_LE_SC_OOB_INCLUDED == TRUE */
#if BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
extern void smp_set_derive_lk(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_derive_lk_from_ltk(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_proc_pair_cmd(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_proc_sec_grant(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_proc_sl_keys_rsp(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_send_pair_rsp(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_check_auth_req(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_key_distribution(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_proc_lk(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_key_distr_by_transp(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_br_pairing_cmpl(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#endif
#endif

/* smp_l2c */
extern void smp_l2cap_if_init(void);
extern void smp_data_ind(BD_ADDR bd_addr, BT_HDR *p_buf);

/* smp_util.c */
extern BOOLEAN smp_send_cmd(UINT8 cmd_code, tSMP_CB *p_cb);
extern void smp_cb_cleanup(tSMP_CB *p_cb);
extern void smp_reset_control_value(tSMP_CB *p_cb);
extern void smp_proc_pairing_cmpl(tSMP_CB *p_cb);
extern void smp_convert_string_to_tk(BT_OCTET16 tk, UINT32 passkey);
extern void smp_mask_enc_key(UINT8 loc_enc_size, UINT8 *p_data);
extern void smp_rsp_timeout(TIMER_LIST_ENT *p_tle);
extern void smp_xor_128(BT_OCTET16 a, BT_OCTET16 b);
extern BOOLEAN smp_encrypt_data(UINT8 *key, UINT8 key_len,
                                UINT8 *plain_text, UINT8 pt_len,
                                tSMP_ENC *p_out);
extern BOOLEAN smp_find_cmd_inval_params(tSMP_CB *p_cb);
extern void smp_reject_unexp_pair_cmd(BD_ADDR bd_addr);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
extern tSMP_ASSO_MODEL smp_select_assoc_model(tSMP_CB *p_cb);
extern void smp_reverse_array(UINT8 *arr, UINT8 len);
extern UINT8 smp_calc_ri(UINT8 *random, UINT8 round);
extern void smp_collect_loc_iocap(UINT8 *iocap, tSMP_CB *p_cb);
extern void smp_collect_peer_iocap(UINT8 *iocap, tSMP_CB *p_cb);
extern void smp_collect_loc_le_addr(UINT8 *le_addr, tSMP_CB *p_cb);
extern void smp_collect_peer_le_addr(UINT8 *le_addr, tSMP_CB *p_cb);
extern BOOLEAN smp_check_commitment(tSMP_CB *p_cb);
extern void smp_save_sc_ltk(tSMP_CB *p_cb);
extern BOOLEAN smp_calc_f5_mackey_and_ltk(tSMP_CB *p_cb);
extern void smp_remove_fixed_chnl_by_transport(tSMP_CB *p_cb);
#if SMP_LE_SC_OOB_INCLUDED == TRUE
extern BOOLEAN smp_req_oob_data(tSMP_CB *p_cb);
#endif /* SMP_LE_SC_OOB_INCLUDED == TRUE */
#endif

/* smp_keys.c */
extern void smp_generate_confirm(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_generate_compare(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_generate_stk(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_generate_ltk(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_generate_passkey(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_genenrate_rand_cont(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE && SMP_LE_SC_INCLUDED == TRUE
extern void smp_create_priv_key(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_use_oob_priv_key(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);         /* TBD to add SMP_LE_SC_OOB_INCLUDED == TRUE */
extern void smp_compute_dhkey(tSMP_CB *p_cb);
extern void smp_calc_loc_commitm(tSMP_CB *p_cb);
extern void smp_calc_peer_commitm(tSMP_CB *p_cb, BT_OCTET16 output_buf);
extern void smp_calc_num_to_disp_nc(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_calc_loc_dhkey_chck(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_calc_peer_dhkey_chck(tSMP_CB *p_cb, tSMP_INT_DATA *p_data);
extern void smp_start_gener_nonce(tSMP_CB *p_cb);
#if BTM_CROSS_TRANSP_KEY_DERIVATION == TRUE
extern BOOLEAN smp_calc_lk_from_sc_ltk(tSMP_CB *p_cb);
extern BOOLEAN smp_calc_sc_ltk_from_lk(tSMP_CB *p_cb);
#endif
extern void smp_calc_f4(UINT8 *u, UINT8 *v, UINT8 *x, UINT8 z, UINT8 *c);
extern UINT32 smp_calc_g2(UINT8 *u, UINT8 *v, UINT8 *x, UINT8 *y);
extern BOOLEAN smp_calc_f5(UINT8 *w, UINT8 *n1, UINT8 *n2, UINT8 *a1, UINT8 *a2,
                           UINT8 *mac_key, UINT8 *ltk);
extern BOOLEAN smp_calc_f5_mackey_or_ltk(UINT8 *t, UINT8 *counter, UINT8 *key_id,
        UINT8 *n1, UINT8 *n2, UINT8 *a1, UINT8 *a2,
        UINT8 *length, UINT8 *mac);
extern BOOLEAN smp_calc_f5_key(UINT8 *w, UINT8 *t);
extern BOOLEAN smp_calc_f6(UINT8 *w, UINT8 *n1, UINT8 *n2, UINT8 *r, UINT8 *iocap,
                           UINT8 *a1, UINT8 *a2, UINT8 *f3);
extern BOOLEAN smp_calc_h6(UINT8 *w, UINT8 *keyid, UINT8 *h2);
#if SMP_DEBUG == TRUE
extern void smp_debug_print_nbyte_little_endian(UINT8 *p, const char *key_name,
        UINT8 len);
#else
#define smp_debug_print_nbyte_little_endian(p, key_name, len)
#endif
#endif

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
/* smp_cmac.c */
extern BOOLEAN AES_CMAC(BT_OCTET16 key, UINT8 *input, UINT16 length, UINT16 tlen, UINT8 *p_signature);      /* LVE TBD to move it to smp_api.h */
extern void print128(BT_OCTET16 x, const UINT8 *key_name);
#endif

#endif /* SMP_INT_H */

