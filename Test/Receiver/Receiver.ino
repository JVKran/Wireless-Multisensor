#include "Manchester.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace these with values suitable for your network.
const char* ssid = "SSID";
const char* password = "WPA";
const char* mqtt_server = "MQTT-Broker-IP";

WiFiClient espClient;
PubSubClient client(espClient);


#define RX_PIN D2

#define BUFFER_SIZE 11
uint8_t buffer[BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.begin(19200);
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (man.receiveComplete()) {
    Serial.print("Buffersize: ");
    Serial.print(buffer[0]);
    Serial.print(", id: ");
    Serial.print(buffer[1]);
    Serial.print(", motion: ");
    Serial.print(buffer[2]);

    Serial.print(", temperature: ");
    int16_t temperature = buffer[3] + (buffer[4] << 8);
    String temp = String(temperature).substring(0, 2);
    String commaTemp = String(temperature).substring(2);
    Serial.print(temp);
    Serial.print('.');
    Serial.print(commaTemp);

    Serial.print(", humidity: ");
    int16_t humidity = buffer[5] + (buffer[6] << 8);
    String hum = String(humidity).substring(0, 2);
    String commaHum = String(humidity).substring(2);
    Serial.print(hum);
    Serial.print('.');
    Serial.print(commaHum);

    Serial.print(", pressure: ");
    int32_t pressure = buffer[7] + ((buffer[8] << 8) + ((buffer[9] << 16) + (buffer[10] << 24)));
    String press = String(pressure).substring(0, 4);
    String commaPress = String(pressure).substring(4);
    Serial.print(press);
    Serial.print('.');
    Serial.println(commaPress);

    if(buffer[1] == 1){     // If ID == 1;
        client.publish("/slaapkamer/beweging", String(buffer[2]).c_str());
        String temp = String(temperature).substring(0, 2);
        String commaTemp = String(temperature).substring(2);
        client.publish("/slaapkamer/temperatuur", String(temp + '.' + commaTemp).c_str());
        client.publish("/slaapkamer/vochtigheid", String(hum + '.' + commaHum).c_str());
        client.publish("/slaapkamer/luchtdruk", String(press + '.' + commaPress).c_str());
    }

    man.beginReceiveArray(BUFFER_SIZE, buffer);
  }
}