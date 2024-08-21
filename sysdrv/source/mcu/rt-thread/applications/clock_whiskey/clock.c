#include <rtthread.h>
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

//#define CLOCK_MOVE_UPDOWN

/* display win layers */
#define CLOCK_TOP_WIN       0
#define CLOCK_BOTTOM_WIN    1

#define CLOCK_MAX_XRES      320
#define CLOCK_MAX_YRES      320

/* display region define */
#define CLOCK_REGION_X      0
#define CLOCK_REGION_Y      20

#define CLOCK_BOTTOM_COLOR_DEPTH    16
#define CLOCK_BOTTOM_XRES   320 // must aligned_32
#define CLOCK_BOTTOM_YRES   320 // must aligned_32
#ifdef CLOCK_MOVE_UPDOWN
#define CLOCK_BOTTOM_FB_W   CLOCK_BOTTOM_XRES
#define CLOCK_BOTTOM_FB_H   (CLOCK_BOTTOM_YRES * 3)
#else
#define CLOCK_BOTTOM_FB_W   (CLOCK_BOTTOM_XRES * 3)
#define CLOCK_BOTTOM_FB_H   CLOCK_BOTTOM_YRES
#endif

#define CLOCK_TOP_COLOR_DEPTH       16
#define CLOCK_TOP_XRES      320 // must aligned_32
#define CLOCK_TOP_YRES      320 // must aligned_32
#ifdef CLOCK_MOVE_UPDOWN
#define CLOCK_TOP_FB_W      CLOCK_TOP_XRES
#define CLOCK_TOP_FB_H      (CLOCK_TOP_YRES * 3)
#else
#define CLOCK_TOP_FB_W      (CLOCK_TOP_XRES * 3)
#define CLOCK_TOP_FB_H      CLOCK_TOP_YRES
#endif

/* Command define */
#define CMD_CLOCK_REFRESH           (0x01UL << 0)
#define CMD_MOVE_START              (0x01UL << 1)
#define CMD_MOVE_REFRESH            (0x01UL << 2)
#define CMD_MOVE_AUTORUN            (0x01UL << 3)
#define CMD_MOVE_END                (0x01UL << 4)

/* Event define */
#define EVENT_CLOCK_UPDATE          (0x01UL << 0)
#define EVENT_REFR_UPDATE           (0x01UL << 1)
#define EVENT_REFR_DONE             (0x01UL << 2)

#define APPCLK_CLOCK_UPDATE_TICKS   (RT_TICK_PER_SECOND)
#define APPCLK_CLOCK_TEST_TICKS     (RT_TICK_PER_SECOND * 3)

#if defined(RT_USING_TOUCH_DRIVERS)
#define TOP_XMOVE_STEP      80
#define BOTTOM_XMOVE_STEP   80
#else
#define TOP_XMOVE_STEP      1
#define BOTTOM_XMOVE_STEP   1
#endif

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
extern image_info_t clock_bkg_info;
extern image_info_t clock_center_info;
extern image_info_t clock_sec_info;
extern image_info_t clock_min_info;
extern image_info_t clock_hour_info;

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct clock_whiskey_data
{
    rt_display_data_t disp;
    rt_event_t  disp_event;
    rt_timer_t  clock_timer;
    rt_uint32_t cmd;

    rt_uint32_t ticks;
    rt_uint8_t  hour;
    rt_uint8_t  minute;
    rt_uint8_t  second;
    rt_uint8_t  month;
    rt_uint8_t  day;
    rt_uint8_t  week;

    rt_uint8_t *bottom_fb;
    rt_uint32_t bottom_fblen;
    rt_uint8_t *top_fb;
    rt_uint32_t top_fblen;

    rt_int8_t   top_id;
    rt_int16_t  topmov_dir;
    rt_int16_t  topmov_offset;

    rt_int8_t   bottom_id;
    rt_int16_t  bottomov_dir;
    rt_int16_t  bottomov_offset;

    rt_timer_t  slide_timer;
    rt_int16_t  slide_cnt;
};
static  struct clock_whiskey_data *g_clock_data = RT_NULL;

struct clock_bottom_info
{
    rt_uint16_t w;
    rt_uint16_t h;
    const char *name;
};

#define CLOCK_BOTTOM_NUM_MAX    3
static struct clock_bottom_info bottomlist[CLOCK_BOTTOM_NUM_MAX] =
{
    { 320, 320, "/clock_bkg_320_320_1.dta"},
    { 320, 320, "/clock_bkg_320_320_2.dta"},
    { 320, 320, "/clock_bkg_320_320_3.dta"},
};

/*
 **************************************************************************************************
 *
 * Clock_whiskey functions.
 *
 **************************************************************************************************
 */

#if defined(RT_USING_TOUCH_DRIVERS)

#ifdef CLOCK_MOVE_UPDOWN
#define COORDINATE   y_coordinate
#else
#define COORDINATE   x_coordinate
#endif

#define BKGSLIDE_THRESHOLD_MIN  2
#define BKGSLIDE_THRESHOLD_MAX  20
static struct rt_touchpanel_block whiskey_bkg_touch_block;
struct rt_touch_data pre_point[1];

static rt_err_t app_clock_whiskey_bkg_touch_cb(struct rt_touch_data *point, rt_uint8_t num)
{
    struct clock_whiskey_data *pdata = g_clock_data;
    struct rt_touch_data *p = &point[0];
    struct rt_touch_data *pre_p = &pre_point[0];
    struct rt_touchpanel_block *b = &whiskey_bkg_touch_block;

    if (RT_EOK != rt_touchpoint_is_valid(p, b))
    {
        return RT_ERROR;
    }

    switch (b->event)
    {
    case RT_TOUCH_EVENT_DOWN:
        rt_memcpy(pre_p, p, sizeof(struct rt_touch_data));

        pdata->topmov_dir      = 0;
        pdata->bottomov_dir    = 0;
        pdata->topmov_offset   = 0;
        pdata->bottomov_offset = 0;

        pdata->cmd = CMD_MOVE_START;
        rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
        break;

    case RT_TOUCH_EVENT_MOVE:
        if (ABS(p->COORDINATE - pre_p->COORDINATE) >= BKGSLIDE_THRESHOLD_MIN)
        {
            pdata->topmov_dir = 1;
            pdata->bottomov_dir = 1;
            if (p->COORDINATE > pre_p->COORDINATE)
            {
                pdata->topmov_dir = -1;
                pdata->bottomov_dir = -1;
            }
            pdata->topmov_offset   += (rt_int16_t)(pre_p->COORDINATE - p->COORDINATE);
            pdata->bottomov_offset += (rt_int16_t)(pre_p->COORDINATE - p->COORDINATE);
            rt_memcpy(pre_point, p, sizeof(struct rt_touch_data));

            pdata->cmd = CMD_MOVE_REFRESH;
            rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
        }
        break;

    case RT_TOUCH_EVENT_UP:
        if (RT_TOUCH_EVENT_MOVE == pre_p->event)
        {
            if (ABS(p->COORDINATE - pre_p->COORDINATE) >= BKGSLIDE_THRESHOLD_MAX)
            {
                pdata->topmov_dir = 1;
                pdata->bottomov_dir = 1;
                if (p->COORDINATE > pre_p->COORDINATE)
                {
                    pdata->topmov_dir = -1;
                    pdata->bottomov_dir = -1;
                }
            }
            pdata->cmd = CMD_MOVE_AUTORUN;
            rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
        }
        break;
    default:
        break;
    }

    return RT_EOK;
}

static void app_clock_whiskey_bkg_touch_init(struct rt_touchpanel_block *block)
{
    struct rt_device_graphic_info *info = &g_clock_data->disp->info;

    rt_memset(block, 0, sizeof(struct rt_touchpanel_block));

    block->w = CLOCK_TOP_XRES;
    block->h = CLOCK_TOP_YRES;
    block->x = CLOCK_REGION_X + ((info->width  - block->w) / 2);
    block->y = CLOCK_REGION_Y;// + ((info->height - block->h) / 2);
    block->name = "whiskey";
    block->callback = app_clock_whiskey_bkg_touch_cb;
}

#else

// test: test timer
static void app_clock_slide_timer(void *parameter)
{
    struct clock_whiskey_data *pdata = (struct clock_whiskey_data *)parameter;

    if (pdata->slide_cnt < 5)
    {
        pdata->topmov_dir = 1;
        pdata->bottomov_dir = 1;
    }
    else
    {
        pdata->topmov_dir = -1;
        pdata->bottomov_dir = -1;
    }

    if (++pdata->slide_cnt >= 10)
    {
        pdata->slide_cnt = 0;
    }

    pdata->cmd = CMD_MOVE_START | CMD_MOVE_AUTORUN;
    rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
}

static void app_clock_slide_timer_restart(void *parameter)
{
    struct clock_whiskey_data *pdata = (struct clock_whiskey_data *)parameter;
    rt_tick_t arg = APPCLK_CLOCK_TEST_TICKS;

    rt_timer_control(pdata->slide_timer, RT_TIMER_CTRL_SET_TIME, &arg);
    rt_timer_start(pdata->slide_timer);
}
#endif

/**
 * Display clock demo timer callback.
 */
static void app_clock_whiskey_timer(void *parameter)
{
    struct clock_whiskey_data *pdata = (struct clock_whiskey_data *)parameter;

    if (++pdata->second >= 60)
    {
        pdata->second = 0;
        if (++pdata->minute == 60)
        {
            pdata->minute = 0;
            if (++pdata->hour >= 24)
            {
                pdata->hour = 0;

                if (++pdata->week == 7)
                {
                    pdata->week = 0;
                }

                if (++pdata->day > 31)
                {
                    pdata->day = 1;
                    if (++pdata->month > 12)
                    {
                        pdata->month = 1;
                    }
                }
            }
        }
    }

    pdata->cmd |= CMD_CLOCK_REFRESH;
    rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
}

/**
 * get clock background resource from file.
 */
static rt_err_t app_clock_whiskey_get_bkg(struct clock_bottom_info *info, rt_uint16_t *pbuf, rt_uint16_t w, rt_uint16_t h, rt_uint16_t offset)
{
    int fd;
    rt_uint32_t flen;

    fd = open(info->name, O_RDONLY, 0);
    RT_ASSERT(fd >= 0);
    lseek(fd, 0, SEEK_SET);

#ifdef CLOCK_MOVE_UPDOWN
    rt_uint32_t xoffset = (w - info->w) / 2;
    rt_uint32_t yoffset = offset * info->h;
#else
    rt_uint32_t xoffset = offset * info->w;
    rt_uint32_t yoffset = (h - info->h) / 2;
#endif

    for (rt_uint16_t j = yoffset; j < yoffset + info->h; j++)
    {
        flen = read(fd,
                    pbuf + j * w + xoffset,
                    info->w * sizeof(rt_uint16_t));

        if (!flen)
        {
            break;
        }
    }

    close(fd);

    return RT_EOK;
}

/**
 * clock dmeo init.
 */
static rt_err_t app_clock_whiskey_init(struct clock_whiskey_data *pdata)
{
    /* top framebuffer malloc */
    pdata->top_fblen = CLOCK_TOP_FB_W * CLOCK_TOP_FB_H * CLOCK_TOP_COLOR_DEPTH / 8;
    pdata->top_fb   = (rt_uint8_t *)rt_malloc_psram(pdata->top_fblen);
    RT_ASSERT(pdata->top_fb != RT_NULL);
    rt_memset((void *)pdata->top_fb, 0, pdata->top_fblen);

    /* bottom framebuffer malloc */
    pdata->bottom_fblen = CLOCK_BOTTOM_FB_W * CLOCK_BOTTOM_FB_H * CLOCK_BOTTOM_COLOR_DEPTH / 8;
    pdata->bottom_fb     = (rt_uint8_t *)rt_malloc_psram(pdata->bottom_fblen);
    RT_ASSERT(pdata->bottom_fb != RT_NULL);
    rt_memset((void *)pdata->bottom_fb, 0, pdata->bottom_fblen);

    /* init background data */
    pdata->bottom_id = 1;
    app_clock_whiskey_get_bkg(&bottomlist[1], (rt_uint16_t *)pdata->bottom_fb, CLOCK_BOTTOM_FB_W, CLOCK_BOTTOM_FB_H, 1);

    /* variable init */
    pdata->hour   = 3;
    pdata->minute = 30;
    pdata->second = 0;

    /* init timer */
    pdata->clock_timer = rt_timer_create("appclk_timer",
                                         app_clock_whiskey_timer, (void *)pdata,
                                         APPCLK_CLOCK_UPDATE_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(pdata->clock_timer != RT_NULL);
    rt_timer_start(pdata->clock_timer);

    pdata->cmd |= CMD_CLOCK_REFRESH;
    rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);

#if defined(RT_USING_TOUCH_DRIVERS)
    app_clock_whiskey_bkg_touch_init(&whiskey_bkg_touch_block);
    rt_touchpanel_block_register(&whiskey_bkg_touch_block);
#else
    pdata->slide_timer = rt_timer_create("apptst_timer",
                                         app_clock_slide_timer, (void *)pdata,
                                         APPCLK_CLOCK_TEST_TICKS + RT_TICK_PER_SECOND / 2,
                                         RT_TIMER_FLAG_ONE_SHOT);
    RT_ASSERT(pdata->slide_timer != RT_NULL);
    rt_timer_start(pdata->slide_timer);
#endif

    return RT_EOK;
}

/**
 * clock demo deinit.
 */
static void app_clock_whiskey_deinit(struct clock_whiskey_data *pdata)
{
    rt_err_t ret;

    // test: delete test timer
#if defined(RT_USING_TOUCH_DRIVERS)
    rt_touchpanel_block_unregister(&whiskey_bkg_touch_block);
#else
    rt_timer_stop(pdata->slide_timer);
    rt_timer_delete(pdata->slide_timer);
#endif

    rt_timer_stop(pdata->clock_timer);
    ret = rt_timer_delete(pdata->clock_timer);
    RT_ASSERT(ret == RT_EOK);

    rt_free_psram((void *)pdata->top_fb);
    rt_free_psram((void *)pdata->bottom_fb);
}

/**
 * draw clock.
 */
static rt_err_t app_clock_top_draw(struct clock_whiskey_data *pdata)
{
    int32_t hour, angle;
    image_info_t  *img_info = NULL;
    rt_uint8_t    *fb = pdata->top_fb;

    //clear clock area
#ifdef CLOCK_MOVE_UPDOWN
    rt_uint16_t *pbuf = (rt_uint16_t *)pdata->top_fb;
    for (rt_uint16_t j = CLOCK_TOP_YRES; j < CLOCK_TOP_YRES + CLOCK_TOP_YRES; j++)
    {
        for (rt_uint16_t i = 0; i < CLOCK_TOP_XRES; i++)
        {
            pbuf[j * CLOCK_TOP_FB_W + i] = 0;
        }
    }
#else
    rt_uint16_t *pbuf = (rt_uint16_t *)pdata->top_fb;
    for (rt_uint16_t j = 0; j < CLOCK_TOP_YRES; j++)
    {
        for (rt_uint16_t i = CLOCK_TOP_XRES; i < CLOCK_TOP_XRES + CLOCK_TOP_XRES; i++)
        {
            pbuf[j * CLOCK_TOP_FB_W + i] = 0;
        }
    }
#endif

    //draw clock
    if (1)
    {
#ifdef CLOCK_MOVE_UPDOWN
        rt_uint16_t xoffset = (CLOCK_TOP_XRES / 2);
        rt_uint16_t yoffset = (CLOCK_TOP_YRES / 2) + CLOCK_TOP_YRES;
#else
        rt_uint16_t xoffset = (CLOCK_TOP_XRES / 2) + CLOCK_TOP_XRES;
        rt_uint16_t yoffset = (CLOCK_TOP_YRES / 2);
#endif

        //draw hour line
        img_info = &clock_hour_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_XRES / 2);
        RT_ASSERT(img_info->h <= CLOCK_TOP_YRES / 2);

        hour = pdata->hour;
        if (hour >= 12)
        {
            hour -= 12;
        }
        angle = hour * (360 / 12) + (pdata->minute * 30) / 60 - 90;
        if (angle < 0)
        {
            angle += 360;
        }
        rt_display_rotate_16bit((float)angle, img_info->w, img_info->h, (unsigned short *)img_info->data,
                                (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                                CLOCK_TOP_FB_W, 0, img_info->h / 2);
        //draw min line
        img_info = &clock_min_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_XRES / 2);
        RT_ASSERT(img_info->h <= CLOCK_TOP_YRES / 2);

        angle = pdata->minute * (360 / 60);
        angle -= 90;
        if (angle < 0)
        {
            angle += 360;
        }
        rt_display_rotate_16bit((float)angle, img_info->w, img_info->h, (unsigned short *)img_info->data,
                                (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                                CLOCK_TOP_FB_W, 0, img_info->h / 2);

        //draw second line
        img_info = &clock_sec_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_XRES / 2);
        RT_ASSERT(img_info->h <= CLOCK_TOP_YRES / 2);

        angle = pdata->second * (360 / 60);
        angle -= 90;
        if (angle < 0)
        {
            angle += 360;
        }
        rt_display_rotate_16bit((float)angle, img_info->w, img_info->h, (unsigned short *)img_info->data,
                                (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                                CLOCK_TOP_FB_W, 0, img_info->h / 2);

        //draw centre
        img_info = &clock_center_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_XRES);
        RT_ASSERT(img_info->h <= CLOCK_TOP_YRES);

        yoffset  -= img_info->h / 2;
        xoffset  -= img_info->w / 2;
        rt_display_img_fill(img_info, fb, CLOCK_TOP_FB_W, xoffset, yoffset);
    }
    return RT_EOK;
}

/**
 * top move
 */
static rt_err_t app_clock_top_move_start(struct clock_whiskey_data *pdata)
{
    rt_uint16_t *pbuf = (rt_uint16_t *)pdata->top_fb;

    /* move buf2(clock data) to buf1 & buf3
    |<-- buf1 -->|<-- buf2 -->|<-- buf3 -->|
    ----------------------------------------
    |            | . . . . .  |            |
    |        <-- | . . . . .  | -->        |
    |            | . . . . .  |            |
    ----------------------------------------
    */

#ifdef CLOCK_MOVE_UPDOWN
    for (rt_uint16_t j = CLOCK_TOP_YRES; j < CLOCK_TOP_YRES + CLOCK_TOP_YRES; j++)
    {
        for (rt_uint16_t i = 0; i < CLOCK_TOP_XRES; i++)
        {
            pbuf[(j - CLOCK_TOP_YRES)  * CLOCK_TOP_FB_W + i] = pbuf[j * CLOCK_TOP_FB_W + i];
            pbuf[(j + CLOCK_TOP_YRES)  * CLOCK_TOP_FB_W + i] = pbuf[j * CLOCK_TOP_FB_W + i];
        }
    }
#else
    for (rt_uint16_t j = 0; j < CLOCK_TOP_YRES; j++)
    {
        for (rt_uint16_t i = CLOCK_TOP_XRES; i < CLOCK_TOP_XRES + CLOCK_TOP_XRES; i++)
        {
            pbuf[j * CLOCK_TOP_FB_W + i - CLOCK_TOP_XRES] = pbuf[j * CLOCK_TOP_FB_W + i];
            pbuf[j * CLOCK_TOP_FB_W + i + CLOCK_TOP_XRES] = pbuf[j * CLOCK_TOP_FB_W + i];
        }
    }
#endif

    return RT_EOK;
}

static void app_clock_top_move_autorun(struct clock_whiskey_data *pdata)
{
    pdata->topmov_offset += TOP_XMOVE_STEP * pdata->topmov_dir;
}

static rt_err_t app_clock_top_move_check(struct clock_whiskey_data *pdata)
{
    if (ABS(pdata->topmov_offset) >= CLOCK_TOP_XRES)
    {
        pdata->topmov_offset = CLOCK_TOP_XRES * pdata->topmov_dir;
        return RT_EEMPTY;
    }

    return RT_EOK;
}

static rt_err_t app_clock_top_move_end(struct clock_whiskey_data *pdata)
{
    pdata->topmov_offset = 0;
    pdata->topmov_dir = 0;

    return RT_EOK;
}

/**
 * top layer refresh.
 */
static rt_err_t app_clock_top_refresh(struct clock_whiskey_data *pdata,
                                      struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &pdata->disp->info;
    rt_uint32_t fblen  = pdata->top_fblen;
    rt_uint8_t  *fb    = pdata->top_fb;

    wincfg->winId  = CLOCK_TOP_WIN;
    wincfg->colorkey = COLOR_KEY_EN | 0;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut    = RT_NULL;
    wincfg->lutsize = 0;
#ifdef CLOCK_MOVE_UPDOWN
    wincfg->fb    = fb    + (CLOCK_TOP_YRES + pdata->topmov_offset) * CLOCK_TOP_FB_W * sizeof(rt_uint16_t);
    wincfg->fblen = CLOCK_TOP_YRES * CLOCK_TOP_FB_W * sizeof(rt_uint16_t);
#else
    wincfg->fb    = fb    + (CLOCK_TOP_XRES + pdata->topmov_offset) * sizeof(rt_uint16_t);
    wincfg->fblen = fblen - (CLOCK_TOP_XRES + pdata->topmov_offset) * sizeof(rt_uint16_t);
#endif
    wincfg->xVir  = CLOCK_TOP_FB_W;
    wincfg->w     = CLOCK_TOP_XRES;
    wincfg->h     = CLOCK_TOP_YRES;
    wincfg->x     = CLOCK_REGION_X + ((info->width  - wincfg->w) / 2);
    wincfg->y     = CLOCK_REGION_Y;// + ((info->height - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * CLOCK_TOP_COLOR_DEPTH) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

/**
 * bottom move
 */
static rt_err_t app_clock_bottom_move_start(struct clock_whiskey_data *pdata)
{
    rt_int8_t id;

    // get left bottom buffer
    id = pdata->bottom_id - 1;
    if (id < 0)
    {
        id = CLOCK_BOTTOM_NUM_MAX - 1;
    }
    app_clock_whiskey_get_bkg(&bottomlist[id], (rt_uint16_t *)pdata->bottom_fb, CLOCK_BOTTOM_FB_W, CLOCK_BOTTOM_FB_H, 0);

    // get right bottom buffer
    id = pdata->bottom_id + 1;
    if (id >= CLOCK_BOTTOM_NUM_MAX)
    {
        id = 0;
    }
    app_clock_whiskey_get_bkg(&bottomlist[id], (rt_uint16_t *)pdata->bottom_fb, CLOCK_BOTTOM_FB_W, CLOCK_BOTTOM_FB_H, 2);

    return RT_EOK;
}

static void app_clock_bottom_move_autorun(struct clock_whiskey_data *pdata)
{
    pdata->bottomov_offset += BOTTOM_XMOVE_STEP * pdata->bottomov_dir;
}

static rt_err_t app_clock_bottom_move_check(struct clock_whiskey_data *pdata)
{
    if (ABS(pdata->bottomov_offset) >= CLOCK_BOTTOM_XRES)
    {
        pdata->bottomov_offset = CLOCK_BOTTOM_XRES * pdata->bottomov_dir;
        return RT_EEMPTY;
    }

    return RT_EOK;
}

static rt_err_t app_clock_bottom_move_end(struct clock_whiskey_data *pdata)
{
    RT_ASSERT(ABS(pdata->bottomov_dir) == 1);

    pdata->bottom_id += pdata->bottomov_dir;
    if (pdata->bottom_id < 0)
    {
        pdata->bottom_id = CLOCK_BOTTOM_NUM_MAX - 1;
    }
    else if (pdata->bottom_id >= CLOCK_BOTTOM_NUM_MAX)
    {
        pdata->bottom_id = 0;
    }
    app_clock_whiskey_get_bkg(&bottomlist[pdata->bottom_id], (rt_uint16_t *)pdata->bottom_fb, CLOCK_BOTTOM_FB_W, CLOCK_BOTTOM_FB_H, 1);

    pdata->bottomov_offset = 0;
    pdata->bottomov_dir = 0;

    return RT_EOK;
}

/**
 * bottom layer refresh.
 */
static rt_err_t app_clock_bottom_refresh(struct clock_whiskey_data *pdata,
        struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &pdata->disp->info;

    wincfg->winId  = CLOCK_BOTTOM_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut    = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->xVir  = CLOCK_BOTTOM_FB_W;
    wincfg->w     = CLOCK_BOTTOM_XRES;
    wincfg->h     = CLOCK_BOTTOM_YRES;
#ifdef CLOCK_MOVE_UPDOWN
    wincfg->fb    = pdata->bottom_fb  + (CLOCK_BOTTOM_YRES + pdata->bottomov_offset) * CLOCK_BOTTOM_FB_W * sizeof(rt_uint16_t);
    wincfg->fblen = CLOCK_BOTTOM_YRES * CLOCK_BOTTOM_FB_W * sizeof(rt_uint16_t);
#else
    wincfg->fb    = pdata->bottom_fb    + (CLOCK_BOTTOM_XRES + pdata->bottomov_offset) * sizeof(rt_uint16_t);
    wincfg->fblen = pdata->bottom_fblen - (CLOCK_BOTTOM_XRES + pdata->bottomov_offset) * sizeof(rt_uint16_t);
#endif
    wincfg->x     = CLOCK_REGION_X + ((info->width  - wincfg->w) / 2);
    wincfg->y     = CLOCK_REGION_Y;// + ((info->height - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * CLOCK_BOTTOM_COLOR_DEPTH) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

static rt_err_t app_clock_is_move_end(struct clock_whiskey_data *pdata)
{
    if ((pdata->topmov_dir == 0) && (pdata->bottomov_dir == 0))
    {
        return RT_EOK;
    }

    return RT_ERROR;
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
    return (rt_event_send(g_clock_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t app_clock_refr_request(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;

    //request refresh display data to Pannel
    disp_mq->disp_finish = app_clock_refr_done;
    ret = rt_mq_send(g_clock_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_clock_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * app clock display task.
 */
static rt_err_t app_clock_refr_tasks(struct clock_whiskey_data *pdata)
{
    rt_err_t ret, ret1;
    struct rt_display_mq_t disp_mq;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    disp_mq.win[CLOCK_TOP_WIN].zpos = WIN_TOP_LAYER;
    disp_mq.win[CLOCK_BOTTOM_WIN].zpos = WIN_BOTTOM_LAYER;

    if ((pdata->cmd & CMD_MOVE_START) == CMD_MOVE_START)
    {
        pdata->cmd &= ~CMD_MOVE_START;

        app_clock_top_move_start(pdata);
        app_clock_bottom_move_start(pdata);
    }

    if ((pdata->cmd & CMD_MOVE_AUTORUN) == CMD_MOVE_AUTORUN)
    {
        app_clock_top_move_autorun(pdata);
        app_clock_bottom_move_autorun(pdata);

        pdata->cmd |= CMD_MOVE_REFRESH;
    }

    if ((pdata->cmd & CMD_MOVE_REFRESH) == CMD_MOVE_REFRESH)
    {
        pdata->cmd &= ~CMD_MOVE_REFRESH;
        pdata->cmd |= CMD_CLOCK_REFRESH;

        ret  = app_clock_top_move_check(pdata);
        ret1 = app_clock_bottom_move_check(pdata);
        if ((ret != RT_EOK) || (ret1 != RT_EOK))
        {
            pdata->cmd |= CMD_MOVE_END;
        }
    }

    if ((pdata->cmd & CMD_CLOCK_REFRESH) == CMD_CLOCK_REFRESH)
    {
        pdata->cmd &= ~CMD_CLOCK_REFRESH;

        if (RT_EOK == app_clock_is_move_end(pdata))
        {
            ret = app_clock_top_draw(pdata);
            RT_ASSERT(ret == RT_EOK);
        }

        ret = app_clock_top_refresh(pdata, &disp_mq.win[CLOCK_TOP_WIN]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << CLOCK_TOP_WIN);

        ret = app_clock_bottom_refresh(pdata, &disp_mq.win[CLOCK_BOTTOM_WIN]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << CLOCK_BOTTOM_WIN);
    }

    if ((pdata->cmd & CMD_MOVE_END) == CMD_MOVE_END)
    {
        pdata->cmd &= ~CMD_MOVE_END;
        pdata->cmd &= ~CMD_MOVE_AUTORUN;

        app_clock_top_move_end(pdata);
        app_clock_bottom_move_end(pdata);

#ifndef RT_USING_TOUCH_DRIVERS
        app_clock_slide_timer_restart(pdata);
#endif
    }

    if (disp_mq.cfgsta)
    {
        app_clock_refr_request(&disp_mq);
    }

    if (pdata->cmd != 0)
    {
        rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
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
    struct clock_whiskey_data *pdata;

    g_clock_data = pdata = (struct clock_whiskey_data *)rt_malloc(sizeof(struct clock_whiskey_data));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct clock_whiskey_data));

    pdata->disp = rt_display_get_disp();
    RT_ASSERT(pdata->disp != RT_NULL);

    struct rt_device_graphic_info *info = &pdata->disp->info;
    if ((info->width < CLOCK_MAX_XRES) || (info->height < CLOCK_MAX_YRES))
    {
        rt_kprintf("Error: the pannel size(%dx%d) is less than required size(%dx%d)!\n",
                   info->width, info->height, CLOCK_MAX_XRES, CLOCK_MAX_YRES);
        RT_ASSERT(!(info->width < CLOCK_MAX_XRES));
        RT_ASSERT(!(info->height < CLOCK_MAX_YRES));
    }

    ret = rt_display_screen_clear();
    RT_ASSERT(ret == RT_EOK);

    pdata->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(pdata->disp_event != RT_NULL);

    ret = app_clock_whiskey_init(pdata);
    RT_ASSERT(ret == RT_EOK);

    while (1)
    {
        ret = rt_event_recv(pdata->disp_event, EVENT_REFR_UPDATE,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER, &event);
        if (ret != RT_EOK)
        {
            /* Reserved... */
        }

        if (event & EVENT_REFR_UPDATE)
        {
            ret = app_clock_refr_tasks(pdata);
            RT_ASSERT(ret == RT_EOK);
            continue;
        }
    }

    /* Thread deinit */
    app_clock_whiskey_deinit(pdata);

    rt_event_delete(pdata->disp_event);

    rt_free(pdata);
}

/**
 * clock demo init
 */
int clock_whiskey_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("clockwhiskey", app_clock_thread, RT_NULL, 2048, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(clock_whiskey_init);
