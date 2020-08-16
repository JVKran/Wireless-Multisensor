# Multi-Sensor
This directory contains everything that's needed to setup your very own Wireless-Multisensor. Everything is taken care of; wiring, compiling and flashing. If anything is unclear or you're having trouble with anything, feel free to contact me and I'll do my best to resolve the issue.

## How to use
<img src="/Documentation/Multi-Sensor.png" width=30% height=30% align="right">

Well, in contrary of the bridge, the amount of ways to use it, are pretty small; it just does what it does... The code above should be flashed to an ATtiny85V running at 1MHz with the Brownout Detector enabled at 1.8V. After that has been done, one can plug it in a breadboard to recreate the [schematic](Documentation/Multi-Sensor%20Schematic.png) or solder it onto the [PCB](/PCB). 

For people who arent used to reading schematics, I also made a [Fritzing Diagram](/Documentation/Multi-Sensor.png) that's visible on the right. For stability reasons, I strongly recommend everyone to use the PCB for pronlonged periods of use; it's way more stable in operation. Only use a breadboard for testing purposes and verifying wether or not the flashing was succesful.

## Flashing
The codebase utilizes [SpenceKonde's ATTinyCore](https://github.com/SpenceKonde/ATTinyCore). Hence that has got to be installed. The instructions on the linked page are very clear and easy to follow. Once that has been installed, there are also some libraries needed to compile the code. Hence, the following libraries have to be installed:
- [Forced-BME280](https://github.com/JVKran/Forced-BME280); a small and efficient library for the BME280.
- [OneTime-BH1750](https://github.com/JVKran/OneTime-BH1750); a small and efficient library for the BH1750.
- [Manchester](https://github.com/mchr3k/arduino-libs-manchester); a Manchester Encoding library.

Once that's all downloaded and installed, one is almost ready to compile and flash the code. There are only a couple more settings to alter to make sure the mentioned battery life is possible and operation is stable.

<img src="/Documentation/Flash-Settings.png" width=30% height=30% align="right">

- ATtiny85 has got to be selected.
- Clock should be set at "1MHz internal"
- B.O.D. Level should be set at 1.8V
- millis()/micros() should be disabled

<img src="/Documentation/Uno-Flash.png" width=30% height=30% align="right">

You're now ready to flash the code! By uploading the Arduino ISP sketch to an Arduino UNO, wiring the ATtiny85V to the Arduino UNO as in this schematic and uploading the Multi-Sensor.ino to the port to which the Arduino UNO is connected, the ATtiny85V is flashed and ready to use! I strongly recommend recreating the abovely visible schematic on a breadboard and setting everything up as you're willing to use it in the future to verify that everything works as expected. Don't be scared when it stops working after several days; that instability is caused by the long jumper wires on the breadboard and will not occur when everything is solderd onto the PCB.
