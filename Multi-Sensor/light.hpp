/// @file

#ifndef __LIGHT_HPP
#define __LIGHT_HPP

#define LHT_5MIN  300000
#define LHT_10MIN 600000
#define LHT_15MIN 900000

#include <BH1750.h>
#include "ManchesterTransmitter.hpp"

/// \brief
/// Light Intensity measurements.
/// \details
/// Responsible for letting the Bh1750 take measurements at the interval it's supposed to make measurements at.
/// After that's been done, the ManchesterTransmitter is notified about a change.
class Light {
	private:
		ManchesterTransmitter & transmitter;
		BH1750 & lightSensor;

		uint16_t lastUpdateCycles = 0;
		const uint16_t updateCycles;
	public:
		Light(BH1750 & lightSensor, ManchesterTransmitter & transmitter, const uint32_t updatePeriod = LHT_5MIN);

		void begin();
		void operator()();
};

#endif //__LIGHT_HPP