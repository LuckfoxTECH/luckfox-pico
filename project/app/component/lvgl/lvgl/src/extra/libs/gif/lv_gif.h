/**
 * @file lv_gif.h
 *
 */

#ifndef LV_GIF_H
#define LV_GIF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../../lvgl.h"
#if LV_USE_GIF

#include "gifdec.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
#if LV_GIF_CACHE
typedef struct lv_gif_canvas {
  uint8_t *data;
  struct lv_gif_canvas *next;
} lv_gif_canvas_t;
#endif

typedef struct {
  lv_img_t img;
  gd_GIF *gif;
  lv_timer_t *timer;
  lv_img_dsc_t imgdsc;
  uint32_t last_call;
#if LV_GIF_CACHE
  lv_gif_canvas_t *canvas_list;
  lv_gif_canvas_t *canvas_cur;
#endif
} lv_gif_t;

extern const lv_obj_class_t lv_gif_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t *lv_gif_create(lv_obj_t *parent);
void lv_gif_set_src(lv_obj_t *obj, const void *src);
void lv_gif_restart(lv_obj_t *gif);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_GIF*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_GIF_H*/
