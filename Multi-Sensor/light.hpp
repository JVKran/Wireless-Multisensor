#ifndef __LIGHT_HPP
#define __LIGHT_HPP

#define LHT_5MIN 300000

#include <BH1750.h>
#include "ManchesterTransmitter.hpp"

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