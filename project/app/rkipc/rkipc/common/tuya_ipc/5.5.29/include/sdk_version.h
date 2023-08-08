/***********************************************************
 *  File: sdk_version.h
 *  Author:
 *  Date:
 ***********************************************************/
#ifndef _SDK_VERSION_H
#define _SDK_VERSION_H

#include "tuya_iot_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
IOT_BV(BS_VER) 30.00 < - > IOT_SDK 4.1.0
IOT_BV(BS_VER) 30.01 < - > IOT_SDK 4.1.1 < - > RTL8170 2.0.0
IOT_BV(BS_VER) 30.02 < - > IOT_SDK 4.1.2
IOT_BV(BS_VER) 30.03 < - > IOT_SDK 4.1.3
IOT_BV(BS_VER) 30.04 < - > IOT_SDK 4.1.4 < - > RTL8170 2.0.1
IOT_BV(BS_VER) 30.05 < - > IOT_SDK 4.1.5


IOT_BV(BS_VER) 40.00 < - > IOT_SDK 4.2.0

*/

#define BS_VER "40.00"
#define PT_VER "2.2"
#if defined(ENABLE_LAN_ENCRYPTION) && (ENABLE_LAN_ENCRYPTION == 1)
#define LAN_PRO_VER "3.4" //必须3.3版本及以上才支持局域网加密功能
#else
#define LAN_PRO_VER "3.2" // 3.3以下不支持局域网加密
#endif
// 1.0.0:启用CID机制,固件升级启用4.1
// 1.0.1:APP优化流程
// 1.0.2:增加mbid，用于群组控制
// 1.0.3:OTA channel升级（该升级处理不可跳过）
// 1.0.4:支持动态增删OTA channel（1.0.3的处理可以移除）
#define CAD_VER "1.0.4"
#define CD_VER "1.0.0" //固件支持mqtt消息确认

#if defined(WIFI_GW) && (WIFI_GW == 1)
#define DEV_ATTR_THINGCONFIG (1 << 0)
#else
#define DEV_ATTR_THINGCONFIG (0)
#endif

#define DEV_ATTR_LOCAL_DEV_DP_QUERY (1 << 1)

#if defined(ENABLE_LOCAL_LINKAGE) && (ENABLE_LOCAL_LINKAGE == 1)
#define DEV_ATTR_LOCAL_SCENE_LINKAGE (1 << 2)
#else
#define DEV_ATTR_LOCAL_SCENE_LINKAGE (0)
#endif

#if defined(WIFI_CONTROL_SUPPORT) && (WIFI_CONTROL_SUPPORT == 1)
#define DEV_WIFI_SCAN_SUPPORT (1 << 3)
#else
#define DEV_WIFI_SCAN_SUPPORT (0)
#endif

#if defined(ENABLE_GOOGLE_LOCAL_HOME) && (ENABLE_GOOGLE_LOCAL_HOME == 1)
#define DEV_ATTR_GOOGLE_LOCAL_HOME (1 << 4)
#else
#define DEV_ATTR_GOOGLE_LOCAL_HOME (0 << 4)
#endif

#if defined(TY_PEGASUS_NETCFG) && (TY_PEGASUS_NETCFG == 1)
#define DEV_ATTR_PEGASUS_CONFIG (1 << 5)
#else
#define DEV_ATTR_PEGASUS_CONFIG (0)
#endif

#if defined(TY_BT_MOD) && (TY_BT_MOD == 1)
#define DEV_ATTR_BLT_CONTROL (1 << 6)
#else
#define DEV_ATTR_BLT_CONTROL (0)
#endif

#if defined(ENABLE_ALARM) && (ENABLE_ALARM == 1)
#define DEV_ATTR_HOME_SECURITY (1 << 7)
#else
#define DEV_ATTR_HOME_SECURITY (0 << 7)
#endif

#if defined(ENABLE_ASTRO_TIMER) && (ENABLE_ASTRO_TIMER == 1)
#define DEV_ATTR_ASTRO_TIMER (1 << 9)
#else
#define DEV_ATTR_ASTRO_TIMER (0)
#endif

#if defined(ENABLE_WIFI_PROTECT) && (ENABLE_WIFI_PROTECT == 1)
#define DEV_ATTR_WIFI_PROTECT (1 << 12)
#else
#define DEV_ATTR_WIFI_PROTECT (0)
#endif

#define DEV_ATTR_OTA (1 << 11)

//设备属性,采用按位与方式
#define DEV_ATTRIBUTE                                                                              \
	(DEV_ATTR_THINGCONFIG | DEV_ATTR_LOCAL_DEV_DP_QUERY | DEV_ATTR_LOCAL_SCENE_LINKAGE |           \
	 DEV_WIFI_SCAN_SUPPORT | DEV_ATTR_BLT_CONTROL | DEV_ATTR_GOOGLE_LOCAL_HOME |                   \
	 DEV_ATTR_ASTRO_TIMER | DEV_ATTR_PEGASUS_CONFIG | DEV_ATTR_WIFI_PROTECT)

#define INT2STR(NUM) #NUM
#define I2S(R) INT2STR(R)

#define SDK_INFO_1                                                                                 \
	"< TUYA IOT SDK V:" IOT_SDK_VER " BS:" BS_VER "_PT:" PT_VER "_LAN:" LAN_PRO_VER                \
	"_CAD:" CAD_VER "_CD:" CD_VER " >"
#define SDK_INFO_2                                                                                 \
	"< BUILD AT:" BUILD_DATE "_" BUILD_TIME " BY " GIT_USER " FOR " PROJECT_NAME                   \
	" AT " TARGET_PLATFORM " >"
#define SDK_IOT_ATTR                                                                                                                                                                   \
	"IOT DEFS <"                                                                                                                                                                       \
	" WIFI_GW:" I2S(WIFI_GW) " DEBUG:" I2S(TUYA_IOT_DEBUG) " KV_FILE:" I2S(                                                                                                            \
	    KV_FILE) " SHUTDOWN_"                                                                                                                                                          \
	             "MODE:" I2S(SHUTDOWN_MODE) " LITTLE_END:" I2S(LITTLE_END) " TLS_MODE:" I2S(TLS_MODE) " ENABLE_CLOUD_OPERATION:" I2S(ENABLE_CLOUD_OPERATION) " OPERATING_SYSTEM:" I2S( \
	                 OPERATING_SYSTEM) " ENABLE_SYS_RPC:" I2S(ENABLE_SYS_RPC) " RELIABLE_"                                                                                             \
	                                                                          "TRANSFER:" I2S(                                                                                         \
	                                                                              RELIABLE_TRANSFER) " ENABLE_LAN_"                                                                    \
	                                                                                                 "ENCRYPTION"                                                                      \
	                                                                                                 ":" I2S(ENABLE_LAN_ENCRYPTION) " ENABLE_LAN_LINKAGE:" I2S(                        \
	                                                                                                     ENABLE_LAN_LINKAGE) " ENABLE_LAN_DEV:" I2S(ENABLE_LAN_DEV) " >"

#define SDK_INFO SDK_INFO_1 "\r\n" SDK_INFO_2 "\r\n" SDK_IOT_ATTR "\r\n"

#ifdef __cplusplus
}
#endif
#endif /*_SDK_VERSION_H*/
