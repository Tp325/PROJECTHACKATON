#include "MeshLora.h"

MeshLora::MeshLora(int nodeID, int baurate, long frequency, uint8_t ttlLimit, uint8_t csPin, uint8_t resetPin, uint8_t irqPin) {
  this->nodeID = nodeID;
  this->baurate = baurate;
  this->frequency = frequency;
  this->csPin = csPin;
  this->resetPin = resetPin;
  this->irqPin = irqPin;
  Serial.begin(9600);
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("Lora is ready");
}

void MeshLora::sendMessage(String message, int destinationID) {
  LoRa.beginPacket();
  LoRa.print(String(nodeID) + "," + String(destinationID) + "," + String(ttlLimit) + "," + message);
  LoRa.endPacket();
  int time = millis();
  uint8_t lope = 1;
  while (ACK() != 1 && lope <= 3) {
    if (millis() - time >= 10000) {
      LoRa.beginPacket();
      LoRa.print(String(nodeID) + "," + String(destinationID) + "," + String(ttlLimit) + "," + message);
      LoRa.endPacket();
      time = millis();
      lope++;
    }
  }
  if (lope == 4) {
    Serial.println("Fail to send");
  } else {
    Serial.println("Sent");
  }
}
bool MeshLora::ACK() {
  int packetSize = LoRa.parsePacket();
  String receiveData = "";
  if (packetSize) {
    while (LoRa.available()) {
      receiveData += LoRa.read();
    }
  }
  if (receiveData == "received" || receiveData != "")
    return 1;
  else  {
    receiveData = "";
    return 0;
  }
}
void MeshLora::receiveMessage() {
  int packetSize = LoRa.parsePacket();
  String receiveData = "";
  if (packetSize) {
    while (LoRa.available()) {
      receiveData += LoRa.read();
    }
    parsePacket(receiveData);
  }
}
void MeshLora::parsePacket(String packet) {
  int senderID = packet.substring(0, packet.indexOf(',')).toInt();
  destinationID = packet.substring(packet.indexOf(','), packet.indexOf(',') + 1).toInt();
  int ttl = packet.substring(packet.indexOf(',') + 1, packet.lastIndexOf(',')).toInt();
  String message = packet.substring(packet.lastIndexOf(',') + 1);
  relayMessage(message, senderID, ttl);
}
void MeshLora::relayMessage(String message, int senderID, int ttl) {
  if (shouldRelay(senderID, ttl, destinationID)) {
    String packet = String(nodeID) + "," + String(destinationID) + "," + String(ttl - 1) + "," + message;
    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.endPacket();
  } else if (destinationID == nodeID) {
    this->receiveMsg = message;
    LoRa.beginPacket();
    LoRa.print("received");
    LoRa.endPacket();
  } else {
    this->receiveMsg = "";
  }
}

bool MeshLora::shouldRelay(int senderID, int ttl, int destinationID) {
  if (ttl > 0 && senderID != nodeID) {
    return 1;
  } else return 0;
  if (nodeID == destinationID) {
    return 0;
  }
}
