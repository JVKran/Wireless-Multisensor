/// @file

#include "Light.hpp"

/// \brief
/// Cnstruct Instance
/// \details
/// Construct a Light instance.
/// \param lightSensor The sensor to use for measurements.
/// \param transmitter The transmitter to notify when value has changed.
/// \param updatePeriod The period in milliseconds to keep between measurements.
Light::Light(BH1750 & lightSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod):
	lightSensor(lightSensor),
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000)
{}

/// \brief
/// Begin Instance
/// \details
/// Begin Light by initializing value for light intensity.
void Light::begin(){
	lightSensor.begin();		// Forced measurement is taken here
	transmitter.updateLightIntensity(lightSensor.getLightIntensity());
}

/// \brief
/// Update Value
/// \details
/// Update value by taking a measurement at the configured interval.
void Light::operator()(){
	if(++lastUpdateCycles >= updateCycles){
		transmitter.updateLightIntensity(lightSensor.getLightIntensity());
		lastUpdateCycles = 0;
	}
}