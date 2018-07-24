// add a web page with a listing of the SPIFFS "/" folder

void handleDir() {
  htmlStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><HR>");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    addCstring("<P>");
    addCstring(fileName);
    addCstring("&emsp;");
    itoa(dir.fileSize(),fileSizeStr,10);
    addCstring(fileSizeStr);
  }
  addCstring( "<HR></body></html>" );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void listFile() {
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    if (strncmp(fileName,userText,9)==0) {
      fileSize=dir.fileSize();
      ltoa(fileSize,fileSizeStr,10);
      break;
    }
  }
  strcpy(htmlStr,"file: ");
  addCstring(userText);
  addCstring(" size: ");
  addCstring(fileSizeStr);
  addCstring("\r\r");
  fh = SPIFFS.open(userText, "r");

  if (fileSize > HTML_SIZE) {
    fh.seek((HTML_SIZE-100),SeekEnd);
  }
  while (fh.available()) {
    int k=fh.readBytesUntil('\r',charBuf,80);
    charBuf[k]='\0';
    addCstring(charBuf);
    yield();
  }
  fh.close();
  server.send ( 200, "text/plain", htmlStr );
}

void helpPage() {
  htmlStr[0]='\0';
  addCstring("Valid options include:");
  addCstring("\n");
  addCstring("8.3 filename");
  addCstring("\n");
  addCstring("day");
  addCstring("\n");
  addCstring("diags");
  addCstring("\n");
  addCstring("dir");
  addCstring("\n");
  addCstring("metrics");
  addCstring("\n");
  addCstring("remdiags");
  addCstring("\n");
  addCstring("remerrs");
  addCstring("\n");
  addCstring("reset");
  addCstring("\n");
  addCstring("shutdown");
  addCstring("\n");
  addCstring("week");
  addCstring("\n");
  server.send ( 200, "text/plain", htmlStr );
  //Serial.println(htmlStr);
}
