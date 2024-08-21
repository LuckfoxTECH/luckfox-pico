#include <rtthread.h>
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"
#include "auto_version.h"

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

#include "app_main.h"

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */
#define APPCLK_CLOCK_UPDATE_TICKS   (RT_TICK_PER_SECOND / 10)

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct app_main_data_t *app_main_data = RT_NULL;
struct app_lvgl_data_t *g_lvdata      = RT_NULL;

const rt_uint8_t month_days[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 **************************************************************************************************
 *
 * Clock tick funcitons
 *
 **************************************************************************************************
 */

static void app_lvgl_init(void)
{
    struct app_lvgl_data_t *pdata;
    rt_err_t ret;

    g_lvdata = pdata = (struct app_lvgl_data_t *)rt_malloc(sizeof(struct app_lvgl_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct app_lvgl_data_t));

    /* framebuffer malloc */
    pdata->fblen = LV_HOR_RES * LV_VER_RES * LV_COLOR_DEPTH / 8;
    pdata->fb   = (rt_uint8_t *)rt_malloc_psram(pdata->fblen);
    RT_ASSERT(pdata->fb != RT_NULL);

    ret = littlevgl2rtt_init("lcd");
    RT_ASSERT(ret == RT_EOK);
}

/**
 * hook function for lvgl set graphic info.
 */
rt_err_t lv_set_graphic_info(struct rt_device_graphic_info *info)
{
    struct display_state *state = (struct display_state *)app_main_data->disp->device->user_data;
    struct rt_device_graphic_info *graphic_info = &state->graphic_info;

    graphic_info->bits_per_pixel = LV_COLOR_DEPTH;
    graphic_info->width          = LV_HOR_RES;
    graphic_info->height         = LV_VER_RES;
    graphic_info->framebuffer    = g_lvdata->fb;

    memcpy(info, graphic_info, sizeof(struct rt_device_graphic_info));

    return RT_EOK;
}

void lv_clock_img_file_load(lv_img_dsc_t *img_dsc, const char *file)
{
    lv_res_t res;

    //rt_kprintf("name = %s\n", file);

    lv_img_header_t info;
    res = lv_img_decoder_get_info(file, &info);
    RT_ASSERT(res == LV_RES_OK);

    uint8_t px_size = lv_img_cf_get_px_size(info.cf);
    RT_ASSERT(px_size == LV_COLOR_DEPTH);

    //rt_kprintf("%s: %d, %d, %d, %d\n", file, info.w, info.h, px_size, info.w * info.h * px_size / 8);

    uint8_t *img_buf;
    if (RT_PSRAM_MALLOC_THRRESH > info.w * info.h * px_size / 8)
    {
        img_buf = rt_malloc(info.w * info.h * px_size / 8);
    }
    else
    {
        img_buf = rt_malloc_psram(info.w * info.h * px_size / 8);
        //img_buf = rt_malloc_dtcm(info.w * info.h * px_size / 8);
    }
    RT_ASSERT(img_buf != RT_NULL);
    rt_memset((void *)img_buf, 0, info.w * info.h * px_size / 8);

    lv_img_decoder_dsc_t dsc;
    res = lv_img_decoder_open(&dsc, file, LV_COLOR_WHITE);
    RT_ASSERT(res == LV_RES_OK);

    res = lv_img_decoder_read_line(&dsc, 0, 0, info.w * info.h, img_buf);
    //RT_ASSERT(res == LV_RES_OK);

    lv_img_decoder_close(&dsc);

    img_dsc->header.cf = info.cf;
    img_dsc->header.always_zero = info.always_zero;
    img_dsc->header.w = info.w;
    img_dsc->header.h = info.h;
    img_dsc->data_size = info.w * info.h * px_size / 8;
    img_dsc->data = img_buf;
}

void lv_clock_img_dsc_free(lv_img_dsc_t *img_dsc)
{
    if (RT_PSRAM_MALLOC_THRRESH > img_dsc->data_size)
        rt_free((void *)img_dsc->data);
    else
        rt_free_psram((void *)img_dsc->data);
}

rt_err_t app_clock_load_img(img_load_info_t *info, rt_uint8_t *pbuf, rt_uint16_t w, rt_uint16_t h, rt_uint16_t offset, rt_uint8_t bitsize)
{
    int fd;
    rt_uint32_t flen;

    //rt_kprintf("name = %s\n", info->name);
    fd = open(info->name, O_RDONLY, 0);
    RT_ASSERT(fd >= 0);
    lseek(fd, 0, SEEK_SET);

    rt_uint32_t xoffset = offset;
    rt_uint32_t yoffset = (h - info->h) / 2;

    for (rt_uint16_t j = yoffset; j < yoffset + info->h; j++)
    {
        flen = read(fd, pbuf + (j * w + xoffset) * bitsize, info->w * bitsize);
        if (!flen)
        {
            break;
        }
    }

    close(fd);

    return RT_EOK;
}

static rt_uint32_t app_clock_str2num(const char *str, uint8_t len)
{
    rt_uint32_t num = 0;

    for (rt_uint8_t i = 0; i < len; i++)
    {
        num += (str[i] - '0') * pow(10, (len - i - 1));
    }

    return num;
}

static rt_uint8_t get_day_of_week(uint16_t year, uint8_t month, uint8_t day)
{
    rt_uint32_t a = month < 3 ? 1 : 0;
    rt_uint32_t b = year - a;

    rt_uint32_t day_of_week = (day + (31 * (month - 2 + 12 * a) / 12) + b + (b / 4) - (b / 100) + (b / 400)) % 7;

    return day_of_week;
}

/*
 **************************************************************************************************
 *
 * Clock tick funcitons
 *
 **************************************************************************************************
 */
static void app_clock_init_variable(clock_time_t *time)
{
    const char *str = FIRMWARE_AUTO_VERSION;

    /*Init the date of clock*/
    time->year     = app_clock_str2num(&str[0], 4);
    time->month    = app_clock_str2num(&str[5], 2);
    time->day      = app_clock_str2num(&str[8], 2);
    time->weekdays = get_day_of_week(time->year, time->month, time->day);

    /*Init the date of time*/
    time->hour    = app_clock_str2num(&str[11], 2);
    time->minute  = app_clock_str2num(&str[14], 2);
    time->second  = app_clock_str2num(&str[17], 2);
}

static void app_clock_timer_tick(void *parameter)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)parameter;
    clock_time_t *time = &pdata->tmr_data;

    rt_device_control(pdata->wdt_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    if (++time->tick >= (RT_TICK_PER_SECOND / APPCLK_CLOCK_UPDATE_TICKS))
    {
        time->tick = 0;
        if (++time->second >= 60)
        {
            time->second = 0;
            if (++time->minute >= 60)
            {
                time->minute = 0;
                if (++time->hour >= 24)
                {
                    time->hour = 0;
                    if (++time->weekdays >= 7)
                    {
                        time->weekdays = 0;
                    }

                    if (++time->day > month_days[time->month - 1])
                    {
                        time->day = 1;
                        if (++time->month > 12)
                        {
                            time->month = 1;
                            if (++time->year > 3000)
                            {
                                time->year = 1900;
                            }
                        }
                    }
                }
            }
        }
    }

    if (pdata->timer_cb)
    {
        pdata->timer_cb();
    }
}

void app_clock_timer_cb_register(void (*cb)(void))
{
    app_main_data->timer_cb = cb;
}

void app_clock_timer_cb_unregister(void)
{
    app_main_data->timer_cb = RT_NULL;
}

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

static rt_err_t app_clock_touch_process(struct rt_touch_data *point, rt_uint8_t num)
{
    rt_err_t ret;
    struct app_main_data_t *pdata = app_main_data;
    struct rt_touch_data *p       = &point[0];
    struct rt_touch_data *pre_p   = &pdata->pre_point[0];
    struct rt_touch_data *cur_p   = &pdata->cur_point[0];
    struct rt_touch_data *down_p  = &pdata->down_point[0];
    struct rt_touchpanel_block *b = &pdata->touch_block;

    if (RT_EOK != rt_touchpoint_is_valid(p, b))
    {
        return RT_ERROR;
    }

    struct rt_touch_data cur_point;
    rt_memcpy(&cur_point, p, sizeof(struct rt_touch_data));
    p = &cur_point;
    p->x_coordinate -= b->x;
    p->y_coordinate -= b->y;

    switch (b->event)
    {
    case RT_TOUCH_EVENT_DOWN:
        rt_memcpy(pre_p, p, sizeof(struct rt_touch_data));
        rt_memcpy(cur_p, p, sizeof(struct rt_touch_data));
        rt_memcpy(down_p, p, sizeof(struct rt_touch_data));

        pdata->tb_flag  = 0;
        pdata->dir_mode = 0;
        pdata->xdir     = 0;
        pdata->ydir     = 0;
        pdata->xoffset  = 0;
        pdata->yoffset  = 0;
        pdata->mov_fix = 0;

        if (pdata->tp_touch_down)
        {
            ret = pdata->tp_touch_down(pdata);
            RT_ASSERT(ret == RT_EOK);
        }
        break;

    case RT_TOUCH_EVENT_MOVE:
        if (pdata->dir_mode == TOUCH_DIR_MODE_NULL)
        {
            if ((ABS(p->x_coordinate - pre_p->x_coordinate) >= TOUCH_START_THRESHOLD) ||
                    (ABS(p->y_coordinate - pre_p->y_coordinate) >= TOUCH_START_THRESHOLD))
            {
                if (ABS(p->x_coordinate - down_p->x_coordinate) > ABS(p->y_coordinate - down_p->y_coordinate))
                {
                    pdata->dir_mode = TOUCH_DIR_MODE_LR;
                    if (pdata->tp_move_lr_start)
                    {
                        ret = pdata->tp_move_lr_start(pdata);
                        RT_ASSERT(ret == RT_EOK);
                    }
                }
                else
                {
                    pdata->dir_mode = TOUCH_DIR_MODE_UPDN;
                    if (pdata->tp_move_updn_start)
                    {
                        ret = pdata->tp_move_updn_start(pdata);
                        RT_ASSERT(ret == RT_EOK);
                    }
                }
            }
        }
        else if (pdata->dir_mode == TOUCH_DIR_MODE_LR)
        {
            if ((pdata->tb_flag) || (ABS(p->x_coordinate - pre_p->x_coordinate) >= TOUCH_MOVE_THRESHOLD))
            {
                if (pdata->tb_flag == 0)
                {
                    pdata->xdir = -1;
                    if (p->x_coordinate > pre_p->x_coordinate)
                    {
                        pdata->xdir = 1;
                    }
                }
                pdata->xoffset += (rt_int16_t)(p->x_coordinate - pre_p->x_coordinate);
                pdata->mov_speed = ((int32_t)p->x_coordinate - (int32_t)pre_p->x_coordinate) * 1000 / (int32_t)(p->timestamp - pre_p->timestamp);
                pdata->mov_fix = 0;
                pdata->down_timestamp = HAL_GetTick();

                rt_memcpy(cur_p, p, sizeof(struct rt_touch_data));
                if (pdata->tp_move_lr)
                {
                    pdata->tb_flag = 0;
                    ret = pdata->tp_move_lr(pdata);
                    if (RT_EOK != ret)
                    {
                        pdata->tb_flag = 1;
                    }
                }
                rt_memcpy(pre_p, p, sizeof(struct rt_touch_data));
            }
        }
        else if (pdata->dir_mode == TOUCH_DIR_MODE_UPDN)
        {
            if ((pdata->tb_flag) || (ABS(p->y_coordinate - pre_p->y_coordinate) >= TOUCH_MOVE_THRESHOLD))
            {
                if (pdata->tb_flag == 0)
                {
                    pdata->ydir = -1;
                    if (p->y_coordinate > pre_p->y_coordinate)
                    {
                        pdata->ydir = 1;
                    }
                }
                pdata->yoffset += (rt_int16_t)(p->y_coordinate - pre_p->y_coordinate);
                pdata->mov_speed = ((int32_t)p->y_coordinate - (int32_t)pre_p->y_coordinate) * 1000 / (int32_t)(p->timestamp - pre_p->timestamp);
                pdata->mov_fix = 0;
                pdata->down_timestamp = HAL_GetTick();

                rt_memcpy(cur_p, p, sizeof(struct rt_touch_data));
                if (pdata->tp_move_updn)
                {
                    pdata->tb_flag = 0;
                    ret = pdata->tp_move_updn(pdata);
                    if (RT_EOK != ret)
                    {
                        pdata->tb_flag = 1;
                    }
                }
                rt_memcpy(pre_p, p, sizeof(struct rt_touch_data));
            }
        }
        break;

    case RT_TOUCH_EVENT_UP:
        pdata->mov_fix = 0;
        if (pdata->dir_mode != TOUCH_DIR_MODE_NULL)
        {
            if (pdata->tp_move_up)
            {
                ret = pdata->tp_move_up(pdata);
                RT_ASSERT(ret == RT_EOK);
            }
        }
        else
        {
            if (pdata->tp_touch_up)
            {
                ret = pdata->tp_touch_up(pdata);
                RT_ASSERT(ret == RT_EOK);

                pdata->tb_flag  = 0;
                pdata->dir_mode = 0;
                pdata->xdir     = 0;
                pdata->ydir     = 0;
                pdata->xoffset  = 0;
                pdata->yoffset  = 0;
            }
        }
        break;

    default:
        break;
    }

    return RT_EOK;
}

static void app_clock_touch_init(struct rt_touchpanel_block *block)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;

    rt_memset(block, 0, sizeof(struct rt_touchpanel_block));

    block->x = TOUCH_REGION_X + ((info->width  - DISP_XRES) / 2);
    block->y = TOUCH_REGION_Y + ((info->height - DISP_YRES) / 2);
    block->w = TOUCH_REGION_W;
    block->h = TOUCH_REGION_H;
    block->name = "exh20";
    block->callback = app_clock_touch_process;
}

void app_clock_touch_value_reset(void)
{
    struct app_main_data_t *pdata = app_main_data;

    pdata->tb_flag  = 0;
    pdata->dir_mode = 0;
    pdata->xdir     = 0;
    pdata->ydir     = 0;
    pdata->xoffset  = 0;
    pdata->yoffset  = 0;
}

void app_clock_touch_unregister(void)
{
    struct app_main_data_t *pdata = app_main_data;

    pdata->tp_touch_down      = RT_NULL;

    pdata->tp_move_lr_start   = RT_NULL;
    pdata->tp_move_updn_start = RT_NULL;

    pdata->tp_move_lr         = RT_NULL;
    pdata->tp_move_updn       = RT_NULL;

    pdata->tp_move_up         = RT_NULL;
    pdata->tp_touch_up        = RT_NULL;
}

void app_clock_touch_register(struct app_main_touch_cb_t *tcb)
{
    struct app_main_data_t *pdata = app_main_data;

    pdata->tp_touch_down      = tcb->tp_touch_down;

    pdata->tp_move_lr_start   = tcb->tp_move_lr_start;
    pdata->tp_move_updn_start = tcb->tp_move_updn_start;

    pdata->tp_move_lr         = tcb->tp_move_lr;
    pdata->tp_move_updn       = tcb->tp_move_updn;

    pdata->tp_move_up         = tcb->tp_move_up;
    pdata->tp_touch_up        = tcb->tp_touch_up;
}

/*
 **************************************************************************************************
 *
 * Design subroutines
 *
 **************************************************************************************************
 */
/**
 * Design request.
 */
void app_clock_design_request(rt_uint8_t urgent, design_cb_t *design, void *param)
{
    register rt_base_t level;
    rt_err_t ret;

    struct app_main_data_t *pdata = app_main_data;
    rt_slist_t *head = &pdata->design_list;

    level = rt_hw_interrupt_disable();
    if (urgent)
    {
        rt_slist_insert(head, &design->list);
    }
    else
    {
        rt_slist_append(head, &design->list);
    }
    rt_hw_interrupt_enable(level);

    clock_exh_mq_t mq = {MQ_DESIGN_UPDATE, param};
    ret = rt_mq_send(pdata->mq, &mq, sizeof(clock_exh_mq_t));
    RT_ASSERT(ret != RT_ERROR);
}

/**
 * Design task.
 */
static void app_clock_design(struct app_main_data_t *pdata, void *param)
{
    rt_err_t ret;
    rt_slist_t *head = &pdata->design_list;
    rt_slist_t *list = rt_slist_first(head);
    register rt_base_t level;

    if (list != RT_NULL)
    {
        level = rt_hw_interrupt_disable();
        design_cb_t *design = rt_slist_entry(list, design_cb_t, list);
        rt_slist_remove(head, list);
        rt_hw_interrupt_enable(level);

        ret = design->cb(param);
        RT_ASSERT(ret == RT_EOK);
    }
}

/*
 **************************************************************************************************
 *
 * Display refresh subroutines
 *
 **************************************************************************************************
 */

/**
 * Refresh callback register.
 */
void app_clock_refresh_register(rt_uint8_t winid, void *cb, void *param)
{
    app_main_data->refr[winid].cb    = cb;
    app_main_data->refr[winid].param = param;
}

/**
 * Refresh callback unregister.
 */
void app_clock_refresh_unregister(rt_uint8_t winid)
{
    app_main_data->refr[winid].cb    = RT_NULL;
    app_main_data->refr[winid].param = RT_NULL;
}

/**
 * Refresh callback request.
 */
void app_clock_refresh_request(void *param)
{
    clock_exh_mq_t mq = {MQ_REFR_UPDATE, param};
    rt_err_t ret = rt_mq_send(app_main_data->mq, &mq, sizeof(clock_exh_mq_t));
    RT_ASSERT(ret != RT_ERROR);
}

/**
 * app clock display refresh request: request send data to LCD pannel.
 */
static rt_err_t app_clock_refr_done(void)
{
    return (rt_event_send(app_main_data->event, EVENT_REFR_DONE));
}

static rt_err_t app_clock_refr_request(struct rt_display_mq_t *disp_mq, rt_int32_t wait)
{
    rt_err_t ret;

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(app_main_data->event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        wait, &event);
    if (ret != RT_EOK)
    {
        return ret;
    }

    //request refresh display data to Pannel
    disp_mq->disp_finish = app_clock_refr_done;
    ret = rt_mq_send(app_main_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * app clock display task.
 */
static rt_err_t app_clock_refresh(struct app_main_data_t *pdata, void *param)
{
    rt_err_t ret = RT_EOK;
    struct rt_display_mq_t disp_mq;
    refrsh_request_param_t *par = (refrsh_request_param_t *)param;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    for (rt_uint8_t winid = 0; winid < 3; winid++)
    {
        if ((pdata->refr[winid].cb) && (par->wflag & (0x01 << winid)))
        {
            ret = pdata->refr[winid].cb(&disp_mq.win[winid], pdata->refr[winid].param);
            RT_ASSERT(ret == RT_EOK);
            disp_mq.cfgsta |= (0x01 << winid);
        }
    }

    if (disp_mq.cfgsta)
    {
        ret = app_clock_refr_request(&disp_mq, par->wait);
    }

    return ret;
}

/**
 * Direct refrest to LCD.
 */
rt_err_t app_clock_refresh_now(void *param)
{
    struct app_main_data_t *pdata = app_main_data;

    return (app_clock_refresh(pdata, param));
}

/*
 **************************************************************************************************
 *
 * clock demo init & thread
 *
 **************************************************************************************************
 */
/**
 * clock demo thread
 */
static void app_clock_thread(void *p)
{
    rt_err_t ret;
    clock_exh_mq_t mq;
    struct app_main_data_t *pdata;

    app_main_data = pdata = (struct app_main_data_t *)rt_malloc(sizeof(struct app_main_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct app_main_data_t));

    pdata->disp = rt_display_get_disp();
    RT_ASSERT(pdata->disp != RT_NULL);

    struct rt_device_graphic_info *info = &pdata->disp->info;
    if ((info->width < DISP_XRES) || (info->height < DISP_YRES))
    {
        rt_kprintf("Error: the pannel size(%dx%d) is less than required size(%dx%d)!\n",
                   info->width, info->height, DISP_XRES, DISP_YRES);
        RT_ASSERT(!(info->width < DISP_XRES));
        RT_ASSERT(!(info->height < DISP_YRES));
    }

    pdata->mq = rt_mq_create("clock_mq", sizeof(struct rt_display_mq_t), CLOCK_EXH_MQ_NUM, RT_IPC_FLAG_FIFO);
    RT_ASSERT(pdata->mq != RT_NULL);

    pdata->event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(pdata->event != RT_NULL);
    rt_event_send(pdata->event, EVENT_REFR_DONE);

    pdata->clk_timer = rt_timer_create("appclk_timer",
                                       app_clock_timer_tick, (void *)pdata,
                                       APPCLK_CLOCK_UPDATE_TICKS, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    RT_ASSERT(pdata->clk_timer != RT_NULL);

    pdata->workqueue = rt_workqueue_create("appclk_work", 1024 * 2, 6);
    RT_ASSERT(pdata->workqueue);

    // touch init...
    app_clock_touch_init(&pdata->touch_block);
    rt_touchpanel_block_register(&pdata->touch_block);

    // lvgl GUI init...
    app_lvgl_init();

    // variable init...
    app_clock_init_variable(&pdata->tmr_data);

    // init list...
    app_clock_init(); // First app init
    app_setting_main_init();
    app_setting_menu_init();
    app_func_list_init();
    app_alpha_win_init();
    app_dialog_box_init();

    uint32_t top = 300;
    int type = 0;
    pdata->wdt_dev = rt_device_find("dw_wdt");
    rt_device_init(pdata->wdt_dev);
    rt_device_control(pdata->wdt_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &top);
    rt_device_control(pdata->wdt_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);

    rt_timer_start(pdata->clk_timer);
    rt_device_control(pdata->wdt_dev, RT_DEVICE_CTRL_WDT_START, &type);
    while (1)
    {
        ret = rt_mq_recv(pdata->mq, &mq, sizeof(clock_exh_mq_t), RT_WAITING_FOREVER);
        RT_ASSERT(ret == RT_EOK);

        switch (mq.cmd)
        {
        case MQ_DESIGN_UPDATE:
            app_clock_design(pdata, mq.param);
            break;
        case MQ_REFR_UPDATE:
            app_clock_refresh(pdata, mq.param);
            break;
        default:
            break;
        }
    }

    rt_workqueue_destroy(pdata->workqueue);

    rt_timer_stop(pdata->clk_timer);
    ret = rt_timer_delete(pdata->clk_timer);
    RT_ASSERT(ret == RT_EOK);

    rt_event_delete(pdata->event);
    rt_mq_delete(pdata->mq);

    rt_free(pdata);
}

/**
 * clock demo init
 */
int clock_exh2020_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("clockexh", app_clock_thread, RT_NULL, 8192, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(clock_exh2020_init);
