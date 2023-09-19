#include "settings_screen.h"
#include "parsecs.h"
#include "time_screen.h"
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <CST816S.h>
#include <driver/gpio.h>
#include <Ticker.h>
#include <WiFi.h>
#include <esp_bt.h>
#include "driver/adc.h"


 static uint8_t* message = (uint8_t*)malloc(sizeof(7));

// void flush_display(lv_disp_drv_t *disp,const lv_area_t *area,lv_color_t* colour){
//   uint32_t w = (area->x2 - area->x1 + 1);
//   uint32_t h = (area->y2 - area->y1 + 1);

//   tft.startWrite();
//   tft.setAddrWindow(area->x1,area->y1,w,h);
//   tft.pushColors((uint16_t*)&colour->full,(w*h),true);
//   tft.endWrite();

//   lv_disp_flush_ready(disp);
// }

// void touchpad_event(lv_indev_drv_t* indev_driver,lv_indev_data_t* touch_data){
//    uint16_t x,y;
//    char* gesture;
//    static lv_area_t area;
//    static lv_obj_t* child_obj;
//    static lv_coord_t ui_x,ui_y,ui_w,ui_h;
//     if (touch.available()) {
//       if(screen_sleep == false){
//         touch_data->state = LV_INDEV_STATE_PR;
//         x = touch.data.x;
//         y = touch.data.y;
//         touch_data->point.x = x;
//         touch_data->point.y = y;
//       }
     

//       // Serial.println("X: "+String(x));
//       // Serial.println("Y: "+String(y));
//       // for(int i=0;i<lv_obj_get_child_cnt(lv_scr_act());i++){
//       //   child_obj = lv_obj_get_child(lv_scr_act(),i);
//       //   lv_obj_get_click_area(child_obj,&area);
//       //   ui_x = lv_obj_get_x(child_obj);
//       //   ui_y = lv_obj_get_y(child_obj);
//       //   ui_w = lv_obj_get_x2(child_obj);
//       //   ui_h = lv_obj_get_y2(child_obj);
        
//       //   if((x >= area.x1 && x<= area.x2) && (y >= area.y1 && y<= area.y2) && (lv_obj_get_state(child_obj) != LV_STATE_DISABLED)){
//       //     Serial.println("Child Found");
//       //     if(String(touch.gesture().c_str()) == "SINGLE CLICK"){
//       //       lv_event_send(child_obj,LV_EVENT_CLICKED,NULL);
//       //       lv_obj_set_user_data(child_obj,main_screen);
//       //       break;
//       //     }else if(String(touch.gesture().c_str()) == "LONG PRESS"){
//       //       lv_event_send(child_obj,LV_EVENT_LONG_PRESSED,NULL);
//       //       break;
//       //     }else if(String(touch.gesture().c_str()) == "SWIPE LEFT"){
//       //       // lv_event_send(child_obj,LV_EVENT_GESTURE,NULL);
//       //       // Serial.println(lv_indev_get_scroll_dir(lv_indev_get_act()));
//       //       break;
//       //     }else if(String(touch.gesture().c_str()) == "SWIPE UP"){
//       //       Serial.println("Scroll Initiated");
//       //       lv_event_send(child_obj,LV_EVENT_SCROLL,NULL);
//       //       break;
//       //     }
//       //   }else{
//       //     // Serial.println("Not Clicked");
//       //   }
//       // }

    
  
//     }else{
//       touch_data->state = LV_INDEV_STATE_REL;
//     }

// }

// void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
//   switch(id){
//      case ESP_AVRC_MD_ATTR_TITLE:
//         Serial.print("Title: ");
//         // title = (char*)text;
//         break;
//      case ESP_AVRC_MD_ATTR_ARTIST:
//         Serial.print("Artist: ");
//         // artist = (char*)text;
//         break;
//   }

// }
time_t current_time;

void test_cb(lv_obj_t * obj, lv_event_t event)
{
    Serial.println("Dial Pressed");
    if(lv_event_get_code(&event) == LV_EVENT_CLICKED) {
        // Button was clicked
        Serial.println("Dial Pressed,Creating A New Screen");
    }
}

lv_obj_t* btn;

void setup() {
  // put your setup code here, to run once:

  pinMode(LCD_BL_PIN,OUTPUT);

  digitalWrite(LCD_BL_PIN,HIGH);

  // touch.begin();

  // /**
  //  * Initialize the TFT_eSPI Drivers
  //  * */
  // tft.init();
  // tft.setRotation(0);

  init_watch();

  init_ble_server();

  message = (uint8_t*)"Message";

  // xTaskCreatePinnedToCore(sendMessage(&message[0],7),"Send Message",10000,NULL,1,&bluetooth_task_handle,1);

  // tft.fillScreen(TFT_WHITE);
  // Serial.begin(115200);


  /**
   * Initialize LVGL Library and Functions
   **/

  // a2dp_sink.start("ESP32 A2DP Sink");
  // a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  // lv_init();

  // // lv_disp_buf_init(&draw_buf,buf_1,NULL,240*10); 
  // lv_disp_draw_buf_init(&draw_buf, buf_1, NULL, 240 * 10);//Initialize Display Buffer

  // static lv_disp_drv_t disp_drv;
  // lv_disp_drv_init(&disp_drv);

  // disp_drv.hor_res = screenWidth;
  // disp_drv.ver_res = screenHeight;
  // disp_drv.flush_cb = flush_display;
  // disp_drv.draw_buf = &draw_buf;
  // lv_disp_drv_register(&disp_drv);

  // //todo: Add Touch Functions and Displays to LVGL

  // static lv_indev_drv_t indev_drv;
  // lv_indev_drv_init(&indev_drv);
  // indev_drv.type = LV_INDEV_TYPE_POINTER;
  // indev_drv.read_cb = touchpad_event;
  // lv_indev_t* touchpad = lv_indev_drv_register(&indev_drv);

  // // display_watch_dial(lv_scr_act());

  // // Create a style for the waveform
  // /* Create a container object */
  //   // lv_obj_t * label;


  /**
   * Creating the Background Dial Style and Button
   * */

  main_screen = lv_obj_create(NULL);
  /**
   * Creating a watch second and minute hands and hour hands
   * 
   **/
  
  current_time = time(nullptr);

  init_screen(main_screen);

  // lv_style_t transp_button;

  // lv_style_init(&transp_button);
  // lv_style_set_bg_opa(&transp_button,LV_STATE_DEFAULT,0);
  // lv_style_set_border_width(&transp_button,LV_STATE_DEFAULT,0);
  // lv_style_set_outline_width(&transp_button,LV_STATE_DEFAULT,0);

  // btn = lv_btn_create(time_screen,NULL);
  // lv_obj_align(btn,NULL,LV_ALIGN_IN_TOP_MID,0,0);
  // lv_obj_add_style(btn,LV_BTN_PART_MAIN,&transp_button);
  // lv_obj_set_style_local_bg_color(btn,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
  // lv_obj_set_style_local_value_str(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,"Button");
  // lv_obj_set_style_local_value_color(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_WHITE);
  // lv_obj_set_event_cb(btn,test_cb);

  // lv_obj_t *back_icon = lv_img_create(btn, NULL);
  // lv_img_set_src(back_icon, LV_SYMBOL_LEFT);
  // lv_obj_set_size(back_icon, 20, 20);
  // lv_obj_set_pos(back_icon, 10, 5);
  // lv_obj_set_style_local_text_color(back_icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE); // Set the icon color to white

  // init_music_screen(main_screen);

  // init_settings_screen(main_screen);

  lv_scr_load(main_screen);
  
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = 1ULL <<GPIO_NUM_0;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);
  //todo: Create circular line meter for battery level

  Serial.print(touch.data.version);
  Serial.print("\t");
  Serial.print(touch.data.versionInfo[0]);
  Serial.print("-");
  Serial.print(touch.data.versionInfo[1]);
  Serial.print("-");    
  Serial.println(touch.data.versionInfo[2]);

  // lv_obj_t *img_obj = lv_img_create(lv_scr_act());

  // lv_img_set_src(img_obj,"A:src/space.jpg");

  // lv_scr_load(main_screen);
  initTimer();

  // Serial.begin(115200);
  // Serial.println(xPortGetCoreID());
}

void loop() {
  // put your main code here, to run repeatedly

  // sendMessage(&message[0],7);

  // Serial.println(xPortGetCoreID());
  

  lv_scr_load(lv_scr_act());

  lv_task_handler();
  lv_timer_handler();

  current_time = now();

  update_screen(current_time);

  // Serial.println(String(touch.gesture().c_str()));

  // advertise_ble_server();


  // if(gpio_get_level(GPIO_NUM_0) == 0)
  // {
  //   Serial.println("Button Pressed");
  // }

  // if(play_button_task != NULL)
  // {
  //   // Serial.println( "Play Button Task is not NULL");
  // }else{
  //   Serial.println("Play Button Task is NULL");
  // }

  lv_tick_inc(1);
  // update_music_screen();

  // vTaskDelay(pdMS_TO_TICKS(16));

  // delay(1000);
  
}