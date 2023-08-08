/**
 * @file base_event_info.h
 * @brief tuya simple event module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef __BASE_EVENT_INFO_H__
#define __BASE_EVENT_INFO_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief the event define
 *
 */
#define EVENT_RESET "dev.reset" // device reset
#define EVENT_INIT "dev.init"   // sdk init finished, device init start
#define EVENT_RUN "dev.run"     // sdk active finished, device run start
#define EVENT_REBOOT_REQ                                                                           \
	"dev.reboot.req" // device health check reboot request, application should subscribe it if
	                 // needed
#define EVENT_REBOOT_ACK                                                                           \
	"dev.reboot.ack" // device health check reboot ack, application should publish when it ready
#define EVENT_CAD_VER_UPDATE "cad.update"         // cad protocol update
#define EVENT_FW_VER_UPDATE "fw.update"           // firmware version update
#define EVENT_POST_ACTIVATE "activate.post"       // active
#define EVENT_PROD_INFO_CHANGED "prod.chg"        //
#define EVENT_HTTP_FAILED "http.failed"           // http request failed
#define EVENT_SDK_EARLY_INIT_OK "early.init"      // before kv flash init
#define EVENT_SDK_DB_INIT_OK "db.init"            // before db init
#define EVENT_SCHEMA_UPDATE "schema.update"       // schema update
#define EVENT_OTA_START_NOTIFY "ota.start"        // ota start
#define EVENT_NETCFG_ERROR "netcfg.error"         // error happened when netcfg
#define EVENT_BLE_ACTIVE_START "ble.active.start" // ble active start
#define EVENT_MQTT_CONNECTED "mqtt.con"           // mqtt connect
#define EVENT_MQTT_DISCONNECTED "mqtt.disc"       // mqtt disconnect
#define EVENT_LAN_DISABLE "lan.disable"           // lan disable
#define EVENT_LAN_ENABLE "lan.enable"             // lan enable
#define EVENT_RECOVER_DB "dev.recover.db"         // db is lost, device need recover from cloud
#define EVENT_SUMMER_TZ "summer.tz"               // summer time zone update
#define EVENT_LAN_CLIENT_CLOSE "lan.cli.close"    // lan client close

// ota
#define EVENT_OTA_START_NOTIFY "ota.start"
#define EVENT_OTA_PROCESS_NOTIFY "ota.process"
#define EVENT_OTA_FAILED_NOTIFY "ota.failed"
#define EVENT_OTA_FINISHED_NOTIFY "ota.finished"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__BASE_EVENT_H__ */
