#include <rtthread.h>
#if defined(RT_USING_CLOCK_DEMO) && defined(RT_USING_LITTLEVGL2RTT)
#include <stdio.h>
#include <math.h>

#include <littlevgl2rtt.h>
#include <lvgl/lvgl.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

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
 *  LV_HOR_RES          300
 *  LV_VER_RES          480
 */

/* display win layers */
#define LVGL_HOME_GRAY1_WIN     0
#define LVGL_CLOCK_RGB332_WIN   1
#define LVGL_LOGO_RGB565_WIN    2

#define CLOCK_MAX_XRES      720
#define CLOCK_MAX_YRES      1080

/* display region define */
#define LOGO_REGION_X       0
#define LOGO_REGION_Y       0
#define LOGO_REGION_H       80
#define LOGO_WIN_PIXELS     16

#define LVGL_REGION_X       0
#define LVGL_REGION_Y       (LOGO_REGION_Y + LOGO_REGION_H)
#define LVGL_REGION_H       600
#define LVGL_FB_W           LV_HOR_RES      /* clock frame buffer w */
#define LVGL_FB_H           LV_VER_RES      /* clock frame buffer h */
#define LVGL_WIN_PIXELS     RT_LV_COLOR_DEPTH

#define HOME_REGION_X       0
#define HOME_REGION_Y       (LVGL_REGION_Y + LVGL_REGION_H + 200)
#define HOME_REGION_H       (1080 - HOME_REGION_Y)
#define HOME_FB_W           704 // must aligned_32
#define HOME_FB_H           192
#define HOME_WIN_PIXELS     1

/* Command define */
#define CMD_REFR_LOGO               (0x01UL << 0)
#define CMD_REFR_HOME               (0x01UL << 1)
#define CMD_REFR_LVGL               (0x01UL << 2)
#define CMD_REFR_LVGL_CLOCK         (0x01UL << 3)

/* Event define */
#define EVENT_REFR_UPDATE           (0x01UL << 0)
#define EVENT_LVGL_UPDATE           (0x01UL << 1)
#define EVENT_REFR_DONE             (0x01UL << 2)

#define APPCLK_CLOCK_UPDATE_TICKS   (RT_TICK_PER_SECOND / 50)   //20ms
#define APPCLK_LOGO_LOOP_TICKS      (RT_TICK_PER_SECOND / 100 * 3)
#define APPCLK_LOGO_LOOP_STEP       2

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
extern image_info_t clock_bkg_info;
extern image_info_t clock_sec_info;
extern image_info_t clock_min_info;
extern image_info_t clock_hour_info;
extern image_info_t clock_logo_info;
extern image_info_t clock_home_info;

LV_FONT_DECLARE(lv_font_36_0_9);
static lv_font_t *g_lvgl_font = &lv_font_36_0_9;

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct lvgl_clock_data
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
    lv_obj_t   *lv_date;
    lv_style_t *lv_date_style;
    lv_obj_t   *lv_btn;
    lv_obj_t   *lv_btnlabel;
    char        date_str[30];

    rt_uint8_t *home_fb;
    rt_uint32_t home_fblen;

    rt_timer_t  logo_loop_timer;
    rt_uint16_t logo_xLoopOffset;
};
static  struct lvgl_clock_data *g_lvgl_data = RT_NULL;

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
    return 0;
}

/**
 * Display clock demo timer callback.
 */
static void app_clock_lvgl_timer(void *parameter)
{
    struct lvgl_clock_data *lv_clock_data = (struct lvgl_clock_data *)parameter;

    if (++lv_clock_data->ticks >= RT_TICK_PER_SECOND / APPCLK_CLOCK_UPDATE_TICKS)
    {
        lv_clock_data->ticks = 0;
        if (++lv_clock_data->second >= 60)
        {
            lv_clock_data->second = 0;
            if (++lv_clock_data->minute == 60)
            {
                lv_clock_data->minute = 0;
                if (++lv_clock_data->hour >= 24)
                {
                    lv_clock_data->hour = 0;

                    if (++lv_clock_data->week == 7)
                    {
                        lv_clock_data->week = 0;
                    }

                    if (++lv_clock_data->day > 31)
                    {
                        lv_clock_data->day = 1;
                        if (++lv_clock_data->month > 12)
                        {
                            lv_clock_data->month = 1;
                        }
                    }
                }
            }
        }

        lv_clock_data->cmd |= CMD_REFR_LVGL | CMD_REFR_LVGL_CLOCK;
        rt_event_send(lv_clock_data->disp_event, EVENT_REFR_UPDATE);
    }
    rt_event_send(lv_clock_data->disp_event, EVENT_LVGL_UPDATE);
}

/**
 * Clock region display refresh.
 */
static rt_err_t app_clock_lvgl_clock_design(struct lvgl_clock_data *lv_clock_data)
{
    // fill background
    if (1)
    {
        lv_img_dsc_t *dsc      = &clock_bkg_img_dsc;
        image_info_t *img_info = &clock_bkg_info;

        lvgl_img_dsc_init(dsc, img_info);
        lv_img_set_src(lv_clock_data->lv_clock, dsc);
        lv_obj_invalidate(lv_clock_data->lv_clock);

        // draw hour,min,sec
        {
            int32_t hour, angle;

            rt_uint16_t xoffset = (img_info->w / 2);
            rt_uint16_t yoffset = (img_info->h / 2);

            //draw hour line
            img_info = &clock_hour_info;
            hour = lv_clock_data->hour;
            if (hour >= 12)
            {
                hour -= 12;
            }
            angle = hour * (360 / 12) + (lv_clock_data->minute * 30) / 60 - 90;
            if (angle < 0)
            {
                angle += 360;
            }
            rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, img_info->data,
                                   (unsigned char *)((uint32_t)dsc->data + yoffset * dsc->header.w + xoffset),
                                   dsc->header.w, 8, img_info->h / 2);

            //draw min line
            img_info = &clock_min_info;
            angle = lv_clock_data->minute * (360 / 60);
            angle -= 90;
            if (angle < 0)
            {
                angle += 360;
            }
            rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, img_info->data,
                                   (unsigned char *)((uint32_t)dsc->data + yoffset * dsc->header.w + xoffset),
                                   dsc->header.w, 8, img_info->h / 2);

            //draw second line
            img_info = &clock_sec_info;
            angle = lv_clock_data->second * (360 / 60);
            angle -= 90;
            if (angle < 0)
            {
                angle += 360;
            }
            rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, img_info->data,
                                   (unsigned char *)((uint32_t)dsc->data + yoffset * dsc->header.w + xoffset),
                                   dsc->header.w, 8, img_info->h / 2);
        }
    }

    // draw date & week
    if (1)
    {
        /* date obj(label) init */
        snprintf(lv_clock_data->date_str, sizeof(lv_clock_data->date_str), "%02u月%02u日星期%s",
                 lv_clock_data->month, lv_clock_data->day, week_table[lv_clock_data->week]);
        lv_label_set_text(lv_clock_data->lv_date, lv_clock_data->date_str);
        lv_obj_invalidate(lv_clock_data->lv_date);
    }

    return RT_EOK;
}

/**
 * hook function for lvgl set graphic info.
 */
rt_err_t lv_set_graphic_info(struct rt_device_graphic_info *info)
{
    struct display_state *state = (struct display_state *)g_lvgl_data->disp->device->user_data;
    struct rt_device_graphic_info *graphic_info = &state->graphic_info;

    graphic_info->framebuffer = g_lvgl_data->fb;
    memcpy(info, graphic_info, sizeof(struct rt_device_graphic_info));

    return RT_EOK;
}

#if defined(RT_USING_TOUCH_DRIVERS)
/**
 * lvgl touch callback.
 */
static struct rt_touchpanel_block lv_touch_block;

static rt_err_t lv_touch_cb(struct rt_touch_data *point, rt_uint8_t num)
{
    struct rt_touch_data *p = &point[0];
    struct rt_touchpanel_block *b = &lv_touch_block;

    if (RT_EOK != rt_touchpoint_is_valid(p, b))
    {
        return RT_ERROR;
    }

    if (b->event == RT_TOUCH_EVENT_DOWN)
    {
        littlevgl2rtt_send_input_event(p->x_coordinate - b->x, p->y_coordinate - b->y, LITTLEVGL2RTT_INPUT_DOWN);
    }
    else if (b->event == RT_TOUCH_EVENT_MOVE)
    {
        littlevgl2rtt_send_input_event(p->x_coordinate - b->x, p->y_coordinate - b->y, LITTLEVGL2RTT_INPUT_MOVE);
    }
    else if (b->event == RT_TOUCH_EVENT_UP)
    {
        littlevgl2rtt_send_input_event(p->x_coordinate - b->x, p->y_coordinate - b->y, LITTLEVGL2RTT_INPUT_UP);
    }

    g_lvgl_data->cmd |= CMD_REFR_LVGL;
    rt_event_send(g_lvgl_data->disp_event, EVENT_REFR_UPDATE);

    return RT_EOK;
}

static void lv_touch_block_init(struct rt_touchpanel_block *block)
{
    struct rt_device_graphic_info *info = &g_lvgl_data->disp->info;

    rt_memset(block, 0, sizeof(struct rt_touchpanel_block));

    block->x = LVGL_REGION_X + ((info->width   - LVGL_FB_W) / 2);
    block->y = LVGL_REGION_Y + ((LVGL_REGION_H - LVGL_FB_H) / 2);
    block->w = LVGL_FB_W;
    block->h = LVGL_FB_H;
    block->name = "lvgl";
    block->callback = lv_touch_cb;
}

#endif

/**
 * lv btn event callback.
 */
static void lv_btn_event_cb(lv_obj_t *scr, lv_event_t event)
{
    if (event == LV_EVENT_PRESSED)
    {
        /* Example: Reset clock */
        g_lvgl_data->hour   = 0;
        g_lvgl_data->minute = 0;
        g_lvgl_data->second = 0;

        g_lvgl_data->cmd |= CMD_REFR_LVGL | CMD_REFR_LVGL_CLOCK;
        rt_event_send(g_lvgl_data->disp_event, EVENT_REFR_UPDATE);
    }
}

/**
 * lvgl clock dmeo init.
 */
static rt_err_t app_clock_lvgl_init(struct lvgl_clock_data *lv_clock_data)
{
    rt_err_t    ret;

    RT_ASSERT(LV_HOR_RES >= 300);
    RT_ASSERT(LV_VER_RES >= 480);
    RT_ASSERT(RT_LV_COLOR_DEPTH == 8);

    lv_clock_data->fblen = ((LVGL_FB_W * LVGL_FB_H + 3) / 4) * 4;
    lv_clock_data->fb     = (rt_uint8_t *)rt_malloc_large(lv_clock_data->fblen);
    RT_ASSERT(lv_clock_data->fb != RT_NULL);
    rt_memset((void *)lv_clock_data->fb, 0, lv_clock_data->fblen);

    {
        rt_uint16_t x, y, w, h;
        lv_area_t area;

        /* Var init */
        lv_clock_data->hour   = 3;
        lv_clock_data->minute = 30;
        lv_clock_data->second = 0;
        lv_clock_data->day    = 1;
        lv_clock_data->month  = 9;
        lv_clock_data->week   = 3;

#if defined(RT_USING_TOUCH_DRIVERS)
        /* init touch for littlevGL GUI */
        lv_touch_block_init(&lv_touch_block);
        rt_touchpanel_block_register(&lv_touch_block);
#endif

        /* init littlevGL */
        ret = littlevgl2rtt_init("lcd");
        RT_ASSERT(ret == RT_EOK);

        /* lvgl clock obj init */
        lv_clock_data->lv_clock = lv_img_create(lv_scr_act(), NULL);
        RT_ASSERT(lv_clock_data->lv_clock != NULL);

        image_info_t *img_info = &clock_bkg_info;
        x = (LVGL_FB_W - img_info->w) / 2;
        y = 0;
        w = img_info->w;
        h = img_info->h;
        RT_ASSERT(y + h < LVGL_FB_H);
        lv_obj_set_pos(lv_clock_data->lv_clock, x, y);
        lv_obj_set_size(lv_clock_data->lv_clock, w, h);

        /* lvgl date obj init */
        lv_clock_data->lv_date = lv_label_create(lv_scr_act(), NULL);
        RT_ASSERT(lv_clock_data->lv_date != NULL);

        lv_clock_data->lv_date_style = (lv_style_t *)rt_malloc(sizeof(lv_style_t));
        RT_ASSERT(lv_clock_data->lv_date_style != RT_NULL);
        lv_style_init(lv_clock_data->lv_date_style);
        lv_style_set_text_color(lv_clock_data->lv_date_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_style_set_text_font(lv_clock_data->lv_date_style, LV_STATE_DEFAULT, g_lvgl_font);
        lv_obj_add_style(lv_clock_data->lv_date, LV_OBJ_PART_MAIN, lv_clock_data->lv_date_style);

        lv_obj_get_coords(lv_clock_data->lv_clock, &area);
        x = 30;
        y = area.y2 + 20;
        w = LVGL_FB_W;
        h = g_lvgl_font->line_height;
        RT_ASSERT(y + h < LVGL_FB_H);
        lv_obj_set_pos(lv_clock_data->lv_date, x, y);
        lv_obj_set_size(lv_clock_data->lv_date, w, h);

        /* lvgl btn obj init */
        lv_obj_get_coords(lv_clock_data->lv_date, &area);
        x = 70;
        y = area.y2 + 40;
        w = 160;
        h = 80;
        RT_ASSERT(y + h < LVGL_FB_H);
        lv_clock_data->lv_btn = lv_btn_create(lv_disp_get_scr_act(NULL), NULL);
        lv_obj_set_pos(lv_clock_data->lv_btn, x, y);
        lv_obj_set_size(lv_clock_data->lv_btn, w, h);
        lv_obj_set_event_cb(lv_clock_data->lv_btn, lv_btn_event_cb);
        lv_clock_data->lv_btnlabel = lv_label_create(lv_clock_data->lv_btn, NULL);
        lv_label_set_text(lv_clock_data->lv_btnlabel, "Reset");
    }

    /* init timer */
    lv_clock_data->clock_timer = rt_timer_create("appclk_timer",
                                 app_clock_lvgl_timer, (void *)lv_clock_data,
                                 APPCLK_CLOCK_UPDATE_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(lv_clock_data->clock_timer != RT_NULL);
    rt_timer_start(lv_clock_data->clock_timer);

    lv_clock_data->cmd |= CMD_REFR_LVGL | CMD_REFR_LVGL_CLOCK;
    rt_event_send(lv_clock_data->disp_event, EVENT_REFR_UPDATE);

    return RT_EOK;
}

/**
 * lvgl clock demo deinit.
 */
static void app_clock_lvgl_deinit(struct lvgl_clock_data *lv_clock_data)
{
    rt_err_t ret;

    rt_timer_stop(lv_clock_data->clock_timer);
    ret = rt_timer_delete(lv_clock_data->clock_timer);
    RT_ASSERT(ret == RT_EOK);

    lv_obj_del(lv_clock_data->lv_btn);

    lv_obj_remove_style(lv_clock_data->lv_date, LV_OBJ_PART_MAIN, lv_clock_data->lv_date_style);
    rt_free(lv_clock_data->lv_date_style);

    lv_obj_del(lv_clock_data->lv_date);

    lv_obj_del(lv_clock_data->lv_clock);

    littlevgl2rtt_deinit();

#if defined(RT_USING_TOUCH_DRIVERS)
    rt_touchpanel_block_unregister(&lv_touch_block);
#endif

    rt_free_large((void *)lv_clock_data->fb);
}

/**
 * home region refresh.
 */
static rt_err_t app_clock_lvgl_refresh(struct lvgl_clock_data *lv_clock_data,
                                       struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &lv_clock_data->disp->info;

    if ((lv_clock_data->cmd & CMD_REFR_LVGL_CLOCK) == CMD_REFR_LVGL_CLOCK)
    {
        lv_clock_data->cmd &= ~CMD_REFR_LVGL_CLOCK;

        app_clock_lvgl_clock_design(lv_clock_data);
    }

    lv_task_handler();
    lvgl_img_dsc_remove_all();

    wincfg->winId  = LVGL_CLOCK_RGB332_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB332;
    wincfg->lut    = bpp_lut;
    wincfg->lutsize = sizeof(bpp_lut) / sizeof(bpp_lut[0]);
    wincfg->fb    = lv_clock_data->fb;
    wincfg->fblen = lv_clock_data->fblen;
    wincfg->w     = LVGL_FB_W;
    wincfg->h     = LVGL_FB_H;
    wincfg->x     = LVGL_REGION_X + ((info->width   - wincfg->w) / 2);
    wincfg->y     = LVGL_REGION_Y + ((LVGL_REGION_H - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

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
static void app_clock_logo_loop_timer(void *parameter)
{
    struct lvgl_clock_data *lv_clock_data = (struct lvgl_clock_data *)parameter;

    lv_clock_data->cmd |= CMD_REFR_LOGO;
    rt_event_send(lv_clock_data->disp_event, EVENT_REFR_UPDATE);
}

/**
 * lvgl logo dmeo init.
 */
static rt_err_t app_clock_logo_init(struct lvgl_clock_data *lv_clock_data)
{
    lv_clock_data->logo_xLoopOffset = 0;
    lv_clock_data->cmd |= CMD_REFR_LOGO;
    rt_event_send(lv_clock_data->disp_event, EVENT_REFR_UPDATE);

    lv_clock_data->logo_loop_timer = rt_timer_create("appclk_logo_timer",
                                     app_clock_logo_loop_timer, (void *)lv_clock_data,
                                     APPCLK_LOGO_LOOP_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(lv_clock_data->logo_loop_timer != RT_NULL);
    rt_timer_start(lv_clock_data->logo_loop_timer);

    return RT_EOK;
}

/**
 * lvgl logo demo deinit.
 */
static void app_clock_logo_deinit(struct lvgl_clock_data *lv_clock_data)
{
    rt_err_t ret;

    rt_timer_stop(lv_clock_data->logo_loop_timer);
    ret = rt_timer_delete(lv_clock_data->logo_loop_timer);
    RT_ASSERT(ret == RT_EOK);
}

/**
 * home region refresh.
 */
static rt_err_t app_clock_logo_refresh(struct lvgl_clock_data *lv_clock_data,
                                       struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &lv_clock_data->disp->info;
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

    lv_clock_data->logo_xLoopOffset += APPCLK_LOGO_LOOP_STEP;
    if (lv_clock_data->logo_xLoopOffset >= wincfg->w)
    {
        lv_clock_data->logo_xLoopOffset = 0;
    }
    wincfg->xLoopOffset = lv_clock_data->logo_xLoopOffset;

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
 * lvgl logo dmeo init.
 */
static rt_err_t app_clock_home_init(struct lvgl_clock_data *lv_clock_data)
{
    lv_clock_data->home_fblen = HOME_FB_W * HOME_FB_H / 8;
    lv_clock_data->home_fb    = (rt_uint8_t *)rt_malloc_large(lv_clock_data->home_fblen);
    RT_ASSERT(lv_clock_data->home_fb != RT_NULL);

    rt_memset((void *)lv_clock_data->home_fb, 0x00, lv_clock_data->home_fblen);

    lv_clock_data->cmd |= CMD_REFR_HOME;
    rt_event_send(lv_clock_data->disp_event, EVENT_REFR_UPDATE);

    return RT_EOK;
}

/**
 * lvgl logo demo deinit.
 */
static void app_clock_home_deinit(struct lvgl_clock_data *lv_clock_data)
{
    rt_free_large((void *)lv_clock_data->home_fb);
}

/**
 * home region refresh.
 */
static rt_err_t app_clock_home_refresh(struct lvgl_clock_data *lv_clock_data,
                                       struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &lv_clock_data->disp->info;
    image_info_t *img_info = &clock_home_info;

    wincfg->winId = LVGL_HOME_GRAY1_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_GRAY1;
    wincfg->lut  = bpp1_lut;
    wincfg->lutsize = sizeof(bpp1_lut) / sizeof(bpp1_lut[0]);
    wincfg->fb    = lv_clock_data->home_fb;
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

    RT_ASSERT(wincfg->fblen <= lv_clock_data->home_fblen);

    rt_uint16_t yoffset  = 0;
    rt_uint16_t xoffset  = (wincfg->w - img_info->w) / 2;
    rt_display_img_fill(img_info, wincfg->fb, wincfg->w, xoffset, yoffset);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * clock demo init & thread
 *
 **************************************************************************************************
 */

/**
 * app clock display refresh request: request send data to LCD pannel.
 */
static rt_err_t app_clock_refr_done(void)
{
    return (rt_event_send(g_lvgl_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t app_clock_refr_request(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;

    //request refresh display data to Pannel
    disp_mq->disp_finish = app_clock_refr_done;
    ret = rt_mq_send(g_lvgl_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_lvgl_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * app clock display task.
 */
static rt_err_t app_clock_refr_tasks(struct lvgl_clock_data *lv_clock_data)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    disp_mq.win[0].zpos = WIN_TOP_LAYER;
    disp_mq.win[1].zpos = WIN_MIDDLE_LAYER;
    disp_mq.win[2].zpos = WIN_BOTTOM_LAYER;

    if ((lv_clock_data->cmd & CMD_REFR_HOME) == CMD_REFR_HOME)
    {
        lv_clock_data->cmd &= ~CMD_REFR_HOME;

        ret = app_clock_home_refresh(lv_clock_data, &disp_mq.win[0]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 0);
    }

    if ((lv_clock_data->cmd & CMD_REFR_LVGL) == CMD_REFR_LVGL)
    {
        lv_clock_data->cmd &= ~CMD_REFR_LVGL;

        ret = app_clock_lvgl_refresh(lv_clock_data, &disp_mq.win[1]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 1);
    }

    if ((lv_clock_data->cmd & CMD_REFR_LOGO) == CMD_REFR_LOGO)
    {
        lv_clock_data->cmd &= ~CMD_REFR_LOGO;

        ret = app_clock_logo_refresh(lv_clock_data, &disp_mq.win[2]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 2);
    }

    if (disp_mq.cfgsta)
    {
        app_clock_refr_request(&disp_mq);
    }

    if (lv_clock_data->cmd != 0)
    {
        rt_event_send(lv_clock_data->disp_event, EVENT_REFR_UPDATE);
    }

    return RT_EOK;
}

/**
 * clock demo thread
 */
static void app_clock_thread(void *p)
{
    rt_err_t ret;
    uint32_t event;
    struct lvgl_clock_data *lv_clock_data;

    g_lvgl_data = lv_clock_data = (struct lvgl_clock_data *)rt_malloc(sizeof(struct lvgl_clock_data));
    RT_ASSERT(lv_clock_data != RT_NULL);
    rt_memset((void *)lv_clock_data, 0, sizeof(struct lvgl_clock_data));

    lv_clock_data->disp = rt_display_get_disp();
    RT_ASSERT(lv_clock_data->disp != RT_NULL);

    //check required pannel size
    struct rt_device_graphic_info *info = &lv_clock_data->disp->info;
    if ((info->width < CLOCK_MAX_XRES) || (info->height < CLOCK_MAX_YRES))
    {
        rt_kprintf("Error: the pannel size(%dx%d) is less than required size(%dx%d)!\n",
                   info->width, info->height, CLOCK_MAX_XRES, CLOCK_MAX_YRES);
        RT_ASSERT(!(info->width < CLOCK_MAX_XRES));
        RT_ASSERT(!(info->height < CLOCK_MAX_YRES));
    }

    ret = rt_display_screen_clear();
    RT_ASSERT(ret == RT_EOK);

    lv_clock_data->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(lv_clock_data->disp_event != RT_NULL);

    ret = app_clock_logo_init(lv_clock_data);
    RT_ASSERT(ret == RT_EOK);

    ret = app_clock_home_init(lv_clock_data);
    RT_ASSERT(ret == RT_EOK);

    ret = app_clock_lvgl_init(lv_clock_data);
    RT_ASSERT(ret == RT_EOK);

    while (1)
    {
        ret = rt_event_recv(lv_clock_data->disp_event, EVENT_REFR_UPDATE | EVENT_LVGL_UPDATE,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER, &event);
        if (ret != RT_EOK)
        {
            /* Reserved... */
        }

        if (event & EVENT_REFR_UPDATE)
        {
            ret = app_clock_refr_tasks(lv_clock_data);
            RT_ASSERT(ret == RT_EOK);
            continue;
        }

        if (event & EVENT_LVGL_UPDATE)
        {
            // Reserved......
            // lv_task_handler();
        }
    }

    /* Thread deinit */
    app_clock_home_deinit(lv_clock_data);

    app_clock_logo_deinit(lv_clock_data);

    app_clock_lvgl_deinit(lv_clock_data);

    rt_event_delete(lv_clock_data->disp_event);

    rt_free(lv_clock_data);
}

/**
 * clock demo init
 */
int app_clock_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("appclock", app_clock_thread, RT_NULL, 2048, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(app_clock_init);
#endif
