#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>

// Thông tin Wi-Fi
const char* ssid = "Phong_4";
const char* wifi_password = "1234512345";

// Thông tin MQTT Broker
const char* mqtt_server = "103.221.220.183";
const int mqtt_port = 1883;
const char* mqtt_user = "api1@Iotlab";
const char* mqtt_pass = "Iotlab@2023";

// Các chân LoRa
#define SS 5
#define RST 4
#define DIO0 2

// Tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(9600);
  setupWiFi();

  // Khởi động LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {  // Tần số 433 MHz
    Serial.println("Khởi động LoRa thất bại!");
    while (1)
      ;
  }
  Serial.println("LoRa đã sẵn sàng để nhận dữ liệu!");

  // Kết nối WiFi

  // Kết nối MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  // Kiểm tra xem có gói dữ liệu nào từ LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    Serial.print("Dữ liệu nhận được từ LoRa: ");
    Serial.println(receivedData);

    // Gửi dữ liệu lên MQTT
    if (mqttClient.publish("Hackaton", receivedData.c_str())) {
      Serial.println("Dữ liệu đã được gửi lên MQTT thành công!");
    } else {
      Serial.println("Gửi dữ liệu lên MQTT thất bại!");
    }    
  }
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
