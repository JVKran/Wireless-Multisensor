#ifndef __MEMORY_HPP
#define __MEMORY_HPP

#include <Arduino.h>
#include <EEPROM.h>

/// \brief 
/// Store String in EEPROM
/// \details
/// Handles everything that should be handled for the use of MQTT (Message Queue Telemetry Transport).
/// Has functions for keeping the connection active, connecting to a network, subscribing to topics,
/// receiving messages and publishing messages. Literally takes care of everything.
/// 
/// ~~~~~~~~~~~~~~~{.cpp}
/// #include "Memory.hpp"
///
/// Memory memory = Memory();
///
/// void setup(){
///		memory.begin(512);
///		Serial.begin(9600);
/// 	
///		Serial.println(memory.readText(5));		// This will print "Hello, this is a test!" the second time this sketch is ran.
///		memory.storeText(5, "Hello, this is a test!");
///		
/// }
///
/// void loop(){}
///
/// ~~~~~~~~~~~~~~~
class Memory {
	private:
		int maxAddress = 0;
	public:
		void begin(const int size = 255);

		void storeText(const uint8_t address, const String text);

		String readText(const uint8_t address);
};

#endif //__MEMORY_HPP