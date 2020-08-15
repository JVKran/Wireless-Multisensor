/// @file

#include "passiveInfrared.hpp"

/// \brief
/// Construct Instance
/// \details
/// Construct a PassiveInfrared instance.
/// \param pin The pin to which the sensor is wired.
/// \param transmitter The transmitter to notify when values have changed.
PassiveInfrared::PassiveInfrared(const uint8_t pin, ManchesterTransmitter & transmitter):
	pin(pin),
	transmitter(transmitter)
{}

/// \brief
/// Begin Instance
/// \details
/// Begin PassiveInfrared by attaching interrupt to pin.
void PassiveInfrared::begin(){
	cli();									// Disable interrupts during startup.
	PCMSK |= (1 << pin);					// Enable the interrupthandler (ISR) for the desired pin.
	GIMSK |= (1 << PCIE);             		// Enable PCINT interrupt in the general interrupt mask
  	sei(); 									// Enable the interrupts after startup.
}

/// \brief
/// Possible Interrupt Occured
/// \details
/// Further verification with digitalRead() necessary.
void PassiveInfrared::sensedMotion(){
	possibleStateChange = true;
}

/// \brief
/// Update
/// \details
/// If an interrupt could have occured, measure the state of the pin.
/// When high, notify the transmitter that motion has been detected.
void PassiveInfrared::operator()(){
	if(possibleStateChange && digitalRead(pin)){
		transmitter.updateMotion();
		possibleStateChange = false;
	}
}