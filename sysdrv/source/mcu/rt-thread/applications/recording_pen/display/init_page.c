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

#include "init_page.h"

#define DBG_SECTION_NAME    "WELCPAGE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

/*
 *  Draw welcome text or image here
 */
static lv_obj_t *welcome_label = NULL;
// static lv_obj_t *welcome_img;

void welcome_page_refresh(size_t type, size_t value)
{
    switch (type)
    {
    case TYPE_REBOOT:
        lv_label_set_text(welcome_label, "Please Reboot");
        lv_obj_align(welcome_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 60);
        break;
    default:
        break;
    }
}

void welcome_page_init(int p_page)
{
    if (!welcome_label)
    {
        welcome_label = lv_label_create(lv_scr_act(), NULL);
        lv_obj_add_style(welcome_label, LV_OBJ_PART_MAIN, &label_style_font20);
    }
    lv_label_set_align(welcome_label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(welcome_label, "Welcome");
    lv_obj_set_width(welcome_label, SCREEN_W);
    lv_obj_align(welcome_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 60);
}

void welcome_page_deinit(void)
{
    if (welcome_label)
    {
        lv_obj_remove_style(welcome_label, LV_OBJ_PART_MAIN, &label_style_font20);
        lv_obj_del(welcome_label);
    }
}
