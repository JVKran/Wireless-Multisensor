/* Power Consumption
The ATtiny has the WDT enabled which uses 4uA. Furthermore, the AM312 is 
continuously drawing 13uA, the BME280 draws 0.25uA when in sleep and the BH1750 
draws 6uA in sleep. Furthermore, there are a couple of capacitors resulting 
in a leakage current.

This sums up to 50uA which corresponds to the readings of my multimeter.
With a CR2032 this results in ((230mAh / 0.050mA) / 24 =) 1250 days of
worry-free wireless use.

However, that is without taking the STX882 into account, which isn't fair...
During transmission it draws 34mA... In sleep it's only 0.1uA; no problem there.
The transmission of 1 bit takes (1/2400 =) 416uS. 11 elements * 8 bits = 88 bits
to transmit. That results in (416 * 88 =) 36 milliseconds of transmission.

A 15 minute period results in the following calculation;
900s @ 50uA + 0.036s @ 34000uA = 45000uAs + 1224uAs = 30924uA / 900 = 52uA on average.
This results in an expected lifetime of (1500mAh / 0.052 =) 28800 hours (1200 days).  

That's pretty impressive!

Futhermore, it's very important to set the clock of the ATtiny at 1MHz internal
and enable the Brownout Detection and set it at 1.7V for the ATtiny85V. Don't
forget to burn the bootloader with these changed parameters!

As another flash saving method, millis() can be disabled from te same dropdown
menu; it's not used anyway.
*/

#include <forcedClimate.h>		// Small and efficient BME280 Library
#include <BH1750.h>				// Small and efficient BH1750 library

#include "powerSaving.hpp"
#include "passiveInfrared.hpp"
#include "Climate.hpp"
#include "Light.hpp"
#include "Reed.hpp"
#include "manchesterTransmitter.hpp"

constexpr uint8_t sda 			= PIN_B0;
constexpr uint8_t scl 			= PIN_B2;
constexpr uint8_t transmitPin 	= PIN_B4;
constexpr uint8_t reedPin 		= PCINT3;
constexpr uint8_t pirPin 		= PCINT1;
constexpr uint8_t multiSensorId = 1;

ManchesterTransmitter transmitter = ManchesterTransmitter(multiSensorId);
PassiveInfrared pir = PassiveInfrared(pirPin, transmitter);
Reed reed = Reed(reedPin, transmitter);
PowerManagement power = PowerManagement(transmitter, PM_15MIN);

ForcedClimate bme = ForcedClimate(TinyWireM, 0x76, false);
Climate climate = Climate(bme, transmitter, CLM_15MIN);

BH1750 lightSensor = BH1750();
Light light = Light(lightSensor, transmitter, LHT_15MIN);

ISR(PCINT0_vect){
	cli();						// Disable interrupts to prevent bouncing.
	pir.sensedMotion();
	reed.sensedChange();
}

void setup() {
	transmitter.begin(transmitPin, MAN_2400);

	pir.begin();
	reed.begin();
	light.begin();
	climate.begin();
	power.begin();
}

void loop() {
	climate();					// Measure light temperature, humidity and pressure when it's time to do so.
	light();					// Measure light intensity when it's time to do so.
	power();					// Update voltage when it's time to do so.

	reed();						// so the voltage can settle resulting in an accurate digital reading.
	pir();						// Handle the possible occurence of an interrupt. Fist climate, light and power are taken care of.

	transmitter();				// Transmit data if data has changed.
	sei();						// Enable interrupts again.
	power.sleep(PM_8SEC);		// Sleep for 8 seconds or shorter when an interrupt occurs.
}
