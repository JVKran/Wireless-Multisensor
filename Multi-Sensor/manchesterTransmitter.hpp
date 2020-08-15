/// @file

#ifndef __MANCHESTER_TRANSMITTER_HPP
#define __MANCHESTER_TRANSMITTER_HPP

#include <Arduino.h>
#include <Manchester.h>

/// \brief
/// ManchesterTransmitter
/// \details
/// Transmit all data according to the Manchester Standard.
class ManchesterTransmitter {
	private:
		const uint8_t id;
		bool dataChanged = true;

		int16_t lastTemperature = 0;
		int16_t lastHumidity = 0;
		uint32_t lastPressure = 0;
		uint8_t lastMotion = false;
		uint16_t lastVoltage = 0;
		uint16_t lastIntensity = 0;
		uint8_t lastReedState = false;

		void transmitData();
	public:
		ManchesterTransmitter(const uint8_t id);
		void begin(const uint8_t transmitPin = PIN_B0, const uint8_t baudrate = MAN_1200);

		void operator()();

		void updateMotion();
		void updateVoltage(const uint16_t voltage);
		void updateClimate(const int16_t temperature, const int16_t humidity, const uint32_t pressure);
		void updateLightIntensity(const uint16_t intensity);
		void updateReed(const bool state);

};

#endif //__MANCHESTER_TRANSMITTER_HPP