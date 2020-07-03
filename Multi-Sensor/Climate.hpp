#ifndef __CLIMATE_HPP
#define __CLIMATE_HPP

#include <stdint.h>
#include <Arduino.h>
#include <forcedClimate.h>
#include "manchesterTransmitter.hpp"

#define CLM_5MIN  300000
#define CLM_10MIN 600000
#define CLM_15MIN 900000

class Climate {
	private:
		ForcedClimate & climateSensor;
		ManchesterTransmitter & transmitter;

		uint16_t lastUpdateCycles;
		uint16_t updateCycles;
	public:
		Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod = CLM_5MIN);

		void begin();
		void operator()();
};

#endif //__CLIMATE_HPP
