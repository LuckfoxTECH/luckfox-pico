#ifndef _AP_NETCFG_H_
#define _AP_NETCFG_H_

#include "lan_protocol.h"
#include "netcfg_module.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*启动配网时，切换到ap模式，并开启热点*/
int wifi_netcfg_start_ap_mode();

/*
    配网完成之后的回调，在gw_intf.c中实现，用于保存配网信息到gw,同时触发绑定流程
*/
typedef OPERATE_RET (*AP_CFG_FINISH_CB)(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                                        IN CONST CHAR_T *token);

/*
    func desc:开始ap配网
    parameter:
        input:
        out:
    return:
*/
int wifi_ap_netcfg_start(FN_NETCFG_CB cb, void *args, int type);

/*
    func desc:停止配网，释放资源
    parameters:
        input:
    return:

*/
int wifi_ap_netcfg_stop(int type);

/*
    func desc: 上报配网错误日志到app
*/
void lan_ap_nw_cfg_error_report(IN CONST char *p_desc, IN CONST AP_CFG_ERR_CODE error_code);

/*
    ap netcfg init :注册netcfg 到 netcfg module
*/
int ap_netcfg_init(int netcfg_policy, ApSsidPasswd_t ApInfo);

#ifdef __cplusplus
}
#endif
#endif
