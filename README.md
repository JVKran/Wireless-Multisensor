# Wireless Multi-Sensor
After years of using Home Assistant with power hungry ESP's and the desire to create battery powered sensor modules, I started thinking about alternatives. Soon, I found out about these ATtiny microcontrollers and their incredibly low amount of current draw; that's when I knew this could work.

<img src="/Documentation/Multi-Sensor.png" width="200">

## Research
I've done quite a lot of research to determine the best protocol to use. This can be found in the Documentation folder. The result was, unsurprisingly, that 433MHz is the way to go. There are quite a lot of reasons to choose 433MHz; check out the document for further information.

## Status
Right now, it's possible to let an ATtiny85 with STX822, CR2032 and AM312 inform Home Assistant when there's movement. Furthermore, since this release it's also reporting the temperature, humidity and pressure at configurable intervals.
