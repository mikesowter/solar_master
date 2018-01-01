byte updateAnnual() {
  strcpy(fileName,"SPAN");
  strcat(fileName,p2d(year()/100));
  strcat(fileName,p2d(year()%100));
  strcat(fileName,".csv");
  fh = SPIFFS.open(fileName, "a+");
  if (!fh) return 0;
  strcpy(charBuf,dateStamp());
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyAnnual));
  if (fh.seek(10,SeekEnd)) {
    pvEnergyAnnual = fh.parseFloat();
    pvEnergyAnnual += pvETLast;
  }
  else {
    pvEnergyAnnual = pvETLast;
  }
  fh.seek(0,SeekEnd);
  strcpy(charBuf,dateStamp());
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyAnnual));
  fh.println(charBuf);
  fh.close();
  return 1;
}
