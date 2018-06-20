byte readTotal() {
  fh = SPIFFS.open("/TotalNRG.csv", "r");
  if (!fh) return 0;
  while (fh.available()>1) {
    fh.readBytes(charBuf,17);
    thisTotal = fh.parseFloat();
    if (thisTotal > pvEnergyTotal) pvEnergyTotal = thisTotal;
  }
  fh.close();
  return 1;
}

byte updateTotal() {
  if (dayStored) return 0;
  fh = SPIFFS.open("/TotalNRG.csv", "a");
  if (!fh) return 0;
  pvEnergyTotal += pvEnergyToday;
  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();
  dayStored=true;
  return 1;
}
