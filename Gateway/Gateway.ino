#include "LoraMesh.h"
#include "esp_wifi.h"
#include "esp_bt.h"
// thời gian ngủ
// Các chân LoRa
#define SS 5
// #define RST 4
#define RST 13
#define DIO0 2
#define stationID 1
LoraMesh mesh(SS, RST, DIO0, stationID);


void setup() {
  Serial.begin(9600);
  mesh.begin();
  disable_bluetooth_wifi();
}

void loop() {
  mesh.receiveMessage();
}
void disable_bluetooth_wifi() {
  esp_bt_controller_disable();
  esp_wifi_stop();
  esp_wifi_deinit();
}