/// @file

#ifndef __CONNECTIONS_HPP
#define __CONNECTIONS_HPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Memory.hpp"
#include "SensorBridge.hpp"
#include "Webpage.hpp"

/// \brief 
/// Listen for MQTT-Messages
/// \details
/// Inherit from this class and override the given abstract function to be notified when a 
/// message is received. This can be done like in the example underneath.
/// 
/// ~~~~~~~~~~~~~~~{.cpp}
/// 
/// class alarmSystem : public messageListener {
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
///
/// WiFiClient espClient;
/// mqttClient client(SSID, WPA, BROKER, TOPIC, espClient);
/// alarmSystem alarm = alarmSystem();
/// 
/// void setup() {
///     client.begin();
///     client.setupWifi();                 // Connect to Network
///     client.setupConnections();          // Connect to Broker
///
///     client.addListener(alarm);
/// }
///
/// void loop() {
///     client(); 
/// }
///
///
/// ~~~~~~~~~~~~~~~
class messageListener {
    public:
        virtual void messageReceived(const String & receivedMessage, const char* topic = "") = 0;
};

void callback(char* topic, byte* payload, unsigned int length);

/// \brief 
/// Client for MQTT-Broker.
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
class MqttClient : public SensorListener {
    private:
        // For connection with network
        String ssid;
        String wpa;
        String mqttServer;
        bool credentialsChanged = false;

        // For connection with MQTT-Broker
        const char* clientName;
        const char* clientPassword;
        
        // MQTT-Configurables
        const char* topic;
        PubSubClient client;
        const bool retainedMessages;
        const uint8_t qosLevel;

        AsyncWebServer server = AsyncWebServer(80);
        bool connected = false;
        
        // Listeners to notify on reception of MQTT Message
        messageListener * listeners[20] = {};
        uint8_t amountOfListeners = 0;

        Memory memory = Memory();

        enum class addresses {
            ssid = 0,
            wpa = 64,
            ip = 128
        };

    public:
        MqttClient(char* ssid, char* wpa, char* mqttServer, const char* topic, WiFiClient & espClient, const char* clientName = "SensorBridge", const char* clientPassword = "BridgePass", const bool retainedMessages = true, const uint8_t qosLevel = 1);
        void begin();

        void addListener(messageListener & listener);
        void notifyListeners(const String & mesage, const char* topic);

        void setupWifi();
        void setupConnections();
        void reconnect();
        bool connectionEstablished();

        void operator()();
        void sendMessage(const char* topic, const char* messageToSend);

        virtual void dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity) override;
};

#endif //__CONNECTIONS_HPP