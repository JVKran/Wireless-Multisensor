#ifndef __CLIMATE_HPP
#define __CLIMATE_HPP

#include <stdint.h>
#include <Arduino.h>
#include <forcedClimate.h>
#include "manchesterTransmitter.hpp"

class Climate {
	private:
		ForcedClimate & climateSensor;
		ManchesterTransmitter & transmitter;

		unsigned long lastUpdate = 0;
		uint16_t updatePeriod;
	public:
		Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint16_t & updatePeriod = 20000);

		void operator()();
};

#endif //__CLIMATE_HPP