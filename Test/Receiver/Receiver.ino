#include "Manchester.h"

#define RX_PIN D2

#define BUFFER_SIZE 4
uint8_t buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(19200);
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() {
  if (man.receiveComplete()) {
    uint8_t receivedSize = buffer[0];
    for(uint8_t i=1; i<receivedSize; i++)
      Serial.print(buffer[i]);
      Serial.print(',');
    
    Serial.println();

    man.beginReceiveArray(BUFFER_SIZE, buffer);
  }
}