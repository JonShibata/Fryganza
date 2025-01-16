# Fryganza Thermostat Controller

## ESP Library
URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

ESP32 by Espressif Systems
Version: 3.1.1

# Dependencies

ESP32Servo by Kevin Harrington v 3.0.6
MAX6675 library by Adafruit v 1.1.2
LiquidCrystal v 1.0.7

## Wiring Information









While enjoying a cookout with my friends my buddy's deep frier temperature controller was malfunctioned. To save the day he would take a piece of ice and place it next to the thermostat. After watching his arm hairs look singed decided we should upgrade the system. This is an  arduino C++ project using an ESP32 board configured to be a wifi access point. It serves a web page for  user input and displays the current temperature. A servo motor is used to control the burners on the deep  frier.
<br><br>
The parties kept growing and we decided keeping track of the temperature of our now multiple friers would be helpful. Some upgrades were made. The access point ESP32 was upgraded to accept data from a second arduino wifi board. Now the second temperature reporter can feed the information to the original board. The web page can display the data from both friers. <br><br>