void frostControl() {
  counter13++;
  step = "Frost Control";
  setFanSpeed100();
  recirculationDamperOpen();
  if (osat >= 20 && defrostModeLatch == false) {
    frostControlTimeSetpoint = 0;
  }
  else if (osat >= 10 && osat < 20 && defrostModeLatch == false) {
    frostControlTimeSetpoint = 1;
  }
  else if (osat >= 0 && osat < 10 && defrostModeLatch == false) {
    frostControlTimeSetpoint = 2;
  }
  else if (osat >= -10 && osat < 0 && defrostModeLatch == false) {
    frostControlTimeSetpoint = 3;
  }
  else if (osat < -10 && defrostModeLatch == false) {
    frostControlTimeSetpoint = 5;
  }
  defrostModeLatch = true; // Latches state after first scan and will not allow changes to setpoint.
}
void recirculationDamperOpen() {
  digitalWrite(solenoidOpen, HIGH);
  digitalWrite(solenoidClose, LOW);
  solenoidState = 1023;
}
void recirculationDamperClose() {
  digitalWrite(solenoidOpen, LOW);
  digitalWrite(solenoidClose, HIGH);
  solenoidState = 0;
}
void checkHighLimitOutsideHumidity() {
  if (osah >= outsideHumidityHighLimitSetpoint && ventilationElapsedRunTime > oneMinute) { //This will one-shot latch the Humidity into high limit until the idle mode resets this latch.
    ventilationModeHighOSAHLatch = true;
  }
}
void checkHighLimitInsideHumidity() { // DEHUMIDIFY
  if (isah >= insideHumidityHighLimitSetpoint && ventilationElapsedRunTime > oneMinute && (osah + 1) < isah) { //This will one-shot latch the Humidity into high limit until the idle mode resets this latch.
    ventilationModeHighISAHLatch = true;
    counter15++;
  }
}
void setFanSpeed() {
  counter14++;
  if (fanSpeedSetpoint == 0) {
    setFanSpeed0();
  }
  else if (fanSpeedSetpoint == 20) {
    setFanSpeed20();
  }
  else if (fanSpeedSetpoint == 40) {
    setFanSpeed40();
  }
  else if (fanSpeedSetpoint == 60) {
    setFanSpeed60();
  }
  else if (fanSpeedSetpoint == 80) {
    setFanSpeed80();
  }
  else if (fanSpeedSetpoint == 100) {
    setFanSpeed100();
  }
}
void setFanSpeed0() {
  digitalWrite(fanSpeedLoLo, LOW);
  digitalWrite(fanSpeedLo, LOW);
  digitalWrite(fanSpeedMed, LOW);
  digitalWrite(fanSpeedHi, LOW);
  digitalWrite(fanSpeedHiHi, LOW);
  fanSpeedDisplay = 0;
}
void setFanSpeed20() {
  digitalWrite(fanSpeedLo, LOW);
  digitalWrite(fanSpeedMed, LOW);
  digitalWrite(fanSpeedHi, LOW);
  digitalWrite(fanSpeedHiHi, LOW);
  delay (100);
  digitalWrite(fanSpeedLoLo, HIGH);
  fanSpeedDisplay = 20;
  counter16++;
}
void setFanSpeed40() {
  digitalWrite(fanSpeedLoLo, LOW);
  digitalWrite(fanSpeedMed, LOW);
  digitalWrite(fanSpeedHi, LOW);
  digitalWrite(fanSpeedHiHi, LOW);
  delay (100);
  digitalWrite(fanSpeedLo, HIGH);
  fanSpeedDisplay = 40;
  counter17++;
}
void setFanSpeed60() {
  digitalWrite(fanSpeedLoLo, LOW);
  digitalWrite(fanSpeedLo, LOW);
  digitalWrite(fanSpeedHi, LOW);
  digitalWrite(fanSpeedHiHi, LOW);
  delay (100);
  digitalWrite(fanSpeedMed, HIGH);
  fanSpeedDisplay = 60;
  counter18++;
}
void setFanSpeed80() {
  digitalWrite(fanSpeedLoLo, LOW);
  digitalWrite(fanSpeedLo, LOW);
  digitalWrite(fanSpeedMed, LOW);
  digitalWrite(fanSpeedHiHi, LOW);
  delay (100);
  digitalWrite(fanSpeedHi, HIGH);
  fanSpeedDisplay = 80;
  counter19++;
}
void setFanSpeed100() {
  digitalWrite(fanSpeedLoLo, LOW);
  digitalWrite(fanSpeedLo, LOW);
  digitalWrite(fanSpeedMed, LOW);
  digitalWrite(fanSpeedHi, LOW);
  delay (100);
  digitalWrite(fanSpeedHiHi, HIGH);
  counter14 += counter14;
  fanSpeedDisplay = 100;
  counter20++;
}
