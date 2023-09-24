#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// class BLECallback : public BLECharacteristicCallbacks{
//     void onWrite(BLECharacteristic* pCharacteristic){
//         Serial.println("BLE Write Request");
//     }

//     void onRead(BLECharacteristic* pCharacteristic){
//         Serial.println("BLE Read Request");
//     }
// };

// std::string parseMusicData(char* musicMessage){
//   char *tokens = strtok(musicMessage,"$");

//   while(tokens != NULL){
//     Serial.println(tokens);
//     tokens = strtok(NULL,"$");
//   }
// }

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
static std::string ble_time_value;


class MusicBLECallback : public BLECharacteristicCallbacks{
    void onWrite(BLECharacteristic* pCharacteristic){
      bool read_song = false;
      pAdvertising->stop();
      std::string song = "";
      std::string artist = "";
      std::string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++){
          if(value[i] == ';'){
            read_song = true;
            break;
          }
          if(read_song == false){
            song = song + value[i];
          }else{
            artist = artist + value[i];
          }
        }
      }
      ble_music_value = ble_music_value + song;
      has_music_metadata_changed = true;
      pAdvertising->start();
    }

    void onRead(BLECharacteristic* pCharacteristic){
      
    }
};

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

class BLEServerCallback : public BLEServerCallbacks{
  void onConnect(BLEServer* pServer){
      is_ble_client_connected = true;
  }

  void onDisconnect(BLEServer* pServer){
      is_ble_client_connected = false;
  }
};