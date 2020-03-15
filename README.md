# Smart-Home-Network
After years of using Home Assistant with power hungry ESP's and the desire to create battery powered sensor modules, I started creating a universal wireless network optimized for battery powered devices. The main goal is to create battery powered ATtiny's with motion and climate sensors that report to Home Assistant.

![alt text](/Documentation/Multi-Sensor.png "Multi-Sensor Schematic")

## Research
I've done quite a lot of research to determine the best protocol to use. This can be found in the Documentation folder. The result was, unsurprisingly, that 433MHz is the way to go. There are quite a lot of reasons to choose 433MHz; check out the document for further information.

## Status
Right now, it's possible to let an ATtiny85 with STX822, CR2032 and AM312 Home Assistant know when there's movement. Furthermore, since this release it's also reporting the temperature, humidity and pressure at configurable intervals.
