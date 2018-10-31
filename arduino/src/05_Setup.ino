//          ______   ________  ________  __    __  _______
//         /      \ |        \|        \|  \  |  \|       \ 
//        |  $$$$$$\| $$$$$$$$ \$$$$$$$$| $$  | $$| $$$$$$$\
//        | $$___\$$| $$__       | $$   | $$  | $$| $$__/ $$
//         \$$    \ | $$  \      | $$   | $$  | $$| $$    $$
//         _\$$$$$$\| $$$$$      | $$   | $$  | $$| $$$$$$$
//        |  \__| $$| $$_____    | $$   | $$__/ $$| $$
//         \$$    $$| $$     \   | $$    \$$    $$| $$
//          \$$$$$$  \$$$$$$$$    \$$     \$$$$$$  \$$
void setup() {
digitalWrite(resetCPU, HIGH);  //Set the ResetPIN low so that it doesn't latch in a CPU reset on the EN pin.

  
  //  Serial.begin(152000);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INITIALIZE DHT22 SENSOR 
  delay(1000);  //Need this delay inside Setup loop so that on power loss I have a one-shot delay to read the temperature from the sensors otherwise I return minimum values.
  dhtInside.setup(32);  //DHT22 sensor ESP32 PinsWork  3,15,32,33    DontWork=1,2,13  // Put 10kOhm resistor on Data to Vcc for longer legnth comms
  dhtOutside.setup(33); //DHT22 sensor ESP32 PinsWork  3,15,32,33    DontWork=1,2,13  // Put 10kOhm resistor on Data to Vcc for longer legnth comms
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INITIALIZE OLED DISPLAY (ADAFRUIT address = 0x3D, GENERICALIEXPRESS address = 0x3C)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64) -- by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.clearDisplay();  // Clear the buffer.

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BOOTUP SCREEN
  display.clearDisplay(); display.setCursor(10, 0); display.setTextSize(2); display.setTextColor(BLACK, WHITE);  display.println("-Booting-");
  display.setTextSize(1);  display.setTextColor(WHITE); display.println("");
  display.setCursor(25, 25); display.print("Waiting for");
  display.setCursor(20, 35); display.print("WiFi and Blynk");
  display.setCursor(25, 45); display.print("to connect");
  display.display();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~WIFI
  WiFi.mode(WIFI_STA);
  //timer.setInterval(10000, wifiReconnect);  //Used for wifiReconnection() method for dropped connections
  timer.setInterval(10000, CheckConnections);  //Used for wifiReconnection() method for dropped connections

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BLYNK SERVER
  Blynk.begin(auth, ssid, pass);
  //while (Blynk.connect() == false) {
  // step = "connecting";
  //}
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ARDUINO OTA (OVER THE AIR) BOOTLOADER
  ArduinoOTA.setHostname("HRV (esp32)"); // OPTIONAL
  //ArduinoOTA.setPassword("123"); // No authentication by default
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  /* OneButton.Library--------------------Methods and Functions (KEYWORD2)
  CAUTION:  THE USE OF DELAYS IN YOUR CODE MAKES THE BUTTON CLICKS SOMETIMES NON-RESPONSIVE.  THE WORST IS DOUBLE CLICK.
  I modified the OneButton.CPP library variables for the following
        _debounceTicks      from 50  -> 30  ---------  This is the debounce mechanism
        _clickTicks = 100   from 600 -> 100 ---------  This is the single click action
        _pressTicks = 1000                  ---------  This is the long press action

  setClickTicks  KEYWORD2___________Default value of 600 millisec that have to pass by before a click is detected.
  setPressTicks KEYWORD2____________Default value of 1000 millisec that have to pass by before a long button press is detected.
  attachClick KEYWORD2
  attachDoubleClick KEYWORD2
  attachLongPressStart  KEYWORD2____After button is held for 1 seconds will trigger
  attachLongPressStop KEYWORD2______Hold button for 1 seconds and after release will trigger
  attachDuringLongPress KEYWORD2____After button is held for 1 seconds will trigger event every scan until button is relased
  isLongPressed KEYWORD2
  tick  KEYWORD2  */
  button.attachClick(singleClick);  // link the myClickFunction function to be called on a click event.
  button.attachDoubleClick(doubleClick);    // link the doubleclick function to be called on a doubleclick event.
  button.attachLongPressStart(longPress);  //Enter config mode.... removed the (now - previousMillisPB) since the time since last PB press didn't work.

  
  //-----------------------PIN ASSIGNMENTS
  pinMode(pushButton, INPUT_PULLUP); // set the digital pin as input: USE THE INTERNAL PULL-UP RESISTOR SINCE I AM USING THE PCB AS A GROUND PB.
  pinMode(fanSpeedLoLo, OUTPUT);
  pinMode(fanSpeedLo, OUTPUT);
  pinMode(fanSpeedMed, OUTPUT);
  pinMode(fanSpeedHi, OUTPUT);
  pinMode(fanSpeedHiHi, OUTPUT);
  pinMode(solenoidClose, OUTPUT);
  pinMode(solenoidOpen, OUTPUT);
  pinMode(resetCPU, OUTPUT);
//  pinMode(blynkConnectedLed, OUTPUT);
//  pinMode(wifiConnectedLed, OUTPUT);
}

