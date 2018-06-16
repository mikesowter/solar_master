byte updateTotal() {
  if (dayStored) return 0;
  fh = SPIFFS.open("/TotalNRG.csv", "a+");
  if (!fh) return 0;
  while (fh.available()>3) {
    fh.readBytes(charBuf,17);
    if (strncmp(charBuf,dateStamp(),6)!=0) {
      pvEnergyTotal = fh.parseFloat();
    }
    else diagMess("day already written");
  }
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
