#include <WiFi.h>
#include <PubSubClient.h>
#include "LoraMesh.h"
#include "esp_sleep.h"

// Thông tin Wi-Fi
const char* ssid = "Phong_4";
const char* wifi_password = "1234512345";

// Thông tin MQTT Broker
const char* mqtt_server = "mqtt.cusc.vn";
const int mqtt_port = 1883;
const char* mqtt_user = "Hardware_Test";
const char* mqtt_pass = "test@123";
// thời gian ngủ
#define sleeptime 15 * 60  // Giây
// Các chân LoRa
#define SS 5
#define RST 13
#define DIO0 2
#define stationID 1
#define uS_TO_S_FACTOR 1000000ULL
LoraMesh mesh(SS, RST, DIO0, stationID);

// Tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(9600);
  mesh.begin();
  //Kết nối WiFi
  setupWiFi();

  // Kết nối MQTT

  // Serial.flush();
  // delay(500);
  // esp_sleep_enable_timer_wakeup(sleeptime * 1000000ULL);  // ngủ sau 5 giây
  // esp_deep_sleep_start();
}



void loop() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  connectMQTT();
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
  mesh.sendMessage("-1", 10, stationID, 5);
  delay(3000);
  for (int i = 2; i <= 5; i++) {
    mesh.sendMessage("-1", i, stationID, 5);
    long time = millis();
    while (!mesh.receiveMessage() && millis() - time <= 5000)
      ;
    Serial.println(mesh.receiveMSG);

    // Gửi dữ liệu lên MQTT
    if (mesh.receiveMSG != "") {
      if (mqttClient.publish("Hackaton", mesh.receiveMSG.c_str())) {
        Serial.println("Dữ liệu đã được gửi lên MQTT thành công!");
      } else {
        Serial.println("Gửi dữ liệu lên MQTT thất bại!");
      }
      delay(2000);
    }
  }
  Serial.println("Going to sleep now");
  Serial.flush();
  delay(500);
  esp_sleep_enable_timer_wakeup(15 * uS_TO_S_FACTOR * 60);
  esp_deep_sleep_start();
}


// Hàm kết nối Wi-Fi
void setupWiFi() {
  Serial.print("Đang kết nối với WiFi...");
  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Đã kết nối với WiFi!");
}

// Hàm kết nối MQTT
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Đang kết nối với MQTT Broker...");
    if (mqttClient.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println(" Đã kết nối với MQTT Broker!");
    } else {
      Serial.print(" Thất bại với mã lỗi: ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}
