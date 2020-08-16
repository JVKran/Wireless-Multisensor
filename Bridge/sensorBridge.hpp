/// @file

#ifndef __SENSOR_BRIDGE_HPP
#define __SENSOR_BRIDGE_HPP

#include <Manchester.h>
#define BUFFER_SIZE 16

/// \brief 
/// Listen for messages from SensorBridge
/// \details
/// Inherit from this class and override the given abstract function to be notified when 
/// data is received. This can be done like in the example underneath.
/// 
/// ~~~~~~~~~~~~~~~{.hpp}
/// 
/// class alarmSystem : public sensorListener {
///        private:
///            void fire(){
///                // Do something; blink led, buzz buzzer, etc.
///            }
///        public:
///            virtual void dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity) override {
///                if(motion){
///                    fire();
///                }
///            }
/// };
/// 
/// SensorBridge sensorBridge = SensorBridge();
/// alarmSystem alarm = alarmSystem();
/// 
/// void setup() {
///     sensorBridge.addListener(alarm);
/// }
///
/// void loop() {
///     sensorBridge();
/// }
///
/// ~~~~~~~~~~~~~~~
class SensorListener {
	public:
		/// \brief
		/// Receive Data
		/// \details
		/// Read the received data..
		/// \param sensorId The id of the sensor that sent the received message.
		/// \param motion Wether or not there was motion detected.
		/// \param reedState The state of the switch.
		/// \param temperature The measured temperature.
		/// \param humidity The measured humidity.
		/// \param pressure The measured pressure.
		/// \param lightIntensity The measured light intensity.
		/// \param voltage The voltage of the battery.
		virtual void dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity) = 0;
};

/// \brief 
/// Listen for messages from Wireless-Multisensors.
/// \details
/// This class forms a bridge between the Wireless Sensor module and the MQTT client.
/// It receives, processes and forwards the messages received from the sensor module.
/// 
/// ~~~~~~~~~~~~~~~{.hpp}
/// 
/// class alarmSystem : public sensorListener {
///        private:
///            void fire(){
///                // Do something; blink led, buzz buzzer, etc.
///            }
///        public:
///            virtual void dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity) override {
///                if(motion){
///                    fire();
///                }
///            }
/// };
/// 
/// SensorBridge sensorBridge = SensorBridge();
/// alarmSystem alarm = alarmSystem();
/// 
/// void setup() {
///     sensorBridge.addListener(alarm);
/// }
///
/// void loop() {
///     sensorBridge();
/// }
///
/// ~~~~~~~~~~~~~~~
class SensorBridge {
	private:
		Manchester & receiver;
		uint8_t buffer[BUFFER_SIZE];

		SensorListener * listeners[10] = {};
        uint8_t amountOfListeners = 0;

        void notifyListeners(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity);
	public:
		SensorBridge();
		void begin(const uint8_t receivePin, const uint8_t baudrate);

		void addListener(SensorListener & listenerToAdd);
		void operator()();
};

#endif //__SENSOR_BRIDGE_HPP