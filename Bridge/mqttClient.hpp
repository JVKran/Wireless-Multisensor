/// @file

#ifndef __CONNECTIONS_HPP
#define __CONNECTIONS_HPP

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "sensorBridge.hpp"

/// \brief 
/// messageListener
/// \details
/// Inherit from this class and override the given abstract function to be notified when a 
/// message is received. This can be done like in the example underneath.
/// 
/// ~~~~~~~~~~~~~~~{.hpp}
/// 
/// class alarm : public messageListener {
///        private:
///            void armAway(){
///                // Do something.
///            }
///        public:
///            virtual void messageReceived(const String & receivedMessage, const char* topic = "") override {
///                if(topic == "/alarm"){
///                    if (receivedMessage == "arm_away"){
///                        armAway();
///                    }
///                }
///            }
/// };
/// ~~~~~~~~~~~~~~~
class messageListener {
    public:
        virtual void messageReceived(const String & receivedMessage, const char* topic = "") = 0;
};

void callback(char* topic, byte* payload, unsigned int length);

/// \brief 
/// mqttClient
/// \details
/// Handles everything that should be handled for the use of MQTT (Message Queue Telemetry Transport).
/// Has functions for keeping the connection active, connecting to a network, subscribing to topics,
/// receiving messages and publishing messages. Literally takes care of everything.
/// 
/// ~~~~~~~~~~~~~~~{.cpp}
/// #include <ESP8266WiFi.h>
/// #include "mqttClient.hpp"
/// 
/// WiFiClient espClient;
/// mqttClient client(SSID, WPA, BROKER, TOPIC, espClient);
///
/// void callback(char* topic, byte* payload, unsigned int length) {
///     static String message;
///     for (int i = 0; i < length; i++) {
///         message.concat((char)payload[i]);
///     }
///     client.notifyListeners(message, topic);
///     message = "";  
/// }
///
/// void setup() {
///     client.setupWifi();                 // Connect to Network
///     client.setupConnections();          // Connect to Broker
/// }
///
/// void loop() {
///     client();                           // Keep connections active and listen for messages
/// }
/// ~~~~~~~~~~~~~~~
class mqttClient : public SensorListener {
    private:
        // For connection with network
        char* ssid;
        char* password;
        char* mqttServer;

        // For connection with MQTT-Broker
        const char* clientName;
        const char* clientPassword;
        
        // MQTT-Configurables
        const char* topic;
        PubSubClient client;
        const bool retainedMessages;
        const uint8_t qosLevel;
        
        // Listeners to notify on reception of MQTT Message
        messageListener * listeners[20] = {};
        uint8_t amountOfListeners = 0;
    public:
        mqttClient(char* ssid, char* password, char* mqttServer, const char* topic, WiFiClient & espClient, const char* clientName = "SensorBridge", const char* clientPassword = "BridgePass", const bool retainedMessages = true, const uint8_t qosLevel = 1);
        void begin();

        void addListener(messageListener & listener);
        void notifyListeners(const String & mesage, const char* topic);

        void setupWifi();
        void setupConnections();
        void reconnect();

        void operator()();
        void sendMessage(const char* topic, const char* messageToSend);

        virtual void dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity) override;
};

#endif //__CONNECTIONS_HPP