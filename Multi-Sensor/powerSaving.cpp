/// @file

#include "powerSaving.hpp"

/// \brief
/// Cnstruct Instance
/// \details
/// Construct a PowerManagement instance.
/// \param transmitter The transmitter to notify when values have changed.
/// \param updatePeriod The period in milliseconds to keep between measurements.
PowerManagement::PowerManagement(ManchesterTransmitter & transmitter, const uint32_t updatePeriod):
	transmitter(transmitter),
	updateCycles(updatePeriod / 8000)			// Wakes up at least every 8 seconds
{}

/// \brief
/// Begin Instance
/// \details
/// Begin PowerManagement by initializing value for voltage.
void PowerManagement::begin(){
	transmitter.updateVoltage(readVoltage());
}

/// \brief
/// Go to sleep
/// \details
/// Go to sleep by disabling as much as possible.
/// \param timerPrescaler Defines how long to sleep. Can be any of the following.
/// 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
/// 6=1sec, 7=2sec, 8=4sec, 9=8sec
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

/// \brief
/// Set ADC
/// \details
/// Enable or disable the ADC to save 230uA.
/// \param state The state to put the ADC in.
void PowerManagement::setAdc(const bool state){
	if(state){
		ADCSRA |= (1<<ADEN);
	} else {
		ADCSRA &= ~(1<<ADEN);
	}
}

/// \brief
/// Enable Watchdog
/// \details
/// Setup the watchdog for a timer.
/// \param timerPrescaler Defines how long to sleep. Can be any of the following.
/// 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
/// 6=1sec, 7=2sec, 8=4sec, 9=8sec
void PowerManagement::setupWatchdog(int timerPrescaler){
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

/// \brief
/// Measure Battery Voltage
/// \details
/// Measure the Vcc voltage.
/// \return The voltage in millivolts.
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

	ADCSRA |= _BV(ADSC); 				// Start conversion
	while (bit_is_set(ADCSRA,ADSC)); 	// measuring

	low  = ADCL; 						// must read ADCL first - it then locks ADCH 
	high = ADCH; 						// unlocks both

	result = (high<<8) | low;

	result = 1125300L / result; 		// Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	setAdc(false); 						// Saves 230uA
	return result; 						// Vcc in millivolts 
}

/// \brief
/// Update Voltage
/// \details
/// Update value by taking a measurement at the configured interval.
void PowerManagement::operator()(){
	if(++lastUpdateCycles >= updateCycles){
		lastUpdateCycles = 0;
		transmitter.updateVoltage(readVoltage());
	}
}

/// \brief
/// ISR
/// \details
/// ISR used for waking up by watchdog timer.
ISR(WDT_vect){}
