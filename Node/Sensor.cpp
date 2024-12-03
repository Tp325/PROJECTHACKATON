#include "Sensor.h"
#include "Arduino.h"
SoftwareSerial mySerial(16, 17);
soild7in1 ::soild7in1(int RX, int TX) {
  this->RX = RX;
  this->TX = TX;
}
void soild7in1 ::begin(long baurate) {
  mySerial.begin(baurate);
}
int soild7in1::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                              uint8_t possion) {
  mySerial.write(dataForSend, 8);
  delay(200);
  if (mySerial.available()) {
    //    byte response[sizeOfData - 1];
    mySerial.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    Serial.println("ero");
    return 0;
  }
}
int soild7in1 ::getHumidity() {
  return getSensorValue(_queryTemHum, 10, 3);
}
int soild7in1 ::getTemperature() {
  return getSensorValue(_queryTemHum, 10, 5);
}
int soild7in1 ::getPH() {
  return getSensorValue(_queryPH, 8, 3);
}
int soild7in1 ::getNito() {
  return getSensorValue(_queryN, 8, 3);
}
int soild7in1 ::getPhotpho() {
  return getSensorValue(_queryP, 8, 3);
}
int soild7in1 ::getKali() {
  return getSensorValue(_queryK, 8, 3);
}
int soild7in1 ::getEC() {
  return getSensorValue(_queryEC, 8, 3);
}


soild3in1::soild3in1(int RX, int TX) {
  this->RX = RX;
  this->TX = TX;
}
void soild3in1::begin(long baurate) {
  mySerial.begin(baurate);
}
int soild3in1::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                              uint8_t possion) {
  mySerial.write(dataForSend, 8);
  delay(200);
  if (mySerial.available()) {
    //    byte response[sizeOfData - 1];
    mySerial.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}

int soild3in1 ::getHumidity() {
  return getSensorValue(_queryTemHum, 10, 3);
}
int soild3in1 ::getTemperature() {
  return getSensorValue(_queryTemHum, 10, 5);
}
int soild3in1 ::getPH() {
  return getSensorValue(_queryPH, 8, 3);
}

distance::distance(int RX, int TX) {
  this->RX = RX;
  this->TX = TX;
}
void distance::begin(long baurate) {
  mySerial.begin(baurate);
}
int distance::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                             uint8_t possion) {
  mySerial.write(dataForSend, 8);
  delay(200);
  if (mySerial.available()) {
    //    //    byte response[sizeOfData - 1];
    mySerial.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
int distance::getDistance() {
  return getSensorValue(_queryDis, 8, 3);
}
URM08::URM08(int RX, int TX) {
  this->RX = RX;
  this->TX = TX;
}
void URM08::begin(long baurate) {
  mySerial.begin(baurate);
}
int URM08::getSensorValue(byte dataForSend[8], uint8_t sizeOfData,
                          uint8_t possion) {
  mySerial.write(dataForSend, 8);
  delay(200);
  if (mySerial.available()) {
    mySerial.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
int URM08::getDistance() {
  return getSensorValue(distanceArray, 8, 5);
}
float URM08::getTemperature() {
  return getSensorValue(temperatureArray, 8, 5) / 10.0;
}
int URM08::setAddress(byte newAddress) {
  byte addressformat[8] = { 0x55, 0xAA, 0xAB, 0x01, 0x55, newAddress, 0x12 };
  for (int i = 0; i <= 7; i++) {
    mySerial.write(addressformat[i]);
  }
  if (mySerial.available()) {
    mySerial.readBytes(receiveData, 6);
  }
  if (receiveData[5] == 0xCC) {
    return 1;
  } else {
    return 0;
  }
}