#include <SPI.h>
#include "RF24.h"

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("RF24/Dakota-client"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
 
  // Setup and configure radio

  radio.begin();
  radio.startListening();

}

void loop() {
  // put your main code here, to run repeatedly:

}
