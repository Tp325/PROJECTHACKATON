#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Plantower_PMS7003.h"
#include "LoraMesh.h"

#define StationID 2
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

// dust
SoftwareSerial mySerial(33, 25);
Plantower_PMS7003 pms7003;

//độ đục nước
#define doducnuoc = 15

unsigned long int waketime = 0;

void setup() {
  pinMode(Sensor1, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  pms7003.init(&mySerial);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  digitalWrite(LED_red, HIGH);
  mesh.begin();
  waketime = millis();
}


void loop() {
  if (mesh.receiveMessage() == 1) {
    Serial.println(mesh.receiveMSG);
    waketime = millis();
  }
  if (mesh.receiveMSG == "-1") {
    digitalWrite(Sensor1, HIGH);
    long time = millis();
    int airDust;
    int batteryAnalog;
    while (millis() - time <= 20000) {
      digitalWrite(LED_yellow, HIGH);
      pms7003.updateFrame();
      airDust = pms7003.getPM_2_5();
      batteryAnalog = analogRead(ANALOG_PIN);
      delay(100);
    }
    digitalWrite(LED_yellow, LOW);


    String dataPacket = String("StationID:") + StationID + ",airDust:" + airDust + ",Battery:" + batteryAnalog;
    delay(200);

    Serial.println("Gói dữ liệu chuẩn bị gửi:");
    Serial.println(dataPacket);
    mesh.sendMessage(dataPacket, mesh.sendednode, StationID, 5);
    Serial.println("Gói dữ liệu đã gửi thành công!");
    digitalWrite(LED_blue, HIGH);
    delay(500);
    digitalWrite(LED_blue, LOW);

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
