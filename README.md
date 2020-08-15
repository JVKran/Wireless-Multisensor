[![](https://img.shields.io/github/v/release/JVKran/Wireless-Multisensor.svg?style=flat-square)](https://github.com/JVKran/Wireless-Multisensor/releases/latest)

# Wireless Multi-Sensor
ESP8266's are great! They are suited for an endless amount of fun projects. However, when you want to create small wireless modules, they are not so great; they consume way too much power and are pretty bulky. That's where this Wireless Multisensor comes into play; it features a sleep current of 50uA resulting in a lifetime of 3 years when using a CR123A battery! This is possible due to the use of an ATtiny85V with energy efficient components, sensors and libraries. Furthermore, it has a neat PCB and casing that really finishes it off!

All in all this sensor measures temperature, humidity, pressure, motion and illuminance. While that's already impressive, it's also possible to connect a switch to the JST-Connector to furter increase possibilities and tweakability. Hence one can connect a reed switch to determine the state of windows or doors, a button to toggle the lights in the room or a vibration sensor to detect movement of the sensor. These features will now be examined further.

## Features
### Climate
The Multisensor is equipped with a BME280 that's able to measure temperature, relative humidity and pressure. The interval at which these are measured and transmitted can easily be changed. Due to the encredibly well engineerd chip, measurements are very accurate. The temperature and humidity have an absolute accuracy of 1% while the pressure reading has an absolute accuracy of 3%. Further specifications can be read in the [datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf). By using my [Forced-BME280 library](https://github.com/JVKran/Forced-BME280) the chip only uses 0.25uA when in sleep. That's impressive!

### Motion
An AM312 Passive Infrared sensor is used; these are small, only draw 16uA and dont suffer from false positives. The detection distance is approximately 3-5m depending on light circumstances. As if that isn't impressive yet, the detection is practically instant since an interrupt is used. By disabling interrupts after the interrupt occured, and waiting for the voltage to settle at the pin, false positives are practically impossible. All in all this is a solid but small PIR-Sensor perfectly suited for this kind of use.

### Reed Switch
Since it could be useful to detect the position of a door or window, one pin is exposed through a JST-Connector. This is done so one can also use a button to trigger a transmission; hence it doesn't have to be wired to a reed switch. A button to toggle lights or a vibration sensor to fire the alarm are all possibly useful scenario's. It's important to note that current draw is very limited; due to the small amount of available energy, the +-rail of the JST-Connector is placed in series with a 1M resistor. Hence a button with a LED is not possible. Otherwise the battery would be dead in no time.

### Illuminance
Last but not least the Multisensor is also equipped with a BH1750 to measure illuminance. By only using my [OneTime-BH1750 library](https://github.com/JVKran/OneTime-BH1750), the chip is incredibly energy efficient; it only draws 6uA when in sleep making it ideal for a wireless multisensor like this. This mode results in an accuracy of 4lx. That's totally acceptable since a measurement in High Res mode, with an accuracy of 1lx, would take 160ms; that's a little long which would impact the battery life significantly. All in all it's way more efficient than a traditional LDR making it perfect for this Wireless Multisensor

### Years of worryless use
The ATtiny85V with a minimum voltage of 1.7V and a clock set at 1MHz paired with the abovely mentioned components and some very efficient libraries, results in a current draw of 52uA. Based on a 1500mAh CR123A this means the battery has got to be changed every (1500mAh / 0.052 =) 28800 hours or 1200 days or 3.3 years!

## Home-Assistant
Since the bridge is used to 'bridge' the messages from the Wireless Multisensor to MQTT, all we have to do is to create some binary and normal MQTT sensors.

```yaml
sensor:
  - platform: mqtt
    state_topic: "/sensormodules/1/spanning"                     
  - platform: mqtt
    state_topic: "/sensormodules/1/temperatuur"
    device_class: temperature
  - platform: mqtt
    state_topic: "/sensormodules/1/vochtigheid"                                                                             
    device_class: humidity                                                                                                                                   
  - platform: mqtt                                                                                                          
    state_topic: "/sensormodules/1/luchtdruk"                                                                               
    device_class: pressure                                                                                                  
  - platform: mqtt                                                                                                          
    state_topic: "/sensormodules/1/lichtsterkte"                                                                            
    device_class: illuminance 

binary_sensor:
  - platform: mqtt                                                                                                          
    state_topic: "/sensormodules/1/beweging"
    payload_on: "1"                                                                                                         
    name: "Beweging 1"                                                                                                      
    off_delay: 5                                                                                                            
    device_class: "motion"
  - platform: mqtt                                                                                                          
    state_topic: "/sensormodules/1/reed"                                                                                    
    payload_on: "0"                                                                                                         
    payload_off: "1"
    device_class: "door"                                                       
```
