#include <arduino.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <fs.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

ESP8266WebServer server ( 80 );
WiFiUDP udp;
WiFiClient client,dclient;
time_t getNtpTime();
FSInfo fs_info;
File fh;          // logs, diagnostics and errors
Ticker secondTick;
volatile int watchDog = 0;

char fileName[] = "/XXyymmdd.csv";
char todayName[] = "/XXyymmdd.csv";
char date15Back[] = "/XXyymmdd.csv";
char userText[] = "/XXyymmdd.csv";
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
char errMess[5][60];
unsigned long getTime();
unsigned long sendNTPrequest(IPAddress& address);
unsigned long getNTPreply();

char ssid[] = "TelstraCF6EC7";
char pass[] = "meauff8qqwn9";
char d2Str[] = "12";
char d8Str[] = "12345.78";

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned long importWh = 0;
unsigned int localPort = 2395;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP,fileServerIP;
const char* ntpServerName = "au.pool.ntp.org";
const char* ftpServerName = "ftp.sowter.com";

SoftwareSerial mySerial(10, 9); // RX, TX


uint8_t outStr1[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			// request inverter reconnect
uint8_t outStr2[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x45, 0x0A, 0x0D };			// is anybody out there?
uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x32, 0x32, 0x31, 0x33, 0x31, 0x31, 0x31,
						0x30, 0x30, 0x30, 0x36, 0x20, 0x20, 0x20, 0x11, 0xFB, 0x3B, 0x0A, 0x0D };	// assign address 0x11
uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x11, 0x31, 0x42, 0x00, 0xFE, 0x31, 0x0A, 0x0D };			// request data from 0x11
uint8_t inStr[60];
uint8_t oldSec=60;
int sampleCount;

float powerMax, powerMin, powerAv;
