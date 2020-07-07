#include "Climate.hpp"

Climate::Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod):
	climateSensor(climateSensor),
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000),		// Wakes up at least every 8 seconds.
	lastUpdateCycles(updateCycles)
{}

void Climate::operator()(){
	if(++lastUpdateCycles > updateCycles){
		climateSensor.takeForcedMeasurement();
		int16_t temp = climateSensor.getTemperatureCelcius();
		int16_t hum = climateSensor.getRelativeHumidity();
		int32_t pres = climateSensor.getPressure();
		transmitter.updateClimate(temp, hum, pres);
		lastUpdateCycles = 0;
	}
}

void Climate::begin(){
	climateSensor.begin();		// Forced measurement is taken here
	int16_t temp = climateSensor.getTemperatureCelcius();
	int16_t hum = climateSensor.getRelativeHumidity();
	int32_t pres = climateSensor.getPressure();
	transmitter.updateClimate(temp, hum, pres);
}
