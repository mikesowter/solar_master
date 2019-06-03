void setupTime() {
  startSeconds=getTime();
  setTime(startSeconds);
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  t0 = millis();
  oldMin = minute();
  oldQtr = oldMin/15;
  oldHour = hour();
  oldDay = day();
  oldMonth = month();

  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  Serial.println(charBuf);
  strcpy(todayName,"/pv");
  strcat(todayName,dateStamp());
  strcat(todayName,".csv");
}

void watchWait(uint32_t timer) {
  t0 = millis();
  while (millis()-t0 < timer) {  // wait for timeout
    if (t0>millis()) t0=millis(); // check for wrap around
    yield();
    //  check for web requests
    server.handleClient();
    // check for OTA
    ArduinoOTA.handle();
    // check for FTP request
		ftpSrv.handleFTP();
  }
}

void dayCheck() {
  if (oldDay == day()) return;
  delay(2000);   //wait 2s to clear midNight reliably
//  secsSinceRestart = now()-startSeconds;
//  midNight = now();
  setupTime();
//  t1 = millis()-t0;     // ms taken to chat with NTP server
//  timeSlipSecs = now()-midNight-t1/1000;
//  sprintf(charBuf,"%d seconds drift in %d \n",timeSlipSecs,secsSinceRestart);
//  diagMess(charBuf);
  dayStored = false;
  prevEnergyToday = 0.0;
  sumEnergyToday = 0.0;
  return;
}
