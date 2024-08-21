/**
 * @file lv_clock.h
 *
 */

#ifndef LV_CLOCK_H
#define LV_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"

#if LV_USE_CLOCK != 0

/*Testing of dependencies*/
#if LV_USE_LINEMETER == 0
#error "lv_clock: lv_linemeter is required. Enable it in lv_conf.h (LV_USE_LINEMETER  1) "
#endif

#include "../lv_core/lv_obj.h"
#include "lv_linemeter.h"
#include "lv_label.h"
#include "lv_line.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef void (*lv_clock_format_cb_t)(lv_obj_t *clock, char *buf, int bufsize, int32_t value);

/*Needle ID*/
enum
{
    NEEDLE_HOUR = 0,
    NEEDLE_MIN,
    NEEDLE_SEC,
    NEEDLE_MAX_CONT,
};

/*Needle ID*/
enum
{
    LABEL_CONT_NULL = 0,
    LABEL_CONT_FOUR = 4,
    LABEL_CONT_TWELVE = 12,
};

typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} lv_clock_time_t;

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekdays;
} lv_clock_date_t;

typedef struct
{
    lv_color_t color;
    uint16_t   width;
    uint16_t   length;
    int16_t    centre;

    lv_color_t hollow_color;
    uint16_t   hollow_width; /*width padding*/
    uint16_t   hollow_lpad; /*left padding*/
    uint16_t   hollow_rpad; /*right padding*/

    const void *img;
} lv_clock_needle_style_t;

typedef struct
{
    uint8_t label_count;
    const void *bk_img;
    const void *centre_img;
} lv_clock_panel_style_t;

/*Data of clock*/
typedef struct
{
    lv_linemeter_ext_t lmeter; /*Ext. of ancestor*/
    lv_clock_date_t *date;
    lv_clock_time_t *time;
    lv_clock_panel_style_t  panel_style;
    lv_clock_needle_style_t needle_style[NEEDLE_MAX_CONT];
    lv_task_t *task;
    lv_style_list_t style_needle;
    lv_style_list_t style_strong;
    uint8_t label_count;              /*Number of labels on the scale*/
    lv_clock_format_cb_t format_cb;
} lv_clock_ext_t;

/*Styles*/
enum
{
    LV_CLOCK_PART_MAIN = LV_LINEMETER_PART_MAIN,
    LV_CLOCK_PART_MAJOR = _LV_LINEMETER_PART_VIRTUAL_LAST,
    LV_CLOCK_PART_NEEDLE,
    _LV_CLOCK_PART_VIRTUAL_LAST = _LV_LINEMETER_PART_VIRTUAL_LAST,
    _LV_CLOCK_PART_REAL_LAST = _LV_LINEMETER_PART_REAL_LAST,
};
typedef uint8_t lv_clock_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a clock objects
 * @param par pointer to an object, it will be the parent of the new clock
 * @param copy pointer to a clock object, if not NULL then the new object will be copied from it
 * @return pointer to the created clock
 */
lv_obj_t *lv_clock_create(lv_obj_t *par, const lv_obj_t *copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the clock date
 * @param clock pointer to a clock
 * @param date the date value pointer
 */
void lv_clock_set_date(lv_obj_t *clock, lv_clock_date_t *date);

/**
 * Set the time of a clock
 * @param clock pointer to a clock
 * @param time the pointer of time
 */
void lv_clock_set_time(lv_obj_t *clock, lv_clock_time_t *time);

/**
 * Set the style of a needle
 * @param clock pointer to a clock
 * @param needle the style of the needles
 */
void lv_clock_set_needle_style(lv_obj_t *clock, lv_clock_needle_style_t *needle);

/**
 * Set the style of a pannel
 * @param clock pointer to a clock
 * @param panel the style of the pannel
 */
void lv_clock_set_panel_style(lv_obj_t *clock, lv_clock_panel_style_t *panel);

/**
 * Set minimum and the maximum values of a clock
 * @param clock pointer to he clock object
 * @param min minimum value
 * @param max maximum value
 */
static inline void lv_clock_set_range(lv_obj_t *clock, int32_t min, int32_t max)
{
    lv_linemeter_set_range(clock, min, max);
}

/**
 * Set a critical value on the scale. After this value 'line.color' scale lines will be drawn
 * @param clock pointer to a clock object
 * @param value the critical value
 */
static inline void lv_clock_set_critical_value(lv_obj_t *clock, int32_t value)
{
    lv_linemeter_set_value(clock, value);
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
void lv_clock_set_scale(lv_obj_t *clock, uint16_t angle, uint8_t line_cnt);

/**
 * Set the set an offset for the clock's angles to rotate it.
 * @param clock pointer to a line meter object
 * @param angle angle offset (0..360), rotates clockwise
 */
static inline void lv_clock_set_angle_offset(lv_obj_t *clock, uint16_t angle)
{
    lv_linemeter_set_angle_offset(clock, angle);
}

/**
 * Assign a function to format clock values
 * @param clock pointer to a clock object
 * @param format_cb pointer to function of lv_clock_format_cb_t
 */
void lv_clock_set_formatter_cb(lv_obj_t *clock, lv_clock_format_cb_t format_cb);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the minimum value of a clock
 * @param clock pointer to a clock object
 * @return the minimum value of the clock
 */
static inline int32_t lv_clock_get_min_value(const lv_obj_t *lmeter)
{
    return lv_linemeter_get_min_value(lmeter);
}

/**
 * Get the maximum value of a clock
 * @param clock pointer to a clock object
 * @return the maximum value of the clock
 */
static inline int32_t lv_clock_get_max_value(const lv_obj_t *lmeter)
{
    return lv_linemeter_get_max_value(lmeter);
}

/**
 * Get a critical value on the scale.
 * @param clock pointer to a clock object
 * @return the critical value
 */
static inline int32_t lv_clock_get_critical_value(const lv_obj_t *clock)
{
    return lv_linemeter_get_value(clock);
}

/**
 * Get the scale number of a clock
 * @param clock pointer to a clock object
 * @return number of the scale units
 */
static inline uint16_t lv_clock_get_line_count(const lv_obj_t *clock)
{
    return lv_linemeter_get_line_count(clock);
}

/**
 * Get the scale angle of a clock
 * @param clock pointer to a clock object
 * @return angle of the scale
 */
static inline uint16_t lv_clock_get_scale_angle(const lv_obj_t *clock)
{
    return lv_linemeter_get_scale_angle(clock);
}

/**
 * Get the offset for the clock.
 * @param clock pointer to a clock object
 * @return angle offset (0..360)
 */
static inline uint16_t lv_clock_get_angle_offset(lv_obj_t *clock)
{
    return lv_linemeter_get_angle_offset(clock);
}

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_CLOCK*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_CLOCK_H*/
