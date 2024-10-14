// SPDX-License-Identifier: GPL-2.0+
/*
 * Rockchip UFS Host Controller driver
 *
 * Copyright (C) 2024 Rockchip Electronics Co.Ltd.
 */

#define WELL_BOOT_LU_A		0x01
#define WELL_BOOT_LU_B		0x02

#define DEFAULT_BOOT_LUN	WELL_BOOT_LU_A
#define DEFAULT_ACTIVE_LUN	0

#define CONFIGURATION_DESC_V31_LENGTH	0xE6
#define CONFIGURATION_DESC_V22_LENGTH	0x90
#define UNIT_DESCS_COUNT		8

/* Byte swap u16 */
static inline uint16_t swap_16(uint16_t val)
{
	return (uint16_t)((val << 8) | (val >> 8)); /* shift 8 */
}

/* Byte swap unsigned int */
static inline uint32_t swap_32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF); /* shift 8 */
	return (uint32_t)((val << 16) | (val >> 16)); /* shift 16 */
}

static inline uint16_t to_bigendian16(uint16_t val)
{
#ifdef HOST_BIG_ENDIAN
	return val;
#else
	return swap_16(val);
#endif
}

static inline uint32_t to_bigendian32(uint32_t val)
{
#ifdef HOST_BIG_ENDIAN
	return val;
#else
	return swap_32(val);
#endif
}

enum attr_id {
	B_BOOT_LUNEN = 0x0,
	B_CURRENT_PM = 0x2,
	B_ACTIV_ICC_LEVEL = 0x3,
	B_OUT_OF_ORDER_DATAEN = 0x4,
	B_BCKGND_OPS_STATUS = 0x5,
	B_PURGE_STATUS = 0x6,
	B_MAX_DATA_IN_SIZE = 0x7,
	B_MAX_DATA_OUT_SIZE = 0x8,
	D_DYN_CAP_NEEDED = 0x9,
	B_REFCLK_FREQ = 0xA,
	B_CONFIG_DESC_LOCK = 0xB,
	B_MAX_NUM_OF_RTT = 0xC,
	W_EXCEPTION_EVENT_CONTROL = 0xD,
	W_EXCEPTION_EVENT_STATUS = 0xE,
	D_SECONDS_PASSED = 0xF,
	W_CONTEXT_CONF = 0x10,
	D_CORR_PRG_BLKNUM = 0x11
};

struct ufs_dev_desc_configuration_param {
	uint8_t b_length;
	uint8_t b_descriptor_idn;
	uint8_t b_conf_desc_continue;
	uint8_t b_boot_enable;
	uint8_t b_descr_access_en;
	uint8_t b_init_power_mode;
	uint8_t b_high_priority_lun;
	uint8_t b_secure_removal_type;
	uint8_t b_init_active_icc_level;
	uint16_t w_periodic_rtc_update;
	uint8_t reserved[5]; /* 5 reserved, 11 in ufs3.1 */
	uint8_t b_write_booster_buffer_reserve_user_space_en;
	uint8_t b_write_booster_buffer_type;
	uint32_t d_num_shared_write_booster_buffer_alloc_units;
} __attribute__ ((packed));

struct ufs_unit_desc_configuration_param {
	uint8_t b_lu_enable;
	uint8_t b_boot_lun_id;
	uint8_t b_lu_write_protect;
	uint8_t b_memory_type;
	uint32_t d_num_alloc_units;
	uint8_t b_data_reliability;
	uint8_t b_logical_block_size;
	uint8_t b_provisioning_type;
	uint16_t w_context_capabilities;
	uint8_t reserved[13]; /* 3 reserved, 13 in ufs3.1 */
} __attribute__ ((packed));

struct ufs_configuration_descriptor {
	struct ufs_dev_desc_configuration_param dev_desc_conf_param;
	struct ufs_unit_desc_configuration_param unit_desc_conf_param[UNIT_DESCS_COUNT];
} __attribute__ ((packed));

struct ufs_geometry_descriptor {
	uint8_t b_length;
	uint8_t b_descriptor_idn;
	uint8_t b_media_technology;
	uint8_t reserved;
	uint64_t q_total_raw_device_capacity;
	uint8_t b_max_number_lu;
	uint32_t d_segment_size;
	uint8_t b_allocation_unit_size;
	uint8_t b_min_addr_block_size;
	uint8_t b_optimal_read_block_size;
	uint8_t b_optimal_write_block_size;
	uint8_t b_max_in_buffer_size;
	uint8_t b_max_out_buffer_size;
	uint8_t b_rpmb_read_write_size;
	uint8_t b_dynamic_capacity_resource_policy;
	uint8_t b_data_ordering;
	uint8_t b_max_contex_id_number;
	uint8_t b_sys_data_tag_unit_size;
	uint8_t b_sys_data_tag_res_size;
	uint8_t b_supported_sec_rtypes;
	uint16_t w_supported_memory_types;
	uint32_t d_system_code_max_alloc_u;
	uint16_t w_system_code_cap_adj_fac;
	uint32_t d_non_persist_max_alloc_u;
	uint16_t w_non_persist_cap_adj_fac;
	uint32_t d_enhanced1_max_alloc_u;
	uint16_t w_enhanced1_cap_adj_fac;
	uint32_t d_enhanced2_max_alloc_u;
	uint16_t w_enhanced2_cap_adj_fac;
	uint32_t d_enhanced3_max_alloc_u;
	uint16_t w_enhanced3_cap_adj_fac;
	uint32_t d_enhanced4_max_alloc_u;
	uint16_t w_enhanced4_cap_adj_fac;
	uint32_t d_optimal_logical_block_size;
	/* 15 reserved in ufs3.1 */
	uint8_t b_hpb_region_size;
	uint8_t b_hpb_number_lu;
	uint8_t b_hpb_sub_region_size;
	uint16_t w_device_max_active_hpb_regions;
	uint16_t w_eserved;
	uint32_t d_write_booster_buffer_max_alloc_units;
	uint8_t b_device_max_write_booster_lus;
	uint8_t b_write_booster_buffer_cap_adj_fac;
	uint8_t b_supported_write_booster_buffer_user_space_reduction_types;
	uint8_t b_supported_write_booster_buffer_types;
} __attribute__ ((packed));
