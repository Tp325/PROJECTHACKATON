#include <SoftwareSerial.h>
#include "Arduino.h"
#ifndef HS000179_h
#define HS000179_h

class HS000179 {
private:
  int analogSensorPin;
  float getVoltage();
public:
  HS000179(int analogSensorPin);
  float voltageOfSensor;
  void begin();
  float getTurbinity();
};
#endif