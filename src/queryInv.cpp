#include <arduino.h>
#include <TimeLib.h>
#include "protocol.h"
#include "functions.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

extern float pvMinuteMax, pvMinuteMin, pvMinuteAvg, pvPower;
extern float pvQtrMax, pvQtrMin, qtrEnergy, pvEnergyToday;
uint8_t inStr[128], pvFault, prevFault;
extern SoftwareSerial mySerial;	
extern bool invReply,dayStored;
extern uint32_t fileSize, pvHours;
extern char charBuf[];
extern int16_t sampleCount;
extern volatile int watchDog;

double pvEnergyTotal,thisTotal,pvETLast,thisEnergyToday,prevEnergyToday,sumEnergyToday;
float pvInvTemp, pvVolts1, pvVolts2, pvAmps1, pvAmps2, acVolts, acFrequency;

void queryInv() {
  for (int j=0; j<8; j++) {       // 8 attempts to connect
    mySerial.write(outStr4, 11);	// query inverter
    watchWait(1000);
	  if ( mySerial.available() == 52 ) {
      readBytes( false );
      if ( goodCheckSum(52) ) {
        invReply = true;
    		pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
  //      pvEnergyToday = (256 * inStr[9] + inStr[10]) / 100.0;
        thisEnergyToday = (256 * inStr[9] + inStr[10]) / 100.0;
        if (thisEnergyToday<prevEnergyToday) sumEnergyToday += prevEnergyToday;
        prevEnergyToday = thisEnergyToday;
    		pvVolts1 = 0.9*pvVolts1 + (256 * inStr[11] + inStr[12]) / 100.0;
    		pvAmps1 = 0.9*pvAmps1 + (256 * inStr[13] + inStr[14]) / 100.0;
        acVolts = (256 * inStr[15] + inStr[16]) / 10.0;
        acFrequency = (256 * inStr[17] + inStr[18]) / 100.0;
        pvPower = (256 * inStr[19] + inStr[20]) ;
  //      pvEnergyTotal = (256.0 * 256.0 * (double)inStr[24] + 256.0 * (double)inStr[25] + (double)inStr[26]) / 10.0;
        pvHours = (256 * inStr[29] + inStr[30]);
        pvFault = inStr[32];
        if ( pvFault > 1 && pvFault != prevFault ) {
          strcpy(charBuf,"Inverter Fault: ");
          strcat(charBuf,i2sd(pvFault));
          errMess(charBuf);
          prevFault = pvFault;
        }
        if ( pvFault == 1 ) prevFault = 1;
      }
      sampleCount++;
      return;

/* old inverter
      pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
  		pvVolts1 = (256 * inStr[9] + inStr[10]) / 10.0;
  		pvVolts2 = (256 * inStr[11] + inStr[12]) / 10.0;
  		pvAmps1 = 0.9*pvAmps1+(float)(256 * inStr[13] + inStr[14]) / 100.0;
  		pvAmps2 = 0.9*pvAmps2+(float)(256 * inStr[15] + inStr[16]) / 100.0;
  		thisEnergyToday = (256 * inStr[17] + inStr[18]) / 100.0;
      if (thisEnergyToday<prevEnergyToday) sumEnergyToday += prevEnergyToday;
      prevEnergyToday = thisEnergyToday;
      acVolts = (256 * inStr[21] + inStr[22]) / 10.0;
      acFrequency = (256 * inStr[23] + inStr[24]) / 100.0;
  		pvPower = (256 * inStr[25] + inStr[26]) / 10.0;
      return;   */
	  }
    else while (mySerial.available()>0) {   // flush buffer
      mySerial.read();
      yield();
    }
    sprintf(charBuf,"scan attempt %d failed",j);
    diagMess(charBuf);
  } // 8 attempts failed
  invReply = false;
  diagMess("no reply");
  if ( hour()>=16 && !dayStored ) {
    if (!updateTotal()) diagMess("updateTotal failed");
    else dayStored = true;
  }
  for (int i=0;i<10;i++) {    // wait 5 minutes before trying to reconnect
    watchDog = 0;
    watchWait(30000);
  }
  pvInvTemp = 0.0;
  acFrequency = 50.0;
}

bool goodCheckSum(uint8_t len) {
  uint16_t sum = 0;
  for (int i=0;i<len-3;i++) sum += inStr[i];
  sum += inStr[len-3]*256;  // 2 checksum bytes
  sum += inStr[len-2];      // followed by 0A,0D
  return sum == 0;
}
