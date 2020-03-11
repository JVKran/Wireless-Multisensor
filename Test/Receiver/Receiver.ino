#include "Manchester.h"

#define RX_PIN D2

#define BUFFER_SIZE 11
uint8_t buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(19200);
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() {
  if (man.receiveComplete()) {
    Serial.print("Buffersize: ");
    Serial.print(buffer[0]);
    Serial.print(", id: ");
    Serial.print(buffer[1]);
    Serial.print(", motion: ");
    Serial.print(buffer[2]);

    Serial.print(", temperature: ");
    int16_t temperature = buffer[3] + (buffer[4] << 8);
    Serial.print(temperature);

    Serial.print(", humidity: ");
    int16_t humidity = buffer[5] + (buffer[6] << 8);
    Serial.print(humidity);

    Serial.print(", pressure: ");
    int32_t pressure = buffer[7] + ((buffer[8] << 8) + ((buffer[9] << 16) + (buffer[10] << 24)));
    Serial.println(pressure);

    man.beginReceiveArray(BUFFER_SIZE, buffer);
  }
}