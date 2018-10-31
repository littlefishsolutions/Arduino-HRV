void doubleClick() { 
    screenSaverAccum = millis();
}


void singleClick() {
  if (screenChange < 8) {
    screenChange++; 
  }
  else {
    screenChange = 0;
  }
  screenSaverAccum = millis();
}


void longPress() {
  if(screenChange != 8){ //If the OLED page is on the Reset CPU page then do not change the status.
    status++;
  }
  else{
    digitalWrite(resetCPU, LOW);  //Set the ResetPIN low so that it doesn't latch in a CPU reset on the EN pin.
  }
  previousMillis = now;
  ventilationElapsedRunTime = 0;
  if (status > 4) {
    status = 1;
  }
  screenSaverAccum = millis();
}
