/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_display.h
  * @version V0.1
  * @brief   display driver for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */

#ifndef __DRV_DISPLAY_H__
#define __DRV_DISPLAY_H__

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"

#ifdef RT_USING_VOP

#include "drv_panel.h"
#include "drv_panel_cfg.h"

/*******************************************************************************
 * Pre-processor Definitions
 ******************************************************************************/
#define DISPLAY_DEBUG 0

#if DISPLAY_DEBUG
#define display_dbg(dev, fmt, ...) \
    rt_kprintf("[%s]: " fmt, ((struct rt_device *)dev)->parent.name, ##__VA_ARGS__)
#else
#define display_dbg(dev, fmt, ...) \
do { \
} while(0)
#endif

#define RK_DISPLAY_CTRL_AP_COP_MODE         0x1000
#define RK_DISPLAY_CTRL_COMMIT              0x1001
#define RK_DISPLAY_CTRL_DISABLE             0x1002
#define RK_DISPLAY_CTRL_DISPLAY_SYNC        0x1003
#define RK_DISPLAY_CTRL_ENABLE              0x1004
#define RK_DISPLAY_CTRL_LOAD_LUT            0x1005
#define RK_DISPLAY_CTRL_MIPI_SWITCH         0x1006
#define RK_DISPLAY_CTRL_SET_AOD_OFF         0x1007
#define RK_DISPLAY_CTRL_SET_AOD_ON          0x1008
#define RK_DISPLAY_CTRL_SET_BCSH            0x1009
#define RK_DISPLAY_CTRL_SET_COLOR_MATRIX    0x100a
#define RK_DISPLAY_CTRL_SET_GAMMA_COE       0x100b
#define RK_DISPLAY_CTRL_SET_PLANE           0x100c
#define RK_DISPLAY_CTRL_SET_POST_CLIP       0x100d
#define RK_DISPLAY_CTRL_SET_SCALE           0x100e
#define RK_DISPLAY_CTRL_UPDATE_BL           0x100f
#define RK_DISPLAY_CTRL_CLOSE_WIN           0x1011
#define RK_DISPLAY_CTRL_POST_PLANE          0x1012
#define RK_DISPLAY_CTRL_DISABLE_LUT         0x1013

#define RK_DISPLAY_MAX_CRTC_WIN_NUM         3
#define RK_DISPLAY_TIME_OUT                 50

enum rockchip_cmd_type
{
    CMD_TYPE_DEFAULT,
    CMD_TYPE_SPI,
    CMD_TYPE_MCU
};

enum rockchip_display_mode
{
    DISPLAY_VIDEO_MODE,
    DISPLAY_CMD_MODE,
};

enum rockchip_area_display_mode
{
    DISABLE_AREA_DISPLAY,
    ENABLE_AREA_DISPLAY,
};

enum rockchip_display_sync_mode
{
    DISPLAY_SYNC_MODE,
    DISPLAY_ASYNC_MODE,
};

enum rockchip_display_sync_cmd
{
    DISPLAY_SET_ASYNC_MODE,
    DISPLAY_SYNC,
};
/*******************************************************************************
 *Private Variable Definition
 ******************************************************************************/
struct display_state;

/*******************************************************************************
 * Public Types
 ******************************************************************************/
struct video_mode
{
    uint16_t xres;
    uint16_t yres;
    uint32_t pixclock;
    uint16_t lanembps;
    uint16_t left_margin;
    uint16_t right_margin;
    uint16_t upper_margin;
    uint16_t lower_margin;
    uint16_t hsync_len;
    uint16_t vsync_len;
    uint16_t flag;
};

struct rockchip_crtc_funcs
{
    void (*init)(struct display_state *state);
    void (*deinit)(struct display_state *state);
    void (*enable)(struct display_state *state);
    void (*disable)(struct display_state *state);

    void (*set_plane)(struct display_state *state, struct CRTC_WIN_STATE *win_state);
    void (*set_scale)(struct display_state *state);
    void (*set_area)(struct display_state *state, struct DISPLAY_RECT *display_rect);
    void (*commit)(struct display_state *state);
    void (*load_lut)(struct display_state *state, uint8_t winId,
                     uint32_t *lut, uint16_t lut_size);
    void (*disable_lut)(struct display_state *state, uint8_t winId);

    void (*set_color_matrix_coe)(struct display_state *state,
                                 struct VOP_COLOR_MATRIX_INFO *color_matrix_info);
    void (*set_gamma_coe)(struct display_state *state,
                          struct VOP_GAMMA_COE_INFO *gamma_info);
    void (*set_bcsh)(struct display_state *state,
                     struct VOP_BCSH_INFO *bcsh_info);
    void (*set_post_clip)(struct display_state *state,
                          struct VOP_POST_CLIP_INFO *clip_info);
    void (*mipi_switch)(struct display_state *state, eVOP_MipiSwitchPath path);
    void (*send_mcu_cmd)(struct display_state *state, uint8_t type, uint32_t value);
    void (*enable_dsp_hold_mode)(struct display_state *state, uint8_t enable);
    int (*display_sync)(struct display_state *state);

    void (*power_on)(struct display_state *state);
    void (*power_off)(struct display_state *state);
};

struct crtc_lut_state
{
    uint8_t win_id;
    uint16_t lut_size;
    uint32_t *lut;
};

struct crtc_state
{
    struct VOP_REG *hw_base;
    const struct rockchip_crtc_funcs *funcs;
    struct CRTC_WIN_STATE win_state[RK_DISPLAY_MAX_CRTC_WIN_NUM];
    struct VOP_POST_SCALE_INFO post_scale;

    uint8_t irqno;
    rt_event_t frm_fsh_event;
    bool wait_frm_fsh;
};

struct rockchip_connector_funcs
{
    /*
     * init connector, prepare resource to ensure
     * detect and get_timing can works
     */
    void (*init)(struct display_state *state);
    void (*deinit)(struct display_state *state);
    void (*prepare)(struct display_state *state);
    void (*enable)(struct display_state *state);
    void (*disable)(struct display_state *state);
    void (*update_mode)(struct display_state *state, struct DISPLAY_MODE_INFO *mode);
    void (*set_area)(struct display_state *state, struct DISPLAY_RECT *display_rect);
    rt_err_t (*transfer)(struct display_state *state, struct rockchip_cmd_desc *desc);
    rt_err_t (*set_brightness)(struct display_state *state, uint16_t brightness);
};

struct connector_state
{
    struct DSI_REG *hw_base;
    const struct rockchip_connector_funcs *funcs;
    bool enabled;
};

struct rockchip_panel_funcs
{
    void (*init)(struct display_state *state);
    void (*deinit)(struct display_state *state);
    void (*prepare)(struct display_state *state);
    void (*unprepare)(struct display_state *state);
    void (*enable)(struct display_state *state);
    void (*disable)(struct display_state *state);
    void (*update_bl)(struct display_state *state, uint16_t brightness);
    void (*set_aod)(struct display_state *state, bool on);
};

struct panel_state
{
    bool prepared;
    bool enabled;
    const struct rockchip_panel_funcs *funcs;
    struct video_mode vmode;
    uint16_t bus_format;
    uint16_t conn_type;
    uint8_t cmd_type;       /* use mipi/mcu/spi to init panel */
    uint8_t display_mode;   /* command mode or video mode */
    uint8_t area_display;   /* enable area display or not */
    uint16_t max_brightness;
    uint16_t brightness;

    uint16_t xact_align;
    uint16_t yact_align;
    uint16_t xpos_align;
    uint16_t ypos_align;
    /* gpio connect to panel TE pin, default use te TE trigger new frame, otherwise use gpio trigger*/
    uint32_t gpio_trigger_pin_info;

    struct rockchip_panel_cmds on_cmds;
    struct rockchip_panel_cmds off_cmds;
    struct rockchip_panel_cmds aod_on_cmds;
    struct rockchip_panel_cmds aod_off_cmds;
};

struct display_sync
{
    uint8_t cmd;
    uint8_t winid;
    uint32_t addr;
};

struct display_state
{
    struct rt_device_graphic_info graphic_info;
    struct rt_device lcd;
    uint32_t *rtt_framebuffer;

    struct crtc_state crtc_state;
    struct connector_state conn_state;
    struct panel_state panel_state;
    struct DISPLAY_MODE_INFO mode;
    struct rt_mutex display_lock;
    struct rt_workqueue *isr_workqueue;
    struct rt_work work;
    struct rt_event event;
    uint8_t AP_COP_mode;
#if DISPLAY_DEBUG
    bool enabled;
    bool inited;
#endif
    bool te_irq;
    rt_event_t te_event;
};

/*******************************************************************************
 * Public Data
 ******************************************************************************/

/*******************************************************************************
 * Inline Functions
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
void rk_display_vmode_to_drm_mode(struct video_mode *mode_input,
                                  struct DISPLAY_MODE_INFO *mode_output,
                                  uint16_t area_w, uint16_t area_h);

/* The `const' in roundup() prevents gcc-3.3 from calling __divdi3 */
#define roundup(x, y) (                 \
{                           \
    const typeof(y) __y = y;            \
    (((x) + (__y - 1)) / __y) * __y;        \
}                           \
)
#define rounddown(x, y) (               \
{                           \
    typeof(x) __x = (x);                \
    __x - (__x % (y));              \
}                           \
)
#define IS_ALIGNED(x, a)        ((((x) & ((typeof(x))(a) - 1)) == 0) || \
                ((x) == (a)))

#endif
#endif /* __DRV_DISPLAY_H__ */
