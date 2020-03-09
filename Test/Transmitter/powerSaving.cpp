#include "powerSaving.hpp"

void sleep(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	set_adc(false);            				// Turn off ADC (saves ~230uA).
	power_all_disable();  					// Power off ADC, Timer 0 and 1 and the serial interface.
	sleep_enable();
	sleep_cpu();
	sleep_disable();   
	power_all_enable();    					// Power everything back on.
	wdt_disable();							// Prevent wakeup during being awake.
}

void set_adc(const bool state){
	if(state){
		ADCSRA |= (1<<ADEN);
	} else {
		ADCSRA &= ~(1<<ADEN);
	}
}

//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
void setup_watchdog(int timerPrescaler) {
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

ISR(WDT_vect){}