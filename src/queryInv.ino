void queryInv() {
  mySerial.write(outStr4, 11);	// query inverter
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
}
