// add a web page with a listing of the SPIFFS "/" folder

void handleDir() {
  char fileSize[]="999999";
  htmlStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><HR>");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    addCstring("<P>");
    addCstring(fileName);
    addCstring("&emsp;");
    itoa(dir.fileSize(),fileSize,7);
    addCstring(fileSize);
  }
  addCstring( "<HR></body></html>" );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}
/*
uint8_t listDiags() {
  char line[66];
  int i,j,k;
  const int DIAG_ROWS = 100;
  uint32_t ptrs[DIAG_ROWS];
  htmlStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><HR>");
  fd.close();
  fd = SPIFFS.open("/diags.txt", "r");
  i=0;
  uint8_t numRows=0;

  while (fd.available()) {
    k=fd.readBytesUntil('\r',line,64);
    line[k]='\0';
    addCstring(line);
    addCstring("<P>");
  }
  addCstring( "<HR></body></html>" );
  //Serial.println(htmlStr);
  server.send ( 200, "text/html", htmlStr );
  fd.close();
  return 1;
}
*/
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
