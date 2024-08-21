/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-04     Jair Wu      First version
 * 2020-04-24     Jair Wu      Move to top layer
 *
 */

#include "recpen.h"

#define DBG_SECTION_NAME    "DIALOG"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

static lv_obj_t *dialog_box = NULL;
static lv_obj_t *dialog_box_text;
static lv_obj_t *dialog_pick;
static lv_obj_t *label_yes;
static lv_obj_t *label_no;

static dialog_callback g_callback = NULL;
static int g_value;

static char label_str[2][RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    {"确认", "Yes"},
    {"取消", "No"}
};

int dialog_box_key(size_t type, size_t value)
{
    if (!dialog_box)
        return DIALOG_BOX_NONE;

    if (type == TYPE_KEY_PRESS)
    {
        switch (value)
        {
        case KEY_DOWN:
        case KEY_UP:
            if (g_value == DIALOG_BOX_CONFIRM)
            {
                g_value = DIALOG_BOX_CANCEL;

                lv_obj_align(dialog_pick, label_no, LV_ALIGN_CENTER, 0, 0);
            }
            else
            {
                g_value = DIALOG_BOX_CONFIRM;

                lv_obj_align(dialog_pick, label_yes, LV_ALIGN_CENTER, 0, 0);
            }
            break;
        case KEY_CANCEL:
            g_callback(DIALOG_BOX_CANCEL);
            dialog_box_deinit();
            g_callback = NULL;
            break;
        case KEY_OK:
            g_callback(g_value);
            dialog_box_deinit();
            g_callback = NULL;
            break;
        default:
            break;
        }
    }

    return DIALOG_BOX_EXIST;
}

void dialog_box_init(const char *str, dialog_callback cb)
{
    int x, y;

    if (!dialog_box && !g_callback)
    {
        g_callback = cb;
        g_value = DIALOG_BOX_CONFIRM;

        dialog_box = lv_obj_create(lv_layer_top(), NULL);
        lv_obj_add_style(dialog_box, LV_OBJ_PART_MAIN, &label_style_font20);
        lv_obj_set_size(dialog_box, SCREEN_W, SCREEN_H);
        lv_obj_align(dialog_box, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

        dialog_box_text = lv_label_create(dialog_box, NULL);
        lv_label_set_long_mode(dialog_box_text, LV_LABEL_LONG_BREAK);
        lv_label_set_align(dialog_box_text, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(dialog_box_text, LABEL_W, LABEL_H);
        lv_obj_align(dialog_box_text, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
        lv_label_set_text(dialog_box_text, str);

        dialog_pick = lv_obj_create(dialog_box, NULL);
        lv_obj_add_style(dialog_pick, LV_OBJ_PART_MAIN, &style_bar);
        lv_obj_set_size(dialog_pick, LABEL_W, LABEL_H + LABEL_Y_GAP * 2);

        label_yes = lv_label_create(dialog_box, NULL);
        lv_label_set_long_mode(label_yes, LV_LABEL_LONG_BREAK);
        lv_label_set_align(label_yes, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(label_yes, LABEL_W, LABEL_H);
        lv_label_set_text(label_yes, label_str[0][g_app_info.language]);
        lv_obj_add_style(label_yes, LV_OBJ_PART_MAIN, &label_style_font20_hl);

        label_no = lv_label_create(dialog_box, NULL);
        lv_label_set_long_mode(label_no, LV_LABEL_LONG_BREAK);
        lv_label_set_align(label_no, LV_LABEL_ALIGN_CENTER);
        lv_obj_set_size(label_no, LABEL_W, LABEL_H);
        lv_label_set_text(label_no, label_str[1][g_app_info.language]);
        lv_obj_add_style(label_no, LV_OBJ_PART_MAIN, &label_style_font20_hl);

        x = 0;
        y = -LABEL_Y_GAP;
        lv_obj_align(label_no, NULL, LV_ALIGN_IN_BOTTOM_MID, x, y);
        y -= (lv_obj_get_height(label_no) + LABEL_Y_GAP);
        lv_obj_align(label_yes, NULL, LV_ALIGN_IN_BOTTOM_MID, x, y);
        lv_obj_align(dialog_pick, label_yes, LV_ALIGN_CENTER, 0, 0);
    }
}

void dialog_box_deinit(void)
{
    if (dialog_box)
    {
        lv_obj_clean_style_list(label_no, LV_OBJ_PART_MAIN);
        lv_obj_clean_style_list(label_yes, LV_OBJ_PART_MAIN);
        lv_obj_clean_style_list(dialog_pick, LV_OBJ_PART_MAIN);
        lv_obj_clean_style_list(dialog_box, LV_OBJ_PART_MAIN);

        lv_obj_del(dialog_box);
        dialog_box = NULL;
    }
}
