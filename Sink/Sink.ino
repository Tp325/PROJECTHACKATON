#include <WiFi.h>
#include <PubSubClient.h>
#include "MeshLora.h"

// Thông tin Wi-Fi
const char* ssid = "Phong_4";
const char* wifi_password = "1234512345";

// Thông tin MQTT Broker
const char* mqtt_server = "103.221.220.183";
const int mqtt_port = 1883;
const char* mqtt_user = "api1@Iotlab";
const char* mqtt_pass = "Iotlab@2023";
// thời gian ngủ
#define sleeptime 15 * 60  // Giây
// Các chân LoRa
#define SS 5
#define RST 4
#define DIO0 2
#define masterID 1
MeshLora mesh(masterID, 9600, 433E6, 5, SS, RST, DIO0);

// Tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(9600);
  // Kết nối WiFi
  setupWiFi();

  // Kết nối MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  connectMQTT();
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
  for (int i = 2; i <= 4; i++) {
    mesh.sendMessage("wakeup", i);
    mesh.receiveMessage();
    Serial.print("Dữ liệu nhận được từ LoRa: ");
    Serial.println(mesh.receiveMsg);

    //Gửi dữ liệu lên MQTT
    if (mqttClient.publish("Hackaton", mesh.receiveMsg.c_str())) {
      Serial.println("Dữ liệu đã được gửi lên MQTT thành công!");
    } else {
      Serial.println("Gửi dữ liệu lên MQTT thất bại!");
    }
  }
  Serial.flush();
  delay(500);
  esp_sleep_enable_timer_wakeup(sleeptime * 1000000ULL);  // ngủ sau 5 giây
  esp_deep_sleep_start();
}

void loop() {
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
