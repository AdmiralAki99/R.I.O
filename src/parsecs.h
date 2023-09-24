#pragma once
#include <config.h>

/**
 * Global Declarations
*/
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define TICK_INTERVAL_ANGLE 6 // Angle of each minute interval line
#define DIAL_RADIUS 120 // Radius of the circle
#define DIAL_CENTER_RADIUS_X 120 // X coordinate of the center of the screen
#define DIAL_CENTER_RADIUS_Y 120 // Y coordinate of the center of the screen
#define SLEEP_TIMEOUT 60 // Seconds before the screen goes to sleep
#define DEEP_SLEEP_TIMEOUT 300 // Seconds before the system goes to deep sleep

/**
 * TFT eSPI Variables
*/

TFT_eSPI tft = TFT_eSPI(SCREEN_HEIGHT,SCREEN_WIDTH);

/**
 * CST816S Touch Screen Variables
*/

CST816S touch(TP_SDA_PIN, TP_SCL_PIN, TP_RST_PIN, TP_INT_PIN);  // sda, scl, rst, irq

/**
 * LVGL Variables
*/
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[SCREEN_WIDTH*10];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

//todo: Implement GPS Variables and Functions

/**
 * GPS Variables
*/

uint8_t gps_satellites;
// Position Variables
double gps_pos_latitude;
double gps_pos_longitude;
double gps_pos_altitude;
// Time Variables
uint8_t gps_time_hour;
uint8_t gps_time_minute;
uint8_t gps_time_second;
// Date Variables
uint16_t gps_date_year;
uint8_t gps_date_month;
uint8_t gps_date_day;

/**
 * WiFi Variables
*/

/**
 * UDP Server Variables
*/





/**
 * Global Variables
*/

static TaskHandle_t bluetooth_task_handle;

const unsigned long GPS_INTERVAL = 600000; // 10 Minutes

unsigned int long gps_last_time = 0;

static time_t main_time;
struct tm* info;
int timer;
static bool screen_sleep = false;
static bool modem_sleep = false;
static Ticker ticker;

static uint8_t sys_time_hour = 0;
static uint8_t sys_time_minute = 0;
static uint8_t sys_time_second = 0;

static TaskHandle_t* gps_task_handle;


static bool time_changed = false;

/**
 * Lazarus Helper Function Definitions
*/
void init_lvgl();
void init_touch_screen_drivers();
void init_watch();
void init_tft_espi();
void init_system_variables();
void initTimer();
void gps_task(void *parameter);
/**
 * LVGL Helper Functions Definitions
*/
void flush_display(lv_disp_drv_t *disp,const lv_area_t *area,lv_color_t* colour);
void touchpad_event(lv_indev_drv_t* indev_driver,lv_indev_data_t* touch_data);

void init_watch(){
    setTime(0); // Initialize System Time to +0:00:00
    init_tft_espi();
    init_touch_screen_drivers();
    init_lvgl();
    init_system_variables();
}

void init_tft_espi(){ 
 /**
  * Initialize the TFT_eSPI Drivers
  * */

  tft.init();
  tft.setRotation(0);
}

void init_touch_screen_drivers(){
    touch.begin();
}

void init_lvgl(){
    //Init LVGL
    lv_init();

    //Init Display Driver
    lv_disp_draw_buf_init(&draw_buf, buf_1, NULL, SCREEN_WIDTH * 10);//Initialize Display Buffer
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = flush_display;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    //Init Touch Screen Handler
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_event;
    lv_indev_t* touchpad = lv_indev_drv_register(&indev_drv);

}

void init_system_variables(){
    timer=0;
}
/**
 * LVGL Helper Functions
*/

//Render Screen
void flush_display(lv_disp_drv_t *disp,const lv_area_t *area,lv_color_t* colour){
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1,area->y1,w,h);
  tft.pushColors((uint16_t*)&colour->full,(w*h),true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

//Touchpad Event Handler

void touchpad_event(lv_indev_drv_t* indev_driver,lv_indev_data_t* touch_data){
   uint16_t x,y;
   char* gesture;
   static lv_area_t area;
   static lv_obj_t* child_obj;
   static lv_coord_t ui_x,ui_y,ui_w,ui_h;
    if (touch.available()) {
      if(screen_sleep == false){
        touch_data->state = LV_INDEV_STATE_PR;
        x = touch.data.x;
        y = touch.data.y;
        touch_data->point.x = x;
        touch_data->point.y = y;
      }
     

      // Serial.println("X: "+String(x));
      // Serial.println("Y: "+String(y));
      // for(int i=0;i<lv_obj_get_child_cnt(lv_scr_act());i++){
      //   child_obj = lv_obj_get_child(lv_scr_act(),i);
      //   lv_obj_get_click_area(child_obj,&area);
      //   ui_x = lv_obj_get_x(child_obj);
      //   ui_y = lv_obj_get_y(child_obj);
      //   ui_w = lv_obj_get_x2(child_obj);
      //   ui_h = lv_obj_get_y2(child_obj);
        
      //   if((x >= area.x1 && x<= area.x2) && (y >= area.y1 && y<= area.y2) && (lv_obj_get_state(child_obj) != LV_STATE_DISABLED)){
      //     Serial.println("Child Found");
      //     if(String(touch.gesture().c_str()) == "SINGLE CLICK"){
      //       lv_event_send(child_obj,LV_EVENT_CLICKED,NULL);
      //       lv_obj_set_user_data(child_obj,main_screen);
      //       break;
      //     }else if(String(touch.gesture().c_str()) == "LONG PRESS"){
      //       lv_event_send(child_obj,LV_EVENT_LONG_PRESSED,NULL);
      //       break;
      //     }else if(String(touch.gesture().c_str()) == "SWIPE LEFT"){
      //       // lv_event_send(child_obj,LV_EVENT_GESTURE,NULL);
      //       // Serial.println(lv_indev_get_scroll_dir(lv_indev_get_act()));
      //       break;
      //     }else if(String(touch.gesture().c_str()) == "SWIPE UP"){
      //       Serial.println("Scroll Initiated");
      //       lv_event_send(child_obj,LV_EVENT_SCROLL,NULL);
      //       break;
      //     }
      //   }else{
      //     // Serial.println("Not Clicked");
      //   }
      // }

    
  
    }else{
      touch_data->state = LV_INDEV_STATE_REL;
    }

}

/**
 * System Helper Functions
*/

void setModemSleep(){
  WiFi.setSleep(true);
  if(!setCpuFrequencyMhz(80)){
    Serial.println("Failed to set CPU Frequency");
  }else{
    Serial.println("CPU Frequency set to 80MHz");
    modem_sleep=true;
  }
}

void wakeModemSleep(){
  setCpuFrequencyMhz(240);
  WiFi.setSleep(false);
  // Serial.println("Modem Wake");
  modem_sleep=false;
}

void incrementCounter() {
  timer++;
  // Serial.println(timer);
  if(touch.available()){
    timer=0;
    if(modem_sleep == true){
      wakeModemSleep();
    }
    digitalWrite(6,HIGH);
    screen_sleep=false;
  }
  if (timer >= SLEEP_TIMEOUT && screen_sleep == false) {
    digitalWrite(6,LOW);
    screen_sleep=true;
  }
  if(timer >= DEEP_SLEEP_TIMEOUT && modem_sleep == false) {
      Serial.println("Modem Sleep");
      setModemSleep();
  }
}

void initTimer(){
  ticker.attach(1.0,incrementCounter);
}

void initGPS(){
  
}
/**
 * GPS Functions
*/

void get_location(){
    
}

void get_time(){
  
}

uint8_t get_hour(){
  return gps.time.hour();
}

uint8_t get_minute(){
  return gps.time.minute();
}

uint8_t get_second(){
  return gps.time.second();
}

void gps_task(void *parameter){
  uint8_t temp_hour,temp_minute,temp_second;

  while(ss.available() > 0){
    gps.encode(ss.read());
    temp_hour = get_hour();
    temp_minute = get_minute();
    temp_second = get_second();
  
    if(temp_hour != 0){
      gps_time_hour = temp_hour; // Time is Correct
      if(temp_minute != 0){
        gps_time_minute = temp_minute;
        if(temp_second != 0){
          gps_time_second = temp_second;
        }
      }
    }
  }
  
}

void advertise_ble_server(){
  pAdvertising->start();
}

void init_ble_server(){
  Serial.begin(115200);
  Serial.println("Starting BLE Server");

  BLEDevice::init("Lazarus");

  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  mCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  mCharacteristic->setValue("Hello World");
  mCharacteristic->setCallbacks(new MusicBLECallback());

  tCharacteristic = pService->createCharacteristic(TIME_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  tCharacteristic->setValue("Time Value");
  tCharacteristic->setCallbacks(new TimeBLECallback());
  pService->start();

  pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();
}

void readMessage(){

}

TaskFunction_t sendMessage(uint8_t *message,int length){
  uint8_t transmit = 0;

  // for(uint8_t i =0 ; i< length ; i++){
  //   pCharacteristic->setValue(&message[i],1);
  //   pCharacteristic->notify();
  //   delay(100);
  // }

  for(;;){
    Serial.println("Sending Message");
  }
}

//todo: Function to Parse Music Information
