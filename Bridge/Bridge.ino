#include <ESP8266WiFi.h>
#include "MqttClient.hpp"
#include "SensorBridge.hpp"

// Can be left blank for wireless setup; only interesting for the ones that are embedding the code in an already existing project.
#define SSID ""
#define WPA ""
#define BROKER ""
#define TOPIC "/raspberrypi/homeassistant"

WiFiClient espClient;                   // Can also be passed to other instances.
MqttClient client(SSID, WPA, BROKER, TOPIC, espClient);
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
    Serial.begin(9600);
    client.begin();

    sensorBridge.begin(3, MAN_2400);
	sensorBridge.addListener(client);   // Let client listen to sensor readings
}

void loop() {
    client();                           // Handle incoming sensor readings and publish them
    sensorBridge();                     // Handle incoming transmissions from Wireless Multi-Sensors.   
}