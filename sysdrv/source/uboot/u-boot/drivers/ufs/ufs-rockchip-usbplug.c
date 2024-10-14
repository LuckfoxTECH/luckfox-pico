// SPDX-License-Identifier: GPL-2.0+
/*
 * Rockchip UFS Host Controller driver
 *
 * Copyright (C) 2024 Rockchip Electronics Co.Ltd.
 */

#include <charset.h>
#include <common.h>
#include <dm.h>
#include <log.h>
#include <dm/lists.h>
#include <dm/device-internal.h>
#include <malloc.h>
#include <hexdump.h>
#include <scsi.h>
#include <asm/io.h>
#include <asm/dma-mapping.h>
#include <linux/bitops.h>
#include <linux/delay.h>

#include "ufs-rockchip-usbplug.h"
#include "ufs.h"
/* Query request retries */
#define QUERY_REQ_RETRIES 3
/* Query request timeout */
#define QUERY_REQ_TIMEOUT 1500 /* 1.5 seconds */

#if defined(CONFIG_SUPPORT_USBPLUG)
int _ufs_start(struct ufs_hba *hba);

static void ufs_info_show_dev_desc(void *buf)
{
	struct ufs_device_descriptor *dev = (struct ufs_device_descriptor *)buf;

	printf("---------------------------\n");
	printf("---UFS Device Descriptor---\n");
	printf("---------------------------\n");
	printf("bLength: 0x%x\n", dev->b_length);
	printf("bDescriptorIDN: 0x%x\n", dev->b_descriptor_idn);
	printf("bDevice: 0x%x\n", dev->b_device);
	printf("bDeviceClass: 0x%x\n", dev->b_device_class);
	printf("bDeviceSubClass: 0x%x\n", dev->b_device_sub_class);
	printf("bProtocol: 0x%x\n", dev->b_protocol);
	printf("bNumberLU: 0x%x\n", dev->b_number_lu);
	printf("bNumberWLU: 0x%x\n", dev->b_number_wlu);
	printf("bBootEnable: 0x%x\n", dev->b_boot_enable);
	printf("bDescrAccessEn: 0x%x\n", dev->b_descr_access_en);
	printf("bInitPowerMode: 0x%x\n", dev->b_init_power_mode);
	printf("bHighPriorityLUN: 0x%x\n", dev->b_high_priority_lun);
	printf("bSecureRemovalType: 0x%x\n", dev->b_secure_removal_type);
	printf("bSecurityLU: 0x%x\n", dev->b_security_lu);
	printf("bBackgroundOpsTermLat: 0x%x\n", dev->b_background_ops_term_lat);
	printf("bInitActiveICCLevel: 0x%x\n", dev->b_init_active_icc_level);
	printf("wSpecVersion: 0x%x\n", to_bigendian16(dev->w_spec_version));
	printf("wManufactureDate: 0x%x\n", to_bigendian16(dev->w_manufacture_date));
	printf("iManufacturerName: 0x%x\n", dev->i_manufacturer_name);
	printf("iProductName: 0x%x\n", dev->i_product_name);
	printf("iSerialNumber: 0x%x\n", dev->i_serial_number);
	printf("iOemID: 0x%x\n", dev->i_oem_id);
	printf("wManufacturerID: 0x%x\n", to_bigendian16(dev->w_manufacturer_id));
	printf("bUD0BaseOffset: 0x%x\n", dev->b_ud_0base_offset);
	printf("bUDConfigPLength: 0x%x\n", dev->b_ud_config_plength);
	printf("bDeviceRTTCap: 0x%x\n", dev->b_device_rtt_cap);
	printf("wPeriodicRTCUpdate: 0x%x\n", to_bigendian16(dev->w_periodic_rtc_update));
	printf("bUFSFeatureSupport: 0x%x\n", dev->b_ufs_feature_support);
	printf("bFFUTimeout: 0x%x\n", dev->b_ffu_timeout);
	printf("bQueueDepth: 0x%x\n", dev->b_queue_depth);
	printf("wDeviceVersion: 0x%x\n", to_bigendian16(dev->w_device_version));
	printf("bNumSecureWPArea: 0x%x\n", dev->b_num_secure_wp_area);
	printf("dPSAMaxDataSize: 0x%x\n", to_bigendian32(dev->d_psa_max_data_size));
	printf("bPSAStateTimeout: 0x%x\n", dev->b_psa_state_timeout);
	printf("iProductRevisionLevel: 0x%x\n", dev->i_product_revision_level);
}

static void ufs_info_show_conf_desc(void *buf)
{
	struct ufs_configuration_descriptor *c_desc = (struct ufs_configuration_descriptor *)buf;
	struct ufs_dev_desc_configuration_param *dev;
	struct ufs_unit_desc_configuration_param *unit;
	int i;

	dev = &c_desc->dev_desc_conf_param;
	printf("----------------------------------------\n");
	printf("---UFS Device Descriptor Config Param---\n");
	printf("----------------------------------------\n");
	printf("bLength: 0x%x\n", dev->b_length);
	printf("bDescriptorIDN: 0x%x\n", dev->b_descriptor_idn);
	printf("bConfDescContinue: 0x%x\n", dev->b_conf_desc_continue);
	printf("bBootEnable: 0x%x\n", dev->b_boot_enable);
	printf("bDescrAccessEn: 0x%x\n", dev->b_descr_access_en);
	printf("bInitPowerMode: 0x%x\n", dev->b_init_power_mode);
	printf("bHighPriorityLUN: 0x%x\n", dev->b_high_priority_lun);
	printf("bSecureRemovalType: 0x%x\n", dev->b_secure_removal_type);
	printf("bInitActiveICCLevel: 0x%x\n", dev->b_init_active_icc_level);
	printf("wPeriodicRTCUpdate: 0x%x\n", to_bigendian16(dev->w_periodic_rtc_update));
	printf("bSecureRemovalType: 0x%x\n", dev->b_secure_removal_type);
	printf("bInitActiveICCLevel: 0x%x\n", dev->b_init_active_icc_level);
	printf("wPeriodicRTCUpdate: 0x%x\n", to_bigendian16(dev->w_periodic_rtc_update));
	printf("bWB_EN: 0x%x\n", dev->b_write_booster_buffer_reserve_user_space_en);
	printf("WB_TYPE: 0x%x\n", dev->b_write_booster_buffer_type);
	printf("WB_alloc_units: 0x%x\n", to_bigendian32(dev->d_num_shared_write_booster_buffer_alloc_units));

	for (i = 0; i < UNIT_DESCS_COUNT; i++) {
		unit = &c_desc->unit_desc_conf_param[i];

		printf("-----------------------------------------\n");
		printf("---UFS Unit %d Descriptor Config Param---\n", i);
		printf("-----------------------------------------\n");
		printf("bLUEnable: 0x%x\n", unit->b_lu_enable);
		printf("bBootLunID: 0x%x\n", unit->b_boot_lun_id);
		printf("bLUWriteProtect: 0x%x\n", unit->b_lu_write_protect);
		printf("bMemoryType: 0x%x\n", unit->b_memory_type);
		printf("dNumAllocUnits: 0x%x\n", to_bigendian32(unit->d_num_alloc_units));
		printf("bDataReliability: 0x%x\n", unit->b_data_reliability);
		printf("bLogicalBlockSize: 0x%x\n", unit->b_logical_block_size);
		printf("bProvisioningType: 0x%x\n", unit->b_provisioning_type);
		printf("wContextCapabilities: 0x%x\n", to_bigendian16(unit->w_context_capabilities));
	}
}

static int ufs_get_configuration_desc(struct ufs_hba *hba, struct ufs_configuration_descriptor *c_desc)
{
	u8 desc_buf[CONFIGURATION_DESC_V22_LENGTH];
	u8 *buf = desc_buf;
	int length = CONFIGURATION_DESC_V22_LENGTH;
	int err;

	if (CONFIGURATION_DESC_V31_LENGTH == hba->desc_size.conf_desc) {
		buf = (u8 *)c_desc;
		length = CONFIGURATION_DESC_V31_LENGTH;
	} else if(CONFIGURATION_DESC_V22_LENGTH != hba->desc_size.conf_desc) {
		return -EINVAL;
	}

	err = ufshcd_read_desc_param(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 0, buf, length);
	if (err) {
		dev_err(hba->dev, "%s: Failed reading configuration Desc. err = %d\n",
			__func__, err);
		return err;
	}

	if (CONFIGURATION_DESC_V22_LENGTH == hba->desc_size.conf_desc) {
		memcpy(&c_desc->dev_desc_conf_param, buf, 0x10);
		buf += 0x10;
		for (int i = 0; i < UNIT_DESCS_COUNT; i++) {
			memcpy(&c_desc->unit_desc_conf_param[i], buf,  0x10);
			buf += 0x10;
		}
	}

	return err;
}

static int ufshcd_write_desc_param(struct ufs_hba *hba, enum desc_idn desc_id,
			   int desc_index, u8 param_offset, u8 *param_read_buf,
			   u8 param_size)
{
	int ret;
	u8 *desc_buf;
	int buff_len;

	/* Safety check */
	if (desc_id >= QUERY_DESC_IDN_MAX || !param_size)
		return -EINVAL;

	/* Get the max length of descriptor from structure filled up at probe
	 * time.
	 */
	ret = ufshcd_map_desc_id_to_length(hba, desc_id, &buff_len);

	/* Sanity checks */
	if (ret || !buff_len) {
		dev_err(hba->dev, "%s: Failed to get full descriptor length\n",
			__func__);
		return ret;
	}

	desc_buf = param_read_buf;
	/* Request for full descriptor */
	ret = ufshcd_query_descriptor_retry(hba, UPIU_QUERY_OPCODE_WRITE_DESC,
					    desc_id, desc_index, 0, desc_buf,
					    &buff_len);

	if (ret)
		dev_err(hba->dev, "%s: Failed write descriptor. desc_id %d, desc_index %d, param_offset %d, ret %d\n",
			__func__, desc_id, desc_index, param_offset, ret);

	return ret;
}

static int ufs_write_configuration_desc(struct ufs_hba *hba, struct ufs_configuration_descriptor *c_desc)
{
	u8 desc_buf[CONFIGURATION_DESC_V22_LENGTH];
	u8 *buf = desc_buf;
	int length = CONFIGURATION_DESC_V22_LENGTH;
	int err;

	if (CONFIGURATION_DESC_V31_LENGTH == hba->desc_size.conf_desc) {
		buf = (u8 *)c_desc;
		length = CONFIGURATION_DESC_V31_LENGTH;
	} else if(CONFIGURATION_DESC_V22_LENGTH == hba->desc_size.conf_desc) {
		memcpy(buf, &c_desc->dev_desc_conf_param, 0x10);
		buf += 0x10;
		for (int i = 0; i < UNIT_DESCS_COUNT; i++) {
			memcpy(buf, &c_desc->unit_desc_conf_param[i], 0x10);
			buf += 0x10;
		}
		buf = desc_buf;
	} else {
		return -EINVAL;
	}

	err = ufshcd_write_desc_param(hba, QUERY_DESC_IDN_CONFIGURATION, 0, 0, buf, length);
	if (err) {
		dev_err(hba->dev, "%s: Failed reading configuration Desc. err = %d\n",
			__func__, err);
		return err;
	}

	return err;
}

static void ufs_lu_configuration(struct ufs_hba *hba, struct ufs_configuration_descriptor *c_desc)
{
	uint32_t denominator = hba->geo_desc->b_allocation_unit_size * to_bigendian32(hba->geo_desc->d_segment_size);
	struct ufs_dev_desc_configuration_param *dev;
	struct ufs_unit_desc_configuration_param *unit;
	uint32_t alloced_units = 0;
	int i, cap_adj_fac;
	uint64_t total_raw_device_capacity;
	uint32_t max_wb_alloc_units = cpu_to_be32(hba->geo_desc->d_write_booster_buffer_max_alloc_units);
	uint32_t wb_alloc_units;

	cap_adj_fac = to_bigendian16(hba->geo_desc->w_enhanced1_cap_adj_fac) / 256;
	total_raw_device_capacity = cpu_to_be64(hba->geo_desc->q_total_raw_device_capacity);

	dev = &c_desc->dev_desc_conf_param;
	dev->b_boot_enable = 0x1;
	dev->b_descr_access_en = 0x0;
	dev->b_init_power_mode = 0x1;
	dev->b_high_priority_lun = 0x7F;
	dev->b_secure_removal_type = 0x0;
	dev->b_init_active_icc_level = 0x0;
	dev->w_periodic_rtc_update = 0x0;

	unit = &c_desc->unit_desc_conf_param[0];
	/* lu 1: boot lu A 4MB */
	unit[1].b_boot_lun_id = WELL_BOOT_LU_A; /* lu 0, boot a */
	unit[1].b_memory_type = 0x3;
	unit[1].d_num_alloc_units = (4 * 0x800 * cap_adj_fac + denominator - 1) / denominator;
	alloced_units += unit[1].d_num_alloc_units;
	/* lu 2: boot lu B 4MB */
	unit[2].b_boot_lun_id = WELL_BOOT_LU_B; /* lu 1, boot b */
	unit[2].b_memory_type = 0x3; /* lu 0, Enhanced Memory */
	unit[2].d_num_alloc_units = (4 * 0x800 * cap_adj_fac + denominator - 1) / denominator;
	alloced_units += unit[2].d_num_alloc_units;
	/* lu 3: data lu 8MB */
	unit[3].b_boot_lun_id = 0x0; /* lu 2 */
	unit[3].b_memory_type = 0x3; /* lu 2, Enhanced Memory */
	unit[3].d_num_alloc_units = (8 * 0x800 * cap_adj_fac + denominator - 1) / denominator;
	alloced_units += unit[3].d_num_alloc_units;

	if (max_wb_alloc_units) {
		wb_alloc_units = max_wb_alloc_units;
		if (wb_alloc_units > max_wb_alloc_units)
			wb_alloc_units = max_wb_alloc_units;
		dev->b_write_booster_buffer_reserve_user_space_en = 1;
		dev->b_write_booster_buffer_type = 1;
		dev->d_num_shared_write_booster_buffer_alloc_units = to_bigendian32(wb_alloc_units);
	}

	/* lu 0: data lu, max capacity*/
	unit[0].b_boot_lun_id = 0x0; /* lu 3 */
	unit[0].b_memory_type = 0x0; /* lu 3, Normal Memory */
	unit[0].d_num_alloc_units = lower_32_bits(total_raw_device_capacity) / denominator - alloced_units;

	for (i = 0; i <= 3; i++) { /* lu 0 - 3 */
		unit[i].b_lu_enable = 0x1;
		unit[i].b_lu_write_protect = 0x0;
		unit[i].b_data_reliability = 0x1;
		unit[i].b_logical_block_size = 0x0c;
		unit[i].b_provisioning_type = 0x2;
		unit[i].w_context_capabilities = 0x0;
		unit[i].d_num_alloc_units = to_bigendian32(unit[i].d_num_alloc_units);
	}
}

static int compair_conf_desp(struct ufs_configuration_descriptor *cda, struct ufs_configuration_descriptor *cdb)
{
	struct ufs_dev_desc_configuration_param *dev_a, *dev_b;
	struct ufs_unit_desc_configuration_param *unit_a, *unit_b;
	int i, ret;

	dev_a = &cda->dev_desc_conf_param;
	dev_b = &cdb->dev_desc_conf_param;

	if (dev_a->b_boot_enable != dev_b->b_boot_enable)
		return 0x3;
	if (dev_a->b_descr_access_en != dev_b->b_descr_access_en)
		return 0x4;
	if (dev_a->b_init_power_mode != dev_b->b_init_power_mode)
		return 0x5;
	if (dev_a->b_high_priority_lun != dev_b->b_high_priority_lun)
		return 0x6;
	if (dev_a->b_secure_removal_type != dev_b->b_secure_removal_type)
		return 0x7;
	if (dev_a->b_init_active_icc_level != dev_b->b_init_active_icc_level)
		return 0x8;
	if (dev_a->w_periodic_rtc_update != dev_b->w_periodic_rtc_update)
		return 0x9;
	if (dev_a->b_write_booster_buffer_reserve_user_space_en !=
	    dev_b->b_write_booster_buffer_reserve_user_space_en)
		return 0xA;
	if (dev_a->b_write_booster_buffer_type != dev_b->b_write_booster_buffer_type)
		return 0xB;
	if (dev_a->d_num_shared_write_booster_buffer_alloc_units !=
	    dev_b->d_num_shared_write_booster_buffer_alloc_units)
		return 0xC;

	for (i = 0; i < UNIT_DESCS_COUNT; i++) {
		unit_a = &cda->unit_desc_conf_param[i];
		unit_b = &cdb->unit_desc_conf_param[i];

		ret = 0x10 * (i + 1);
		if (unit_a->b_lu_enable != unit_b->b_lu_enable)
			return ret;
		if (unit_a->b_boot_lun_id != unit_b->b_boot_lun_id)
			return ret + 0x1;
		if (unit_a->b_lu_write_protect != unit_b->b_lu_write_protect)
			return ret + 0x2;
		if (unit_a->b_memory_type != unit_b->b_memory_type)
			return ret + 0x3;
		if (unit_a->d_num_alloc_units != unit_b->d_num_alloc_units)
			return ret + 0x4;
		if (unit_a->b_data_reliability != unit_b->b_data_reliability)
			return ret + 0x8;
		if (unit_a->b_logical_block_size != unit_b->b_logical_block_size)
			return ret + 0x9;
		if (unit_a->b_provisioning_type != unit_b->b_provisioning_type)
			return ret + 0xA;
		if (unit_a->w_context_capabilities != unit_b->w_context_capabilities)
			return ret + 0xB;
	}
	return 0;
}


/**
 * ufshcd_init_query() - init the query response and request parameters
 */
static inline void ufshcd_init_query(struct ufs_hba *hba,
				     struct ufs_query_req **request,
				     struct ufs_query_res **response,
				     enum query_opcode opcode,
				     u8 idn, u8 index, u8 selector)
{
	*request = &hba->dev_cmd.query.request;
	*response = &hba->dev_cmd.query.response;
	memset(*request, 0, sizeof(struct ufs_query_req));
	memset(*response, 0, sizeof(struct ufs_query_res));
	(*request)->upiu_req.opcode = opcode;
	(*request)->upiu_req.idn = idn;
	(*request)->upiu_req.index = index;
	(*request)->upiu_req.selector = selector;
}

/**
 * ufshcd_query_flag() - API function for sending flag query requests
 */
static int ufshcd_query_attribute(struct ufs_hba *hba,enum query_opcode opcode,
				  enum attr_id idn, u8 index, u8 selector, u32 *value)
{
	struct ufs_query_req *request = &hba->dev_cmd.query.request;
	struct ufs_query_res *response = &hba->dev_cmd.query.response;
	int err;
	int timeout = QUERY_REQ_TIMEOUT;

	memset(request, 0, sizeof(struct ufs_query_req));
	memset(response, 0, sizeof(struct ufs_query_res));
	request->upiu_req.opcode = opcode;
	request->upiu_req.idn = idn;
	request->upiu_req.index = 0;
	request->upiu_req.selector = 0;

	switch (opcode) {
	case UPIU_QUERY_OPCODE_WRITE_ATTR:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		request->upiu_req.value = be32_to_cpu(*value);
		break;
	case UPIU_QUERY_OPCODE_READ_ATTR:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		break;
	default:
		dev_err(hba->dev,
			"%s: Expected query flag opcode but got = %d\n",
			__func__, opcode);
		err = -EINVAL;
		goto out;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, timeout);

	if (err) {
		dev_err(hba->dev,
			"%s: Sending flag query for idn %d failed, err = %d\n",
			__func__, idn, err);
		goto out;
	}

	if (value)
		*value = be32_to_cpu(response->upiu_res.value);

out:
	return err;
}

static int ufshcd_query_attribute_retry(struct ufs_hba *hba, enum query_opcode opcode,
					enum attr_id idn, u8 index, u8 selector, u32 *value)
{
	int ret;
	int retries;

	for (retries = 0; retries < QUERY_REQ_RETRIES; retries++) {
		ret = ufshcd_query_attribute(hba, opcode, idn, index, selector, value);
		if (ret)
			dev_dbg(hba->dev,
				"%s: failed with error %d, retries %d\n",
				__func__, ret, retries);
		else
			break;
	}

	if (ret)
		dev_err(hba->dev,
			"%s: query attribute, opcode %d, idn %d, failed with error %d after %d retires\n",
			__func__, opcode, idn, ret, retries);
	return ret;
}

static int read_attribute(struct ufs_hba *hba, enum attr_id idn, u8 index, u8 selector, u32 *value)
{
	int ret;

	ret = ufshcd_query_attribute_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
					    idn, index, 0, value);
	return ret;
}

static int write_attribute(struct ufs_hba *hba, enum attr_id idn, u8 index, u8 selector, u32 *value)
{
	int ret;

	ret = ufshcd_query_attribute_retry(hba, UPIU_QUERY_OPCODE_WRITE_ATTR,
					    idn, index, 0, value);
	return ret;
}

static int set_boot_lu_enable(struct ufs_hba *hba)
{
	uint32_t value = 0;
	uint32_t target_value = DEFAULT_BOOT_LUN;
	int ret;

	ret = read_attribute(hba, B_BOOT_LUNEN, 0, 0, &value);
	if (ret) {
		printf("read bBootLunEn fail. ret = %d\n", ret);
		return ret;
	}

	if (value != 0)
		printf("UFS get boot W-LU-%c\n", (value == WELL_BOOT_LU_A) ? 'A' : 'B');

	if (value == target_value)
		return 0;

	/* set default boot from Boot LU A */
	value = target_value;
	ret = write_attribute(hba, B_BOOT_LUNEN, 0, 0, &value);
	if (ret) {
		printf("write bBootLunEn attribute fail. ret = %d\n", ret);
		return ret;
	}

	ret = read_attribute(hba, B_BOOT_LUNEN, 0, 0, &value);
	if (ret) {
		printf("read bBootLunEn fail. ret = %d\n", ret);
		return ret;
	}

	if (target_value == value)
		return 0;

	printf("UFS set boot W-LU(%c) Fail value = %x\n", (value == WELL_BOOT_LU_A) ? 'A' : 'B', value);
	return 0;
}

static int ufs_set_ref_clk(struct ufs_hba *hba)
{
	uint32_t value;
	int ret;
	uint32_t target_ref_clk;

	target_ref_clk = 1; /* 26 MHz */

	ret = read_attribute(hba, B_REFCLK_FREQ, 0, 0, &value);
	if (ret) {
		printf("read bRefClkFreq fail. ret = %d\n", ret);
		return ret;
	}

	printf("UFS get ref clock %d Mhz\n", (value == 1) ? 26 : 19);
	if (target_ref_clk == value)
		return 0;

	/* set default boot from Boot LU A */
	ret = write_attribute(hba, B_REFCLK_FREQ, 0, 0, &target_ref_clk);
	if (ret) {
		printf("write bRefClkFreq attribute fail. ret = %d\n", ret);
		return ret;
	}

	ret = read_attribute(hba, B_REFCLK_FREQ, 0, 0, &value);
	if (ret) {
		printf("read bRefClkFreq fail. ret = %d\n", ret);
		return ret;
	}

	if (target_ref_clk == value)
		return 0;

	printf("UFS set bRefClkFreq  26Mhz Fail\n");
	return -EINVAL;
}

int ufs_create_partition_inventory(struct ufs_hba *hba)
{
	int err, length;

	length = (int)sizeof(struct ufs_geometry_descriptor);
	if (length > hba->desc_size.geom_desc)
		length = hba->desc_size.geom_desc;
	err = ufshcd_read_desc_param(hba, QUERY_DESC_IDN_GEOMETRY, 0, 0, (u8 *)hba->geo_desc, length);
	if (err) {
		dev_err(hba->dev, "%s: Failed reading geometry Desc. err = %d\n", __func__, err);
		return err;
	}

	dev_err(hba->dev, "%s: WB_max_alloc_units = %x\n", __func__,
		hba->geo_desc->d_write_booster_buffer_max_alloc_units);

	err = ufs_get_configuration_desc(hba, hba->rc_desc);
	if (err) {
		dev_err(hba->dev, "%s: Failed getting conf info. err = %d\n", __func__, err);
		return err;
	}
	ufs_info_show_conf_desc(hba->rc_desc);

	memset(hba->wc_desc, 0, sizeof(struct ufs_configuration_descriptor));
	hba->wc_desc->dev_desc_conf_param.b_length = hba->rc_desc->dev_desc_conf_param.b_length;
	hba->wc_desc->dev_desc_conf_param.b_descriptor_idn = hba->rc_desc->dev_desc_conf_param.b_descriptor_idn;
	ufs_lu_configuration(hba, hba->wc_desc);
	ufs_info_show_conf_desc(hba->wc_desc);

	err = compair_conf_desp(hba->wc_desc, hba->rc_desc);
	printf("compair_conf_desp: 0x%x\n", err);

	if (!err)
		goto out;

	err = ufs_write_configuration_desc(hba, hba->wc_desc);
	if (err)
		dev_err(hba->dev, "%s: Failed write conf info. err = %d\n", __func__, err);

	err =  _ufs_start(hba);
        if (err)
                return err;
out:
	ufs_info_show_dev_desc(hba->dev_desc);

	ufs_set_ref_clk(hba);

        return set_boot_lu_enable(hba);
}
#endif
