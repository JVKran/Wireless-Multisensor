//#pragma GCC optimize ("-Os")

#include <Manchester.h>			// 433MHz Manchester encoding
#include <avr/sleep.h>        	// Sleep Modes
#include <avr/power.h>        	// Power management
#include <avr/delay.h>        	// Delay for < 8MHz
#include <avr/io.h>				// Required headers for AVR defines
#include <avr/interrupt.h>		// Required for ISR function
#include <forcedClimate.h>		// Small and efficient BME280 Library

#include "powerSaving.hpp"
#include "passiveInfrared.hpp"
#include "Climate.hpp"
#include "manchesterTransmitter.hpp"

const uint8_t transmitPin = PIN_B4;
const uint8_t pirPin = PCINT1;

ManchesterTransmitter transmitter = ManchesterTransmitter(1, transmitPin, MAN_1200);
PassiveInfrared pirSensor = PassiveInfrared(pirPin, transmitter);
// ForcedClimate bme = ForcedClimate(Wire, 0x76);
// Climate climateSensor = Climate(bme, transmitter);

ISR(PCINT0_vect){
	pirSensor.sensedMotion();
}

void setup() {
	
}

void loop() {
	pirSensor();
	// climateSensor();
	transmitter();

	setup_watchdog(9);						// Trigger after 8 seconds.
	sleep();
}