
//         __        ______    ______   _______
//        |  \      /      \  /      \ |       \ 
//        | $$     |  $$$$$$\|  $$$$$$\| $$$$$$$\
//        | $$     | $$  | $$| $$  | $$| $$__/ $$
//        | $$     | $$  | $$| $$  | $$| $$    $$
//        | $$     | $$  | $$| $$  | $$| $$$$$$$
//        | $$_____| $$__/ $$| $$__/ $$| $$
//        | $$     \\$$    $$ \$$    $$| $$
//         \$$$$$$$$ \$$$$$$   \$$$$$$  \$$
void loop() {  

//~~~~~~~~~~~~MANAGE WiFi CONNECTION
  timer.run(); //SimpleTimer
  wifiStatus = WiFi.status();


//~~~~~~~~~~~~MANAGE BLYNK CONNECTION
  blynkConnectionStatus = Blynk.connected();
  if (Blynk.connected()) {
//    digitalWrite(blynkConnectedLed, HIGH);
    blynkLastConnected = millis();  // Time that the Blynk connection was established;
    blynkConnectionTime = (float(blynkLastConnected) / float(oneMinute));
    //ADD THIS CODE  https://community.blynk.cc/t/check-connection-status-in-loop-and-reconect/7651/68
  }
  else {
//    digitalWrite(blynkConnectedLed, LOW);
    counter2 ++;
    //BlynkReconnect();
  }
    Blynk.run();  // run Blynk activities
  
  ArduinoOTA.handle();
  button.tick(); //Always watch the pushbutton
  now = millis();
    osat =  dhtOutside.toFahrenheit(dhtOutside.getTemperature());  //Smoothing --> osah = .8 * osah + .2 * (40 - analogRead(analogPin) / 11.00)
    osah =  dhtOutside.getHumidity();
    osadp = dhtOutside.computeDewPoint(osat, osah, 1);
    isat =  dhtInside.toFahrenheit(dhtInside.getTemperature());
    isah =  dhtInside.getHumidity();
    isadp = dhtInside.computeDewPoint(isat, isah, 1);

  //-----------------------NEED ALL SETPOINTS INSIDE LOOP SO THAT BLYNK CAN UPDATE VALUES
  ventilationIdleTimeSetpoint = 60 - ventilationRunTimeSetpoint;
  if (status != 4) {
    economizerSampleOneShot = 0;
  }

  //SCREEN CHANGES
  if(now - screenSaverAccum > screenSaverDelay){
      screenSaverIsActive = true;
      screenChange = 9;
      displayScreenSaver();
  }
  else{
      screenSaverIsActive = false;
  }
  switch (screenChange) { //MANAGE SCREEN CHANGE from double click
    case 0:
      displayMain();
      break;
    case 1:
      displayStatus();
      break;
    case 2:
      displaySetpoints1();
      break;
    case 3:
      displaySetpoints2();
      break;
    case 4:
      displayTimers();
      break;
    case 5:
      displayBlynkWifi();
      break;
    case 6:
      displayDebug();
      break;
    case 7:
      displayDebugI2C();
      break;
    case 8:
      displayResetCPU();
      break;
  }
  
  
  switch (status) {
    //    ____   ____ ____
    //   / __ \ / __// __/
    //  / /_/ // _/ / _/
    //  \____//_/  /_/
    case 1:
      mode = "OFF";
      step = "";
      previousMillis = now;
      stepTimeRemaining = 0;
      setFanSpeed0();
      recirculationDamperClose();
      break;
    //    _____ ____   _  __ ______ ____ _  __ __  __ ____   __  __ ____
    //   / ___// __ \ / |/ //_  __//  _// |/ // / / // __ \ / / / // __/
    //  / /__ / /_/ //    /  / /  _/ / /    // /_/ // /_/ // /_/ /_\ \  
    //  \___/ \____//_/|_/  /_/  /___//_/|_/ \____/ \____/ \____//___/
    case 2:
      mode = "Continuous";
      ventilationElapsedRunTime = now - previousMillis;
      //                        __  __ ____  __  _  _____  _  _      ____  _____  _  ____  __  _
      //                        \ \/ /| ===||  \| ||_   _|| || |__  / () \|_   _|| |/ () \|  \| |
      //                         \__/ |____||_|\__|  |_|  |_||____|/__/\__\ |_|  |_|\____/|_|\__|
      if (ventilationElapsedRunTime < (ventilationRunTimeSetpoint * oneMinute) && step != "Idle") {
        //ventilationElapsedRunTime = now - previousMillis;
        stepTimeRemaining = ventilationRunTimeSetpoint * oneMinute - ventilationElapsedRunTime;
        checkHighLimitOutsideHumidity(); //This will one-shot latch the Humidity into high limit until the idle mode resets this latch. OSAT > Setpoint
        checkHighLimitInsideHumidity(); //This will one-shot latch the Humidity into high limit until the idle mode resets this latch. ISAT > Setpoint
        counter8 += 1;

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~DEFROST MODE (RECIRCULATION DAMPER) -- LATCH~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (ventilationElapsedRunTime >= oneMinute &&
            ventilationElapsedRunTime <= (oneMinute + (frostControlTimeSetpoint * oneMinute)) &&
            (osat <= 20 || defrostModeLatch == true) &&
            ventilationModeHighOSAHLatch == false &&
            ventilationModeHighISAHLatch == false) {
              frostControl();  //  INSIDE THIS FUNCTION..... LATCH TO PREVENT TEMP BOUNCE AROUND THRESHOLD AND TOGGLE IN/OUT OF MODES!!!
              counter9 += 1;
            }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~HIGH OSAH HUMIDITY (VENTILATION LOCKOUT) -- LATCH~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (ventilationElapsedRunTime <= ventilationRunTimeSetpoint * oneMinute &&
                 ventilationElapsedRunTime >= oneMinute &&
                 ventilationModeHighOSAHLatch == true) { // ventilationModeHighOSAHLatch
          setFanSpeed0();
          step = "High OSAH";
          counter10 += 1;
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~HIGH ISAH HUMIDITY (Dehumidify) -- LATCH~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (ventilationElapsedRunTime <= ventilationRunTimeSetpoint * oneMinute &&
                 ventilationElapsedRunTime >= oneMinute &&
                 ventilationModeHighISAHLatch == true) { // ventilationModeHighISAHLatch
          setFanSpeed100();
          step = "High ISAH";
           counter11 += 1;
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~UNINHIBITED CONTINOUS VENTILATION (NO INTERLOCKS)~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (ventilationElapsedRunTime < ventilationRunTimeSetpoint * oneMinute && ventilationModeHighOSAHLatch == false && ventilationModeHighISAHLatch == false) {
          if(ventilationElapsedRunTime < oneMinute) {
            step = "Sampling";
          }
          else {
            step = "Ventilation";
          }
          defrostModeLatch = false;
          recirculationDamperClose();
          setFanSpeed();
          counter12 += 1;
        }
      }
      //                         _  ____  _     ____
      //                        | || _) \| |__ | ===|
      //                        |_||____/|____||____|
      else if (ventilationElapsedRunTime >= (ventilationRunTimeSetpoint * oneMinute) && ventilationElapsedRunTime < (ventilationRunTimeSetpoint + ventilationIdleTimeSetpoint) * oneMinute) {
        if (stepTimeRemaining == 0) {
          previousMillis = now;
        }
        //ventilationElapsedRunTime = now - previousMillis;
        stepTimeRemaining = ((ventilationRunTimeSetpoint + ventilationIdleTimeSetpoint) * oneMinute) - ventilationElapsedRunTime;
        step = "Idle";
        setFanSpeed0();
        recirculationDamperClose();
      }
      //This step moves from IDLE back into the VENTILATION state
      else if (ventilationElapsedRunTime > (ventilationRunTimeSetpoint + ventilationIdleTimeSetpoint) * oneMinute) { //&& step == "Idle") {
        previousMillis = now;
        //ventilationElapsedRunTime = now - previousMillis;
        step = "Ventilation";
        ventilationModeHighOSAHLatch = false;
        ventilationModeHighISAHLatch = false;
      }

      break; // exits the case 2 statement of CONTINOUS
    //     __ __ _        __     ____
    //    / // /(_)___ _ / /    / __ \ ____ ____ __ __ ___  ___ _ ___  ____ __ __
    //   / _  // // _ `// _ \  / /_/ // __// __// // // _ \/ _ `// _ \/ __// // /
    //  /_//_//_/ \_, //_//_/  \____/ \__/ \__/ \_,_// .__/\_,_//_//_/\__/ \_, /
    //           /___/                              /_/                   /___/
    case 3:
      if (ventilationElapsedRunTime <= highOccupancyTimeSetpoint * 3600000 && ventilationModeHighOSAHLatch == false) {
        checkHighLimitOutsideHumidity(); //This will one-shot latch the Humidity into high limit until the idle mode resets this latch.
        ventilationElapsedRunTime = now - previousMillis;
        mode = "High Occupancy";
        step = "";
        stepTimeRemaining = (highOccupancyTimeSetpoint) * 3600000 - ventilationElapsedRunTime;
        setFanSpeed100();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~DEFROST MODE (RECIRCULATION DAMPER) -- LATCH~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (ventilationElapsedRunTime >= oneMinute &&
            ventilationElapsedRunTime <= (oneMinute + (frostControlTimeSetpoint * oneMinute)) &&
            (osat <= 20 || defrostModeLatch == true) &&
            ventilationModeHighOSAHLatch == false) {
          frostControl();  //  INSIDE THIS FUNCTION..... LATCH TO PREVENT TEMP BOUNCE AROUND THRESHOLD AND TOGGLE IN/OUT OF MODES!!!
          //setFanSpeed100();

        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~HIGH HUMIDITY (VENTILATION LOCKOUT) -- LATCH~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (ventilationElapsedRunTime <= ventilationRunTimeSetpoint * oneMinute &&
                 ventilationElapsedRunTime >= oneMinute &&
                 ventilationModeHighOSAHLatch == true) { // ventilationModeHighOSAHLatch
          setFanSpeed0();
          step = "High OSAH";
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~UNINHIBITED CONTINOUS VENTILATION (NO INTERLOCKS)~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     /*   else if (ventilationElapsedRunTime < ventilationRunTimeSetpoint * oneMinute && ventilationModeHighOSAHLatch == false) {
          step = "Ventilation";
          defrostModeLatch = false;
          recirculationDamperClose();
          setFanSpeed();
        }*/
      }
      else {
        status = 2;
        previousMillis = now;
        ventilationModeHighOSAHLatch = false;
        //step = "resetHighOccupancy";
      }
      break;
    //     ___    _        ____                              _
    //    / _ |  (_)____  / __/____ ___   ___  ___   __ _   (_)___ ___  ____
    //   / __ | / // __/ / _/ / __// _ \ / _ \/ _ \ /  ' \ / //_ // -_)/ __/
    //  /_/ |_|/_//_/   /___/ \__/ \___//_//_/\___//_/_/_//_/ /__/\__//_/
    case 4:
      if (economizerSampleOneShot == 0) {
        previousMillis = now;
      }
      economizerSampleOneShot += 1;
      mode = "Air Economizer";
      counter1++;
      // HIGH LIMIT SETPOINTS insideHumidityHighLimitSetpoint, outsideHumidityHighLimitSetpoint, outsideTempHighLimitSetpoint, economizerTargetSetpoint
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~FREE COOLING MODE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if ((osat < outsideTempHighLimitSetpoint &&
           osah < outsideHumidityHighLimitSetpoint &&
           isat > economizerTargetSetpoint &&
           economizerInhibit == false) ||
          economizerRunTime < oneMinute) {
        step = "Free Cooling";
        setFanSpeed();
        counter2++;
        economizerInhibitTimestamp = now + oneHour;  // Always stamps time so that when this mode is inhibited the last time is stamped.
      }
      /* if (step == "Free Cooling") {
         economizerRunTime = now - previousMillis;
        }
        if (step != "Free Cooling") {
         economizerRunTime = 0;
        }*/
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~AC MODE (High OSAT)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if (osat >= outsideTempHighLimitSetpoint) {
        economizerInhibit = true;
        step = "AC(High OSAT)";
        setFanSpeed0();
        counter3++;
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~AC MODE (High OSAH)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if (osah >= outsideHumidityHighLimitSetpoint) {
        economizerInhibit = true;
        step = "AC(High OSAH)";
        setFanSpeed0();
        counter4++;
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~AC MODE (Temperature Satisfied)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if (isat <= economizerTargetSetpoint) {
        economizerInhibit = true;
        step = "FreeCool@Temp";
        setFanSpeed0();
        counter5++;
      }

      // Inhibit modes and timer accumlators
      if (economizerInhibit == false) {
        economizerRunTime = now - previousMillis;
        stepTimeRemaining = 0;
        counter6++;
      }
      else {
        economizerCountdownSample = economizerInhibitTimestamp - now;
        stepTimeRemaining = float(economizerCountdownSample); // float(oneHour);
        counter7++;
        if (economizerCountdownSample < 0) {
          economizerInhibit = false;
          economizerRunTime = 0;
          previousMillis = now;
        }
      }
      break;
    default: // if nothing else matches, do the default // default is optional
      break;

  } // End of Switch Case statement
} // END OF LOOP

