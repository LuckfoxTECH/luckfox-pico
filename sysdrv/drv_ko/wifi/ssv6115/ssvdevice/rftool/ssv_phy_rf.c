#include <linux/kthread.h>

#include <ssv_cfg.h>
#include <hwif/hwif.h>
#include <hci/ssv_hci.h>
#include "ssvdevice/dev.h"

#include "fmac/fmac_msg_tx.h"
#include "fmac/fmac.h"
#include "fmac/fmac_rx.h"
#include "ipc_msg.h"
#include "rf_table.h"
#include "ssv_phy_rf.h"
#include "ssv_efuse.h"
#include "ssv_rftool.h"

#include "ssv_debug.h"

#define  snprintf_res ssv_snprintf_res 

extern struct ssv6xxx_cfg ssv_cfg;
extern struct ssv_rftool_cfg rftool_cfg;

struct st_rf_table def_rf_conf_table = {
                    /* Signature */
                    RF_API_SIGNATURE,
                    /* Structure version */
                    RF_API_TABLE_VERSION,
                    /* boot flag */
                    EN_FIRST_BOOT,
                    /* work mode */
                    EN_WORK_NOMAL,
                    /* reserved */
                    0,
                    /* reserved */
                    0,
                    /* thermal config */
                    {
                        { { 18,  18 ,18,  18,  18,  18,  18} , 0x87, 0x87,  0,  0,  0,  0,  0,  0,  0 },
                        { { 18,  18 ,18,  18,  18,  18,  18} , 0x87, 0x87,  0,  0,  0,  0,  0,  0,  0 },
                        { { 18,  18 ,18,  18,  18,  18,  18} , 0x87, 0x87,  0,  0,  0,  0,  0,  0,  0 },
                        { { 18,  18 ,18,  18,  18,  18,  18} , 0x87, 0x87,  0,  0,  0,  0,  0,  0,  0 },
                        { { 18,  18 ,18,  18,  18,  18,  18} , 0x87, 0x87,  0,  0,  0,  0,  0,  0,  0 },
                    },
                    /* ble thermal config */
                    {
                        { { 6,  6} , { 0,  0} },
                        { { 6,  6} , { 0,  0} },
                        { { 6,  6} , { 0,  0} },
                        { { 6,  6} , { 0,  0} },
                        { { 6,  6} , { 0,  0} },
                    },
                    /* temperature boundarty */
                    { -15, 10, 55, 90},
                    /* rate gain table */
                    { 
                        {15, 18, 17, 16, 15, 18, 17, 16, 15, 18, 17, 16, 15 ,
                          18, 17, 16, 15, 13, 18, 17, 16, 15, 13, 18, 17, 16, 15, 13, 18, 17, 16, 15, 13},
                        {0, 0, 0, 0, 0, 0, 0}
                    },
                    /*abs power*/
                    15,
                    /*ble DTM mode*/
                    0,
                    /* DCDC flag */
                    1,
                    /* padpd flag*/
                    0,       
};

#define  OFDM_RATE_GAIN_256QAM_OFFSET        (-2)
#define OFDM_RATE_GAIN_64QAM_OFFSET         (0)
#define OFDM_RATE_GAIN_16QAM_OFFSET         (1)
#define OFDM_RATE_GAIN_QPSK_OFFSET           (2)
#define OFDM_RATE_GAIN_BPSK_OFFSET             (3)


//static s8 thermal_band_offset[] = {-5, -2, 0, 1, 2};
//static s8 thermal_xtal_offset[] ={6, 6, 0, 6, 30};
//static s8 ble_thermal_pow_offset[] = {3, 1, 0, -1, -2};
static u8 rf_engmode = 0;

static char* thermal_name[] = {"LT2 Config", "LT1 Config", "RT Config", "HT1 Config", "HT2 Config"};

extern int ssv_private_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len);
extern int ssv_rftool_private_msg_to_hci(struct ssv_rftool_softc *srfc, u8 *msg_buffer, u32 msg_len);
int ssv_send_priv_msg_rf_update_table(struct ssv_rftool_softc *srfc, struct st_rf_table *param)
{
    struct st_rf_table *p_table;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct st_rf_table);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_SET_TABLE;
    req->msglen = sizeof(struct st_rf_table);
    p_table = (struct st_rf_table *)req->data;
    memcpy(p_table, param, sizeof(struct st_rf_table));

    //ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);
    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}

int ssv_send_priv_msg_rf_cmd(struct ssv_rftool_softc *srfc, struct ssv_rf_tool_param *param)
{
    struct ssv_rf_tool_param *rf_tool_param;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct ssv_rf_tool_param);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_OPS;
    req->msglen = sizeof(struct ssv_rf_tool_param);
    rf_tool_param = (struct ssv_rf_tool_param *)req->data;
    memcpy(rf_tool_param, param, sizeof(struct ssv_rf_tool_param));
    
    //ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);
    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}
int ssv_send_priv_msg_rf_cmd_wait_resp(struct ssv_rftool_softc *srfc, struct ssv_rf_tool_param *param)
{
    struct ssv_rf_tool_param *rf_tool_param;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct ssv_rf_tool_param);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_OPS;
    req->msglen = sizeof(struct ssv_rf_tool_param);
    rf_tool_param = (struct ssv_rf_tool_param *)req->data;
    memcpy(rf_tool_param, param, sizeof(struct ssv_rf_tool_param));
    
    //ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);


    if (!wait_for_completion_interruptible_timeout(&srfc->ssv_rftool_cmd_done, 1000)) 
    {
            SSV_LOG_DBG("wait for rf cmd %d\n", rf_tool_param->rf_cmd); 
    }

    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}
int ssv_send_priv_msg_rfble_cmd(struct ssv_rftool_softc *srfc, struct ssv_rfble_tool_param *param)
{
    struct ssv_rfble_tool_param *rfble_tool_param;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct ssv_rfble_tool_param);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_OPS;
    req->msglen = sizeof(struct ssv_rfble_tool_param);
    rfble_tool_param = (struct ssv_rfble_tool_param *)req->data;
    memcpy(rfble_tool_param, param, sizeof(struct ssv_rfble_tool_param));
    
    //ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);
    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}
int ssv_send_priv_msg_rfble_cmd_wait_resp(struct ssv_rftool_softc *srfc, struct ssv_rfble_tool_param *param)
{
    struct ssv_rfble_tool_param *rfble_tool_param;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct ssv_rfble_tool_param);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_OPS;
    req->msglen = sizeof(struct ssv_rfble_tool_param);
    rfble_tool_param = (struct ssv_rfble_tool_param *)req->data;
    memcpy(rfble_tool_param, param, sizeof(struct ssv_rfble_tool_param));
    
    //ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);

    if (!wait_for_completion_interruptible_timeout(&srfc->ssv_rftool_cmd_done, 1000)) 
    {
            SSV_LOG_DBG("wait for rfble cmd %d\n", rfble_tool_param->rfble_cmd); 
    }
    
    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}

void _ssv_fwreset_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len, u32 msg_type);
int ssv_send_priv_msg_set_pll_phy_rf(struct ssv_rftool_softc *srfc, struct ssv_softc *sc, struct ssv_rf_cali *param)
{
    struct ssv_rf_cali *p_cali;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct ssv_rf_cali);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_INIT_TABLE;
    req->msglen = sizeof(struct ssv_rf_cali);
    p_cali = (struct ssv_rf_cali *)req->data;
    memcpy(p_cali, param, sizeof(struct ssv_rf_cali));

    if (sc->recovery_flag == true) {
        struct fw_reset_cmd *reset_cmd;
        reset_cmd = kzalloc(msg_total_len + sizeof(struct fw_reset_cmd) + 1, GFP_KERNEL);
        reset_cmd->msg_type = E_IPC_TYPE_PRIV_MSG;
        reset_cmd->msg_len = msg_total_len;
        memcpy(reset_cmd->data, req, msg_total_len);
        list_add_tail(&reset_cmd->list, &sc->reset_cmd.list);
    }
    
    //ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);
    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}

#if 0 //reserved for low power
int ssv_send_priv_msg_agc_reg_table(struct ssv_softc *sc)
{
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG) + sizeof(ssv6030b1_agc_setting);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);

    //SSV_LOG_DBG("ssv6030b1_agc_setting size %ld\n", sizeof(ssv6030b1_agc_setting));
    if(!req)
    {
        return -ENOMEM;
    }
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_AGC_REG_TABLE;
    req->msglen = sizeof(ssv6030b1_agc_setting);
    memcpy(req->data, ssv6030b1_agc_setting, sizeof(ssv6030b1_agc_setting));
    ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    if(req)  {
        kfree(req);
        req = NULL;
    }
    return 0;
}

int ssv_send_priv_msg_rfphy_reg_talbe(struct ssv_softc *sc)
{
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG) + sizeof(ssv6030b1_phy_setting);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);

    //SSV_LOG_DBG("ssv6030b1_phy_setting size %ld\n", sizeof(ssv6030b1_phy_setting));
    if(!req)
    {
        return -ENOMEM;
    }
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFPHY_REG_TABLE;
    req->msglen = sizeof(ssv6030b1_phy_setting);
    memcpy(req->data, ssv6030b1_phy_setting, sizeof(ssv6030b1_phy_setting));
    ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    if(req)  {
        kfree(req);
        req = NULL;
    }
    return 0;
}
#endif
int ssv_send_priv_msg_rf_calib_result_backup(struct ssv_softc *sc)
{
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);

    //SSV_LOG_DBG("ssv6030b1_phy_setting size %ld\n", sizeof(ssv6030b1_phy_setting));
    if(!req)
        return -ENOMEM;
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_RFCALIB_RESULT_BACKUP;
    req->msglen = 0;
    //req->msglen = ssv_calibr_rsult_info.size;
    //memcpy(req->data, ssv_calibr_rsult_info.ptr_calib_result, ssv_calibr_rsult_info.size);
    ssv_private_msg_to_hci(sc, (u8*)req, msg_total_len);
    if(req)  {
        kfree(req);
        req = NULL;
    }
    return 0;
}
 

void ssv_chg_xtal_freq_offset(struct ssv_hw *sh, u8 xtal)
{
    int i;
    u8 freq_xi = sh->rf_conf_table.rf_config[2].freq_xi;
    u8 freq_xo = sh->rf_conf_table.rf_config[2].freq_xo;
    //SSV_LOG_DBG("xtal offset %d %d %d %d %d\n", ssv_cfg.thermal_xtal_offset[0], ssv_cfg.thermal_xtal_offset[1], ssv_cfg.thermal_xtal_offset[2], ssv_cfg.thermal_xtal_offset[3], ssv_cfg.thermal_xtal_offset[4]);
    if(xtal != 0)
    {
	freq_xi = xtal;
        freq_xo = xtal;
    }

    for(i=0; i<RF_THREMAL_TABLE_NUM; i++)
    {
        sh->rf_conf_table.rf_config[i].freq_xi = freq_xi+ssv_cfg.thermal_xtal_offset[i];
        sh->rf_conf_table.rf_config[i].freq_xo = freq_xo+ssv_cfg.thermal_xtal_offset[i];
    }
}

void ssv_chg_gain_b_mode(struct ssv_hw *sh, u8 gain)
{
    if(gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[0] = gain;
    }
}
void ssv_chg_gain_g_mode(struct ssv_hw *sh, u8 gain)
{
    if(gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[1] = gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[2] = gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[3] = gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[4] = gain;
    }
}
void ssv_chg_gain_n_mode(struct ssv_hw *sh, u8 bw20_gain,  u8 bw40_gain)
{
    if(bw20_gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[5] = bw20_gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[6] = bw20_gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[7] = bw20_gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[8] = bw20_gain;
    }
    // set ht20 rate gain
    if(bw40_gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[9] = bw40_gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[10] = bw40_gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[11] = bw40_gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[12] = bw40_gain;
    }
}
void ssv_chg_gain_su_mode(struct ssv_hw *sh, u8 bw20_gain,  u8 bw40_gain)
{
    // set su20 rate gain
    if(bw20_gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[13] = bw20_gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[14] = bw20_gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[15] = bw20_gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[16] = bw20_gain;
        sh->rf_conf_table.rate_gain_table.gain[17] = bw20_gain+OFDM_RATE_GAIN_256QAM_OFFSET;
    }
    // set su40 rate gain
    if(bw40_gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[18] = bw40_gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[19] = bw40_gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[20] = bw40_gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[21] = bw40_gain;
        sh->rf_conf_table.rate_gain_table.gain[22] = bw40_gain+OFDM_RATE_GAIN_256QAM_OFFSET;
    }
}
void ssv_chg_gain_tb_mode(struct ssv_hw *sh, u8 bw20_gain,  u8 bw40_gain)
{
    // set tb20 rate gain
    if(bw20_gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[23] = bw20_gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[24] = bw20_gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[25] = bw20_gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[26] = bw20_gain;
        sh->rf_conf_table.rate_gain_table.gain[27] = bw20_gain+OFDM_RATE_GAIN_256QAM_OFFSET;
    }
    // set tb40 rate gain
    if(bw40_gain != 0)
    {
        sh->rf_conf_table.rate_gain_table.gain[28] = bw40_gain+OFDM_RATE_GAIN_BPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[29] = bw40_gain+OFDM_RATE_GAIN_QPSK_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[30] = bw40_gain+OFDM_RATE_GAIN_16QAM_OFFSET;
        sh->rf_conf_table.rate_gain_table.gain[31] = bw40_gain;
        sh->rf_conf_table.rate_gain_table.gain[32] = bw40_gain+OFDM_RATE_GAIN_256QAM_OFFSET;
    }
}
void ssv_chg_band_gain(struct ssv_hw *sh, u8 power_ch_1_7, u8 power_ch_8_14)
{
    int i;

    u8 gain0 = sh->rf_conf_table.rf_config[2].band_gain[0];
    u8 gain1 = sh->rf_conf_table.rf_config[2].band_gain[1];
    u8 gain2 = sh->rf_conf_table.rf_config[2].band_gain[2];
    u8 gain3 = sh->rf_conf_table.rf_config[2].band_gain[3];
    u8 gain4 = sh->rf_conf_table.rf_config[2].band_gain[4];
    u8 gain5 = sh->rf_conf_table.rf_config[2].band_gain[5];
    u8 gain6 = sh->rf_conf_table.rf_config[2].band_gain[6];
    //SSV_LOG_DBG("wifi offset %d %d %d %d %d\n", ssv_cfg.thermal_wifi_gain_offset[0], ssv_cfg.thermal_wifi_gain_offset[1], ssv_cfg.thermal_wifi_gain_offset[2], ssv_cfg.thermal_wifi_gain_offset[3], ssv_cfg.thermal_wifi_gain_offset[4]);

    if(power_ch_1_7 != 0)
    {
        gain0 = power_ch_1_7;
        gain1 = power_ch_1_7;
        gain2 = power_ch_1_7;
        gain3 = power_ch_1_7;
    }
    if(power_ch_8_14 != 0)
        {
        gain4 = power_ch_8_14;
        gain5 = power_ch_8_14;
        gain6 = power_ch_8_14;
    }

        for(i=0; i<RF_THREMAL_TABLE_NUM; i++)
        {
        sh->rf_conf_table.rf_config[i].band_gain[0] = gain0+ssv_cfg.thermal_wifi_gain_offset[i];
        sh->rf_conf_table.rf_config[i].band_gain[1] = gain1+ssv_cfg.thermal_wifi_gain_offset[i];
        sh->rf_conf_table.rf_config[i].band_gain[2] = gain2+ssv_cfg.thermal_wifi_gain_offset[i];
        sh->rf_conf_table.rf_config[i].band_gain[3] = gain3+ssv_cfg.thermal_wifi_gain_offset[i];
        sh->rf_conf_table.rf_config[i].band_gain[4] = gain4+ssv_cfg.thermal_wifi_gain_offset[i];
        sh->rf_conf_table.rf_config[i].band_gain[5] = gain5+ssv_cfg.thermal_wifi_gain_offset[i];
        sh->rf_conf_table.rf_config[i].band_gain[6] = gain6+ssv_cfg.thermal_wifi_gain_offset[i]; 
    }
    
    // 6030B1 not support 5G band.
}
void ssv_chg_ble_power(struct ssv_hw *sh, u32 ble_power)
{
    int i;

    u8 ble_pow_0 = sh->rf_conf_table.ble_rf_config[2].band_gain[0];
    u8 ble_pow_1 = sh->rf_conf_table.ble_rf_config[2].band_gain[1];

    if(ble_power != 0)
    {
        ble_pow_0 = ble_power;
        ble_pow_1 = ble_power;
    }
    //SSV_LOG_DBG("wifi offset %d %d %d %d %d\n", ssv_cfg.thermal_ble_gain_offset[0], ssv_cfg.thermal_ble_gain_offset[1], ssv_cfg.thermal_ble_gain_offset[2], ssv_cfg.thermal_ble_gain_offset[3], ssv_cfg.thermal_ble_gain_offset[4]);

    for(i=0; i<RF_THREMAL_TABLE_NUM; i++)
            {
        sh->rf_conf_table.ble_rf_config[i].band_gain[0] = ble_pow_0+ssv_cfg.thermal_ble_gain_offset[i];
        sh->rf_conf_table.ble_rf_config[i].band_gain[1] = ble_pow_1+ssv_cfg.thermal_ble_gain_offset[i];
    }
}

void ssv_chg_thermal_boundary(struct ssv_hw *sh, int *boundary)
{
    //SSV_LOG_DBG("boundary offset %d %d %d %d\n", boundary[0], boundary[1], boundary[2], boundary[3]);

    if( (boundary[0] == 0) && (boundary[1] == 0) && (boundary[2] == 0) && (boundary[3] == 0))
        return;
    if( (boundary[0]<=127) && (boundary[0]>=-128) )
        sh->rf_conf_table.temperature_boundary[0] = boundary[0];
    if( (boundary[1]<=127) && (boundary[1]>=-128) )
        sh->rf_conf_table.temperature_boundary[1] = boundary[1];
    if( (boundary[2]<=127) && (boundary[2]>=-128) )
        sh->rf_conf_table.temperature_boundary[2] = boundary[2];
    if( (boundary[3]<=127) && (boundary[3]>=-128) )
        sh->rf_conf_table.temperature_boundary[3] = boundary[3];
}

void ssv_chg_ref_power(struct ssv_hw *sh, u8 ref_power)
{
    //hetb power
    if(rftool_cfg.hetb_cali_power !=0)
        sh->rf_conf_table.abs_power = ref_power;
    
}
int ssv_update_rf_conf_table(struct ssv_hw *sh)
{
    //load default table ,poritory 1. bin, 2. efuse, 3. default
    memcpy(&sh->rf_conf_table, &def_rf_conf_table, sizeof(struct st_rf_table) );

    return 0;
}
int ssv_set_rf_conf_table(struct ssv_hw *sh) //phase out 
{
    struct ssv_rftool_softc* srfc = sh->srfc;
    ssv_send_priv_msg_rf_update_table(srfc, &sh->rf_conf_table);
    return 0;
}
int ssv_set_pll_phy_rf(struct ssv_hw *sh)
{
    struct ssv_rftool_softc* srfc = sh->srfc;
    int  ret = 0;

    struct ssv_rf_cali rf_cali = {0};
    struct ssv_softc *sc = sh->sc;
    
    // no support 5g
    rf_cali.support_5g = 0;
    // tempature 
    if (ssv_cfg.disable_fw_thermal == 1) {
        rf_cali.thermal = 0;
    } else {
        rf_cali.thermal = 1;
    }
    
    rf_cali.greentx_en = ssv_cfg.greentx_en;
    //rf_cali.disable_cci = (true == sh->cfg.disable_cci) ? 1 : 0;
    //rf_cali.xtal = ssv6020_turismoE_update_xtal(sh);
    //rf_cali.bus_clk = 120; 
    
    // update rf table 
    memcpy(&rf_cali.rf_table, &sh->rf_conf_table, sizeof(struct st_rf_table));
    // dcdc
    //if (sh->cfg.volt_regulator == SSV6XXX_VOLT_LDO_CONVERT) 
    //    ptr->rf_table.dcdc_flag = 0;
    //else
    //    ptr->rf_table.dcdc_flag = 1;

    ret = ssv_send_priv_msg_set_pll_phy_rf(srfc, sc, &rf_cali);
    return ret;
}

int ssv_kdump_rf_conf_table(struct ssv_hw *sh)
{
        int i, j;
        uint8_t *rate_gain_table = 0;
               
        SSV_LOG_DBG("=======================\n");
        SSV_LOG_DBG("Signature: %s\n", sh->rf_conf_table.signature);
        SSV_LOG_DBG("Version: 0x%x\n", sh->rf_conf_table.version);
        //SSV_LOG_DBG("DCDC flag: %d\n", sh->rf_conf_table.dcdc_flag);
        SSV_LOG_DBG("=======================\n");

        for(j=0; j<5; j++)
        {
            SSV_LOG_DBG("%s:\t", thermal_name[j]);
            for(i=0; i<7; i++)
            {
                SSV_LOG_DBG_ONE_LINE(" %2d", sh->rf_conf_table.rf_config[j].band_gain[i]);
            }
            SSV_LOG_DBG_ONE_LINE(" %3d %3d\n", sh->rf_conf_table.rf_config[j].freq_xi, sh->rf_conf_table.rf_config[j].freq_xi);
        }

        SSV_LOG_DBG("=======================\n");
         //BLE
        for(j=0; j<RF_THREMAL_TABLE_NUM; j++)
        {
            SSV_LOG_DBG("%s %s:\t", "BLE", thermal_name[j]);
            for(i=0; i<2; i++)
            {
                SSV_LOG_DBG_ONE_LINE(" %2d", sh->rf_conf_table.ble_rf_config[j].band_gain[i]);
            }   
            SSV_LOG_DBG_ONE_LINE("\n");
        }
        
        rate_gain_table = sh->rf_conf_table.rate_gain_table.gain;
        SSV_LOG_DBG("===================\n");
        SSV_LOG_DBG("B Gain:\t %2d\n", rate_gain_table[0]);
        SSV_LOG_DBG("G Gain:\t %2d, %2d, %2d, %2d\n", rate_gain_table[1], rate_gain_table[2], rate_gain_table[3], rate_gain_table[4]);
        SSV_LOG_DBG("20N Gain:\t %2d, %2d, %2d, %2d\n", rate_gain_table[5], rate_gain_table[6], rate_gain_table[7], rate_gain_table[8]);
        SSV_LOG_DBG("40N Gain:\t %2d, %2d, %2d, %2d\n", rate_gain_table[9], rate_gain_table[10], rate_gain_table[11], rate_gain_table[12]);
        SSV_LOG_DBG("20HESU Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[13], rate_gain_table[14], rate_gain_table[15], rate_gain_table[16], rate_gain_table[17]);
        SSV_LOG_DBG("40HESU Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[18], rate_gain_table[19], rate_gain_table[20], rate_gain_table[21], rate_gain_table[22]);
        SSV_LOG_DBG("20HETB Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[23], rate_gain_table[24], rate_gain_table[25], rate_gain_table[26], rate_gain_table[27]);
        SSV_LOG_DBG("40HETB Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[28], rate_gain_table[29], rate_gain_table[30], rate_gain_table[31], rate_gain_table[32]);
        SSV_LOG_DBG("Temperature Boundary:\t %d, %d, %d, %d\n", sh->rf_conf_table.temperature_boundary[0], sh->rf_conf_table.temperature_boundary[1], sh->rf_conf_table.temperature_boundary[2], sh->rf_conf_table.temperature_boundary[3]);
        SSV_LOG_DBG("===================\n");  
    return 0;
}
int ssv_dump_rf_conf_table(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->cmd_data;
        int i, j;
        uint8_t *rate_gain_table = 0;
        
        snprintf_res(cmd_data,"=======================\n");
        snprintf_res(cmd_data,"Signature: %s\n", sh->rf_conf_table.signature);
        snprintf_res(cmd_data,"Version: 0x%x\n", sh->rf_conf_table.version);
        //snprintf_res(cmd_data,"DCDC flag: %d\n", sh->rf_conf_table.dcdc_flag);
        snprintf_res(cmd_data,"=======================\n");

        for(j=0; j<RF_THREMAL_TABLE_NUM; j++)
        {
            snprintf_res(cmd_data,"%s:\t", thermal_name[j]);
            for(i=0; i<7; i++)
    {
                snprintf_res(cmd_data," %2d", sh->rf_conf_table.rf_config[j].band_gain[i]);
    }
            snprintf_res(cmd_data," %3d %3d\n", sh->rf_conf_table.rf_config[j].freq_xi, sh->rf_conf_table.rf_config[j].freq_xi);
        }

        snprintf_res(cmd_data,"=======================\n");
         //BLE
        for(j=0; j<RF_THREMAL_TABLE_NUM; j++)
        {
            snprintf_res(cmd_data,"%s %s:\t", "BLE", thermal_name[j]);
            for(i=0; i<2; i++)
            {
                snprintf_res(cmd_data," %2d", sh->rf_conf_table.ble_rf_config[j].band_gain[i]);
            }   
            snprintf_res(cmd_data,"\n");
        }
        rate_gain_table = sh->rf_conf_table.rate_gain_table.gain;
        snprintf_res(cmd_data,"===================\n");
        snprintf_res(cmd_data,"B Gain:\t\t %2d\n", rate_gain_table[0]);
        snprintf_res(cmd_data,"G Gain:\t\t %2d, %2d, %2d, %2d\n", rate_gain_table[1], rate_gain_table[2], rate_gain_table[3], rate_gain_table[4]);
        snprintf_res(cmd_data,"20N Gain:\t %2d, %2d, %2d, %2d\n", rate_gain_table[5], rate_gain_table[6], rate_gain_table[7], rate_gain_table[8]);
        snprintf_res(cmd_data,"40N Gain:\t %2d, %2d, %2d, %2d\n", rate_gain_table[9], rate_gain_table[10], rate_gain_table[11], rate_gain_table[12]);
        snprintf_res(cmd_data,"20HESU Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[13], rate_gain_table[14], rate_gain_table[15], rate_gain_table[16], rate_gain_table[17]);
        snprintf_res(cmd_data,"40HESU Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[18], rate_gain_table[19], rate_gain_table[20], rate_gain_table[21], rate_gain_table[22]);
        snprintf_res(cmd_data,"20HETB Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[23], rate_gain_table[24], rate_gain_table[25], rate_gain_table[26], rate_gain_table[27]);
        snprintf_res(cmd_data,"40HETB Gain:\t %2d, %2d, %2d, %2d, %2d\n", rate_gain_table[28], rate_gain_table[29], rate_gain_table[30], rate_gain_table[31], rate_gain_table[32]);
        snprintf_res(cmd_data,"Temperature Boundary:\t %d, %d, %d, %d\n", sh->rf_conf_table.temperature_boundary[0], sh->rf_conf_table.temperature_boundary[1], sh->rf_conf_table.temperature_boundary[2], sh->rf_conf_table.temperature_boundary[3]);
        snprintf_res(cmd_data,"===================\n");    
        
    return 0;
}

void ssv_6030_rf(struct ssv_hw *sh, int argc, char *argv[])
{
    //struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 regval = 0;
    char *endp;
    struct ssv_rftool_softc *sc = sh->srfc;
    int ch = 0;
    enum nl80211_channel_type ch_type;
    struct ssv_rf_tool_param rf_tool_param = {0};
    //struct sk_buff *skb = NULL;
    //struct cfg_host_cmd *host_cmd;
    struct ssv_cmd_data *cmd_data = &sh->cmd_data;
/*
    unsigned char rate_tbl[] = {
        0x00,0x01,0x02,0x03,                        // B mode long preamble [0~3]
        0x00,0x12,0x13,                             // B mode short preamble [4~6], no 2M short preamble
        0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,    // G mode [7~14]
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,    // N mode HT20 long GI mixed format [15~22]
        0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,    // N mode HT20 short GI mixed format  [23~30]
        0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,    // N mode HT40 long GI mixed format [31~38]
        0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,    // N mode HT40 short GI mixed format  [39~46]
    };
*/
    if (argc < 2)
        goto out;

    else if(!strcmp(argv[1], "engmode"))
    {
        if (argc == 3) {
            regval = simple_strtoul(argv[2], &endp, 0);

            if (1) 
            { 
                rf_engmode = regval;
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_ENGMODE;
                rf_tool_param.engmode = regval;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set engmode to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf engmode [0|1]\n");
        }
        return;

    } 
    else if(!strcmp(argv[1], "disablethermal"))
    {
        if (argc == 3)
        {
            regval = simple_strtoul(argv[2], &endp, 0);
            regval = (0 == regval) ? 0 : 1;

            if (1) 
            {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_THERMAL;
                rf_tool_param.disable_thermal = regval;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"\n   rf thermal %d\n", regval);
            } else {
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
		    }	

            return;

        } else {
            snprintf_res(cmd_data,"\n\t Incorrect rf rate set format\n");
            return;
        }
    
    } 
    else if(!strcmp(argv[1], "temp"))
    {
        if (argc == 2){

            if (1) 
            {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_TEMPERATURE;
                rf_tool_param.wait_resp = 1;

                ssv_send_priv_msg_rf_cmd_wait_resp(sc, &rf_tool_param);
                snprintf_res(cmd_data,"temperature = %d\n", rftool_cfg.temperature);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf temp\n");
        }
        return;

    } 
    else if(!strcmp(argv[1], "dump"))
    {
        ssv_dump_rf_conf_table(sh);
        return; 
    }
    
    if(rf_engmode == 0)
    {
        snprintf_res(cmd_data,"\nPlease Enter Engineering mode First!!!\n");
        return;
    }
    
    if (!strcmp(argv[1], "tx")) {

        if (argc == 3)
            regval = simple_strtoul(argv[2], &endp, 0);
        else
            regval = 200;

        if (1) {
            rf_tool_param.rf_cmd = SSV6XXX_RFPHY_CMD_RF_TOOL_TX;
            rf_tool_param.count = 0xffffffff;
            rf_tool_param.interval = regval;
        
            ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
            snprintf_res(cmd_data,"\n   RF TX\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

        return;

    }
    else if (!strcmp(argv[1], "rx")) 
    {
        //sc->sc_flags |= SC_OP_BLOCK_CNTL;
        //sc->sc_flags |= SC_OP_CHAN_FIXED; // fixed channel
        snprintf_res(cmd_data,"\n   RF RX\n");
        return ;

    } 

    else if (!strcmp(argv[1], "rxreset")) 
    {
        if (argc == 2) 
        {
            if (1) 
            {
              rf_tool_param.rf_cmd = SSV6XXX_RFPHY_CMD_RF_TOOL_RX_RESET;
               rf_tool_param.rx_reset = 1;
            
             ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"\n   RF RX Reset\n");
            } else {
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
            }
        }
        return;
        
    } 

    else if(!strcmp(argv[1], "stop")) 
    {
        if (1) 
        {
         
            rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_STOP;
            ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
            snprintf_res(cmd_data,"\n   RF STOP\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        return;
    
    } 
    else if (!strcmp(argv[1], "ch")) 
    {
        
        if ((argc == 3) || (argc == 4)) {
            ch = simple_strtoul(argv[2], &endp, 0);
            ch_type = NL80211_CHAN_HT20;

            if (argc == 4) {
                if (!strcmp(argv[3], "bw40")) {
                    if ((ch == 3) || (ch == 4) || (ch == 5) || (ch == 6) ||
                        (ch == 7) || (ch == 8) || (ch == 9) || (ch == 10) ||
                        (ch == 11)) {
                    
                        ch_type = NL80211_CHAN_HT40PLUS;
                        ch = ch - 2; // find center chan
                    }
                }
            }

            if (1) {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_CH;
                rf_tool_param.ch = ch;
                rf_tool_param.ch_type = ch_type;
                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"\n RF ch %d, ch_type %d\n", ch, (int)ch_type);
            } else {
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
            }
        }
        return;

    } 
    else if(!strcmp(argv[1], "rate"))
    {
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);
            if ((regval != 4) && (regval <= 101 )) 
            {

                if (1) 
                {
                    rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_RATE;
                    rf_tool_param.rate = regval;

                    ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                    snprintf_res(cmd_data,"\n   rf rate index %d\n", regval);
                } else{
                    snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
				}

            } else {
                snprintf_res(cmd_data,"Not support rf rate index %d\n", regval);
            }
            return;

        } else {
            snprintf_res(cmd_data,"\n\t Incorrect rf rate set format\n");
            return;
        }

    } 
    else if(!strcmp(argv[1], "freq"))
    {
        if (argc == 3)
        {
            regval = simple_strtoul(argv[2], &endp, 0);

            if (1) 
            {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_FREQ;
                rf_tool_param.freq = regval;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set cbanki/cbanko to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf freq [value]\n");
        }
        return;

    } 
    else if(!strcmp(argv[1], "rfreq"))
    {
        if (argc == 2){
            sh->hci_ops->hci_read_word(sh->hci_priv, 0x08d01000, &regval);
            
            snprintf_res(cmd_data,"Get freq 0x%x/0x%x\n", (regval>>8)&0xff, (regval>>16)&0xff);
        } else {
            snprintf_res(cmd_data,"./cli rf rfreq\n");
        }
        return;

    } 
    else if(!strcmp(argv[1], "r_dacgain"))
    {
        if (argc == 2){
            sh->hci_ops->hci_read_word(sh->hci_priv, 0x0810edac, &regval);
            
            snprintf_res(cmd_data,"Get dacgain 0x%x\n", regval);
        } else {
            snprintf_res(cmd_data,"./cli rf r_dacgain\n");
        }
        return;

    } 
    else if(!strcmp(argv[1], "r_rategain"))
    {
        if (argc == 2){
            int rategain = 0;
            sh->hci_ops->hci_read_word(sh->hci_priv, 0x08160208, &regval);
            if( (regval&0x80)==0x80 )
            {
                rategain = regval - 0xFF;
                snprintf_res(cmd_data,"Get rategain %d\n", rategain);
            }
            else
                snprintf_res(cmd_data,"Get rategain %d\n", regval);
        } else {
            snprintf_res(cmd_data,"./cli rf r_rategain\n");
        }
        return;

    } 
#if 0
    else if(!strcmp(argv[1], "dcdc"))
    {
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);
            regval = (regval == 0) ? 0 : 1;

            skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
            if (skb != NULL) {
                skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_DCDC;
                host_cmd->blocking_seq_no =
                    (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_DCDC);
                host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);

                rf_tool_param = (struct ssv_rf_tool_param *)host_cmd->un.dat8;
                memset(rf_tool_param, 0, sizeof(struct ssv_rf_tool_param));
                rf_tool_param->dcdc = regval;

                HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data,"Set dcdc to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf dcdc [value]\n");
        }
        return;

    }
#endif
    else if(!strcmp(argv[1], "dacgain"))
    {
        if (argc == 3)
        {
            regval = simple_strtoul(argv[2], &endp, 0);
            if (regval > 31)
                regval = 31;

            if (1) {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_DACGAIN;
                rf_tool_param.dacgain = regval;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set dgcgain to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf dacgain [value]\n");
        }
        return;
    
    }
    else if(!strcmp(argv[1], "rategain"))
    {
        if (argc == 3){
            int16_t power = simple_strtol(argv[2], &endp, 0);
            
            if (1) {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_RATEPOWER;
                rf_tool_param.ratepower = power;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set rategain to %d\n", power);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf rategain [value]\n");
        }
        return;  
    }
#if 0
    else if(!strcmp(argv[1], "ratebgain"))
    {
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);
            if (regval > 15)
                regval = 15;

            skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
            if (skb != NULL) {
                skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_RATEBGAIN;
                host_cmd->blocking_seq_no =
                    (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_RATEBGAIN);
                host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);

                rf_tool_param = (struct ssv_rf_tool_param *)host_cmd->un.dat8;
                memset(rf_tool_param, 0, sizeof(struct ssv_rf_tool_param));
                rf_tool_param->ratebgain = regval;

                HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data,"Set rate b gain to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf ratebgain [value]\n");
        }
        return;  
    }
#endif
    else if(!strcmp(argv[1], "padpd"))
    {
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);
            regval = (regval == 0) ? 0 : 3;

            if(1)
            {
                sh->hwif_ops->writereg(sh->dev, 0x0810ed1c, regval);
                
                snprintf_res(cmd_data,"Set padpd to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf padpd [value]\n");
        }
        return;
    }
    else if(!strcmp(argv[1], "wfble"))
    {
        if (argc == 3) {
        #define RF_TOOL_MODE_WIFI       1
        #define RF_TOOL_MODE_BLE        0
            regval = simple_strtoul(argv[2], &endp, 0);

            if (1) 
            {
                
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_WFBLE;
                rf_tool_param.wfble = (regval == 0) ? RF_TOOL_MODE_WIFI : RF_TOOL_MODE_BLE;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set wfble to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf wfble [0|1]\n");
        }
        return;

    } 
    else if(!strcmp(argv[1], "count"))
    {
        if  (argc == 3) {
            
            if (1) 
            {
                u32 count = 0;
                u32 err = 0;
                u32 rssi = 0;
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_RX_MIB;
                if ((!strcmp(argv[2], "0")) || (!strcmp(argv[2], "2") ))
                    rf_tool_param.rx_mib = 1;
                else
                    rf_tool_param.rx_mib = 0;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                
                #ifdef CONFIG_HWIF_AND_HCI
                    if (sh->hci_ops->hci_read_word)
                    {
                        sh->hci_ops->hci_read_word(sh->hci_priv, 0x08160080, &count);
                        sh->hci_ops->hci_read_word(sh->hci_priv, 0x08160084, &err);
                        sh->hci_ops->hci_read_word(sh->hci_priv, 0x08160094, &rssi);
                        rssi = rssi&0xff;
                    }
                #else    
                    if (ssv_hw->hwif_ops->readreg) 
                    {
                        sh->hwif_ops->readreg(sh->dev, 0x08160080, &count);
                        sh->hwif_ops->readreg(sh->dev, 0x08160084, &err);
                        sh->hwif_ops->readreg(sh->dev, 0x08160094, &rssi);
                        rssi = rssi&0xff;
                    }
                #endif
                // show result
                snprintf_res(cmd_data,"count = %d\n", count);
                //snprintf_res(cmd_data,"err = %d\n", err);
                snprintf_res(cmd_data,"rssi = -%d\n", 255-rssi);
                return;
            } else {
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
                return;
            }
        }
        snprintf_res(cmd_data,"\n\t./cli rf count 0|1\n");
        return;  
    } 
    else if(!strcmp(argv[1], "hetbruid"))
    {
        if (argc == 3)
        {
            regval = simple_strtoul(argv[2], &endp, 0);

            if (1) {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_HETBRUIDX;
                rf_tool_param.hetbruid = regval;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set hetbruid to %d\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf hetbruid [value]\n");
        }
        return; 
    }
    else if(!strcmp(argv[1], "tonegen"))
    {
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);

            if (1) 
            {
                rf_tool_param.rf_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TONE_GEN;
                rf_tool_param.tone_gen = regval;

                ssv_send_priv_msg_rf_cmd(sc, &rf_tool_param);
                snprintf_res(cmd_data,"Set tone gen to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf tonegen [value]\n");
        }
        return;

    }
#if 0
    else if(!strcmp(argv[1], "phy_txgen")) 
    {

        ssv6020_cmd_turismoE_rfphy_ops_start(sc, 1);
        sc->sc_flags |= SC_OP_BLOCK_CNTL;
        sc->sc_flags |= SC_OP_CHAN_FIXED; // fixed channel
        snprintf_res(cmd_data,"\n   RF TX\n");
        return;
	}else if(!strcmp(argv[1], "block")){
		sc->sc_flags |= SC_OP_BLOCK_CNTL;
        sc->sc_flags |= SC_OP_CHAN_FIXED;
        snprintf_res(cmd_data,"\n\t block control form system\n");
        return;
    } else if(!strcmp(argv[1], "unblock")){
   		sc->sc_flags &= ~SC_OP_BLOCK_CNTL;
        sc->sc_flags &= ~SC_OP_CHAN_FIXED;
        ssv6020_cmd_turismoE_rfphy_ops_stop(sc);
        snprintf_res(cmd_data,"\n\t unblock control form system\n");
        return;
	}
#endif
	else 
	{

        snprintf_res(cmd_data, 
                "\n\t./cli rf wfble|engmode|tx|rx|rxreset|stop|ch|rate|disablethermal|freq|rfreq|dacgain|r_dacgain|rategain|r_rategain|count|hetbruid|padpd|tonegen|temp|dump\n");
        return;
    }

out:
    sh->hci_ops->hci_read_word(sh->hci_priv, 0x0810e478, &ch);
    ch = ch&0xff;
    snprintf_res(cmd_data,"\n\t Current RF tool settings: ch %d\n", ch);

    //if (sc->sc_flags && SC_OP_BLOCK_CNTL) {
    //    ssv_snprintf_res(cmd_data,"\t system control is blocked\n");
    //} else {
    //    ssv_snprintf_res(cmd_data,"\t WARNING system control is not blocked\n");
    //}
}
void ssv_6030_rfble(struct ssv_hw *sh, int argc, char *argv[])
{
    //struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 value = 0;
    char *endp;
    struct ssv_rftool_softc *sc = sh->srfc;
    struct ssv_rfble_tool_param rfble_tool_param = {0};
    //struct sk_buff *skb = NULL;
    //struct cfg_host_cmd *host_cmd;
    struct ssv_cmd_data *cmd_data = &sh->cmd_data;

    if (argc < 2) {
        snprintf_res(cmd_data,"\n rfble [tx|rate|ch|stop|rx|rxreset|count|pattern]\n");
        return;
    }

    if(rf_engmode == 0)
    {
        snprintf_res(cmd_data,"\nPlease Enter Engineering mode First!!!\n");
        return;
    }
    
    if (!strcmp(argv[1], "tx")) 
    {

        if (argc == 3)
            value = simple_strtoul(argv[2], &endp, 0);
        else
            value = 0;

        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_START;
            rfble_tool_param.count = value;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE TX\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

    } 
    else if (!strcmp(argv[1], "rate")) 
    {
        
        if (argc != 3) {
            snprintf_res(cmd_data,"\n rfble rate value\n");
            return;
        }

        value = simple_strtoul(argv[2], &endp, 0);
        if ((value < 0) || (value > 3)) {
            snprintf_res(cmd_data,"\n rate range 0 ~ 3\n");
            return;
        }

        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_RATE;
            rfble_tool_param.rate = value;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE rate %d\n", value);
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
    
    }
    else if (!strcmp(argv[1], "dacgain"))
    {
        
        if (argc != 3) {
            snprintf_res(cmd_data,"\n rfble dacgain value\n");
            return;
        }

        value = simple_strtoul(argv[2], &endp, 0);
        if ((value < 1) || (value > 31)) {
            snprintf_res(cmd_data,"\n dacgain range 1 ~ 31\n");
            return;
        }

        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_DACGAIN;
            rfble_tool_param.dacgain = value;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE dacgain %d\n", value);
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
    
    }
    else if (!strcmp(argv[1], "ch")) 
    {
        
        if (argc != 3) {
            snprintf_res(cmd_data,"\n rfble ch value\n");
            return;
        }

        value = simple_strtoul(argv[2], &endp, 0);
        
        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_CH;
            rfble_tool_param.chan = value;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE chan %d\n", value);
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
    
    } 
    else if (!strcmp(argv[1], "stop")) 
    {
        
        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_STOP;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE stop\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
    
    }
    else if (!strcmp(argv[1], "rxreset")) 
    {

        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_RESET;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE reset\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
    
    }
    else if (!strcmp(argv[1], "count")) 
    {

        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_COUNT;
            rfble_tool_param.wait_resp = 1;
            
            //ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            ssv_send_priv_msg_rfble_cmd_wait_resp(sc, &rfble_tool_param);
            // show result
            snprintf_res(cmd_data,"count = %d\n", rftool_cfg.ble_rx_count);
            snprintf_res(cmd_data,"err = %d\n", rftool_cfg.ble_rx_err_count);
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
    
    }  
    else if (!strcmp(argv[1], "rx")) 
    {
        snprintf_res(cmd_data,"\n   RFBLE RX\n");

    }
    else if (!strcmp(argv[1], "pattern")) 
    {
        
        if (argc != 3) {
            snprintf_res(cmd_data,"\n rfble pattern value\n");
            return;
        }

        value = simple_strtoul(argv[2], &endp, 0);
        if ((value < 0) || (value > 7)) {
            snprintf_res(cmd_data,"\n rate range 0 ~ 7\n");
            return;
        }

        if (1) 
        {
            rfble_tool_param.rfble_cmd = (u32)SSV6XXX_RFPHY_CMD_RFBLE_TOOL_PATTERN;
            rfble_tool_param.pattern = value;

            ssv_send_priv_msg_rfble_cmd(sc, &rfble_tool_param);
            snprintf_res(cmd_data,"\n   RFBLE pattern %d\n", value);
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

    }
    else
    {
        snprintf_res(cmd_data,"\n rfble [tx|rate|ch|stop|rx|rxreset|count|pattern]\n");
    }
    
    return;
}

int ssv_res_rf_update_to_cfg(void* param)
{
    struct ssv_rf_tool_param *tool_param = (struct ssv_rf_tool_param*)param;

    if(tool_param->rf_cmd < SSV6XXX_RFPHY_CMD_RFBLE_TOOL_RATE)  //rf response
    {
        switch(tool_param->rf_cmd)
        {
            case SSV6XXX_RFPHY_CMD_RF_TOOL_TEMPERATURE:
                //SSV_LOG_DBG("temperature === %d\n", tool_param->temperature);
                rftool_cfg.temperature = tool_param->temperature;
                break;
        }
    }
    else    //rfble response
    {
        struct ssv_rfble_tool_param *rfble_param = (struct ssv_rfble_tool_param*)param;
        
        switch(rfble_param->rfble_cmd)
        {
            case SSV6XXX_RFPHY_CMD_RFBLE_TOOL_COUNT:
                //SSV_LOG_DBG("count === %d\n", rfble_param->rx_count);
                //SSV_LOG_DBG("count === %d\n", rfble_param->rx_err_count);
                rftool_cfg.ble_rx_count = rfble_param->rx_count;
                rftool_cfg.ble_rx_err_count = rfble_param->rx_err_count;
                break;
        }
    }

    return 0;
}

u8 ssv_get_engmode(void)
{
    return rf_engmode;
}
