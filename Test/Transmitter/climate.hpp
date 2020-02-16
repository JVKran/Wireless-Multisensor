#ifndef __CLIMATE_HPP
#define __CLIMATE_HPP

#include <dht11.h>
#include <RH_ASK.h>

class tempHumSensor {
	private:
		dht11 sensor;
		const uint8_t sensorPin;

		const unsigned int pollPeriod;
		unsigned long lastMeasurement = 0;

		RH_ASK & transmitter;
	public:
		tempHumSensor(RH_ASK & transmitter, const uint8_t sensorPin, const unsigned int pollPeriod);

		void measureAndPublish();
		bool timeToMeasure();

};

#endif //__CLIMATE_HPP