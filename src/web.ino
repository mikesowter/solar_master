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
  Serial.print(userText);

  if (SPIFFS.exists(userText)) {
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>Sending File: \"");
    strcat(charBuf,userText);
    strcat(charBuf,"\"<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
    strcpy(fileName,userText);
    if ( !openFile("r") ) return;
    Serial.println(":");
    while (fh.available()) {
      Serial.println(fh.readStringUntil('\n'));
      yield();
    }
  fh.close();
  }
  else Serial.println(" is not a valid option");
//  }
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
