void CheckConnections(){
  
  //CHECK WIFI CONNECTION STATUS
  if(wifiStatus != WL_CONNECTED) {
//    digitalWrite(wifiConnectedLed, LOW);
    WiFi.begin(ssid, pass); // wait 10 seconds for connection:
    counter1 ++;
  }
  else {
//    digitalWrite(wifiConnectedLed, HIGH);
  } 
}
