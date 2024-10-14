#include <linux/module.h>
#include <linux/version.h>
#include <linux/inetdevice.h>
#include <linux/of.h>
#include <linux/etherdevice.h>

#include "rftool/ssv_efuse.h"
#include <ssv_cfg.h>
#include "dev.h"
#include "rf_table.h"
#include "rftool/ssv_rftool.h"
#include "rftool/ssv_phy_rf.h"
#include "ssv_debug.h"

extern struct ssv6xxx_cfg ssv_cfg;
extern struct ssv_efuse_info efuse_info;

struct st_rf_table cus_rf_conf_table = {
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
int ssv_custom_modify_rf_conf_table(struct ssv_hw *sh)
{
    SSV_LOG_DBG("ssv_custom_modify_rf_conf_table");
#if 1
    //================================================================//
    //The efuse setting is on efuse_info, SSV apply the efuse setting to the rf configuration table by these APIs.
    //Finally, driver pass the sh->rf_conf_table to the WIFI Chip.
    //PS: Please refer the docuemnt about default procedure and configuration parameter
    //================================================================//
    
    ssv_chg_xtal_freq_offset(sh, efuse_info.xtal);                                           //xtal
    ssv_chg_band_gain(sh, efuse_info.power_ch_1_7, efuse_info.power_ch_8_14);  //band gain
    ssv_chg_gain_b_mode(sh, efuse_info.gain_b);
    ssv_chg_gain_g_mode(sh, efuse_info.gain_g);
    ssv_chg_gain_n_mode(sh, efuse_info.gain_n20, efuse_info.gain_n40);
    ssv_chg_gain_su_mode(sh, efuse_info.gain_su20, efuse_info.gain_su40);
    ssv_chg_gain_tb_mode(sh, efuse_info.gain_tb20, efuse_info.gain_tb40);
    
    ssv_chg_ble_power(sh, efuse_info.ble_power);                                             //ble gain
    ssv_chg_thermal_boundary(sh, ssv_cfg.thermal_boundary);
#else
    //================================================================//
    //If SSV's default procedure and configuration parameter can't satisfy the customer's requirement, 
    //We can modify the cus_rf_conf_table directly, and then copy to sh->rf_conf_table。
    //================================================================//

    // To Do: modify cus_rf_conf_table 
    
    //Copy the final setting to sh->rf_conf_table
    memcpy((void *)(&sh->rf_conf_table),(const void *)(&cus_rf_conf_table),sizeof(cus_rf_conf_table));

#endif
    return 0;
}

int ssv_custom_modify_macaddr(struct ssv_hw *sh)
{
    char mac_addr1[6] = {0xaa, 0x0, 0xcc, 0xdd, 0xee, 0xfe}; //Default mac address.

    if(is_valid_ether_addr(&ssv_cfg.maddr[0][0])) {
             // use ssv6x5x-wifi.cfg mac address
            SSV_LOG_DBG("Apply mac address from ssv6x5x-wifi.cfg\r\n");
            memcpy((void *)&sh->maddr[0][0], (const void *)ssv_cfg.maddr, ETH_ALEN);
    }else{
        if (!is_valid_ether_addr(efuse_info.efuse_mac)) {                    
            // use hardcode mac address
            SSV_LOG_DBG("WARN!WARN!WARN! Appl hard code mac address\r\n");
            memcpy((void *)&sh->maddr[0][0], (const void *)mac_addr1, ETH_ALEN);            
        } else{
            // use efuse mac address
            SSV_LOG_DBG("Apply mac address from efuse\r\n");
            memcpy((void *)&sh->maddr[0][0], (const void *)efuse_info.efuse_mac, ETH_ALEN);
        }
    }

    memcpy((void *)&sh->maddr[1][0], (void *)&sh->maddr[0][0], ETH_ALEN);
    /* notice: the second mac address rule must be following the fw*/
    sh->maddr[1][5] ^= 0x1;
    
    SSV_LOG_DBG("mac address %pM/%pM\n",&sh->maddr[0][0], &sh->maddr[1][0]);    
    
    return 0;
}
