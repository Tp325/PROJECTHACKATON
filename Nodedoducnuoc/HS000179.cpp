#include "HS000179.h"


HS000179::HS000179(int analogSensorPin){
  this->analogSensorPin=analogSensorPin;
}
void HS000179::begin(){
  pinMode(this->analogSensorPin,INPUT);
}
float HS000179::getVoltage(){
  return (analogRead(this->analogSensorPin)*3.3/4095)*5/3.3;
}
//0.11V=49.5NTU
float HS000179::getTurbinity(){
  return this->getVoltage()*49.5/0.11;
}
