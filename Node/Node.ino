#include <LoRa.h>
#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>
#include "softwareSerial"
// dust
softwareSerial Serial1(16, 17);
Plantower_PMS7003 pms7003 = Plantower_PMS7003();
//
#define StationID 2
#define sleeptime 15 * 60  // Giây
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

void setup() {
  pinMode(Sensor1, OUTPUT);
  digitalWrite(Sensor1, HIGH);
  Serial.begin(9600);
  Serial1.begin(9600);
  pms7003.init(&Serial1);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {  // Chọn tần số LoRa (433 MHz)
    Serial.println("Khởi động LoRa thất bại!");
    while (1)
      ;
  }

  Serial.println("LoRa đã sẵn sàng để truyền!");
  // Đọc giá trị pin lần đầu
  int batteryAnalog = analogRead(ANALOG_PIN);
  // Nếu pin thấp, vào chế độ ngủ ngay
  //  if (batteryAnalog < 1500) {
  //    Serial.println("Pin yếu. Đưa vào chế độ ngủ ngay.");
  //    esp_deep_sleep_start();
  //  }

  // Nếu pin đủ, thực hiện đo và gửi ba lần
  for (int i = 0; i <= 3; i++) {
    // Điều khiển LED và cảm biến
    digitalWrite(LED_blue, HIGH);
    digitalWrite(LED_red, HIGH);
    digitalWrite(LED_yellow, HIGH);
    delay(500);
    digitalWrite(LED_blue, LOW);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_yellow, LOW);


    // Thu thập dữ liệu từ cảm biến
    //dust 
     pms7003.updateFrame();
    int airDust = pms7003.pms7003.getPM_2_5();
    //





    // Tạo gói dữ liệu
    String dataPacket = String("StationID:") + StationID + ",airDust:" + airDust + ",Battery:" + batteryAnalog;

    // In gói dữ liệu ra Serial Monitor
    Serial.println("Gói dữ liệu chuẩn bị gửi:");
    Serial.println(dataPacket);

    // Gửi dữ liệu qua LoRa
    LoRa.beginPacket();
    LoRa.print(dataPacket);
    LoRa.endPacket();

    // Xác nhận gói dữ liệu đã gửi
    Serial.println("Gói dữ liệu đã gửi thành công!");

    // Đợi trước khi gửi lần tiếp theo
    delay(500);
  }

  // Đưa ESP32 vào chế độ ngủ sau khi hoàn thành
  digitalWrite(Sensor1, LOW);
  Serial.flush();
  delay(500);
  esp_sleep_enable_timer_wakeup(sleeptime * 1000000ULL);  // ngủ sau 5 giây
  esp_deep_sleep_start();
  // Cấu hình thời gian sleep
}

void loop() {
}
