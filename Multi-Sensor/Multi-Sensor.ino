/* Power Consumption
The ATtiny has the WDT enabled which uses 4uA. Furthermore, the AM312 is 
continuously drawing 13uA and the BME280 draws 0.25uA when in sleep.

This sums up to 17.25uA which corresponds to the readings of my multimeter.
With a CR2032 this results in ((230mAh / 0.01727mA) / 24 =) 563 days of
worry-free wireless use.

However, that is without taking the STX882 into account, which isn't fair...
During transmission it draws 34mA... In sleep it's only 0.1uA; no problem there.
The transmission of 1 bit takes (1/1200 =) 833uS. 11 elements * 8 bits = 88 bits
to transmit. That results in (833 * 88 =) 73 milliseconds of transmission.

With a 5 minute period, that results in the following calculation;
300s @ 17.25uA + 0.073s @ 34000uA = 5175uAs + 2482uAs = 7657uAs = 0.0021mAh.
0.0021mA per 5 minutes = 0.025mA per hour. (230 / 0.025) / 24 = 383 days.

That's pretty impressive!

Futhermore, it's very important to set the clock of the ATtiny at 8MHz internal
and enable the Brownout Detection and set it at the corresponding voltage. Don't
forget to burn the bootloader with these changed parameters!

As another flash saving method, millis() can be disabled from te same dropdown
menu.
*/

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

const uint8_t sda 			= PIN_B0;
const uint8_t scl 			= PIN_B2;
const uint8_t transmitPin 	= PIN_B4;
const uint8_t pirPin 		= PCINT1;
const uint8_t multiSensorId = 1;

ManchesterTransmitter transmitter = ManchesterTransmitter(multiSensorId, transmitPin, MAN_1200);
PassiveInfrared pirSensor = PassiveInfrared(pirPin, transmitter);
ForcedClimate bme = ForcedClimate(TinyWireM, 0x76, false);
Climate climateSensor = Climate(bme, transmitter, CLM_5MIN);
PowerManagement power = PowerManagement(transmitter);

ISR(PCINT0_vect){
	pirSensor.sensedMotion();
}

void setup() {
	TinyWireM.begin();
	bme.begin();
	climateSensor.begin();
	power.begin();
}

void loop() {
	pirSensor();		// Handle the possible occurence of an interrupt.
	climateSensor();	
	power();

	transmitter();

	power.sleep(PM_8SEC);		// Sleep for 8 seconds or shorter when an interrupt occurs.
}