# Arduino-HRV
HRV Arduino Controller with Air Economizer

So my history with this project is I live in Minnesota and my HRV PCB board fried.  I always wanted something with an air economizer since our springs and falls are perfect times to take in cool outside air and condition the house rather than turning on the air conditioner or opening windows.  This is where this project fits in.

HARDWARE:
(1) Arduino ESP32 - Used due to the amount of GPIO required for this project.  ESP8266 just wasn't enough GPIO.
(2) DHT 22 temperature sensors - Measure inside and outside temperature and humidity.
(1) 0.96" OLED display for local indication of the mode it is operating and for temp/humidity indication.
(1) 8-Channel SSR 5 Volt High Level trigger relay board
(1) LM1117 Linear Voltage regulator to power ESP32 at 3.3V
(1) Encoder KY-040 used as a push button.  Future functionality to include a menu and be able to select modes of operation and control setpoints.

SOFTWARE:
Blynk App for iOS to control and monitor the status of the controller.

ARDUINO LIBRARIES USED:
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>     //Blynk
#include <ArduinoOTA.h>           //OTA
#include <mDNS.h>                 //OTA
#include <WiFiUdp.h>              //OTA
#include <SPI.h>                  //OLED
#include <Wire.h>                 //OLED
#include <Adafruit_SSD1306.h>     //OLED
