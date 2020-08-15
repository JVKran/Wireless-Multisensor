/// @file

#include "Climate.hpp"

/// \brief
/// Cnstruct Instance
/// \details
/// Construct a climate instance.
/// \param climateSensor The sensor to use for measurements.
/// \param transmitter The transmitter to notify when values have changed.
/// \param updatePeriod The period in milliseconds to keep between measurements.
Climate::Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod):
	climateSensor(climateSensor),
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000)		// Wakes up at least every 8 seconds.
{}

/// \brief
/// Update
/// \details
/// Update values by taking a measurement when the updatePeriod has ended.
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

/// \brief
/// Begin Instance
/// \details
/// Begin Climate by initializing values for temperature, humidity and pressure.
void Climate::begin(){
	climateSensor.begin();		// Forced measurement is taken here
	transmitter.updateClimate(	climateSensor.getTemperatureCelcius(), 
									climateSensor.getRelativeHumidity(), 
									climateSensor.getPressure()
								);
}
