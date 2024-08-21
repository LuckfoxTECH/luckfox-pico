/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-04     Jair Wu      First version
 *
 */

#include "recpen.h"
#if defined(RT_USING_RECORDING_PEN_APP)
#include "init_page.h"
#include "main_page.h"
#include "recording_page.h"
#include "setting_page.h"
#include "setting_date.h"
#include "setting_language.h"
#include "setting_backlight.h"
#include "setting_playmode.h"
#include "setting_record.h"
#include "setting_rec_seglen.h"
#include "udisk_page.h"

#define DBG_SECTION_NAME    "DISPLAY"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#ifdef RT_USING_PMIC
#include "drv_pmic.h"

rt_uint32_t g_screen_off = 0;

void disp_irq_hook(void)
{
    if (g_screen_off == 1)
    {
        rt_pm_request(1);
        g_screen_off = 0;
    }
}
#endif

/* disp_event define */
#define EVENT_REFR_UPDATE           (0x01UL << 0)
#define EVENT_REFR_DONE             (0x01UL << 1)

struct recpen_disp_data
{
    rt_display_data_t disp;
    rt_event_t  disp_event;
    rt_uint32_t cmd;
#ifdef RT_USING_VOP
    rt_uint8_t *fb;
    rt_uint32_t fblen;
#endif
};
static struct recpen_disp_data *g_recdisp_data = RT_NULL;

lv_style_t page_common_style;
lv_style_t label_style_font20;
lv_style_t label_style_font20_letter;
lv_style_t label_style_font20_hl;
lv_style_t label_style_font20_sel;
lv_style_t label_style_font16;
lv_style_t style_bar;

/*
 **************************************************************************************************
 *
 * lvgl GUI sub functions: Win layer0.
 *
 **************************************************************************************************
 */
struct lvgl_malloc_list
{
    struct lvgl_malloc_list *next;
    uint8_t *buf;
};

static struct lvgl_malloc_list lvgl_malloc_head =
{
    .next = &lvgl_malloc_head,
    .buf  = RT_NULL,
};

struct lvgl_page_func
{
    char *name;
    void (*init)(int p_page);
    void (*deinit)(void);
    void (*refresh)(size_t type, size_t value);
    void (*key)(size_t type, size_t value);
};

struct lvgl_page_func page_func[MENU_MAX_PAGE] =
{
    {"MENU_WELCOME_PAGE", welcome_page_init, welcome_page_deinit, welcome_page_refresh, NULL},
    {"MENU_MAIN_PAGE", main_page_init, main_page_deinit, main_page_refresh, main_page_key},
    {"MENU_SETTING_PAGE", setting_page_init, setting_page_deinit, NULL, setting_page_key},
    {"MENU_SETTING_DATE_PAGE", setting_date_init, setting_date_deinit, NULL, setting_date_key},
    {"MENU_SETTING_LANGUAGE_PAGE", setting_language_init, setting_language_deinit, NULL, setting_language_key},
    {"MENU_SETTING_BACKLIGHT_PAGE", setting_backlight_init, setting_backlight_deinit, NULL, setting_backlight_key},
    {"MENU_SETTING_BL_TIMEOUT_PAGE", setting_bl_timeout_init, setting_bl_timeout_deinit, NULL, setting_bl_timeout_key},
    {"MENU_SETTING_BL_BRIGHTNESS_PAGE", setting_brightness_init, setting_brightness_deinit, NULL, setting_brightness_key},
    {"MENU_SETTING_PLAYMODE_PAGE", setting_playmode_init, setting_playmode_deinit, NULL, setting_playmode_key},
    {"MENU_SETTING_RECORD_PAGE", setting_record_init, setting_record_deinit, NULL, setting_record_key},
    {"MENU_SETTING_REC_SEGLEN_PAGE", setting_seglen_init, setting_seglen_deinit, NULL, setting_seglen_key},
    {"MENU_REC_PAGE", recording_page_init, recording_page_deinit, recording_page_refresh, recording_page_key},
    {"MENU_UDISK_PAGE", udisk_page_init, udisk_page_deinit, NULL, NULL},
    /* WIP */
    {"MENU_FILE_PAGE", NULL, NULL, NULL, NULL},
};
/**
 * malloc large buffer for image data, and list buffer to a list_head
 */
struct lvgl_malloc_list *lvgl_malloc_add_list(rt_size_t size)
{
    struct lvgl_malloc_list *head = &lvgl_malloc_head;

    rt_uint8_t *pbuf = (rt_uint8_t *)rt_malloc_large(size);
    if (pbuf == RT_NULL)
    {
        return RT_NULL;
    }
    rt_memset((void *)pbuf, 0, size);

    struct lvgl_malloc_list *new = (struct lvgl_malloc_list *)rt_malloc(sizeof(struct lvgl_malloc_list));
    RT_ASSERT(new != RT_NULL);

    new->next = head->next;
    new->buf  = pbuf;
    head->next = new;

    return new;
}

/**
 * free malloced large buffer, and remove buffer to a list_head
 */
static void lvgl_free_remove_list(struct lvgl_malloc_list *new)
{
    struct lvgl_malloc_list *head = &lvgl_malloc_head;

    RT_ASSERT(new !=  &lvgl_malloc_head);
    while (head->next != &lvgl_malloc_head)
    {
        if (head->next == new)
        {
            head->next = new->next;
            break;
        }
        head = head->next;
    }

    RT_ASSERT(new->buf !=  RT_NULL);
    rt_free_large(new->buf);
    rt_free(new);
}

/**
 * free all malloced large buffer, and clear list_head
 */
static void lvgl_free_remove_all(void)
{
    struct lvgl_malloc_list *head;

    while (lvgl_malloc_head.next != &lvgl_malloc_head)
    {
        head = lvgl_malloc_head.next;
        lvgl_malloc_head.next = head->next;

        RT_ASSERT(head->buf != RT_NULL);
        rt_free_large(head->buf);
        rt_free(head);
    }
}

/**
 * Init lvgl_dsc info by image_info
 */
void *lvgl_img_dsc_init(lv_img_dsc_t *dsc, image_info_t *info)
{
    dsc->header.always_zero = 0;
    dsc->header.w = info->w;
    dsc->header.h = info->h;
    dsc->data_size = (info->w * info->h) * LV_COLOR_SIZE / 8;
    dsc->header.cf = LV_IMG_CF_TRUE_COLOR;
    dsc->data = info->data;

    if (info->type == IMG_TYPE_COMPRESS)
    {
        struct lvgl_malloc_list *new = lvgl_malloc_add_list(dsc->data_size);
        RT_ASSERT(new != RT_NULL);
        rt_display_img_fill(info, new->buf, info->w, 0, 0);

        dsc->data = new->buf;

        return new;
    }

    return RT_NULL;
}

/**
 * deinit lvgl_dsc info & free malloced buffer by lvgl_img_dsc_init()
 */
RT_UNUSED static void lvgl_img_dsc_deinit(lv_img_dsc_t *dsc, void *parm)
{
    lvgl_free_remove_list((struct lvgl_malloc_list *)parm);

    dsc->data = RT_NULL;
}

/**
 * dfree all malloced buffer by lvgl_img_dsc_init()
 */
static void lvgl_img_dsc_remove_all(void)
{
    lvgl_free_remove_all();
}

/**
 * Hook function for lvgl GUI, used for wait data transfer finish.
 */
uint32_t lv_refr_areas_wait_hook(void *par)
{
    return RT_EOK;
}

/**
 * Hook function for lvgl GUI, used for start transfer data to LCD
 */
uint32_t lv_refr_areas_update_hook(void *par)
{
    rt_err_t ret = RT_EOK;


    return ret;
}

static void styles_init(void)
{
    lv_style_init(&page_common_style);
    lv_style_set_bg_color(&page_common_style, LV_STATE_DEFAULT, RECPEN_COLOR_BG);
    lv_style_set_bg_grad_color(&page_common_style, LV_STATE_DEFAULT, RECPEN_COLOR_BG);
    lv_style_set_border_width(&page_common_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&page_common_style, LV_STATE_DEFAULT, LABEL_Y_GAP / 2);
    lv_style_set_pad_bottom(&page_common_style, LV_STATE_DEFAULT, LABEL_Y_GAP / 2);
    lv_style_set_text_color(&page_common_style, LV_STATE_DEFAULT, RECPEN_COLOR_TEXT);
    lv_style_set_text_font(&page_common_style, LV_STATE_DEFAULT, &lv_font_20);

    lv_style_init(&label_style_font20_letter);
    lv_style_set_bg_color(&label_style_font20_letter, LV_STATE_DEFAULT, RECPEN_COLOR_BG);
    lv_style_set_bg_grad_color(&label_style_font20_letter, LV_STATE_DEFAULT, RECPEN_COLOR_BG);
    lv_style_set_border_width(&label_style_font20_letter, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&label_style_font20_letter, LV_STATE_DEFAULT, LABEL_Y_GAP / 2);
    lv_style_set_pad_bottom(&label_style_font20_letter, LV_STATE_DEFAULT, LABEL_Y_GAP / 2);
    lv_style_set_text_color(&label_style_font20_letter, LV_STATE_DEFAULT, RECPEN_COLOR_TEXT_HL);
    lv_style_set_text_font(&label_style_font20_letter, LV_STATE_DEFAULT, &lv_font_montserrat_20);

    lv_style_init(&label_style_font20);
    lv_style_copy(&label_style_font20, &page_common_style);

    lv_style_init(&label_style_font20_hl);
    lv_style_copy(&label_style_font20_hl, &label_style_font20);
    lv_style_set_text_color(&label_style_font20_hl, LV_STATE_DEFAULT, RECPEN_COLOR_TEXT_HL);

    lv_style_init(&label_style_font20_sel);
    lv_style_copy(&label_style_font20_sel, &label_style_font20);
    lv_style_set_bg_opa(&label_style_font20_sel, LV_STATE_DEFAULT, LV_OPA_50);
    lv_style_set_bg_color(&label_style_font20_sel, LV_STATE_DEFAULT, RECPEN_COLOR_BAR);
    lv_style_set_bg_grad_color(&label_style_font20_sel, LV_STATE_DEFAULT, RECPEN_COLOR_BAR_2);

    lv_style_init(&label_style_font16);
    lv_style_copy(&label_style_font16, &label_style_font20);
    lv_style_set_text_font(&label_style_font16, LV_STATE_DEFAULT, &lv_font_16);

    lv_style_init(&style_bar);
    lv_style_copy(&style_bar, &page_common_style);
    lv_style_set_bg_color(&style_bar, LV_STATE_DEFAULT, RECPEN_COLOR_BAR);
    lv_style_set_bg_grad_color(&style_bar, LV_STATE_DEFAULT, RECPEN_COLOR_BAR_2);
}

rt_err_t display_page(int p_page)
{
    struct recpen_app_msg send_msg;
    int ret;

    if (p_page != MENU_NONE_PAGE)
    {
        send_msg.type = TYPE_SHOW_MENU;
        send_msg.value = p_page;
        ret = rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
        if (ret == -RT_EFULL)
        {
            LOG_W("Msg queue full\n");
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

#ifdef RT_USING_VOP
/**
 * hook function for lvgl set graphic info.
 */
rt_err_t lv_set_graphic_info(struct rt_device_graphic_info *info)
{
    struct display_state *state = (struct display_state *)g_recdisp_data->disp->device->user_data;
    struct rt_device_graphic_info *graphic_info = &state->graphic_info;

    graphic_info->framebuffer = g_recdisp_data->fb;
    memcpy(info, graphic_info, sizeof(struct rt_device_graphic_info));

    return RT_EOK;
}

/**
 * rk iot display refresh request: request send data to LCD pannel.
 */
static rt_err_t recpen_disp_refr_done(void)
{
    return (rt_event_send(g_recdisp_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t recpen_disp_refr_request(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;

    //request refresh display data to Pannel
    disp_mq->disp_finish = recpen_disp_refr_done;
    ret = rt_mq_send(g_recdisp_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_recdisp_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * rkiot display task.
 */
static rt_err_t lcd_refr_request(void)
{
    struct rt_display_mq_t disp_mq;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    disp_mq.win[0].zpos = WIN_MIDDLE_LAYER;
    disp_mq.win[0].winId  = WIN_MIDDLE_LAYER;
    disp_mq.win[0].format = RTGRAPHIC_PIXEL_FORMAT_RGB332;
    disp_mq.win[0].lut    = bpp_lut;
    disp_mq.win[0].lutsize = sizeof(bpp_lut) / sizeof(bpp_lut[0]);
    disp_mq.win[0].fb    = g_recdisp_data->fb;
    disp_mq.win[0].fblen = g_recdisp_data->fblen;
    disp_mq.win[0].x     = 0;
    disp_mq.win[0].y     = 0;
    disp_mq.win[0].ylast = disp_mq.win[0].y;
    disp_mq.win[0].w     = SCREEN_W;
    disp_mq.win[0].h     = SCREEN_H;

    RT_ASSERT((disp_mq.win[0].w % 4) == 0);
    RT_ASSERT((disp_mq.win[0].y % 2) == 0);
    RT_ASSERT((disp_mq.win[0].h % 2) == 0);

    disp_mq.cfgsta = (0x01 << 0);

    recpen_disp_refr_request(&disp_mq);

    return RT_EOK;
}
#else
static rt_err_t lcd_refr_done(void)
{
    return (rt_event_send(g_recdisp_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t lcd_refr_request(void)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;
    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));

    //request refresh display data to Pannel
    disp_mq.disp_finish = lcd_refr_done;
    disp_mq.cfgsta |= (0x01 << 0);
    ret = rt_mq_send(g_recdisp_data->disp->disp_mq, &disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_recdisp_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}
#endif

static void display_task(void *arg)
{
    rt_err_t ret;
    struct recpen_app_msg rcv_msg;
    struct recpen_disp_data *recdisp_data;
    int backlight = 0;

    g_recdisp_data = recdisp_data = (struct recpen_disp_data *)rt_malloc(sizeof(struct recpen_disp_data));
    RT_ASSERT(recdisp_data != RT_NULL);
    rt_memset((void *)recdisp_data, 0, sizeof(struct recpen_disp_data));

    recdisp_data->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(recdisp_data->disp_event != RT_NULL);

    recdisp_data->disp = rt_display_get_disp();
    RT_ASSERT(recdisp_data->disp != RT_NULL);

#ifdef RT_USING_VOP
    recdisp_data->fblen = SCREEN_W * SCREEN_H;
    recdisp_data->fb = (rt_uint8_t *)rt_malloc_large(recdisp_data->fblen);
#endif

    /* init littlevGL */
    ret = littlevgl2rtt_init(DISPLAY_DEVICE_NAME);
    RT_ASSERT(ret == RT_EOK);

    styles_init();
    g_app_info.now_page = MENU_WELCOME_PAGE;
    page_func[g_app_info.now_page].init(MENU_NONE_PAGE);

#ifdef RT_USING_PMIC
    pmic_set_primary_hook(disp_irq_hook);
#endif
    while (1)
    {
        toast_refresh();
        lv_task_handler();
        if (backlight != g_app_info.backlight)
        {
            if (g_app_info.backlight)
            {
                rt_device_control(recdisp_data->disp->device, RTGRAPHIC_CTRL_POWERON, NULL);
                lcd_refr_request();
                if (g_app_info.bl_device)
                {
                    rt_device_control(g_app_info.bl_device, RTGRAPHIC_CTRL_POWERON, NULL);
                    rt_device_control(g_app_info.bl_device,
                                      RTGRAPHIC_CTRL_RECT_UPDATE,
                                      &g_app_info.brightness);
                }
            }
            else
            {
                if (g_app_info.bl_device)
                    rt_device_control(g_app_info.bl_device, RTGRAPHIC_CTRL_POWEROFF, NULL);
                rt_device_control(recdisp_data->disp->device, RTGRAPHIC_CTRL_POWEROFF, NULL);
#ifdef RT_USING_PMIC
                if (g_app_info.state == RECPEN_APP_STATE_IDLE)
                {
                    recpen_player_stop();
                    g_screen_off = 1;
                    rt_pm_release(1);
                }
#endif
            }
            backlight = g_app_info.backlight;
        }
        else if (g_app_info.backlight)
        {
            lcd_refr_request();
        }
        lvgl_img_dsc_remove_all();
        ret = rt_mq_recv(g_app_info.display_mq, (void *)&rcv_msg, sizeof(struct recpen_app_msg), RT_WAITING_FOREVER);
        if (ret == RT_EOK)
        {
            LOG_D("Disp rev msg:%d %d", rcv_msg.type, rcv_msg.value);
            switch (rcv_msg.type)
            {
            case TYPE_SHOW_MENU:
                if (rcv_msg.value > MENU_MAX_PAGE)
                    continue;
                LOG_I("Now page %s -> %s", page_func[g_app_info.now_page].name, page_func[rcv_msg.value].name);
                if (!page_func[g_app_info.now_page].deinit || !page_func[rcv_msg.value].init)
                {
                    LOG_W("Page func null:%p -> %p, ignore this msg", page_func[g_app_info.now_page].deinit, page_func[rcv_msg.value].init);
                    break;
                }
                page_func[g_app_info.now_page].deinit();
                page_func[rcv_msg.value].init(g_app_info.now_page);
                g_app_info.now_page = rcv_msg.value;
                break;
            case TYPE_KEY_PRESS:
                if (g_app_info.bl_timer)
                    rt_timer_start(g_app_info.bl_timer);
                if (g_app_info.backlight == 0)
                {
                    /* Key is invalid if backlight off */
                    g_app_info.backlight = 1;
                    break;
                }
                if (dialog_box_key(rcv_msg.type, rcv_msg.value) == DIALOG_BOX_EXIST)
                    break;
                if (page_func[g_app_info.now_page].key)
                    page_func[g_app_info.now_page].key(rcv_msg.type, rcv_msg.value);
                break;
            default:
                if (rcv_msg.type == TYPE_REFRESH && rcv_msg.value == REFRESH_DISP_TICK)
                    break;
                if (page_func[g_app_info.now_page].refresh)
                    page_func[g_app_info.now_page].refresh(rcv_msg.type, rcv_msg.value);
                break;
            }
        }
    }

    littlevgl2rtt_deinit();

    rt_event_delete(recdisp_data->disp_event);

    rt_free(recdisp_data);
}

rt_err_t recpen_display_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("rp_disp",
                           display_task,
                           RT_NULL,
                           RECPEN_DISPLAY_TASK_STACK_SIZE,
                           RECPEN_DISPLAY_TASK_PRIORITY,
                           10);

    if (tid != RT_NULL)
    {
        LOG_D("Create recpen_disp OK");
        rt_thread_startup(tid);
        return RT_EOK;
    }

    LOG_E("Create recpen_disp failed");
    RT_ASSERT(0);

    return -RT_ERROR;
}

#endif
