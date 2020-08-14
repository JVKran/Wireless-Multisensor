#ifndef __REED_HPP
#define __REED_HPP

class Reed {
	private:
		const uint8_t pin;
		bool possibleStateChange = false;
		bool state = false;

		ManchesterTransmitter & transmitter;
	public:
		Reed(const uint8_t pin, ManchesterTransmitter & transmitter):
			pin(pin),
			transmitter(transmitter)
		{}

		void begin(){
			cli();									// Disable interrupts during startup.
			PCMSK |= (1 << pin);					// Enable the interrupthandler (ISR) for the desired pin.
			GIMSK |= (1 << PCIE);             		// Enable PCINT interrupt in the general interrupt mask
		  	sei();									// Enable the interrupts after startup.

		  	transmitter.updateReed(state);
		}

		void sensedChange(){
			possibleStateChange = true;
		}

		void operator()(){
			if(possibleStateChange){
				delay(20);		// Wait for voltage to settle.
				bool state = digitalRead(pin);
				transmitter.updateReed(state);
				possibleStateChange = false;
			}
		}
};

#endif //__REED_HPP