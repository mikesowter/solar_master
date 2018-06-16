void setupInv() {
  mySerial.begin(9600);
  while (true) {
    mySerial.write(outStr1,11);
    delay(200);
    mySerial.write(outStr2,11);
    delay(200);
    mySerial.write(outStr3,28);
    watchWait(400UL);

    if (mySerial.available()==12) break;
    invReply=false;
    watchWait(30000UL);
  }
  readBytes();
  invReply=true;
}

void readBytes() {
  int i = 0;
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    inStr[i++] = X;
  }
}
