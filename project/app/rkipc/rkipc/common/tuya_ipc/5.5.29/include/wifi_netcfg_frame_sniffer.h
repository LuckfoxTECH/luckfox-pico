#ifndef __WIFI_NETCFG_FRAME_SNIFFER_H__
#define __WIFI_NETCFG_FRAME_SNIFFER_H__

#include "tuya_cloud_types.h"
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
    wifi netcfg frame sniffer用于wifi sniffer共享管理，
    需要通过wifi sniffer获取802.11数据包的组件都可以注册
    回调接口到该组件，获取wifi空中数据包；
    是原有ez配网中的wf_nw_sniffer.c中的功能
*/

/*
    角色定义：
    Sniffer:是提供嗅探服务的组件
    SnifferUser:使用Sniffer组件的嗅探服务的用户组件

*/
typedef int (*fnSnifferUserCallback_t)(void *ptrArgs, uint8_t *buf, uint16_t len,
                                       const SCHAR_T rssi);
typedef struct SnifferUserParameters_s {
	fnSnifferUserCallback_t cb;
	void *ptrArgs;
} SnifferUserParameters_t, *ptrSnifferUserParameters_t;

/*Sniffer组件内部的生命周期：
    全局：				Init->						 stop -> uninit
    SnifferUser：			register ->	start ->unregister
*/
typedef int (*fnSnifferUserRegister)(int netcfg_type, ptrSnifferUserParameters_t pParam);
typedef int (*fnSnifferUserUnregister)(int netcfg_type);

typedef int (*fnSnifferUserStart)(int netcfg_type);

typedef int (*fnSnifferUserStop)(bool isInternalCall);
/*
    关闭除netcfg_type之外，其他配网的sniffer收包
    在共存配网时，当探测到netcfg_type的配网意图时，关闭其他配网的收包
*/
typedef int (*fnSnifferUserStopAllOtherNetcfg)(int netcfg_type, bool isInternalCall);
/*
    打开除netcfg_type之外，其他配网的sniffer收包
    在共存配网时，当netcfg_type超时退出时，重新打开其他配网的收包，继续执行共存配网
*/
typedef int (*fnSnifferUserStartAllOtherNetcfg)(int netcfg_type, bool isInternalCall);

typedef struct WifiNetcfgFrameSniffer_s {
	bool isSnifferStart;
	fnSnifferUserRegister registerSnifferUserFn;
	fnSnifferUserStart startSnifferUserFn;
	fnSnifferUserStop stopSnifferUserFn;
	fnSnifferUserUnregister unregisterSnifferUserFn;
	fnSnifferUserStartAllOtherNetcfg startAllOtherUserFn;
	fnSnifferUserStopAllOtherNetcfg stopAllOtherUserFn;
} WifiNetcfgFrameSniffer_t, *ptrWifiNetcfgFrameSniffer_t;

/*共存配网启动时，执行sniffer callback设置*/
int WifiNetcfgFrameSnifferCallbackSet();
/*ez only配网时，使能sniffer*/
int WifiNetcfgFrameSnifferEnable();

/*获取wifi_netcfg_frame_sniffer的实例*/
ptrWifiNetcfgFrameSniffer_t getWifiNetcfgFrameSniffer();

/*销毁wifi_netcfg_frame_sniffer实例*/
void destroyWifiNetcfgFrameSniffer();

int WifiNetcfgFrameSnifferSessionInit();
int WifiNetcfgFrameSnifferSessionUnInit();

#ifdef __cplusplus
}
#endif
#endif
