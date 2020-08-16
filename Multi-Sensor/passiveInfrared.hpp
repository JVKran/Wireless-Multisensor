/// @file

#ifndef __PASSIVE_INFRARED_HPP
#define __PASSIVE_INFRARED_HPP

#include "manchesterTransmitter.hpp"

/// \brief
/// Detects motion
/// \details
/// Notify ManchesterTransmitter when motion has been detected.
class PassiveInfrared {
	private:
		const uint8_t pin;
		bool possibleStateChange = false;

		ManchesterTransmitter & transmitter;
	public:
		PassiveInfrared(const uint8_t pin, ManchesterTransmitter & transmitter);
		void begin();

		void sensedMotion();
		void operator()();
};

#endif //__PASSIVE_INFRARED_HPP