/* Power Consumption
The ATtiny has the WDT enabled which uses 4uA. Furthermore, the AM312 is 
continuously drawing 13uA and the BME280 draws 0.25uA when in sleep. Furthermore,
there are a couple of capacitors resulting in a leakage current.

This sums up to 33uA which corresponds to the readings of my multimeter.
With a CR2032 this results in ((230mAh / 0.033mA) / 24 =) 290 days of
worry-free wireless use.

However, that is without taking the STX882 into account, which isn't fair...
During transmission it draws 34mA... In sleep it's only 0.1uA; no problem there.
The transmission of 1 bit takes (1/2400 =) 416uS. 11 elements * 8 bits = 88 bits
to transmit. That results in (416 * 88 =) 36 milliseconds of transmission.

A 15 minute period results in the following calculation;
900s @ 33uA + 0.036s @ 34000uA = 29700uAs + 1224uAs = 30924uA / 900 = 35uA on average.
This results in an expected lifetime of (230mAh / 0.035 =) 6571 hours (270 days).  

That's pretty impressive!

Futhermore, it's very important to set the clock of the ATtiny at 1MHz internal
and enable the Brownout Detection and set it at 1.7V for the ATtiny85V. Don't
forget to burn the bootloader with these changed parameters!

As another flash saving method, millis() can be disabled from te same dropdown
menu; it's not used anyway.
*/

#include <forcedClimate.h>		// Small and efficient BME280 Library
#include "powerSaving.hpp"
#include "passiveInfrared.hpp"
#include "Climate.hpp"
#include "manchesterTransmitter.hpp"

const uint8_t sda 			= PIN_B0;
const uint8_t scl 			= PIN_B2;
const uint8_t transmitPin 	= PIN_B4;
const uint8_t enablePin 	= PIN_B3;
const uint8_t pirPin 		= PCINT1;
const uint8_t multiSensorId = 1;

ManchesterTransmitter transmitter = ManchesterTransmitter(multiSensorId, enablePin);
PassiveInfrared pir = PassiveInfrared(pirPin, transmitter);
ForcedClimate bme = ForcedClimate(TinyWireM, 0x76, false);
Climate climate = Climate(bme, transmitter, CLM_15MIN);
PowerManagement power = PowerManagement(transmitter, PM_15MIN);

ISR(PCINT0_vect){
	pir.sensedMotion();
}

void setup() {
	TinyWireM.begin();
	transmitter.begin(transmitPin, MAN_2400);
	pir.begin();
	climate.begin();
	power.begin();
}

void loop() {
	pir();						// Handle the possible occurence of an interrupt.
	climate();	
	power();

	transmitter();
	power.sleep(PM_8SEC);		// Sleep for 8 seconds or shorter when an interrupt occurs.
}
