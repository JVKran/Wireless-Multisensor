#ifndef __POWER_SAVING_HPP
#define __POWER_SAVING_HPP

#include <Arduino.h>
#include <avr/sleep.h>        	// Sleep Modes
#include <avr/power.h>        	// Power management
#include <avr/delay.h>        	// Delay for < 8MHz
#include <avr/io.h>				// Required headers for AVR defines
#include <avr/interrupt.h>		// Required for ISR function
#include <avr/wdt.h> 			// Needed to enable/disable watch dog timer
#include "manchesterTransmitter.hpp"

#define PM_8SEC 9

class PowerManagement {
	private:
		ManchesterTransmitter & transmitter;

		uint16_t lastVoltage;
	public:
		PowerManagement(ManchesterTransmitter & transmitter);
		void begin();

		void sleep(int timerPrescaler = 0);
		void set_adc(const bool state);
		void setup_watchdog(int timerPrescaler);

		uint16_t readVoltage();
		void operator()();
};


ISR(WDT_vect);					// Just used for waking up

#endif //__POWER_SAVING_HPP