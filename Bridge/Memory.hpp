#ifndef __MEMORY_HPP
#define __MEMORY_HPP

#include <Arduino.h>
#include <EEPROM.h>

class Memory {
	public:
		void begin(const int size = 255);

		void storeText(const uint8_t address, const String text);

		String readText(const uint8_t address);
};

#endif //__MEMORY_HPP