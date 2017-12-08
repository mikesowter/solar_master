void setupInv() {
  mySerial.begin(9600);
  while (true) {
    mySerial.write(outStr1,11);
    delay(200);
    mySerial.write(outStr2,11);
    delay(200);
    mySerial.write(outStr3,28);
    delay(500);
    if (mySerial.available()==12) break;
    while (mySerial.available()>0) mySerial.read();
    delay(10000);
  }
  showBytes();
}

void showBytes() {
  if (mySerial.available()==0) return;
  Serial.print(timeStamp());
  Serial.print("Rx:");
  Serial.print(mySerial.available());
  Serial.print(" -> ");
  int i = 0;
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    printHex(X);
    inStr[i++] = X;
  }
  Serial.println();
}
