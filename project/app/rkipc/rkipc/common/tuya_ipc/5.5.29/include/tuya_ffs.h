
#ifndef __TUYA_FFS_H__
#define __TUYA_FFS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "wifi_netcfg_frame_sniffer.h"

#include "http_manager.h"
#include "tuya_ffs_inc.h"
#include "tuya_ffs_stream.h"
#include "uni_time_queue.h"
#include "wifi_netcfg_frame_transporter.h"

//#include "tuya_ffs_dss.h"

typedef enum {
	TUYA_FFS_STATUS_IDLE,
	TUYA_FFS_STATUS_COMPUTE_SSID_PWD,
	TUYA_FFS_STATUS_SCAN_AP,
	TUYA_FFS_STATUS_CONNECT_NETWORK,
	TUYA_FFS_STATUS_START_DSS,
	TUYA_FFS_STATUS_PRE_ACTIVE,
	TUYA_FFS_STATUS_CONNECT_TUYA_MQTT,
	TUYA_FFS_STATUS_FINISHED,
	TUYA_FFS_STATUS_STOPED,
} tuya_ffs_status_e;

typedef struct probeRequestTransportParam_s {
	char ffsSsid[FFS_WIFI_SSID_LEN + 1];
} ProbeRequestTransportParam_t, *ptrProbeRequestTransportParam_t;

typedef struct {
	uint8_t status; // ref tuya_ffs_status_e.
	char ssid[FFS_WIFI_SSID_LEN + 1];
	char passwd[FFS_WIFI_PASSWD_LEN + 1];
	char nonce[FFS_CLIENT_NONCE_SIZE];
	char *session_token;
	char *gw_endpoint;
	FN_NETCFG_CB cb;
	TIMER_ID netcfg_timeout_timer; /*ffs配网超时*/

	mbedtls_pk_context cloudPublicKey;   //!< Cloud public key.
	mbedtls_pk_context devicePrivateKey; //!< Device private key.
	BOOL_T found_ffs_ssid;
	uint32_t dss_port; /*echo:8888; simple_setup:443*/

	tuya_ffs_config_t config;

	ptrWifiNetcfgFrameTransporter_t ptrWifiTransporter;
	ProbeRequestTransportParam_t ProbeRequestTransportParameter;
	TransporteeParameters_t TransporteeParameter;

	ptrWifiNetcfgFrameSniffer_t ptrWifiSniffer;
	SnifferUserParameters_t SnifferUserParammeter;
} tuya_ffs_info_t;

OPERATE_RET tuya_ffs_init(tuya_ffs_config_t *config);

OPERATE_RET tuya_ffs_start(FN_NETCFG_CB cb);

OPERATE_RET tuya_ffs_stop();

OPERATE_RET tuya_ffs_destory();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __TUYA_FFS_H__
