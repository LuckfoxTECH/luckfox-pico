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

#include "rk_lv_menulist.h"

#define RK_LV_MENULIST_TITAL_H  28
#define RK_LV_MENULIST_LABEL_H  48

rk_lv_menulist_t *rk_lv_menulist_create(char *title, lv_obj_t * par, const lv_obj_t * copy, rk_lv_ml_element_t *elements, int elements_num)
{
    if (elements == NULL || elements_num <= 0)
        return NULL;

    rk_lv_menulist_t *ml = (rk_lv_menulist_t *)malloc(sizeof(rk_lv_menulist_t));
    memset((void *)ml, 0, sizeof(rk_lv_menulist_t));

    ml->page = lv_obj_create(par, copy);
    lv_page_set_scrlbar_mode(ml->page, LV_SCRLBAR_MODE_OFF);
    ml->title = lv_label_create(ml->page, copy);
    ml->pick = lv_obj_create(ml->page, copy);

    ml->elements = elements;
    ml->max_items = elements_num;

    lv_label_set_text(ml->title, title);

    return ml;
}

void rk_lv_menulist_remove_style(rk_lv_menulist_t *ml, int list_only)
{
    if (ml->style)
    {
        if (!list_only)
        {
            lv_obj_remove_style(ml->page, LV_OBJ_PART_MAIN, ml->style);
            lv_obj_remove_style(ml->title, LV_OBJ_PART_MAIN, ml->style);
            lv_obj_remove_style(ml->pick, LV_OBJ_PART_MAIN, ml->style2);
            ml->style = NULL;
            ml->style2 = NULL;
        }

        if (ml->label)
        {
            for (int i = 0; i < ml->items_per_page; i++)
                lv_obj_remove_style(ml->label[i], LV_OBJ_PART_MAIN, ml->style);
        }
    }
}

void rk_lv_menulist_destroy(rk_lv_menulist_t *ml)
{
    rk_lv_menulist_remove_style(ml, 0);
    lv_obj_del(ml->page);
    free((void *)ml->label);
    free((void *)ml);
}

static void rk_lv_menulist_refresh(rk_lv_menulist_t *ml)
{
    for (int i = 0, j = ml->start_item; i < ml->items_per_page; i++, j++)
        lv_label_set_text(ml->label[i], ml->elements[j].str[ml->lan]);

    lv_obj_align(ml->pick, ml->label[ml->cur_item], LV_ALIGN_CENTER, 0, 0);
}

void rk_lv_menulist_set_size(rk_lv_menulist_t *ml, uint32_t w, uint32_t h)
{
    uint32_t items_per_page;
    int16_t y = RK_LV_MENULIST_TITAL_H;
    int16_t pad;

    if (h <(RK_LV_MENULIST_TITAL_H + RK_LV_MENULIST_LABEL_H))
    {
        rt_kprintf("Height should at least %d\n", RK_LV_MENULIST_TITAL_H + RK_LV_MENULIST_LABEL_H);
        return;
    }

    lv_obj_set_size(ml->page, w, h);

    pad = 4;
    lv_label_set_long_mode(ml->title, LV_LABEL_LONG_SROLL);
    lv_obj_set_size(ml->title, w - pad * 2, RK_LV_MENULIST_LABEL_H);
    lv_obj_align(ml->title, ml->page, LV_ALIGN_IN_TOP_LEFT, pad, 2);

    lv_obj_set_size(ml->pick, w, RK_LV_MENULIST_LABEL_H);
    lv_obj_align(ml->pick, ml->page, LV_ALIGN_IN_TOP_MID, 0, y);

    items_per_page = MIN((h - RK_LV_MENULIST_TITAL_H) / RK_LV_MENULIST_LABEL_H, ml->max_items);
    if (ml->label && ml->items_per_page != items_per_page)
    {
        rk_lv_menulist_remove_style(ml, 1);
        for (int i = 0; i < ml->items_per_page; i++)
            lv_obj_del(ml->label[i]);
        free(ml->label);
        ml->label = NULL;
    }
    if (ml->label == NULL)
    {
        ml->items_per_page = items_per_page;
        ml->label = malloc(sizeof(void *) * ml->items_per_page);
        for (int i = 0; i < ml->items_per_page; i++)
        {
            ml->label[i] = lv_label_create(ml->page, NULL);
            lv_label_set_long_mode(ml->label[i], LV_LABEL_LONG_CROP);
            lv_obj_align(ml->label[i], ml->page, LV_ALIGN_IN_TOP_LEFT, 0, y + (RK_LV_MENULIST_LABEL_H - SETTING_LABEL_H) / 2);
            lv_obj_set_size(ml->label[i], w, SETTING_LABEL_H);
            if (ml->style)
                lv_obj_add_style(ml->label[i], LV_OBJ_PART_MAIN, ml->style);
            y += RK_LV_MENULIST_LABEL_H;
        }
    }

    rk_lv_menulist_refresh(ml);
}

void rk_lv_menulist_prev(rk_lv_menulist_t *ml)
{
    if (ml->cur_item > 0)
    {
        ml->cur_item--;
    }
    else
    {
        ml->start_item--;
        if (ml->start_item < 0)
        {
            ml->start_item = ml->max_items - ml->items_per_page;
            ml->cur_item = ml->items_per_page - 1;
        }
    }

    rk_lv_menulist_refresh(ml);
}

void rk_lv_menulist_next(rk_lv_menulist_t *ml)
{
    if (ml->cur_item < (ml->items_per_page - 1))
    {
        ml->cur_item++;
    }
    else
    {
        ml->start_item++;
        if ((ml->start_item + ml->items_per_page) > ml->max_items)
        {
            ml->start_item = 0;
            ml->cur_item = 0;
        }
    }

    rk_lv_menulist_refresh(ml);
}

void rk_lv_menulist_select(rk_lv_menulist_t *ml, int index)
{
    ml->cur_item = ml->start_item + index;
    if (ml->cur_item >= ml->max_items)
        ml->cur_item = ml->max_items - 1;
    if (ml->cur_item >= ml->items_per_page)
    {
        ml->start_item = ml->cur_item - (ml->items_per_page - 1);
        ml->cur_item = ml->items_per_page - 1;
    }

    rk_lv_menulist_refresh(ml);
}

void rk_lv_menulist_excute(rk_lv_menulist_t *ml)
{
    int index = ml->start_item + ml->cur_item;
    if (ml->elements[index].cb)
        ml->elements[index].cb(ml->elements[index].arg);
}

void rk_lv_menulist_set_style(rk_lv_menulist_t *ml, lv_style_t * style, lv_style_t * style2)
{
    rk_lv_menulist_remove_style(ml, 0);
    lv_obj_add_style(ml->page, LV_OBJ_PART_MAIN, style);
    lv_obj_add_style(ml->title, LV_OBJ_PART_MAIN, style);
    lv_obj_add_style(ml->pick, LV_OBJ_PART_MAIN, style2);

    rk_lv_menulist_refresh(ml);
    ml->style = style;
    ml->style2 = style2;
}

void rk_lv_menulist_set_language(rk_lv_menulist_t *ml, enum recpen_language lan)
{
    if (!ml)
        return;

    ml->lan = lan;
    rk_lv_menulist_refresh(ml);
}

void rk_lv_menulist_modify(rk_lv_menulist_t *ml, char *title, rk_lv_ml_element_t *elements, int elements_num)
{
    int32_t h, w;

    if (elements == NULL || elements_num <= 0)
        return;

    if (title)
        lv_label_set_text(ml->title, title);

    ml->start_item = 0;
    ml->cur_item = 0;
    ml->elements = elements;
    ml->max_items = elements_num;

    h = lv_obj_get_height(ml->page);
    w = lv_obj_get_width(ml->page);
    rk_lv_menulist_set_size(ml, w, h);
}
