#include <main.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 2.3 20180728");
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
	// lookup reason for restart
	resetReason.toCharArray(charBuf,resetReason.length()+1);
	diagMess(charBuf);       // restart message
	// read yesterday's TotalEnergy
	if (!readTotal()) diagMess("readTotal failed");
}

void loop()
{
	// check inverter comms
	if ( invReply==false ) setupInv();
	// query inverter and wait for response
	queryInv();
	// check for end of solar day
	if ( invReply || !dayStored ) {
		// check for change of minute
		if ( oldMin==minute() || firstPass )
		{
			if (pvPower > pvMax) pvMax = pvPower;
			if (pvPower < pvMin) pvMin = pvPower;
			pvSum += pvPower;
			firstPass = false;
		}
		else minProc();
		// reset watchdog
		watchDog=0;
		// check for OTA
	  ArduinoOTA.handle();
		// check for web request
		server.handleClient();
	}
}

void ISRwatchDog () {
  noInterrupts();
  watchDog++;
  if (watchDog >= 600) {
    errMess("watchDog");
    fd.close();
		fe.close();
    ESP.restart();
  }
  interrupts();
}
