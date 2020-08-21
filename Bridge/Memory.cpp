#include "Memory.hpp"

/// \brief
/// Begin Instance
/// \details
/// Begin the Memory instance by setting the desired size.
void Memory::begin(const int size){
	EEPROM.begin(size);
	maxAddress = size;
}

/// \brief
/// Store Text in EEPROM
/// \details
/// Store the passed text at the desired location. This is done
/// by writing the length of the string at address and storing
/// the other characters at the next addresses.
/// \param address The address to store the String at.
/// \param text The String to store.
void Memory::storeText(const uint8_t address, const String text) {
	uint8_t length = text.length();
	EEPROM.write(address, length);
	for(uint8_t i = address; i < address + length && i < maxAddress; i++){
		EEPROM.write(i + 1, text[i - address]);
	}
	EEPROM.commit();
}

/// \brief
/// Read Text from EEPROM
/// \details
/// Read the text at the given address. This is done by first determining
/// what the length of the String should be; that's the digit at address.
/// Furthermore, the characters at the following addresses are concatenated to
/// the string that's returned afterwards.
/// \param address The address to read the String from.
String Memory::readText(const uint8_t address) {
	uint8_t length = EEPROM.read(address);
	if(length == 255 || length == 0){
		return "";
	}
	String text = "";
	for(uint8_t i = address; i < address + length && i < maxAddress; i++){
		text.concat((char)EEPROM.read(i + 1));
	}
	return text;
}