void queryInv() {
  for (int j=0;j<8;j++) {
    mySerial.write(outStr4, 11);	// query inverter
    watchWait(1000);

	  if (mySerial.available()==53) {
      int i = 0;
      while (mySerial.available()>0) {
        inStr[i++] = mySerial.read();
      }
      if (badCheckSum(51)) break;
      invReply=true;
  		pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
      pvEnergyToday = (256 * inStr[9] + inStr[10]) / 100.0;
//      thisEnergyToday = (256 * inStr[9] + inStr[10]) / 100.0;
//      if (thisEnergyToday<prevEnergyToday) sumEnergyToday += prevEnergyToday;
//      prevEnergyToday = thisEnergyToday;
  		pvVolts1 = (256 * inStr[11] + inStr[12]) / 10.0;
  		pvAmps1 = (256 * inStr[13] + inStr[14]) / 10.0;
//      if (pvAmps1 > 20.0) pvAmps1 = 0.0;
      acVolts = (256 * inStr[15] + inStr[16]) / 10.0;
      acFrequency = (256 * inStr[17] + inStr[18]) / 100.0;
      pvPower = (256 * inStr[19] + inStr[20]) ;
      pvEnergyTotal = (256 * 256 * inStr[24] + 256 * inStr[25] + inStr[26]) / 10.0;
      pvAmps2 = (256 * inStr[27] + inStr[28]) / 10.0;
      pvVolts2 = inStr[21];
      sampleCount++;

/* old inverter
      pvInvTemp = (256 * inStr[7] + inStr[8]) / 10.0;
  		pvVolts1 = (256 * inStr[9] + inStr[10]) / 10.0;
  		pvVolts2 = (256 * inStr[11] + inStr[12]) / 10.0;
  		pvAmps1 = 0.9*pvAmps1+(float)(256 * inStr[13] + inStr[14]) / 100.0;
  		pvAmps2 = 0.9*pvAmps2+(float)(256 * inStr[15] + inStr[16]) / 100.0;
  		thisEnergyToday = (256 * inStr[17] + inStr[18]) / 100.0;
      if (thisEnergyToday<prevEnergyToday) sumEnergyToday += prevEnergyToday;
      prevEnergyToday = thisEnergyToday;
      acVolts = (256 * inStr[21] + inStr[22]) / 10.0;
      acFrequency = (256 * inStr[23] + inStr[24]) / 100.0;
  		pvPower = (256 * inStr[25] + inStr[26]) / 10.0;   */
      return;
	  }
    else while (mySerial.available()>0) mySerial.read();  // flush buffer
  }
  invReply = false;
  diagMess("no reply");
  watchWait(300000UL);
  pvInvTemp = 0.0;
  acFrequency = 50.0;
}

bool badCheckSum(uint8_t len) {
  uint16_t sum = 0;
  for (int i=0;i<len;i++) sum += inStr[i];
  return sum == 0;
}
