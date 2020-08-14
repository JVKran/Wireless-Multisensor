# Wireless Multi-Sensor
ESP8266's are great! They are suited for an endless amount of fun projects. However, when you want to create small wireless modules, they are not so great; they consume way too much power and are pretty bulky. That's where this Wireless Multisensor comes into play; it features a sleep current of 50uA resulting in a lifetime of 3 years when using a CR123A battery! Furthermore, it has a neat PCB and casing that really finishes it off.

## Features
### Climate
The Multisensor is equipped with a BME280 that's able to measure temperature, relative humidity and pressure. The interval at which these are measured and transmitted can easily be changed. Due to the encredibly well engineerd chip, measurements are very accurate.

### Motion
An AM312 Passive Infrared sensor is used; these are small, only draw 16uA and dont suffer from false positives. The detection distance is approximately 3-5m depending on light circumstances.

### Reed Switch
Since it could be useful to detect the position of a door or window, one pin is exposed through a JST-Connector. This is done so one can also use a button to trigger a transmission; hence it doesn't have to be wired to a reed switch.

### Illuminance
Last but not least the Multisensor is also equipped with a BH1750 to measure illuminance with an accuracy of 4lx. It only draws 6uA when in sleep making it ideal for a wireless multisensor like this.

### 3 Years
The ATtiny85V with a minimum voltage of 1.7V and a clock set at 1MHz paired with the abovely mentioned components and some very efficient libraries, results in a current draw of 52uA. Based on a 1500mAh CR123A this means the battery has got to be changed every (1500mAh / 0.052 =) 28800 hours or 1200 days or 3.3 years!