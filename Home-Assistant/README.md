## Home-Assistant
To be able to let the Wireless-Multisensors talk to, for example, Home-Assistant, one would need to use the Bridge to 'bridge' the 433MHz messages to MQTT. This can be done by using the code in the [Bridge Directory](/Bridge). Further information can also be found there. It boils down to two possibilities; using a dedicated ESP or letting the code run on an already in use ESP. Anyhow, the code and principle is the same. 

### Configuration
For the Wireless-Multisensors to show up in Home-Assistant, one has to change the configuration. The required code for configuring the Wireless Multisensor is verly limited and can be found in the [configuration.yaml](/Home-Assistant/configuration.yaml) file. It's just a matter of copy-paste. It is however, worth noticing that the id of the Wireless-Multisensor defines the topic the measurements are published on. Hence it's important to keep track of what id's are already in use. Two Wireless-Multisensors with the same id doesnt't mean they won't work anymore; they'll just end up as one sensor which isn't what we want.

### Lovelace UI
I trust most of you to know how to integrate it into the Lovelace UI but for starters there's an example in [lovelace.yaml](/Home-Assistant/lovelace.yaml); just copy and paste it in a manual card.
