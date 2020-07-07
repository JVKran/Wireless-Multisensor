#ifndef __MANCHESTER_TRANSMITTER_HPP
#define __MANCHESTER_TRANSMITTER_HPP

#include <Arduino.h>
#include <Manchester.h>

class ManchesterTransmitter {
	private:
		const uint8_t id;
		const uint8_t enablePin;
		bool dataChanged = true;

		int16_t lastTemperature = 0;
		int16_t lastHumidity = 0;
		int32_t lastPressure = 0;
		uint8_t lastMotion = false;
		uint16_t lastVoltage = 0;

		void transmitData();
	public:
		ManchesterTransmitter(const uint8_t id, const uint8_t enablePin = PIN_B3);
		void begin(const uint8_t transmitPin = PIN_B0, const uint8_t baudrate = MAN_1200);

		void operator()();

		void updateMotion();
		void updateVoltage(const uint16_t voltage);
		void updateClimate(const int16_t temperature, const int16_t humidity, const int32_t pressure);

};

#endif //__MANCHESTER_TRANSMITTER_HPP