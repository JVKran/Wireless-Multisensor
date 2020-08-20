#include "Memory.hpp"

void Memory::begin(const int size){
	EEPROM.begin(size);
}

void Memory::storeText(const uint8_t address, const String text) {
	uint8_t length = text.length();
	EEPROM.write(address, length);
	for(uint8_t i = address; i < address + length; i++){
		EEPROM.write(i + 1, text[i - address]);
	}
	EEPROM.commit();
}

String Memory::readText(const uint8_t address) {
	uint8_t length = EEPROM.read(address);
	if(length == 255 || length == 0){
		return "";
	}
	String text = "";
	for(uint8_t i = address; i < address + length; i++){
		text.concat((char)EEPROM.read(i + 1));
	}
	return text;
}