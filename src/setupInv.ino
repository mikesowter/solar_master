void setupInv() {
  mySerial.begin(9600);
  while (invReply==false) {
    mySerial.write(outStr1,11);
    delay(100);
    mySerial.write(outStr2,11);
    delay(100);
    readBytes();  // flush serial number
    mySerial.write(outStr3,28);
    watchWait(1000);

    if (mySerial.available()==12) {
      readBytes();  // read serial buffer
      if (inStr[0] != 0xA5) break;
      if (inStr[1] != 0xA5) break;
      invReply=true;
      return;
    }
    else readBytes();  // read serial buffer
    watchDog=0;
    watchWait(30000);
  }
}

void readBytes() {
  if (mySerial.available()==0) {
    Serial.print("0");
    return;
  }
  int i = 0;
  Serial.print(timeStamp());
  Serial.print(" ");
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    printHex(X);
    inStr[i++] = X;
  }
  Serial.print("   ");
  Serial.print(i);
  Serial.println(" bytes");
}
