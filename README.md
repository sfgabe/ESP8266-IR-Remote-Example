# ESP8266-IR-Remote-Example

This sketch is my use case for an ESP8266 as a IR sender controlled by a webpage.
It uses the [IRremoteESP8266 library by markszabo](https://github.com/markszabo/IRremoteESP8266).

This example is the codes for [this particular bluetooth amp board](https://amzn.to/2ZdSxzY), in order for it to work for something else, you will need to program in your devices codes. I use the attached decoder ini for getting the raw codes from the remote.

## Hardware

For my project I'm using this to control a BT reciever and radio. It can now be controlled outside of remote range or through automations. The Wemos D1 Mini is attached to the IR transmitter board via the 4 pins, with RX and TX crossing. The radio board just sits close by and recieves the codes.
- [Wemos D1 Mini](https://amzn.to/2HduRpa)
- [IR Transmitter / Reciever Board](https://amzn.to/2KHA62O)
- [50W+50W Portable Bluetooth Audio Receiver Amplifier Board TF Card Decoder with Remote Control DC 8-26V](https://amzn.to/2ZdSxzY)

![BT & Radio Amp Board](https://images-na.ssl-images-amazon.com/images/I/711fOsizNqL._SL1001_.jpg "BT & Radio Amp Board")
![WEMOS D1 Mini](https://images-na.ssl-images-amazon.com/images/I/61exvKx7IyL._SL1001_.jpg "WEMOS D1 Mini")
![IR Transmitter / Reciever Board](https://images-na.ssl-images-amazon.com/images/I/51Uy6zUWaeL.jpg "IR Transmitter / Reciever Board")
