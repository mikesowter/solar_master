void setupTime() {
  startSeconds=getTime();
  setTime(startSeconds);
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  t0 = millis();
  oldMin = minute();
  oldHour = hour();
  oldDay = day();
  oldMonth = month();

  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf," restart ");
  Serial.println(charBuf);
  strcpy(todayName,"/PR");
  strcat(todayName,dateStamp());
  strcat(todayName,".csv");
}
