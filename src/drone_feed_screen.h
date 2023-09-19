#include <lvgl.h>
#include "config.h"

static lv_obj_t* drone_feed_screen;
static lv_obj_t* video_feed;
static lv_color_t feed_buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(100,100)];

void init_screen(lv_obj_t* screen);
void init_video_feed();

void init_drone_feed_screen(lv_obj_t* screen){
    drone_feed_screen = screen;
    lv_obj_set_style_bg_color(screen,LV_COLOR_BLACK,0);
    init_video_feed();

}

void init_video_feed(){
    video_feed = lv_img_create(drone_feed_screen);
    lv_img_set_src(video_feed,"/Users/akhilesh/Documents/Repos/WatchUI/src/space.jpg");

    lv_obj_set_pos(video_feed,0,0);
    lv_obj_set_size(video_feed,100,100);

    lv_obj_set_style_bg_color(video_feed,LV_COLOR_WHITE,0);

}
