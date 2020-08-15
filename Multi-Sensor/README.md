# Multi-Sensor
This directory contains the code it's all about; the Wireless Multisensor. It measures temperature, relative humidity, pressure, illuminance and motion. Furthermore, a switch can be connected with a JST-Connector.

## How to use
<img src="/Documentation/Multi-Sensor.png" width=50% height=50% style="float:left">
Well, in contrary of the bridge, the amount possibilities are pretty small. It just does what it does. This should be flashed to an ATtiny85V running at 1MHz with the Brownout Detector enabled at 1.8V. After that has been done, one can plug it in a breadboard to recreate the schematic or solder it onto the PCB.

## Flashing
The most common ways to flash ATtiny's is by using an Arduino ISP programmer.
