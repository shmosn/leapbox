# leapbox
This is about how to have a ESP32 Dev Kit communicate with an MQTT Server.

The chosen approach includes:
1 x ESP32 Dev Kit (WROOM32)
1 x bread board
1 x Resistor for pull-down of PIN "D23" to be able to flash the microcontroller (1k)
3 x resistor for current limiting for the used LEDs (100R)
3 x LED

Wiring:
-------

(see pinning: https://raw.githubusercontent.com/playelek/pinout-doit-32devkitv1/master/pinoutDOIT32devkitv1.png)
D23 -- 1k -- GND
D21 -- 100R -- red LED -- GND
D19 -- 100R -- yellow LED -- GND
D18 -- 100R -- green LED -- GND

Used Software:
--------------

Visual Studio
Platformio IDE ("open source ecosystem for IoT development") for VSCode https://platformio.org/platformio-ide 
library: PubSubClient inside VSCode (Visual Studio)
free MQTT Server / Broker: https://dioty.co (see an overview here: http://www.steves-internet-guide.com/iot-mqtt-dashboards/)


Inspiration:
------------

Setting up MQTT using with Visual Studio (Arduino) https://www.youtube.com/watch?v=5tG3JXFYrUo
Quick Start Guide: http://docs.platformio.org/en/latest/platforms/espressif32.html?utm_source=github&utm_medium=arduino-esp32
