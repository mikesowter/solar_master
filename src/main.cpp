// a rebuild of solar master with better modularity

#include "main.h"
#include "functions.h"

void setup() {
	Serial.begin(57600);
	Serial.println("\n\rSolar Master Rev 3.2 20200401");
	// join local network and internet
	joinNet();
	// setup over the air updates
	init_OTA();
	// setup watch dog
	secondTick.attach(1,ISRwatchDog);
	// Set epoch and timers
	setupTime();
	// setup server responses
	setupServer();
	// setup file system and diag files
	setupSPIFFS();
	// setup FTP server
	ftpSrv.begin("mike","iron");
	// lookup reason for restart
	resetReason.toCharArray(charBuf,resetReason.length()+1);
	diagMess(charBuf);       // restart message
	resetDetail.toCharArray(charBuf,resetDetail.length()+1);
	if ( charBuf[16] != '0' )	diagMess(charBuf);       		
	// read yesterday's TotalEnergy
	readTotal();
}

void loop() {
	// check inverter comms
	if ( !invReply ) setupInv();    // timeout after 2 secs
	// query inverter and wait for response
	if ( invReply ) queryInv();
	// check for end of solar day
	if ( invReply || !dayStored ) {
		// check for first pass
		if ( firstPass ) {
      Serial.println("\nfirstPass\n");
			pvMax = pvPower;
			pvMin = pvPower;
			pvSum += pvPower;
			firstPass = false;
		}
		if ( pvPower > pvMax ) pvMax = pvPower;
		if ( pvPower < pvMin ) pvMin = pvPower;
		pvSum += pvPower;
	}
  // check background
	if ( !invReply ) watchWait(30000);
  else watchWait(5000);
  // check for change of minute
  if ( minute() != oldMin ) minProc();
  // check prometheus
  checkScan();
}

void ISRwatchDog () {
  watchDog++;
  if (watchDog >= 120) {
    errMess("Watch Dog 120s");
    fd.close();
		fe.close();
    ESP.restart();
  }
}

void checkScan() {
  if (  millis()-lastScan > 200000UL ) {
    diagMess("Prometheus 3m scan fail");
    // rejoin local network if necessary
    if (WiFi.status() != WL_CONNECTED) {
      errMess("\nnetwork disconnected - rebooting\n");
      fd.close();
		  fe.close();
      ESP.restart();
    }
  }
}

void watchWait(uint32_t timer) {
  t0 = millis();
  Serial.print("*");
  while (millis()-t0 < timer) {   // wait for timeout
    if (t0 > millis()) t0 = 0;    // check for wrap around
    yield();
    // reset watchdog
    watchDog = 0;
    //  check for web requests
    server.handleClient();
    // check for OTA
    ArduinoOTA.handle();
    // check for FTP request
		ftpSrv.handleFTP();
  }
}

