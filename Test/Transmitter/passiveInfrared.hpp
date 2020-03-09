#ifndef __PASSIVE_INFRARED_HPP
#define __PASSIVE_INFRARED_HPP

#include <Manchester.h>
#include <avr/sleep.h>        	// Sleep Modes
#include <avr/power.h>        	// Power management
#include <avr/delay.h>        	// Delay for < 8MHz
#include <avr/io.h>				// Required headers for AVR defines
#include <avr/interrupt.h>		// Required for ISR function

class PassiveInfrared {
	private:
		const uint8_t pin;
		bool sendMotion = false;

	public:
		PassiveInfrared(const uint8_t pin);

		void sensedMotion();
		void operator()();
};

#endif //__PASSIVE_INFRARED_HPP