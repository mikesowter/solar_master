void queryInv() {
  while(true) {
    mySerial.write(outStr4, 11);	// query inverter
	  delay(400);
  	int bytes=mySerial.available();
//    Serial.print(bytes);
	  if (bytes==59) {
      int i = 0;
      while (mySerial.available()>0) {
        uint8_t X = mySerial.read();
        inStr[i++] = X;
      }
  		pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
  		pvVolts1 = (256 * inStr[9] + inStr[10]) / 10.0;
  		pvVolts2 = (256 * inStr[11] + inStr[12]) / 10.0;
  		pvAmps1 = (256 * inStr[13] + inStr[14]) / 10.0;
  		pvAmps2 = (256 * inStr[15] + inStr[16]) / 10.0;
  		pvEnergyToday = (256 * inStr[17] + inStr[18]) / 100.0;
      acVolts = (256 * inStr[21] + inStr[22]) / 10.0;
      acFrequency = (256 * inStr[23] + inStr[24]) / 100.0;
  		pvPower = (256 * inStr[25] + inStr[26]) / 10.0;
      return;
	  }
    else showBytes();
    yield();
   }
}
