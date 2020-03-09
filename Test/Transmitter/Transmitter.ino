#include <Manchester.h>
#include <avr/sleep.h>        	// Sleep Modes
#include <avr/power.h>        	// Power management
#include <avr/delay.h>        	// Delay for < 8MHz
#include <avr/io.h>				// Required headers for AVR defines
#include <avr/interrupt.h>		// Required for ISR function
#include "powerSaving.hpp"
#include "passiveInfrared.hpp"
#include "forcedClimate.hpp"
#include <Wire.h>

PassiveInfrared pirSensor = PassiveInfrared(PCINT1);
//ForcedClimate climateSensor = ForcedClimate(Wire);

ISR(PCINT0_vect){
	pirSensor.sensedMotion();
}

void setup() {
  	man.setupTransmit(PIN_B0, MAN_1200);
}

void loop() {
	pirSensor();

	setup_watchdog(9);
	sleep();
}
