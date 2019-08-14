#include <arduino.h>
#include <fs.h>
#include <ESP8266WebServer.h>
#include "functions.h"

extern float pvMinuteMax, pvMinuteMin, pvMinuteAvg, pvPower;
extern float pvQtrMax, pvQtrMin, qtrEnergy, pvEnergyToday;
extern float pvInvTemp, pvVolts1, pvVolts2, pvAmps1, pvAmps2, acVolts, acFrequency;
extern double pvEnergyTotal,thisTotal,pvETLast,thisEnergyToday,prevEnergyToday,sumEnergyToday;
extern char longStr[],fileSizeStr[],fileName[],userText[],charBuf[];
extern ESP8266WebServer server;
extern uint32_t fileSize, pvHours, lastScan;
extern File fd, fe;
extern uint16_t longStrLen;

uint8_t storeData();

void handleMetrics() {
  longStr[0]='\0';
  addCstring("# TYPE pvInvTemp guage" );
  addCstring("\npvInvTemp ");
  addCstring(f8ds(pvInvTemp));
  addCstring("\n# TYPE pvVolts1 guage" );
  addCstring("\npvVolts1 ");
  addCstring(f8ds(pvVolts1));
//  addCstring("\n# TYPE pvVolts2 guage" );
//  addCstring("\npvVolts2 ");
//  addCstring(f8ds(pvVolts2));
  addCstring("\n# TYPE pvAmps1 guage" );
  addCstring("\npvAmps1 ");
  addCstring(f8ds(pvAmps1));
//  addCstring("\n# TYPE pvAmps2 guage" );
//  addCstring("\npvAmps2 ");
//  addCstring(f8ds(pvAmps2));
  addCstring("\n# TYPE pvPower guage" );
  addCstring("\npvPower ");
  addCstring(f8ds(pvPower));
  addCstring("\n# TYPE pvMinuteMin guage" );
  addCstring("\npvMinuteMin ");
  addCstring(f8ds(pvMinuteMin));
  addCstring("\n# TYPE pvMinuteAvg guage" );
  addCstring("\npvMinuteAvg ");
  addCstring(f8ds(pvMinuteAvg));
  addCstring("\n# TYPE pvMinuteMax guage" );
  addCstring("\npvMinuteMax ");
  addCstring(f8ds(pvMinuteMax));
  addCstring("\n# TYPE pvEnergyToday guage" );
  addCstring("\npvEnergyToday ");
  addCstring(f8ds(sumEnergyToday+thisEnergyToday));
//  addCstring(f8ds(pvEnergyToday));
  addCstring("\n# TYPE pvEnergyTotal guage" );
  addCstring("\npvEnergyTotal ");
  addCstring(f8ds(pvEnergyTotal));
  addCstring("\n# TYPE pvHours guage" );
  addCstring("\npvHours ");
  addCstring(f8ds((float)pvHours));
  addCstring("\n# TYPE acVolts guage" );
  addCstring("\nacVolts ");
  addCstring(f8ds(acVolts));
  addCstring("\n# TYPE acFrequency guage" );
  addCstring("\nacFrequency ");
  addCstring(f8ds(acFrequency));
  addCstring("\n# TYPE pvWifiSignal guage" );
  addCstring("\npvWifiSignal ");
  addCstring(f8ds(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", longStr );
  lastScan = millis();
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
    diagMess("diags deleted");
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
  else if (strncmp(userText,"/reset",5)==0) {
    fd.close();
    fe.close();
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>Reset requested<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
    storeData();
    ESP.restart();
  }
  else {
    Serial.print(timeStamp());
    Serial.print(userText);
    Serial.println(" is not a valid option");
  }
//  }
}

void addCstring(const char* s) {
  // find end of longStr
  uint16_t p;
  for (p=0;p<longStrLen;p++) {
    if ( p>longStrLen-32) {
      diagMess("longStrLen exceeded");
      break;
    }
    if (longStr[p]=='\0') {
      break;    // p now points to end of old string
    }
  }
  uint16_t q=0;
  for (;p<longStrLen;p++) {
    longStr[p]=s[q];
    if (s[q++]=='\0') break;
  }
}
