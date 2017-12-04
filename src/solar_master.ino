#include <solar_master.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 1.0 20171203");
	// join local network and internet
	joinNet();
	// setup over the air updates
	init_OTA();
	// setup watch dog
	secondTick.attach(1,ISRwatchDog);
	// Set epoch and timers
	setupTime();
	// connect to solar inverter
	setupInv();
	// setup server responses
	server.on ( "/", handleMetrics );
	server.on ( "/dir", handleDir );
	server.on ( "/metrics", handleMetrics );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println( "HTTP server started" );
	server.handleClient();
	//if(!SPIFFS.format()||!SPIFFS.begin())     //use to format SPIFFS drive
	if(!SPIFFS.begin())
	{
		Serial.println("SPIFFS.begin failed");
	}
	SPIFFS.info(fs_info);
	Serial.print(fs_info.totalBytes);
	Serial.println(" bytes available");
	Serial.print(fs_info.usedBytes);
	Serial.println(" bytes used:");

//	ThingSpeak.begin(client);
}

void loop()
{
	// query inverter and wait for response
	queryInv();
	// check for change of minute
	if (oldMin == minute())
	{
		if (pvPower > pvPowerMax) pvPowerMax = pvPower;
		if (pvPower < pvPowerMin) pvPowerMin = pvPower;
		pvPowerAvg += pvPower;
		sampleCount++;
	}
	else minProc();
	// reset watchdog
	watchDog=0;
	// check for OTA
  ArduinoOTA.handle();
	// check for web request
	server.handleClient();
}

void printFloat(char* mess,float f)
{
	Serial.print(mess);
	long i = f;
	Serial.print(i);
	Serial.print('.');
	int j = ( f-(float)i ) * 10.0;
	Serial.print(j);
	Serial.print('/');
}

void ISRwatchDog () {
  noInterrupts();
  watchDog++;
	if (watchDog == 30) {
    diagMess("watchDog 30s alert");
  }
  if (watchDog >= 60) {
    diagMess("watchDog 60s timeout");
//    fh.close();
    ESP.restart();
  }
  interrupts();
}
