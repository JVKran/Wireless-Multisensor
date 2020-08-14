#include "Light.hpp"

Light::Light(BH1750 & lightSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod):
	lightSensor(lightSensor),
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000)
{}

void Light::begin(){
	lightSensor.begin();		// Forced measurement is taken here
	transmitter.updateLightIntensity(lightSensor.getLightIntensity());
}

void Light::operator()(){
	if(++lastUpdateCycles >= updateCycles){
		transmitter.updateLightIntensity(lightSensor.getLightIntensity());
		lastUpdateCycles = 0;
	}
}