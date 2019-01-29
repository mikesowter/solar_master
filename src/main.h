#include <arduino.h>
#include "secrets.h"
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266FtpServer.h>
#include <WiFiUdp.h>
#include <fs.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

const uint16_t HTML_SIZE = 10000;
const uint8_t NTP_PACKET_SIZE = 48;
const uint8_t BUFFER_SIZE = 128;
const uint8_t TIME_ZONE = 10;
const char* ntpServerName = "au.pool.ntp.org";
const uint8_t SS_BUFFER = 128;									// software serial buffer size
const uint8_t outStr1[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			// request inverter disconnect
const uint8_t outStr2[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x40, 0x00, 0xFE, 0x45, 0x0A, 0x0D };			// is anybody out there?
/* old inverter
const uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x32, 0x32, 0x31, 0x33, 0x31, 0x31, 0x31,
												0x30, 0x30, 0x30, 0x36, 0x20, 0x20, 0x20, 0x11, 0xFB, 0x3B, 0x0A, 0x0D };	// assign address 0x11
const uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x11, 0x31, 0x42, 0x00, 0xFE, 0x31, 0x0A, 0x0D };			// request data from 0x11
new inverter */
const uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x31, 0x33, 0x31, 0x35, 0x30, 0x35, 0x31,
												0x30, 0x30, 0x33, 0x35, 0x20, 0x20, 0x20, 0x01, 0xFB, 0x44, 0x0A, 0x0D };	// assign address 0x01
const uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x01, 0x31, 0x42, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			// request data from 0x01

String resetReason = ESP.getResetReason();
SoftwareSerial mySerial(5,4,false,SS_BUFFER); 	// TX=D1=IO5, RX=D2=IO4 (Wemos mini)
ESP8266WebServer server ( 80 );
FtpServer ftpSrv;
WiFiUDP udp;
WiFiClient client,dclient;
time_t getNtpTime();
FSInfo fs_info;
File fh,fd,fe;
Ticker secondTick;
volatile int watchDog = 0;

IPAddress localIP,timeServerIP,fileServerIP;
IPAddress ip(192, 168, 1, 52);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

bool invert = false;  												// invert logic on serial interface
bool invReply = false;												// inverter has replied
bool firstPass = true;												// reset after inverter first replies
bool dayStored = false;												// days energy stored in totalNRG.csv

char fileName[] = "/XXyymmdd.csv";
char fileSizeStr[10];
char todayName[] = "/XXyymmdd.csv";
char date15Back[] = "/XXyymmdd.csv";
char userText[] = "/XXyymmdd.csv";
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
char htmlStr[HTML_SIZE];
char charBuf[BUFFER_SIZE];
char d2Str[] = "12";
char d8Str[] = "12345.78";

double pvEnergyTotal,thisTotal,pvETLast,thisEnergyToday,prevEnergyToday=0.0,sumEnergyToday=0.0;

float pvMax = 0.0, pvMin = 0.0, pvSum = 0.0;
float pvMinuteMax, pvMinuteMin, pvMinuteAvg, pvPower;
float pvQtrMax,pvQtrMin,qtrEnergy,pvEnergyToday;
float pvInvTemp,pvVolts1,pvVolts2,pvAmps1,pvAmps2,acVolts,acFrequency;

uint8_t byteBuf[NTP_PACKET_SIZE];
uint8_t oldMin,oldQtr,oldHour,oldDay,oldMonth;
uint8_t inStr[128],pvFault, prevFault=1;
int16_t sampleCount=0, timeSlipSecs;
uint32_t fileSize, pvHours, secsSinceRestart;
uint32_t t0, t1, minMillis, startMillis, startSeconds, midNight, lastScan;
uint32_t importWh = 0;
uint32_t localPort = 2395;   //  a random local port to listen for UDP packets
