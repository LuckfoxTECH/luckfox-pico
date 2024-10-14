/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_panel.c
  * @version V0.1
  * @brief   display panel driver for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup Panel
 *  @{
 */

/** @defgroup Panel_How_To_Use How To Use
 *  @{

 The panel driver use to parse panel init config and control panel power on/off.
 provide to sent mcu/mipi/spi init cmd.

 @} */
#include <rtdevice.h>
#include <rtthread.h>
#include "board.h"
#ifdef RT_USING_VOP

#include "drv_display.h"
#include "drv_panel_cfg.h"
#include "hal_base.h"

/********************* Private Function Definition ***************************/
/** @defgroup Panel_Private_Function Private Function
 *  @{
 */

/**
 * @brief  Parse panel init command.
 * @param  state: display state.
 * @param  cmds_input: command input before parse.
 * @param  cmd_cnt: command count.
 * @param  cmds_output: command output after parse.
 * @return rt_err_t.
 */
static rt_err_t rockchip_panel_parse_cmd(struct display_state *state,
        const struct rockchip_cmd *cmds_input,
        uint16_t cmd_cnt,
        struct rockchip_panel_cmds *cmds_output)
{
    uint16_t i;

    if (cmd_cnt == 0)
        return RT_EOK;

    cmds_output->cmd_cnt = cmd_cnt;
    cmds_output->cmds = rt_malloc(sizeof(struct rockchip_cmd_desc) * cmd_cnt);
    if (!cmds_output->cmds)
        return -RT_ENOMEM;

    for (i = 0; i < cmd_cnt; i++)
    {
        cmds_output->cmds[i].header.data_type = cmds_input[i].data_type;
        cmds_output->cmds[i].header.delay_ms = cmds_input[i].delay_ms;
        cmds_output->cmds[i].header.payload_length = cmds_input[i].payload_length;
        cmds_output->cmds[i].payload = cmds_input[i].payload;
    }

    return RT_EOK;
}

/**
 * @brief  Panel command init.
 * @param  state: display state.
 */
static void rockchip_panel_cmd_init(struct display_state *state)
{
    struct panel_state *panel_state = &state->panel_state;
    uint16_t cmd_cnt;

    cmd_cnt = HAL_ARRAY_SIZE(cmd_on);
    if (rockchip_panel_parse_cmd(state, cmd_on, cmd_cnt, &panel_state->on_cmds))
        rt_kprintf("rockchip panel parse cmd on failed!\n");

    cmd_cnt = HAL_ARRAY_SIZE(cmd_off);
    if (rockchip_panel_parse_cmd(state, cmd_off, cmd_cnt, &panel_state->off_cmds))
        rt_kprintf("rockchip panel parse cmd off failed!\n");

#ifdef RT_HW_LCD_SUPPORT_AOD_MODE
    cmd_cnt = HAL_ARRAY_SIZE(cmd_aod_on);
    if (rockchip_panel_parse_cmd(state, cmd_aod_on, cmd_cnt, &panel_state->aod_on_cmds))
        rt_kprintf("rockchip panel parse cmd aod_on failed!\n");

    cmd_cnt = HAL_ARRAY_SIZE(cmd_aod_off);
    if (rockchip_panel_parse_cmd(state, cmd_aod_off, cmd_cnt, &panel_state->aod_off_cmds))
        rt_kprintf("rockchip panel parse cmd aod_off failed!\n");
#endif
}

/**
 * @brief  Panel init.
 * @param  state: display state.
 */
static void rockchip_panel_init(struct display_state *state)
{
    struct panel_state *panel_state = &state->panel_state;

    panel_state->vmode.xres = RT_HW_LCD_XRES;
    panel_state->vmode.yres = RT_HW_LCD_YRES;
    panel_state->vmode.pixclock = RT_HW_LCD_PIXEL_CLOCK;
    panel_state->vmode.lanembps = RT_HW_LCD_LANE_MBPS;
    panel_state->vmode.left_margin = RT_HW_LCD_LEFT_MARGIN;
    panel_state->vmode.right_margin = RT_HW_LCD_RIGHT_MARGIN;
    panel_state->vmode.upper_margin = RT_HW_LCD_UPPER_MARGIN;
    panel_state->vmode.lower_margin = RT_HW_LCD_LOWER_MARGIN;
    panel_state->vmode.hsync_len = RT_HW_LCD_HSYNC_LEN;
    panel_state->vmode.vsync_len = RT_HW_LCD_VSYNC_LEN;
    panel_state->vmode.flag = RT_HW_LCD_VMODE_FLAG;
    panel_state->bus_format = RT_HW_LCD_BUS_FORMAT;
    panel_state->conn_type = RT_HW_LCD_CONN_TYPE;
    panel_state->cmd_type = RT_HW_LCD_INIT_CMD_TYPE;
    panel_state->display_mode = RT_HW_LCD_DISPLAY_MODE;
    panel_state->area_display = RT_HW_LCD_AREA_DISPLAY;
#ifdef RT_HW_LCD_MAX_BRIGHTNESS
    panel_state->max_brightness = RT_HW_LCD_MAX_BRIGHTNESS;
#else
    panel_state->max_brightness = 255;
#endif

    panel_state->xact_align = RT_HW_LCD_XACT_ALIGN;
    panel_state->yact_align = RT_HW_LCD_YACT_ALIGN;
    panel_state->xpos_align = RT_HW_LCD_XPOS_ALIGN;
    panel_state->ypos_align = RT_HW_LCD_YPOS_ALIGN;

#ifdef RT_HW_LCD_GPIO_TRIGGER_PIN
    panel_state->gpio_trigger_pin_info = RT_HW_LCD_GPIO_TRIGGER_PIN;
#endif

    rockchip_panel_cmd_init(state);
    /**
     * todo:
     * panel power control.
     */
}

/**
 * @brief  Panel deinit.
 * @param  state: display state.
 */
static void rockchip_panel_deinit(struct display_state *state)
{
}

/**
 * @brief  Enable panel power.
 * @param  state: display state.
 */
static void rockchip_panel_enable_power(struct display_state *state)
{
#ifdef RT_HW_LCD_POWER_EN_PIN
    rt_pin_mode(RT_HW_LCD_POWER_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(RT_HW_LCD_POWER_EN_PIN, RT_HW_LCD_POWER_EN_FLAG);
#endif
}

/**
 * @brief  Disable panel power.
 * @param  state: display state.
 */
static void rockchip_panel_disable_power(struct display_state *state)
{
#ifdef RT_HW_LCD_POWER_EN_PIN
    rt_pin_mode(RT_HW_LCD_POWER_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(RT_HW_LCD_POWER_EN_PIN, !RT_HW_LCD_POWER_EN_FLAG);
#endif
}

/**
 * @brief  Reset panel.
 * @param  state: display state.
 */
static void rockchip_panel_reset(struct display_state *state)
{
    /**
     * todo:
     * panel reset control.
     */
}

/**
 * @brief  Set panel brightness.
 * @param  state: display state.
 * @param  brightness: backlight value.
 */
static void rockchip_panel_set_brightness(struct display_state *state, uint16_t brightness)
{
    if (brightness > state->panel_state.max_brightness)
    {
        rt_kprintf("brightness is out of range\n");
        return;
    }

    state->panel_state.brightness = brightness;
#ifdef RT_HW_LCD_DCS_BACKLIGHT
    if (state->conn_state.funcs->set_brightness(state, brightness))
        rt_kprintf("failed to set dcs brightness %d\n", brightness);
#endif

}

/**
 * @brief  Send spi command.
 * @param  state: display state.
 * @param  cmds: init command.
 */
static int rockchip_panel_send_spi_cmds(struct display_state *state,
                                        struct rockchip_panel_cmds *cmds)
{
    /**
     * todo
     */

    return RT_EOK;
}

/**
 * @brief  Send mcu command.
 * @param  state: display state.
 * @param  cmds: init command.
 */
static int rockchip_panel_send_mcu_cmds(struct display_state *state,
                                        struct rockchip_panel_cmds *cmds)
{
    uint16_t i;

    if (!cmds)
        return -RT_EINVAL;

    state->crtc_state.funcs->send_mcu_cmd(state, MCU_SETBYPASS, 1);
    for (i = 0; i < cmds->cmd_cnt; i++)
    {
        struct rockchip_cmd_desc *desc = &cmds->cmds[i];
        int value = 0;

        value = desc->payload[0];
        state->crtc_state.funcs->send_mcu_cmd(state, desc->header.data_type, value);
        if (desc->header.delay_ms)
            HAL_DelayMs(desc->header.delay_ms);
    }
    state->crtc_state.funcs->send_mcu_cmd(state, MCU_SETBYPASS, 0);

    return RT_EOK;
}

/**
 * @brief  Send dsi command.
 * @param  state: display state.
 * @param  cmds: init command.
 */
static int rockchip_panel_send_dsi_cmds(struct display_state *state,
                                        struct rockchip_panel_cmds *cmds)
{
    uint16_t i;
    rt_err_t ret = RT_EOK;
    struct connector_state *conn_state = &state->conn_state;

    if (!cmds)
        return -RT_EINVAL;

    for (i = 0; i < cmds->cmd_cnt; i++)
    {
        struct rockchip_cmd_desc *desc = &cmds->cmds[i];
        ret = conn_state->funcs->transfer(state, desc);
        if (ret)
            return ret;

        if (desc->header.delay_ms)
            HAL_DelayMs(desc->header.delay_ms);
    }

    return RT_EOK;
}

/**
 * @brief  Set panel aod mode on/off.
 * @param  state: display state.
 * @param  on: enter or exit aod mode.
 */
static void rockchip_panel_set_aod(struct display_state *state, bool on)
{
#ifdef RT_HW_LCD_SUPPORT_AOD_MODE
    rt_err_t ret = RT_EOK;

    if (on)
    {
        ret = rockchip_panel_send_dsi_cmds(state, &state->panel_state.aod_on_cmds);
    }
    else
    {
        ret = rockchip_panel_send_dsi_cmds(state, &state->panel_state.aod_off_cmds);
    }

    if (ret)
        rt_kprintf("failed to send set aod %d cmds\n", ret);
#endif
}

/**
 * @brief  Panel prepare.
 * @param  state: display state.
 */
static void rockchip_panel_prepare(struct display_state *state)
{
    struct panel_state *panel_state = &state->panel_state;
    int ret = 0;

    if (panel_state->prepared == true)
        return;

    rockchip_panel_enable_power(state);
    rockchip_panel_reset(state);
    rt_thread_delay(10);
    if (&panel_state->on_cmds)
    {
        if (panel_state->conn_type == RK_DISPLAY_CONNECTOR_DSI)
            ret = rockchip_panel_send_dsi_cmds(state, &panel_state->on_cmds);
        else if (panel_state->cmd_type == CMD_TYPE_SPI)
            ret = rockchip_panel_send_spi_cmds(state, &panel_state->on_cmds);
        if (ret)
            rt_kprintf("failed to send on cmds: %d\n", ret);
    }

    panel_state->prepared = true;
}

/**
 * @brief  Panel unprepare.
 * @param  state: display state.
 */
static void rockchip_panel_unprepare(struct display_state *state)
{
    struct panel_state *panel_state = &state->panel_state;
    int ret = 0;

    if (panel_state->prepared == false)
        return;

    rt_thread_delay(10);
    if (&panel_state->off_cmds)
    {
        if (panel_state->conn_type == RK_DISPLAY_CONNECTOR_DSI)
            ret = rockchip_panel_send_dsi_cmds(state, &panel_state->off_cmds);
        else if (panel_state->cmd_type == CMD_TYPE_SPI)
            ret = rockchip_panel_send_spi_cmds(state, &panel_state->off_cmds);
        if (ret)
            rt_kprintf("failed to send off cmds: %d\n", ret);
    }

    rockchip_panel_disable_power(state);

    panel_state->prepared = false;
}

/**
 * @brief  Panel enable.
 * @param  state: display state.
 */
static void rockchip_panel_enable(struct display_state *state)
{
    struct panel_state *panel_state = &state->panel_state;

    if (panel_state->enabled == true)
        return;

    /**
     * mcu cmd is send through crtc, so move here after crtc is enabled.
     */
    if (panel_state->cmd_type == CMD_TYPE_MCU)
        if (rockchip_panel_send_mcu_cmds(state, &panel_state->on_cmds))
            rt_kprintf("failed to send on cmds\n");

    /**
     * todo: enable backlight
     */

    panel_state->enabled = true;
}

/**
 * @brief  Panel disable.
 * @param  state: display state.
 */
static void rockchip_panel_disable(struct display_state *state)
{
    struct panel_state *panel_state = &state->panel_state;

    if (panel_state->enabled == false)
        return;

    /**
     * todo: disable backlight
     */

    /**
     * mcu cmd is send through crtc, so move here before crtc is disabled.
     */
    if (panel_state->cmd_type == CMD_TYPE_MCU)
        if (rockchip_panel_send_mcu_cmds(state, &panel_state->off_cmds))
            rt_kprintf("failed to send off cmds\n");

    panel_state->enabled = false;
}
/** @} */

/** @defgroup Panel_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  panel control functions.
 * @param  init: Init panel hardware config.
 * @param  deinit: Panel deinit.
 * @param  prepare: Prepare panel.
 * @param  unprepare: Unprepare panel.
 * @param  enable: Enable panel.
 * @param  disable: Disable panel.
 */

const struct rockchip_panel_funcs rockchip_panel_funcs =
{
    .init = rockchip_panel_init,
    .deinit = rockchip_panel_deinit,
    .prepare = rockchip_panel_prepare,
    .unprepare = rockchip_panel_unprepare,
    .enable = rockchip_panel_enable,
    .disable = rockchip_panel_disable,
    .set_aod = rockchip_panel_set_aod,
    .update_bl = rockchip_panel_set_brightness,
};
/** @} */

#endif

/** @} */

/** @} */
