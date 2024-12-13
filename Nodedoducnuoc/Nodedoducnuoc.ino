#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>
#include "LoraMesh.h"
#include "HS000179.h"
#define StationID 3

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

unsigned long int waketime = 0;

void setup() {
  pinMode(Sensor1, OUTPUT);
  Serial.begin(9600);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  pinMode(DIO0, INPUT);
  mesh.begin();
  doducnuoc.begin();
  digitalWrite(LED_red, HIGH);
}


void loop() {
   if (mesh.receiveMessage() == 1) {
    Serial.println(mesh.receiveMSG);
    waketime = millis();
  }
  if (mesh.receiveMSG == "-1") {
    digitalWrite(Sensor1, HIGH);
    long time = millis();
    float waterTurbidity=doducnuoc.getTurbinity();
    while (millis() - time <= 20000) {
      digitalWrite(LED_yellow, HIGH);
      waterTurbidity = 0;
      delay(100);
    }
    digitalWrite(LED_yellow, LOW);


    // Đọc giá trị pin lần đầu
    int batteryAnalog = analogRead(ANALOG_PIN);

    // Tạo gói dữ liệu
    String dataPacket = String("StationID:") + StationID + ",waterTurbidity:" + waterTurbidity + ",Battery:" + batteryAnalog;
    delay(200);

    // In gói dữ liệu ra Serial Monitor
    Serial.println("Gói dữ liệu chuẩn bị gửi:");
    Serial.println(dataPacket);
    // Gửi dữ liệu qua LoRa
    mesh.sendMessage(dataPacket, mesh.sendednode, StationID, 5);
    // Xác nhận gói dữ liệu đã gửi
    Serial.println("Gói dữ liệu đã gửi thành công!");
    // Điều khiển LED và cảm biến
    digitalWrite(LED_blue, HIGH);
    delay(500);
    digitalWrite(LED_blue, LOW);

    // Đợi trước khi gửi lần tiếp theo
    digitalWrite(Sensor1, LOW);
  }

 if (millis() - waketime >= 60000) {
    Serial.println("Đưa vào chế độ ngủ");
    digitalWrite(LED_red, LOW);
    Serial.println("Going to sleep now");
    Serial.flush();
    delay(500);
    esp_sleep_enable_timer_wakeup(15 * uS_TO_S_FACTOR * 60);
    esp_deep_sleep_start();
    //     Serial.flush();
    //     delay(500);
    //     // gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_HIGH_LEVEL);
    //     // esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_ON);  // Giữ nguồn điện cho các chân GPIO
    //     // esp_sleep_enable_gpio_wakeup();
    //     // esp_light_sleep_start();  // Bắt đầu chế độ ngủ nhẹ
    //     // waketime = millis();
    //     // LoRa.idle();
    //     // esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, HIGH);
    //     // esp_deep_sleep_start();
  }
}



