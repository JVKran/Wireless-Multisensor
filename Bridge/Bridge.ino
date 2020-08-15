#include <ESP8266WiFi.h>
#include "mqttClient.hpp"
#include "sensorBridge.hpp"

// /* Replace these with your own credentials and then uncomment.
//    Has been commented so you get a compile error when you forget.
#define SSID "SSID"
#define WPA "WPA2"
#define BROKER "0.0.0.0"
#define TOPIC "/raspberry/home-assistant"
//*/

WiFiClient espClient;                   // Can also be passed to other instances.
mqttClient client(SSID, WPA, BROKER, TOPIC, espClient);
SensorBridge sensorBridge = SensorBridge();

void callback(char* topic, byte* payload, unsigned int length) {
    static String message;
    for (int i = 0; i < length; i++) {
        message.concat((char)payload[i]);
    }
    client.notifyListeners(message, topic);
    message = "";  
}

void setup() {
    client.begin();
    client.setupWifi();                 // Connect to Network
    client.setupConnections();          // Connect to Broker

    sensorBridge.begin(3, MAN_2400);
	sensorBridge.addListener(client);   // Let client listen to sensor readings
}

void loop() {
    client();                           // Handle incoming sensor readings and publish them
    sensorBridge();                     // Handle incoming transmissions from Wireless Multi-Sensors.
}