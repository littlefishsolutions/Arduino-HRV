/* Copyright (c) 2018, Mike Voelker <wiggum_guy@yahoo.com> 
   All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//Built using ESP32 -- NodeMCU-32S is the board, Upload Speed 512000, Flash 40MHz

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>     //Blynk
#include <ArduinoOTA.h>           //OTA
#include <mDNS.h>                 //OTA
#include <WiFiUdp.h>              //OTA
#include <SPI.h>                  //OLED
#include <Wire.h>                 //OLED
#include <Adafruit_SSD1306.h>     //OLED
//#include <ESP8266WiFi.h>        //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
//#include <DNSServer.h>          //Local DNS Server used for redirecting all requests to the configuration portal
//#include <ESP8266WebServer.h>   //Local WebServer used to serve the configuration portal
//#include <WiFiManager.h>        //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


#include <SimpleTimer.h>          //SimpleTimer
SimpleTimer timer;                //instantiate Simple Timer object

#include "DHTesp.h"               //DHT Library
DHTesp dhtInside;                 //instantiate DHT Inside temp object
DHTesp dhtOutside;                //instantiate DHT Inside temp object

#include <OneButton.h>            //Button library --- http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

#define BLYNK_PRINT Serial        /* Comment this out to disable prints and save space */
#define blynkPinOSAT V10          // Use Virtual pin 10 for outside air temp
#define OLED_RESET 4              //Wired to OLED reset pin to the this arduino GPIO pin // FOR GENERIC OLED WIRE TO RESET PIN 10KOHM RESISTOR TO VCC AND 22uF CAPACITOR TO GROUND

char auth[] = "ADD YOUR BLYNK AUTH TOKEN HERE"; // You should get Auth Token in the Blynk App. // Go to the Project Settings (nut icon).
char ssid[] = "ADD YOUR WIFI CREDENTIALS HERE";                          //WiFi Credentials
char pass[] = "ADD YOUR WIFI CREDENTIALS HERE";                       //WiFi Credentials

//############################################################################################
//-------------------------------------CONSTANTS----------------------------------------------
//############################################################################################
//const int ESP_BUILTIN_LED = 2;
//Available Pins for inputs--> 4, 15, 2 maybe but reserved for builtin_led
//Avoid using these Pins for inputs--> 0, 2, 12, 15
//    not used____GPIO 15                        D15 Avoid using this Pins for input
//    not used____GPIO 02                        D2  Avoid using this Pins for input (ESP_BUILTIN_LED)
//    GPIO 04                                    D4  Used for the Reset pin on the SSD1306 OLED
const int fanSpeedLoLo = 16;                  // RX2 [P4 @73V]  -  RED Wire to XFMR
const int solenoidOpen = 17;                  // TX2
const int solenoidClose = 5;                  // D5 is high on ESP32 during boot mode
//    not used____GPIO 18                        D18
//    not used____GPIO 19                        D19
//    GPIO 21                                    D21  Used for the SDA pin on the SSD1306 OLED (Pin assigned in Method)
//    not used____GPIO 03                        RX0  (EPS32 - High During Serial Flash / Boot Pins)
//    not used____GPIO 01                        TX0  (ESP32 - High During Serial Flash / Boot Pins)
//    GPIO 22                                    D22  Used for the SCL pin on the SSD1306 OLED (Pin assigned in Method)
const int resetCPU = 23;                      // D23

//    not used____GPIO 13                        D13
//    not used____GPIO 12                        D12
const int fanSpeedHiHi = 14;                  // D14 [P6 @120V] -  BLACK Wire to XFMR  --- (P5 is 0V and used as Nuetral)
const int fanSpeedHi = 27;                    // D27 [P3 @103V] -  BROWN Wire to XFMR
const int fanSpeedMed = 26;                   // D26 [P2 @93V]  -  YELLOW Wire to XFMR
const int fanSpeedLo = 25;                    // D25 [P1 @83V]  -  BLUE Wire to XFMR
//    TEMP SENSOR____GPIO 33                     D33 DHT22 Temperature Outside (Pin assigned in Method)
//    TEMP SENSOR____GPIO 32                     D32 DHT22 Temperature Inside (Pin assigned in Method)
//    not used____GPIO 35                        D35
const int pushButton =  34;                   // D3  -- pin that the Pushbutton is attached to on the KY-040 Rotary Encoder SW pin (Power During Flash)
const int encoderPin1 = 39;                   // [FUTURE FUNCTIONALITY] VN  -- pin that the KY-040 Rotary Encoder CLK pin is attached to(NEED INTERRUPT)
const int encoderPin2 = 36;                   // [FUTURE FUNCTIONALITY] VP  -- pin that the KY-040 Rotary Encoder DT pin is attached to (NEED INTERRUPT)

//############################################################################################
//-------------------------------------VARIABLES----------------------------------------------
//############################################################################################
//int tempVar = LOW;                          // ledState used to set the LED
int status = 2;                               // variable for storing the status of alarm and configuration states
bool blynkConnectionStatus = false;           // stores the status of connection to the Blynk server.
bool blynkTimeout = false;                    // stores the status if the Blynk connection timed out on retries.
bool blynkAppConnected = false;               // stores the status if the Blynk App is connected to the Arduino.
bool defrostModeLatch = false;                // will latch the the Defrost Control into a locked setpoint and not allow bouncing in/out of timed intervals.
bool ventilationModeHighOSAHLatch = false;    // will latch the the High Humidity interlock in the Ventilation mode from running.  Will reset once IDLE mode moves back to ventilation.
bool ventilationModeHighISAHLatch = false;    // will latch the the High Humidity interlock in the Ventilation mode from running.  Will reset once IDLE mode moves back to ventilation.
bool economizerInhibit = false;               // Will inhibit operation of the economizer if a high limit setpoint is exceeded.
String mode = "not set";                      // variable for storing the status of HRV states
String step = "-";                            // variable for storing the step of HRV.  Subset of the mode.  i.e. mode=Continous and step=(Ventilation or Idle)
String stepBlynk = "-";                       // variable for storing the step of HRV.  This will display extended details in Blynk app
int fanSpeedDisplay = 0;                      // variable for displaying the speed of the fan
int fanSpeedBlynk = 1;                        // variable for displaying the speed of the fan
int fanSpeedSetpoint = 20;                    // setpoint for controlling the speed of the fan
int ventilationRunTimeSetpoint = 20;          // MINUTES (Multiply x60,000)  Will store how long the step is allowed to run.  Use "unsigned long" for variables that hold time
int ventilationIdleTimeSetpoint = 40;         // MINUTES (Multiply x60,000)  Will store how long the step is allowed to run.  Use "unsigned long" for variables that hold time
int frostControlTimeSetpoint = 1;             // MINUTES (Multiply x60,000)  Will store how long the step is allowed to run.  Use "unsigned long" for variables that hold time
int highOccupancyTimeSetpoint = 8;            // HOURS (Multiply x3,600,000)  Will store how long the step is allowed to run.  Use "unsigned long" for variables that hold time
int economizerTargetSetpoint = 70;            // In Economizer mode this is the target temperature setpoint that the HRV will try to maintain.
int economizerSampleOneShot = 0;              // In Economizer mode this is the one shot bit for first pass sample counter..
int insideHumidityHighLimitSetpoint = 45;     // High Limit setpoint for humidity.
int outsideHumidityHighLimitSetpoint = 75;    // High Limit setpoint for humidity.
int outsideTempHighLimitSetpoint = 72;        // High Limit setpoint for temperature.
int centerStepText = 0;                       // This centers the step time remaining text in the exact center of OLED screen
int centerFanSpeedText = 0;                   // This centers the fan speed display text in the exact center of OLED screen
int solenoidState = 0;                        // Stores the state of the recirculation damper. 1023 = Blynk Full LED Brightness.
int rssi = 0;                                 // Stores the WiFi RSSi signal strength.
int wifiBars = 0;                             // Converts the signal strength from 0 to 5 bars
unsigned long ventilationElapsedRunTime = 0;  // will store how long the step has been running.  Use "unsigned long" for variables that hold time
unsigned long economizerRunTime = 0;          // In Economizer mode this is the 60 second startup delay timer to allow operation to draw a sample and ignore High Limit interlocks.
long economizerCountdownSample = 0;           // In Economizer mode this is the 60 minute countdown timer delay until the next sample is taken while inhibit mode.
unsigned long economizerInhibitTimestamp = 0; // In Economizer mode this is time stamp when the Free Cooling was inhibited.  Starts a 60 minute countdown timer.
unsigned long previousMillis = 0;             // will store last time event was triggered.  Use "unsigned long" for variables that hold time
unsigned long now = 0;                        // Present Time using millis() function.  Use "unsigned long" for variables that hold time
unsigned long blynkLastConnected = 0;         // Time that the Blynk connection was established.
unsigned long wifiStatus = 0;                 // variable for storing the wifi status
float osat = 2;                               // variable to store the value coming from the sensor
float osah = 2;                               // variable to store the value coming from the sensor
float osadp = 30;                             // variable to store the value coming from the sensor
float isat = 1;                               // variable to store the value coming from the sensor
float isah = 1;                               // variable to store the value coming from the sensor
float isadp = 0;                              // variable to store the value coming from the sensor
float stepTimeRemaining = 0;                  // MINUTES (Multiply x60,000)  The time remaining in the current step.
float blynkConnectionTime = 0;                // MINUTES (Multiply x60,000)  The time remaining in the current step.
int screenChange = 0;                         // This is the counter used to change the screen view on the OLED display.  0 = default (MAIN SCREEN)
int oneMinute = 60000;                        // 500 works best for rapid timing.  1000 good for closely watching steps and counters/
int oneHour = oneMinute * 60;                 // Created variable for debugging to speed up counter to cycle through modes
Adafruit_SSD1306 display (OLED_RESET);        //(OLED_RESET);  //was pin 2 before I remapped
OneButton button(pushButton, true);           // Setup a new OneButton on pin no. xx
int screenSaverDelay = 600000;                // Delay before screensaver becomes active
int screenSaverAccum = 0;                     // Accumulated time since last time a button was pressed
bool screenSaverIsActive = false;             // When TRUE the screen saver mode is active.

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// COUNTER VARIABLES FOR THE DEBUGGING SCREENS ON OLED.... EACH ROW HAS 5 COUNTER VALUES
int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
int counter4 = 0;
int counter5 = 0;
int counter6 = 0;
int counter7 = 0;
int counter8 = 0;
int counter9 = 0; 
int counter10 = 0;
int counter11 = 0;
int counter12 = 0;
int counter13 = 0;
int counter14 = 0;
int counter15 = 0;
int counter16 = 0;
int counter17 = 0;
int counter18 = 0;
int counter19 = 0;
int counter20 = 0;
