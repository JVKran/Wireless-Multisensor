#include "passiveInfrared.hpp"

PassiveInfrared::PassiveInfrared(const uint8_t pin):
	pin(pin)
{
	cli();									// Disable interrupts during startup.
	PCMSK |= (1 << pin);					// Enable the interrupthandler (ISR) for the desired pin.
	GIMSK |= (1 << PCIE);             		// Enable PCINT interrupt in the general interrupt mask
  	sei(); 									// Enable the interrupts after startup.

  	pinMode(4, OUTPUT);
}

void PassiveInfrared::sensedMotion(){
	sendMotion = true;
}

void PassiveInfrared::operator()(){
	if(sendMotion){
		man.transmit(111);
		sendMotion = false;
	} else {
		digitalWrite(4, HIGH);
		delay(1000);
		digitalWrite(4, LOW);
	}
}