#include <WiFi.h>
#include <PubSubClient.h>
#include "LoraMesh.h"
#include "esp_sleep.h"
#include <WiFiManager.h>
// Thông tin Wi-Fi
const char* ssid = "Phong_4";
const char* wifi_password = "1234512345";

// Thông tin MQTT Broker
const char* mqtt_server = "mqtt.cusc.vn";
const int mqtt_port = 1883;
const char* mqtt_user = "hackathon";
const char* mqtt_pass = "hackathon@123";
// thời gian ngủ
#define sleeptime 15 * 60  // Giây
// Các chân LoRa
#define SS 5
#define RST 4
#define DIO0 2
#define stationID 0
#define uS_TO_S_FACTOR 1000000ULL
LoraMesh mesh(SS, RST, DIO0, stationID);

// Tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

//Wifi
WiFiManager wm;

void setup() {
  Serial.begin(9600);
  mesh.begin();
  connectWifi();
  // Kết nối MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  connectMQTT();
  // Serial.flush();
  // delay(500);
  // esp_sleep_enable_timer_wakeup(sleeptime * 1000000ULL);  // ngủ sau 5 giây
  // esp_deep_sleep_start();
}



void loop() {
  while (!mesh.receiveMessage())
    ;
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
  Serial.println(mesh.receiveMSG);
  // Gửi dữ liệu lên MQTT
  if (mesh.receiveMSG != "") {
    if (mqttClient.publish("Hackathon", mesh.receiveMSG.c_str())) {
      Serial.println("Dữ liệu đã được gửi lên MQTT thành công!");
    } else {
      Serial.println("Gửi dữ liệu lên MQTT thất bại!");
    }
  }
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
void connectWifi() {
  bool res;
  res = wm.autoConnect("AutoConnectAP", "AutoConnectAP");
  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected");
  }
}
