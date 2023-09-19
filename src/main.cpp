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
time_t current_time;

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

  lv_scr_load(main_screen);
  initTimer();
}

void loop() {
  // put your main code here, to run repeatedly

  // lv_scr_load(lv_scr_act());

  // lv_task_handler();
  lv_timer_handler();

  current_time = now();

  update_screen(current_time);

  lv_tick_inc(1);
  
}