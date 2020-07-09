#include "manchesterTransmitter.hpp"

ManchesterTransmitter::ManchesterTransmitter(const uint8_t id, const uint8_t enablePin):
	id(id),
	enablePin(enablePin)
{}

void ManchesterTransmitter::begin(const uint8_t transmitPin, const uint8_t baudrate){
	pinMode(enablePin, OUTPUT);
	digitalWrite(enablePin, LOW);
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
	uint8_t bufferSize = 13;
	uint8_t data[bufferSize] = {bufferSize, id, lastMotion, 
								lastTemperature & 0x00FF, lastTemperature >> 8, 
								lastHumidity & 0x00FF, lastHumidity >> 8, 
								lastPressure & 0x00FF, lastPressure >> 8, lastPressure >> 16, lastPressure >> 24, 
								lastVoltage & 0x00FF, lastVoltage >> 8
								};

	digitalWrite(enablePin, HIGH);
	man.transmitArray(bufferSize, data);
	digitalWrite(enablePin, LOW);
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
