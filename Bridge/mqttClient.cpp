/// @file

#include "mqttClient.hpp"

/// \brief
/// Construct Instance
/// \details
/// Construct an instance of a MqttClient.
/// \param ssid The SSID of the network to connect to.
/// \param password The WPA-Password of the network to connect to.
/// \param topic The topic to subscribe to.
/// \param espClient The WiFiClient to use for communicating with the network.
/// \param clientName The clientId to use for connection with the broker.
/// \param clientPassword The password to use for authenticating with the MQTT broker.
/// \param retainedMessages Wether or not messages should be retained.
/// \param qosLevel The Quality of Service of the messages.
mqttClient::mqttClient(char* ssid, char* password, char* mqttServer, const char* topic, WiFiClient & espClient, const char* clientName, const char* clientPassword, const bool retainedMessages, const uint8_t qosLevel):
    ssid(ssid),
    password(password),
    mqttServer(mqttServer),
    clientName(clientName),
    clientPassword(clientPassword),
    topic(topic),
    client(espClient),
    retainedMessages(retainedMessages),
    qosLevel(qosLevel)
{}

/// \brief
/// Begin Instance
/// \details
/// Begin the MqttClient instance by setting the WiFi-Mode to Station.
void mqttClient::begin(){
    WiFi.mode(WIFI_STA);
}

/// \brief
/// Add Listener
/// \details
/// Add listener to the list of listeners to notify on reception of message.
/// \param listener The listener to add.
void mqttClient::addListener(messageListener & listener){
    if(amountOfListeners < 20){
        listeners[amountOfListeners++] = &listener;
    }
}

/// \brief
/// Notify Listeners
/// \details
/// Notify the listeners with a message and topic.
/// \param message The received message.
/// \param topic The topic on which the message was received.
void mqttClient::notifyListeners(const String & message, const char* topic){
	for (int i = 0; i < amountOfListeners; i++){
        listeners[i]->messageReceived(message, topic);
    }
}

/// \brief
/// Connect to Wifi
/// \details
/// Connect to the wifi network with the given ssid and wpa password.
void mqttClient::setupWifi() {
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

/// \brief
/// Connect to MQTT-Broker
/// \details
/// Connect to the MQTT-Broker with the given ip, subscribe to the desired topic and enable callback.
void mqttClient::setupConnections(){
    client.setServer(mqttServer, 1883);
    client.setCallback(callback);
    client.subscribe(topic, qosLevel);
    reconnect();                //Establish a connection by signing in with credentials.
}

/// \brief
/// Reconnect to MQTT-Broker
/// \details
/// Connect to the MQTT-Broker with the given ip, clientId and clientPassword.
void mqttClient::reconnect() {
    while (!client.connected()) {
        if (client.connect("Wireless Multisensor Bridge", clientName, clientPassword)) {
            client.subscribe(topic);
            for (int i = 0; i < amountOfListeners; i++){
                listeners[i]->messageReceived("CONNECTED");
            }
            break;
        } else {
            delay(500);
        }
    }
}

/// \brief
/// Keep connection alive
/// \details
/// Loop the client and reconnect when connection is lost.
void mqttClient::operator()(){
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}


/// \brief
/// Publish Message
/// \details
/// Publish a message to the passed topic.
/// \param topic The topic to publish the message to.
/// \param messageToSend The message to publish on the topic.
void mqttClient::sendMessage(const char* topic, const char* messageToSend){
    client.publish(topic, messageToSend, retainedMessages);
}

/// \brief
/// Publish data from SensorBridge.
/// \details
/// Publish the from the SensorBridge received data.
/// \param sensorId The id of the sensor that sent the received message.
/// \param motion Wether or not there was motion detected.
/// \param reedState The state of the switch.
/// \param temperature The measured temperature.
/// \param humidity The measured humidity.
/// \param pressure The measured pressure.
/// \param lightIntensity The measured light intensity.
/// \param voltage The voltage of the battery.
void mqttClient::dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity){
    String temp = String(temperature).substring(0, 2);
    String commaTemp = String(temperature).substring(2);

    String hum = String(humidity).substring(0, 2);
    String commaHum = String(humidity).substring(2);

    String press = String(pressure).substring(0, 4);
    String commaPress = String(pressure).substring(4);

    client.publish(String("/sensormodules/" + String(sensorId) + "/motion").c_str(),      String(motion).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/reed").c_str(),          String(reedState).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/temperature").c_str(),   String(temp + '.' + commaTemp).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/humidity").c_str(),   String(hum + '.' + commaHum).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/pressure").c_str(),     String(press + '.' + commaPress).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/voltage").c_str(),      String(voltage).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/illuminance").c_str(),  String(lightIntensity).c_str());
}
