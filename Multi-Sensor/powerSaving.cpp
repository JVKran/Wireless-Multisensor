#include "powerSaving.hpp"

PowerManagement::PowerManagement(ManchesterTransmitter & transmitter, const uint32_t voltageUpdatePeriod):
	transmitter(transmitter),
	voltageUpdateCycles(voltageUpdatePeriod / 8000)			// Wakes up at least every 8 seconds
{}

void PowerManagement::begin(){
	transmitter.updateVoltage(readVoltage());
}

void PowerManagement::sleep(int timerPrescaler){
	if(timerPrescaler){
		setupWatchdog(timerPrescaler);
	}
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_cpu();

	sleep_disable();   
	wdt_disable();
}

void PowerManagement::setAdc(const bool state){
	if(state){
		ADCSRA |= (1<<ADEN);
	} else {
		ADCSRA &= ~(1<<ADEN);
	}
}

//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
void PowerManagement::setupWatchdog(int timerPrescaler) {
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
	setAdc(true);
	// Read 1.1V reference against AVcc
	// set the reference to Vcc and the measurement to the internal 1.1V reference
	#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
	#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
	#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#endif 

	delay(2); 							// Wait for Vref to settle
	ADCSRA |= _BV(ADSC); 				// Start conversion
	while (bit_is_set(ADCSRA,ADSC)); 	// measuring

	uint8_t low  = ADCL; 				// must read ADCL first - it then locks ADCH 
	uint8_t high = ADCH; 				// unlocks both

	long result = (high<<8) | low;

	result = 1125300L / result; 		// Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	setAdc(false); 						// Saves 230uA
	return result; 						// Vcc in millivolts 
}

void PowerManagement::operator()(){
	if(lastUpdateCycles++ > voltageUpdateCycles){
		lastUpdateCycles = 0;
		transmitter.updateVoltage(readVoltage());
	}
}

ISR(WDT_vect){}
