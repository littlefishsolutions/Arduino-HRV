//    ___  __          __
//   / _ )/ /_ _____  / /__
//  / _  / / // / _ \/  '_/
// /____/_/\_, /_//_/_/\_\
//        /___/  on ESP8266
//BLYNK requires that <100 commands per second can be sent.
//If you send more than 100 values per second - you may cause Flood Error and your hardware will be automatically disconnected from the server;

BLYNK_WRITE(V0) { //MODE SELECTION FROM BYLNK
  status = param.asInt(); // Get value as integer
  previousMillis = now;
  stepTimeRemaining = 0;
}
BLYNK_WRITE(V2) {  //FAN SPEED SETPOINT FROM BLYNK
  fanSpeedBlynk = param.asInt(); // Get value as integer
  switch (fanSpeedBlynk) { //Translate Blynk 1-5 values to the fanSpeedSetpoint 20-100% values
    case 1:
      fanSpeedSetpoint = 20;
      break;
    case 2:
      fanSpeedSetpoint = 40;
      break;
    case 3:
      fanSpeedSetpoint = 60;
      break;
    case 4:
      fanSpeedSetpoint = 80;
      break;
    case 5:
      fanSpeedSetpoint = 100;
      break;
  }
}
BLYNK_WRITE(V20) { //SETPOINT Ventilation Run Time (10-50 min)
  ventilationRunTimeSetpoint = param.asInt(); // Get value as integer
}
BLYNK_WRITE(V21) { //SETPOINT Economizer Target Temperature (60-85 Deg F)
  economizerTargetSetpoint = param.asInt(); // Get value as integer
}
BLYNK_WRITE(V22) { //SETPOINT High Occupancy Duration Time (1-12 hours)
  highOccupancyTimeSetpoint = param.asInt(); // Get value as integer
}
BLYNK_WRITE(V23) { //SETPOINT High Limit setpoint for humidity. (25-80%)
  outsideHumidityHighLimitSetpoint = param.asInt(); // Get value as integer
}
BLYNK_WRITE(V24) { //SETPOINT High Limit setpoint for Temperature. (65-90 Deg F)
  outsideTempHighLimitSetpoint = param.asInt(); // Get value as integer
}
BLYNK_WRITE(V25) { //SETPOINT High Limit setpoint for humidity. (0-40%)
  insideHumidityHighLimitSetpoint = param.asInt(); // Get value as integer
}
BLYNK_READ(12) {// This function tells Arduino what to do if there is a Widget which is requesting data for Virtual Pin (5)
  Blynk.virtualWrite(0, status);                              // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  // 1 =
  Blynk.virtualWrite(2, fanSpeedBlynk);                       // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(3, step);                                // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(4, solenoidState);                       // Recirculation Damper Open 1=Recirculation (Frost Control)
  Blynk.virtualWrite(5, fanSpeedDisplay);                     // Diplays the scaled value of fan speed from 0 - 100
  Blynk.virtualWrite(10, osat);                               // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(11, osah);                               // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(12, isat);                               // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(13, isah);                               // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(14, stepTimeRemaining / 60000);          // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  //15
  Blynk.virtualWrite(16, isadp);                              // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(17, osadp);                              // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  // 18
  // 19
  Blynk.virtualWrite(20, ventilationRunTimeSetpoint);         //SETPOINT Ventilation Run Time (10-50 min)
  Blynk.virtualWrite(21, economizerTargetSetpoint);           //SETPOINT Economizer Target Temperature (60-85 Deg F)
  Blynk.virtualWrite(22, highOccupancyTimeSetpoint);          //SETPOINT High Occupancy Duration Time (1-12 hours)
  Blynk.virtualWrite(23, outsideHumidityHighLimitSetpoint);   //SETPOINT High Limit setpoint for humidity. (25-80%)
  Blynk.virtualWrite(24, outsideTempHighLimitSetpoint);       //SETPOINT High Limit setpoint for Temperature. (65-90 Deg F)
  Blynk.virtualWrite(25, insideHumidityHighLimitSetpoint);    //SETPOINT High Limit setpoint for humidity. (0-40%)
  // 26 BLYNK_WRITE (see below) = SETPOINT
  // 27 BLYNK_WRITE (see below) = SETPOINT
  // 28 BLYNK_WRITE (see below) = SETPOINT
  // 29 BLYNK_WRITE (see below) = SETPOINT
  // 30 BLYNK_WRITE (see below) = SETPOINT
  // 31 BLYNK_WRITE (see below) = SETPOINT
  // 32 DO NOT USE ANY VALUES HIGHER THAN 32 -- DOES NOT WRITE FOR SOME REASON!!!!!
}
