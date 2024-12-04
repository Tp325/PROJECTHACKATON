
/*********************************************************************
This is an example which demonstrates use of the Plantower PMS7003
laser particle counter. This should also work the the PMS5003 sensor,
as they share the same protocol.

For more information about the PMS7003, see README.md.

Written by Jason Striegel.
BSD license. See license.txt for details.
**********************************************************************/

#include "Plantower_PMS7003.h"


char output[256];
Plantower_PMS7003 pms7003 = Plantower_PMS7003();


void setup() {
  Serial.begin(9600);

  Serial1.begin(9600);
  pms7003.init(&Serial1);
  //pms7003.debug = true;
}

void loop() {
  pms7003.updateFrame();
  if (pms7003.hasNewData()) {
    sprintf(output, "    PM2.5 (ug/m3): %2d     [atmos: %d]\n",
            pms7003.getPM_2_5(),
            pms7003.getPM_2_5_atmos());
    Serial.print(output);
  }
}
