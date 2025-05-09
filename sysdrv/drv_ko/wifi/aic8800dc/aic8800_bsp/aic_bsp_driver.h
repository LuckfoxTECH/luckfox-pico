/**
 ******************************************************************************
 *
 * rwnx_cmds.h
 *
 * Copyright (C) RivieraWaves 2014-2019
 *
 ******************************************************************************
 */

#ifndef _AIC_BSP_DRIVER_H
#define _AIC_BSP_DRIVER_H

#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/module.h>
#include "aic_bsp_export.h"

#define RWNX_80211_CMD_TIMEOUT_MS 6000 //500//300

#define RWNX_CMD_FLAG_NONBLOCK BIT(0)
#define RWNX_CMD_FLAG_REQ_CFM BIT(1)
#define RWNX_CMD_FLAG_WAIT_PUSH BIT(2)
#define RWNX_CMD_FLAG_WAIT_ACK BIT(3)
#define RWNX_CMD_FLAG_WAIT_CFM BIT(4)
#define RWNX_CMD_FLAG_DONE BIT(5)
/* ATM IPC design makes it possible to get the CFM before the ACK,
 * otherwise this could have simply been a state enum */
#define RWNX_CMD_WAIT_COMPLETE(flags)                                          \
	(!(flags & (RWNX_CMD_FLAG_WAIT_ACK | RWNX_CMD_FLAG_WAIT_CFM)))

#define RWNX_CMD_MAX_QUEUED 8

#define IPC_E2A_MSG_PARAM_SIZE 256

#define RWNX_FN_ENTRY_STR ">>> %s()\n", __func__

/* message levels */
#define LOGERROR 0x0001
#define LOGINFO 0x0002
#define LOGTRACE 0x0004
#define LOGDEBUG 0x0008
#define LOGDATA 0x0010

extern int aicwf_dbg_level_bsp;

#define AICWF_LOG "AICWFDBG("

#define AICWFDBG(level, args, arg...)                                          \
	do {                                                                   \
		if (aicwf_dbg_level_bsp & level) {                             \
			printk(AICWF_LOG #level ")\t" args, ##arg);            \
		}                                                              \
	} while (0)

#define RWNX_DBG(fmt, ...)                                                     \
	do {                                                                   \
		if (aicwf_dbg_level_bsp & LOGTRACE) {                          \
			printk(AICWF_LOG "LOGTRACE)\t" fmt, ##__VA_ARGS__);    \
		}                                                              \
	} while (0)

/// Message structure for MSGs from Emb to App
struct ipc_e2a_msg {
	u16 id; ///< Message id.
	u16 dummy_dest_id;
	u16 dummy_src_id;
	u16 param_len; ///< Parameter embedded struct length.
	u32 pattern; ///< Used to stamp a valid MSG buffer
	u32 param[IPC_E2A_MSG_PARAM_SIZE]; ///< Parameter embedded struct. Must be word-aligned.
};

typedef u16 lmac_msg_id_t;
typedef u16 lmac_task_id_t;

struct lmac_msg {
	lmac_msg_id_t id; ///< Message id.
	lmac_task_id_t dest_id; ///< Destination kernel identifier.
	lmac_task_id_t src_id; ///< Source kernel identifier.
	u16 param_len; ///< Parameter embedded struct length.
	u32 param[]; ///< Parameter embedded struct. Must be word-aligned.
};

#define rwnx_cmd_e2amsg ipc_e2a_msg
#define rwnx_cmd_a2emsg lmac_msg
#define RWNX_CMD_A2EMSG_LEN(m) (sizeof(struct lmac_msg) + m->param_len)
#define RWNX_CMD_E2AMSG_LEN_MAX (IPC_E2A_MSG_PARAM_SIZE * 4)

static inline void put_u16(u8 *buf, u16 data)
{
	buf[0] = (u8)(data & 0x00ff);
	buf[1] = (u8)((data >> 8) & 0x00ff);
}

enum rwnx_cmd_mgr_state {
	RWNX_CMD_MGR_STATE_DEINIT,
	RWNX_CMD_MGR_STATE_INITED,
	RWNX_CMD_MGR_STATE_CRASHED,
};

struct rwnx_cmd {
	struct list_head list;
	lmac_msg_id_t id;
	lmac_msg_id_t reqid;
	struct rwnx_cmd_a2emsg *a2e_msg;
	char *e2a_msg;
	u32 tkn;
	u16 flags;
	struct completion complete;
	u32 result;
};

struct aic_sdio_dev;
struct rwnx_cmd;
typedef int (*msg_cb_fct)(struct rwnx_cmd *cmd, struct rwnx_cmd_e2amsg *msg);

struct rwnx_cmd_mgr {
	enum rwnx_cmd_mgr_state state;
	spinlock_t lock;
	u32 next_tkn;
	u32 queue_sz;
	u32 max_queue_sz;
	spinlock_t cb_lock;
	void *sdiodev;

	struct list_head cmds;

	int (*queue)(struct rwnx_cmd_mgr *, struct rwnx_cmd *);
	int (*llind)(struct rwnx_cmd_mgr *, struct rwnx_cmd *);
	int (*msgind)(struct rwnx_cmd_mgr *, struct rwnx_cmd_e2amsg *,
		      msg_cb_fct);
	void (*print)(struct rwnx_cmd_mgr *);
	void (*drain)(struct rwnx_cmd_mgr *);

	struct work_struct cmdWork;
	struct workqueue_struct *cmd_wq;
};

void rwnx_cmd_mgr_init(struct rwnx_cmd_mgr *cmd_mgr);
void rwnx_cmd_mgr_deinit(struct rwnx_cmd_mgr *cmd_mgr);
int cmd_mgr_queue_force_defer(struct rwnx_cmd_mgr *cmd_mgr,
			      struct rwnx_cmd *cmd);
void rwnx_set_cmd_tx(void *dev, struct lmac_msg *msg, uint len);

enum {
	TASK_NONE = (u8)-1,

	// MAC Management task.
	TASK_MM = 0,
	// DEBUG task
	TASK_DBG,
	/// SCAN task
	TASK_SCAN,
	/// TDLS task
	TASK_TDLS,
	/// SCANU task
	TASK_SCANU,
	/// ME task
	TASK_ME,
	/// SM task
	TASK_SM,
	/// APM task
	TASK_APM,
	/// BAM task
	TASK_BAM,
	/// MESH task
	TASK_MESH,
	/// RXU task
	TASK_RXU,
	// This is used to define the last task that is running on the EMB processor
	TASK_LAST_EMB = TASK_RXU,

	// nX API task
	TASK_API,
	TASK_MAX,
};

#define LMAC_FIRST_MSG(task) ((lmac_msg_id_t)((task) << 10))
#define DRV_TASK_ID 100
#define MSG_I(msg) ((msg) & ((1 << 10) - 1))
#define MSG_T(msg) ((lmac_task_id_t)((msg) >> 10))

enum dbg_msg_tag {
	/// Memory read request
	DBG_MEM_READ_REQ = LMAC_FIRST_MSG(TASK_DBG),
	/// Memory read confirm
	DBG_MEM_READ_CFM,
	/// Memory write request
	DBG_MEM_WRITE_REQ,
	/// Memory write confirm
	DBG_MEM_WRITE_CFM,
	/// Module filter request
	DBG_SET_MOD_FILTER_REQ,
	/// Module filter confirm
	DBG_SET_MOD_FILTER_CFM,
	/// Severity filter request
	DBG_SET_SEV_FILTER_REQ,
	/// Severity filter confirm
	DBG_SET_SEV_FILTER_CFM,
	/// LMAC/MAC HW fatal error indication
	DBG_ERROR_IND,
	/// Request to get system statistics
	DBG_GET_SYS_STAT_REQ,
	/// COnfirmation of system statistics
	DBG_GET_SYS_STAT_CFM,
	/// Memory block write request
	DBG_MEM_BLOCK_WRITE_REQ,
	/// Memory block write confirm
	DBG_MEM_BLOCK_WRITE_CFM,
	/// Start app request
	DBG_START_APP_REQ,
	/// Start app confirm
	DBG_START_APP_CFM,
	/// Start npc request
	DBG_START_NPC_REQ,
	/// Start npc confirm
	DBG_START_NPC_CFM,
	/// Memory mask write request
	DBG_MEM_MASK_WRITE_REQ,
	/// Memory mask write confirm
	DBG_MEM_MASK_WRITE_CFM,

	DBG_RFTEST_CMD_REQ,
	DBG_RFTEST_CMD_CFM,
	DBG_BINDING_REQ,
	DBG_BINDING_CFM,
	DBG_BINDING_IND,

	DBG_CUSTOM_MSG_REQ,
	DBG_CUSTOM_MSG_CFM,
	DBG_CUSTOM_MSG_IND,

	DBG_GPIO_WRITE_REQ,
	DBG_GPIO_WRITE_CFM,

	/// Max number of Debug messages
	DBG_MAX,
};

#if !defined(CONFIG_M2D_OTA_LZMA_SUPPORT)
#define FW_M2D_OTA_NAME "m2d_ota.bin"
#else
#define FW_M2D_OTA_NAME "m2d_ota_lzma.bin"
#endif

enum {
	HOST_START_APP_AUTO = 1,
	HOST_START_APP_CUSTOM,
	HOST_START_APP_FNCALL = 4,
	HOST_START_APP_DUMMY = 5,
};

struct dbg_mem_block_write_req {
	u32 memaddr;
	u32 memsize;
	u32 memdata[1024 / sizeof(u32)];
};

/// Structure containing the parameters of the @ref DBG_MEM_BLOCK_WRITE_CFM message.
struct dbg_mem_block_write_cfm {
	u32 wstatus;
};

/// Structure containing the parameters of the @ref DBG_MEM_WRITE_REQ message.
struct dbg_mem_write_req {
	u32 memaddr;
	u32 memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_WRITE_CFM message.
struct dbg_mem_write_cfm {
	u32 memaddr;
	u32 memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_READ_REQ message.
struct dbg_mem_read_req {
	u32 memaddr;
};

/// Structure containing the parameters of the @ref DBG_MEM_READ_CFM message.
struct dbg_mem_read_cfm {
	u32 memaddr;
	u32 memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_MASK_WRITE_REQ message.
struct dbg_mem_mask_write_req {
	u32 memaddr;
	u32 memmask;
	u32 memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_MASK_WRITE_CFM message.
struct dbg_mem_mask_write_cfm {
	u32 memaddr;
	u32 memdata;
};

/// Structure containing the parameters of the @ref DBG_START_APP_REQ message.
struct dbg_start_app_req {
	u32 bootaddr;
	u32 boottype;
};

/// Structure containing the parameters of the @ref DBG_START_APP_CFM message.
struct dbg_start_app_cfm {
	u32 bootstatus;
};

int aicwf_plat_patch_load_8800dc(struct aic_sdio_dev *sdiodev);
int aicwf_plat_rftest_load_8800dc(struct aic_sdio_dev *sdiodev);
#if defined(CONFIG_DPD) || defined(CONFIG_LOFT_CALIB)
int aicwf_misc_ram_valid_check_8800dc(struct aic_sdio_dev *sdiodev,
				      int *valid_out);
int aicwf_plat_calib_load_8800dc(struct aic_sdio_dev *sdiodev);
#endif

int rwnx_load_firmware(u32 **fw_buf, const char *name, struct device *device);
int aicwf_patch_table_load(struct aic_sdio_dev *rwnx_hw, char *filename);

int rwnx_send_dbg_mem_read_req(struct aic_sdio_dev *sdiodev, u32 mem_addr,
			       struct dbg_mem_read_cfm *cfm);
int rwnx_send_dbg_mem_block_write_req(struct aic_sdio_dev *sdiodev,
				      u32 mem_addr, u32 mem_size,
				      u32 *mem_data);
int rwnx_send_dbg_mem_write_req(struct aic_sdio_dev *sdiodev, u32 mem_addr,
				u32 mem_data);
int rwnx_send_dbg_mem_mask_write_req(struct aic_sdio_dev *sdiodev, u32 mem_addr,
				     u32 mem_mask, u32 mem_data);
int rwnx_send_dbg_start_app_req(struct aic_sdio_dev *sdiodev, u32 boot_addr,
				u32 boot_type,
				struct dbg_start_app_cfm *start_app_cfm);

int rwnx_plat_bin_fw_upload_android(struct aic_sdio_dev *sdiodev, u32 fw_addr,
				    const char *filename);

void rwnx_rx_handle_msg(struct aic_sdio_dev *sdiodev, struct ipc_e2a_msg *msg);
int aicbsp_platform_init(struct aic_sdio_dev *sdiodev);
void aicbsp_platform_deinit(struct aic_sdio_dev *sdiodev);
int aicbsp_driver_fw_init(struct aic_sdio_dev *sdiodev);
#if (defined(CONFIG_DPD) && !defined(CONFIG_FORCE_DPD_CALIB))
int is_file_exist(char *name);
#endif
int aicbsp_resv_mem_init(void);
int aicbsp_resv_mem_deinit(void);

#define AICBSP_FW_PATH CONFIG_AIC_FW_PATH
#define AICBSP_FW_PATH_MAX 200

#define RAM_FMAC_FW_ADDR 0x00120000
#define RAM_FMAC_FW_PATCH_ADDR 0x00190000
#define FW_RAM_ADID_BASE_ADDR 0x00161928
#define FW_RAM_ADID_BASE_ADDR_U03 0x00161928
#define FW_RAM_PATCH_BASE_ADDR 0x00100000
#define RAM_8800DC_U01_ADID_ADDR 0x00101788
#define RAM_8800DC_U02_ADID_ADDR 0x001017d8
#define RAM_8800DC_FW_PATCH_ADDR 0x00184000
#define FW_RESET_START_ADDR 0x40500128
#define FW_RESET_START_VAL 0x40
#define FW_ADID_FLAG_ADDR 0x40500150
#define FW_ADID_FLAG_VAL 0x01
#define FW_RAM_ADID_BASE_ADDR_8800D80 0x002017E0
#define FW_RAM_PATCH_BASE_ADDR_8800D80 0x0020B2B0
#define FW_RAM_ADID_BASE_ADDR_8800D80_U02 0x00201940
#define FW_RAM_PATCH_BASE_ADDR_8800D80_U02 0x0020b43c

#define AICBT_PT_TAG "AICBT_PT_TAG"

/*****************************************************************************
 * Addresses within RWNX_ADDR_CPU
 *****************************************************************************/
#define RAM_LMAC_FW_ADDR 0x00150000

#define ROM_FMAC_FW_ADDR 0x00010000
#define ROM_FMAC_PATCH_ADDR 0x00180000

#define RAM_FMAC_FW_PATCH_NAME "fmacfw_patch.bin"
#define RWNX_MAC_CALIB_BASE_NAME_8800DC "fmacfw_calib_8800dc"
#define RWNX_MAC_CALIB_NAME_8800DC_U02                                         \
	RWNX_MAC_CALIB_BASE_NAME_8800DC "_u02.bin"
#ifdef CONFIG_SDIO_BT
#define RWNX_MAC_CALIB_NAME_8800DC_H_U02                                       \
	RWNX_MAC_CALIB_BASE_NAME_8800DC "_hbt_u02.bin"
#else
#define RWNX_MAC_CALIB_NAME_8800DC_H_U02                                       \
	RWNX_MAC_CALIB_BASE_NAME_8800DC "_h_u02.bin"
#endif

#if defined(CONFIG_DPD) || defined(CONFIG_LOFT_CALIB)
#define ROM_FMAC_CALIB_ADDR 0x00130000
#endif
#ifdef CONFIG_DPD
#ifndef CONFIG_FORCE_DPD_CALIB
#define FW_DPDRESULT_NAME_8800DC "aic_dpdresult_lite_8800dc.bin"
#endif
#endif

#define RWNX_MAC_FW_RF_BASE_NAME_8800DC "lmacfw_rf_8800dc.bin"

#ifdef CONFIG_FOR_IPCOM
#define RWNX_MAC_PATCH_BASE_NAME_8800DC "fmacfw_patch_8800dc_ipc"
#define RWNX_MAC_PATCH_NAME2_8800DC RWNX_MAC_PATCH_BASE_NAME_8800DC ".bin"
#else
#define RWNX_MAC_PATCH_BASE_NAME_8800DC "fmacfw_patch_8800dc"
#define RWNX_MAC_PATCH_NAME2_8800DC RWNX_MAC_PATCH_BASE_NAME_8800DC ".bin"
#define RWNX_MAC_PATCH_NAME2_8800DC_U02                                        \
	RWNX_MAC_PATCH_BASE_NAME_8800DC "_u02.bin"
#ifdef CONFIG_SDIO_BT
#define RWNX_MAC_PATCH_NAME2_8800DC_H_U02                                      \
	RWNX_MAC_PATCH_BASE_NAME_8800DC "_hbt_u02.bin"
#else
#define RWNX_MAC_PATCH_NAME2_8800DC_H_U02                                      \
	RWNX_MAC_PATCH_BASE_NAME_8800DC "_h_u02.bin"
#endif
#endif

#define RWNX_MAC_PATCH_TABLE_NAME_8800DC "fmacfw_patch_tbl_8800dc"
#define RWNX_MAC_PATCH_TABLE_8800DC RWNX_MAC_PATCH_TABLE_NAME_8800DC ".bin"
#define RWNX_MAC_PATCH_TABLE_8800DC_U02                                        \
	RWNX_MAC_PATCH_TABLE_NAME_8800DC "_u02.bin"
#ifdef CONFIG_SDIO_BT
#define RWNX_MAC_PATCH_TABLE_8800DC_H_U02                                      \
	RWNX_MAC_PATCH_TABLE_NAME_8800DC "_hbt_u02.bin"
#else
#define RWNX_MAC_PATCH_TABLE_8800DC_H_U02                                      \
	RWNX_MAC_PATCH_TABLE_NAME_8800DC "_h_u02.bin"
#endif

#define RWNX_MAC_RF_PATCH_BASE_NAME_8800DC "fmacfw_rf_patch_8800dc"
#define RWNX_MAC_RF_PATCH_NAME_8800DC RWNX_MAC_RF_PATCH_BASE_NAME_8800DC ".bin"
#define FW_USERCONFIG_NAME_8800DC "aic_userconfig_8800dc.txt"

enum {
	FW_NORMAL_MODE = 0,
	FW_RFTEST_MODE = 1,
	FW_BLE_SCAN_WAKEUP_MODE = 2,
	FW_M2D_OTA_MODE = 3,
	FW_DPDCALIB_MODE = 4,
	FW_BLE_SCAN_AD_FILTER_MODE = 5,
};

enum aicbt_patch_table_type {
	AICBT_PT_INF = 0x00,
	AICBT_PT_TRAP = 0x1,
	AICBT_PT_B4,
	AICBT_PT_BTMODE,
	AICBT_PT_PWRON,
	AICBT_PT_AF,
	AICBT_PT_VER,
};

enum aicbt_btport_type {
	AICBT_BTPORT_NULL,
	AICBT_BTPORT_MB,
	AICBT_BTPORT_UART,
};

/*  btmode
 * used for force bt mode,if not AICBSP_MODE_NULL
 * efuse valid and vendor_info will be invalid, even has beed set valid
*/
enum aicbt_btmode_type {
	AICBT_BTMODE_BT_ONLY_SW = 0x0, // bt only mode with switch
	AICBT_BTMODE_BT_WIFI_COMBO, // wifi/bt combo mode
	AICBT_BTMODE_BT_ONLY, // bt only mode without switch
	AICBT_BTMODE_BT_ONLY_TEST, // bt only test mode
	AICBT_BTMODE_BT_WIFI_COMBO_TEST, // wifi/bt combo test mode
	AICBT_BTMODE_BT_ONLY_COANT, // bt only mode with no external switch
	AICBT_MODE_NULL = 0xFF, // invalid value
};

/*  uart_baud
 * used for config uart baud when btport set to uart,
 * otherwise meaningless
*/
enum aicbt_uart_baud_type {
	AICBT_UART_BAUD_115200 = 115200,
	AICBT_UART_BAUD_921600 = 921600,
	AICBT_UART_BAUD_1_5M = 1500000,
	AICBT_UART_BAUD_3_25M = 3250000,
};

enum aicbt_uart_flowctrl_type {
	AICBT_UART_FLOWCTRL_DISABLE = 0x0, // uart without flow ctrl
	AICBT_UART_FLOWCTRL_ENABLE, // uart with flow ctrl
};

enum aicbsp_cpmode_type {
	AICBSP_CPMODE_WORK,
	AICBSP_CPMODE_TEST,
	AICBSP_CPMODE_MAX,
};

enum chip_rev {
	CHIP_REV_U01 = 1,
	CHIP_REV_U02 = 3,
	CHIP_REV_U03 = 7,
	CHIP_REV_U04 = 7,
};

#define AIC_M2D_OTA_INFO_ADDR 0x88000020
#define AIC_M2D_OTA_DATA_ADDR 0x88000040
#if !defined(CONFIG_M2D_OTA_LZMA_SUPPORT)
#define AIC_M2D_OTA_FLASH_ADDR 0x08004000
#define AIC_M2D_OTA_CODE_START_ADDR (AIC_M2D_OTA_FLASH_ADDR + 0x0188)
#define AIC_M2D_OTA_VER_ADDR (AIC_M2D_OTA_FLASH_ADDR + 0x018C)
#else
#define AIC_M2D_OTA_FLASH_ADDR 0x08005000
#define AIC_M2D_OTA_CODE_START_ADDR (AIC_M2D_OTA_FLASH_ADDR + 0x1188)
#define AIC_M2D_OTA_VER_ADDR (AIC_M2D_OTA_FLASH_ADDR + 0x0010)
#endif
///aic bt tx pwr lvl :lsb->msb: first byte, min pwr lvl; second byte, max pwr lvl;
///pwr lvl:20(min), 30 , 40 , 50 , 60(max)
#define AICBT_TXPWR_LVL 0x00006020
#define AICBT_TXPWR_LVL_8800dc 0x00006f2f
#define AICBT_TXPWR_LVL_8800d80 0x00006f2f
#define AICBT_TXPWR_LVL_8800d80x2 0x00006f2f

#define AICBSP_HWINFO_DEFAULT (-1)
#define AICBSP_CPMODE_DEFAULT AICBSP_CPMODE_WORK
#define AICBSP_FWLOG_EN_DEFAULT 0

#define AICBT_BTMODE_DEFAULT_8800d80x2 AICBT_BTMODE_BT_ONLY_COANT
#define AICBT_BTMODE_DEFAULT_8800d80 AICBT_BTMODE_BT_ONLY_COANT
#define AICBT_BTMODE_DEFAULT AICBT_BTMODE_BT_ONLY_SW
#ifdef CONFIG_SDIO_BT
#define AICBT_BTPORT_DEFAULT AICBT_BTPORT_MB
#else
#define AICBT_BTPORT_DEFAULT AICBT_BTPORT_UART
#endif
#define AICBT_UART_BAUD_DEFAULT AICBT_UART_BAUD_1_5M
#define AICBT_UART_FC_DEFAULT AICBT_UART_FLOWCTRL_ENABLE
#define AICBT_LPM_ENABLE_DEFAULT 0
#define AICBT_TXPWR_LVL_DEFAULT AICBT_TXPWR_LVL
#define AICBT_TXPWR_LVL_DEFAULT_8800dc AICBT_TXPWR_LVL_8800dc
#define AICBT_TXPWR_LVL_DEFAULT_8800d80 AICBT_TXPWR_LVL_8800d80
#define AICBT_TXPWR_LVL_DEFAULT_8800d80x2 AICBT_TXPWR_LVL_8800d80x2

#define FEATURE_SDIO_CLOCK 50000000 // 0: default, other: target clock rate
#define FEATURE_SDIO_CLOCK_V3 150000000 // 0: default, other: target clock rate
#define FEATURE_SDIO_PHASE 2 // 0: default, 2: 180°

struct aicbt_patch_table {
	char *name;
	uint32_t type;
	uint32_t *data;
	uint32_t len;
	struct aicbt_patch_table *next;
};

struct aicbt_info_t {
	uint32_t btmode;
	uint32_t btport;
	uint32_t uart_baud;
	uint32_t uart_flowctrl;
	uint32_t lpm_enable;
	uint32_t txpwr_lvl;
};

struct aicbt_patch_info_t {
	uint32_t info_len;
	//base len start
	uint32_t adid_addrinf;
	uint32_t addr_adid;
	uint32_t patch_addrinf;
	uint32_t addr_patch;
	uint32_t reset_addr;
	uint32_t reset_val;
	uint32_t adid_flag_addr;
	uint32_t adid_flag;
	//base len end
	//ext patch nb
	uint32_t ext_patch_nb_addr;
	uint32_t ext_patch_nb;
	uint32_t *ext_patch_param;
};

struct aicbsp_firmware {
	const char *desc;
	const char *bt_adid;
	const char *bt_patch;
	const char *bt_table;
	const char *wl_fw;
	const char *bt_ext_patch;
};

struct aicbsp_info_t {
	int hwinfo;
	int hwinfo_r;
	uint32_t cpmode;
	uint32_t chip_rev;
	bool fwlog_en;
	uint8_t irqf;
};

extern struct aicbsp_info_t aicbsp_info;
extern struct mutex aicbsp_power_lock;
extern const struct aicbsp_firmware *aicbsp_firmware_list;
extern const struct aicbsp_firmware fw_u02[];
extern const struct aicbsp_firmware fw_u03[];
extern const struct aicbsp_firmware fw_8800dc_u01[];
extern const struct aicbsp_firmware fw_8800dc_u02[];
extern const struct aicbsp_firmware fw_8800dc_h_u02[];
extern const struct aicbsp_firmware fw_8800d80_u01[];
extern const struct aicbsp_firmware fw_8800d80_u02[];
extern const struct aicbsp_firmware fw_8800d80_h_u02[];
extern const struct aicbsp_firmware fw_8800d80x2[];

#endif
