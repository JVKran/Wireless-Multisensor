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

		unsigned long lastUpdateCycles = 0;
		uint16_t updateCycles;
	public:
		Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint16_t & updateCycles = 5);

		void begin();
		void operator()();
};

#endif //__CLIMATE_HPP