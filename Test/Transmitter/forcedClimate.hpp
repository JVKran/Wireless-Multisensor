#ifndef __FORCED_CLIMATE_HPP
#define __FORCED_CLIMATE_HPP

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

class ForcedClimate {
	private:
		TwoWire & bus;
		uint8_t address;

		int16_t T[4], P[10], H[7];
		int32_t BME280t_fine;
	public:
		ForcedClimate(TwoWire & bus, const uint8_t address = 0x77);

		float getTemperature();
		float getPressure();
		float getHumidity();

};


#endif //__FORCED_CLIMATE_HPP