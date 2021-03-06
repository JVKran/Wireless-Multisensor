# Bridge
The bridge functions as a, you guessed it, bridge between the wireless sensor module and the MQTT broker. Hence, it needs to have a 433Mhz receiver and be able to connect to an MQTT Broker. That's exactly what's happening here. 

## How to use
There are several ways to use this program. One can flash this code on an ESP-01, connect a SRX882 and just put it somewhere. However, when you already have an ESP8266 running somewhere, you could easily just copy this code with additional benefit a very easy to use MQTT client. That's what I've done with my [Central Hub](https://github.com/JVKran/Sensor-Module). If you're planning on doing so, I strongly recommend you to read the [documentation](/Documentation). I use some pretty advanced principles that aren't very Arduino friendly.

If you're not, and just want to use this code as-is, it's a lot easier to setup. Just flash the code on your desired microcontroller (ESP-01, D1 Mini, NodeMCU V3, etc.), connect to the 'Wireless-Multisensor Bridge' Acces-Point, navigate to '192.168.4.1' and enter your credentials. This is a one-time process; after this is done, the credentials will be stored in the EEPROM. For the more visual people, I also made a [video](https://youtu.be/CQGhy87DgMU) on how to set it up. When it's connected to your network, the interface will remain available; however, the IP-address has changed.

## Flashing
Since there is infinte amount of possible mirocontrollers, I won't go through all required steps to upload the code. For a D1 Mini or NodeMCU V3 for example, it's just a matter of flashing it via USB while for an ESP-01, it's a matter of using an FTDI programmer. 

What however is the same for all possible hardware/software combinations, is what libraries have to be installed. They are listed below.
- [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi); library needed for connection with Wifi-Network.
- [PubSubClient](https://github.com/knolleary/pubsubclient); an MQTT-Client library.
- [Manchester](https://github.com/mchr3k/arduino-libs-manchester); a Manchester Encoding library.
