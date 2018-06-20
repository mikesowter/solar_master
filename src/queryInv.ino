void queryInv() {
  for (int j=0;j<3;j++) {
    mySerial.write(outStr4, 11);	// query inverter
    watchWait(400UL);

	  if (mySerial.available()==59) {
      int i = 0;
      while (mySerial.available()>0) {
        uint8_t X = mySerial.read();
        inStr[i++] = X;
      }
  		pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
  		pvVolts1 = (256 * inStr[9] + inStr[10]) / 10.0;
  		pvVolts2 = (256 * inStr[11] + inStr[12]) / 10.0;
  		pvAmps1 = 0.9*pvAmps1+(float)(256 * inStr[13] + inStr[14]) / 100.0;
  		pvAmps2 = 0.9*pvAmps2+(float)(256 * inStr[15] + inStr[16]) / 100.0;
  		pvEnergyToday = (256 * inStr[17] + inStr[18]) / 100.0;
//      pvEnergyTotal =  (256*256*inStr[30]+256*inStr[31]+inStr[32])/10.0;
      acVolts = (256 * inStr[21] + inStr[22]) / 10.0;
      acFrequency = (256 * inStr[23] + inStr[24]) / 100.0;
  		pvPower = (256 * inStr[25] + inStr[26]) / 10.0;
      return;
	  }
    else readBytes();   // flush serial buffer
  }
  invReply=false;
  watchWait(10000UL);
}
