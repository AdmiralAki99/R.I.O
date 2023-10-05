#pragma once

#include <FS.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <CST816S.h>
#include <driver/gpio.h>
#include <Ticker.h>
#include <WiFi.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ble_config.h"

#define LV_COLOR_BLACK  LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_GREY LV_COLOR_MAKE(0x80, 0x80, 0x80)

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TIME_CHARACTERISTIC_UUID "b9dc3d38-4b22-11ee-be56-0242ac120002"
#define TODO_CHARACTERISTIC_UUID "28bd3c28-635d-11ee-8c99-0242ac120002"

/**
 * Pin Definitions for GC9A01 TFT SCREEN
*/

#define MISO_PIN 37
#define MOSI_PIN 35
#define SCLK_PIN 34
#define LCD_CS_PIN 7
#define LCD_DC_PIN 3
#define LCD_RST_PIN 4
#define LCD_BL_PIN 6
#define TP_SDA_PIN 8
#define TP_SCL_PIN 9
#define TP_INT_PIN 21
#define TP_RST_PIN 5

/**
 * Pin Definitions for the GPS Module
*/

#define GPS_RX_PIN 2
#define GPS_TX_PIN 36

/**
 * Declaring the Global Variables that will be used commonly
*/

lv_obj_t* main_screen;

TinyGPSPlus gps = TinyGPSPlus();
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

#define ANALOG_WATCH_FACE