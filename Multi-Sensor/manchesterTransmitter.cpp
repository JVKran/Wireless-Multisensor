/// @file

#include "manchesterTransmitter.hpp"

/// \brief
/// Cnstruct Instance
/// \details
/// Construct a ManchesterTransmitter instance.
/// \param id The id of the Sensor Module.
ManchesterTransmitter::ManchesterTransmitter(const uint8_t id):
	id(id)
{}

/// \brief
/// Begin Instance
/// \details
/// Setup the transmitter.
/// \param transmitPin The pin the transmitter is connected to.
/// \param baudrate The baudrate at which to transmit packets.
void ManchesterTransmitter::begin(const uint8_t transmitPin, const uint8_t baudrate){
	man.setupTransmit(transmitPin, baudrate);
}

/// \brief
/// Transmit data when changes have occured.
/// \details
/// When at least one value changed since the last transmission, all values are transmitted.
void ManchesterTransmitter::operator()(){
	if(dataChanged){
		transmitData();
		dataChanged = false;
		lastMotion = false;
	}
}

/// \brief
/// Transmit Data
void ManchesterTransmitter::transmitData(){
	uint8_t bufferSize = 16;
	uint8_t data[bufferSize] = {bufferSize, id, lastMotion, lastReedState,
								lastTemperature & 0x00FF, lastTemperature >> 8, 
								lastHumidity & 0x00FF, lastHumidity >> 8, 
								lastPressure & 0x00FF, lastPressure >> 8, lastPressure >> 16, lastPressure >> 24, 
								lastVoltage & 0x00FF, lastVoltage >> 8,
								lastIntensity & 0x00FF, lastIntensity >> 8
								};

	man.transmitArray(bufferSize, data);
}

/// \brief
/// Notify Motion
void ManchesterTransmitter::updateMotion(){
	lastMotion = true;
	dataChanged = true;
}

/// \brief
/// Update Voltage
/// \param voltage The measured voltage.
void ManchesterTransmitter::updateVoltage(const uint16_t voltage){
	lastVoltage = voltage;
	dataChanged = true;
}

/// \brief
/// Update Climate
/// \param temperature The measured temperature.
/// \param humidity The measured humidity.
/// \param pressure The measured pressure.
void ManchesterTransmitter::updateClimate(const int16_t temperature, const int16_t humidity, const uint32_t pressure){
	lastTemperature = temperature;
	lastHumidity = humidity;
	lastPressure = pressure - 6400;			// Minus 6400 for 1MHz mode for some unkown reason; bug? After substraction everything is how it should be.
	dataChanged = true;
}

/// \brief
/// Update Illuminance
/// \param intensity The measured light intensity.
void ManchesterTransmitter::updateLightIntensity(const uint16_t intensity){
	lastIntensity = intensity;
	dataChanged = true;
}

/// \brief
/// Update Reed
/// \param state The new state of the reedswitch.
void ManchesterTransmitter::updateReed(const bool state){
	if(lastReedState != state){
		lastReedState = state;
		dataChanged = true;
	}
}
