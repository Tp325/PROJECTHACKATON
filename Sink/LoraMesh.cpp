#include "LoraMesh.h"
LoraMesh::LoraMesh(uint8_t SS, uint8_t RST, uint8_t DI0, int stationID) {
  this->SS = SS;
  this->RST = RST;
  this->DI0 = DI0;
  this->stationID = stationID;
}
void LoraMesh::begin() {
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) {  // Chọn tần số LoRa (433 MHz)
    Serial.println("Khởi động LoRa thất bại!");
    while (1)
      ;
  }
  Serial.println("LoRa đã sẵn sàng để truyền!");
}

void LoraMesh::sendMessage(String message, int destinationID, int ttlLimit) {
  LoRa.beginPacket();
  LoRa.print(String(destinationID) + "." + String(ttlLimit) + "." + message);
  LoRa.endPacket();
  Serial.println("Message sent: " + String(destinationID) + "." + String(ttlLimit) + "." + message);
}

bool LoraMesh::receiveMessage() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String message = "";
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }
    Serial.print("Received message: ");
    Serial.println(message);
    int destinationID = message.substring(0, message.indexOf('.')).toInt();
    int ttlLimit = message.substring(message.indexOf('.') + 1, message.lastIndexOf('.')).toInt();
    String receiveMSG = message.substring(message.lastIndexOf('.') + 1);
    if (ttlLimit > 0 && destinationID != this->stationID) {
      sendMessage(receiveMSG, destinationID, ttlLimit - 1);
    }
    if(destinationID == this->stationID){
      this->receiveMSG = receiveMSG;
    }
    return 1;
  } else {
    receiveMSG = "";
    return 0;
  }
}
