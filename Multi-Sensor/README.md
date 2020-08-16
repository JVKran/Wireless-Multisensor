# Multi-Sensor
This directory contains the code it's all about; the Wireless Multisensor. It measures temperature, relative humidity, pressure, illuminance and motion. Furthermore, a switch can be connected with a JST-Connector. But how do I go about putting it in use?

## How to use
<img src="/Documentation/Multi-Sensor.png" width=30% height=30% align="right">

Well, in contrary of the bridge, the amount of ways to use it, are pretty small. It just does what it does. The code above should be flashed to an ATtiny85V running at 1MHz with the Brownout Detector enabled at 1.8V. After that has been done, one can plug it in a breadboard to recreate the [schematic](Documentation/Multi-Sensor%20Schematic.png) or solder it onto the [PCB](/PCB). For people who arent used to reading schematics, I also made a [Fritzing Diagram](/Documentation/Multi-Sensor.png) that's visible on the right. For stability reasons, I strongly recommend everyone to use the PCB for pronlonged periods of use; it's way more stable in operation. Just use a breadboard for testing purposes.

## Flashing
The codebase utilizes [SpenceKonde's ATTinyCore](https://github.com/SpenceKonde/ATTinyCore). Hence that has got to be installed. The instructions on the linked page are very clear and easy to follow. Once that has been done, there are also some libraries needed to compile the code. Hence, the following libraries have to be installed:
- [Forced-BME280](https://github.com/JVKran/Forced-BME280)
- [OneTime-BH1750](https://github.com/JVKran/OneTime-BH1750)
- [Manchester](https://github.com/mchr3k/arduino-libs-manchester)

Once that's all done, one is almost ready to compile and flash the code. There are only a couple more settings to alter to make sure the mentioned battery life is possible.
- ATtiny85 has got to be selected.
- Clock should be set at "1MHz internal"
- B.O.D. Level should be set at 1.8V
- millis()/micros() should be disabled

You're now ready to flash the code!
