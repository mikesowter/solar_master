byte updateTotal() {
  int yy,mm,dd;
  diagMess("updateTotal entry ");
  fh = SPIFFS.open("/TotalNRG.csv", "a+");
  if (!fh) return 0;
  while (fh.available()>3) {
    fh.readBytes(charBuf,17);
    if (strncmp(charBuf,dateStamp(),6)!=0) {
      pvEnergyTotal = fh.parseFloat();
    }
  }
  pvEnergyTotal += pvEnergyToday;
  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();
  return 1;
}
