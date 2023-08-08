/*============================================================================
*                                                                            *
* Copyright (C) by Tuya Inc                                                  *
* All rights reserved                                                        *
*                                                                            *
=============================================================================*/

#ifndef ___PROD_TEST_H__
#define ___PROD_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============================ INCLUDES ======================================*/
#include "mf_test.h"
#include "stdint.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
	char ssid[32 + 1];
	signed char rssi;
} prodtest_ssid_info_t;

typedef OPERATE_RET (*prodtest_app_cb_t)(int flag, prodtest_ssid_info_t *info, uint8_t info_count);

typedef struct {
	GW_WF_CFG_MTHD_SEL gwcm_mode;
	uint8_t ssid_count;
	const char **ssid_list;
	prodtest_app_cb_t app_cb;
	MF_USER_PRODUCT_TEST_CB product_cb;
	char *file_name;
	char *file_ver;
} prodtest_app_cfg_t;

typedef struct {
	MF_USER_PRODUCT_TEST_CB product_cb;
	char *file_name;
	char *file_ver;
} autotest_prod_cfg_t;

/*============================ PROTOTYPES ====================================*/
OPERATE_RET autotest_prod_start(autotest_prod_cfg_t *cfg);

int prodtest_identify_listen_start(char *app_name, char *app_version);
int prodtest_identify_listen_stop(void);
bool prodtest_ssid_scan(uint32_t timeout);
int prodtest_app_register(const prodtest_app_cfg_t *app_cfg);

#ifdef __cplusplus
} // extern "C"
#endif

#endif