#ifndef __PASSIVE_INFRARED_HPP
#define __PASSIVE_INFRARED_HPP

#include <Arduino.h>
#include <Manchester.h>
#include <avr/interrupt.h>
#include "manchesterTransmitter.hpp"

class PassiveInfrared {
	private:
		const uint8_t pin;
		bool possibleStateChange = false;

		ManchesterTransmitter & transmitter;
	public:
		PassiveInfrared(const uint8_t pin, ManchesterTransmitter & transmitter);
		void begin();

		void sensedMotion();
		void operator()();
};

#endif //__PASSIVE_INFRARED_HPP