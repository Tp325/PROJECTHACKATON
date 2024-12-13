#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>
#include "LoraMesh.h"
#include "HS000179.h"

#define StationID 3
#define SinkID 0
// Thiết lập ngủ
#define sleeptime 15 * 60  //phút
#define uS_TO_S_FACTOR 1000000ULL

#define ANALOG_PIN 22
#define LED_blue 26
#define LED_red 32
#define LED_yellow 27
// Khởi động cảm biến
#define Sensor1 21

// Các chân LoRa
#define SS 5
#define RST 4
#define DIO0 2

// khởi tạo mesh
LoraMesh mesh(SS, RST, DIO0, StationID);



//độ đục nước
//#define doducnuoc = 15
HS000179 doducnuoc(15);


void setup() {
  Serial.begin(9600);
  pinMode(Sensor1, OUTPUT);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  digitalWrite(LED_red, HIGH);
  mesh.begin();
  doducnuoc.begin();
}


void loop() {
  float waterTurbidity;
  float batteryAnalog;
  int time = millis();
  digitalWrite(Sensor1, HIGH);
  while (millis() - time <= 20000) {
    digitalWrite(LED_yellow, HIGH);
    waterTurbidity = doducnuoc.getTurbinity();
    batteryAnalog = (analogRead(ANALOG_PIN) * 3.3 / 4095) * 5 / 3.3;
    delay(100);
  }
  digitalWrite(LED_yellow, LOW);
  digitalWrite(Sensor1, LOW);

  String dataPacket = String("StationID:") + StationID + ",waterTurbidity:" + waterTurbidity + ",Battery:" + batteryAnalog;
  delay(200);

  Serial.println("Gói dữ liệu chuẩn bị gửi:");
  Serial.println(dataPacket);
  mesh.sendMessage(dataPacket, SinkID, StationID, 5);
  Serial.println("Gói dữ liệu đã gửi thành công!");
  digitalWrite(LED_blue, HIGH);
  delay(500);
  digitalWrite(LED_blue, LOW);

  Serial.println("Đưa vào chế độ ngủ");
  digitalWrite(LED_red, LOW);
  Serial.println("Going to sleep now");
  Serial.flush();
  delay(500);
  esp_sleep_enable_timer_wakeup(sleeptime* uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
