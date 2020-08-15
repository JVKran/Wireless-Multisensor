/// @file

#include "sensorBridge.hpp"

/// \brief
/// Contruct Instance
/// \details
/// Construct a SensorBridge Instance.
SensorBridge::SensorBridge():
	receiver(man)
{}

/// \brief
/// Begin Instance
/// \details
/// Start listening for incoming packets.
/// \param receivePin The pin to which the 433MHz receiver is connected.
/// \param baudrate The baudrate at which the packets are sent.
void SensorBridge::begin(const uint8_t receivePin, const uint8_t baudrate){
	receiver.setupReceive(receivePin, baudrate);
	receiver.beginReceiveArray(BUFFER_SIZE, buffer);
}

/// \brief
/// Notify Listeners
/// \param sensorId The id of the sensor that sent the received message.
/// \param motion Wether or not there was motion detected.
/// \param reedState The state of the switch.
/// \param temperature The measured temperature.
/// \param humidity The measured humidity.
/// \param pressure The measured pressure.
/// \param lightIntensity The measured light intensity.
/// \param voltage The voltage of the battery.
void SensorBridge::notifyListeners(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity){
	for(uint8_t i = 0; i < amountOfListeners; i++){
		listeners[i]->dataReceived(sensorId, motion, reedState, temperature, humidity, pressure, voltage, lightIntensity);
	}
}

/// \brief
/// Add Listener
/// \details
/// Add listener to the list of listeners to notify on reception of message.
/// \param listenerToAdd The listener to add.
void SensorBridge::addListener(SensorListener & listenerToAdd){
	listeners[amountOfListeners++] = &listenerToAdd;
}

/// \brief
/// Listen for messages
/// \details
/// When complete, the message is decoded and all listeners are notified.
void SensorBridge::operator()(){
	if(receiver.receiveComplete()){
		uint8_t sensorId = buffer[1];
		bool motion = buffer[2];
		bool reedState = buffer[3];
		int16_t temperature = buffer[4] + (buffer[5] << 8);
		int16_t humidity = buffer[6] + (buffer[7] << 8);
		int32_t pressure = buffer[8] + ((buffer[9] << 8) + ((buffer[10] << 16) + (buffer[11] << 24)));
		uint16_t voltage = buffer[12] + (buffer[13] << 8);
		uint16_t lightIntensity = buffer[14] + (buffer[15] << 8);
		notifyListeners(sensorId, motion, reedState, temperature, humidity, pressure, voltage, lightIntensity);
		receiver.beginReceiveArray(BUFFER_SIZE, buffer);
	}
}