void handleRoot() {
/*  htmlStr[0]='\0';
  addCstring( htmlStr0 );
  addCstring( "'timeofday'" );
  addCstring( htmlStr1 );
  add3PowerData();
  addCstring( htmlStr2 );
  add3TitleData();
  addCstring( htmlStr3 );
  addErrMess();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );  */
  //Serial.println(htmlStr);
}

void handleMetrics() {
  htmlStr[0]='\0';
  addCstring("# TYPE pvInvTemp guage" );
  addCstring("\npvInvTemp ");
  addCstring(p8d(pvInvTemp));
  addCstring("\n# TYPE pvVolts1 guage" );
  addCstring("\npvVolts1 ");
  addCstring(p8d(pvVolts1));
  addCstring("\n# TYPE pvVolts2 guage" );
  addCstring("\npvVolts2 ");
  addCstring(p8d(pvVolts2));
  addCstring("\n# TYPE pvAmps1 guage" );
  addCstring("\npvAmps1 ");
  addCstring(p8d(pvAmps1));
  addCstring("\n# TYPE pvAmps2 guage" );
  addCstring("\npvAmps2 ");
  addCstring(p8d(pvAmps2));
  addCstring("\n# TYPE pvPowerMin guage" );
  addCstring("\npvPowerMin ");
  addCstring(p8d(pvPowerMin));
  addCstring("\n# TYPE pvPowerAvg guage" );
  addCstring("\npvPowerAvg ");
  addCstring(p8d(pvPowerAvg));
  addCstring("\n# TYPE pvPowerMax guage" );
  addCstring("\npvPowerMax ");
  addCstring(p8d(pvPowerMax));
  addCstring("\n# TYPE pvEnergyToday guage" );
  addCstring("\npvEnergyToday ");
  addCstring(p8d(pvEnergyToday));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
  //Serial.println(htmlStr);
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);
  Serial.print(timeStamp());
  Serial.println(userText);
/*  if (strncmp(userText,"/reset",6)==0) {
    errMessage("User requested restart");
    fd.close();
    fe.close();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>User requested restart<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    ESP.restart();
  }
  else if (strncmp(userText,"/diags",6)==0) {
    listDiags();
  }
  else if (strncmp(userText,"/remdiags",9)==0) {
    SPIFFS.remove("/diags.txt");
    fd = SPIFFS.open("/diags.txt", "a+");
    fd.println(dateStamp());
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Diags deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/filesave",9)==0) {
    diagMess("User requested file save");
    uploadDay();
    uploadMonth();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Safe to Shutdown<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (SPIFFS.exists(userText)) {
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Sending File: \"");
    strcat(outBuf,userText);
    strcat(outBuf,"\"<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    strcpy(fileName,userText);
    uploadFile();
    delay(5);
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else if (strncmp(userText,"/dir",4)==0) {
    listFiles();
  }
  else {
    strcpy(outBuf,userText);
    strcat(outBuf," is not a valid option");
    diagMess(outBuf);
    helpPage();
  }  */
}

uint8_t listDiags() {
/*  char line[81];
  htmlStr[0]='\0';
  fd.seek(0UL,SeekSet);
  while (fd.available()) {
    int k=fd.readBytesUntil('\r',line,80);
    line[k]='\0';
    addCstring(line);
    addCstring("\n");
  }
  fd.print("length of diag list:");
  fd.println(htmlLen);
  server.send ( 200, "text/plain", htmlStr );
  return 1; */
}

void addCstring(char* s) {
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

float scaleCheck(uint16_t n) {
  float f = (float)n/100.0;
  if ( f == 0.0 ) {
    return 25.0;
  }
  else if ( f > 40.0) return 40.0;
  else if ( f < 0.0) return 0.0;
  return f;
}
