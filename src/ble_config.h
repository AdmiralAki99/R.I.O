/**
 * @file ble_config.h
 * @author Akhilesh Warty
 * @brief BLE Server Module for Smartwatch
 * @version 0.2
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

/**
 * Bluetooth Variables
*/

static bool is_ble_client_connected = false;
static bool has_music_metadata_changed = false;


static BLEServer* pServer;
static BLEService* pService;
static BLECharacteristic* mCharacteristic;
static BLECharacteristic* tCharacteristic;
static BLEAdvertising* pAdvertising;

std::string ble_music_value = "";
std::string ble_music_artist = "";
static std::string ble_time_value;
bool read_song = false;

/**
 * BLE Callbacks for Each Characteristic
*/

//Music Callback handles music metadata

class MusicBLECallback : public BLECharacteristicCallbacks{
    void onWrite(BLECharacteristic* pCharacteristic){
      pAdvertising->stop();
      read_song = false;
      std::string song = "";
      std::string artist = "";
      std::string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++){
          if(value[i] == ';'){
            read_song = true;
            continue;
          }
          if(read_song == false){
            song = song + value[i];
          }else{
            artist = artist + value[i];
          }
        }
      }
      Serial.println(song.c_str());
      Serial.println(artist.c_str());

      ble_music_value = song.c_str();
      ble_music_artist = artist.c_str();
      has_music_metadata_changed = true;
      pAdvertising->start();
    }

    void onRead(BLECharacteristic* pCharacteristic){
      
    }
};

// Time Callback handles time metadata

class TimeBLECallback : public BLECharacteristicCallbacks{
  void onWrite(BLECharacteristic* pCharacteristic){
     pAdvertising->stop();
      std::string valor = "";
      std::string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++){
          valor = valor + value[i];
        }
      }
      ble_time_value = valor;
      // Serial.print(ble_music_value.c_str());
      pAdvertising->start();
  }

  void onRead(BLECharacteristic* pCharacteristic){
    
  }
};

//General Server Callback

class BLEServerCallback : public BLEServerCallbacks{
  void onConnect(BLEServer* pServer){
      is_ble_client_connected = true;
  }

  void onDisconnect(BLEServer* pServer){
      is_ble_client_connected = false;
  }
};