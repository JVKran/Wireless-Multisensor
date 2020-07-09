#include "Climate.hpp"

Climate::Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod):
	climateSensor(climateSensor),
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000)		// Wakes up at least every 8 seconds.
{}

void Climate::operator()(){
	if(++lastUpdateCycles >= updateCycles){
		climateSensor.takeForcedMeasurement();
		transmitter.updateClimate(	climateSensor.getTemperatureCelcius(), 
									climateSensor.getRelativeHumidity(), 
									climateSensor.getPressure()
								);
		lastUpdateCycles = 0;
	}
}

void Climate::begin(){
	climateSensor.begin();		// Forced measurement is taken here
	transmitter.updateClimate(	climateSensor.getTemperatureCelcius(), 
									climateSensor.getRelativeHumidity(), 
									climateSensor.getPressure()
								);
}
