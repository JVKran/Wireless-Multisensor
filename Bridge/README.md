# Bridge
The bridge functions as a, you guessed it, bridge between the wireless sensor module and the MQTT broker. Hence, it needs to have a 433Mhz receiver and be able to connect to an MQTT Broker. That's exactly what's happening here. 

## How to use
There are several ways to use this program. One can flash this code on an ESP-01, connect a SRX882 and just put it somewhere. However, when you already have an ESP8266 running somewhere, you could easily just copy this code with additional benefit a very easy to use MQTT client. That's what I've done with my [Central Hub](https://github.com/JVKran/Sensor-Module). 

## Flashing
Since there are an infinte amount of possibilities, I won't go through all required steps to upload the code. For a D1 Mini or NodeMCU V3 for example, it's just a matter of flashing it via USB while for an ESP-01, it's a matter of using an FTDI programmer. 