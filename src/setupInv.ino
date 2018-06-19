void setupInv() {
  mySerial.begin(9600);
  invReply=false;
  while (!invReply) {
    delay(200);
    mySerial.write(outStr1,11);
    delay(200);
    mySerial.write(outStr2,11);
    delay(200);
    readBytes();  // flush serial buffer
    mySerial.write(outStr3,28);
    watchWait(400UL);

    if (mySerial.available()==12) {
      invReply=true;
      readBytes();  // flush serial buffer
      return;
    }
    watchDog=0;
  }
}

void readBytes() {
  int i = 0;
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    inStr[i++] = X;
  }
}
