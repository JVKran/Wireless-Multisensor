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

#define PM_HOUR 3600000

class PowerManagement {
	private:
		ManchesterTransmitter & transmitter;

		unsigned long updatePeriod;
		unsigned long lastUpdate;
	public:
		PowerManagement(ManchesterTransmitter & transmitter, const unsigned long updatePeriod = PM_HOUR);

		void begin(){
			transmitter.updateVoltage(readVoltage());
		}

		void sleep();
		void set_adc(const bool state);
		void setup_watchdog(int timerPrescaler);

		uint16_t readVoltage();

		void operator()();
};


ISR(WDT_vect);					// Just used for waking up

#endif //__POWER_SAVING_HPP