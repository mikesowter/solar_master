#include <SoftwareSerial.h>
#include <Print.h>
#include "Wire.h"

SoftwareSerial mySerial(10, 9); // RX, TX

uint8_t oldSec=60, second, minute, hour, day, month, year, dow;

uint8_t outStr1[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			// request inverter reconnect
uint8_t outStr2[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x45, 0x0A, 0x0D };			// is anybody out there?
uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x32, 0x32, 0x31, 0x33, 0x31, 0x31, 0x31,
						0x30, 0x30, 0x30, 0x36, 0x20, 0x20, 0x20, 0x11, 0xFB, 0x3B, 0x0A, 0x0D };	// assign address 0x11
uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x11, 0x31, 0x42, 0x00, 0xFE, 0x31, 0x0A, 0x0D };			// request data from 0x11
uint8_t inStr[60];
char timeStr[10];
int sampleCount;

float powerMax, powerMin, powerAv;

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\rSolar Master Rev 0.4 20141124\r\n\n");
	Wire.begin();						// enable I2C interface to RTC
	
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
}

void loop() // run over and over
{
/*	while (mySerial.available())		//flush buffer
	{
		char c = mySerial.read();
		Serial.print(c,HEX);
	}
	Serial.println('+');		*/

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

	Wire.beginTransmission(0x68);		//Send the address of DS1307
	Wire.write(0);						//Sending address	
	Wire.endTransmission();				//The end of the IIC communication

	Wire.requestFrom(0x68, 7);			//IIC communication is started, you can continue to access another address (address auto increase) and the number of visits
	second = bcd2bin(Wire.read());		//read time
	minute = bcd2bin(Wire.read());
	hour = bcd2bin(Wire.read());
	dow = bcd2bin(Wire.read());
	day = bcd2bin(Wire.read());
	month = bcd2bin(Wire.read());
	year = bcd2bin(Wire.read());

	// check for new day

/*	if (day != oldDay)
	{
		sprintf(fileName, "/RMS/%4i%i%i%i%i.csv", year, month / 10, month % 10, day / 10, day % 10);
		fileName[19] = '\0';
		oldDay = day;
	}		*/

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
/*		digitalWrite(sdPin, LOW);	            // enable SD card interface
		rmsFile = SD.open(fileName, FILE_WRITE);
		if (!rmsFile) printf("\n\r File %s could not be opened\n\r", fileName);	*/
//		if (sampleCount != 0)
//		{
/*			rmsFile.print(timeStr);
			rmsFile.print(",,");
			rmsFile.print(buff[1]);    //volts
			rmsFile.print(',');		*/

			if (sampleCount == 0) powerAv = 0;
			else powerAv /= sampleCount;
//			sprintf(valueStr, ",%i,%i,%i,", powerMax[i], powerAv[i], powerMin[i]);
//			rmsFile.print(valueStr);
//			printf("   Sample Count: %i\n\n\r%s: ", sampleCount, timeStr);
//			rmsFile.println(sampleCount);
//		}
//		rmsFile.close();
//		digitalWrite(sdPin, HIGH);	    // disable SD card interface
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

int bcd2bin(int temp)  //BCD  to decimal
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