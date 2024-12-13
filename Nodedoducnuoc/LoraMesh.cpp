#include "LoraMesh.h"
LoraMesh::LoraMesh(uint8_t SS, uint8_t RST, uint8_t DI0, int stationID) {
  this->SS = SS;
  this->RST = RST;
  this->DI0 = DI0;
  this->stationID = stationID;
}
void LoraMesh::begin() {
  LoRa.setPins(SS, RST, DI0);
  while (!LoRa.begin(433E6)) {  // Chọn tần số LoRa (433 MHz)
    Serial.println("Khởi động LoRa thất bại!");
  }
  Serial.println("LoRa đã sẵn sàng để truyền!");
}

void LoraMesh::sendMessage(String message, int destinationID, int sendednode, int ttlLimit) {
  LoRa.beginPacket();
  LoRa.print(String(destinationID) + "." + String(sendednode) + "." + String(ttlLimit) + "." + message);
  LoRa.endPacket();
  Serial.println("Message sent: " + String(destinationID) + "." + String(sendednode) + "." + String(ttlLimit) + "." + message);
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

    int firstDot = message.indexOf('.');
    int secondDot = message.indexOf('.', firstDot + 1);
    int thirdDot = message.indexOf('.', secondDot + 1);
    

    // Phần 1: destinationID (Trước dấu chấm đầu tiên)
    int destinationID = message.substring(0, message.indexOf('.')).toInt();

    // Phần 2: sendednode (Giữa dấu chấm đầu tiên và thứ hai)
    int sendednode = message.substring(firstDot + 1, secondDot).toInt();
    
    // Phần 3: ttlLimit (Giữa dấu chấm thứ hai và thứ ba)
    int ttlLimit = message.substring(secondDot + 1, thirdDot).toInt();

    // Phần 4: receiveMSG (Sau dấu chấm thứ ba)
    String receiveMSG = message.substring(thirdDot + 1);

    if (ttlLimit > 0 && destinationID != this->stationID) {
      sendMessage(receiveMSG, destinationID, sendednode, ttlLimit - 1);
    }
    if (destinationID == this->stationID) {
      this->receiveMSG = receiveMSG;
      this->sendednode=sendednode;
    }
    else{
      this->receiveMSG = "";
    }
    return 1;
  } else {
    this->receiveMSG = "";
    return 0;
  }
}
