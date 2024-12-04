#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Plantower_PMS7003.h"
#include "LoraMesh.h"

#define masterID 1
#define StationID 2
#define ANALOG_PIN 26
#define LED_blue 27
#define LED_red 4
#define LED_yellow 14
// Khởi động cảm biến
#define Sensor1 32
// Các chân LoRa
#define SS 5
#define RST 13
#define DIO0 2
//

// khởi tạo mesh
LoraMesh mesh(SS, RST, DIO0, 2);
// dust
SoftwareSerial mySerial(16, 17);
Plantower_PMS7003 pms7003;
//
void setup() {
  pinMode(Sensor1, OUTPUT);
  digitalWrite(Sensor1, HIGH);
  Serial.begin(9600);
  mySerial.begin(9600);
  pms7003.init(&mySerial);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  mesh.begin();
}


void loop() {
  // Đọc giá trị pin lần đầu
  int batteryAnalog = analogRead(ANALOG_PIN);
  // Nếu pin thấp, vào chế độ ngủ ngay
  //  if (batteryAnalog < 1500) {
  //    Serial.println("Pin yếu. Đưa vào chế độ ngủ ngay.");
  //    esp_deep_sleep_start();
  //  }

  // Điều khiển LED và cảm biến
  digitalWrite(LED_blue, HIGH);
  digitalWrite(LED_red, HIGH);
  digitalWrite(LED_yellow, HIGH);
  digitalWrite(LED_blue, LOW);
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_yellow, LOW);


  // Thu thập dữ liệu từ cảm biến
  //dust
  pms7003.updateFrame();
  int airDust = pms7003.getPM_2_5();
  //
  mesh.receiveMessage();
  Serial.println(mesh.receiveMSG);
  if (mesh.receiveMSG == String(StationID)) {
    // Tạo gói dữ liệu
    delay(200);
    String dataPacket = String("StationID:") + StationID + ",airDust:" + airDust + ",Battery:" + batteryAnalog;
    delay(200);

    // In gói dữ liệu ra Serial Monitor
    Serial.println("Gói dữ liệu chuẩn bị gửi:");
    Serial.println(dataPacket);

    // Gửi dữ liệu qua LoRa
    mesh.sendMessage(dataPacket, 1, 3);


    // Xác nhận gói dữ liệu đã gửi
    Serial.println("Gói dữ liệu đã gửi thành công!");
    // Đợi trước khi gửi lần tiếp theo
    delay(500);
  }
  // Đưa ESP32 vào chế độ ngủ sau khi hoàn thành
  digitalWrite(Sensor1, LOW);
  // Serial.flush();
  // delay(500);
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_14,1); //1 = High, 0 = Low
  // esp_deep_sleep_start();
}
