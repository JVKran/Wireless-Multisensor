#include "Climate.hpp"

Climate::Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint32_t & updatePeriod):
	climateSensor(climateSensor),
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000)		// Wakes up at least every 8 seconds.
{}

void Climate::operator()(){
	// Millis() wouls be of great use here, I know... But it saves some flash memory!
	if(++lastUpdateCycles > updateCycles){
		climateSensor.takeForcedMeasurement();
		transmitter.updateClimate(climateSensor.getTemperature(), climateSensor.getHumidity(), climateSensor.getPressure());
		lastUpdateCycles = 0;
	}
}

void Climate::begin(){
	climateSensor.takeForcedMeasurement();
	transmitter.updateClimate(climateSensor.getTemperature(), climateSensor.getHumidity(), climateSensor.getPressure());
}