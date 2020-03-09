#ifndef __POWER_SAVING_HPP
#define __POWER_SAVING_HPP

#include <Arduino.h>
#include <avr/sleep.h>        	// Sleep Modes
#include <avr/power.h>        	// Power management
#include <avr/delay.h>        	// Delay for < 8MHz
#include <avr/io.h>				// Required headers for AVR defines
#include <avr/interrupt.h>		// Required for ISR function
#include <avr/wdt.h> 			// Needed to enable/disable watch dog timer

void sleep();
void set_adc(const bool state);
void setup_watchdog(int timerPrescaler);

ISR(WDT_vect);					// Just used for waking up

#endif //__POWER_SAVING_HPP