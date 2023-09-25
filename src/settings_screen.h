/**
 * @file settings_screen.h
 * @author Akhilesh Warty
 * @brief Settings Screen of Smart Watch
 * @version 0.1
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "parsecs.h"

#define LV_COLOR_BLACK  LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_GREY LV_COLOR_MAKE(0x80, 0x80, 0x80)

char* settings_label[] = {"Time", "Music", "Settings", "About","Set Time"};

static lv_obj_t* scroll_menu;

void init_settings_scroll_menu(lv_obj_t* scr);
void settings_scroll_event_cb(lv_event_t * e);

void init_settings_screen(lv_obj_t* scr){
    init_settings_scroll_menu(scr);
}

void init_settings_scroll_menu(lv_obj_t* scr){
    lv_obj_set_style_bg_color(scr, LV_COLOR_BLACK, 0);
    scroll_menu = lv_obj_create(scr);
    lv_obj_set_size(scroll_menu, 240, 240);
    lv_obj_align(scroll_menu, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(scroll_menu, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_color(scroll_menu, LV_COLOR_BLACK, 0);
    lv_obj_add_event_cb(scroll_menu, settings_scroll_event_cb, LV_EVENT_SCROLL, NULL);

    lv_obj_set_style_radius(scroll_menu, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(scroll_menu, 0, 0);
    lv_obj_set_style_clip_corner(scroll_menu, true, 0);
    lv_obj_set_scroll_dir(scroll_menu, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(scroll_menu, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(scroll_menu, LV_SCROLLBAR_MODE_OFF);

    for(int i = 0;i<sizeof(settings_label)/sizeof(settings_label[0]);i++){
        lv_obj_t* scroll_btn = lv_btn_create(scroll_menu);
        lv_obj_set_width(scroll_btn, 200);
        lv_obj_set_style_bg_color(scroll_btn, lv_palette_main(LV_PALETTE_CYAN), 0);

        lv_obj_t* scroll_label = lv_label_create(scroll_btn);
        lv_label_set_text(scroll_label, settings_label[i]);
    }

    lv_event_send(scroll_menu, LV_EVENT_SCROLL, NULL);
    lv_obj_scroll_to_view(lv_obj_get_child(scroll_menu, 0), LV_ANIM_OFF);
    lv_obj_set_style_anim_speed(scroll_menu, 100, 0);

}

void settings_scroll_event_cb(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        } else {
            /*Use Pythagoras theorem to get x from radius and y*/
            lv_coord_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

//todo: Change Settings Screen

