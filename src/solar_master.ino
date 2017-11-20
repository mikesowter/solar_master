#include <solar_master.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 1.0 20171120");
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

	server.on ( "/", handleRoot );
	server.on ( "/metrics", handleMetrics );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println( "HTTP server started" );
	server.handleClient();

	ThingSpeak.begin(client);
}

void loop()
{
	mySerial.write(outStr4, 11);
	delay(300);
	int i = 0;
	Serial.print("bytes available:");
	Serial.println(mySerial.available());
	while (mySerial.available()>0)
	{
		uint8_t X = mySerial.read();
		printHex(X);
		inStr[i++] = X;
	}
	float invTemp = (256 * inStr[7] + inStr[8]) / 10.0;
	float pvVolts1 = (256 * inStr[9] + inStr[10]) / 10.0;
	float pvVolts2 = (256 * inStr[11] + inStr[12]) / 10.0;
	float pvAmps1 = (256 * inStr[13] + inStr[14]) / 10.0;
	float pvAmps2 = (256 * inStr[15] + inStr[16]) / 10.0;
	float pvEnergyToday = (256 * inStr[17] + inStr[18]) / 100.0;
	float pvEnergyTotal = (256.0 * 256.0 * (float)inStr[30] + 256.0 * (float)inStr[31] + (float)inStr[32]) / 10.0;

	float pvStr1 = pvVolts1 * pvAmps1;
	float pvStr2 = pvVolts2 * pvAmps2;
	float dcPower = pvStr1 + pvStr2;
	float acPower = (256 * inStr[25] + inStr[26]) / 10.0;

	// check for change of minute
	if (oldSec <= second)  //same minute
	{
		oldSec = second;
		if (acPower > powerMax) powerMax = acPower;
		if (acPower < powerMin) powerMin = acPower;
		powerAv += acPower;
		sampleCount++;
	}
	else				//new minute
	{
		oldSec = 0;
		sprintf(timeStr, "%i%i:%i%i:%i%i ", hour / 10, hour % 10, minute / 10, minute % 10, second / 10, second % 10);
		timeStr[9] = '\0';
		Serial.print(timeStr);
		if (sampleCount == 0) powerAv = 0;
		else powerAv /= sampleCount;

		printFloat("Temp = ", invTemp);
		printFloat("PV1 = ", pvVolts1);
		printFloat("PV2 = ", pvVolts2);
		printFloat("PA1 = ", pvAmps1);
		printFloat("PA2 = ", pvAmps2);
		printFloat("PW1 = ", pvStr1);
		printFloat("PW2 = ", pvStr2);
		printFloat("Pmin = ", powerMin);
		printFloat("Pav = ", powerAv);
		printFloat("Pmax = ", powerMax);
		printFloat("E today = ", pvEnergyToday);
		printFloat("E total = ", pvEnergyTotal);
		Serial.print(sampleCount);
		Serial.println(" samples");

		powerMax = 0;
		powerMin = 9999;
		powerAv = 0;
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

int bcd2bin(int temp)
{
	int a, b, c;
	a = temp;
	b = 0;
	if (a >= 16)
	{
		while (a >= 16)
		{
			a = a - 16;
			b = b + 10;
			c = a + b;
			temp = c;
		}
	}
	return temp;
}
