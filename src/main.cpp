// a rebuild of solar master with better modularity

#include "main.h"
#include "functions.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 3.1 20190813");
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
	if (!readTotal()) diagMess("readTotal failed");
}

void loop()
{
	// check inverter comms
	if ( !invReply ) setupInv();
	// query inverter and wait for response
	queryInv();
	// check for end of solar day
	if ( invReply || !dayStored ) {
		// check for first pass
		if ( firstPass ) {
			pvMax = pvPower;
			pvMin = pvPower;
			pvSum += pvPower;
			firstPass = false;
		}
		if ( pvPower > pvMax ) pvMax = pvPower;
		if ( pvPower < pvMin ) pvMin = pvPower;
		pvSum += pvPower;
		// check for change of minute
		if ( minute() != oldMin ) minProc();
		// reset watchdog
		watchDog = 0;
		// check for OTA
	  ArduinoOTA.handle();
		// check for web request
		server.handleClient();
		// check for FTP request
		ftpSrv.handleFTP();
    // check prometheus scan
    void checkScan();
	}
}

void ISRwatchDog () {
  watchDog++;
  if (watchDog >= 60) {
    errMess("watchDog 60s");
    fd.close();
		fe.close();
    ESP.restart();
  }
}

void checkScan() {
  if (  millis()-lastScan > 150000UL) {
    diagMess("Prometheus 2m scan fail");
    // rejoin local network if necessary
    if (WiFi.status() != WL_CONNECTED) joinNet();
  }
}

void watchWait(uint32_t timer) {
  t0 = millis();
  while (millis()-t0 < timer) {  // wait for timeout
    if (t0 > millis()) t0 = millis(); // check for wrap around
    yield();
    //  check for web requests
    server.handleClient();
    // check for OTA
    ArduinoOTA.handle();
    // check for FTP request
		ftpSrv.handleFTP();
  }
}

