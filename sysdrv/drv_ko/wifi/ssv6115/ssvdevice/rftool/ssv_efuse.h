#ifndef __SSV_EFUSE_H__
#define __SSV_EFUSE_H__

#include "ssvdevice/dev.h"
#include "ipc_msg.h"


/* Maximum chip ID length */
#define SSV_CHIP_ID_LENGTH                      (24)

struct ssv_efuse_info{
    u8 efuse_mac[6];
    u8 xtal;
    u8 power_ch_1_7;
    u8 power_ch_8_14;
    u8 gain_b;
    u8 gain_g;
    u8 gain_n20;
    u8 gain_n40;
    u8 gain_su20;
    u8 gain_su40;
    u8 gain_tb20;
    u8 gain_tb40;
    u32 ble_power;
    //hw chip id
    u8 chip_id[SSV_CHIP_ID_LENGTH];
};


const char *efuse_get_chip_id(void);
void ssv_6030_efuse(struct ssv_hw *sh, int argc, char *argv[]);
void efuse_read_all_map(struct ssv_hw *sh);
int ssv_res_update_to_cfg(struct ssv_efuse_tool_param *param);

#endif /* __SSV_EFUSE_H__ */
