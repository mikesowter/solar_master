void setupInv() {
  mySerial.begin(9600);
  mySerial.write(outStr1,11);
  delay(100);
  mySerial.write(outStr2,11);
  delay(100);
  mySerial.write(outStr3,28);
  delay(500);
  int i = 0;
  Serial.print("bytes available:");
  Serial.println(mySerial.available());
  {
    uint8_t X = mySerial.read();
    printHex(X);
    inStr[i++] = X;
  }
  Serial.print("   ");
  Serial.print(i);
  Serial.println(" chars");
}
