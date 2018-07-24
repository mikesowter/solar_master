void setupInv() {
  mySerial.begin(9600);
  while (invReply==false) {
    mySerial.write(outStr1,11);
    delay(100);
    mySerial.write(outStr1,11);
    delay(100);
    mySerial.write(outStr1,11);
    delay(100);
    mySerial.write(outStr2,11);
    delay(100);
    readBytes();  // read serial number
    mySerial.write(outStr3,28);
    watchWait(1000);

    if (mySerial.available()>10) {
      readBytes();  // read serial buffer
      if (inStr[0] != 0xA5) break;
      if (inStr[1] != 0xA5) break;
      invReply=true;
      return;
    }
    else readBytes();  // read serial buffer
    watchDog=0;
    watchWait(10000);
  }
}

void readBytes() {
  if (mySerial.available()==0) {
    Serial.print("0");
    return;
  }
  int i = 0;
  htmlStr[0]='\0';
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    addCstring(p2h(X));
    addCstring(" ");
    inStr[i++] = X;
  }
  diagMess(htmlStr);
  fd.flush();
}
