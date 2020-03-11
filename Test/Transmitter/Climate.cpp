#include "Climate.hpp"

Climate::Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint16_t & updatePeriod):
	climateSensor(climateSensor),
	transmitter(transmitter),
	updatePeriod(updatePeriod)
{}

void Climate::operator()(){
	if(millis() - lastUpdate > updatePeriod){
		climateSensor.takeForcedMeasurement();
		transmitter.updateClimate(climateSensor.getTemperature(), climateSensor.getHumidity(), climateSensor.getPressure());
		lastUpdate = millis();
	}
}