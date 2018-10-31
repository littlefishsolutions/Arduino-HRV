
void displayMain() {
  display.clearDisplay();
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = -10.2;
  //LINE 0 - LABELS
  int yline0 = 0; int xTextLeft = 5; int xTextMid = 85;
  //This is the top icon row
  blynkIcon();
  wifiBarsIcon();
  //Center the step ETM text always based on the decrementing value
  if ((stepTimeRemaining / oneMinute) < 10) {
    centerStepText = 100;
  }
  else if ((stepTimeRemaining / oneMinute) >= 10 && (stepTimeRemaining / oneMinute) < 100) {
    centerStepText = 95;
  }
  else if ((stepTimeRemaining / oneMinute) >= 100) {
    centerStepText = 90;
  }
  display.setCursor(centerStepText, 9); display.setTextColor(BLACK, WHITE); display.print(stepTimeRemaining / oneMinute, 1); display.print("m"); //Step Time Remaining
  //STEP and MODE display
  display.setTextSize(1);  display.setTextColor(WHITE);
  display.setCursor(0, yline0);
  display.println(step);
  display.print(mode);
  //LINE 1 - LABELS
  int yline1 = yline0 + 20;
  display.setTextSize(2);  display.setTextColor(WHITE); display.setCursor(xTextLeft - 2, yline1); display.print("IN");
  display.setCursor(xTextMid + 7, yline1); display.println("OUT");
  //LINE 2 - HUMIDITY
  int yline2 = yline1 + 20; display.setTextSize(1);  display.setTextColor(WHITE);
  display.setCursor(xTextLeft, yline2); display.print(isah, 1); display.print("%"); //Indoor Humidity
  display.setCursor(xTextMid, yline2); display.print(osah, 1); display.println("%"); //Outdoor Humidity
  //LINE 3 - TEMPERATURE
  int yline3 = yline2 + 10;
  display.setCursor(xTextLeft, yline3); display.print(isat, 1); display.print((char)247); display.print("F"); //Indoor Temperature
  display.setCursor(xTextMid, yline3); display.print(osat, 1); display.print((char)247); display.println("F"); //Outdoor Temperature
  //FAN SPEED
  if (fanSpeedDisplay < 10) {
    centerFanSpeedText = 51;
  }
  else if (fanSpeedDisplay >= 10 && fanSpeedDisplay < 100) {
    centerFanSpeedText = 44;
  }
  else if (fanSpeedDisplay >= 100 && fanSpeedDisplay < 1000) {
    centerFanSpeedText = 38;
  }
  display.setCursor(centerFanSpeedText, 19); display.setTextSize(2); display.setTextColor(BLACK, WHITE); display.print(fanSpeedDisplay); display.print("%"); //Fan Speed
  //DRAW BOXES AROUND SENSOR DATA
  display.drawLine(0, yline2 - 4, 127, yline2 - 4, WHITE); //top
  display.drawLine(0, yline3 + 10, 127, yline3 + 10, WHITE); //bottom
  display.drawLine(0, yline1 + 16, 0, yline3 + 10, WHITE); //left
  display.drawLine(127, yline1 + 16, 127, yline3 + 10, WHITE); //right
  display.display();
}
void displayStatus() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("STATUS");
  display.setTextSize(1);
  display.print("Status:"); display.println(status);
  display.print("Fan:   ");  display.print(fanSpeedDisplay); display.println(" %");
  display.print("Mode:  ");  display.println(mode);
  display.print("Step:  ");  display.println(step);
  display.print("Now: "); display.print(float(now)/1000); display.print("sec");
  display.display();
}
void displaySetpoints1() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("SETPOINTS1");
  display.setTextSize(1);
  display.print("Vent Run SP:    ");  display.print(ventilationRunTimeSetpoint); display.println("min");
  display.print("Fan Speed SP:  ");  display.print(fanSpeedSetpoint); display.println("min");
  display.print("Frost SP:       ");  display.print(frostControlTimeSetpoint); display.println("min");
  display.print("Hi Occ Duration : ");  display.print(highOccupancyTimeSetpoint); display.println("H");
  display.display();
}
void displaySetpoints2() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("SETPOINTS2");
  display.setTextSize(1);
  display.print("OSAH High Limit: ");  display.print(outsideHumidityHighLimitSetpoint); display.println("%");
  display.print("OSAT High Limit: ");  display.print(outsideTempHighLimitSetpoint); display.print((char)222); display.println("F");
  display.print("ISAH High Limit: ");  display.print(insideHumidityHighLimitSetpoint); display.println("%");
  display.print("Econ Target Temp: ");  display.print(economizerTargetSetpoint); display.println("F");
  display.display();
}
void displayTimers() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("TIMERS");
  display.setTextSize(1);
  display.print("Step: ");  display.println(step);
  display.print("NextStep:  ");  display.print(stepTimeRemaining / oneMinute, 1); display.println("min");
  display.print("PrvsMillis ");  display.println(previousMillis);
  display.print("Now:       ");  display.println(now);
  display.print("Step ETM:  ");  display.println(stepTimeRemaining, 0);
  display.print("Next Step: ");  display.print(stepTimeRemaining / oneMinute, 1); display.println("min");
  display.print("Mode: ");  display.println(mode);
  display.display();
}
void displayBlynkWifi() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("Blynk/WiFi");
  display.setTextSize(1);
  //display.print("WiFi Status: "); display.println(WiFi.status());
  display.print("WF:");
  display.print(WiFi.status() == 0 ? "WL_IDLE_STATUS" : "");
  display.print(WiFi.status() == 1 ? "WL_NO_SSID_AVAIL" : "");
  display.print(WiFi.status() == 2 ? "WL_SCAN_COMPLETED" : "");
  display.print(WiFi.status() == 3 ? "WL_CONNECTED" : "");
  display.print(WiFi.status() == 4 ? "WL_CONNECT_FAILED" : "");
  display.print(WiFi.status() == 5 ? "WL_CONNECTION_LOST" : "");
  display.print(WiFi.status() == 6 ? "WL_DISCONNECTED" : "");
  display.print(WiFi.status() == 255 ? "WL_NO_SHIELD" : "");
  display.println("");
  display.print("WiFi RSSI:    ");  display.print(WiFi.RSSI()); display.println("db");
  display.print("Blynk Connected: ");  display.println(blynkConnectionStatus == 1 ? "Yes" : "No");
  display.print("Blynk Timed Out: ");  display.println(blynkTimeout == 1 ? "Yes" : "No");
  display.print("BlynkUptime:");  display.print(blynkConnectionTime < 60 ? blynkConnectionTime : blynkConnectionTime/60, 1); display.println(blynkConnectionTime < 60 ? "m" : "h");
//  display.print("WiFi: "); display.print(counter1); display.print("  Blynk: "); display.println(counter2);
  display.display();
}
void displayDebug() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("Debug");
  display.setTextSize(1);
  display.println(step); display.setCursor(86, 16);
//  display.print(stepTimeRemaining / oneMinute > 60 ? stepTimeRemaining / oneMinute / 60 : stepTimeRemaining / oneMinute, 1); display.println(stepTimeRemaining / oneMinute > 60 ? "hr" : "mn"); //Convert to Hours if minutes are greater than 60.
//  display.print("HiHumLatch:");  display.print(ventilationModeHighOSAHLatch); display.print("  "); display.print(osah, 1); display.println("%"); //Outdoor Humidity
//  display.print(osah);  display.print("% >= ");  display.print(outsideHumidityHighLimitSetpoint); display.println("%");
//  display.print("temperature: "); display.println("%"); 
//  display.print("econRunTime: ");  display.println(economizerRunTime);
//  display.print("CountDn:");  display.print(economizerCountdownSample); display.print(" | "); display.println(economizerInhibitTimestamp);
//  display.print(counter1);  display.print("| ");  display.print(counter2);  display.print("| ");  display.print(counter3);  display.print("| ");  display.print(counter4);  display.print("| ");  display.println(counter5);
//  display.print(counter6);  display.print("| ");  display.print(counter7);  display.print("| ");  display.print(counter8);  display.print("| ");  display.print(counter9);  display.print("| ");  display.println(counter10);
//  display.print(counter11);  display.print("| ");  display.print(counter12);  display.print("| ");  display.print(counter13);  display.print("| ");  display.print(counter14);  display.print("| ");  display.println(counter15);
//  display.print(counter16);  display.print("| ");  display.print(counter17);  display.print("| ");  display.print(counter18);  display.print("| ");  display.print(counter19);  display.print("| ");  display.println(counter20);
    display.println("");
    display.print("Now: "); display.println(now);
    display.print("Sc Sav Acc: "); display.println(screenSaverAccum);
    display.print("Sc Sav Delay: "); display.println(screenSaverDelay);
    display.print("isActive: "); display.println(screenSaverIsActive);

  wifiBarsIcon();
  display.display();
}
void displayDebugI2C() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("Debug Temp");
  display.setTextSize(1);
  display.print("In: ");  display.print(isat, 1); display.print((char)247); display.print("F "); display.print(isah, 1); display.println("% ");
  display.print("    ");  display.print(isadp, 1);  display.print((char)247); display.println("F - DewPT"); display.println();
  display.print("Out: ");  display.print(osat, 1); display.print((char)247); display.print("F "); display.print(osah, 1); display.println("% ");
  display.print("     ");  display.print(osadp, 1);  display.print((char)247); display.println("F - DewPT"); 
  wifiBarsIcon();
  display.display();
}

void displayScreenSaver() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  //display.println("ScreenSaver Mode");
  display.setTextSize(1);
  wifiBarsIcon();
  display.display();
}

void displayResetCPU() {
  display.clearDisplay(); display.setCursor(0, 0); display.setTextSize(2); display.setTextColor(WHITE);
  display.println("RESET CPU?"); display.println();
  display.setTextSize(1);
  display.println("Press and hold the ");
  display.println("button for 5 seconds");
  display.println("on this page.");
  display.display();
}

void blynkIcon() { //Displays the Blynk icon in the upper right corner
  display.setCursor(90, 0); display.setTextSize(1);
  display.setTextColor(blynkConnectionStatus == 1 ? WHITE : BLACK , blynkConnectionStatus == 1 ? BLACK : WHITE); display.print("Blynk");
}
void wifiIcon() {
  display.setCursor(104, 0); display.setTextColor(WiFi.status() == 3 ? WHITE : BLACK , WiFi.status() == 3 ? BLACK : WHITE); display.print("WiFi");
}
void wifiBarsIcon() { //Displays the WiFi icon in the upper right corner
  //WIFI SIGNAL STRENGTH
  if(wifiStatus != WL_CONNECTED){
    return;
  }
  rssi = WiFi.RSSI();
  if (rssi >= -55) {
    wifiBars = 5;                                    // 5. High quality: 90% ~= -55db
    display.drawLine(119    , 6, 119    , 5, WHITE); //One Bar
    display.drawLine(119 + 2, 6, 119 + 2, 4, WHITE); //Two Bars
    display.drawLine(119 + 4, 6, 119 + 4, 3, WHITE); //Three Bars
    display.drawLine(119 + 6, 6, 119 + 6, 2, WHITE); //Four Bars
    display.drawLine(119 + 8, 6, 119 + 8, 1, WHITE); //Five Bars
  } else if (rssi < -55 & rssi >= -65) {
    wifiBars = 4;                                    // 4. Good quality: 75% ~= -65db
    display.drawLine(119    , 6, 119    , 5, WHITE); //One Bar
    display.drawLine(119 + 2, 6, 119 + 2, 4, WHITE); //Two Bars
    display.drawLine(119 + 4, 6, 119 + 4, 3, WHITE); //Three Bars
    display.drawLine(119 + 6, 6, 119 + 6, 2, WHITE); //Four Bars
  } else if (rssi < -65 & rssi >= -75) {
    wifiBars = 3;                                    // 3. Medium quality: 50% ~= -75db
    display.drawLine(119    , 6, 119    , 5, WHITE); //One Bar
    display.drawLine(119 + 2, 6, 119 + 2, 4, WHITE); //Two Bars
    display.drawLine(119 + 4, 6, 119 + 4, 3, WHITE); //Three Bars
  } else if (rssi < -75 & rssi >= -85) {
    wifiBars = 2;                                    // 2. Low quality: 30% ~= -85db
    display.drawLine(119    , 6, 119    , 5, WHITE); //One Bar
    display.drawLine(119 + 2, 6, 119 + 2, 4, WHITE); //Two Bars
  } else if (rssi < -85 & rssi >= -96) {
    wifiBars = 1;                                    // 1. Unusable quality: 8% ~= -96db
    display.drawLine(119    , 6, 119    , 5, WHITE); //One Bar
  } else {
    wifiBars = 0;                                    // 0. No signal
  }
}
