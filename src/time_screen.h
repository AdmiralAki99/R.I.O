#pragma once

#include <lvgl.h>
#include <TimeLib.h>
#include "config.h"

#include "main_menu_screen.h"

#define LV_COLOR_BLACK  LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_GREY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define LV_COLOR_BLUE  LV_COLOR_MAKE(0x00, 0x00, 0xFF)
#define LV_COLOR_RED  LV_COLOR_MAKE(0xFF, 0x00, 0x00)

lv_obj_t* time_screen;
lv_obj_t* flashlight;

bool flashlight_mode = false;

static lv_style_t dial_style;
static lv_obj_t* dial;

static lv_style_t watch_dial_style;
static lv_obj_t* watch_dial;
static lv_point_t dial_points[2];

static lv_obj_t* linemeter;
static lv_style_t linemeter_style;

static lv_style_t line_style;
static lv_obj_t* second_line;

static lv_style_t line_style2;
static lv_obj_t* minute_line;

static lv_style_t hour_hand_style;
static lv_obj_t* hour_line;

lv_coord_t seconds_angle = 0;
lv_coord_t minutes_angle = 0;
lv_coord_t hours_angle = 0;

static lv_point_t rotation_points_seconds[2];
static lv_point_t rotation_points_minutes[2];
static lv_point_t rotation_points_hours[2];

//todo: Sort Functions that are common to Analog and Digital Watch Faces

#ifdef ANALOG_WATCH_FACE


void initialize_background_dial_style(lv_style_t* dial_style);
void initialize_background_dial(lv_obj_t* dial,lv_style_t* dial_style,lv_obj_t* act_scr);
void initialize_watch_dial_style(lv_style_t* watch_dial_style);
void initialize_buttons(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr);
void initialize_second_line_style(lv_style_t* line_style);
void initialize_second_hand(lv_obj_t** second_line, lv_style_t* line_style, lv_obj_t* act_scr);
void initialize_minute_line_style(lv_style_t* line_style2);
void initialize_minute_hand(lv_obj_t** minute_line, lv_style_t* line_style2, lv_obj_t* act_scr);
void initialize_hour_line_style(lv_style_t* hour_hand_style);
void initialize_hour_hand(lv_obj_t** hour_line, lv_style_t* hour_hand_style, lv_obj_t* act_scr);
void initialize_linemeter_style(lv_style_t* linemeter_style);
void initialize_linemeter(lv_obj_t* linemeter,lv_style_t* linemeter_style,lv_obj_t* act_scr);
void linemeter_cb(lv_event_t* event);
void screen_cb(lv_event_t* event);
void long_press_cb(lv_event_t* event);
void flashlight_cb(lv_event_t* event);
void flashlight_toggle_cb(lv_event_t* event);

static void background_dial_cb(lv_obj_t * obj, lv_event_t event);

void init_screen(lv_obj_t* scr){
  time_screen = scr;
  initialize_background_dial_style(&dial_style);
  initialize_background_dial(dial,&dial_style,scr);

  initialize_linemeter_style(&linemeter_style);
  initialize_linemeter(linemeter,&linemeter_style,scr);

  initialize_second_line_style(&line_style);
  initialize_second_hand(&second_line,&line_style,scr);
  // lv_obj_t * line = lv_line_create(lv_scr_act(), NULL);

   
  initialize_minute_line_style(&line_style2);
  initialize_minute_hand(&minute_line,&line_style2,scr);

  initialize_hour_line_style(&hour_hand_style);
  initialize_hour_hand(&hour_line,&hour_hand_style,scr);
}

void update_screen(time_t current_time){

   if(time_changed == true){
    setTime(sys_time_hour,sys_time_minute,sys_time_second,0,0,0);
    time_changed = false;
  }

  struct tm* info = localtime(&current_time);
  int seconds = info->tm_sec;
  int minutes = info->tm_min;
  int hours = info->tm_hour;

  int seconds_angle = map(seconds,0,60,0,360);
  int minutes_angle = map(minutes,0,60,0,360);
  int hour_angle  = map(hours,0,12,0,360);

  int seconds_end_x = 120+(105 * cos((seconds_angle-90)*DEG_TO_RAD));
  int seconds_end_y = 120+(105 * sin((seconds_angle-90)*DEG_TO_RAD));

  int minutes_end_x = 120+(95 * cos((minutes_angle-90)*DEG_TO_RAD));
  int minutes_end_y = 120+(95 * sin((minutes_angle-90)*DEG_TO_RAD));

  int hours_end_x = 120+(65 * cos((hour_angle-90)*DEG_TO_RAD));
  int hours_end_y = 120+(65 * sin((hour_angle-90)*DEG_TO_RAD));

  rotation_points_seconds[0].x = 120;
  rotation_points_seconds[0].y = 120;
  rotation_points_seconds[1].x = seconds_end_x;
  rotation_points_seconds[1].y = seconds_end_y;

  rotation_points_minutes[0].x = 120;
  rotation_points_minutes[0].y = 120;
  rotation_points_minutes[1].x = minutes_end_x;
  rotation_points_minutes[1].y = minutes_end_y;

  rotation_points_hours[0].x = 120;
  rotation_points_hours[0].y = 120;
  rotation_points_hours[1].x = hours_end_x;
  rotation_points_hours[1].y = hours_end_y;

  lv_line_set_points(second_line,rotation_points_seconds,2);
  lv_line_set_points(minute_line,rotation_points_minutes,2);
  lv_line_set_points(hour_line,rotation_points_hours,2);
  lv_obj_invalidate(time_screen);
}


void initialize_background_dial_style(lv_style_t* dial_style){
  lv_style_init(dial_style);
  // lv_style_set_bg_opa(dial_style,100);
  lv_style_set_border_width(dial_style,0);
  lv_style_set_outline_width(dial_style,0);

}

void initialize_background_dial(lv_obj_t* dial,lv_style_t* dial_style,lv_obj_t* act_scr){
  dial = lv_btn_create(act_scr);
  lv_obj_set_size(dial,240,240);
  lv_obj_align(dial,LV_ALIGN_CENTER,0,0);
  lv_obj_add_style(dial,dial_style,0);
  lv_obj_set_style_bg_color(dial,LV_COLOR_BLACK,0);
  // lv_obj_add_event_cb(dial,background_dial_cb,LV_EVENT_CLICKED,NULL);
}

void initialize_watch_dial_style(lv_style_t* watch_dial_style){
  lv_style_init(watch_dial_style);
  lv_style_set_bg_opa(watch_dial_style,100);
  lv_style_set_border_width(watch_dial_style,0);
  lv_style_set_outline_width(watch_dial_style,0);
}

void initialize_buttons(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr){
  button = lv_btn_create(act_scr);
  lv_obj_set_size(button,50,50);
  lv_obj_align(button,LV_ALIGN_LEFT_MID,0,0);
  lv_obj_add_style(button,button_style,0);
  lv_obj_set_style_bg_color(button,LV_COLOR_BLUE,0);
}

void initialize_second_line_style(lv_style_t* line_style){
  lv_style_init(line_style);
  lv_style_set_line_width(line_style,2);
  lv_style_set_line_color(line_style,LV_COLOR_RED);
  lv_style_set_line_rounded(line_style,true);
}

void initialize_second_hand(lv_obj_t** second_line, lv_style_t* line_style, lv_obj_t* act_scr) {
  *second_line = lv_line_create(act_scr);
  // static const lv_point_t minute_hand[] = { {120,120},{120,15}};
  // lv_line_set_points(*second_line, minute_hand, 2);
  lv_obj_add_style(*second_line, line_style,0);
}

void initialize_minute_line_style(lv_style_t* line_style2){
  lv_style_init(line_style2);
  lv_style_set_line_width(line_style2, 2);
  lv_style_set_line_color(line_style2, LV_COLOR_WHITE);
}

void initialize_minute_hand(lv_obj_t** minute_line, lv_style_t* line_style2, lv_obj_t* act_scr) {
  *minute_line = lv_line_create(act_scr);
  // static const lv_point_t minute_hand[] = { {120,120},{120,15}};
  // lv_line_set_points(*minute_line, minute_hand, 2);
  lv_obj_add_style(*minute_line, line_style2,0);
}

void initialize_hour_line_style(lv_style_t* hour_hand_style){
  lv_style_init(hour_hand_style);
  lv_style_set_line_width(hour_hand_style, 3);
  lv_style_set_line_color(hour_hand_style, LV_COLOR_WHITE);
}

void initialize_hour_hand(lv_obj_t** hour_line, lv_style_t* hour_hand_style, lv_obj_t* act_scr) {
  *hour_line = lv_line_create(act_scr);
  // static const lv_point_t minute_hand[] = { {120,120},{120,15}};
  // lv_line_set_points(*hour_line, minute_hand, 2);
  lv_obj_add_style(*hour_line, hour_hand_style,0);
}

static void background_dial_cb(lv_event_t event)
{
  Serial.println("Dial Pressed,Creating A New Screen");
}

void initialize_linemeter_style(lv_style_t* linemeter_style){
  lv_style_init(linemeter_style);
  lv_style_set_line_width(linemeter_style,4);
  lv_style_set_bg_opa(linemeter_style,0);
  lv_style_set_line_color(linemeter_style,LV_COLOR_WHITE);
  // lv_style_set_line_rounded(linemeter_style,LV_STATE_DEFAULT,true);
}

void initialize_linemeter(lv_obj_t* linemeter,lv_style_t* linemeter_style,lv_obj_t* act_scr){
  linemeter = lv_meter_create(act_scr);
  lv_obj_set_size(linemeter, 240, 240);
  lv_obj_align(linemeter, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_color(linemeter,LV_COLOR_BLACK,0);
  lv_obj_set_style_text_color(linemeter,LV_COLOR_BLACK,0);
  lv_obj_set_style_border_width(linemeter,0,0);
  lv_obj_add_event_cb(linemeter,linemeter_cb,LV_EVENT_ALL,NULL);
  lv_obj_add_event_cb(linemeter,flashlight_cb,LV_EVENT_CLICKED,NULL);
  // lv_obj_add_event_cb(linemeter,long_press_cb,LV_EVENT_ALL,NULL);

  lv_meter_scale_t* hour_scale = lv_meter_add_scale(linemeter);
  lv_meter_set_scale_ticks(linemeter,hour_scale,12,0,0,LV_COLOR_WHITE); // Set Scale for hour ticks
  lv_meter_set_scale_major_ticks(linemeter,hour_scale,1,2,20,LV_COLOR_WHITE,10); // Set Scale for hour ticks
  lv_meter_set_scale_range(linemeter,hour_scale,0,12,330,300); // Set Range for hour ticks

  // lv_meter_scale_t* minute_scale = lv_meter_add_scale(linemeter);
  // lv_meter_set_scale_ticks(linemeter,minute_scale,61,1,10,LV_COLOR_GREY); // Set Scale for minute ticks
  // lv_meter_set_scale_range(linemeter,minute_scale,0,60,360,270); // Set Range for minute ticks

  // lv_meter_indicator_t* battery_indicator = lv_meter_add_arc(linemeter,hour_scale,1,LV_COLOR_RED,0);

  // lv_meter_set_indicator_start_value(linemeter,battery_indicator,0);
  // lv_meter_set_indicator_end_value(linemeter,battery_indicator,50);

  lv_obj_set_parent(linemeter,act_scr);
}

void linemeter_cb(lv_event_t* event){
  if(lv_event_get_code(event) == LV_EVENT_CLICKED){
    if(String(touch.gesture().c_str()) == "LONG PRESS"){
      main_menu_screen = lv_obj_create(NULL);
      init_main_menu_screen(main_menu_screen);
      delay(100);
      lv_scr_load(main_menu_screen);
    }else{
     
    }
  }
}

void flashlight_cb(lv_event_t* event){
  if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT){
    flashlight = lv_obj_create(NULL);
    lv_obj_add_event_cb(flashlight,screen_cb,LV_EVENT_GESTURE,NULL);
    lv_obj_add_event_cb(flashlight,flashlight_toggle_cb,LV_EVENT_CLICKED,NULL);
    lv_scr_load(flashlight);
  }
}

void screen_cb(lv_event_t* event){
  if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
    lv_scr_load(time_screen);
    lv_obj_del(flashlight);
  }
}

void long_press_cb(lv_event_t* event){

}

void flashlight_toggle_cb(lv_event_t* event){
  if(lv_event_get_code(event) == LV_EVENT_CLICKED){
    if(flashlight_mode == false){
      lv_obj_set_style_bg_color(flashlight,LV_COLOR_WHITE,0);
      flashlight_mode = true;
    }
    else{
      lv_obj_set_style_bg_color(flashlight,LV_COLOR_BLACK,0);
      flashlight_mode = false;
    }
  }
}

#endif

//todo: Create Digital Watch Face

#ifdef DIGITAL_WATCH_FACE

static lv_obj_t* hour_button;
static lv_obj_t* minute_button;
static lv_obj_t* battery_arc;

lv_obj_t* hour_label;
lv_obj_t* minute_label;

static lv_style_t hour_button_style;
static lv_style_t minute_button_style;
static lv_obj_t* battery_arc_style;

void initialize_background_dial_style(lv_style_t* dial_style);
void initialize_background_dial(lv_obj_t* dial,lv_style_t* dial_style,lv_obj_t* act_scr);
void initialize_watch_dial_style(lv_style_t* watch_dial_style);
void initialize_buttons(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr);
void initialize_second_line_style(lv_style_t* line_style);
void initialize_second_hand(lv_obj_t** second_line, lv_style_t* line_style, lv_obj_t* act_scr);
void initialize_minute_line_style(lv_style_t* line_style2);
void initialize_minute_hand(lv_obj_t** minute_line, lv_style_t* line_style2, lv_obj_t* act_scr);
void initialize_hour_line_style(lv_style_t* hour_hand_style);
void initialize_hour_hand(lv_obj_t** hour_line, lv_style_t* hour_hand_style, lv_obj_t* act_scr);
void initialize_linemeter_style(lv_style_t* linemeter_style);
void initialize_linemeter(lv_obj_t* linemeter,lv_style_t* linemeter_style,lv_obj_t* act_scr);
void initialize_hour_button(lv_obj_t* scr,lv_obj_t* hour_button,lv_style_t* hour_button_style);
void initialize_hour_button_style(lv_style_t* hour_button_style);
void initialize_minute_button(lv_obj_t* scr,lv_obj_t* minute_button,lv_style_t* minute_button_style);
void initialize_minute_button_style(lv_style_t* minute_button_style);
void linemeter_cb(lv_event_t* event);
void screen_cb(lv_event_t* event);
void long_press_cb(lv_event_t* event);
void flashlight_cb(lv_event_t* event);
void flashlight_toggle_cb(lv_event_t* event);

void init_screen(lv_obj_t* scr){
  time_screen = scr;
  initialize_background_dial_style(&dial_style);
  initialize_background_dial(dial,&dial_style,scr);

  // initialize_hour_button_style(&hour_button_style);
  initialize_hour_button(scr,hour_button,&hour_button_style);
  initialize_minute_button(scr,minute_button,&minute_button_style);

}

void update_screen(time_t current_time){

   if(time_changed == true){
    setTime(sys_time_hour,sys_time_minute,sys_time_second,0,0,0);
    time_changed = false;
  }

  struct tm* info = localtime(&current_time);
  int seconds = info->tm_sec;
  int minutes = info->tm_min;
  int hours = info->tm_hour;

  Serial.println("Hours: "+String(hours));
  Serial.println("Minutes: "+String(minutes));
  Serial.println("Seconds: "+String(seconds));

  

  lv_obj_invalidate(time_screen);
}


void initialize_background_dial_style(lv_style_t* dial_style){
  lv_style_init(dial_style);
  // lv_style_set_bg_opa(dial_style,100);
  lv_style_set_border_width(dial_style,0);
  lv_style_set_outline_width(dial_style,0);

}

void initialize_background_dial(lv_obj_t* dial,lv_style_t* dial_style,lv_obj_t* act_scr){
  dial = lv_btn_create(act_scr);
  lv_obj_set_size(dial,240,240);
  lv_obj_align(dial,LV_ALIGN_CENTER,0,0);
  lv_obj_add_style(dial,dial_style,0);
  lv_obj_set_style_bg_color(dial,LV_COLOR_BLACK,0);
  // lv_obj_add_event_cb(dial,background_dial_cb,LV_EVENT_CLICKED,NULL);
}

void initialize_watch_dial_style(lv_style_t* watch_dial_style){
  lv_style_init(watch_dial_style);
  lv_style_set_bg_opa(watch_dial_style,100);
  lv_style_set_border_width(watch_dial_style,0);
  lv_style_set_outline_width(watch_dial_style,0);
}

void initialize_buttons(lv_obj_t* button,lv_style_t* button_style,lv_obj_t* act_scr){
  button = lv_btn_create(act_scr);
  lv_obj_set_size(button,50,50);
  lv_obj_align(button,LV_ALIGN_LEFT_MID,0,0);
  lv_obj_add_style(button,button_style,0);
  lv_obj_set_style_bg_color(button,LV_COLOR_BLUE,0);
}

void initialize_second_line_style(lv_style_t* line_style){
  lv_style_init(line_style);
  lv_style_set_line_width(line_style,2);
  lv_style_set_line_color(line_style,LV_COLOR_RED);
  lv_style_set_line_rounded(line_style,true);
}

void initialize_second_hand(lv_obj_t** second_line, lv_style_t* line_style, lv_obj_t* act_scr) {
  *second_line = lv_line_create(act_scr);
  // static const lv_point_t minute_hand[] = { {120,120},{120,15}};
  // lv_line_set_points(*second_line, minute_hand, 2);
  lv_obj_add_style(*second_line, line_style,0);
}

void initialize_minute_line_style(lv_style_t* line_style2){
  lv_style_init(line_style2);
  lv_style_set_line_width(line_style2, 2);
  lv_style_set_line_color(line_style2, LV_COLOR_WHITE);
}

void initialize_minute_hand(lv_obj_t** minute_line, lv_style_t* line_style2, lv_obj_t* act_scr) {
  *minute_line = lv_line_create(act_scr);
  // static const lv_point_t minute_hand[] = { {120,120},{120,15}};
  // lv_line_set_points(*minute_line, minute_hand, 2);
  lv_obj_add_style(*minute_line, line_style2,0);
}

void initialize_hour_line_style(lv_style_t* hour_hand_style){
  lv_style_init(hour_hand_style);
  lv_style_set_line_width(hour_hand_style, 3);
  lv_style_set_line_color(hour_hand_style, LV_COLOR_WHITE);
}

void initialize_hour_hand(lv_obj_t** hour_line, lv_style_t* hour_hand_style, lv_obj_t* act_scr) {
  *hour_line = lv_line_create(act_scr);
  // static const lv_point_t minute_hand[] = { {120,120},{120,15}};
  // lv_line_set_points(*hour_line, minute_hand, 2);
  lv_obj_add_style(*hour_line, hour_hand_style,0);
}

static void background_dial_cb(lv_event_t event)
{
  Serial.println("Dial Pressed,Creating A New Screen");
}

void initialize_linemeter_style(lv_style_t* linemeter_style){
  lv_style_init(linemeter_style);
  lv_style_set_line_width(linemeter_style,4);
  lv_style_set_bg_opa(linemeter_style,0);
  lv_style_set_line_color(linemeter_style,LV_COLOR_WHITE);
  // lv_style_set_line_rounded(linemeter_style,LV_STATE_DEFAULT,true);
}

void initialize_linemeter(lv_obj_t* linemeter,lv_style_t* linemeter_style,lv_obj_t* act_scr){
  linemeter = lv_meter_create(act_scr);
  lv_obj_set_size(linemeter, 240, 240);
  lv_obj_align(linemeter, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_color(linemeter,LV_COLOR_BLACK,0);
  lv_obj_set_style_text_color(linemeter,LV_COLOR_BLACK,0);
  lv_obj_set_style_border_width(linemeter,0,0);
  lv_obj_add_event_cb(linemeter,linemeter_cb,LV_EVENT_ALL,NULL);
  lv_obj_add_event_cb(linemeter,flashlight_cb,LV_EVENT_CLICKED,NULL);
  // lv_obj_add_event_cb(linemeter,long_press_cb,LV_EVENT_ALL,NULL);

  lv_meter_scale_t* hour_scale = lv_meter_add_scale(linemeter);
  lv_meter_set_scale_ticks(linemeter,hour_scale,12,0,0,LV_COLOR_WHITE); // Set Scale for hour ticks
  lv_meter_set_scale_major_ticks(linemeter,hour_scale,1,2,20,LV_COLOR_WHITE,10); // Set Scale for hour ticks
  lv_meter_set_scale_range(linemeter,hour_scale,0,12,330,300); // Set Range for hour ticks

  // lv_meter_scale_t* minute_scale = lv_meter_add_scale(linemeter);
  // lv_meter_set_scale_ticks(linemeter,minute_scale,61,1,10,LV_COLOR_GREY); // Set Scale for minute ticks
  // lv_meter_set_scale_range(linemeter,minute_scale,0,60,360,270); // Set Range for minute ticks

  // lv_meter_indicator_t* battery_indicator = lv_meter_add_arc(linemeter,hour_scale,1,LV_COLOR_RED,0);

  // lv_meter_set_indicator_start_value(linemeter,battery_indicator,0);
  // lv_meter_set_indicator_end_value(linemeter,battery_indicator,50);

  lv_obj_set_parent(linemeter,act_scr);
}

void initialize_hour_button_style(lv_style_t* hour_button_style){
  lv_style_init(hour_button_style);
  lv_style_set_bg_opa(hour_button_style,100);
  lv_style_set_border_width(hour_button_style,0);
  lv_style_set_outline_width(hour_button_style,0);
}

void initialize_hour_button(lv_obj_t* scr,lv_obj_t* hour_button,lv_style_t* hour_button_style){
  hour_button = lv_btn_create(scr);
  lv_obj_set_size(hour_button,20,20);
  lv_obj_align(hour_button,LV_ALIGN_CENTER,0,0);
  // lv_obj_add_style(hour_button,hour_button_style,0);
  lv_obj_set_style_bg_color(hour_button,LV_COLOR_BLUE,0);
  lv_obj_set_style_bg_opa(hour_button,0,0);

  hour_label = lv_label_create(hour_button);
  lv_label_set_text(hour_label,"12");
  lv_obj_align(hour_label,LV_ALIGN_CENTER,-30,0);
  lv_obj_set_size(hour_label,80,80);
  lv_obj_set_style_text_color(hour_label,LV_COLOR_WHITE,0);
}

void initialize_minute_button_style(lv_style_t* minute_button_style){
  lv_style_init(minute_button_style);
  lv_style_set_bg_opa(minute_button_style,100);
  lv_style_set_border_width(minute_button_style,0);
  lv_style_set_outline_width(minute_button_style,0);
}

void initialize_minute_button(lv_obj_t* scr,lv_obj_t* minute_button,lv_style_t* minute_button_style){
  minute_button = lv_btn_create(scr);
  lv_obj_set_size(minute_button,20,20);
  lv_obj_align(minute_button,LV_ALIGN_CENTER,0,0);
  // lv_obj_add_style(minute_button,minute_button_style,0);
  lv_obj_set_style_bg_color(minute_button,LV_COLOR_BLUE,0);
  lv_obj_set_style_bg_opa(minute_button,0,0);

  minute_label = lv_label_create(minute_button);
  lv_label_set_text(minute_label,"10");
  lv_obj_align(minute_label,LV_ALIGN_CENTER,30,0);
  lv_obj_set_size(minute_label,80,80);
  lv_obj_set_style_text_color(minute_label,LV_COLOR_WHITE,0);
}

void linemeter_cb(lv_event_t* event){
  if(lv_event_get_code(event) == LV_EVENT_CLICKED){
    if(String(touch.gesture().c_str()) == "LONG PRESS"){
      main_menu_screen = lv_obj_create(NULL);
      init_main_menu_screen(main_menu_screen);
      delay(100);
      lv_scr_load(main_menu_screen);
    }else{
     
    }
  }
}

void flashlight_cb(lv_event_t* event){
  if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT){
    flashlight = lv_obj_create(NULL);
    lv_obj_add_event_cb(flashlight,screen_cb,LV_EVENT_GESTURE,NULL);
    lv_obj_add_event_cb(flashlight,flashlight_toggle_cb,LV_EVENT_CLICKED,NULL);
    lv_scr_load(flashlight);
  }
}

void screen_cb(lv_event_t* event){
  if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
    lv_scr_load(time_screen);
    lv_obj_del(flashlight);
  }
}

void long_press_cb(lv_event_t* event){

}

void flashlight_toggle_cb(lv_event_t* event){
  if(lv_event_get_code(event) == LV_EVENT_CLICKED){
    if(flashlight_mode == false){
      lv_obj_set_style_bg_color(flashlight,LV_COLOR_WHITE,0);
      flashlight_mode = true;
    }
    else{
      lv_obj_set_style_bg_color(flashlight,LV_COLOR_BLACK,0);
      flashlight_mode = false;
    }
  }
}


#endif


