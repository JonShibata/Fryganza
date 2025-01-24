
# Fryganza Thermostat Controller

## ESP Library
URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

ESP32 by Espressif Systems

Version: 3.1.1
<br><br>

## ESP32 Board

Board: Seeed Studios XIAO ESP32C6
<br><br>

## Dependencies

ESP32Servo by Kevin Harrington v 3.0.6

MAX6675 library by Adafruit v 1.1.2

LiquidCrystal v 1.0.7

Include symlink to this in `src/Thermostat/src` directory
https://github.com/jonshibata/thermostat
<br><br>

## Wiring

[XIAO ESP32C6 Pin Description](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/)


ESP32 PIN | Device                | Pin  | Pin Description | Color
----------|-----------------------|------|-----------------| ------
D0        | servo motor           | ?    | Control         | white
5V        | servo motor           | ?    | VCC             | red
GND       | servo motor           | ?    | GND             | black
5V        | MAX6675               | VCC  | 3.3v            | red
GND       | MAX6675               | GND  | ground          | black
D8        | MAX6675               | SCK  | clock           | ?
D9        | MAX6675               | SO   | MISO            | ?
D10       | MAX6675               | CS   | MOSI            | ?

<br><br>
