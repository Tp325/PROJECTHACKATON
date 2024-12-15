#ifndef LoraMesh_h
#define LoraMesh_h
#include <LoRa.h>
#include "Arduino.h"
#include <SPI.h>
class LoraMesh {
private:
  uint8_t SS;
  uint8_t RST;
  uint8_t DI0;
  int stationID;

public:
  LoraMesh(uint8_t SS, uint8_t RST, uint8_t DI0,int stationID);
  void begin();
  void sendMessage(String message, int destinationID, int sendednode, int ttlLimit);
  bool receiveMessage();
  String receiveMSG;
  int sendednode;
};
#endif