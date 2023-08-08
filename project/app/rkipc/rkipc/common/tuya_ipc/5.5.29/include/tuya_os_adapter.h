/**
 * @file tuya_os_adapter.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _TUYA_OS_ADAPTER_H
#define _TUYA_OS_ADAPTER_H

#include "tuya_cloud_types.h"
#include "tuya_os_adapter_error_code.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef enum {
	INTF_MEMORY,
	INTF_MUTEX,
	INTF_SEMAPHORE,
	INTF_THREAD,
	INTF_FILE_SYSTEM,
	INTF_NETWORK,
	INTF_OUTPUT,
	INTF_SYSTEM,
	INTF_INTERRUPT,
	INTF_QUEUE,
	INTF_WIFI,
	INTF_BT,
	INTF_BLE_MESH_PROV,
	INTF_STORAGE,
	INTF_OTA,
	INTF_WIRED,
	INTF_CELLULAR,
	ALL_INTF_NUMS
} INTF_TYPE_ENUM;

/********************************************************************************
 *********************************tuya_os_fs_intf********************************
 ********************************************************************************/
// 文件访问权限
#define TUYA_IRUSR 0400 /* Read by owner.  */
#define TUYA_IWUSR 0200 /* Write by owner.  */
#define TUYA_IXUSR 0100 /* Execute by owner.  */

// 缓冲区搜索起始位置类型
#define TUYA_SEEK_SET 0 /* Seek from beginning of file.  */
#define TUYA_SEEK_CUR 1 /* Seek from current position.  */
#define TUYA_SEEK_END 2 /* Seek from end of file.  */

typedef void *TUYA_DIR;
typedef void *TUYA_FILEINFO;
typedef void *TUYA_FILE;

typedef struct {
	OPERATE_RET (*fs_mkdir)(CONST CHAR_T *path);
	OPERATE_RET (*fs_remove)(CONST CHAR_T *path);
	OPERATE_RET (*fs_mode)(CONST CHAR_T *path, UINT_T *mode);
	OPERATE_RET (*fs_is_exist)(CONST CHAR_T *path, BOOL_T *is_exist);
	OPERATE_RET (*fs_rename)(CONST CHAR_T *path_old, CONST CHAR_T *path_new);
	OPERATE_RET (*dir_open)(CONST CHAR_T *path, TUYA_DIR *dir);
	OPERATE_RET (*dir_close)(TUYA_DIR dir);
	OPERATE_RET (*dir_read)(TUYA_DIR dir, TUYA_FILEINFO *info);
	OPERATE_RET (*dir_name)(TUYA_FILEINFO info, CONST CHAR_T **name);
	OPERATE_RET (*dir_is_dir)(TUYA_FILEINFO info, BOOL_T *is_dir);
	OPERATE_RET (*dir_is_regular)(TUYA_FILEINFO info, BOOL_T *is_regular);
	TUYA_FILE (*fopen)(CONST CHAR_T *path, CONST CHAR_T *mode);
	OPERATE_RET (*fclose)(TUYA_FILE file);
	OPERATE_RET (*fread)(VOID_T *buf, INT_T bytes, TUYA_FILE file);
	OPERATE_RET (*fwrite)(VOID_T *buf, INT_T bytes, TUYA_FILE file);
	OPERATE_RET (*fsync)(TUYA_FILE file);
	CHAR_T *(*fgets)(CHAR_T *buf, INT_T len, TUYA_FILE file);
	OPERATE_RET (*feof)(TUYA_FILE file);
	OPERATE_RET (*fseek)(TUYA_FILE file, INT64_T offs, INT_T whence);
	INT64_T (*ftell)(TUYA_FILE file);
} TUYA_OS_FS_INTF;

/********************************************************************************
 *********************************tuya_os_memory_intf****************************
 ********************************************************************************/
typedef struct {
	VOID_T *(*malloc)(CONST size_t size);
	VOID_T (*free)(VOID_T *ptr);
	VOID_T *(*calloc)(size_t nitems, size_t size);
	VOID_T *(*realloc)(VOID_T *ptr, size_t size);
} TUYA_OS_MEMORY_INTF;

/********************************************************************************
 *********************************tuya_os_mutex_intf*****************************
 ********************************************************************************/
typedef void *MUTEX_HANDLE;

typedef struct {
	OPERATE_RET (*init)(MUTEX_HANDLE *pMutexHandle);
	OPERATE_RET (*lock)(const MUTEX_HANDLE mutexHandle);
	OPERATE_RET (*unlock)(const MUTEX_HANDLE mutexHandle);
	OPERATE_RET (*release)(const MUTEX_HANDLE mutexHandle);
} TUYA_OS_MUTEX_INTF;

/********************************************************************************
 *********************************tuya_os_network_intf***************************
 ********************************************************************************/

/* tuya sdk definition of 127.0.0.1 */
#define TY_IPADDR_LOOPBACK ((uint32_t)0x7f000001UL)
/* tuya sdk definition of 0.0.0.0 */
#define TY_IPADDR_ANY ((uint32_t)0x00000000UL)
/* tuya sdk definition of 255.255.255.255 */
#define TY_IPADDR_BROADCAST ((uint32_t)0xffffffffUL)

/* tuya sdk definition of shutdown type */
#define UNW_SHUT_RD 0
#define UNW_SHUT_WR 1
#define UNW_SHUT_RDWR 2

#if defined(OPERATING_SYSTEM) &&                                                                   \
    ((OPERATING_SYSTEM == SYSTEM_LINUX) || (OPERATING_SYSTEM == SYSTEM_LITEOS))
/* fd 最大个数, 不同平台根据实际情况定义 */
#define UNW_FD_MAX_COUNT (1024)
#else
#define UNW_FD_MAX_COUNT (64)
#endif
/* tuya sdk definition of fd operations */
typedef struct {
	UINT8_T placeholder[(UNW_FD_MAX_COUNT + 7) / 8];
} UNW_FD_SET_T;

/* tuya sdk definition of socket protocol */
typedef enum {
	PROTOCOL_TCP = 0,
	PROTOCOL_UDP = 1,
	PROTOCOL_RAW = 2,
} UNW_PROTOCOL_TYPE;

/* tuya sdk definition of transfer type */
typedef enum {
	TRANS_RECV = 0,
	TRANS_SEND = 1,
} UNW_TRANS_TYPE_E;

/* tuya sdk definition of IP addr */
typedef UINT_T UNW_IP_ADDR_T;

/* tuya sdk definition of IP info */
typedef struct {
	char ip[16];   /* ip addr:  xxx.xxx.xxx.xxx  */
	char mask[16]; /* net mask: xxx.xxx.xxx.xxx  */
	char gw[16];   /* gateway:  xxx.xxx.xxx.xxx  */
} NW_IP_S;

// the MAC addr len
#define TY_MAC_ADDR_LEN 6

/* tuya sdk definition of MAC info */
typedef struct {
	uint8_t mac[TY_MAC_ADDR_LEN]; /* mac address */
} NW_MAC_S;

/* add begin: by sunkz, interface regist */
typedef struct {
	int (*get_errno)(void);
	int (*fd_set1)(int fd, UNW_FD_SET_T *fds);
	int (*fd_clear)(int fd, UNW_FD_SET_T *fds);
	int (*fd_isset)(int fd, UNW_FD_SET_T *fds);
	int (*fd_zero)(UNW_FD_SET_T *fds);
	int (*select)(const int maxfd, UNW_FD_SET_T *readfds, UNW_FD_SET_T *writefds,
	              UNW_FD_SET_T *errorfds, const UINT_T ms_timeout);
	int (*get_nonblock)(const int fd);
	int (*set_block)(const int fd, const BOOL_T block);
	int (*close)(const int fd);
	int (*shutdown)(const int fd, const int how);
	int (*socket_create)(const UNW_PROTOCOL_TYPE type);
	int (*connect)(const int fd, const UNW_IP_ADDR_T addr, const UINT16_T port);
	int (*connect_raw)(const int fd, void *p_socket, const int len);
	int (*bind)(const int fd, const UNW_IP_ADDR_T addr, const UINT16_T port);
	int (*listen)(const int fd, const int backlog);
	int (*send)(const int fd, const void *buf, const UINT_T nbytes);
	int (*send_to)(const int fd, const void *buf, const UINT_T nbytes, const UNW_IP_ADDR_T addr,
	               const UINT16_T port);
	int (*recv)(const int fd, void *buf, const UINT_T nbytes);
	int (*recvfrom)(const int fd, void *buf, const UINT_T nbytes, UNW_IP_ADDR_T *addr,
	                UINT16_T *port);
	int (*set_timeout)(const int fd, const int ms_timeout, const UNW_TRANS_TYPE_E type);
	int (*set_bufsize)(const int fd, const int buf_size, const UNW_TRANS_TYPE_E type);
	int (*set_reuse)(const int fd);
	int (*disable_nagle)(const int fd);
	int (*set_boardcast)(const int fd);
	int (*gethostbyname)(const char *domain, UNW_IP_ADDR_T *addr);
	int (*accept)(const int fd, UNW_IP_ADDR_T *addr, UINT16_T *port);
	int (*recv_nd_size)(const int fd, void *buf, const UINT_T buf_size, const UINT_T nd_size);
	UNW_IP_ADDR_T (*str2addr)(const char *ip_str);
	char *(*addr2str)(UNW_IP_ADDR_T ipaddr);
	int (*set_keepalive)(int fd, const bool_t alive, const UINT_T idle, const UINT_T intr,
	                     const UINT_T cnt);
	int (*socket_bind)(int fd, const char *ip);
	int (*set_cloexec)(const int fd);
	int (*get_socket_ip)(int fd, UNW_IP_ADDR_T *addr);
	UNW_IP_ADDR_T (*addr)(const char *cp);
} TUYA_OS_NETWORK_INTF;

/********************************************************************************
 *********************************tuya_os_output_intf****************************
 ********************************************************************************/
typedef struct {
	VOID_T (*output_log)(CONST CHAR_T *str);
	OPERATE_RET (*log_close)(VOID_T);
	OPERATE_RET (*log_open)(VOID_T);
} TUYA_OS_OUTPUT_INTF;

/********************************************************************************
 *********************************tuya_os_semaphore_intf*************************
 ********************************************************************************/
typedef void *SEM_HANDLE;
#define TUYA_OS_ADAPT_SEM_FOREVER 0xFFFFffff

typedef struct {
	OPERATE_RET (*init)(SEM_HANDLE *pHandle, CONST UINT_T semCnt, CONST UINT_T sem_max);
	OPERATE_RET (*wait)(CONST SEM_HANDLE semHandle);
	OPERATE_RET (*waittimeout)(CONST SEM_HANDLE semHandle, unsigned int timeout);
	OPERATE_RET (*post)(CONST SEM_HANDLE semHandle);
	OPERATE_RET (*release)(CONST SEM_HANDLE semHandle);
} TUYA_OS_SEMAPHORE_INTF;

/********************************************************************************
 *********************************tuya_os_system_intf****************************
 ********************************************************************************/
#if OPERATING_SYSTEM == SYSTEM_LINUX
typedef UINT64_T SYS_TICK_T;
#else
typedef UINT_T SYS_TICK_T;
#endif

typedef enum {
	TY_RST_POWER_OFF = 0,
	TY_RST_HARDWARE_WATCHDOG,
	TY_RST_FATAL_EXCEPTION,
	TY_RST_SOFTWARE_WATCHDOG,
	TY_RST_SOFTWARE,
	TY_RST_DEEPSLEEP,
	TY_RST_HARDWARE,
	TY_RST_OTHER = 0xAA,
	TY_RST_UNSUPPORT = 0xFF,
} TY_RST_REASON_E;

typedef enum {
	TY_CPU_SLEEP,
	TY_CPU_DEEP_SLEEP,
} TY_CPU_SLEEP_MODE_E;

typedef struct {
	SYS_TICK_T (*get_systemtickcount)(void);
	UINT_T (*get_tickratems)(void);
	void (*system_sleep)(const unsigned long msTime);
	void (*system_reset)(void);
	unsigned int (*watchdog_init_start)(const unsigned int timeval);
	void (*watchdog_refresh)(void);
	void (*watchdog_stop)(void);
	int (*system_getheapsize)(void);
	TY_RST_REASON_E (*system_get_rst_info)(void);
	OPERATE_RET (*system_get_rst_ext_info)(CHAR_T **ext_info);
	int (*get_random_data)(const unsigned int range);
	int (*set_cpu_lp_mode)(const bool_t en, const TY_CPU_SLEEP_MODE_E mode);
} TUYA_OS_SYSTEM_INTF;

/********************************************************************************
 *********************************tuya_os_thread_intf****************************
 ********************************************************************************/
typedef void *THREAD_HANDLE;
typedef void (*THREAD_FUNC_T)(void *);

typedef struct {
	OPERATE_RET(*create)
	(THREAD_HANDLE *thread, CONST char *name, UINT_T stack_size, UINT_T priority,
	 THREAD_FUNC_T func, VOID_T *CONST arg);
	OPERATE_RET (*release)(THREAD_HANDLE thread);
	OPERATE_RET (*is_self)(THREAD_HANDLE thread, BOOL_T *is_self);
	OPERATE_RET (*set_self_name)(CONST CHAR_T *name);
	OPERATE_RET (*watermark)(THREAD_HANDLE thread, UINT_T *watermark);
} TUYA_OS_THREAD_INTF;

/********************************************************************************
 *********************************tuya_os_queue_intf****************************
 ********************************************************************************/
/* tuya sdk definition of queue context */
typedef void *QUEUE_HANDLE;

typedef struct {
	OPERATE_RET (*init)(QUEUE_HANDLE *queue, int msgsize, int msgcount);
	VOID_T (*free)(QUEUE_HANDLE queue);
	OPERATE_RET (*post)(QUEUE_HANDLE queue, void *data, unsigned int timeout);
	OPERATE_RET (*fetch)(QUEUE_HANDLE queue, void *msg, unsigned int timeout);
} TUYA_OS_QUEUE_INTF;

/********************************************************************************
 *********************************tuya_os_bt_intf********************************
 ********************************************************************************/
#define DEVICE_NAME_LEN 16

typedef INT_T BT_ABILITY;
#define BT_ABI_NEED_RESET_STACK (1 << 0)
//#define BT_ABI_OTHER   (1<<1)

typedef enum {
	TY_BT_SCAN_BY_NAME = 0x01,
	TY_BT_SCAN_BY_MAC = 0x02,
	TY_BT_SCAN_BY_ADV = 0x03, //接收蓝牙广播
} ty_bt_scan_type_t;

typedef enum {
	TY_BT_EVENT_DISCONNECTED, /* APP断开连接 */
	TY_BT_EVENT_CONNECTED,    /* APP连接上设备 */
	TY_BT_EVENT_RX_DATA,      /* 接收到APP数据*/
	TY_BT_EVENT_ADV_READY,    /* start adv. */
} ty_bt_cb_event_t;

typedef enum {
	TY_BT_MODE_PERIPHERAL = BIT(0),
	TY_BT_MODE_CENTRAL = BIT(1),
	TY_BT_MODE_MESH_PROVISIONER = BIT(2),
	TY_BT_MODE_MESH_DEVICE = BIT(3),
} ty_bt_mode_t;

typedef struct {
	unsigned int len;
	unsigned char *data;
} tuya_ble_data_buf_t;

typedef enum {
	TY_BT_MESH_PROV_EVENT_STACK_STATE,
	TY_BT_MESH_PROV_EVENT_ADD_DEV,
	TY_BT_MESH_PROV_EVENT_DEL_DEV,
	TY_BT_MESH_PROV_EVENT_RECV_DATA,
	TY_BT_MESH_PROV_EVENT_RECV_UNPROV_ADV,
	TY_BT_MESH_PROV_EVENT_MAX,
} ty_bt_mesh_prov_cb_event_t;

typedef enum {
	TY_BT_MESH_PROV_STACK_INIT_STATE_IDLE,
	TY_BT_MESH_PROV_STACK_INIT_STATE_INITING,
	TY_BT_MESH_PROV_STACK_INIT_STATE_READY,
	TY_BT_MESH_PROV_STACK_INIT_STATE_DEINITING,
} ty_bt_mesh_prov_init_state_t;

typedef union {
	struct ble_mesh_prov_stack_state_param {
		INT_T err_code; /*!< Indicate the result of BLE Mesh initialization */
		ty_bt_mesh_prov_init_state_t init_state; /*!< Mesh stack init state */
	} prov_stack_state; /*!< Event parameter of TY_BT_MESH_PROV_EVENT_STACK_STATE */
	struct ble_mesh_prov_add_comp_param {
		INT_T err_code;        /*!< Indicate the result of BLE Mesh initialization */
		UINT16_T unicast_addr; /*!< Primary address of the provisioned device */
		UINT8_T dev_uuid[16];  /*!< Device UUID of the unprovisioned device */
		UINT8_T mac[6];        /*!< Device address of the unprovisioned device */
		UINT8_T dev_key[16];
		UINT8_T *composite_data;
		UINT16_T composite_data_len;
	} prov_add_comp; /*!< Event parameter of TY_BT_MESH_PROV_EVENT_ADD_DEV */

	struct ble_mesh_prov_del_comp_param {
		INT_T err_code;        /*!< Indicate the result of BLE Mesh initialization */
		UINT16_T unicast_addr; /*!< Primary address of the provisioned device */
	} prov_del_comp;           /*!< Event parameter of TY_BT_MESH_PROV_EVENT_DEL_DEV */

	struct ble_mesh_prov_recv_data_param {
		INT_T err_code;        /*!< Indicate the result of BLE Mesh initialization */
		UINT16_T unicast_addr; /*!< Primary address of the provisioned device */
		UINT8_T *data;
		UINT_T len;
	} prov_recv_data; /*!< Event parameter of TY_BT_MESH_PROV_EVENT_RECV_DATA */

	struct ble_mesh_prov_recv_unprov_adv_pkt_param {
		UINT8_T dev_uuid[16];   /*!< Device UUID of the unprovisioned device */
		UINT8_T mac[6];         /*!< Device address of the unprovisioned device */
		UINT8_T addr_type;      /*!< Device address type */
		UINT16_T oob_info;      /*!< OOB Info of the unprovisioned device */
		UINT8_T adv_type;       /*!< Avertising type of the unprovisioned device */
		INT8_T rssi;            /*!< RSSI of the received advertising packet */
	} prov_recv_unprov_adv_pkt; /*!< Event parameter of TY_BT_MESH_PROV_EVENT_RECV_UNPROV_ADV */

} ty_bt_mesh_prov_cb_param_t;

typedef enum {
	TY_BLE_CENTRAL_STACK_INIT_STATE_IDLE,
	TY_BLE_CENTRAL_STACK_INIT_STATE_INITING,
	TY_BLE_CENTRAL_STACK_INIT_STATE_READY,
	TY_BLE_CENTRAL_STACK_INIT_STATE_DEINITING,
} ty_ble_central_init_state_t;

typedef enum {
	TY_BLE_GAP_ADV_EVT_TYPE_UNDIRECTED = 0,  /**<  Connectable  undirected advertising. */
	TY_BLE_GAP_ADV_EVT_TYPE_DIRECTED = 1,    /**<  Connectable directed advertising. */
	TY_BLE_GAP_ADV_EVT_TYPE_SCANNABLE = 2,   /**<  Scanable undirected advertising. */
	TY_BLE_GAP_ADV_EVT_TYPE_NON_CONNECTABLE, /**<  Nonconnectable undirected advertising. */
	TY_BLE_GAP_ADV_EVT_TYPE_SCAN_RSP         /**<  scan response. */
} ty_ble_gap_adv_type_t;

typedef struct {
#define TY_BT_UUID_LEN_16 2
#define TY_BT_UUID_LEN_32 4
#define TY_BT_UUID_LEN_128 16
	UINT16_T len; /*!< UUID length, 16bit, 32bit or 128bit */
	union {
		UINT16_T uuid16;                     /*!< 16bit UUID */
		UINT_T uuid32;                       /*!< 32bit UUID */
		UINT8_T uuid128[TY_BT_UUID_LEN_128]; /*!< 128bit UUID */
	} uuid;                                  /*!< UUID */
} __attribute__((packed)) ty_bt_uuid_t;

typedef enum {
	TUYA_BLE_ADDR_RANDOM = 0x01,
	TUYA_BLE_ADDR_PUBLIC = 0x02,
} ty_ble_addr_type_t;

typedef enum {
	TY_BLE_CENTRAL_EVENT_STACK_STATE,
	TY_BLE_CENTRAL_EVENT_GAP_RECV_SCAN_INFO,
	TY_BLE_CENTRAL_EVENT_GAP_CONN_EVT,
	TY_BLE_CENTRAL_EVENT_GAP_DISCONN_EVT,
	TY_BLE_CENTRAL_EVENT_GATTC_RECV_DATA,
	TY_BLE_CENTRAL_EVENT_GATTC_CFG_MTU_EVT,
	TY_BLE_CENTRAL_EVENT_GATTC_DIS_SRVC_CMPL_EVT, /*!< When the ble discover service complete, the
	                                                 event comes */
	TY_BLE_CENTRAL_EVENT_MAX,
} ty_ble_central_cb_event_t;

typedef union {
	struct ble_cent_stack_state_param {
		INT_T err_code; /*!< Indicate the result of BLE Central initialization */
		ty_ble_central_init_state_t init_state; /*!< BLE Central stack init state */
	} ble_cent_stack_state; /*!< Event parameter of TY_BLE_CENTRAL_EVENT_STACK_STATE */
	struct ble_gap_scan_info_param {
		UINT8_T mac[6];                      /**< Bluetooth address of remote device. */
		ty_ble_addr_type_t remote_addr_type; /**< Address type of remote device. */
		ty_ble_gap_adv_type_t adv_type;      /**< Advertising event type. */
		INT8_T rssi;                         /**< RSSI. */
		UINT8_T data_len;
		UINT8_T data[31];
	} gap_scan_info; /*!< Event parameter of TY_BLE_CENTRAL_EVENT_GAP_RECV_SCAN_INFO */
	struct ble_gap_conn_evt_param {
		UINT8_T mac[6]; /**< Bluetooth address of remote device. */
	} gap_conn_evt;     /*!< Event parameter of TY_BLE_CENTRAL_EVENT_GAP_CONN_EVT */
	struct ble_gap_disconn_evt_param {
		INT_T err_code; /*!< Indicate the result of BLE Central initialization */
		BOOL_T discovery_success;
		UINT8_T mac[6]; /**< Bluetooth address of remote device. */
	} gap_disconn_evt;  /*!< Event parameter of TY_BLE_CENTRAL_EVENT_GAP_DISCONN_EVT */
	struct ble_gattc_cfg_mtu_evt_param {
		UINT8_T mac[6];   /**< Bluetooth address of remote device. */
		UINT16_T conn_id; /*!< Connection id */
		UINT16_T mtu;     /*!< MTU size */
	} gattc_cfg_mtu;      /*!< Event parameter of TY_BLE_CENTRAL_EVENT_GATTC_CFG_MTU_EVT */
	struct gattc_dis_srvc_cmpl_evt_param {
		INT_T err_code; /*!< Indicate the result of BLE Central initialization */
		UINT8_T mac[6]; /**< Bluetooth address of remote device. */
		ty_ble_addr_type_t remote_addr_type; /**< Address type of remote device. */
		UINT16_T conn_id;                    /*!< Connection id */
	} gattc_dis_srvc_cmpl; /*!< Event parameter of TY_BLE_CENTRAL_EVENT_GATTC_DIS_SRVC_CMPL_EVT */
	struct ble_gattc_recv_data_param {
		UINT8_T mac[6]; /**< Bluetooth address of remote device. */
		UINT_T data_len;
		UINT8_T *data;
	} gattc_recv_data; /*!< Event parameter of TY_BLE_CENTRAL_EVENT_GATTC_RECV_DATA */
} ty_ble_central_cb_param_t;

typedef VOID (*TY_BT_MSG_CB)(INT_T id, ty_bt_cb_event_t e, tuya_ble_data_buf_t *buf);
typedef VOID (*TY_BT_MESH_PROV_MSG_CB)(ty_bt_mesh_prov_cb_event_t event,
                                       ty_bt_mesh_prov_cb_param_t *params);
typedef VOID (*TY_BLE_CENTRAL_MSG_CB)(ty_ble_central_cb_event_t event,
                                      ty_ble_central_cb_param_t *params);

typedef struct {
	CHAR_T name[DEVICE_NAME_LEN];
	ty_bt_mode_t mode;
	UCHAR_T link_num;
	TY_BT_MSG_CB cb;
	tuya_ble_data_buf_t adv;
	tuya_ble_data_buf_t scan_rsp;
} ty_bt_param_t;

typedef VOID (*TY_BT_SCAN_ADV_CB)(CHAR_T *data, UINT_T len, UCHAR_T *mac, UINT8_T type);
typedef struct {
	CHAR_T scan_type; /* ref ty_bt_scan_type_t. */
	CHAR_T name[DEVICE_NAME_LEN];
	CHAR_T bd_addr[6];
	SCHAR_T rssi;
	UCHAR_T channel;
	UCHAR_T timeout_s; /* second. */
	TY_BT_SCAN_ADV_CB scan_adv_cb;
} ty_bt_scan_info_t;

#define TY_BT_MESH_APP_KEY_LEN 16
#define TY_BT_MESH_NET_KEY_LEN 16
typedef struct {
	UINT8_T netkey[TY_BT_MESH_NET_KEY_LEN];
	UINT8_T appkey[TY_BT_MESH_APP_KEY_LEN];
	UINT16_T local_addr;
} ty_ble_mesh_info_t;

typedef struct {
	/** Scan interval in 0.625ms units */
	UINT16_T scan_itvl;

	/** Scan window in 0.625ms units */
	UINT16_T scan_window;

	/** Minimum value for connection interval in 1.25ms units */
	UINT16_T itvl_min;

	/** Maximum value for connection interval in 1.25ms units */
	UINT16_T itvl_max;

	/** Connection latency */
	UINT16_T latency;

	/** Supervision timeout in 10ms units */
	UINT16_T supervision_timeout;

	/** Minimum length of connection event in 0.625ms units */
	UINT16_T min_ce_len;

	/** Maximum length of connection event in 0.625ms units */
	UINT16_T max_ce_len;
} ty_ble_gap_conn_params_t;

typedef struct {
	TY_BLE_CENTRAL_MSG_CB ble_central_cb;
} ty_ble_central_param_t;

typedef struct {
	OPERATE_RET (*port_init)(ty_bt_param_t *p);
	OPERATE_RET (*port_deinit)(VOID_T);
	OPERATE_RET (*gap_disconnect)(VOID_T);
	OPERATE_RET (*send)(BYTE_T *data, UINT8_T len);
	OPERATE_RET (*reset_adv)(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *scan_resp);
	OPERATE_RET (*get_rssi)(SCHAR_T *rssi);
	OPERATE_RET (*start_adv)(VOID_T);
	OPERATE_RET (*stop_adv)(VOID_T);
	OPERATE_RET (*assign_scan)(INOUT ty_bt_scan_info_t *info);
	OPERATE_RET (*scan_init)(IN TY_BT_SCAN_ADV_CB scan_adv_cb);
	OPERATE_RET (*start_scan)(VOID_T);
	OPERATE_RET (*stop_scan)(VOID_T);
	OPERATE_RET (*get_ability)(VOID_T);
	OPERATE_RET (*set_mac)(CONST NW_MAC_S *mac);
	OPERATE_RET (*get_mac)(NW_MAC_S *mac);
	OPERATE_RET(*send_beacon)
	(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *scan_resp, UINT16_T interval, UINT16_T timeout);

	OPERATE_RET (*central_init)(ty_ble_central_param_t *p);
	OPERATE_RET (*central_deinit)(VOID_T);
	OPERATE_RET(*master_connect)
	(UINT8_T mac[6], ty_ble_addr_type_t type, UINT_T timeout_ms, ty_ble_gap_conn_params_t *params);
	OPERATE_RET (*master_disconnect)(UINT8_T mac[6], ty_ble_addr_type_t type);
	OPERATE_RET (*master_scan)(BOOL_T enable);
	OPERATE_RET (*master_get_max_slave_num)(UINT16_T *num);
	OPERATE_RET (*gattc_send_data)(UINT8_T mac[6], UINT8_T *data, UINT_T length);
	OPERATE_RET (*gattc_start_discovery)(UINT8_T mac[6], ty_bt_uuid_t *uuid);
} TUYA_OS_BT_INTF;

typedef struct {
	TY_BT_MESH_PROV_MSG_CB mesh_prov_cb;
} ty_ble_mesh_prov_param_t;

typedef struct {
	OPERATE_RET (*mesh_prov_init)(ty_ble_mesh_prov_param_t *p);
	OPERATE_RET (*mesh_prov_deinit)(VOID_T);
	OPERATE_RET (*mesh_prov_set_info)(ty_ble_mesh_info_t *info);
	OPERATE_RET (*mesh_prov_set_addr)(UINT16_T mesh_addr);
	OPERATE_RET (*mesh_prov_scan)(BOOL_T enable);
	OPERATE_RET (*mesh_prov_add_dev)(UINT8_T uuid[16], UINT8_T mac[6], UINT16_T mesh_addr);
	OPERATE_RET (*mesh_prov_del_dev_with_addr)(UINT16_T mesh_addr, UINT8_T dev_key[16]);
	OPERATE_RET(*mesh_prov_send_data)
	(UINT_T opcode, UINT16_T mesh_addr, UINT8_T *data, UINT_T length, UINT8_T dev_key[16]);
	OPERATE_RET (*mesh_prov_node_reset)(VOID_T);
} TUYA_OS_BLE_MESH_RPOV_INTF;

/********************************************************************************
 *********************************tuya_os_wifi_intf******************************
 ********************************************************************************/
#define TAG_SSID_NUMBER 0
#define TAG_PAYLOAD_NUMBER 221

#define PROBE_REQUEST_TYPE_SUBTYPE 0x0040
#define PROBE_REQSPONSE_TYPE_SUBTYPE 0x0050
#define PROBE_REQUEST_DURATION_ID 0x0
#define PROBE_RESPONSET_DURATION_ID 0x0
#define PROBE_REQUEST_PAYLOAD_LEN_MAX 255
#define BROADCAST_MAC_ADDR 0xFFFFFFFF

/* tuya sdk definition of wifi ap info */
#define WIFI_SSID_LEN 32   // tuya sdk definition WIFI SSID MAX LEN
#define WIFI_PASSWD_LEN 64 // tuya sdk definition WIFI PASSWD MAX LEN
typedef struct {
	unsigned char channel;                 ///< AP channel
	signed char rssi;                      ///< AP rssi
	unsigned char bssid[6];                ///< AP bssid
	unsigned char ssid[WIFI_SSID_LEN + 1]; ///< AP ssid array
	unsigned char s_len;                   ///< AP ssid len
	unsigned char security;                // refer to WF_AP_AUTH_MODE_E
	unsigned char resv1;
	uint8_t data_len;
	uint8_t data[0];
} AP_IF_S;

typedef enum { COUNTRY_CODE_CN, COUNTRY_CODE_US, COUNTRY_CODE_JP, COUNTRY_CODE_EU } COUNTRY_CODE_E;

/**
 * @brief WIFI芯片探测本地AP信息结构体
 * @struct MIMO_IF_S
 */
typedef enum {
	MIMO_TYPE_NORMAL = 0,
	MIMO_TYPE_HT40,
	MIMO_TYPE_2X2,
	MIMO_TYPE_LDPC,

	MIMO_TYPE_NUM,
} MIMO_TYPE_E;

typedef struct {
	signed char rssi;      /*!< MIMO包信号 */
	MIMO_TYPE_E type;      /*!< MIMO包类型 */
	unsigned short len;    /*!< MIMO包长度 */
	unsigned char channel; /*!< MIMO包信道 */
	unsigned char mcs;
} MIMO_IF_S;

typedef enum {
	WFT_PROBE_REQ = 0x40, ///< Probe request
	WFT_PROBE_RSP = 0x50, ///< Probe response
	WFT_AUTH = 0xB0,      // auth
	WFT_BEACON = 0x80,    ///< Beacon
	WFT_DATA = 0x08,      ///< Data
	WFT_QOS_DATA = 0x88,  ///< QOS Data
	WFT_MIMO_DATA = 0xff, ///< MIMO Data
} WLAN_FRM_TP_E;

#pragma pack(1)
typedef struct {
	// 802.11 management
	unsigned char id;
	unsigned char len;
	char data[0]; ///< data
} WLAN_MANAGEMENT_S;

typedef struct {
	unsigned char frame_type;       ///< WLAN Frame type
	unsigned char frame_ctrl_flags; ///< Frame Control flags
	unsigned short duration;        ///< Duration
	unsigned char dest[6];          ///< Destination MAC Address
	unsigned char src[6];           ///< Source MAC Address
	unsigned char bssid[6];         ///< BSSID MAC Address
	unsigned short seq_frag_num;    ///< Sequence and Fragmentation number
} WLAN_PROBE_REQ_IF_S;

typedef struct {
	unsigned char frame_ctrl_flags; ///< Frame Control flags
	unsigned short duration;        ///< Duration
	unsigned char dest[6];          ///< Destination MAC Address
	unsigned char src[6];           ///< Source MAC Address
	unsigned char bssid[6];         ///< BSSID MAC Address
	unsigned short seq_frag_num;    ///< Sequence and Fragmentation number
	unsigned char timestamp[8];     ///< Time stamp
	unsigned short beacon_interval; ///< Beacon Interval
	unsigned short cap_info;        ///< Capability Information
	unsigned char ssid_element_id;  ///< SSID Element ID
	unsigned char ssid_len;         ///< SSID Length
	char ssid[0];                   ///< SSID
} WLAN_BEACON_IF_S;

#define TO_FROM_DS_MASK 0x03
#define TFD_IBSS 0x00    ///< da+sa+bssid
#define TFD_TO_AP 0x01   ///< bssid+sa+da
#define TFD_FROM_AP 0x02 ///< ds+bssid+sa
#define TFD_WDS 0x03     ///< ra+ta+da

typedef unsigned char BC_DA_CHAN_T;
#define BC_TO_AP 0
#define BC_FROM_AP 1
#define BC_CHAN_NUM 2

typedef struct {
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char addr3[6];
} WLAN_COM_ADDR_S;

typedef struct {
	unsigned char bssid[6];
	unsigned char src[6];
	unsigned char dst[6];
} WLAN_TO_AP_ADDR_S;

typedef struct {
	unsigned char dst[6];
	unsigned char bssid[6];
	unsigned char src[6];
} WLAN_FROM_AP_ADDR_S;

typedef union {
	WLAN_COM_ADDR_S com;
	WLAN_TO_AP_ADDR_S to_ap;
	WLAN_FROM_AP_ADDR_S from_ap;
} WLAN_ADDR_U;

typedef struct {
	unsigned char frame_ctrl_flags; ///< Frame Control flags
	unsigned short duration;        ///< Duration
	WLAN_ADDR_U addr;               ///< address
	unsigned short seq_frag_num;    ///< Sequence and Fragmentation number
	unsigned short qos_ctrl;        ///< QoS Control bits
} WLAN_DATA_IF_S;

/*!
\brief WLAN Frame info
\struct WLAN_FRAME_S
*/
typedef struct {
	unsigned char frame_type; ///< WLAN Frame type
	union {
		WLAN_BEACON_IF_S beacon_info; ///< WLAN Beacon info
		WLAN_DATA_IF_S data_info;     ///< WLAN Data info
		MIMO_IF_S mimo_info;          ///< mimo info
	} frame_data;
} WLAN_FRAME_S, *P_WLAN_FRAME_S;

typedef struct {
	unsigned short type_and_subtype;
	unsigned short duration_id;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char addr3[6];
	unsigned short seq_ctrl;
} PROBE_REQUEST_PACKAGE_HEAD_S;

typedef struct {
	uint16_t type_and_subtype;
	uint16_t duration_id;
	uint8_t addr1[6];
	uint8_t addr2[6];
	uint8_t addr3[6];
	uint16_t seq_ctrl;
	uint8_t timestamp[8];     // Time stamp
	uint16_t beacon_interval; // Beacon Interval
	uint16_t cap_info;        // Capability Information
} PROBE_RESPONSE_PACKAGE_HEAD_S;

typedef struct {
	unsigned char index;
	unsigned char len;
	unsigned char ptr[0];
} BEACON_TAG_DATA_UNIT_S;
#pragma pack()

typedef struct {
	PROBE_REQUEST_PACKAGE_HEAD_S pack_head;
	BEACON_TAG_DATA_UNIT_S tag_ssid;
} PROBE_REQUEST_FIX_S;

/* tuya sdk definition of wifi function type */
typedef enum {
	WF_STATION = 0, ///< station type
	WF_AP,          ///< ap type
} WF_IF_E;

/* tuya sdk definition of wifi work mode */
typedef enum {
	WWM_LOWPOWER = 0, ///< wifi work in lowpower mode
	WWM_SNIFFER,      ///< wifi work in sniffer mode
	WWM_STATION,      ///< wifi work in station mode
	WWM_SOFTAP,       ///< wifi work in ap mode
	WWM_STATIONAP,    ///< wifi work in station+ap mode
	WWM_UNKNOWN,      ///< wifi work in unknown mode
} WF_WK_MD_E;

/* tuya sdk definition of wifi encryption type */
typedef enum {
	WAAM_OPEN = 0,     ///< open
	WAAM_WEP,          ///< WEP
	WAAM_WPA_PSK,      ///< WPA—PSK
	WAAM_WPA2_PSK,     ///< WPA2—PSK
	WAAM_WPA_WPA2_PSK, ///< WPA/WPA2
	WAAM_UNKNOWN,      // unknown
} WF_AP_AUTH_MODE_E;

/* tuya sdk definition of ap config info */
typedef struct {
	unsigned char ssid[WIFI_SSID_LEN + 1];     ///< ssid
	unsigned char s_len;                       ///< len of ssid
	unsigned char passwd[WIFI_PASSWD_LEN + 1]; ///< passwd
	unsigned char p_len;                       ///< len of passwd
	unsigned char chan;                        ///< channel. default:6
	WF_AP_AUTH_MODE_E md;                      ///< encryption type
	unsigned char ssid_hidden;                 ///< ssid hidden  default:0
	unsigned char max_conn;                    ///< max sta connect nums default:3
	unsigned short ms_interval;                ///< broadcast interval default:100
	NW_IP_S ip;                                ///< ip info for ap mode
} WF_AP_CFG_IF_S;

/* tuya sdk definition of wifi station work status */
typedef enum {
	WSS_IDLE = 0,     ///< not connected
	WSS_CONNECTING,   ///< connecting wifi
	WSS_PASSWD_WRONG, ///< passwd not match
	WSS_NO_AP_FOUND,  ///< ap is not found
	WSS_CONN_FAIL,    ///< connect fail
	WSS_CONN_SUCCESS, ///< connect wifi success
	WSS_GOT_IP,       ///< get ip success
} WF_STATION_STAT_E;

/* for fast connect*/
typedef struct {
	unsigned int len;      ///< data len
	unsigned char data[0]; ///< data buff
} FAST_WF_CONNECTED_AP_INFO_V2_S;

/**
 * @brief callback function: SNIFFER_CALLBACK
 *        when wifi sniffers package from air, notify tuya-sdk
 *        with this callback. the package should include
 * @param[in]       buf         the buf wifi recv
 * @param[in]       len         the len of buf
 */
typedef void (*SNIFFER_CALLBACK)(CONST UINT8_T *buf, CONST UINT16_T len, CONST SCHAR_T rssi);

/**
 * @brief receive wifi management callback
 *
 */
typedef void (*WIFI_REV_MGNT_CB)(UCHAR_T *buf, UINT_T len);

typedef struct {
	OPERATE_RET (*all_ap_scan)(AP_IF_S **ap_ary, UINT_T *num);
	OPERATE_RET (*assign_ap_scan)(CONST SCHAR_T *ssid, AP_IF_S **ap);
	OPERATE_RET (*release_ap)(AP_IF_S *ap);
	OPERATE_RET (*set_cur_channel)(CONST UCHAR_T chan);
	OPERATE_RET (*get_cur_channel)(UCHAR_T *chan);
	OPERATE_RET (*sniffer_set)(CONST BOOL_T en, CONST SNIFFER_CALLBACK cb);
	OPERATE_RET (*get_ip)(CONST WF_IF_E wf, NW_IP_S *ip);
	OPERATE_RET (*set_mac)(CONST WF_IF_E wf, CONST NW_MAC_S *mac);
	OPERATE_RET (*get_mac)(CONST WF_IF_E wf, NW_MAC_S *mac);
	OPERATE_RET (*set_work_mode)(CONST WF_WK_MD_E mode);
	OPERATE_RET (*get_work_mode)(WF_WK_MD_E *mode);
	OPERATE_RET (*ap_start)(CONST WF_AP_CFG_IF_S *cfg);
	OPERATE_RET (*ap_stop)(VOID_T);
	OPERATE_RET (*get_connected_ap_info_v2)(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info);
	OPERATE_RET (*fast_station_connect_v2)(CONST FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info);
	OPERATE_RET (*station_connect)(CONST SCHAR_T *ssid, CONST SCHAR_T *passwd);
	OPERATE_RET (*station_disconnect)(VOID_T);
	OPERATE_RET (*station_get_conn_ap_rssi)(SCHAR_T *rssi);
	OPERATE_RET (*get_bssid)(UCHAR_T *mac);
	OPERATE_RET (*station_get_status)(WF_STATION_STAT_E *stat);
	OPERATE_RET (*set_country_code)(CONST COUNTRY_CODE_E ccode);
	OPERATE_RET (*send_mgnt)(CONST UCHAR_T *buf, CONST UINT_T len);
	OPERATE_RET (*register_recv_mgnt_callback)(CONST BOOL_T enable, CONST WIFI_REV_MGNT_CB recv_cb);
	OPERATE_RET (*set_lp_mode)(CONST BOOL_T en, CONST UCHAR_T dtim);
	BOOL_T (*rf_calibrated)(VOID_T);
	OPERATE_RET (*send_beacon)(uint8_t channel, uint8_t *ssid, uint8_t ssid_len);
} TUYA_OS_WIFI_INTF;

/********************************************************************************
 *********************************tuya_os_storage_intf*****************************
 ********************************************************************************/
/**
 * @brief storage description
 *
 */
typedef struct {
	unsigned int start_addr; ///< user physical flash start address
	unsigned int flash_sz;   ///< user flash size
	unsigned int block_sz;   ///< flash block/sector size

	// For data backup and power down protection data recovery
	unsigned int swap_start_addr; ///< swap flash start address
	unsigned int swap_flash_sz;   ///< swap flash size

	/// for restore factor of flash encryption key
	unsigned int key_restore_addr;

	// for protected data
	unsigned int protected_addr;
	unsigned int flash_physical_block_sz;
} UNI_STORAGE_DESC_S;

#define PARTATION_NUM_MAX 3

/**
 * @brief UF partition
 *
 */
typedef struct {
	unsigned int uf_partition_start_addr;
	unsigned int uf_partiton_flash_sz;
} UF_PARTITION;

/**
 * @brief UF description
 *
 */
typedef struct {
	unsigned int sector_size;
	unsigned int uf_partition_num;
	UF_PARTITION uf_partition[PARTATION_NUM_MAX];
} UF_PARTITION_TABLE_S;

typedef UF_PARTITION_TABLE_S PARTITION_TABLE_S;

typedef struct {
	OPERATE_RET (*read)(CONST UINT_T addr, UCHAR_T *dst, CONST UINT_T size);
	OPERATE_RET (*write)(CONST UINT_T addr, CONST UCHAR_T *src, CONST UINT_T size);
	OPERATE_RET (*erase)(CONST UINT_T addr, CONST UINT_T size);
	UNI_STORAGE_DESC_S *(*get_storage_desc)(void);
	UF_PARTITION_TABLE_S *(*get_uf_desc)(void);
	PARTITION_TABLE_S *(*get_rcd_desc)(void);
	OPERATE_RET (*get_legacy_swap_desc)(UINT_T *addr, UINT_T *size);
} TUYA_OS_STORAGE_INTF;

/********************************************************************************
 *********************************tuya_os_ota_intf*****************************
 ********************************************************************************/
typedef enum {
	AIR_OTA_TYPE,
	UART_OTA_TYPE,
} OTA_TYPE;

typedef struct {
	OPERATE_RET (*start)(UINT_T file_size, OTA_TYPE type);
	OPERATE_RET(*process)
	(CONST UINT_T total_len, CONST UINT_T offset, CONST UCHAR_T *data, CONST UINT_T len,
	 UINT_T *remain_len, void *pri_data);
	OPERATE_RET (*end)(BOOL_T reset);
} TUYA_OS_OTA_INTF;

/********************************************************************************
 *********************************tuya_os_wired_intf*****************************
 ********************************************************************************/
typedef uint8_t GW_BASE_NW_STAT_T;
typedef struct {
	OPERATE_RET (*get_ip)(NW_IP_S *ip);
	OPERATE_RET (*get_mac)(NW_MAC_S *mac);
	OPERATE_RET (*set_mac)(CONST NW_MAC_S *mac);
	OPERATE_RET (*set_station_connect)(CONST CHAR_T *ssid, CONST CHAR_T *passwd);
	OPERATE_RET (*station_get_conn_ap_rssi)(SCHAR_T *rssi);
	OPERATE_RET (*get_nw_stat)(GW_BASE_NW_STAT_T *stat);
	OPERATE_RET (*if_connect_internet)(BOOL_T *status);
	BOOL_T (*need_cfg)(VOID);
	BOOL_T (*station_conn)(VOID);
} TUYA_OS_WIRED_INTF;

/********************************************************************************
 *********************************tuya_cat1_intf*****************************
 ********************************************************************************/
typedef struct {
	BOOL_T (*sim_card_test)(VOID);
	BOOL_T (*tf_card_test)(VOID);
	BOOL_T (*sound_loopback_test)(INT_T rate);
	BOOL_T (*uart_loopback_test)(VOID);
	unsigned int (*adc_self_test)(VOID);
} CELLULAR_TEST_INTF;

typedef enum {
	CELLULAR_GET_TEST_INTF_CMD = 0x00,
	CELLULAR_GET_CELLSITE_INFO_CMD = 0x01,
	CELLULAR_RECOVER_SYS_CONFIG = 0x02,
} CELLULAR_CMD_E;

typedef struct {
	unsigned char oper_id[6];   //运营商ID
	unsigned char area_code[2]; //小区编码
	unsigned char cell_id[4];   //基站编码
} CELL_SITE_INFO;

typedef struct {
	OPERATE_RET (*set_imei)(CONST CHAR_T imei[15 + 1]);
	OPERATE_RET (*get_imei)(CHAR_T imei[15 + 1]);
	OPERATE_RET (*get_iccid)(CHAR_T iccid[20 + 1]);
	OPERATE_RET (*get_ip)(NW_IP_S *ip);
	OPERATE_RET (*get_simcard_state)(uint8_t *state);
	OPERATE_RET (*get_network_state)(BOOL_T *state);
	OPERATE_RET (*get_rssi)(SCHAR_T *rssi);
	OPERATE_RET (*control)(uint8_t cmd, void *arg);
	BOOL_T (*rf_calibrated)(VOID_T);
} TUYA_OS_CELLULAR_INTF;

/**
 * @brief register system interfaces in various types of platform
 *
 * @param[in] type: interface type
 * @param[in] intf_ptr: interface pointer
 *
 * @note This API is used to register system interfaces in various types of platform.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_os_adapt_reg_intf(INTF_TYPE_ENUM type, void *intf_ptr);

/**
 * @brief Initialize system interface
 *
 * @param VOID
 *
 * @note This API is used to initialize system interface.
 *
 * @return VOID
 */
VOID_T tuya_os_adapter_intf_impl_init(VOID_T);

/**
 * @brief Get the pointer of various types of system interfaces
 *
 * @param VOID
 *
 * @note This API is used to get the pointer of various types of system interfaces.
 *
 * @return the pointer of system interface
 */
VOID_T *tuya_os_adapter_get_intf(INTF_TYPE_ENUM type);

/**
 * @brief Get version of platform
 *
 * @param VOID
 *
 * @note This API is used to get the version of platform.
 *
 * @return the version of platform
 */
CHAR_T *tuya_os_adapter_get_platform_ver(VOID_T);

#ifdef __cplusplus
}
#endif

#endif /* _TUYA_OS_ADAPTER_H */
