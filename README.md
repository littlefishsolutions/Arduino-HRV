# Arduino-HRV
<h3>HRV Arduino Controller with Air Economizer</H3>

So my history with this project is I live in Minnesota and my HRV PCB board fried.  I always wanted something with an air economizer since our springs and falls are perfect times to take in cool outside air and condition the house rather than turning on the air conditioner or opening windows.  This is where this project fits in.

<H2>HARDWARE:</H2></BR>
(1) [Arduino ESP32]<https://www.aliexpress.com/item/Official-DOIT-ESP32-Development-Board-WiFi-Bluetooth-Ultra-Low-Power-Consumption-Dual-Core-ESP-32-ESP/32819024360.html?spm=a2g0s.9042311.0.0.27424c4do7rOpC> - Used due to the amount of GPIO required for this project.  ESP8266 just wasn't enough GPIO.</BR>
(1) [120V to 5V step down power supply](https://www.aliexpress.com/item/1x-HLK-PM01-AC-DC-220V-To-5V-Step-Down-Power-Supply-Module-Household-Switch-P0/32795822005.html?spm=a2g0s.9042311.0.0.27424c4dLldESR)</BR>
(2) DHT 22 temperature sensors - [Inside sensor](https://www.aliexpress.com/item/1pcs-Free-Shipping-DHT22-AM2302-Digital-Temperature-and-Humidity-Sensor/1041496689.html?spm=a2g0s.9042311.0.0.27424c4dZmffRx) & [Outside Remote Sensor](https://www.aliexpress.com/item/1PCSX-DHT22-AM2302-Digital-Temperature-and-Humidity-Sensor-AM2302-with-wire/32767774502.html?spm=a2g0s.9042311.0.0.27424c4dZmffRx). Measure inside and outside temperature and humidity.</BR>
(1) [0.96" OLED display](https://www.aliexpress.com/item/free-shipping-0-96-inch-OLED-display-module-128X64-OLED-For-arduino-I2C-IIC-SPI-7p/32595065663.html?spm=a2g0s.9042311.0.0.27424c4dTsEpJw) for local indication of the mode it is operating and for temp/humidity indication.</BR>
(1) [8-Channel SSR 5 Volt High Level trigger relay board](https://www.aliexpress.com/item/Free-Shipping-5V-8-Way-High-Level-Trigger-Solid-State-Relay-Module-with-Fuse-Arduin-250V2A/32572982881.html?spm=a2g0s.9042311.0.0.27424c4dCzzOlT)</BR>
(1) [LM1117 Linear Voltage regulator](https://www.aliexpress.com/item/10PCS-LM1117T-3-3-TO220-LM1117-3-3-LM1117T-3-3V-LM1117-TO-220-free-shipping/32714386720.html?spm=a2g0s.9042311.0.0.27424c4dqXaiEi) to power ESP32 at 3.3V</BR>
(1) [Rotary Encoder KY-040](https://www.aliexpress.com/item/The-32TA1600-logic-board-320WA01C-BF-C67271-In-stock-Best-price-and-good-service/599712551.html?spm=a2g0s.9042311.0.0.27424c4dRSGR5c) & [Knob]() used as a push button.  Future functionality to include a menu and be able to select modes of operation and control setpoints.</BR>
(1) [Enclosure](https://www.aliexpress.com/item/1Pcs-DIY-2-Size-Plastic-Waterproof-Housing-Enclosure-Box-Electronic-Project-Instrument-Case-Outdoor-Junction-Box/32839180611.html?spm=a2g0s.9042311.0.0.27424c4dLldESR) to house the OLED display and encloder.</BR>

<H2>SOFTWARE:</H2></BR>
Blynk App for iOS to control and monitor the status of the controller.

<H2>ARDUINO LIBRARIES USED:</H2></BR>
WiFi.h, WiFiClient.h, BlynkSimpleEsp32.h, ArduinoOTA.h, mDNS.h, WiFiUdp.h, SPI.h, Wire.h, Adafruit_SSD1306.h
