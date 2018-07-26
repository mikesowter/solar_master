void handleMetrics() {
  htmlStr[0]='\0';
  addCstring("# TYPE pvInvTemp guage" );
  addCstring("\npvInvTemp ");
  addCstring(p8d(pvInvTemp));
  addCstring("\n# TYPE pvVolts1 guage" );
  addCstring("\npvVolts1 ");
  addCstring(p8d(pvVolts1));
//  addCstring("\n# TYPE pvVolts2 guage" );
//  addCstring("\npvVolts2 ");
//  addCstring(p8d(pvVolts2));
  addCstring("\n# TYPE pvAmps1 guage" );
  addCstring("\npvAmps1 ");
  addCstring(p8d(pvAmps1));
//  addCstring("\n# TYPE pvAmps2 guage" );
//  addCstring("\npvAmps2 ");
//  addCstring(p8d(pvAmps2));
  addCstring("\n# TYPE pvPower guage" );
  addCstring("\npvPower ");
  addCstring(p8d(pvPower));
  addCstring("\n# TYPE pvMinuteMin guage" );
  addCstring("\npvMinuteMin ");
  addCstring(p8d(pvMinuteMin));
  addCstring("\n# TYPE pvMinuteAvg guage" );
  addCstring("\npvMinuteAvg ");
  addCstring(p8d(pvMinuteAvg));
  addCstring("\n# TYPE pvMinuteMax guage" );
  addCstring("\npvMinuteMax ");
  addCstring(p8d(pvMinuteMax));
  addCstring("\n# TYPE pvEnergyToday guage" );
  addCstring("\npvEnergyToday ");
//  addCstring(p8d(sumEnergyToday+thisEnergyToday));
  addCstring(p8d(pvEnergyToday));
  addCstring("\n# TYPE pvEnergyTotal guage" );
  addCstring("\npvEnergyTotal ");
  addCstring(p8d(pvEnergyTotal));
  addCstring("\n# TYPE pvHours guage" );
  addCstring("\npvHours ");
  addCstring(p8d((float)pvHours));
  addCstring("\n# TYPE acVolts guage" );
  addCstring("\nacVolts ");
  addCstring(p8d(acVolts));
  addCstring("\n# TYPE acFrequency guage" );
  addCstring("\nacFrequency ");
  addCstring(p8d(acFrequency));
  addCstring("\n# TYPE pvWifiSignal guage" );
  addCstring("\npvWifiSignal ");
  addCstring(p8d(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
  //Serial.println(htmlStr);
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);

  if (SPIFFS.exists(userText)) {
    listFile();
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else if (strncmp(userText,"/deldiags",9)==0) {
    fd.close();
    SPIFFS.remove("/diags.txt");
    fd = SPIFFS.open("/diags.txt", "a");
    fd.println(dateStamp());
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>Diagnostics deleted<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
  }
  else if (strncmp(userText,"/delerrs",8)==0) {
    fe.close();
    SPIFFS.remove("/errmess.txt");
    fe = SPIFFS.open("/errmess.txt", "a");
    fe.println(dateStamp());
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>Error Messages deleted<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
  }
  else {
    Serial.print(timeStamp());
    Serial.print(userText);
    Serial.println(" is not a valid option");
  }
//  }
}

void addCstring(const char* s) {
  // find end of htmlStr
  uint16_t p;
  for (p=0;p<HTML_SIZE;p++) {
    if ( p>HTML_SIZE-32) {
      diagMess("HTML_SIZE exceeded");
      break;
    }
    if (htmlStr[p]=='\0') {
      break;    // p now points to end of old string
    }
  }
  uint16_t q=0;
  for (;p<HTML_SIZE;p++) {
    htmlStr[p]=s[q];
    if (s[q++]=='\0') break;
  }
}
