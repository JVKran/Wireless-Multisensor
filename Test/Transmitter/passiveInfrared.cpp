#include "passiveInfrared.hpp"

PassiveInfrared::PassiveInfrared(const uint8_t pin, ManchesterTransmitter & transmitter):
	pin(pin),
	transmitter(transmitter)
{
	cli();									// Disable interrupts during startup.
	PCMSK |= (1 << pin);					// Enable the interrupthandler (ISR) for the desired pin.
	GIMSK |= (1 << PCIE);             		// Enable PCINT interrupt in the general interrupt mask
  	sei(); 									// Enable the interrupts after startup.
}

void PassiveInfrared::sensedMotion(){
	sendMotion = true;
}

void PassiveInfrared::operator()(){
	if(sendMotion){
		transmitter.updateMotion();
		sendMotion = false;
	}
}