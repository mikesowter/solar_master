#include <solar_master.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 1.0 20171129");
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
	server.on ( "/metrics", handleMetrics );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println( "HTTP server started" );
	server.handleClient();

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
