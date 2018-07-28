void setupInv() {
  mySerial.begin(9600);
  while (invReply==false) {
    mySerial.write(outStr1,11);
    watchWait(100);
    mySerial.write(outStr2,11);
    watchWait(100);
    readBytes();                      // read serial number
    watchWait(1000);
    mySerial.write(outStr3,28);       // assign address
    watchWait(1000);

    if (mySerial.available()==12) {
      readBytes();                    // read acknowledgement
      if (badCheckSum(10)) break;
      invReply = true;
      firstPass = true;       // necessary for first min/max processing
      return;
    }
    while (mySerial.available()>0) mySerial.read();  // flush buffer
    watchDog = 0;
    watchWait(10000);
  }
}

void readBytes() {
  int i = 0;
  htmlStr[0]='\0';
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    addCstring(p2h(X));
    addCstring(" ");
    inStr[i++] = X;
  }
  if ( i==0 || inStr[0]==0xFF ) return;
  diagMess(htmlStr);
  fd.flush();
}
