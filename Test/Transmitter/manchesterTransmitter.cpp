#include "manchesterTransmitter.hpp"

ManchesterTransmitter::ManchesterTransmitter(const uint8_t & id, const uint8_t & pin, const uint8_t & baudrate):
	id(id)
{
	man.setupTransmit(pin, baudrate);
}

void ManchesterTransmitter::operator()(){
	if(dataChanged){
		transmitData();
		dataChanged = false;
		lastMotion = false;
	}
}

void ManchesterTransmitter::transmitData(){
	// uint8_t bufferSize = 9;
	// uint8_t data[bufferSize] = {bufferSize, id, lastMotion, (lastTemperature & 0xF0) >> 8, lastTemperature & 0x0F, (lastHumidity & 0xF0) >> 8, lastHumidity & 0x0F, (lastPressure & 0xF0) >> 8, lastPressure & 0x0F};
	uint8_t bufferSize = 11;
	uint8_t data[bufferSize] = {bufferSize, id, lastMotion, lastTemperature & 0x00FF, lastTemperature >> 8, lastHumidity & 0x00FF, lastHumidity >> 8, lastPressure & 0x00FF, lastPressure >> 8, lastPressure >> 16, lastPressure >> 24};
	man.transmitArray(bufferSize, data);
}

void ManchesterTransmitter::updateMotion(){
	lastMotion = true;
	dataChanged = true;
}

void ManchesterTransmitter::updateClimate(const int16_t temperature, const int16_t humidity, const int32_t pressure){
	lastTemperature = temperature;
	lastHumidity = humidity;
	lastPressure = pressure;
	dataChanged = true;
}
