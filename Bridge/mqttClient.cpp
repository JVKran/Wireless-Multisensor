/// @file

#include "MqttClient.hpp"

/// \brief
/// Construct Instance
/// \details
/// Construct an instance of a MqttClient.
/// \param ssid The SSID of the network to connect to.
/// \param wpa The WPA-Password of the network to connect to.
/// \param topic The topic to subscribe to.
/// \param espClient The WiFiClient to use for communicating with the network.
/// \param clientName The clientId to use for connection with the broker.
/// \param clientPassword The password to use for authenticating with the MQTT broker.
/// \param retainedMessages Wether or not messages should be retained.
/// \param qosLevel The Quality of Service of the messages.
MqttClient::MqttClient(char* ssid, char* wpa, char* mqttServer, const char* topic, WiFiClient & espClient, const char* clientName, const char* clientPassword, const bool retainedMessages, const uint8_t qosLevel):
    ssid(ssid),
    wpa(wpa),
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
/// Begin the MqttClient instance by checking wether or not there are valid credentials in the EEPROM memory.
/// If there are, Station Mode is activated and operation is as normals. When there aren't, Access Point Mode is activated
/// and nothing will be done until valid credentials have been entered.
void MqttClient::begin(){
    memory.begin(512);
    delay(1000);
    Serial.println("Started up!");

    if(ssid == "" || wpa == ""){
        ssid = memory.readText((uint8_t)addresses::ssid);
        wpa = memory.readText((uint8_t)addresses::wpa);
        mqttServer = memory.readText((uint8_t)addresses::ip);

        Serial.println("These credentials are found in EEPROM:");
        Serial.print("SSID: \"");
        Serial.print(ssid);
        Serial.print("\" \nWPA: \"");
        Serial.print(wpa);
        Serial.print("\" \nBroker: \"");
        Serial.print(mqttServer);
        Serial.println("\"");

        if(ssid == "" || wpa == ""){
            WiFi.softAP("Multisensor Bridge");
            Serial.println("Access Point Mode activated. Please connect to it and navigate to '192.168.4.1'.");
        } else {
            WiFi.mode(WIFI_STA);
            Serial.println("Station Mode activated.");
        }
    } else {
        Serial.println("Station Mode activated.");
    }
    Serial.println();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    server.on("/get", HTTP_GET, [this] (AsyncWebServerRequest *request) {
        this->ssid = request->getParam("ssid")->value();
        this->wpa = request->getParam("wpa")->value();
        this->mqttServer = request->getParam("ip")->value();
        this->credentialsChanged = true;

        this->memory.storeText((uint8_t)this->addresses::ssid, this->ssid);
        this->memory.storeText((uint8_t)this->addresses::wpa, this->wpa);
        this->memory.storeText((uint8_t)this->addresses::ip, this->mqttServer);

        request->send(200, "text/html", "Connection is being established. If credentials are wrong, the Acces Point will be activated again; no worries.");

        Serial.println("New credentials entered:");
        Serial.print("SSID: \"");
        Serial.print(this->ssid);
        Serial.print("\" \nWPA: \"");
        Serial.print(this->wpa);
        Serial.print("\" \nBroker: \"");
        Serial.print(this->mqttServer);
        Serial.println("\"\n");

    });

    server.begin();

    if(ssid != "" && wpa != ""){
        setupWifi();
        setupConnections();
    }
}

/// \brief
/// Add Listener
/// \details
/// Add listener to the list of listeners to notify on reception of message.
/// \param listener The listener to add.
void MqttClient::addListener(messageListener & listener){
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
void MqttClient::notifyListeners(const String & message, const char* topic){
	for (int i = 0; i < amountOfListeners; i++){
        listeners[i]->messageReceived(message, topic);
    }
}

/// \brief
/// Connect to Wifi
/// \details
/// Connect to the wifi network with the given ssid and wpa password.
void MqttClient::setupWifi() {
    if(ssid != "" && wpa != ""){
        WiFi.begin(ssid, wpa);
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            WiFi.mode(WIFI_AP);
            WiFi.softAP("Wireless-Multisensor Bridge");
            Serial.println("Credentials are wrong. Activating Access Point Mode! Please connect to it and navigate to '192.168.4.1'.");
            return;
        } else {
            WiFi.mode(WIFI_STA);
            connected = true;
            Serial.print("Connected to ");
            Serial.print(ssid);
            Serial.print(" with IP-Address: ");
            Serial.println(WiFi.localIP());
            Serial.println();
        }
    } else {
        WiFi.softAP("Wireless-Multisensor Bridge");
    }
}

/// \brief
/// Connect to MQTT-Broker
/// \details
/// Connect to the MQTT-Broker with the given ip, subscribe to the desired topic and enable callback.
void MqttClient::setupConnections(){
    if(connected){
        client.setServer(mqttServer.c_str(), 1883);
        client.setCallback(callback);
        client.subscribe(topic, qosLevel);
        reconnect();    
    }
}

/// \brief
/// Reconnect to MQTT-Broker
/// \details
/// Connect to the MQTT-Broker with the given ip, clientId and clientPassword.
void MqttClient::reconnect() {
    while (!client.connected()) {
        if (client.connect("Wireless Multisensor Bridge", clientName, clientPassword)) {
            client.subscribe(topic);
            for (int i = 0; i < amountOfListeners; i++){
                listeners[i]->messageReceived("CONNECTED");
            }
            sendMessage("Test", "Connected to broker!");
            break;
        } else {
            delay(500);
        }
    }
}

/// \brief
/// Is client connected
/// \return
/// Wether or not the client is connected to a wifi network.
bool MqttClient::connectionEstablished(){
    return connected;
}

/// \brief
/// Keep connection alive
/// \details
/// Loop the client and reconnect when connection is lost.
void MqttClient::operator()(){
    if (connected) {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();
    } if(credentialsChanged) {
        Serial.println("Attempting connection.");

        setupWifi();
        setupConnections();
        credentialsChanged = false;
    }
}


/// \brief
/// Publish Message
/// \details
/// Publish a message to the passed topic.
/// \param topic The topic to publish the message to.
/// \param messageToSend The message to publish on the topic.
void MqttClient::sendMessage(const char* topic, const char* messageToSend){
    if(connected){
        client.publish(topic, messageToSend, retainedMessages);
    }
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
void MqttClient::dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity){
    String temp = String(temperature).substring(0, 2);
    String commaTemp = String(temperature).substring(2);

    String hum = String(humidity).substring(0, 2);
    String commaHum = String(humidity).substring(2);

    String press = String(pressure).substring(0, 4);
    String commaPress = String(pressure).substring(4);

    if(connected){
        client.publish(String("/sensormodules/" + String(sensorId) + "/motion").c_str(),      String(motion).c_str());
        client.publish(String("/sensormodules/" + String(sensorId) + "/reed").c_str(),          String(reedState).c_str());
        client.publish(String("/sensormodules/" + String(sensorId) + "/temperature").c_str(),   String(temp + '.' + commaTemp).c_str());
        client.publish(String("/sensormodules/" + String(sensorId) + "/humidity").c_str(),   String(hum + '.' + commaHum).c_str());
        client.publish(String("/sensormodules/" + String(sensorId) + "/pressure").c_str(),     String(press + '.' + commaPress).c_str());
        client.publish(String("/sensormodules/" + String(sensorId) + "/voltage").c_str(),      String(voltage).c_str());
        client.publish(String("/sensormodules/" + String(sensorId) + "/illuminance").c_str(),  String(lightIntensity).c_str());
    }
}
