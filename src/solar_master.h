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
File fh,fd,fe;
Ticker secondTick;
volatile int watchDog = 0;

const uint16_t HTML_SIZE = 10000;
const uint8_t NTP_PACKET_SIZE = 48;
const uint8_t BUFFER_SIZE = 128;
const uint8_t TIME_ZONE = 10;
char fileName[] = "/XXyymmdd.csv";
char todayName[] = "/XXyymmdd.csv";
char date15Back[] = "/XXyymmdd.csv";
char userText[] = "/XXyymmdd.csv";
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
char htmlStr[HTML_SIZE];
uint8_t byteBuf[NTP_PACKET_SIZE];
char charBuf[BUFFER_SIZE];
uint8_t oldMin,oldQtr,oldHour,oldDay,oldMonth;


char ssid[] = "TelstraCF6EC7";
char pass[] = "meauff8qqwn9";
char d2Str[] = "12";
char d8Str[] = "12345.78";

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned long importWh = 0;
unsigned int localPort = 2395;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP,fileServerIP;
IPAddress ip(192, 168, 1, 52);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);
const char* ntpServerName = "au.pool.ntp.org";
const char* ftpServerName = "ftp.sowter.com";
bool invert = false;  												// invert logic on serial interface
bool invReply = false;												// inverter has replied
bool dayStored = false;												// days energy stored in totalNRG.csv
const uint8_t BUF_SIZ = 128;									// software serial buffer size
SoftwareSerial mySerial(5,4,invert,BUF_SIZ); 	// TX=D1=IO5, RX=D2=IO4 (Wemos mini)
// notes: gnd=1=orange, rx=2=black, tx=3=yellow

uint8_t outStr1[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			// request inverter reconnect
uint8_t outStr2[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x40, 0x00, 0xFE, 0x45, 0x0A, 0x0D };			// is anybody out there?
uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x32, 0x32, 0x31, 0x33, 0x31, 0x31, 0x31,
												0x30, 0x30, 0x30, 0x36, 0x20, 0x20, 0x20, 0x11, 0xFB, 0x3B, 0x0A, 0x0D };	// assign address 0x11
uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x11, 0x31, 0x42, 0x00, 0xFE, 0x31, 0x0A, 0x0D };			// request data from 0x11
uint8_t inStr[60];
int sampleCount=0, zeroCount=0;

float pvMinuteMax, pvMinuteMin, pvMinuteAvg, pvPower, pvMin, pvMax, pvSum = 0.0;
float pvQtrMax,pvQtrMin,qtrEnergy,thisTotal;
float pvInvTemp,pvVolts1,pvVolts2,pvAmps1,pvAmps2,acVolts,acFrequency;
double pvEnergyTotal,pvETLast,thisEnergyToday,prevEnergyToday=0.0,sumEnergyToday=0.0;
