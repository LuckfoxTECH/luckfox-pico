/**
* @file lv_clock.c
*
*/

/*********************
 *      INCLUDES
 *********************/
#include "lv_clock.h"
#if LV_USE_CLOCK != 0

#include "../lv_misc/lv_debug.h"
#include "../lv_draw/lv_draw.h"
#include "../lv_themes/lv_theme.h"
#include "../lv_misc/lv_txt.h"
#include "../lv_misc/lv_math.h"
#include "../lv_misc/lv_utils.h"
#include "lv_img.h"
#include <stdio.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/
#define LV_OBJX_NAME "lv_clock"

#define LV_CLOCK_DEF_NEEDLE_COLOR LV_COLOR_RED
#define LV_CLOCK_DEF_LABEL_COUNT 13
#define LV_CLOCK_DEF_LINE_COUNT 61 /*Should be: ((label_cnt - 1) * internal_lines) + 1*/
#define LV_CLOCK_DEF_ANGLE 360

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_design_res_t lv_clock_design(lv_obj_t *clock, const lv_area_t *clip_area, lv_design_mode_t mode);
static lv_res_t lv_clock_signal(lv_obj_t *clock, lv_signal_t sign, void *param);
static lv_style_list_t *lv_clock_get_style(lv_obj_t *clock, uint8_t part);
static void lv_clock_draw_labels(lv_obj_t *clock, const lv_area_t *mask);
static void lv_clock_draw_needle(lv_obj_t *clock, const lv_area_t *clip_area);
static void lv_clock_draw_pannel(lv_obj_t *clock, const lv_area_t *clip_area);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_design_cb_t ancestor_design;
static lv_signal_cb_t ancestor_signal;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a clock objects
 * @param par pointer to an object, it will be the parent of the new clock
 * @param copy pointer to a clock object, if not NULL then the new object will be copied from it
 * @return pointer to the created clock
 */
lv_obj_t *lv_clock_create(lv_obj_t *par, const lv_obj_t *copy)
{
    LV_LOG_TRACE("clock create started");

    /*Create the ancestor clock*/
    lv_obj_t *clock = lv_linemeter_create(par, copy);
    LV_ASSERT_MEM(clock);
    if (clock == NULL) return NULL;

    /*Allocate the clock type specific extended data*/
    lv_clock_ext_t *ext = lv_obj_allocate_ext_attr(clock, sizeof(lv_clock_ext_t));
    LV_ASSERT_MEM(ext);
    if (ext == NULL)
    {
        lv_obj_del(clock);
        return NULL;
    }

    /*Initialize the allocated 'ext' */
    ext->format_cb     = NULL;
    ext->task          = NULL;

    if (ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(clock);
    if (ancestor_design == NULL) ancestor_design = lv_obj_get_design_cb(clock);

    lv_style_list_init(&ext->style_strong);
    lv_style_list_init(&ext->style_needle);

    /*The signal and design functions are not copied so set them here*/
    lv_obj_set_signal_cb(clock, lv_clock_signal);
    lv_obj_set_design_cb(clock, lv_clock_design);

    /*Init the new clock clock*/
    if (copy == NULL)
    {
        lv_clock_set_scale(clock, LV_CLOCK_DEF_ANGLE, LV_CLOCK_DEF_LINE_COUNT);
        lv_clock_set_critical_value(clock, 100);

        lv_theme_apply(clock, LV_THEME_CLOCK);

    }
    /*Copy an existing clock*/
    else
    {
        lv_clock_ext_t *copy_ext = lv_obj_get_ext_attr(copy);

        lv_style_list_copy(&ext->style_strong, &copy_ext->style_strong);
        lv_style_list_copy(&ext->style_needle, &copy_ext->style_needle);

        ext->label_count = copy_ext->label_count;
        ext->format_cb   = copy_ext->format_cb;

        /*Refresh the style with new signal function*/
        lv_obj_refresh_style(clock, LV_STYLE_PROP_ALL);
    }

    LV_LOG_INFO("clock created");

    return clock;
}

/*=====================
 * Setter functions
 *====================*/
/**
 * Set the clock date
 * @param clock pointer to a clock
 * @param date the date value pointer
 */
void lv_clock_set_date(lv_obj_t *clock, lv_clock_date_t *date)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_clock_ext_t *ext = lv_obj_get_ext_attr(clock);

    ext->date = date;
}

/**
 * Set the time of a clock
 * @param clock pointer to a clock
 * @param time the pointer of time
 */
void lv_clock_set_time(lv_obj_t *clock, lv_clock_time_t *time)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_clock_ext_t *ext = lv_obj_get_ext_attr(clock);

    ext->time = time;
}

/**
 * Set the style of a needle
 * @param clock pointer to a clock
 * @param needle the style of the needles
 */
void lv_clock_set_needle_style(lv_obj_t *clock, lv_clock_needle_style_t *needle)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_clock_ext_t *ext = lv_obj_get_ext_attr(clock);

    lv_clock_needle_style_t *pneedle = needle;

    for (uint8_t i = 0; i < NEEDLE_MAX_CONT; i++)
    {
        ext->needle_style[i].color = pneedle->color;
        ext->needle_style[i].width = pneedle->width;
        ext->needle_style[i].length = pneedle->length;
        ext->needle_style[i].centre = pneedle->centre;

        ext->needle_style[i].hollow_color = pneedle->hollow_color;
        ext->needle_style[i].hollow_width = pneedle->hollow_width;
        ext->needle_style[i].hollow_lpad = pneedle->hollow_lpad;
        ext->needle_style[i].hollow_rpad = pneedle->hollow_rpad;

        ext->needle_style[i].img = pneedle->img;

        pneedle++;
    }
}

/**
 * Set the style of a pannel
 * @param clock pointer to a clock
 * @param panel the style of the pannel
 */
void lv_clock_set_panel_style(lv_obj_t *clock, lv_clock_panel_style_t *panel)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_clock_ext_t *ext = lv_obj_get_ext_attr(clock);

    ext->panel_style.label_count = panel->label_count;
    ext->panel_style.bk_img      = panel->bk_img;
    ext->panel_style.centre_img  = panel->centre_img;
}

/**
 * Set the scale settings of a clock
 * @param clock pointer to a clock object
 * @param angle angle of the scale (0..360)
 * @param line_cnt count of scale lines.
 * To get a given "subdivision" lines between labels:
 * `line_cnt = (sub_div + 1) * (label_cnt - 1) + 1 `
 * @param label_cnt count of scale labels.
 */
void lv_clock_set_scale(lv_obj_t *clock, uint16_t angle, uint8_t line_cnt)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_linemeter_set_scale(clock, angle, line_cnt);

    lv_obj_invalidate(clock);
}

/**
 * Assign a function to format clock values
 * @param clock pointer to a clock object
 * @param format_cb pointer to function of lv_clock_format_cb_t
 */
void lv_clock_set_formatter_cb(lv_obj_t *clock, lv_clock_format_cb_t format_cb)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_clock_ext_t *ext = lv_obj_get_ext_attr(clock);

    ext->format_cb = format_cb;
}

/*=====================
 * Getter functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Handle the drawing related tasks of the clocks
 * @param clock pointer to an object
 * @param clip_area the object will be drawn only in this area
 * @param mode LV_DESIGN_COVER_CHK: only check if the object fully covers the 'mask_p' area
 *                                  (return 'true' if yes)
 *             LV_DESIGN_DRAW: draw the object (always return 'true')
 *             LV_DESIGN_DRAW_POST: drawing after every children are drawn
 * @param return an element of `lv_design_res_t`
 */
static lv_design_res_t lv_clock_design(lv_obj_t *clock, const lv_area_t *clip_area, lv_design_mode_t mode)
{
    lv_clock_ext_t *ext  = lv_obj_get_ext_attr(clock);

    /*Return false if the object is not covers the mask_p area*/
    if (mode == LV_DESIGN_COVER_CHK)
    {
        return LV_DESIGN_RES_NOT_COVER;
    }
    /*Draw the object*/
    else if (mode == LV_DESIGN_DRAW_MAIN)
    {
        if (ext->panel_style.bk_img == NULL)
        {
            ancestor_design(clock, clip_area, mode);
        }

        lv_clock_draw_pannel(clock, clip_area);

        lv_clock_draw_labels(clock, clip_area);

        lv_clock_draw_needle(clock, clip_area);
    }
    /*Post draw when the children are drawn*/
    else if (mode == LV_DESIGN_DRAW_POST)
    {
        if (ext->panel_style.bk_img == NULL)
        {
            ancestor_design(clock, clip_area, mode);
        }
    }

    return LV_DESIGN_RES_OK;
}

/**
 * Signal function of the clock
 * @param clock pointer to a clock object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_clock_signal(lv_obj_t *clock, lv_signal_t sign, void *param)
{
    lv_res_t res;
    if (sign == LV_SIGNAL_GET_STYLE)
    {
        lv_get_style_info_t *info = param;
        info->result = lv_clock_get_style(clock, info->part);
        if (info->result != NULL) return LV_RES_OK;
        else return ancestor_signal(clock, sign, param);
    }

    /* Include the ancient signal function */
    res = ancestor_signal(clock, sign, param);
    if (res != LV_RES_OK) return res;
    if (sign == LV_SIGNAL_GET_TYPE) return lv_obj_handle_get_type_signal(param, LV_OBJX_NAME);

    if (sign == LV_SIGNAL_CLEANUP)
    {
        lv_obj_clean_style_list(clock, LV_CLOCK_PART_NEEDLE);
        lv_obj_clean_style_list(clock, LV_CLOCK_PART_MAJOR);
    }

    return res;
}
/**
 * Get the style descriptor of a part of the object
 * @param page pointer the object
 * @param part the part from `lv_clock_part_t`. (LV_CLOCK_PART_...)
 * @return pointer to the style descriptor of the specified part
 */
static lv_style_list_t *lv_clock_get_style(lv_obj_t *clock, uint8_t part)
{
    LV_ASSERT_OBJ(clock, LV_OBJX_NAME);

    lv_clock_ext_t *ext = lv_obj_get_ext_attr(clock);
    lv_style_list_t *style_dsc_p;

    switch (part)
    {
    case LV_CLOCK_PART_MAIN:
        style_dsc_p = &clock->style_list;
        break;
    case LV_CLOCK_PART_MAJOR:
        style_dsc_p = &ext->style_strong;
        break;
    case LV_CLOCK_PART_NEEDLE:
        style_dsc_p = &ext->style_needle;
        break;
    default:
        style_dsc_p = NULL;
    }

    return style_dsc_p;
}
/**
 * Draw the scale on a clock
 * @param clock pointer to clock object
 * @param mask mask of drawing
 */
static void lv_clock_draw_labels(lv_obj_t *clock, const lv_area_t *mask)
{
    lv_clock_ext_t *ext     = lv_obj_get_ext_attr(clock);

    if (ext->panel_style.label_count == LABEL_CONT_NULL) return;

    /*draw hour scale*/
    uint16_t line_cnt_tmp = ext->lmeter.line_cnt;
    ext->lmeter.line_cnt = 12 + 1;                 /*Only to labels*/
    lv_linemeter_draw_scale(clock, mask, LV_CLOCK_PART_MAJOR);
    ext->lmeter.line_cnt = line_cnt_tmp; /*Restore the parameters*/

    lv_style_int_t scale_width = lv_obj_get_style_scale_width(clock, LV_CLOCK_PART_MAJOR);
    lv_style_int_t left = lv_obj_get_style_pad_left(clock, LV_CLOCK_PART_MAIN);
    lv_style_int_t right = lv_obj_get_style_pad_right(clock, LV_CLOCK_PART_MAIN);
    lv_style_int_t top = lv_obj_get_style_pad_top(clock, LV_CLOCK_PART_MAIN);
    lv_style_int_t txt_pad = lv_obj_get_style_pad_inner(clock, LV_CLOCK_PART_MAIN);
    lv_coord_t r             = (lv_obj_get_width(clock) - left - right) / 2 - scale_width - txt_pad;
    lv_coord_t x_ofs         = clock->coords.x1 + r + left + scale_width + txt_pad;
    lv_coord_t y_ofs         = clock->coords.y1 + r + top + scale_width + txt_pad;
    int16_t scale_angle      = lv_linemeter_get_scale_angle(clock);
    int16_t angle_ofs        = 90 + (scale_angle) / 2 + lv_clock_get_angle_offset(clock);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_obj_init_draw_label_dsc(clock, LV_CLOCK_PART_MAJOR, &label_dsc);

    uint8_t i;
    uint8_t label_count = ext->panel_style.label_count;
    if (label_count > 12) label_count = 12;
    for (i = 1; i <= label_count; i++)
    {
        /*Calculate the position a scale label*/
        int16_t angle = i * (360 / label_count) + angle_ofs;

        lv_coord_t y = (int32_t)((int32_t)_lv_trigo_sin(angle) * r) / LV_TRIGO_SIN_MAX;
        y += y_ofs;

        lv_coord_t x = (int32_t)((int32_t)_lv_trigo_sin(angle + 90) * r) / LV_TRIGO_SIN_MAX;
        x += x_ofs;

        int32_t scale_act = i * (12 / label_count);
        char scale_txt[16];
        if (ext->format_cb == NULL)
            _lv_utils_num_to_str(scale_act, scale_txt);
        else
            ext->format_cb(clock, scale_txt, sizeof(scale_txt), scale_act);

        lv_area_t label_cord;
        lv_point_t label_size;
        _lv_txt_get_size(&label_size, scale_txt, label_dsc.font, label_dsc.letter_space, label_dsc.line_space,
                         LV_COORD_MAX, LV_TXT_FLAG_NONE);

        /*Draw the label*/
        label_cord.x1 = x - label_size.x / 2;
        label_cord.y1 = y - label_size.y / 2;
        label_cord.x2 = label_cord.x1 + label_size.x;
        label_cord.y2 = label_cord.y1 + label_size.y;

        lv_draw_label(&label_cord, mask, &label_dsc, scale_txt, NULL);
    }
}

/**
 * Draw the needles of a clock
 * @param clock pointer to clock object
 * @param mask mask of drawing
 */
static void lv_clock_draw_needle(lv_obj_t *clock, const lv_area_t *clip_area)
{
    lv_clock_ext_t *ext     = lv_obj_get_ext_attr(clock);

    //lv_style_list_t * list = lv_obj_get_style_list(clock, LV_CLOCK_PART_NEEDLE);
    //lv_style_set_line_width(*list->style_list, LV_STATE_DEFAULT, 20);

    lv_style_int_t pad = lv_obj_get_style_pad_inner(clock, LV_CLOCK_PART_NEEDLE);
    lv_style_int_t left = lv_obj_get_style_pad_left(clock, LV_CLOCK_PART_MAIN);
    lv_style_int_t right = lv_obj_get_style_pad_right(clock, LV_CLOCK_PART_MAIN);
    lv_style_int_t top = lv_obj_get_style_pad_top(clock, LV_CLOCK_PART_MAIN);

    lv_coord_t r      = (lv_obj_get_width(clock) - left - right) / 2 - pad;
    lv_coord_t x_ofs  = clock->coords.x1 + r + left + pad;
    lv_coord_t y_ofs  = clock->coords.y1 + r + top + pad;
    uint16_t angle    = lv_linemeter_get_scale_angle(clock);
    int16_t angle_ofs = 90 + (angle) / 2 + lv_clock_get_angle_offset(clock);
    lv_point_t p_mid;
    lv_point_t p_end;
    uint8_t i;

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    lv_obj_init_draw_line_dsc(clock, LV_CLOCK_PART_NEEDLE, &line_dsc);

    p_mid.x = x_ofs;
    p_mid.y = y_ofs;
    for (i = 0; i < NEEDLE_MAX_CONT; i++)
    {
        int16_t needle_angle;
        if (i == NEEDLE_HOUR)
        {
            needle_angle  = (ext->time->hour % 12) * 30 + angle_ofs;
            needle_angle += (ext->time->min) * 30 / 60;
        }
        else if (i == NEEDLE_MIN)
        {
            needle_angle  = (ext->time->min) * 6 + angle_ofs;
            needle_angle += (ext->time->sec) * 6 / 60;
        }
        else
        {
            needle_angle = (ext->time->sec) * 6 + angle_ofs;
        }

        /*Draw line*/
        if (ext->needle_style[i].img == NULL)
        {
            uint16_t length = ext->needle_style[i].length;
            if ((0 == length) || (length > r)) length = r;

            int16_t centre = ext->needle_style[i].centre;
            if (centre >= length) centre = 0;

            if (ext->needle_style[i].color.full != 0) line_dsc.color = ext->needle_style[i].color;
            if (ext->needle_style[i].width) line_dsc.width = ext->needle_style[i].width;

            p_mid.y = (_lv_trigo_sin(needle_angle) * (-centre)) / LV_TRIGO_SIN_MAX + y_ofs;
            p_mid.x = (_lv_trigo_sin(needle_angle + 90) * (-centre)) / LV_TRIGO_SIN_MAX + x_ofs;

            p_end.y = (_lv_trigo_sin(needle_angle) * length) / LV_TRIGO_SIN_MAX + y_ofs;
            p_end.x = (_lv_trigo_sin(needle_angle + 90) * length) / LV_TRIGO_SIN_MAX + x_ofs;

            lv_draw_line(&p_mid, &p_end, clip_area, &line_dsc);

            if (ext->needle_style[i].hollow_width != 0)
            {
                if (ext->needle_style[i].hollow_width < line_dsc.width)
                    line_dsc.width = ext->needle_style[i].hollow_width;

                if (ext->needle_style[i].hollow_color.full != 0)
                    line_dsc.color = ext->needle_style[i].hollow_color;

                if (ext->needle_style[i].hollow_lpad < length)
                    centre = ext->needle_style[i].hollow_lpad;

                if (ext->needle_style[i].hollow_rpad < length)
                    length -= ext->needle_style[i].hollow_rpad;

                p_mid.y = (_lv_trigo_sin(needle_angle) * centre) / LV_TRIGO_SIN_MAX + y_ofs;
                p_mid.x = (_lv_trigo_sin(needle_angle + 90) * centre) / LV_TRIGO_SIN_MAX + x_ofs;

                p_end.y = (_lv_trigo_sin(needle_angle) * length) / LV_TRIGO_SIN_MAX + y_ofs;
                p_end.x = (_lv_trigo_sin(needle_angle + 90) * length) / LV_TRIGO_SIN_MAX + x_ofs;

                lv_draw_line(&p_mid, &p_end, clip_area, &line_dsc);
            }
        }
        /*Draw image*/
        else
        {
            lv_img_header_t info;
            lv_img_decoder_get_info(ext->needle_style[i].img, &info);

            lv_area_t a;
            a.x1 = clock->coords.x1 + lv_area_get_width(&clock->coords) / 2 - ext->needle_style[i].centre;
            a.y1 = clock->coords.y1 + lv_area_get_height(&clock->coords) / 2 - info.h / 2;
            a.x2 = a.x1 + info.w - 1;
            a.y2 = a.y1 + info.h - 1;
            lv_draw_img_dsc_t img_dsc;
            lv_draw_img_dsc_init(&img_dsc);
            lv_obj_init_draw_img_dsc(clock, LV_CLOCK_PART_MAIN, &img_dsc);
            img_dsc.pivot.x = ext->needle_style[i].centre;
            img_dsc.pivot.y = info.h / 2;

            needle_angle = (needle_angle * 10);
            if (needle_angle > 3600) needle_angle -= 3600;
            img_dsc.angle = needle_angle;
            lv_draw_img(&a, clip_area, ext->needle_style[i].img, &img_dsc);
        }
    }

    if (ext->panel_style.centre_img == NULL)
    {
        lv_draw_rect_dsc_t mid_dsc;
        lv_draw_rect_dsc_init(&mid_dsc);
        lv_obj_init_draw_rect_dsc(clock, LV_CLOCK_PART_NEEDLE, &mid_dsc);
        lv_style_int_t size = lv_obj_get_style_size(clock, LV_CLOCK_PART_NEEDLE) / 2;
        lv_area_t nm_cord;
        nm_cord.x1 = x_ofs - size;
        nm_cord.y1 = y_ofs - size;
        nm_cord.x2 = x_ofs + size;
        nm_cord.y2 = y_ofs + size;
        lv_draw_rect(&nm_cord, clip_area, &mid_dsc);
    }
    else
    {
        lv_img_header_t info;
        lv_img_decoder_get_info(ext->panel_style.centre_img, &info);

        lv_area_t a;
        a.x1 = clock->coords.x1 + lv_area_get_width(&clock->coords) / 2 - info.w / 2;
        a.y1 = clock->coords.y1 + lv_area_get_height(&clock->coords) / 2 - info.h / 2;
        a.x2 = a.x1 + info.w - 1;
        a.y2 = a.y1 + info.h - 1;
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_obj_init_draw_img_dsc(clock, LV_CLOCK_PART_MAIN, &img_dsc);

        lv_draw_img(&a, clip_area, ext->panel_style.centre_img, &img_dsc);
    }
}

/**
 * Draw the needles of a clock
 * @param clock pointer to clock object
 * @param mask mask of drawing
 */
static void lv_clock_draw_pannel(lv_obj_t *clock, const lv_area_t *clip_area)
{
    lv_clock_ext_t *ext     = lv_obj_get_ext_attr(clock);

    if (ext->panel_style.bk_img)
    {
        lv_img_header_t info;
        lv_img_decoder_get_info(ext->panel_style.bk_img, &info);

        lv_area_t a;
        a.x1 = clock->coords.x1;
        a.y1 = clock->coords.y1;
        a.x2 = a.x1 + lv_area_get_width(&clock->coords) - 1;
        a.y2 = a.y1 + lv_area_get_height(&clock->coords) - 1;
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_obj_init_draw_img_dsc(clock, LV_CLOCK_PART_MAIN, &img_dsc);

        lv_draw_img(&a, clip_area, ext->panel_style.bk_img, &img_dsc);
    }
}

#endif
