#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Plantower_PMS7003.h"
#include "LoraMesh.h"

#define StationID 2
#define ANALOG_PIN 26
#define LED_blue 34
#define LED_red 32
#define LED_yellow 35
// Khởi động cảm biến
#define Sensor1 21

// Các chân LoRa
#define SS 5
#define RST 4
#define DIO0 2

// khởi tạo mesh
LoraMesh mesh(SS, RST, DIO0, 2);

// dust
SoftwareSerial mySerial(25, 33);
Plantower_PMS7003 pms7003;

//độ đục nước
#define doducnuoc=15

unsigned long int waketime = 0;

void onLoRaInterrupt();
void setup() {
  pinMode(Sensor1, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  pms7003.init(&mySerial);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  pinMode(DIO0, INPUT);
  mesh.begin();
  attachInterrupt(digitalPinToInterrupt(DIO0), onLoRaInterrupt, FALLING);
}


void loop() {

  if (mesh.receiveMSG == "-1") {
    digitalWrite(Sensor1, HIGH);
    long time = millis();
    int airDust;
    while (millis() - time <= 20000) {
      pms7003.updateFrame();
      airDust = pms7003.getPM_2_5();
      delay(100);
    }

    // Đọc giá trị pin lần đầu
    int batteryAnalog = analogRead(ANALOG_PIN);

    // Tạo gói dữ liệu
    String dataPacket = String("StationID:") + StationID + ",airDust:" + airDust + ",Battery:" + batteryAnalog;
    delay(200);

    // In gói dữ liệu ra Serial Monitor
    Serial.println("Gói dữ liệu chuẩn bị gửi:");
    Serial.println(dataPacket);
    // Gửi dữ liệu qua LoRa
    mesh.sendMessage(dataPacket, mesh.sendednode,StationID, 5);
    // Xác nhận gói dữ liệu đã gửi
    Serial.println("Gói dữ liệu đã gửi thành công!");
    // Điều khiển LED và cảm biến
    digitalWrite(LED_blue, HIGH);
    digitalWrite(LED_red, HIGH);
    digitalWrite(LED_yellow, HIGH);
    digitalWrite(LED_blue, LOW);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_yellow, LOW);
    // Đợi trước khi gửi lần tiếp theo
    digitalWrite(Sensor1, LOW);
  }

  //Đưa ESP32 vào chế độ ngủ sau khi hoàn thành
  if (millis() - waketime >= 60000) {
    Serial.println("Đưa vào chế độ ngủ");
    Serial.flush();
    delay(500);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, LOW);
    esp_light_sleep_start();
  }
}

void onLoRaInterrupt() {
  Serial.println("Kích hoạt ngắt");
  mesh.receiveMessage();
  Serial.println(mesh.receiveMSG);
  waketime = millis();
}
