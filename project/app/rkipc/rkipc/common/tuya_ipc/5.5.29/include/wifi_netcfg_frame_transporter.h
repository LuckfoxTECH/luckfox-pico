#ifndef __WIFI_NETCFG_FRAME_TRANSPORTER_H__
#define __WIFI_NETCFG_FRAME_TRANSPORTER_H__
#include "tuya_hal_network.h"
#include "tuya_hal_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    wifi_netcfg_frame_transporter组件是wifi相关配网组件的公共服务模块，原来是ez配网中的
    cc_t(chanChannel_timer)的工作，由于ez+ap共存配网，ffs配网，无感配网等配网的引入，因此
    把wifi 802.11 frame发送部分的功能独立出来，取名wifi_netcfg_frame_transporter，希望能
    更换的为ez+ap共存配网，ffs配网，无感配网，以及后续可能加入的其他wifi配网方式服务；


    角色逻辑：
    1个transporter	<-----> n个transportee
    用户获取:唯一的transporter
    用户通过transporter的接口register,unregister,注册/销毁对应于netcfg_type的Transportee，
    通过start,stop,isRun来开始，停止和判断是否允许该Transportee；
*/

typedef int (*fnProbeRequestTransportCallBack_t)(void *ptrArgs, BYTE_T channel);
typedef int (*fnAuthTransportCallBack_t)(void *ptrArgs, BYTE_T channel);
typedef int (*fnBeaconTransportCallBack_t)(void *ptrArgs, BYTE_T channel);

typedef struct TransporteeParameters_s {
	fnProbeRequestTransportCallBack_t fnProbeRequestTransport;
	fnAuthTransportCallBack_t fnAuthTransport;
	fnBeaconTransportCallBack_t fnBeaconTransport;
	void *ptrArgs;
} TransporteeParameters_t, *ptrTransporteeParameters_t;

/*
    transportee:
        register->start->ungister
        isRun
    transporter:
        transporter is inited and started in WifiNetcfgFrameTransporterSessionInit
        and will be:
        lock->stop
        getApInfo
        in transportee.
*/
typedef int (*fnRegister)(int netcfg_type, ptrTransporteeParameters_t pParam);
typedef int (*fnStart)(int netcfg_type);
typedef bool (*fnIsRun)(int netcfg_type);
typedef int (*fnUnregister)(int netcfg_type);

typedef int (*fnLock)(bool lockState);
typedef int (*fnStartTransporter)();
typedef int (*fnStopTransporter)();
typedef int (*fnGetApInfo)(AP_IF_S **ppApArray, uint32_t *pApNum);

typedef struct WifiNetcfgFrameTransporter_s {
	fnRegister registerTransporteeFn;
	fnStart startTransporteeFn;
	fnIsRun isRunTransporteeFn;
	fnUnregister unregisterTransporteeFn;

	fnLock lockTransporterChannelFn;
	fnStartTransporter startTransporterFn;
	fnStopTransporter stopTransporterFn;
	fnGetApInfo getApInfoFn;
} WifiNetcfgFrameTransporter_t, *ptrWifiNetcfgFrameTransporter_t;

/*配网开始时，执行信道初始化*/
int wifiNetcfgFrameTransporterChannelInitAndStart();

/*
    获取或创建wifi_netcfg_frame_transporter的实例，线程安全
*/
ptrWifiNetcfgFrameTransporter_t getWifiNetcfgFrameTransporter();
/*
    销毁wifi_netcfg_frame_transporter实例，释放内存
*/
void destroyWifiNetcfgFrameTransporter();

/*配网模块全局初始化，创建并初始化TransporterSession，必须在getTransporter之前调用*/
int WifiNetcfgFrameTransporterSessionInit();
/*配网模块全局销毁，在配网活动结束之后调用*/
int WifiNetcfgFrameTransporterSessionUninit();

#ifdef __cplusplus
}
#endif
#endif
