#include <ESP8266WiFi.h>
#include "mqttClient.hpp"
#include "sensorBridge.hpp"

#define SSID ""
#define WPA ""
#define BROKER ""
#define TOPIC ""

WiFiClient espClient;
mqttClient client(SSID, WPA, BROKER, TOPIC, espClient);
SensorBridge sensorBridge = SensorBridge(D5, MAN_2400);

void callback(char* topic, byte* payload, unsigned int length) {
    static String message;
    for (int i = 0; i < length; i++) {
        message.concat((char)payload[i]);
    }
    client.notifyListeners(message, topic);
    message = "";  
}

void setup() {
    client.setupWifi();
    client.setupConnections();
	sensorBridge.addListener(client);
}

void loop() {
    client();               // Handle incoming messages
    sensorBridge();         // Handle incoming transmissions from Wireless Multi-Sensors.
}