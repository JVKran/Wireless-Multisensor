#include "manchesterTransmitter.hpp"

ManchesterTransmitter::ManchesterTransmitter(const uint8_t id):
	id(id)
{}

void ManchesterTransmitter::begin(const uint8_t transmitPin, const uint8_t baudrate){
	man.setupTransmit(transmitPin, baudrate);
}

void ManchesterTransmitter::operator()(){
	if(dataChanged){
		transmitData();
		dataChanged = false;
		lastMotion = false;
	}
}

void ManchesterTransmitter::transmitData(){
	uint8_t bufferSize = 16;
	uint8_t data[bufferSize] = {bufferSize, id, lastMotion, lastReedState,
								lastTemperature & 0x00FF, lastTemperature >> 8, 
								lastHumidity & 0x00FF, lastHumidity >> 8, 
								lastPressure & 0x00FF, lastPressure >> 8, lastPressure >> 16, lastPressure >> 24, 
								lastVoltage & 0x00FF, lastVoltage >> 8,
								lastIntensity & 0x00FF, lastIntensity >> 8
								};

	man.transmitArray(bufferSize, data);
}

void ManchesterTransmitter::updateMotion(){
	lastMotion = true;
	dataChanged = true;
}

void ManchesterTransmitter::updateVoltage(const uint16_t voltage){
	lastVoltage = voltage;
	dataChanged = true;
}

void ManchesterTransmitter::updateClimate(const int16_t temperature, const int16_t humidity, const uint32_t pressure){
	lastTemperature = temperature;
	lastHumidity = humidity;
	lastPressure = pressure - 6400;			// Minus 6400 for 1MHz mode for some unkown reason; bug? After substraction everything is how it should be.
	dataChanged = true;
}

void ManchesterTransmitter::updateLightIntensity(const uint16_t intensity){
	lastIntensity = intensity;
	dataChanged = true;
}

void ManchesterTransmitter::updateReed(const bool state){
	if(lastReedState != state){
		lastReedState = state;
		dataChanged = true;
	}
}
