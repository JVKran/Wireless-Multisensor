# Bridge
The bridge functions as a, you guessed it, bridge between the wireless sensor module and the MQTT broker. Hence, it needs to have a 433Mhz receiver and be able to connect to an MQTT Broker. That's exactly what's happening here. 

## How to use
There are several ways to use this program. One can flash this code on an ESP-01, connect a SRX882 and just put it somewhere. However, when you already have an ESP8266 running somewhere, you could easily just copy this code with additional benefit a very easy to use MQTT client. That's what I've done with my [Central Hub](https://github.com/JVKran/Sensor-Module). If you're planning on doing so, I strongly recommend you to read the [documentation](/Documentation). I use some pretty advanced principles that aren't very Arduino friendly.

## Flashing
Since there are an infinte amount of possibilities, I won't go through all required steps to upload the code. For a D1 Mini or NodeMCU V3 for example, it's just a matter of flashing it via USB while for an ESP-01, it's a matter of using an FTDI programmer. 

What however is the same for all possible hardware/software combinations, is what libraries have to be installed. They are listed below.
- [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi); library needed for connection with Wifi-Network.
- [PubSubClient](https://github.com/knolleary/pubsubclient); an MQTT-Client library.
- [Manchester](https://github.com/mchr3k/arduino-libs-manchester); a Manchester Encoding library.
