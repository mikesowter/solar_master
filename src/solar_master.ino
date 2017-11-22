#include <solar_master.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 1.0 20171122");
	Serial.print("\n\nConnecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, pass);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("local IP address: ");
	localIP=WiFi.localIP();
	Serial.print(localIP);
	long rssi = WiFi.RSSI();
	Serial.print("   signal strength: ");
	Serial.print(rssi);
	Serial.println(" dBm");
	init_OTA();

	udp.begin(localPort);
	// Resolve servers
	WiFi.hostByName(ntpServerName, timeServerIP);
	WiFi.hostByName(ftpServerName, fileServerIP);
	// Set epoch and timers
	getTime();
	setTime(startSeconds);

	// set the data rate for the SoftwareSerial port
	mySerial.begin(9600);
	mySerial.write(outStr1,11);
	delay(100);
	mySerial.write(outStr2,11);
	delay(100);
	mySerial.write(outStr3,28);
	delay(500);
	int i = 0;
	Serial.print("bytes available:");
	Serial.println(mySerial.available());
	{
		uint8_t X = mySerial.read();
		printHex(X);
		inStr[i++] = X;
	}
	Serial.print("   ");
	Serial.print(i);
	Serial.println(" chars");

	dateStamp();

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
	mySerial.write(outStr4, 11);
	delay(300);
	int bytes=mySerial.available();
	if (bytes>0) {
		Serial.print("bytes available:");
		Serial.println(mySerial.available());
		int i = 0;
		while (mySerial.available()>0) {
			uint8_t X = mySerial.read();
			printHex(X);
			inStr[i++] = X;
		}
		pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
		pvVolts1 = (256 * inStr[9] + inStr[10]) / 10.0;
		pvVolts2 = (256 * inStr[11] + inStr[12]) / 10.0;
		pvAmps1 = (256 * inStr[13] + inStr[14]) / 10.0;
		pvAmps2 = (256 * inStr[15] + inStr[16]) / 10.0;
		pvEnergyToday = (256 * inStr[17] + inStr[18]) / 100.0;

		pvPower = (256 * inStr[25] + inStr[26]) / 10.0;
	}

	// check for change of minute
	if (oldMin == minute())
	{
		if (pvPower > pvPowerMax) pvPowerMax = pvPower;
		if (pvPower < pvPowerMin) pvPowerMin = pvPower;
		pvPowerAvg += pvPower;
		sampleCount++;
	}
	else				//new minute
	{
		oldMin = minute();
		if (sampleCount == 0) pvPowerAvg = 0;
		else pvPowerAvg /= sampleCount;
		
		Serial.print(timeStamp());
		printFloat("Temp = ", pvInvTemp);
		printFloat("PV1 = ", pvVolts1);
		printFloat("PV2 = ", pvVolts2);
		printFloat("PA1 = ", pvAmps1);
		printFloat("PA2 = ", pvAmps2);
		printFloat("Pmin = ", pvPowerMin);
		printFloat("Pavg = ", pvPowerAvg);
		printFloat("Pmax = ", pvPowerMax);
		printFloat("E today = ", pvEnergyToday);
		printFloat("samples = ", sampleCount);
		Serial.println();

		pvPowerMax = 0;
		pvPowerMin = 9999;
		pvPowerAvg = 0;
		sampleCount = 0;
	}

}

void printHex(uint8_t x)
{
	if (x<16) Serial.print('0');
	Serial.print(x, HEX);
	Serial.print(' ');
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
