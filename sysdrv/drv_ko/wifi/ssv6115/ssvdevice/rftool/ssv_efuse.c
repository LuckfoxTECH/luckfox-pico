#include <linux/kthread.h>

#include "ssvdevice/dev.h"
#include <hwif/hwif.h>
#include "fmac/fmac.h"
#include "ssv_rftool.h"
#include "ipc_msg.h"

#include "ssv_cfg.h"
#include "ssv_efuse.h"
#include "ssv_debug.h"

struct _6030b1_efuse_st
{
    uint32_t sku_id;
    uint32_t asic_control;
    uint32_t reserve1;
    uint32_t mac1_3_6;
    uint32_t mac1_1_2:16;
    uint32_t mac2_5_6:16;
    uint32_t mac2_1_4;

    //Word 6
    uint32_t rate1_g:5;
    uint32_t rate1_n20:5;
    uint32_t tx_power1_ch_1_7:5;
    uint32_t tx_power1_ch_8_14:5;
    uint32_t hetb1_cal_power:5;
    uint32_t hetb1_cal_index:5;
    uint32_t empty1:2;
	
    //Word 7
    uint32_t rate2_g:5;
    uint32_t rate2_n20:5;
    uint32_t tx_power2_ch_1_7:5;
    uint32_t tx_power2_ch_8_14:5;
    uint32_t hetb2_cal_power:5;
    uint32_t hetb2_cal_index:5;
    uint32_t empty2:2;
    //Word8
    uint32_t ble_rf1:24;
    uint32_t xtal_freq1:8;

    //Word9
    uint32_t ble_rf2:24;
    uint32_t xtal_freq2:8;
	
    uint32_t rom_ctrl;

    //Word11
    uint32_t rate1_b:5;
    uint32_t rate1_n40:5;
    uint32_t rate1_su20:5;
    uint32_t rate1_su40:5;
    uint32_t rate1_tb20:5;
    uint32_t rate1_tb40:5;
    uint32_t empty3:2;
	
    //Word12
    uint32_t rate2_b:5;
    uint32_t rate2_n40:5;
    uint32_t rate2_su20:5;
    uint32_t rate2_su40:5;
    uint32_t rate2_tb20:5;
    uint32_t rate2_tb40:5;
    uint32_t empty4:2;
	
    uint32_t reg_patch_addr1;
    uint32_t reg_patch_value1;
    uint32_t reg_patch_addr2;
    uint32_t reg_patch_value2;
    uint32_t reg_patch_addr3;
    uint32_t reg_patch_value3;
	
    uint32_t rom_patch_addr0;
    uint32_t rom_patch_value0;
    uint32_t rom_patch_addr1;
    uint32_t rom_patch_value1;
    uint32_t rom_patch_addr2;
    uint32_t rom_patch_value2;
    uint32_t rom_patch_addr3;
    uint32_t rom_patch_value3;

    //Word27
    uint16_t usb_pid;
    uint16_t usb_vid;

    uint32_t adc_cali1;
    uint32_t adc_cali2;
    uint32_t adc_cali3;
	
    //Word31
    uint32_t reserve2:24;
    uint32_t sku_id1:8;
};

struct ssv_efuse_info efuse_info;
extern struct ssv6xxx_cfg ssv_cfg;
extern struct ssv_rftool_cfg rftool_cfg;

extern int ssv_rftool_private_msg_to_hci(struct ssv_rftool_softc *srfc, u8 *msg_buffer, u32 msg_len);

int _ssv_eng_fw_chosen(void)
{
    if(ssv_cfg.firmware_choice==2)
    {
        //use ssv6x5x-sw_2.h
        return 1;
    }
    else if(memcmp("ssv6x5x-sw_2.bin", ssv_cfg.firmware_name, strlen("ssv6x5x-sw_2.bin")) == 0)
    {
        //use ssv6x5x-sw_2.bin
        //if ssv6x5x-sw_2.bin isn't exist, maybe has some problem
        return 1;
    }

    return 0;
}
static int ssv_send_priv_msg_efuse_cmd(struct ssv_rftool_softc *srfc, struct ssv_efuse_tool_param *param, u8 wait_resp)
{
#define MAX_BLOCKING_CMD_WAIT_PERIOD 1000
    struct ssv_efuse_tool_param *efuse_tool_param;
    u32 msg_total_len = sizeof(ST_IPC_PRIV_MSG)+sizeof(struct ssv_efuse_tool_param);
    ST_IPC_PRIV_MSG *req = kzalloc(msg_total_len+1, GFP_KERNEL);
    unsigned long expire = msecs_to_jiffies(MAX_BLOCKING_CMD_WAIT_PERIOD);
    
    req->msgid = E_IPC_PRIV_MSG_TYPE_CMD_EFUSE_OPS;
    req->msglen = sizeof(struct ssv_efuse_tool_param);
    efuse_tool_param = (struct ssv_efuse_tool_param *)req->data;
    memcpy(efuse_tool_param, param, sizeof(struct ssv_efuse_tool_param));

    ssv_rftool_private_msg_to_hci(srfc, (u8*)req, msg_total_len);

    if(wait_resp)
    {
        if (!wait_for_completion_interruptible_timeout(&srfc->ssv_rftool_cmd_done, expire)) 
        {
                SSV_LOG_DBG("No Response for efuse cmd %d\n", efuse_tool_param->efuse_cmd); 
        }
    }

    if(req) 
    {
        kfree(req);
        req = NULL;
    }
    return 0;
}

static int ssv_eng_send_priv_msg_efuse_cmd(struct ssv_rftool_softc *sc, struct ssv_efuse_tool_param *param, u8 wait_resp)
{
    if(_ssv_eng_fw_chosen() == 1)
    {
        return ssv_send_priv_msg_efuse_cmd(sc, param, wait_resp);
    }

    return 0;
}

static int _ssv_check_psk(u8 *psk, u8 len)
{
    int i = 0;
    int ret = 0;

    if(0 == len || 0 == psk)
        ret = -1;
    else if(rftool_cfg.psk_len != len)
        ret = -1;
    for(i=0; i<len; i++)
    {
        if(rftool_cfg.psk[i] != psk[i])
        {
            ret = -1;
            break;
        }  
    }

    return ret;
}
static int _ssv_check_item_free(struct ssv_rftool_softc *sc, u8 item)
{
    struct ssv_efuse_tool_param efuse_tool_param = {0};

    if(_ssv_eng_fw_chosen()==0)
    {
        return 0;
    }
    
    efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_CHECK_ITEM_FREE;
    efuse_tool_param.wait_resp = 1;
    efuse_tool_param.check_item_free = item;
    ssv_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);   
    return rftool_cfg.free_item;

}

void ssv_6030_efuse(struct ssv_hw *sh, int argc, char *argv[])
{
#define  snprintf_res ssv_snprintf_res
    struct ssv_cmd_data *cmd_data = &sh->cmd_data;
    char *endp;
    struct ssv_rftool_softc *sc = sh->srfc;
    struct ssv_efuse_tool_param efuse_tool_param = {0};

    int i = 0, value1 = 0, value2 = 0;
    unsigned char mac[6];
  
    if (!strcmp(argv[1], "writepsk")) 
    {
        if(_ssv_eng_fw_chosen() == 0)
        {
            snprintf_res(cmd_data, "SMAC don't support efuse write, Please use SMAC_2\n");
            return;
        }
        
        if (argc == 3) {
            
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data, "write psk range 1 ~ 32\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_PSK;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];

            rftool_cfg.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                rftool_cfg.psk[i] = argv[2][i];

            // show result
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
            snprintf_res(cmd_data, "write psk = %s, len %d\n", argv[2], strlen(argv[2]));

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    } 
    else if (!strcmp(argv[1], "readpsk")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_PSK;
                efuse_tool_param.wait_resp = 1;
                //memset(efuse_tool_param, 0x0, sizeof(struct ssv_efuse_tool_param));
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                // show result
                snprintf_res(cmd_data,"psk = %s, len %d\n", rftool_cfg.psk, rftool_cfg.psk_len);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    } 
    else if (!strcmp(argv[1], "readchipid")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_CHIPID;
                efuse_tool_param.wait_resp = 1;
                //memset(efuse_tool_param, 0x0, sizeof(struct ssv_efuse_tool_param));
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                // show result
                snprintf_res(cmd_data,"chip_id = 0x%08x\n", rftool_cfg.skuid);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "readmac")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_MAC;
                efuse_tool_param.wait_resp = 1;
                //memset(efuse_tool_param, 0x0, sizeof(struct ssv_efuse_tool_param));
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"efuse mac = %02X:%02X:%02X:%02X:%02X:%02X\n", 
                    rftool_cfg.efuse_mac[0], rftool_cfg.efuse_mac[1], rftool_cfg.efuse_mac[2],
                    rftool_cfg.efuse_mac[3], rftool_cfg.efuse_mac[4], rftool_cfg.efuse_mac[5]);
                
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    } 
    else if (!strcmp(argv[1], "writemac")) 
    {
        if (argc == 4) {
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = sscanf(argv[3], "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX", 
                &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 0)==0)
            {
                snprintf_res(cmd_data,"No free space for mac\n");
                return;
            }
        
            
            if (6 != value1) {
                snprintf_res(cmd_data,"write incorrect mac format\n");
                return;
            }
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_MAC;
                efuse_tool_param.wait_resp = 1;
                //memset(efuse_tool_param, 0x0, sizeof(struct ssv_efuse_tool_param));
                memcpy(efuse_tool_param.mac, mac, 6);
                efuse_tool_param.psk_len = strlen(argv[2]);
                for (i= 0; i < efuse_tool_param.psk_len; i++)
                    efuse_tool_param.psk[i] = argv[2][i];
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"write efuse mac = %02X:%02X:%02X:%02X:%02X:%02X\n", 
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    } 
    else if (!strcmp(argv[1], "r_2g_gain")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_TX_POWER1;
                efuse_tool_param.wait_resp = 1;
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"efuse txpower1 = %d, %d\n", rftool_cfg.power_ch_1_7, rftool_cfg.power_ch_8_14);
                
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    } 
    else if (!strcmp(argv[1], "w_2g_gain")) 
    {
        if (argc == 5) {
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) {
                snprintf_res(cmd_data,"ch1~ch7 gain range 1 ~ 31\n");
                return;
            }
            
            value2 = simple_strtoul(argv[4], &endp, 0);
            if ((value2 < 1) || (value2 > 31)) {
                snprintf_res(cmd_data,"ch8~ch14 gain range 1 ~ 31\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 2)==0)
            {
                snprintf_res(cmd_data,"No free space for 2g gain\n");
                return;
            }
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_TX_POWER1;
                efuse_tool_param.wait_resp = 0;
                efuse_tool_param.txpower1_gain2000 = value1;
                efuse_tool_param.txpower1_gain5200 = value2;
                efuse_tool_param.psk_len = strlen(argv[2]);
                for (i= 0; i < efuse_tool_param.psk_len; i++)
                    efuse_tool_param.psk[i] = argv[2][i];
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"write efuse txpower1 = %d,%d\n", value1, value2);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "readxtal")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_XTAL;
                efuse_tool_param.wait_resp = 1;
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"efuse xtal = %d\n", rftool_cfg.xtal);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    } 
    else if (!strcmp(argv[1], "writextal")) 
    {
        if (argc == 4) {
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 0) || (value1 > 255)) {
                snprintf_res(cmd_data,"xtal range 0 ~ 255\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 1)==0)
            {
                snprintf_res(cmd_data,"No free space for xtal\n");
                return;
            }
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_XTAL;
                efuse_tool_param.wait_resp = 0;
                efuse_tool_param.xtal = value1;
                efuse_tool_param.psk_len = strlen(argv[2]);
                for (i= 0; i < efuse_tool_param.psk_len; i++)
                    efuse_tool_param.psk[i] = argv[2][i];
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"write efuse xtal = %d\n", value1);
           
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_bpow")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_B_N40;
                efuse_tool_param.wait_resp = 1;
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"efuse rate gain b   = %d\n", rftool_cfg.gain_b);
                //snprintf_res(cmd_data,"efuse rate gain n40 = %d\n", rftool_cfg.gain_n40);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_bpow")) 
    { 
        if (argc == 4) {
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) {
                snprintf_res(cmd_data,"rate gain b range 1 ~ 31\n");
                return;
            }
            
            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 3)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain b\n");
                return;
            }          
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_B_N40;
                efuse_tool_param.wait_resp = 0;
                efuse_tool_param.rate_gain_b = value1;
                efuse_tool_param.rate_gain_n40 = 0;
                efuse_tool_param.psk_len = strlen(argv[2]);
                for (i= 0; i < efuse_tool_param.psk_len; i++)
                    efuse_tool_param.psk[i] = argv[2][i];
                ssv_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"write efuse rate gain b   = %d\n", value1);
                //snprintf_res(cmd_data,"write efuse rate gain n40 = %d\n", value2);
            
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_n40pow")) 
    {
        if (argc == 2) {
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_B_N40;
                efuse_tool_param.wait_resp = 1;
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                //snprintf_res(cmd_data,"efuse rate gain b   = %d\n", rftool_cfg.gain_b);
                snprintf_res(cmd_data,"efuse rate gain n40 = %d\n", rftool_cfg.gain_n40);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_n40pow")) 
    { 
        if (argc == 4) {
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) {
                snprintf_res(cmd_data,"rate gain n40 range 1 ~ 31\n");
                return;
            }
            
            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 14)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain n40\n");
                return;
            }  
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_B_N40;
                efuse_tool_param.wait_resp = 0;
                efuse_tool_param.rate_gain_b = 0;
                efuse_tool_param.rate_gain_n40 = value1;
                efuse_tool_param.psk_len = strlen(argv[2]);
                for (i= 0; i < efuse_tool_param.psk_len; i++)
                    efuse_tool_param.psk[i] = argv[2][i];
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                //snprintf_res(cmd_data,"write efuse rate gain b   = %d\n", value1);
                snprintf_res(cmd_data,"write efuse rate gain n40 = %d\n", value1);
            
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "checkitemfree")) 
    {
        if (argc == 3) {
            
            value1 = simple_strtoul(argv[2], &endp, 0);
            if ((value1 < 0) || (value1 > 14)) {
                snprintf_res(cmd_data,"item range 0 ~ 14\n");
                snprintf_res(cmd_data,"0: macaddress, 1: freq offset, 2: txpower1, 3: rate gain b, 4: ble power, 5: reserve, 6: rate gain g,\
 7: reference power, 8: rate gain su20, 9: rate gain su40, 10: rate gain tb20, 11: rate gain tb40, 12: usb pid vid, 13: rate gain n20, 14: rate gain n40\n");
                return;
            }
            
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_CHECK_ITEM_FREE;
                efuse_tool_param.wait_resp = 1;
                efuse_tool_param.check_item_free = value1;
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"free count = %d\n", rftool_cfg.free_item);
            
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_ble_pow")) 
    {
        if (argc == 2) {
            

                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_BLE_POWER;
                efuse_tool_param.wait_resp = 1;
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"efuse ble gain = %d\n", rftool_cfg.ble_power);

        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_ble_pow")) 
    {
        if (argc == 4) {
            if (strlen(argv[2]) > 32) {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 15)) {
                snprintf_res(cmd_data,"ble power range 1 ~ 15\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 4)==0)
            {
                snprintf_res(cmd_data,"No free space for ble power\n");
                return;
            }
     
                efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_BLE_POWER;
                efuse_tool_param.wait_resp = 0;
                efuse_tool_param.ble_gain = value1;
                efuse_tool_param.psk_len = strlen(argv[2]);
                for (i= 0; i < efuse_tool_param.psk_len; i++)
                    efuse_tool_param.psk[i] = argv[2][i];
                ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
                // show result
                snprintf_res(cmd_data,"write efuse ble power = %d\n", value1);
            
        } else {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    
    else if (!strcmp(argv[1], "r_gpow")) 
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_G_N20;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"efuse rate gain g   = %d\n", rftool_cfg.gain_g);
            //snprintf_res(cmd_data,"efuse rate gain n20 = %d\n", rftool_cfg.gain_n20);

        }
        else
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_gpow")) 
    { 
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"rate gain g range 1 ~ 31\n");
                return;
            }
            
            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 6)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain g\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_G_N20;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.rate_gain_g = value1;
            efuse_tool_param.rate_gain_n20 = 0;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
                
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse rate gain g   = %d\n", value1);
            //snprintf_res(cmd_data,"write efuse rate gain n20 = %d\n", value2);
            
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_n20pow")) 
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_G_N20;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            //snprintf_res(cmd_data,"efuse rate gain g   = %d\n", rftool_cfg.gain_g);
            snprintf_res(cmd_data,"efuse rate gain n20 = %d\n", rftool_cfg.gain_n20);

        }
        else
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_n20pow")) 
    { 
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"rate gain n20 range 1 ~ 31\n");
                return;
            }
            
            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 13)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain n20\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_G_N20;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.rate_gain_g = 0;
            efuse_tool_param.rate_gain_n20 = value1;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
                
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            //snprintf_res(cmd_data,"write efuse rate gain g   = %d\n", value1);
            snprintf_res(cmd_data,"write efuse rate gain n20 = %d\n", value1);
            
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_refpow")) 
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_HETB_CAL_RATE_POWER;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            //snprintf_res(cmd_data,"efuse hetb cal rate   = %d\n", rftool_cfg.hetb_cali_rate);
            snprintf_res(cmd_data,"efuse reference power = %d\n", rftool_cfg.hetb_cali_power);
        }
        else
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_refpow")) 
    { 
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"reference cal power range 1 ~ 31\n");
                return;
            }
            
            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 7)==0)
            {
                snprintf_res(cmd_data,"No free space for reference power\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_HETB_CAL_RATE_POWER;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.hetb_cal_rate= 0;
            efuse_tool_param.hetb_cal_power = value1;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
                
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse  reference power = %d\n", value1);
            
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }

    else if (!strcmp(argv[1], "r_su20pow"))
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_SU20;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);

            // show result
            snprintf_res(cmd_data,"efuse rate gain su20 = %d\n", rftool_cfg.gain_su20);
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_su20pow"))
    {
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"rate gain su20 range 1 ~ 31\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 8)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain su20\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_SU20;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.rate_gain_su20 = value1;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse rate gain su20 = %d\n", value1);
            
        }
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_su40pow"))
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_SU40;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);

            // show result
            snprintf_res(cmd_data,"efuse rate gain su40 = %d\n", rftool_cfg.gain_su40);
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_su40pow"))
    {
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"rate gain su40 range 1 ~ 31\n");
                return;
            }
            
            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 9)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain su40\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_SU40;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.rate_gain_su40 = value1;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse rate gain su40 = %d\n", value1);
            
        }
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }

    else if (!strcmp(argv[1], "r_tb20pow"))
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_TB20;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);

            // show result
            snprintf_res(cmd_data,"efuse rate gain tb20 = %d\n", rftool_cfg.gain_tb20);
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_tb20pow"))
    {
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"rate gain tb20 range 1 ~ 31\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 10)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain tb20\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_TB20;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.rate_gain_tb20 = value1;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse rate gain tb20 = %d\n", value1);
            
        }
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "r_tb40pow"))
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_TB40;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);

            // show result
            snprintf_res(cmd_data,"efuse rate gain tb40 = %d\n", rftool_cfg.gain_tb40);
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "w_tb40pow"))
    {
        if (argc == 4) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 31)) 
            {
                snprintf_res(cmd_data,"rate gain tb40 range 1 ~ 31\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 11)==0)
            {
                snprintf_res(cmd_data,"No free space for rate gain tb40\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_RATE_GAIN_TB40;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.rate_gain_tb40 = value1;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse rate gain tb40 = %d\n", value1);
            
        }
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }


    else if (!strcmp(argv[1], "readusbpidvid")) 
    {
        if (argc == 2) 
        {
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_READ_USB_PID_VID;
            efuse_tool_param.wait_resp = 1;
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"efuse usb pid = 0x%x\n", rftool_cfg.usb_pid);
            snprintf_res(cmd_data,"efuse usb vid = 0x%x\n", rftool_cfg.usb_vid);
        }
        else
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else if (!strcmp(argv[1], "writeusbpidvid")) 
    { 
        if (argc == 5) 
        {
            if (strlen(argv[2]) > 32) 
            {
                snprintf_res(cmd_data,"psk len 1 ~ 32\n");
                return;
            }
            value1 = simple_strtoul(argv[3], &endp, 0);
            if ((value1 < 1) || (value1 > 0xffff)) 
            {
                snprintf_res(cmd_data,"usb pid range 0x0001 ~ 0xffff\n");
                return;
            }
            
            value2 = simple_strtoul(argv[4], &endp, 0);
            if ((value1 < 1) || (value1 > 0xffff)) 
            {
                snprintf_res(cmd_data,"usb vid range 0x0001 ~ 0xffff\n");
                return;
            }

            if(_ssv_check_psk(argv[2] , strlen(argv[2])) !=0)
            {
                snprintf_res(cmd_data,"Incorrect psk\n");
                return;
            }
            if(_ssv_check_item_free(sc, 12)==0)
            {
                snprintf_res(cmd_data,"No free space for usb pid vid\n");
                return;
            }
            
            efuse_tool_param.efuse_cmd = (u32)SSV6XXX_EFUSE_CMD_WRITE_USB_PID_VID;
            efuse_tool_param.wait_resp = 0;
            efuse_tool_param.usb_pid= value1;
            efuse_tool_param.usb_vid = value2;
            efuse_tool_param.psk_len = strlen(argv[2]);
            for (i= 0; i < efuse_tool_param.psk_len; i++)
                efuse_tool_param.psk[i] = argv[2][i];
                
            ssv_eng_send_priv_msg_efuse_cmd(sc, &efuse_tool_param, efuse_tool_param.wait_resp);
                
            // show result
            snprintf_res(cmd_data,"write efuse usb pid = 0x%x\n", value1);
            snprintf_res(cmd_data,"write efuse usb vid = 0x%x\n", value2);
            
        } 
        else 
        {
            snprintf_res(cmd_data,"Incorrect efuse command format\n");
        }
    }
    else 
    {
        snprintf_res(cmd_data,"\n efuse [ readchipid|readmac|writemac|readxtal|writextal|r_2g_gain|w_2g_gain|r_bpow|w_bpow|r_gpow|w_gpow|\
r_n20pow|w_n20pow|r_n40pow|w_n40pow|r_ble_pow|w_ble_pow|r_refpow|w_refpow|r_su20pow|w_su20pow|r_su40pow|w_su40pow|\
r_tb20pow|w_tb20pow|r_tb40pow|w_tb40pow|readusbpidvid|writeusbpidvid |readpsk |writepsk |checkitemfree ]\n");
    }
   
    return;
}

static u32 _hwif_read_reg(struct ssv_hw *sh, u32 addr)
{
    u32 value = 0;

    if (sh->hwif_ops->readreg)
        sh->hwif_ops->readreg(sh->dev, addr, &value);

    return value;
}
static int _parse_efuse_item(struct ssv_hw *sh, struct _6030b1_efuse_st* efuse_data)
{
    //SKU ID
    rftool_cfg.skuid = efuse_data->sku_id;
    //MAC
    if( (efuse_data->mac1_1_2==0) && (efuse_data->mac1_3_6==0) && (efuse_data->mac2_1_4==0) && (efuse_data->mac2_5_6==0) )
    {
        SSV_LOG_DBG("no mac in efuse\n");
    }
    else if( (efuse_data->mac2_1_4==0) && (efuse_data->mac2_5_6==0) )
    {
        rftool_cfg.efuse_mac[0] = (efuse_data->mac1_1_2>> 8)&0xff;
        rftool_cfg.efuse_mac[1] = (efuse_data->mac1_1_2>> 0)&0xff;
        rftool_cfg.efuse_mac[2] = (efuse_data->mac1_3_6>>24)&0xff;
        rftool_cfg.efuse_mac[3] = (efuse_data->mac1_3_6>>16)&0xff;
        rftool_cfg.efuse_mac[4] = (efuse_data->mac1_3_6>> 8)&0xff;
        rftool_cfg.efuse_mac[5] = (efuse_data->mac1_3_6>> 0)&0xff;
    }
    else
    {
        rftool_cfg.efuse_mac[0] = (efuse_data->mac2_1_4>>24)&0xff;
        rftool_cfg.efuse_mac[1] = (efuse_data->mac2_1_4>>16)&0xff;
        rftool_cfg.efuse_mac[2] = (efuse_data->mac2_1_4>> 8)&0xff;
        rftool_cfg.efuse_mac[3] = (efuse_data->mac2_1_4>> 0)&0xff;
        rftool_cfg.efuse_mac[4] = (efuse_data->mac2_5_6>> 8)&0xff;
        rftool_cfg.efuse_mac[5] = (efuse_data->mac2_5_6>> 0)&0xff;
    }

    //XTAL
    if( (efuse_data->xtal_freq1==0) && (efuse_data->xtal_freq2==0) )
    {
        ;//SSV_LOG_DBG("no xtal in efuse\n");
    }
    else if( efuse_data->xtal_freq2==0 )
    {
        rftool_cfg.xtal = efuse_data->xtal_freq1;
    }
    else
    {
        rftool_cfg.xtal = efuse_data->xtal_freq2;
    }
    
    //TXPOWER
    if( (efuse_data->tx_power1_ch_1_7 ==0) && (efuse_data->tx_power1_ch_8_14==0) && (efuse_data->tx_power2_ch_1_7==0) && (efuse_data->tx_power2_ch_8_14==0) )
    {
        ;//SSV_LOG_DBG("no txpower in efuse\n");
    }
    else if( (efuse_data->tx_power2_ch_1_7==0) && (efuse_data->tx_power2_ch_8_14==0) )
    {
        rftool_cfg.power_ch_1_7 = efuse_data->tx_power1_ch_1_7;
        rftool_cfg.power_ch_8_14 = efuse_data->tx_power1_ch_8_14;
    }
    else
    {
        rftool_cfg.power_ch_1_7 = efuse_data->tx_power2_ch_1_7;
        rftool_cfg.power_ch_8_14 = efuse_data->tx_power2_ch_8_14;
    }

    //rate b 
    if( (efuse_data->rate1_b ==0) && (efuse_data->rate2_b==0) )
    {
        ;//SSV_LOG_DBG("no rate b and n40 in efuse\n");
    }
    else if( (efuse_data->rate2_b==0) )
    {
        rftool_cfg.gain_b = efuse_data->rate1_b;
    }
    else
    {
        rftool_cfg.gain_b = efuse_data->rate2_b ;
    }
    //rate n40
    if( (efuse_data->rate1_n40==0) && (efuse_data->rate2_n40==0) )
    {
        ;//SSV_LOG_DBG("no rate b and n40 in efuse\n");
    }
    else if( (efuse_data->rate2_n40==0) )
    {
        rftool_cfg.gain_n40 = efuse_data->rate1_n40;
    }
    else
    {
        rftool_cfg.gain_n40 = efuse_data->rate2_n40;
    }

    //ble power
    if( (efuse_data->ble_rf1==0) && (efuse_data->ble_rf2==0) )
    {
        ;//SSV_LOG_DBG("no ble power in efuse\n");
    }
    else if( efuse_data->ble_rf2==0 )
    {
        rftool_cfg.ble_power = efuse_data->ble_rf1;
    }
    else
    {
        rftool_cfg.ble_power = efuse_data->ble_rf2 ;
    }

    //rate g 
    if( (efuse_data->rate1_g ==0) && (efuse_data->rate2_g==0) )
    {
        ;//SSV_LOG_DBG("no rate g and n20 in efuse\n");
    }
    else if( (efuse_data->rate2_g==0) )
    {
        rftool_cfg.gain_g = efuse_data->rate1_g;
    }
    else
    {
        rftool_cfg.gain_g = efuse_data->rate2_g ;
    }

    //rate n20
    if( (efuse_data->rate1_n20==0) && (efuse_data->rate2_n20==0) )
    {
        ;//SSV_LOG_DBG("no rate g and n20 in efuse\n");
    }
    else if( (efuse_data->rate2_n20==0) )
    {
        rftool_cfg.gain_n20 = efuse_data->rate1_n20;
    }
    else
    {
        rftool_cfg.gain_n20 = efuse_data->rate2_n20;
    }
    
    //hetb cali power
    if( (efuse_data->hetb1_cal_index==0) && (efuse_data->hetb1_cal_power==0) && (efuse_data->hetb2_cal_index==0) && (efuse_data->hetb2_cal_power==0) )
    {
        ;//SSV_LOG_DBG("no hetb cali power in efuse\n");
    }
    else if( (efuse_data->hetb2_cal_index==0) && (efuse_data->hetb2_cal_power==0) )
    {
        rftool_cfg.hetb_cali_rate = efuse_data->hetb1_cal_index ;
        rftool_cfg.hetb_cali_power = efuse_data->hetb1_cal_power;
    }
    else
    {
        rftool_cfg.hetb_cali_rate = efuse_data->hetb2_cal_index ;
        rftool_cfg.hetb_cali_power = efuse_data->hetb2_cal_power;
    }       

    //su20
    if( (efuse_data->rate1_su20==0) && (efuse_data->rate2_su20==0) )
    {
        ;//SSV_LOG_DBG("no rate su20 in efuse\n");
    }
    else if( efuse_data->rate2_su20==0 )
    {
        rftool_cfg.gain_su20 = efuse_data->rate1_su20 ;
    }
    else
    {
        rftool_cfg.gain_su20 = efuse_data->rate2_su20 ;
    }

    //su40
    if( (efuse_data->rate1_su40==0) && (efuse_data->rate2_su40==0) )
    {
        ;//SSV_LOG_DBG("no rate su40 in efuse\n");
    }
    else if( efuse_data->rate2_su40==0 )
    {
        rftool_cfg.gain_su40 = efuse_data->rate1_su40 ;
    }
    else
    {
        rftool_cfg.gain_su40 = efuse_data->rate2_su40 ;
    }

    //tb20
    if( (efuse_data->rate1_tb20==0) && (efuse_data->rate2_tb20==0) )
    {
        ;//SSV_LOG_DBG("no rate tb20 in efuse\n");
    }
    else if( efuse_data->rate2_tb20==0 )
    {
        rftool_cfg.gain_tb20 = efuse_data->rate1_tb20 ;
    }
    else
    {
        rftool_cfg.gain_tb20 = efuse_data->rate2_tb20 ;
    }

    //tb40
    if( (efuse_data->rate1_tb40==0) && (efuse_data->rate2_tb40==0) )
    {
        ;//SSV_LOG_DBG("no rate tb40 in efuse\n");
    }
    else if( efuse_data->rate2_tb40==0 )
    {
        rftool_cfg.gain_tb40 = efuse_data->rate1_tb40 ;
    }
    else
    {
        rftool_cfg.gain_tb40 = efuse_data->rate2_tb40 ;
    }

    //usb pid vid
    if( (efuse_data->usb_pid==0) && (efuse_data->usb_vid==0) )
    {
        ;//SSV_LOG_DBG("no usb pid vid in efuse\n");
    }
    else
    {
        rftool_cfg.usb_pid = efuse_data->usb_pid ;
        rftool_cfg.usb_vid = efuse_data->usb_vid;
    }
        
    return 0;
}

const char *efuse_get_chip_id(void)
{
    return (const char *)&efuse_info.chip_id[0];
}

const char *_efuse_trans_chip_id(u32 sku_id, u32 sku_id1)
{
    switch(sku_id)
    {
        case 0x5cf30000: //SV6318
        {
            return "SV6318";
        }
        case 0x54f7ffff: //SV6355
        {
            return "SV6355";
        }
        case 0x54f30000: //SV6358
        {
            return "SV6358";
        }
        case 0x5ff7ffff: //SV6115
        {
            if(0x01 == sku_id1)
            {
                return "SV6115C";
            }
            return "SV6115";
        }
        default:
        {
            return "UNDEFINED";
        }
    }
}

void efuse_read_all_map(struct ssv_hw *sh)
{
    int i;
    u32 *pbuf;
    struct _6030b1_efuse_st efuse_layout = {0};

    pbuf = (u32*)&efuse_layout;
    for(i=0; i<=12; i++)
    {
        *(pbuf+i) = _hwif_read_reg(sh, 0x8500000+i*4);
    }
    *(pbuf+27) = _hwif_read_reg(sh, 0x8500000+27*4); //PID VID
    *(pbuf+31) = _hwif_read_reg(sh, 0x8500000+(31*4)); //SKU-ID1

    //apply efuse data to ssv_cfg
    _parse_efuse_item(sh, &efuse_layout);

    memcpy(efuse_info.efuse_mac,rftool_cfg.efuse_mac,6);
    efuse_info.xtal=rftool_cfg.xtal;
    efuse_info.power_ch_1_7=rftool_cfg.power_ch_1_7;
    efuse_info.power_ch_8_14=rftool_cfg.power_ch_8_14;
    efuse_info.gain_b=rftool_cfg.gain_b;
    efuse_info.gain_n40=rftool_cfg.gain_n40;
    efuse_info.gain_g=rftool_cfg.gain_g;
    efuse_info.gain_n20=rftool_cfg.gain_n20;
    efuse_info.gain_su20=rftool_cfg.gain_su20;
    efuse_info.gain_su40=rftool_cfg.gain_su40;
    efuse_info.gain_tb20=rftool_cfg.gain_tb20;
    efuse_info.gain_tb40=rftool_cfg.gain_tb40;
    efuse_info.ble_power=rftool_cfg.ble_power;

    //fill the chip id by the transformed name string
    strncpy((char *)&efuse_info.chip_id[0]
        , (const char *)_efuse_trans_chip_id(efuse_layout.sku_id, efuse_layout.sku_id1)
        , SSV_CHIP_ID_LENGTH);

    SSV_LOG_DBG("EFUSE configuration\n");
    SSV_LOG_DBG("Read efuse skuid       [%08X]\n", rftool_cfg.skuid);
    SSV_LOG_DBG("Read efuse xtal        [%02X]\n", efuse_info.xtal);
    SSV_LOG_DBG("Read efuse MAC address [%pM]\n", efuse_info.efuse_mac);
    SSV_LOG_DBG("Read efuse power_ch_1_7 [%02X]\n", efuse_info.power_ch_1_7);
    SSV_LOG_DBG("Read efuse power_ch_8_14[%02X]\n", efuse_info.power_ch_8_14);
    SSV_LOG_DBG("Read efuse gain_b      [%02X]\n", efuse_info.gain_b    );
    SSV_LOG_DBG("Read efuse gain_n40    [%02X]\n", efuse_info.gain_n40  );
    SSV_LOG_DBG("Read efuse gain_g      [%02X]\n", efuse_info.gain_g    );
    SSV_LOG_DBG("Read efuse gain_n20    [%02X]\n", efuse_info.gain_n20  );
    SSV_LOG_DBG("Read efuse gain_su20   [%02X]\n", efuse_info.gain_su20 );
    SSV_LOG_DBG("Read efuse gain_su40   [%02X]\n", efuse_info.gain_su40 );
    SSV_LOG_DBG("Read efuse gain_tb20   [%02X]\n", efuse_info.gain_tb20 );
    SSV_LOG_DBG("Read efuse gain_tb40   [%02X]\n", efuse_info.gain_tb40 );
    SSV_LOG_DBG("Read efuse ble_power   [%02X]\n", efuse_info.ble_power );


    return;
}
int ssv_res_update_to_cfg(struct ssv_efuse_tool_param *param)
{
    switch(param->efuse_cmd)
    {
            case SSV6XXX_EFUSE_CMD_READ_CHIPID:
                rftool_cfg.skuid = param->chip_id;
                break;
            case SSV6XXX_EFUSE_CMD_READ_MAC:
                memcpy(rftool_cfg.efuse_mac, param->mac, ETH_ALEN);
                break;
            case SSV6XXX_EFUSE_CMD_READ_TX_POWER1:
                rftool_cfg.power_ch_1_7 = param->txpower1_gain2000;
                rftool_cfg.power_ch_8_14 = param->txpower1_gain5200;
                break;
            case SSV6XXX_EFUSE_CMD_READ_XTAL:
                rftool_cfg.xtal  = param->xtal;
                break;
            case SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_B_N40:
                rftool_cfg.gain_b  = param->rate_gain_b;
                rftool_cfg.gain_n40  = param->rate_gain_n40;
                break;
            case SSV6XXX_EFUSE_CMD_READ_PSK:
                {
                    int i=0;
                    rftool_cfg.psk_len = param->psk_len;
                    for (i = 0; i < rftool_cfg.psk_len; i++)
                    {
                        rftool_cfg.psk[i] = param->psk[i];
                    }
                }
                break;
            case SSV6XXX_EFUSE_CMD_CHECK_ITEM_FREE:
                rftool_cfg.free_item   = param->check_item_free;
                break;
            case SSV6XXX_EFUSE_CMD_READ_BLE_POWER:
                rftool_cfg.ble_power   = param->ble_gain;
                break;
            case SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_G_N20:
                rftool_cfg.gain_g  = param->rate_gain_g;
                rftool_cfg.gain_n20 = param->rate_gain_n20;
                break;
            case SSV6XXX_EFUSE_CMD_READ_HETB_CAL_RATE_POWER:
                rftool_cfg.hetb_cali_rate = param->hetb_cal_rate;
                rftool_cfg.hetb_cali_power = param->hetb_cal_power;
                break;
            case SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_SU20:
                rftool_cfg.gain_su20 = param->rate_gain_su20;
                break;
            case SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_SU40:
                rftool_cfg.gain_su40 = param->rate_gain_su40;
                break;
            case SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_TB20:
                rftool_cfg.gain_tb20 = param->rate_gain_tb20;
                break;
            case SSV6XXX_EFUSE_CMD_READ_RATE_GAIN_TB40:
                rftool_cfg.gain_tb40 = param->rate_gain_tb40;
                break;
            case SSV6XXX_EFUSE_CMD_READ_USB_PID_VID:
                rftool_cfg.usb_pid = param->usb_pid;
                rftool_cfg.usb_vid = param->usb_vid;
                break;
            default:
                break;

    }
    return 0;
}
