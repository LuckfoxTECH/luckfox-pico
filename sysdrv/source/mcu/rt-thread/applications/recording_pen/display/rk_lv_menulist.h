/*
 * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-17     Jair Wu      First version
 *
 */

#ifndef __RK_LV_MENULIST_H__
#define __RK_LV_MENULIST_H__

#include "recpen.h"

typedef struct rk_lv_ml_element
{
    char str[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN];
    void (*cb)(void *arg);
    void *arg;
} rk_lv_ml_element_t;

typedef struct rk_lv_menulist
{
    lv_obj_t *page;
    lv_obj_t *title;
    lv_obj_t *pick;
    lv_obj_t **label;
    lv_style_t *style;
    lv_style_t *style2;
    int max_items;
    int items_per_page;
    int cur_item;
    int start_item;
    rk_lv_ml_element_t *elements;
    enum recpen_language lan;
} rk_lv_menulist_t;

rk_lv_menulist_t *rk_lv_menulist_create(char *title, lv_obj_t * par, const lv_obj_t * copy,
                                        rk_lv_ml_element_t *elements, int elements_num);
void rk_lv_menulist_destroy(rk_lv_menulist_t *ml);
void rk_lv_menulist_set_size(rk_lv_menulist_t *ml, uint32_t w, uint32_t h);
void rk_lv_menulist_prev(rk_lv_menulist_t *ml);
void rk_lv_menulist_next(rk_lv_menulist_t *ml);
void rk_lv_menulist_select(rk_lv_menulist_t *ml, int index);
void rk_lv_menulist_excute(rk_lv_menulist_t *ml);
void rk_lv_menulist_set_style(rk_lv_menulist_t *ml, lv_style_t * style, lv_style_t * style2);
void rk_lv_menulist_set_language(rk_lv_menulist_t *ml, enum recpen_language lan);
void rk_lv_menulist_modify(rk_lv_menulist_t *ml, char *title, rk_lv_ml_element_t *elements, int elements_num);

#endif
