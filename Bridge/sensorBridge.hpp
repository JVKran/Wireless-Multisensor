#ifndef __SENSOR_BRIDGE_HPP
#define __SENSOR_BRIDGE_HPP

#include <Manchester.h>
#define BUFFER_SIZE 16

class SensorListener {
	public:
		virtual void dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity) = 0;
};

class SensorBridge {
	private:
		Manchester & receiver;
		uint8_t buffer[BUFFER_SIZE];

		SensorListener * listeners[10] = {};
        uint8_t amountOfListeners = 0;

        void notifyListeners(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity){
        	for(uint8_t i = 0; i < amountOfListeners; i++){
        		listeners[i]->dataReceived(sensorId, motion, reedState, temperature, humidity, pressure, voltage, lightIntensity);
        	}
        }
	public:
		SensorBridge(const uint8_t receivePin, const uint8_t baudrate);

		void addListener(SensorListener & listenerToAdd){
			listeners[amountOfListeners++] = &listenerToAdd;
		}

		void operator()();
};

#endif //__SENSOR_BRIDGE_HPP