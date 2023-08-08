/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <asm/unaligned.h>
#include <asm/io.h>
#include <dm/device.h>
#include <dm/of_access.h>
#include <dm/read.h>
#include <linux/bitfield.h>
#include <linux/list.h>
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/gpio.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "analogix_dp.h"

#define RK3588_GRF_VO1_CON0	0x0000
#define EDP_MODE		BIT(0)
#define RK3588_GRF_VO1_CON1	0x0004

/**
 * struct rockchip_dp_chip_data - splite the grf setting of kind of chips
 * @lcdsel_grf_reg: grf register offset of lcdc select
 * @lcdsel_big: reg value of selecting vop big for eDP
 * @lcdsel_lit: reg value of selecting vop little for eDP
 * @chip_type: specific chip type
 * @ssc: check if SSC is supported by source
 */
struct rockchip_dp_chip_data {
	u32	lcdsel_grf_reg;
	u32	lcdsel_big;
	u32	lcdsel_lit;
	u32	chip_type;
	bool    ssc;

	u32 max_link_rate;
	u32 max_lane_count;
};

static void
analogix_dp_enable_rx_to_enhanced_mode(struct analogix_dp_device *dp,
				       bool enable)
{
	u8 data;

	analogix_dp_read_byte_from_dpcd(dp, DP_LANE_COUNT_SET, &data);

	if (enable)
		analogix_dp_write_byte_to_dpcd(dp, DP_LANE_COUNT_SET,
					       DP_LANE_COUNT_ENHANCED_FRAME_EN |
					       DPCD_LANE_COUNT_SET(data));
	else
		analogix_dp_write_byte_to_dpcd(dp, DP_LANE_COUNT_SET,
					       DPCD_LANE_COUNT_SET(data));
}

static int analogix_dp_is_enhanced_mode_available(struct analogix_dp_device *dp)
{
	u8 data;
	int retval;

	analogix_dp_read_byte_from_dpcd(dp, DP_MAX_LANE_COUNT, &data);
	retval = DPCD_ENHANCED_FRAME_CAP(data);

	return retval;
}

static void analogix_dp_set_enhanced_mode(struct analogix_dp_device *dp)
{
	u8 data;

	data = analogix_dp_is_enhanced_mode_available(dp);
	analogix_dp_enable_rx_to_enhanced_mode(dp, data);
	analogix_dp_enable_enhanced_mode(dp, data);
}

static void analogix_dp_training_pattern_dis(struct analogix_dp_device *dp)
{
	analogix_dp_set_training_pattern(dp, DP_NONE);

	analogix_dp_write_byte_to_dpcd(dp, DP_TRAINING_PATTERN_SET,
				       DP_TRAINING_PATTERN_DISABLE);
}

static int analogix_dp_link_start(struct analogix_dp_device *dp)
{
	u8 buf[4];
	int lane, lane_count, retval;

	lane_count = dp->link_train.lane_count;

	dp->link_train.lt_state = CLOCK_RECOVERY;
	dp->link_train.eq_loop = 0;

	for (lane = 0; lane < lane_count; lane++)
		dp->link_train.cr_loop[lane] = 0;

	/* Set link rate and count as you want to establish*/
	analogix_dp_set_link_bandwidth(dp, dp->link_train.link_rate);
	analogix_dp_set_lane_count(dp, dp->link_train.lane_count);

	/* Setup RX configuration */
	buf[0] = dp->link_train.link_rate;
	buf[1] = dp->link_train.lane_count;
	retval = analogix_dp_write_bytes_to_dpcd(dp, DP_LINK_BW_SET, 2, buf);
	if (retval)
		return retval;

	/* Spread AMP if required, enable 8b/10b coding */
	buf[0] = analogix_dp_ssc_supported(dp) ? DP_SPREAD_AMP_0_5 : 0;
	buf[1] = DP_SET_ANSI_8B10B;
	retval = analogix_dp_write_bytes_to_dpcd(dp, DP_DOWNSPREAD_CTRL,
						 2, buf);
	if (retval < 0)
		return retval;

	/* Set TX voltage-swing and pre-emphasis to minimum */
	for (lane = 0; lane < lane_count; lane++)
		dp->link_train.training_lane[lane] =
				DP_TRAIN_VOLTAGE_SWING_LEVEL_0 |
				DP_TRAIN_PRE_EMPH_LEVEL_0;
	analogix_dp_set_lane_link_training(dp);

	/* Set training pattern 1 */
	analogix_dp_set_training_pattern(dp, TRAINING_PTN1);

	/* Set RX training pattern */
	retval = analogix_dp_write_byte_to_dpcd(dp,
			DP_TRAINING_PATTERN_SET,
			DP_LINK_SCRAMBLING_DISABLE | DP_TRAINING_PATTERN_1);
	if (retval)
		return retval;

	for (lane = 0; lane < lane_count; lane++)
		buf[lane] = DP_TRAIN_PRE_EMPH_LEVEL_0 |
			    DP_TRAIN_VOLTAGE_SWING_LEVEL_0;

	retval = analogix_dp_write_bytes_to_dpcd(dp, DP_TRAINING_LANE0_SET,
						 lane_count, buf);

	return retval;
}

static unsigned char analogix_dp_get_lane_status(u8 link_status[2], int lane)
{
	int shift = (lane & 1) * 4;
	u8 link_value = link_status[lane >> 1];

	return (link_value >> shift) & 0xf;
}

static int analogix_dp_clock_recovery_ok(u8 link_status[2], int lane_count)
{
	int lane;
	u8 lane_status;

	for (lane = 0; lane < lane_count; lane++) {
		lane_status = analogix_dp_get_lane_status(link_status, lane);
		if ((lane_status & DP_LANE_CR_DONE) == 0)
			return -EINVAL;
	}
	return 0;
}

static int analogix_dp_channel_eq_ok(u8 link_status[2], u8 link_align,
				     int lane_count)
{
	int lane;
	u8 lane_status;

	if ((link_align & DP_INTERLANE_ALIGN_DONE) == 0)
		return -EINVAL;

	for (lane = 0; lane < lane_count; lane++) {
		lane_status = analogix_dp_get_lane_status(link_status, lane);
		lane_status &= DP_CHANNEL_EQ_BITS;
		if (lane_status != DP_CHANNEL_EQ_BITS)
			return -EINVAL;
	}

	return 0;
}

static unsigned char
analogix_dp_get_adjust_request_voltage(u8 adjust_request[2], int lane)
{
	int shift = (lane & 1) * 4;
	u8 link_value = adjust_request[lane >> 1];

	return (link_value >> shift) & 0x3;
}

static unsigned char analogix_dp_get_adjust_request_pre_emphasis(
					u8 adjust_request[2],
					int lane)
{
	int shift = (lane & 1) * 4;
	u8 link_value = adjust_request[lane >> 1];

	return ((link_value >> shift) & 0xc) >> 2;
}

static void analogix_dp_reduce_link_rate(struct analogix_dp_device *dp)
{
	analogix_dp_training_pattern_dis(dp);
	analogix_dp_set_enhanced_mode(dp);

	dp->link_train.lt_state = FAILED;
}

static void analogix_dp_get_adjust_training_lane(struct analogix_dp_device *dp,
						 u8 adjust_request[2])
{
	int lane, lane_count;
	u8 voltage_swing, pre_emphasis, training_lane;

	lane_count = dp->link_train.lane_count;
	for (lane = 0; lane < lane_count; lane++) {
		voltage_swing = analogix_dp_get_adjust_request_voltage(
						adjust_request, lane);
		pre_emphasis = analogix_dp_get_adjust_request_pre_emphasis(
						adjust_request, lane);
		training_lane = DPCD_VOLTAGE_SWING_SET(voltage_swing) |
				DPCD_PRE_EMPHASIS_SET(pre_emphasis);

		if (voltage_swing == VOLTAGE_LEVEL_3)
			training_lane |= DP_TRAIN_MAX_SWING_REACHED;
		if (pre_emphasis == PRE_EMPHASIS_LEVEL_3)
			training_lane |= DP_TRAIN_MAX_PRE_EMPHASIS_REACHED;

		dp->link_train.training_lane[lane] = training_lane;
	}
}

static bool analogix_dp_tps3_supported(struct analogix_dp_device *dp)
{
	bool source_tps3_supported, sink_tps3_supported;
	u8 dpcd = 0;

	source_tps3_supported =
		dp->video_info.max_link_rate == DP_LINK_BW_5_4;
	analogix_dp_read_byte_from_dpcd(dp, DP_MAX_LANE_COUNT, &dpcd);
	sink_tps3_supported = dpcd & DP_TPS3_SUPPORTED;

	return source_tps3_supported && sink_tps3_supported;
}

static int analogix_dp_process_clock_recovery(struct analogix_dp_device *dp)
{
	int lane, lane_count, retval;
	u8 voltage_swing, pre_emphasis, training_lane;
	u8 link_status[2], adjust_request[2];
	u8 training_pattern = TRAINING_PTN2;

	drm_dp_link_train_clock_recovery_delay(dp->dpcd);

	lane_count = dp->link_train.lane_count;

	retval =  analogix_dp_read_bytes_from_dpcd(dp,
			DP_LANE0_1_STATUS, 2, link_status);
	if (retval)
		return retval;

	if (analogix_dp_clock_recovery_ok(link_status, lane_count) == 0) {
		if (analogix_dp_tps3_supported(dp))
			training_pattern = TRAINING_PTN3;

		/* set training pattern for EQ */
		analogix_dp_set_training_pattern(dp, training_pattern);

		retval = analogix_dp_write_byte_to_dpcd(dp,
				DP_TRAINING_PATTERN_SET,
				(training_pattern == TRAINING_PTN3 ?
				 DP_TRAINING_PATTERN_3 : DP_TRAINING_PATTERN_2));
		if (retval)
			return retval;

		dev_info(dp->dev, "Link Training Clock Recovery success\n");
		dp->link_train.lt_state = EQUALIZER_TRAINING;

		return 0;
	} else {
		retval = analogix_dp_read_bytes_from_dpcd(dp,
				DP_ADJUST_REQUEST_LANE0_1, 2, adjust_request);
		if (retval)
			return retval;

		for (lane = 0; lane < lane_count; lane++) {
			training_lane = analogix_dp_get_lane_link_training(
							dp, lane);
			voltage_swing = analogix_dp_get_adjust_request_voltage(
							adjust_request, lane);
			pre_emphasis = analogix_dp_get_adjust_request_pre_emphasis(
							adjust_request, lane);

			if (DPCD_VOLTAGE_SWING_GET(training_lane) ==
					voltage_swing &&
			    DPCD_PRE_EMPHASIS_GET(training_lane) ==
					pre_emphasis)
				dp->link_train.cr_loop[lane]++;

			if (dp->link_train.cr_loop[lane] == MAX_CR_LOOP ||
			    voltage_swing == VOLTAGE_LEVEL_3 ||
			    pre_emphasis == PRE_EMPHASIS_LEVEL_3) {
				dev_err(dp->dev, "CR Max reached (%d,%d,%d)\n",
					dp->link_train.cr_loop[lane],
					voltage_swing, pre_emphasis);
				analogix_dp_reduce_link_rate(dp);
				return -EIO;
			}
		}
	}

	analogix_dp_get_adjust_training_lane(dp, adjust_request);
	analogix_dp_set_lane_link_training(dp);

	retval = analogix_dp_write_bytes_to_dpcd(dp,
			DP_TRAINING_LANE0_SET, lane_count,
			dp->link_train.training_lane);
	if (retval)
		return retval;

	return retval;
}

static int analogix_dp_process_equalizer_training(struct analogix_dp_device *dp)
{
	int lane_count, retval;
	u32 reg;
	u8 link_align, link_status[2], adjust_request[2];

	drm_dp_link_train_channel_eq_delay(dp->dpcd);

	lane_count = dp->link_train.lane_count;

	retval = analogix_dp_read_bytes_from_dpcd(dp,
			DP_LANE0_1_STATUS, 2, link_status);
	if (retval)
		return retval;

	if (analogix_dp_clock_recovery_ok(link_status, lane_count)) {
		analogix_dp_reduce_link_rate(dp);
		return -EIO;
	}

	retval = analogix_dp_read_byte_from_dpcd(dp,
			DP_LANE_ALIGN_STATUS_UPDATED, &link_align);
	if (retval)
		return retval;

	if (!analogix_dp_channel_eq_ok(link_status, link_align, lane_count)) {
		/* traing pattern Set to Normal */
		analogix_dp_training_pattern_dis(dp);

		printf("Link Training success!\n");

		analogix_dp_get_link_bandwidth(dp, &reg);
		dp->link_train.link_rate = reg;
		analogix_dp_get_lane_count(dp, &reg);
		dp->link_train.lane_count = reg;

		printf("final link rate = 0x%.2x, lane count = 0x%.2x\n",
		       dp->link_train.link_rate, dp->link_train.lane_count);

		/* set enhanced mode if available */
		analogix_dp_set_enhanced_mode(dp);
		dp->link_train.lt_state = FINISHED;

		return 0;
	}

	/* not all locked */
	dp->link_train.eq_loop++;

	if (dp->link_train.eq_loop > MAX_EQ_LOOP) {
		dev_dbg(dp->dev, "EQ Max loop\n");
		analogix_dp_reduce_link_rate(dp);
		return -EIO;
	}

	retval = analogix_dp_read_bytes_from_dpcd(dp,
			DP_ADJUST_REQUEST_LANE0_1, 2, adjust_request);
	if (retval)
		return retval;

	analogix_dp_get_adjust_training_lane(dp, adjust_request);
	analogix_dp_set_lane_link_training(dp);

	retval = analogix_dp_write_bytes_to_dpcd(dp, DP_TRAINING_LANE0_SET,
			lane_count, dp->link_train.training_lane);

	return retval;
}

static void analogix_dp_get_max_rx_bandwidth(struct analogix_dp_device *dp,
					     u8 *bandwidth)
{
	u8 data;

	/*
	 * For DP rev.1.1, Maximum link rate of Main Link lanes
	 * 0x06 = 1.62 Gbps, 0x0a = 2.7 Gbps
	 * For DP rev.1.2, Maximum link rate of Main Link lanes
	 * 0x06 = 1.62 Gbps, 0x0a = 2.7 Gbps, 0x14 = 5.4Gbps
	 */
	analogix_dp_read_byte_from_dpcd(dp, DP_MAX_LINK_RATE, &data);
	*bandwidth = data;
}

static void analogix_dp_get_max_rx_lane_count(struct analogix_dp_device *dp,
					      u8 *lane_count)
{
	u8 data;

	/*
	 * For DP rev.1.1, Maximum number of Main Link lanes
	 * 0x01 = 1 lane, 0x02 = 2 lanes, 0x04 = 4 lanes
	 */
	analogix_dp_read_byte_from_dpcd(dp, DP_MAX_LANE_COUNT, &data);
	*lane_count = DPCD_MAX_LANE_COUNT(data);
}

static int analogix_dp_init_training(struct analogix_dp_device *dp,
				     enum link_lane_count_type max_lane,
				     int max_rate)
{
	u8 dpcd;

	/*
	 * MACRO_RST must be applied after the PLL_LOCK to avoid
	 * the DP inter pair skew issue for at least 10 us
	 */
	analogix_dp_reset_macro(dp);

	/* Initialize by reading RX's DPCD */
	analogix_dp_get_max_rx_bandwidth(dp, &dp->link_train.link_rate);
	analogix_dp_get_max_rx_lane_count(dp, &dp->link_train.lane_count);

	/* Setup TX lane count & rate */
	dp->link_train.lane_count = min_t(u8, dp->link_train.lane_count,
					  max_lane);
	dp->link_train.link_rate = min_t(u32, dp->link_train.link_rate,
					 max_rate);

	analogix_dp_read_byte_from_dpcd(dp, DP_MAX_DOWNSPREAD, &dpcd);
	dp->link_train.ssc = !!(dpcd & DP_MAX_DOWNSPREAD_0_5);

	/* All DP analog module power up */
	analogix_dp_set_analog_power_down(dp, POWER_ALL, 0);

	return 0;
}

static int analogix_dp_sw_link_training(struct analogix_dp_device *dp)
{
	int retval = 0, training_finished = 0;

	dp->link_train.lt_state = START;

	/* Process here */
	while (!retval && !training_finished) {
		switch (dp->link_train.lt_state) {
		case START:
			retval = analogix_dp_link_start(dp);
			if (retval)
				dev_err(dp->dev, "LT link start failed!\n");
			break;
		case CLOCK_RECOVERY:
			retval = analogix_dp_process_clock_recovery(dp);
			if (retval)
				dev_err(dp->dev, "LT CR failed!\n");
			break;
		case EQUALIZER_TRAINING:
			retval = analogix_dp_process_equalizer_training(dp);
			if (retval)
				dev_err(dp->dev, "LT EQ failed!\n");
			break;
		case FINISHED:
			training_finished = 1;
			break;
		case FAILED:
			return -EREMOTEIO;
		}
	}

	return retval;
}

static int analogix_dp_set_link_train(struct analogix_dp_device *dp,
				      u32 count, u32 bwtype)
{
	int i, ret;

	for (i = 0; i < 5; i++) {
		ret = analogix_dp_init_training(dp, count, bwtype);
		if (ret < 0) {
			dev_err(dp->dev, "failed to init training\n");
			return ret;
		}

		ret = analogix_dp_sw_link_training(dp);
		if (!ret)
			break;
	}

	return ret;
}

static int analogix_dp_config_video(struct analogix_dp_device *dp)
{
	int timeout_loop = 0;
	int done_count = 0;

	analogix_dp_config_video_slave_mode(dp);

	analogix_dp_set_video_color_format(dp);

	if (analogix_dp_get_pll_lock_status(dp) == PLL_UNLOCKED) {
		dev_err(dp->dev, "PLL is not locked yet.\n");
		return -EINVAL;
	}

	for (;;) {
		timeout_loop++;
		if (analogix_dp_is_slave_video_stream_clock_on(dp) == 0)
			break;
		if (timeout_loop > DP_TIMEOUT_LOOP_COUNT) {
			dev_err(dp->dev, "Timeout of video streamclk ok\n");
			return -ETIMEDOUT;
		}

		udelay(2);
	}

	/* Set to use the register calculated M/N video */
	analogix_dp_set_video_cr_mn(dp, CALCULATED_M, 0, 0);

	/* For video bist, Video timing must be generated by register */
	analogix_dp_set_video_timing_mode(dp, VIDEO_TIMING_FROM_REGISTER);

	/* Disable video mute */
	analogix_dp_enable_video_mute(dp, 0);

	/* Configure video slave mode */
	analogix_dp_enable_video_master(dp, 0);

	/* Enable video input */
	analogix_dp_start_video(dp);

	timeout_loop = 0;

	for (;;) {
		timeout_loop++;
		if (analogix_dp_is_video_stream_on(dp) == 0) {
			done_count++;
			if (done_count > 10)
				break;
		} else if (done_count) {
			done_count = 0;
		}
		if (timeout_loop > DP_TIMEOUT_LOOP_COUNT) {
			dev_err(dp->dev, "Timeout of video streamclk ok\n");
			return -ETIMEDOUT;
		}

		udelay(1001);
	}

	return 0;
}

static void analogix_dp_enable_scramble(struct analogix_dp_device *dp,
					bool enable)
{
	u8 data;

	if (enable) {
		analogix_dp_enable_scrambling(dp);

		analogix_dp_read_byte_from_dpcd(dp, DP_TRAINING_PATTERN_SET,
						&data);
		analogix_dp_write_byte_to_dpcd(dp,
			DP_TRAINING_PATTERN_SET,
			(u8)(data & ~DP_LINK_SCRAMBLING_DISABLE));
	} else {
		analogix_dp_disable_scrambling(dp);

		analogix_dp_read_byte_from_dpcd(dp, DP_TRAINING_PATTERN_SET,
						&data);
		analogix_dp_write_byte_to_dpcd(dp,
			DP_TRAINING_PATTERN_SET,
			(u8)(data | DP_LINK_SCRAMBLING_DISABLE));
	}
}

static void analogix_dp_init_dp(struct analogix_dp_device *dp)
{
	analogix_dp_reset(dp);

	analogix_dp_swreset(dp);

	analogix_dp_init_analog_param(dp);
	analogix_dp_init_interrupt(dp);

	/* SW defined function Normal operation */
	analogix_dp_enable_sw_function(dp);

	analogix_dp_config_interrupt(dp);
	analogix_dp_init_analog_func(dp);

	analogix_dp_init_hpd(dp);
	analogix_dp_init_aux(dp);
}

static unsigned char analogix_dp_calc_edid_check_sum(unsigned char *edid_data)
{
	int i;
	unsigned char sum = 0;

	for (i = 0; i < EDID_BLOCK_LENGTH; i++)
		sum = sum + edid_data[i];

	return sum;
}

static int analogix_dp_read_edid(struct analogix_dp_device *dp)
{
	unsigned char *edid = dp->edid;
	unsigned int extend_block = 0;
	unsigned char test_vector;
	int retval;

	/*
	 * EDID device address is 0x50.
	 * However, if necessary, you must have set upper address
	 * into E-EDID in I2C device, 0x30.
	 */

	/* Read Extension Flag, Number of 128-byte EDID extension blocks */
	retval = analogix_dp_read_byte_from_i2c(dp, I2C_EDID_DEVICE_ADDR,
						EDID_EXTENSION_FLAG,
						&extend_block);
	if (retval)
		return retval;

	if (extend_block > 0) {
		debug("EDID data includes a single extension!\n");

		/* Read EDID data */
		retval = analogix_dp_read_bytes_from_i2c(dp,
						I2C_EDID_DEVICE_ADDR,
						EDID_HEADER_PATTERN,
						EDID_BLOCK_LENGTH,
						&edid[EDID_HEADER_PATTERN]);
		if (retval < 0)
			return retval;

		if (analogix_dp_calc_edid_check_sum(edid))
			return -EINVAL;

		/* Read additional EDID data */
		retval = analogix_dp_read_bytes_from_i2c(dp,
				I2C_EDID_DEVICE_ADDR,
				EDID_BLOCK_LENGTH,
				EDID_BLOCK_LENGTH,
				&edid[EDID_BLOCK_LENGTH]);
		if (retval < 0)
			return retval;

		if (analogix_dp_calc_edid_check_sum(&edid[EDID_BLOCK_LENGTH]))
			return -EINVAL;

		analogix_dp_read_byte_from_dpcd(dp, DP_TEST_REQUEST,
						&test_vector);
		if (test_vector & DP_TEST_LINK_EDID_READ) {
			analogix_dp_write_byte_to_dpcd(dp,
				DP_TEST_EDID_CHECKSUM,
				edid[EDID_BLOCK_LENGTH + EDID_CHECKSUM]);
			analogix_dp_write_byte_to_dpcd(dp,
				DP_TEST_RESPONSE,
				DP_TEST_EDID_CHECKSUM_WRITE);
		}
	} else {
		dev_info(dp->dev,
			 "EDID data does not include any extensions.\n");

		/* Read EDID data */
		retval = analogix_dp_read_bytes_from_i2c(dp,
				I2C_EDID_DEVICE_ADDR, EDID_HEADER_PATTERN,
				EDID_BLOCK_LENGTH, &edid[EDID_HEADER_PATTERN]);
		if (retval < 0)
			return retval;

		if (analogix_dp_calc_edid_check_sum(edid))
			return -EINVAL;

		analogix_dp_read_byte_from_dpcd(dp, DP_TEST_REQUEST,
						&test_vector);
		if (test_vector & DP_TEST_LINK_EDID_READ) {
			analogix_dp_write_byte_to_dpcd(dp,
				DP_TEST_EDID_CHECKSUM, edid[EDID_CHECKSUM]);
			analogix_dp_write_byte_to_dpcd(dp,
				DP_TEST_RESPONSE, DP_TEST_EDID_CHECKSUM_WRITE);
		}
	}

	return 0;
}

static int analogix_dp_handle_edid(struct analogix_dp_device *dp)
{
	u8 buf[12];
	int i, try = 5;
	int retval;

retry:
	/* Read DPCD DP_DPCD_REV~RECEIVE_PORT1_CAP_1 */
	retval = analogix_dp_read_bytes_from_dpcd(dp, DP_DPCD_REV, 12, buf);

	if (retval && try--) {
		mdelay(10);
		goto retry;
	}

	if (retval)
		return retval;

	/* Read EDID */
	for (i = 0; i < 3; i++) {
		retval = analogix_dp_read_edid(dp);
		if (!retval)
			break;
	}

	return retval;
}

static int analogix_dp_connector_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct analogix_dp_device *dp = dev_get_priv(conn->dev);

	conn_state->output_if |= dp->id ? VOP_OUTPUT_IF_eDP1 : VOP_OUTPUT_IF_eDP0;
	conn_state->output_mode = ROCKCHIP_OUT_MODE_AAAA;
	conn_state->color_space = V4L2_COLORSPACE_DEFAULT;

	reset_assert_bulk(&dp->resets);
	udelay(1);
	reset_deassert_bulk(&dp->resets);

	conn_state->disp_info  = rockchip_get_disp_info(conn_state->type, dp->id);
	generic_phy_set_mode(&dp->phy, PHY_MODE_DP);
	generic_phy_power_on(&dp->phy);
	analogix_dp_init_dp(dp);

	return 0;
}

static int analogix_dp_connector_get_edid(struct rockchip_connector *conn,
					  struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct analogix_dp_device *dp = dev_get_priv(conn->dev);
	int ret;

	ret = analogix_dp_handle_edid(dp);
	if (ret) {
		dev_err(dp->dev, "failed to get edid\n");
		return ret;
	}

	memcpy(&conn_state->edid, &dp->edid, sizeof(dp->edid));

	return 0;
}

static int analogix_dp_link_power_up(struct analogix_dp_device *dp)
{
	u8 value;
	int ret;

	if (dp->dpcd[DP_DPCD_REV] < 0x11)
		return 0;

	ret = analogix_dp_read_byte_from_dpcd(dp, DP_SET_POWER, &value);
	if (ret < 0)
		return ret;

	value &= ~DP_SET_POWER_MASK;
	value |= DP_SET_POWER_D0;

	ret = analogix_dp_write_byte_to_dpcd(dp, DP_SET_POWER, value);
	if (ret < 0)
		return ret;

	mdelay(1);

	return 0;
}

static int analogix_dp_link_power_down(struct analogix_dp_device *dp)
{
	u8 value;
	int ret;

	if (dp->dpcd[DP_DPCD_REV] < 0x11)
		return 0;

	ret = analogix_dp_read_byte_from_dpcd(dp, DP_SET_POWER, &value);
	if (ret < 0)
		return ret;

	value &= ~DP_SET_POWER_MASK;
	value |= DP_SET_POWER_D3;

	ret = analogix_dp_write_byte_to_dpcd(dp, DP_SET_POWER, value);
	if (ret < 0)
		return ret;

	return 0;
}

static int analogix_dp_connector_enable(struct rockchip_connector *conn,
					struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_dp_chip_data *pdata =
		(const struct rockchip_dp_chip_data *)dev_get_driver_data(conn->dev);
	struct analogix_dp_device *dp = dev_get_priv(conn->dev);
	struct video_info *video = &dp->video_info;
	u32 val;
	int ret;

	if (pdata->lcdsel_grf_reg) {
		if (crtc_state->crtc_id)
			val = pdata->lcdsel_lit;
		else
			val = pdata->lcdsel_big;

		regmap_write(dp->grf, pdata->lcdsel_grf_reg, val);
	}

	if (pdata->chip_type == RK3588_EDP)
		regmap_write(dp->grf, dp->id ? RK3588_GRF_VO1_CON1 : RK3588_GRF_VO1_CON0,
			     EDP_MODE << 16 | FIELD_PREP(EDP_MODE, 1));

	switch (conn_state->bpc) {
	case 12:
		video->color_depth = COLOR_12;
		break;
	case 10:
		video->color_depth = COLOR_10;
		break;
	case 6:
		video->color_depth = COLOR_6;
		break;
	case 8:
	default:
		video->color_depth = COLOR_8;
		break;
	}

	ret = analogix_dp_read_bytes_from_dpcd(dp, DP_DPCD_REV,
					       DP_RECEIVER_CAP_SIZE, dp->dpcd);
	if (ret) {
		dev_err(dp->dev, "failed to read dpcd caps: %d\n", ret);
		return ret;
	}

	ret = analogix_dp_link_power_up(dp);
	if (ret) {
		dev_err(dp->dev, "failed to power up link: %d\n", ret);
		return ret;
	}

	ret = analogix_dp_set_link_train(dp, dp->video_info.max_lane_count,
					 dp->video_info.max_link_rate);
	if (ret) {
		dev_err(dp->dev, "unable to do link train\n");
		return ret;
	}

	analogix_dp_enable_scramble(dp, 1);
	analogix_dp_enable_rx_to_enhanced_mode(dp, 1);
	analogix_dp_enable_enhanced_mode(dp, 1);

	analogix_dp_init_video(dp);
	analogix_dp_set_video_format(dp, &conn_state->mode);

	if (dp->video_bist_enable)
		analogix_dp_video_bist_enable(dp);

	ret = analogix_dp_config_video(dp);
	if (ret) {
		dev_err(dp->dev, "unable to config video\n");
		return ret;
	}

	return 0;
}

static int analogix_dp_connector_disable(struct rockchip_connector *conn,
					 struct display_state *state)
{
	const struct rockchip_dp_chip_data *pdata =
		(const struct rockchip_dp_chip_data *)dev_get_driver_data(conn->dev);
	struct analogix_dp_device *dp = dev_get_priv(conn->dev);

	if (!analogix_dp_get_plug_in_status(dp))
		analogix_dp_link_power_down(dp);

	if (pdata->chip_type == RK3588_EDP)
		regmap_write(dp->grf, dp->id ? RK3588_GRF_VO1_CON1 : RK3588_GRF_VO1_CON0,
			     EDP_MODE << 16 | FIELD_PREP(EDP_MODE, 0));

	return 0;
}

static int analogix_dp_connector_detect(struct rockchip_connector *conn,
					struct display_state *state)
{
	struct analogix_dp_device *dp = dev_get_priv(conn->dev);

	return analogix_dp_detect(dp);
}

static const struct rockchip_connector_funcs analogix_dp_connector_funcs = {
	.init = analogix_dp_connector_init,
	.get_edid = analogix_dp_connector_get_edid,
	.enable = analogix_dp_connector_enable,
	.disable = analogix_dp_connector_disable,
	.detect = analogix_dp_connector_detect,
};

static int analogix_dp_parse_dt(struct analogix_dp_device *dp)
{
	struct udevice *dev = dp->dev;
	int len;
	u32 num_lanes;
	u32 max_link_rate;
	int ret;

	dp->force_hpd = dev_read_bool(dev, "force-hpd");
	dp->video_bist_enable = dev_read_bool(dev, "analogix,video-bist-enable");
	dp->video_info.force_stream_valid =
		dev_read_bool(dev, "analogix,force-stream-valid");

	max_link_rate = dev_read_u32_default(dev, "max-link-rate", 0);
	if (max_link_rate) {
		switch (max_link_rate) {
		case 1620:
		case 2700:
		case 5400:
			dp->video_info.max_link_rate =
				min_t(u8, dp->video_info.max_link_rate,
				      drm_dp_link_rate_to_bw_code(max_link_rate * 100));
			break;
		default:
			dev_err(dev, "invalid max-link-rate %d\n", max_link_rate);
			break;
		}
	}

	if (dev_read_prop(dev, "data-lanes", &len)) {
		num_lanes = len / sizeof(u32);
		if (num_lanes < 1 || num_lanes > 4 || num_lanes == 3) {
			dev_err(dev, "bad number of data lanes\n");
			return -EINVAL;
		}

		ret = dev_read_u32_array(dev, "data-lanes", dp->lane_map,
					 num_lanes);
		if (ret)
			return ret;

		dp->video_info.max_lane_count = num_lanes;
	} else {
		dp->lane_map[0] = 0;
		dp->lane_map[1] = 1;
		dp->lane_map[2] = 2;
		dp->lane_map[3] = 3;
	}

	return 0;
}

static int analogix_dp_probe(struct udevice *dev)
{
	struct analogix_dp_device *dp = dev_get_priv(dev);
	const struct rockchip_dp_chip_data *pdata =
		(const struct rockchip_dp_chip_data *)dev_get_driver_data(dev);
	struct udevice *syscon;
	int ret;

	dp->reg_base = dev_read_addr_ptr(dev);

	dp->id = of_alias_get_id(ofnode_to_np(dev->node), "edp");
	if (dp->id < 0)
		dp->id = 0;

	ret = uclass_get_device_by_phandle(UCLASS_SYSCON, dev, "rockchip,grf",
					   &syscon);
	if (!ret) {
		dp->grf = syscon_get_regmap(syscon);
		if (!dp->grf)
			return -ENODEV;
	}

	ret = reset_get_bulk(dev, &dp->resets);
	if (ret) {
		dev_err(dev, "failed to get reset control: %d\n", ret);
		return ret;
	}

	ret = gpio_request_by_name(dev, "hpd-gpios", 0, &dp->hpd_gpio,
				   GPIOD_IS_IN);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "failed to get hpd GPIO: %d\n", ret);
		return ret;
	}

	generic_phy_get_by_name(dev, "dp", &dp->phy);

	dp->plat_data.dev_type = ROCKCHIP_DP;
	dp->plat_data.subdev_type = pdata->chip_type;
	dp->plat_data.ssc = pdata->ssc;

	dp->video_info.max_link_rate = pdata->max_link_rate;
	dp->video_info.max_lane_count = pdata->max_lane_count;

	dp->dev = dev;

	ret = analogix_dp_parse_dt(dp);
	if (ret) {
		dev_err(dev, "failed to parse DT: %d\n", ret);
		return ret;
	}

	rockchip_connector_bind(&dp->connector, dev, dp->id, &analogix_dp_connector_funcs,
				NULL, DRM_MODE_CONNECTOR_eDP);

	return 0;
}

static const struct rockchip_dp_chip_data rk3288_edp_platform_data = {
	.lcdsel_grf_reg = 0x025c,
	.lcdsel_big = 0 | BIT(21),
	.lcdsel_lit = BIT(5) | BIT(21),
	.chip_type = RK3288_DP,

	.max_link_rate = DP_LINK_BW_2_7,
	.max_lane_count = 4,
};

static const struct rockchip_dp_chip_data rk3368_edp_platform_data = {
	.chip_type = RK3368_EDP,

	.max_link_rate = DP_LINK_BW_2_7,
	.max_lane_count = 4,
};

static const struct rockchip_dp_chip_data rk3399_edp_platform_data = {
	.lcdsel_grf_reg = 0x6250,
	.lcdsel_big = 0 | BIT(21),
	.lcdsel_lit = BIT(5) | BIT(21),
	.chip_type = RK3399_EDP,

	.max_link_rate = DP_LINK_BW_2_7,
	.max_lane_count = 4,
};

static const struct rockchip_dp_chip_data rk3568_edp_platform_data = {
	.chip_type = RK3568_EDP,
	.ssc = true,

	.max_link_rate = DP_LINK_BW_2_7,
	.max_lane_count = 4,
};

static const struct rockchip_dp_chip_data rk3588_edp_platform_data = {
	.chip_type = RK3588_EDP,
	.ssc = true,

	.max_link_rate = DP_LINK_BW_5_4,
	.max_lane_count = 4,
};

static const struct udevice_id analogix_dp_ids[] = {
	{
		.compatible = "rockchip,rk3288-dp",
		.data = (ulong)&rk3288_edp_platform_data,
	}, {
		.compatible = "rockchip,rk3368-edp",
		.data = (ulong)&rk3368_edp_platform_data,
	}, {
		.compatible = "rockchip,rk3399-edp",
		.data = (ulong)&rk3399_edp_platform_data,
	}, {
		.compatible = "rockchip,rk3568-edp",
		.data = (ulong)&rk3568_edp_platform_data,
	}, {
		.compatible = "rockchip,rk3588-edp",
		.data = (ulong)&rk3588_edp_platform_data,
	},
	{}
};

U_BOOT_DRIVER(analogix_dp) = {
	.name = "analogix_dp",
	.id = UCLASS_DISPLAY,
	.of_match = analogix_dp_ids,
	.probe = analogix_dp_probe,
	.priv_auto_alloc_size = sizeof(struct analogix_dp_device),
};
