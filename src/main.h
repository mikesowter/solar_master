// an attempt at a generic include file, with minimal globals

#include <arduino.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266FTPServer.h>
#include <fs.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

String resetReason = "Restart cause: " + ESP.getResetReason();
String resetDetail = ESP.getResetInfo();

ESP8266WebServer server ( 80 );
FtpServer ftpSrv;
time_t getNtpTime();
FSInfo fs_info;
File fh,fd,fe;
Ticker secondTick;
volatile int watchDog = 0;

bool invReply = false;												// inverter has replied
bool firstPass = true;												// reset after inverter first replies
bool dayStored = false;												// days energy stored in totalNRG.csv

char fileName[] = "/XXyymmdd.csv";
char fileSizeStr[10];
char todayName[] = "/XXyymmdd.csv";
char userText[] = "/XXyymmdd.csv";
uint16_t longStrLen = 10000;
char longStr[10000];
char charBuf[128];
char dateStr[7];
char timeStr[10];

float pvMax = 0.0, pvMin = 0.0, pvSum = 0.0;
extern float pvPower;

uint8_t oldMin, oldQtr, oldHour, oldDay, oldMonth;
int16_t sampleCount = 0;
uint32_t fileSize, pvHours, secsSinceRestart;
uint32_t t0, t1, minMillis, startMillis, lastScan;
