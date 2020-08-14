#include "sensorBridge.hpp"

SensorBridge::SensorBridge(const uint8_t receivePin, const uint8_t baudrate):
	receiver(man)
{
	receiver.setupReceive(receivePin, baudrate);
	receiver.beginReceiveArray(BUFFER_SIZE, buffer);
}

void SensorBridge::operator()(){
	if(receiver.receiveComplete()){
		uint8_t sensorId = buffer[1];
		bool motion = buffer[2];
		bool reedState = buffer[3];
		int16_t temperature = buffer[4] + (buffer[5] << 8);
		int16_t humidity = buffer[6] + (buffer[7] << 8);
		int32_t pressure = buffer[8] + ((buffer[9] << 8) + ((buffer[10] << 16) + (buffer[11] << 24)));
		uint16_t voltage = buffer[12] + (buffer[13] << 8);
		uint16_t lightIntensity = buffer[14] + (buffer[15] << 8);
		notifyListeners(sensorId, motion, reedState, temperature, humidity, pressure, voltage, lightIntensity);
		receiver.beginReceiveArray(BUFFER_SIZE, buffer);
	}
}