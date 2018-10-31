# Arduino-HRV
HRV Arduino Controller with Air Economizer

So my history with this project is I live in Minnesota and my HRV PCB board fried.  I always wanted something with an air economizer since our springs and falls are perfect times to take in cool outside air and condition the house rather than turning on the air conditioner or opening windows.  This is where this project fits in.

HARDWARE:</BR>
(1) Arduino ESP32 - Used due to the amount of GPIO required for this project.  ESP8266 just wasn't enough GPIO.</BR>
(2) DHT 22 temperature sensors - Measure inside and outside temperature and humidity.</BR>
(1) 0.96" OLED display for local indication of the mode it is operating and for temp/humidity indication.</BR>
(1) 8-Channel SSR 5 Volt High Level trigger relay board</BR>
(1) LM1117 Linear Voltage regulator to power ESP32 at 3.3V</BR>
(1) Encoder KY-040 used as a push button.  Future functionality to include a menu and be able to select modes of operation and control setpoints.</BR>

SOFTWARE:</BR>
Blynk App for iOS to control and monitor the status of the controller.

ARDUINO LIBRARIES USED:</BR>
WiFi.h, WiFiClient.h, BlynkSimpleEsp32.h, ArduinoOTA.h, mDNS.h, WiFiUdp.h, SPI.h, Wire.h, Adafruit_SSD1306.h
