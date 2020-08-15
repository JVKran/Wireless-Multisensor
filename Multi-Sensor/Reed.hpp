/// @file

#ifndef __REED_HPP
#define __REED_HPP

#include "manchesterTransmitter.hpp"

/// \brief
/// Reed Switch
/// \details
/// Notify when state has changed.
class Reed {
	private:
		const uint8_t pin;
		bool possibleStateChange = false;
		bool state = false;

		ManchesterTransmitter & transmitter;
	public:
		Reed(const uint8_t pin, ManchesterTransmitter & transmitter);
		void begin();

		void sensedChange();
		void operator()();
};

#endif //__REED_HPP