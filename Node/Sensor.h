#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include <SoftwareSerial.h>

class soild7in1 {
private:
  byte _queryTemHum[8] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E };
  byte _queryN[8] = { 0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C };
  byte _queryP[8] = { 0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC };
  byte _queryK[8] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0 };
  byte _queryEC[8] = { 0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE };
  byte _queryPH[8] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B };
  byte receiveData[9];
  uint32_t _baud_rate;
  int RX, TX;
public:
  soild7in1(int RXPIN, int TXPIN);
  void begin(long baurate);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getTemperature();  // oC
  int getHumidity();     //%
  int getPH();           // H+
  int getNito();         // mg/kg
  int getPhotpho();      // mg/kg
  int getKali();         // mg/kg
  int getSoidMoisture();
  int getEC();  // us/cm
};

class soild3in1 {
private:
  byte _queryTemHum[8] = { 0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E };
  byte _queryPH[8] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B };
  byte receiveData[10];
  uint32_t _baud_rate;
  int RX, TX;

public:
  soild3in1(int RX, int TX);
  void begin(long baurate);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getHumidity();     //%
  int getTemperature();  // oC
  int getPH();           // H+
};

class distance {
private:
  byte _queryDis[8] = { 0x50, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC8, 0x45 };
  // byte _queryDis[8] = { 0x11, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC7, 0x54 };
  // byte _queryDis[8] = { 0x22, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC2, 0x97 };
  // char send_frame_3[8] = { 0x33, 0x03, 0x00, 0x34, 0x00, 0x01, 0xC1, 0xD6 };
  uint32_t _baud_rate;
  byte receiveData[10];
  int RX, TX;

public:
  void begin(long baurate);
  distance(int RXPIN, int TXPIN);
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int getDistance();
};
class URM08 {
private:
  byte distanceArray[8] = { 0x55, 0xAA, 0x11, 0x00, 0x02, 0x12 };
  byte temperatureArray[8] = { 0x55, 0xAA, 0x11, 0x00, 0x03, 0x13 };
  byte receiveData[10];
  int getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion);
  int RX, TX;

public:
  URM08(int RX, int TX);
  void begin(long baurate);
  int getDistance();             //cm
  float getTemperature();        //oc
  int setAddress(byte address);  // if == 1 => setOK if == 2 => setnotOK
};
#endif