/// @file

#ifndef __POWER_SAVING_HPP
#define __POWER_SAVING_HPP

#include <Arduino.h>
#include <avr/sleep.h>        	// Sleep Modes
#include <avr/power.h>        	// Power management
#include <avr/delay.h>			// Delay for < 8MHz
#include <avr/io.h>				// Required headers for AVR defines
#include <avr/interrupt.h>		// Required for ISR function
#include <avr/wdt.h> 			// Needed to enable/disable watch dog timer
#include "manchesterTransmitter.hpp"

#define PM_8SEC 9
#define PM_15MIN 900000
#define PM_30MIN 1800000

/// \brief
/// PowerManagement
/// \details
/// Handle updating the Battery Voltage and putting the sensormodule to sleep
/// for saving precious power.
class PowerManagement {
	private:
		ManchesterTransmitter & transmitter;

		const uint16_t updateCycles;
		uint16_t lastUpdateCycles = 0;

		void setAdc(const bool state);
		void setupWatchdog(int timerPrescaler);
	public:
		PowerManagement(ManchesterTransmitter & transmitter, const uint32_t updatePeriod);
		void begin();

		void sleep(int timerPrescaler = 0);
		uint16_t readVoltage();

		void operator()();
};


ISR(WDT_vect);					// Just used for waking up

#endif //__POWER_SAVING_HPP