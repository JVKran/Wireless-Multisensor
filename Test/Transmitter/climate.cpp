#include "climate.hpp"

tempHumSensor::tempHumSensor(RH_ASK & transmitter, const uint8_t sensorPin, const unsigned int pollPeriod):
	sensorPin(sensorPin),
	pollPeriod(pollPeriod),
	transmitter(transmitter)
{}

void tempHumSensor::measureAndPublish(){
	sensor.read(sensorPin);

	transmitter.send((uint8_t *)String("Temp/" + String(float(sensor.temperature))).c_str(), 7);
	transmitter.waitPacketSent();

	transmitter.send((uint8_t *)String("Humd/" + String(float(sensor.humidity))).c_str(), 7);
	transmitter.waitPacketSent();

	lastMeasurement = millis();
}

bool tempHumSensor::timeToMeasure(){
	return(millis() > lastMeasurement + pollPeriod);
}