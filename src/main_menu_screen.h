/**
 * @file main_menu_screen.h
 * @author Akhilesh
 * @brief Main Menu Screen of the Smartwatch
 * @version 0.3
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "parsecs.h"

#include "music_screen.h"
#include "drone_feed_screen.h"
#include "todoList_screen.h"

lv_obj_t* main_menu_screen;
lv_obj_t* temp_scr = NULL;
lv_obj_t* load_scr;

static lv_obj_t* main_menu;
static lv_obj_t* back_button;
static lv_style_t back_button_style;



static lv_obj_t* hour_time_roller;
static lv_obj_t* minute_time_roller;
static lv_obj_t* time_selection_button;

static lv_style_t time_roller_style;
static lv_style_t time_selection_button_style;

const char* minute_option = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n";
const char* hour_option = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24";

void init_main_menu_screen(lv_obj_t* scr);
void init_main_menu_screen_scroll(lv_obj_t* scr);
void scroll_event_main_menu_cb(lv_event_t * e);
void main_menu_event_cb(lv_event_t * e);
void init_back_button(lv_obj_t* scr);
void initialize_back_button_style(lv_style_t* back_button_style);
void back_button_event_cb(lv_event_t * e);
void music_button_cb(lv_event_t * e);
void drone_feed_button_cb(lv_event_t* e);
void init_time_selection(lv_obj_t* scr);
void init_time_roller_style(lv_style_t* style);
void time_roller_cb(lv_event_t* e);
void set_time_button_cb(lv_event_t* e);
void weather_button_cb(lv_event_t* e);
void todoList_button_cb(lv_event_t* e);
void change_time_button_cb(lv_event_t* e);
void update_button_cb(lv_event_t* e);
void settings_button_cb(lv_event_t* e);
void init_time_selection_button();
void init_time_selection_button_style();
void time_selection_screen_event_cb(lv_event_t* event);

char* menu_label[] = {"Music", "Drone Feed", "Weather","To-do List","Set Time","Update","Settings"};

void (*menu_functions[])(lv_event_t*) = {music_button_cb,drone_feed_button_cb,weather_button_cb,todoList_button_cb,set_time_button_cb};

void init_main_menu_screen(lv_obj_t* scr){
    main_menu_screen = scr;
    init_main_menu_screen_scroll(scr);
    lv_obj_add_event_cb(scr,main_menu_event_cb,LV_EVENT_GESTURE,NULL);
}

void init_main_menu_screen_scroll(lv_obj_t* scr){
    // lv_obj_set_style_bg_color(scr, LV_COLOR_BLACK, 0);
    // main_menu = lv_obj_create(scr);
    // lv_obj_set_size(main_menu, SCREEN_WIDTH,SCREEN_HEIGHT);
    // lv_obj_align(main_menu, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_flex_flow(main_menu, LV_FLEX_FLOW_COLUMN);
    // lv_obj_set_style_bg_color(main_menu, LV_COLOR_BLACK, 0);
    // // lv_obj_add_event_cb(main_menu, scroll_event_main_menu_cb, LV_EVENT_SCROLL, NULL);
    // lv_obj_add_event_cb(scr, main_menu_event_cb, LV_EVENT_GESTURE, NULL);

    // lv_obj_set_style_radius(main_menu, LV_RADIUS_CIRCLE, 0);
    // lv_obj_set_style_border_width(main_menu, 0, 0);
    // lv_obj_set_style_clip_corner(main_menu, true, 0);
    // lv_obj_set_scroll_dir(main_menu, LV_DIR_VER);
    // lv_obj_set_scroll_snap_y(main_menu, LV_SCROLL_SNAP_CENTER);
    // lv_obj_set_scrollbar_mode(main_menu, LV_SCROLLBAR_MODE_OFF);

    // for(int i = 0;i<sizeof(menu_label)/sizeof(menu_label[0]);i++){
    //     lv_obj_t* scroll_btn = lv_btn_create(main_menu);
    //     lv_obj_set_width(scroll_btn, 200);
    //     lv_obj_set_style_bg_color(scroll_btn, lv_palette_main(LV_PALETTE_CYAN), 0);
    //     lv_obj_set_style_bg_opa(scroll_btn, LV_OPA_TRANSP, 0);
    //     lv_obj_set_style_border_width(scroll_btn, 0, 0);
    //     lv_obj_set_style_outline_opa(scroll_btn, LV_OPA_TRANSP, 0);
    //     lv_obj_set_style_outline_color(scroll_btn, lv_palette_main(LV_PALETTE_CYAN), 0);
    //     lv_obj_add_event_cb(scroll_btn, menu_functions[i], LV_EVENT_CLICKED, NULL);

    //     lv_obj_t* scroll_label = lv_label_create(scroll_btn);
    //     lv_label_set_text(scroll_label, menu_label[i]);
    // }

    // lv_event_send(main_menu, LV_EVENT_SCROLL, NULL);
    // lv_obj_scroll_to_view(lv_obj_get_child(main_menu, 0), LV_ANIM_OFF);
    // lv_obj_set_style_anim_speed(main_menu, 100, 0);

    // lv_obj_set_parent(main_menu, scr);
    lv_obj_set_style_bg_color(scr, LV_COLOR_BLACK, 0);
    main_menu = lv_list_create(scr);
    lv_obj_set_style_bg_color(main_menu, LV_COLOR_BLACK, 0);
    lv_obj_set_size(main_menu, 240,200);
    lv_obj_set_style_border_color(main_menu,LV_COLOR_BLACK,0);
    lv_obj_set_style_border_opa(main_menu,LV_OPA_TRANSP,0);
    lv_obj_align(main_menu, LV_ALIGN_CENTER, 0, 30);

    lv_obj_t* btn_temp;

    for(uint8_t i=0;i<7;i++){
        btn_temp = lv_list_add_btn(main_menu,NULL,menu_label[i]);
        lv_obj_set_style_bg_color(btn_temp,LV_COLOR_BLACK,0);
        lv_obj_set_style_text_color(btn_temp,LV_COLOR_WHITE,0);
        lv_obj_set_style_border_color(btn_temp,LV_COLOR_BLACK,0);
        lv_obj_set_style_border_opa(btn_temp,LV_OPA_TRANSP,0);
        lv_obj_add_event_cb(btn_temp,menu_functions[i],LV_EVENT_CLICKED,NULL);
    }

}

void scroll_event_main_menu_cb(lv_event_t * e)
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
        }
        else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
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

void main_menu_event_cb(lv_event_t * e){
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
        lv_scr_load(main_screen);
    }
}

void music_button_cb(lv_event_t* e){
    music_screen = lv_obj_create(NULL);
    init_music_screen(music_screen);
    lv_scr_load(music_screen);
    lv_obj_set_user_data(music_screen,main_menu_screen);
}

void todoList_button_cb(lv_event_t* e){
    todo_screen = lv_obj_create(NULL);
    init_todoList_screen(todo_screen);
    lv_scr_load(todo_screen);
    lv_obj_set_user_data(todo_screen,main_menu_screen);
}

void drone_feed_button_cb(lv_event_t* e){
    drone_feed_screen = lv_obj_create(NULL);
    init_drone_feed_screen(drone_feed_screen);
    lv_scr_load(drone_feed_screen);
    lv_obj_set_user_data(drone_feed_screen,main_menu_screen);
}

void update_button_cb(lv_event_t* e){
    Serial.println("Update Button Pressed");
}

void settings_button_cb(lv_event_t* e){
    Serial.println("Settings Button Pressed");
}

void init_time_roller_style(lv_style_t* style){
    lv_style_init(style);
    lv_style_set_bg_color(style, lv_color_black());
    lv_style_set_text_color(style, lv_color_white());
    lv_style_set_border_width(style, 0);
    lv_style_set_pad_all(style, 0);
    lv_obj_add_style(lv_scr_act(), style,0);
}

void init_time_selection(lv_obj_t* scr){

    static lv_style_t style_shadow;
    lv_style_init(&style_shadow);
    lv_style_set_text_opa(&style_shadow, LV_OPA_30);
    lv_style_set_text_color(&style_shadow, lv_color_black());

    /*Create a label for the shadow first (it's in the background)*/
    lv_obj_t * shadow_label = lv_label_create(lv_scr_act());
    lv_obj_add_style(shadow_label, &style_shadow, 0);

    /*Create the main label*/
    lv_obj_t * main_label = lv_label_create(lv_scr_act());
    lv_label_set_text(main_label, "Set Time");

    /*Set the same text for the shadow label*/
    lv_label_set_text(shadow_label, lv_label_get_text(main_label));

    /*Position the main label*/
    lv_obj_align(main_label, LV_ALIGN_TOP_MID, 0, 30);

    /*Shift the second label down and to the right by 2 pixel*/
    lv_obj_align_to(shadow_label, main_label, LV_ALIGN_TOP_LEFT, 2, 2);

    init_time_roller_style(&time_roller_style);
    hour_time_roller = lv_roller_create(lv_scr_act());
    lv_obj_add_style(hour_time_roller, &time_roller_style, 0);
    lv_obj_set_style_bg_opa(hour_time_roller, LV_OPA_TRANSP, LV_PART_SELECTED);

     lv_roller_set_options(hour_time_roller,hour_option
                       ,LV_ROLLER_MODE_INFINITE);

    lv_roller_set_visible_row_count(hour_time_roller, 3);
    lv_obj_align(hour_time_roller, LV_ALIGN_CENTER, -40, -10);
    lv_obj_add_event_cb(hour_time_roller, time_roller_cb, LV_EVENT_ALL, NULL);

    minute_time_roller = lv_roller_create(lv_scr_act());
    lv_obj_add_style(minute_time_roller, &time_roller_style, 0);
    lv_obj_set_style_bg_opa(minute_time_roller, LV_OPA_TRANSP, LV_PART_SELECTED);

    lv_roller_set_options(minute_time_roller,minute_option,LV_ROLLER_MODE_NORMAL);

    lv_roller_set_visible_row_count(minute_time_roller, 3);
    lv_obj_align(minute_time_roller, LV_ALIGN_CENTER, 40, -10);
    lv_obj_add_event_cb(minute_time_roller, time_roller_cb, LV_EVENT_ALL, NULL);

    init_time_selection_button();

}

void time_roller_cb(lv_event_t* e){

}

void weather_button_cb(lv_event_t* e){
    Serial.println("Weather Button Pressed");
}

void set_time_button_cb(lv_event_t* e){
    lv_obj_t* temp_scr = lv_obj_create(NULL);
    lv_scr_load(temp_scr);
    init_time_selection(temp_scr);
    lv_obj_set_user_data(temp_scr,main_menu_screen);
    lv_obj_add_event_cb(temp_scr,time_selection_screen_event_cb,LV_EVENT_GESTURE,NULL);
}

void change_time_button_cb(lv_event_t* e){
    sys_time_hour = lv_roller_get_selected(hour_time_roller);
    sys_time_minute = lv_roller_get_selected(minute_time_roller);
    sys_time_second = 0;
    if(sys_time_hour > 12){
        sys_time_hour = sys_time_hour - 12;
    }
    time_changed = true;
}

void init_time_selection_button(){
    init_time_selection_button_style();
    time_selection_button = lv_btn_create(lv_scr_act());
    lv_obj_set_size(time_selection_button,40,40);
    lv_obj_align(time_selection_button,LV_ALIGN_CENTER,0,70);
    lv_obj_add_style(time_selection_button,&time_selection_button_style,0);
    lv_obj_set_style_bg_opa(time_selection_button,0,0);
    lv_obj_add_event_cb(time_selection_button,change_time_button_cb,LV_EVENT_CLICKED,NULL);

    lv_obj_t* select_button_icon = lv_label_create(time_selection_button);
    lv_label_set_text(select_button_icon,LV_SYMBOL_OK);
    lv_obj_set_style_text_color(select_button_icon,LV_COLOR_WHITE,0);
    lv_obj_align(select_button_icon,LV_ALIGN_CENTER,0,0);
}

void init_time_selection_button_style(){
    lv_style_init(&time_selection_button_style);
    lv_style_set_bg_color(&time_selection_button_style,LV_COLOR_BLACK);
    lv_style_set_radius(&time_selection_button_style,LV_RADIUS_CIRCLE);
    lv_style_set_border_width(&time_selection_button_style, 2);
    lv_style_set_outline_width(&time_selection_button_style, 0);
    lv_style_set_border_color(&time_selection_button_style,LV_COLOR_WHITE );
    
}

void time_selection_screen_event_cb(lv_event_t* event){
    if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
        lv_scr_load(main_menu_screen);
    }
}


/**
 * Redundant Code Incase Swipe Gesture is not possible
*/

// void initialize_back_button_style(lv_style_t* back_button_style){
//     lv_style_init(back_button_style);
//     lv_style_set_border_color(back_button_style,lv_palette_main(LV_PALETTE_LIGHT_BLUE));
//     lv_style_set_bg_opa(back_button_style,LV_OPA_TRANSP);
//     lv_style_set_border_opa(back_button_style,LV_OPA_TRANSP);
//     lv_style_set_border_width(back_button_style,0);
//     lv_style_set_outline_width(back_button_style,0);
//     lv_style_set_border_color(back_button_style,LV_COLOR_BLACK);
//     lv_style_set_outline_color(back_button_style,LV_COLOR_BLACK);
//     lv_style_set_text_color(back_button_style,LV_COLOR_WHITE);
// }

// void init_back_button(lv_obj_t* scr){

//   back_button = lv_btn_create(scr);
//   lv_obj_align(back_button,LV_ALIGN_TOP_MID,-20,20);
//   lv_obj_add_style(back_button,&back_button_style,0);
//   lv_obj_set_style_bg_color(back_button,LV_COLOR_BLACK,0);
//   lv_obj_set_style_radius(back_button,LV_RADIUS_CIRCLE,0);
//   lv_obj_set_style_outline_opa(back_button,LV_OPA_TRANSP,0);
//   lv_obj_add_event_cb(back_button,back_button_event_cb,LV_EVENT_CLICKED,NULL);

//   lv_obj_t *back_icon = lv_img_create(back_button);
//   lv_img_set_src(back_icon, LV_SYMBOL_LEFT);
//   lv_obj_set_size(back_icon, 20, 20);
//   lv_obj_set_pos(back_icon, 10, 5);
//   lv_obj_set_style_text_color(back_icon, LV_COLOR_WHITE,0); // Set the icon color to white

//   lv_obj_set_parent(back_button,scr);
// }

// void back_button_event_cb(lv_event_t * e){
//     if(lv_event_get_code(e) == LV_EVENT_CLICKED){
//         Serial.println("clicked");
//     }  
// }

