byte updateTotal() {
  strcpy(fileName,"TotalNRG.csv");
  fh = SPIFFS.open(fileName, "a+");
  if (!fh) return 0;
  while (fh.available()) {
    fh.readBytes(charBuf,17);
    if (strncmp(charBuf,dateStamp(),6)==0) {
      // today already exists
      diagMess(charBuf);
      break;
    }
    pvEnergyTotal = fh.parseFloat();
    pvEnergyTotal += pvETLast;
  }

  fh.seek(0,SeekEnd);
  strcpy(charBuf,dateStamp());
  strcpy(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();
  return 1;
}
