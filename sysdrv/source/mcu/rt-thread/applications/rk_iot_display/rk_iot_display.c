#include <rtthread.h>
#if defined(RT_USING_IOT_DISPLAY) && defined(RT_USING_LITTLEVGL2RTT)
#include <stdio.h>
#include <math.h>

#include <littlevgl2rtt.h>
#include <lvgl/lvgl.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"
#include "rk_iot_display.h"

/**
 * color palette for 1bpp
 */
static uint32_t bpp1_lut[2] =
{
    0x00000000, 0x00ffffff
};

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

/*
 *  lvgl config parm as below:
 *
 *  RT_LV_COLOR_DEPTH   8
 *  LV_HOR_RES          240
 *  LV_VER_RES          240
 */

/* display win layers */
#define LVGL_HOME_GRAY1_WIN     0
#define LVGL_CLOCK_RGB332_WIN   1
#define LVGL_LOGO_RGB565_WIN    2

#define RKIOT_MAX_XRES      240
#define RKIOT_MAX_YRES      320

/* display region define */
#define LOGO_REGION_X       0
#define LOGO_REGION_Y       0
#define LOGO_REGION_H       20
#define LOGO_WIN_PIXELS     16

#define LVGL_REGION_X       0
#define LVGL_REGION_Y       (LOGO_REGION_Y + LOGO_REGION_H)
#define LVGL_REGION_H       240
#define LVGL_FB_W           LV_HOR_RES      /* clock frame buffer w */
#define LVGL_FB_H           LV_VER_RES      /* clock frame buffer h */
#define LVGL_WIN_PIXELS     RT_LV_COLOR_DEPTH

#define HOME_REGION_X       0
#define HOME_REGION_Y       264//(LVGL_REGION_Y + LVGL_REGION_H)
#define HOME_REGION_H       (320 - HOME_REGION_Y)
#define HOME_FB_W           224 // must aligned_32
#define HOME_FB_H           48
#define HOME_WIN_PIXELS     1

/* Command define */
#define CMD_REFR_LOGO               (0x01UL << 0)
#define CMD_REFR_HOME               (0x01UL << 1)
#define CMD_REFR_LVGL               (0x01UL << 2)
#define CMD_REFR_LVGL_IOT           (0x01UL << 3)
#define CMD_REFR_LVGL_CLOCK         (0x01UL << 4)

/* Event define */
#define EVENT_REFR_UPDATE           (0x01UL << 0)
#define EVENT_REFR_DONE             (0x01UL << 1)

#define LVGL_CLOCK_UPDATE_TICKS     (RT_TICK_PER_SECOND)
#define RKIOT_LOGO_LOOP_TICKS       (RT_TICK_PER_SECOND / 100 * 3)

#define RKIOT_LOGO_LOOP_STEP        1
/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
extern image_info_t date_bkg_info;
extern image_info_t clock_bkg_info;
extern image_info_t clock_sec_info;
extern image_info_t clock_min_info;
extern image_info_t clock_hour_info;
extern image_info_t clock_logo_info;
extern image_info_t clock_home_info;
extern image_info_t content_bkg_info;

LV_FONT_DECLARE(lv_font_16);
static lv_font_t *g_lvgl_font = &lv_font_16;

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct rkiot_disp_data
{
    rt_display_data_t disp;
    rt_event_t  disp_event;
    rt_timer_t  clock_timer;
    rt_uint32_t cmd;

    rt_uint8_t  hour;
    rt_uint8_t  minute;
    rt_uint8_t  second;
    rt_uint32_t ticks;

    rt_uint8_t  month;
    rt_uint8_t  day;
    rt_uint8_t  week;

    rt_uint8_t *fb;
    rt_uint32_t fblen;

    lv_obj_t   *lv_clock;
    lv_obj_t   *lv_datebk;
    lv_obj_t   *lv_contentbk;
    lv_obj_t   *lv_date;
    char        date_str[30];
    lv_obj_t   *lv_week;
    char        week_str[30];
    lv_obj_t   *lv_content;
    char        content_str[256];
    lv_style_t *style;

    rt_uint8_t *home_fb;
    rt_uint32_t home_fblen;

    rt_timer_t  logo_loop_timer;
    rt_uint16_t logo_xLoopOffset;
};

static  struct rkiot_disp_data *g_iotdisp_data = RT_NULL;

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

static char *week_table[] = {"日", "一", "二", "三", "四", "五", "六"};

static lv_img_dsc_t clock_bkg_img_dsc;
static lv_img_dsc_t date_bkg_img_dsc;
static lv_img_dsc_t content_bkg_img_dsc;

//#define LVGL_CONTENT_TEST
#ifdef LVGL_CONTENT_TEST
static const char *asr_cmd_respond_string[] =
{
    " ",
    "已为您打开热水器",
    "已为您关闭热水器",
    "已为您调高温度",
    "已为您调低温度",
    "已为您调至45°",
    "已为您调至65°",
    "已为您调至75°",
    "已为您打开抑菌",
    "已为您关闭抑菌",
    "已为您打开晨浴",
    "已为您关闭晨浴",
    "已为您打开晚浴",
    "已为您关闭晚浴",
    "好的,已为您打开热水器",
    "已为您打开速热补水",
    "已为您关闭速热补水",
    "我在",
    "在的",
    "请吩咐",
    "您请说",
};

uint8_t testid = 0;
#endif

/*
 **************************************************************************************************
 *
 * lvgl GUI sub functions: Win layer0.
 *
 **************************************************************************************************
 */
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
static void *lvgl_img_dsc_init(lv_img_dsc_t *dsc, image_info_t *info)
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
static void lvgl_img_dsc_deinit(lv_img_dsc_t *dsc, void *parm)
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
    return RT_EOK;
}

/**
 * date region display refresh.
 */
rt_err_t lvgl_clock_content_update_callback(uint32_t cmd, void *parm, uint32_t parmlen)
{
    RT_ASSERT(parmlen < 256);

    char *str = (char *)parm;
    struct rkiot_disp_data *iotdisp_data = g_iotdisp_data;

    rt_memset(iotdisp_data->content_str, 0, sizeof(iotdisp_data->content_str));
    rt_memcpy(iotdisp_data->content_str, str, parmlen);

    iotdisp_data->cmd |= CMD_REFR_LVGL | CMD_REFR_LVGL_IOT;
    rt_event_send(iotdisp_data->disp_event, EVENT_REFR_UPDATE);

    return RT_EOK;
}

/**
 * Display clock demo timer callback.
 */
static void rkiot_disp_lvgl_timer(void *parameter)
{
    struct rkiot_disp_data *iotdisp_data = (struct rkiot_disp_data *)parameter;

    if (++iotdisp_data->second >= 60)
    {
        iotdisp_data->second = 0;
        if (++iotdisp_data->minute == 60)
        {
            iotdisp_data->minute = 0;
            if (++iotdisp_data->hour >= 24)
            {
                iotdisp_data->hour = 0;

                if (++iotdisp_data->week == 7)
                {
                    iotdisp_data->week = 0;
                }

                if (++iotdisp_data->day > 31)
                {
                    iotdisp_data->day = 1;
                    if (++iotdisp_data->month > 12)
                    {
                        iotdisp_data->month = 1;
                    }
                }
            }
        }
    }
    iotdisp_data->cmd |= CMD_REFR_LVGL | CMD_REFR_LVGL_CLOCK;
    rt_event_send(iotdisp_data->disp_event, EVENT_REFR_UPDATE);

#ifdef LVGL_CONTENT_TEST
    lvgl_clock_content_update_callback(RT_NULL, (void *)asr_cmd_respond_string[testid], rt_strlen(asr_cmd_respond_string[testid]));
    testid++;
    if (testid >= sizeof(asr_cmd_respond_string) / sizeof(char *))
    {
        testid = 0;
    }
#endif
}

/**
 * Clock region display refresh.
 */
static rt_err_t rkiot_disp_lvgl_clock_design(struct rkiot_disp_data *iotdisp_data)
{
    // draw clock
    if (1)
    {
        lv_img_dsc_t *dsc      = &clock_bkg_img_dsc;
        image_info_t *img_info = &clock_bkg_info;

        // draw clock pannel & set lvgl param
        {
            lvgl_img_dsc_init(dsc, img_info);
            lv_img_set_src(iotdisp_data->lv_clock, dsc);
            lv_obj_set_pos(iotdisp_data->lv_clock, 24, 10);
            lv_obj_set_size(iotdisp_data->lv_clock, img_info->w, img_info->h);
            lv_obj_invalidate(iotdisp_data->lv_clock);
        }

        // draw clock line
        {
            int32_t hour, angle;
            rt_uint16_t xoffset = (img_info->w / 2);
            rt_uint16_t yoffset = (img_info->h / 2);

            //draw hour line
            img_info = &clock_hour_info;
            hour = iotdisp_data->hour;
            if (hour >= 12)
            {
                hour -= 12;
            }
            angle = hour * (360 / 12) + (iotdisp_data->minute * 30) / 60 - 90;
            if (angle < 0)
            {
                angle += 360;
            }
            rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, img_info->data,
                                   (unsigned char *)((uint32_t)dsc->data + yoffset * dsc->header.w + xoffset),
                                   dsc->header.w, 2, img_info->h / 2);

            //draw min line
            img_info = &clock_min_info;
            angle = iotdisp_data->minute * (360 / 60);
            angle -= 90;
            if (angle < 0)
            {
                angle += 360;
            }
            rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, img_info->data,
                                   (unsigned char *)((uint32_t)dsc->data + yoffset * dsc->header.w + xoffset),
                                   dsc->header.w, 2, img_info->h / 2);

            //draw second line
            img_info = &clock_sec_info;
            angle = iotdisp_data->second * (360 / 60);
            angle -= 90;
            if (angle < 0)
            {
                angle += 360;
            }
            rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, img_info->data,
                                   (unsigned char *)((uint32_t)dsc->data + yoffset * dsc->header.w + xoffset),
                                   dsc->header.w, 2, img_info->h / 2);
        }
    }

    // draw date
    if (1)
    {
        // draw week & date pannel
        {
            lv_img_dsc_t *dsc      = &date_bkg_img_dsc;
            image_info_t *img_info = &date_bkg_info;
            lvgl_img_dsc_init(dsc, img_info);

            lv_area_t    area;
            lv_obj_get_coords(iotdisp_data->lv_clock, &area);
            lv_img_set_src(iotdisp_data->lv_datebk, dsc);
            lv_obj_set_pos(iotdisp_data->lv_datebk, area.x2 + 10, 20);
            lv_obj_set_size(iotdisp_data->lv_datebk, img_info->w, img_info->h);
            lv_obj_invalidate(iotdisp_data->lv_datebk);
        }

        lv_font_t  *font = g_lvgl_font;

        // set date param to lvgl
        {
            snprintf(iotdisp_data->date_str, sizeof(iotdisp_data->date_str), "%02u月%02u日",
                     iotdisp_data->month, iotdisp_data->day);
            lv_obj_set_pos(iotdisp_data->lv_date, 12, 18);
            lv_obj_set_size(iotdisp_data->lv_date, lv_obj_get_width(iotdisp_data->lv_datebk), font->line_height);
            lv_label_set_text(iotdisp_data->lv_date, iotdisp_data->date_str);
            lv_obj_invalidate(iotdisp_data->lv_date);
        }

        // set week param to lvgl
        {
            snprintf(iotdisp_data->week_str, sizeof(iotdisp_data->week_str), "星期%s",
                     week_table[iotdisp_data->week]);
            lv_obj_set_pos(iotdisp_data->lv_week, 16, 46);
            lv_obj_set_size(iotdisp_data->lv_week, lv_obj_get_width(iotdisp_data->lv_datebk), font->line_height);
            lv_label_set_text(iotdisp_data->lv_week, iotdisp_data->week_str);
            lv_obj_invalidate(iotdisp_data->lv_week);
        }
    }

    return RT_EOK;
}

/**
 * date region display refresh.
 */
static rt_err_t rkiot_lvgl_iot_design(struct rkiot_disp_data *iotdisp_data)
{
    // Draw iot background
    if (1)
    {
        lv_img_dsc_t *dsc      = &content_bkg_img_dsc;
        image_info_t *img_info = &content_bkg_info;
        lvgl_img_dsc_init(dsc, img_info);

        lv_img_set_src(iotdisp_data->lv_contentbk, dsc);
        lv_obj_set_pos(iotdisp_data->lv_contentbk, (LVGL_FB_W - img_info->w) / 2, 126);
        lv_obj_set_size(iotdisp_data->lv_contentbk, img_info->w, img_info->h);
        lv_obj_invalidate(iotdisp_data->lv_contentbk);
    }

    // Draw iot string
    if (1)
    {
        lv_font_t  *font = g_lvgl_font;

#ifndef LVGL_CONTENT_TEST
        snprintf(iotdisp_data->content_str, sizeof(iotdisp_data->content_str), "请说......");
#endif
        lv_obj_set_pos(iotdisp_data->lv_content, 12, 50);
        lv_obj_set_size(iotdisp_data->lv_content, lv_obj_get_width(iotdisp_data->lv_contentbk), font->line_height);
        lv_label_set_text(iotdisp_data->lv_content, iotdisp_data->content_str);
        lv_obj_invalidate(iotdisp_data->lv_content);
    }

    return RT_EOK;
}

/**
 * hook function for lvgl set graphic info.
 */
rt_err_t lv_set_graphic_info(struct rt_device_graphic_info *info)
{
    struct display_state *state = (struct display_state *)g_iotdisp_data->disp->device->user_data;
    struct rt_device_graphic_info *graphic_info = &state->graphic_info;

    graphic_info->framebuffer = g_iotdisp_data->fb;
    memcpy(info, graphic_info, sizeof(struct rt_device_graphic_info));

    return RT_EOK;
}

/**
 * lvgl clock dmeo init.
 */
static rt_err_t rkiot_disp_lvgl_init(struct rkiot_disp_data *iotdisp_data)
{
    rt_err_t    ret;
    rt_device_t device = iotdisp_data->disp->device;
    struct rt_device_graphic_info info;

    RT_ASSERT(LV_HOR_RES >= 240);
    RT_ASSERT(LV_VER_RES >= 240);
    RT_ASSERT(RT_LV_COLOR_DEPTH == 8);

    ret = rt_device_control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    RT_ASSERT(ret == RT_EOK);

    iotdisp_data->fblen = ((LVGL_FB_W * LVGL_FB_H + 3) / 4) * 4;
    iotdisp_data->fb     = (rt_uint8_t *)rt_malloc_large(iotdisp_data->fblen);
    RT_ASSERT(iotdisp_data->fb != RT_NULL);
    rt_memset((void *)iotdisp_data->fb, 0, iotdisp_data->fblen);

    //lvgl init
    {
        /* Var init */
        iotdisp_data->hour   = 3;
        iotdisp_data->minute = 30;
        iotdisp_data->second = 0;
        iotdisp_data->day    = 1;
        iotdisp_data->month  = 9;
        iotdisp_data->week   = 3;
        iotdisp_data->cmd   |= CMD_REFR_LVGL | CMD_REFR_LVGL_CLOCK | CMD_REFR_LVGL_IOT;

        /* init littlevGL */
        ret = littlevgl2rtt_init("lcd");
        RT_ASSERT(ret == RT_EOK);

        iotdisp_data->style = (lv_style_t *)rt_malloc(sizeof(lv_style_t));
        RT_ASSERT(iotdisp_data->style != RT_NULL);
        lv_style_init(iotdisp_data->style);
        lv_style_set_text_color(iotdisp_data->style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_style_set_text_font(iotdisp_data->style, LV_STATE_DEFAULT, g_lvgl_font);

        iotdisp_data->lv_clock = lv_img_create(lv_scr_act(), NULL);
        RT_ASSERT(iotdisp_data->lv_clock != NULL);

        iotdisp_data->lv_datebk = lv_img_create(lv_scr_act(), NULL);
        RT_ASSERT(iotdisp_data->lv_datebk != NULL);

        iotdisp_data->lv_date = lv_label_create(iotdisp_data->lv_datebk, NULL);
        RT_ASSERT(iotdisp_data->lv_date != NULL);
        lv_obj_add_style(iotdisp_data->lv_date, LV_OBJ_PART_MAIN, iotdisp_data->style);

        iotdisp_data->lv_week = lv_label_create(iotdisp_data->lv_datebk, NULL);
        RT_ASSERT(iotdisp_data->lv_week != NULL);
        lv_obj_add_style(iotdisp_data->lv_week, LV_OBJ_PART_MAIN, iotdisp_data->style);

        iotdisp_data->lv_contentbk = lv_img_create(lv_scr_act(), NULL);
        RT_ASSERT(iotdisp_data->lv_contentbk != NULL);

        iotdisp_data->lv_content = lv_label_create(iotdisp_data->lv_contentbk, NULL);
        RT_ASSERT(iotdisp_data->lv_content != NULL);
        lv_obj_add_style(iotdisp_data->lv_content, LV_OBJ_PART_MAIN, iotdisp_data->style);

        snprintf(iotdisp_data->content_str, sizeof(iotdisp_data->content_str), "请说......");
    }

    /* init timer */
    iotdisp_data->clock_timer = rt_timer_create("rkiot_lvgl_tmr",
                                rkiot_disp_lvgl_timer, (void *)iotdisp_data,
                                LVGL_CLOCK_UPDATE_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(iotdisp_data->clock_timer != RT_NULL);
    rt_timer_start(iotdisp_data->clock_timer);

    return RT_EOK;
}

/**
 * lvgl clock demo deinit.
 */
static void rkiot_disp_lvgl_deinit(struct rkiot_disp_data *iotdisp_data)
{
    rt_err_t ret;
    rt_timer_stop(iotdisp_data->clock_timer);
    ret = rt_timer_delete(iotdisp_data->clock_timer);
    RT_ASSERT(ret == RT_EOK);

    lv_obj_remove_style(iotdisp_data->lv_content, LV_OBJ_PART_MAIN, iotdisp_data->style);
    lv_obj_del(iotdisp_data->lv_content);
    lv_obj_del(iotdisp_data->lv_contentbk);

    lv_obj_remove_style(iotdisp_data->lv_week, LV_OBJ_PART_MAIN, iotdisp_data->style);
    lv_obj_del(iotdisp_data->lv_week);

    lv_obj_remove_style(iotdisp_data->lv_date, LV_OBJ_PART_MAIN, iotdisp_data->style);
    lv_obj_del(iotdisp_data->lv_date);

    lv_obj_del(iotdisp_data->lv_datebk);
    lv_obj_del(iotdisp_data->lv_clock);

    littlevgl2rtt_deinit();

    rt_free(iotdisp_data->style);
    rt_free_large((void *)iotdisp_data->fb);
}

/**
 * lvgl region refresh.
 */
static rt_err_t rkiot_disp_lvgl_refresh(struct rkiot_disp_data *iotdisp_data,
                                        struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &iotdisp_data->disp->info;

    if ((iotdisp_data->cmd & CMD_REFR_LVGL_CLOCK) == CMD_REFR_LVGL_CLOCK)
    {
        iotdisp_data->cmd &= ~CMD_REFR_LVGL_CLOCK;

        rkiot_disp_lvgl_clock_design(iotdisp_data);
    }
    /* else */if ((iotdisp_data->cmd & CMD_REFR_LVGL_IOT) == CMD_REFR_LVGL_IOT)
    {
        iotdisp_data->cmd &= ~CMD_REFR_LVGL_IOT;

        rkiot_lvgl_iot_design(iotdisp_data);
    }

    lv_task_handler();
    lvgl_img_dsc_remove_all();

    wincfg->winId  = LVGL_CLOCK_RGB332_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB332;
    wincfg->lut    = bpp_lut;
    wincfg->lutsize = sizeof(bpp_lut) / sizeof(bpp_lut[0]);
    wincfg->fb    = iotdisp_data->fb;
    wincfg->fblen = iotdisp_data->fblen;
    wincfg->x     = LVGL_REGION_X + ((info->width   - LVGL_FB_W) / 2);
    wincfg->y     = LVGL_REGION_Y + ((LVGL_REGION_H - LVGL_FB_H) / 2);
    wincfg->ylast = wincfg->y;
    wincfg->w     = LVGL_FB_W;
    wincfg->h     = LVGL_FB_H;

    RT_ASSERT(((wincfg->w * LVGL_WIN_PIXELS) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * logo display
 *
 **************************************************************************************************
 */

/**
 * logo loop timer callback.
 */
static void rkiot_disp_logo_loop_timer(void *parameter)
{
    struct rkiot_disp_data *iotdisp_data = (struct rkiot_disp_data *)parameter;

    iotdisp_data->cmd |= CMD_REFR_LOGO;
    rt_event_send(iotdisp_data->disp_event, EVENT_REFR_UPDATE);
}

/**
 * logo dmeo init.
 */
static rt_err_t rkiot_disp_logo_init(struct rkiot_disp_data *iotdisp_data)
{
    iotdisp_data->logo_xLoopOffset = 0;

    iotdisp_data->logo_loop_timer = rt_timer_create("rkiot_logo_timer",
                                    rkiot_disp_logo_loop_timer, (void *)iotdisp_data,
                                    RKIOT_LOGO_LOOP_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(iotdisp_data->logo_loop_timer != RT_NULL);
    rt_timer_start(iotdisp_data->logo_loop_timer);

    return RT_EOK;
}

/**
 * lvgl logo demo deinit.
 */
static void rkiot_disp_logo_deinit(struct rkiot_disp_data *iotdisp_data)
{
    rt_err_t ret;

    rt_timer_stop(iotdisp_data->logo_loop_timer);
    ret = rt_timer_delete(iotdisp_data->logo_loop_timer);
    RT_ASSERT(ret == RT_EOK);
}

/**
 * logo region refresh.
 */
static rt_err_t rkiot_disp_logo_refresh(struct rkiot_disp_data *iotdisp_data,
                                        struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &iotdisp_data->disp->info;
    image_info_t *img_info = &clock_logo_info;

    wincfg->winId = LVGL_LOGO_RGB565_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut   = RT_NULL;
    wincfg->lutsize  = 0;
    wincfg->fb    = img_info->data;
    wincfg->fblen = img_info->size;
    wincfg->x     = LOGO_REGION_X + ((info->width   - img_info->w) / 2);
    wincfg->y     = LOGO_REGION_Y + ((LOGO_REGION_H - img_info->h) / 2);
    wincfg->ylast = wincfg->y;
    wincfg->w     = img_info->w;
    wincfg->h     = img_info->h;

    iotdisp_data->logo_xLoopOffset += RKIOT_LOGO_LOOP_STEP;
    if (iotdisp_data->logo_xLoopOffset >= wincfg->w)
    {
        iotdisp_data->logo_xLoopOffset = 0;
    }
    wincfg->xLoopOffset = iotdisp_data->logo_xLoopOffset;

    RT_ASSERT(((wincfg->w * LOGO_WIN_PIXELS) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * home display
 *
 **************************************************************************************************
 */

/**
 * home init.
 */
static rt_err_t rkiot_disp_home_init(struct rkiot_disp_data *iotdisp_data)
{
    iotdisp_data->home_fblen = HOME_FB_W * HOME_FB_H / 8;
    iotdisp_data->home_fb    = (rt_uint8_t *)rt_malloc_large(iotdisp_data->home_fblen);
    RT_ASSERT(iotdisp_data->home_fb != RT_NULL);

    rt_memset((void *)iotdisp_data->home_fb, 0x00, iotdisp_data->home_fblen);

    return RT_EOK;
}

/**
 * home deinit.
 */
static void rkiot_disp_home_deinit(struct rkiot_disp_data *iotdisp_data)
{
    rt_free_large((void *)iotdisp_data->home_fb);
}

/**
 * home region refresh.
 */
static rt_err_t rkiot_disp_home_refresh(struct rkiot_disp_data *iotdisp_data,
                                        struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &iotdisp_data->disp->info;
    image_info_t *img_info = &clock_home_info;

    wincfg->winId = LVGL_HOME_GRAY1_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_GRAY1;
    wincfg->lut  = bpp1_lut;
    wincfg->lutsize = sizeof(bpp1_lut) / sizeof(bpp1_lut[0]);
    wincfg->fb    = iotdisp_data->home_fb;
    wincfg->w     = HOME_FB_W;
    wincfg->h     = HOME_FB_H;
    wincfg->fblen = wincfg->w * wincfg->h / 8;
    wincfg->x     = HOME_REGION_X + ((info->width   - wincfg->w) / 2);
    wincfg->y     = HOME_REGION_Y + ((HOME_REGION_H - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * HOME_WIN_PIXELS) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    RT_ASSERT(wincfg->fblen <= iotdisp_data->home_fblen);

    rt_uint16_t yoffset  = 0;
    rt_uint16_t xoffset  = (wincfg->w - img_info->w) / 2;
    rt_display_img_fill(img_info, wincfg->fb, wincfg->w, xoffset, yoffset);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * rkiot demo init & thread
 *
 **************************************************************************************************
 */

/**
 * rk iot display refresh request: request send data to LCD pannel.
 */
static rt_err_t rkiot_disp_refr_done(void)
{
    return (rt_event_send(g_iotdisp_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t rkiot_disp_refr_request(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;

    //request refresh display data to Pannel
    disp_mq->disp_finish = rkiot_disp_refr_done;
    ret = rt_mq_send(g_iotdisp_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_iotdisp_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * rkiot display task.
 */
static rt_err_t rkiot_disp_refr_tasks(struct rkiot_disp_data *iotdisp_data)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    disp_mq.win[0].zpos = WIN_TOP_LAYER;
    disp_mq.win[1].zpos = WIN_MIDDLE_LAYER;
    disp_mq.win[2].zpos = WIN_BOTTOM_LAYER;

    if ((iotdisp_data->cmd & CMD_REFR_HOME) == CMD_REFR_HOME)
    {
        iotdisp_data->cmd &= ~CMD_REFR_HOME;

        ret = rkiot_disp_home_refresh(iotdisp_data, &disp_mq.win[0]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 0);
    }

    if ((iotdisp_data->cmd & CMD_REFR_LVGL) == CMD_REFR_LVGL)
    {
        iotdisp_data->cmd &= ~CMD_REFR_LVGL;

        ret = rkiot_disp_lvgl_refresh(iotdisp_data, &disp_mq.win[1]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 1);
    }

    if ((iotdisp_data->cmd & CMD_REFR_LOGO) == CMD_REFR_LOGO)
    {
        iotdisp_data->cmd &= ~CMD_REFR_LOGO;

        ret = rkiot_disp_logo_refresh(iotdisp_data, &disp_mq.win[2]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 2);
    }

    if (disp_mq.cfgsta)
    {
        rkiot_disp_refr_request(&disp_mq);
    }

    if (iotdisp_data->cmd != 0)
    {
        rt_event_send(iotdisp_data->disp_event, EVENT_REFR_UPDATE);
    }

    return RT_EOK;
}

/**
 * lvgl clock demo thread
 */
static void rkiot_disp_thread(void *p)
{
    rt_err_t ret;
    uint32_t event;
    struct rkiot_disp_data *iotdisp_data;

    g_iotdisp_data = iotdisp_data = (struct rkiot_disp_data *)rt_malloc(sizeof(struct rkiot_disp_data));
    RT_ASSERT(iotdisp_data != RT_NULL);
    rt_memset((void *)iotdisp_data, 0, sizeof(struct rkiot_disp_data));

    iotdisp_data->disp = rt_display_get_disp();
    RT_ASSERT(iotdisp_data->disp != RT_NULL);

    //check required pannel size
    struct rt_device_graphic_info *info = &iotdisp_data->disp->info;
    if ((info->width < RKIOT_MAX_XRES) || (info->height < RKIOT_MAX_YRES))
    {
        rt_kprintf("Error: the pannel size(%dx%d) is less than required size(%dx%d)!\n",
                   info->width, info->height, RKIOT_MAX_XRES, RKIOT_MAX_YRES);
        RT_ASSERT(!(info->width < RKIOT_MAX_XRES));
        RT_ASSERT(!(info->height < RKIOT_MAX_YRES));
    }

    ret = rt_display_screen_clear();
    RT_ASSERT(ret == RT_EOK);

    iotdisp_data->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(iotdisp_data->disp_event != RT_NULL);

    ret = rkiot_disp_logo_init(iotdisp_data);
    RT_ASSERT(ret == RT_EOK);

    ret = rkiot_disp_home_init(iotdisp_data);
    RT_ASSERT(ret == RT_EOK);

    ret = rkiot_disp_lvgl_init(iotdisp_data);
    RT_ASSERT(ret == RT_EOK);

    iotdisp_data->cmd |= CMD_REFR_HOME | CMD_REFR_LOGO;
    rt_event_send(iotdisp_data->disp_event, EVENT_REFR_UPDATE);
    while (1)
    {
        ret = rt_event_recv(iotdisp_data->disp_event, EVENT_REFR_UPDATE,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER, &event);
        if (ret != RT_EOK)
        {
            /* Reserved... */
        }

        if (event & EVENT_REFR_UPDATE)
        {
            ret = rkiot_disp_refr_tasks(iotdisp_data);
            RT_ASSERT(ret == RT_EOK);
        }
    }

    rkiot_disp_home_deinit(iotdisp_data);

    rkiot_disp_lvgl_deinit(iotdisp_data);

    rkiot_disp_logo_deinit(iotdisp_data);

    rt_event_delete(iotdisp_data->disp_event);

    rt_free(iotdisp_data);
}

/**
 * lvgl clock demo init
 */
int rkiot_disp_app_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("RkIotDisp", rkiot_disp_thread, RT_NULL, 2048 * 2, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(rkiot_disp_app_init);
#endif
