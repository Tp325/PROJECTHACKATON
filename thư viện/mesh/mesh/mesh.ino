#include "LoraMesh.h"
#define SS 5
#define RST 4
#define DIO0 2
#define masterID 1
#define StationID 2
LoraMesh mesh(SS, RST, DIO0, StationID);
void setup() {
  Serial.begin(9600);
  mesh.begin();
  // mesh.sendMessage("Heeloo", 2, 3);
}

void loop() {
  mesh.receiveMessage();
  if (mesh.receiveMSG == String("Huulo")) {
    mesh.sendMessage("Heeloo", 2, 3);
    Serial.println(mesh.receiveMSG);
  }
}
