/// @file

#include "Reed.hpp"

/// \brief
/// Construct Instance
/// \details
/// Construct a Reed instance.
/// \param pin The pin to which the switch is wired.
/// \param transmitter The transmitter to notify when values have changed.
Reed::Reed(const uint8_t pin, ManchesterTransmitter & transmitter):
	pin(pin),
	transmitter(transmitter)
{}

/// \brief
/// Begin Instance
/// \details
/// Begin PassiveInfrared by attaching interrupt to pin.
void Reed::begin(){
	cli();									// Disable interrupts during startup.
	PCMSK |= (1 << pin);					// Enable the interrupthandler (ISR) for the desired pin.
	GIMSK |= (1 << PCIE);             		// Enable PCINT interrupt in the general interrupt mask
  	sei();									// Enable the interrupts after startup.

  	transmitter.updateReed(state);
}

/// \brief
/// Possible Interrupt Occured
/// \details
/// Further verification with digitalRead() necessary.
void Reed::sensedChange(){
	possibleStateChange = true;
}

/// \brief
/// Update State
/// \details
/// If an interrupt could have occured, measure the state of the pin.
/// When high, notify the transmitter that motion has been detected.
void Reed::operator()(){
	if(possibleStateChange){
		delay(20);		// Wait for voltage to settle.
		bool state = digitalRead(pin);
		transmitter.updateReed(state);
		possibleStateChange = false;
	}
}