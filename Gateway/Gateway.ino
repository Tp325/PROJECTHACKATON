#include "LoraMesh.h"
#include "esp_sleep.h"
// thời gian ngủ
#define sleeptime 15 * 60  // Giây
// Các chân LoRa
#define SS 5
#define RST 13
#define DIO0 2
#define stationID 1
#define uS_TO_S_FACTOR 1000000ULL
LoraMesh mesh(SS, RST, DIO0, stationID);


void setup() {
  Serial.begin(9600);
  mesh.begin();
}

void loop() {
  mesh.receiveMessage()
}

