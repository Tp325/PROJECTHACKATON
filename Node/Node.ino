#include <LoRa.h>
#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>

#define StationID 2
#define sleeptime 15 * 60  // Giây
#define ANALOG_PIN 26
#define LED_blue 27
#define LED_red 4
#define LED_yellow 14
// Khởi động cảm biến
#define Sensor1 32
#define Sensor2 33
// Các chân LoRa
#define SS 5
#define RST 13
#define DIO0 2
//
soild7in1 soilSensor(9600, 16, 17);
distance myDistanceSensor(115200, 16, 17);

void setup() {
  pinMode(Sensor1, OUTPUT);
  pinMode(Sensor2, OUTPUT);
  digitalWrite(Sensor1, HIGH);
  digitalWrite(Sensor2, HIGH);
  delay(30000);
  Serial.begin(9600);
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
  for (int i = 0; i < 3; i++) {
    // Điều khiển LED và cảm biến
    digitalWrite(LED_blue, HIGH);
    digitalWrite(LED_red, HIGH);
    digitalWrite(LED_yellow, HIGH);
    delay(500);
    digitalWrite(LED_blue, LOW);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_yellow, LOW);


    // Thu thập dữ liệu từ cảm biến
    int humidity = soilSensor.getHumidity();
    int kali = soilSensor.getKali();
    int nito = soilSensor.getNito();
    int photpho = soilSensor.getPhotpho();
    int ph = soilSensor.getPH();
    int temperature = soilSensor.getTemperature();
    int ec = soilSensor.getEC();
    int distance = myDistanceSensor.getDistance()/10;

    // Tạo gói dữ liệu
    String dataPacket = String("StationID:") + StationID + ",Humidity:" + humidity + ",Kali:" + kali + ",Nito:" + nito + ",Photpho:" + photpho + ",pH:" + ph + ",Temperature:" + temperature + ",EC:" + ec + ",Distance:" + distance + ",Battery:" + batteryAnalog;

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
  digitalWrite(Sensor2, LOW);
  Serial.flush();
  delay(500);
  esp_sleep_enable_timer_wakeup(sleeptime * 1000000ULL);  // ngủ sau 5 giây
  esp_deep_sleep_start();
  // Cấu hình thời gian sleep
}

void loop() {
}
