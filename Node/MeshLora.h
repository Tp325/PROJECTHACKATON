#ifndef MeshLora_h
#define MeshLora_h
#include <LoRa.h>
#include "Arduino.h"
#include <SPI.h>
class MeshLora {
private:
  int nodeID;
  int baurate;
  long frequency;
  uint8_t csPin;
  uint8_t resetPin;
  uint8_t irqPin;
  uint8_t ttlLimit;
  int destinationID;
  void parsePacket(String packet);
  bool shouldRelay(int senderID, int ttl,int destinationID);
  void relayMessage(String message, int senderID, int ttl);
  bool ACK();
public:
  MeshLora(int nodeID, int baurate, long frequency, uint8_t ttlLimit, uint8_t csPin, uint8_t resetPin, uint8_t irqPin);
  void sendMessage(String message, int destinationID);
  void receiveMessage();
  String receiveMsg="";
};
#endif