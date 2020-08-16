/// @file

#ifndef __CLIMATE_HPP
#define __CLIMATE_HPP

#include <Arduino.h>
#include <forcedClimate.h>
#include "manchesterTransmitter.hpp"

#define CLM_5MIN  300000
#define CLM_10MIN 600000
#define CLM_15MIN 900000

/// \brief
/// Temperature, Humidity and Pressure measurements.
/// \details
/// Responsible for letting the BME280 take measurements at the interval it's supposed to make measurements at.
/// After that's been done, the ManchesterTransmitter is notified about a change.
class Climate {
	private:
		ForcedClimate & climateSensor;
		ManchesterTransmitter & transmitter;

		uint16_t lastUpdateCycles = 0;
		const uint16_t updateCycles;
	public:
		Climate(ForcedClimate & climateSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod = CLM_5MIN);

		void begin();
		void operator()();
};

#endif //__CLIMATE_HPP
