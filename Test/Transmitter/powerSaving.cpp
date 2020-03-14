#include "powerSaving.hpp"

PowerManagement::PowerManagement(ManchesterTransmitter & transmitter, const unsigned long updatePeriod):
	transmitter(transmitter),
	updatePeriod(updatePeriod)
{
	// transmitter.updateVoltage(readVoltage());
}

void PowerManagement::sleep(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	set_adc(false);            				// Turn off ADC (saves ~230uA).
	power_all_disable();  					// Power off ADC, Timer 0 and 1 and the serial interface.
	sleep_enable();
	sleep_cpu();
	sleep_disable();   
	power_all_enable();    					// Power everything back on.
	wdt_disable();							// Prevent wakeup during being awake.
}

void PowerManagement::set_adc(const bool state){
	if(state){
		ADCSRA |= (1<<ADEN);
	} else {
		ADCSRA &= ~(1<<ADEN);
	}
}

//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
void PowerManagement::setup_watchdog(int timerPrescaler) {
	if (timerPrescaler > 9 ){
		timerPrescaler = 9;
	}

	byte bb = timerPrescaler & 7; 
	if (timerPrescaler > 7){
		bb |= (1<<5); 						// Set the special 5th bit if necessary
	}

	MCUSR &= ~(1<<WDRF); 					// Clear the watch dog reset
	WDTCR |= (1<<WDCE) | (1<<WDE); 			// Set WD_change enable, set WD enable
	WDTCR = bb; 							// Set new watchdog timeout value
	WDTCR |= _BV(WDIE); 					// Set the interrupt enable, this will keep unit from resetting after each int
}

uint16_t PowerManagement::readVoltage(){
	#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
		ADMUX = _BV(MUX5) | _BV(MUX0);
	#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		ADMUX = _BV(MUX3) | _BV(MUX2);
	#else
		ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#endif

	delay(2); 								// Wait for the refrence voltage (Vref) to settle
	ADCSRA |= _BV(ADSC); 					// Start conversion
	while (bit_is_set(ADCSRA,ADSC)); 		// Measuring

	uint8_t low  = ADCL; 					// Read ADCL first because it locks ADCH  
	uint8_t high = ADCH; 					// Unlocks both

	uint16_t result = (high << 8) | low;

	result = 1125300L / result; 			// Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	return result; 							// Vcc in millivolts
}

void PowerManagement::operator()(){
	if(millis() - lastUpdate > updatePeriod){
		transmitter.updateVoltage(readVoltage());
		lastUpdate = millis();
	}
}

ISR(WDT_vect){}