#include "config.h"

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