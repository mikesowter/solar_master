void setupInv() {
  mySerial.begin(9600);
  while (true) {
    delay(100);
    mySerial.write(outStr1,11);
    delay(200);
    mySerial.write(outStr2,11);
    delay(200);
    mySerial.write(outStr3,28);
    watchWait(400UL);

    if (mySerial.available()==12) break;
    // check for OTA
    ArduinoOTA.handle();
  	// check for web request
  	server.handleClient();
    delay(5000);
  }
  readBytes();
  Serial.println("\n\rInverter Configured");
}

void readBytes() {
  int i = 0;
  while (mySerial.available()>0) {
    uint8_t X = mySerial.read();
    inStr[i++] = X;
  }
}
