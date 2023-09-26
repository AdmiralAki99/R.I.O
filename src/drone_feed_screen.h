/**
 * @file drone_feed_screen.h
 * @author Akhilesh Warty
 * @brief Drone Feed Screen for Smart Watch (Experimental)
 * @version 0.1
 * @date 2023-09-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "parsecs.h"
#include "video_player.h"

static lv_obj_t* drone_feed_screen;
static lv_obj_t* video_feed;
static lv_color_t feed_buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(100,100)];

VideoPlayer video_player;



void init_screen(lv_obj_t* screen);
void init_video_feed();

void init_drone_feed_screen(lv_obj_t* screen){
    drone_feed_screen = screen;
    lv_obj_set_style_bg_color(screen,LV_COLOR_BLACK,0);
    init_video_feed();

}

void init_video_feed(){
   

}
