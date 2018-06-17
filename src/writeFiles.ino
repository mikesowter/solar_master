byte storeData() {
  strcpy(fileName,todayName);
  if ( !openFile("a") ) return 0;
  WriteQtr();
  fh.close();
  return 1;
}

//----------------- open file for appending

byte openFile(const char* s) {
  fh = SPIFFS.open(fileName, s);
  if (!fh) {
    strcpy(charBuf,fileName);
    strcat(charBuf," failed to open");
    diagMess(charBuf);
    return 0;
  }
  return 1;
}

void WriteQtr() {
  fh.print(p2d(hour()));
  fh.print(":");
  fh.print(p2d(minute()));
  fh.print(",");
  fh.print(pvQtrMin);
  fh.print(",");
  fh.print(pvQtrMax);
  fh.print(",");
  fh.println(pvEnergyToday);
  yield();
}
