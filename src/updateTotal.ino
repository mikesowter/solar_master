byte readTotal() {
  pvEnergyTotal = 0.0;
  fh = SPIFFS.open("/TotalNRG.csv", "r");
  while (fh.available()>1) {
    // first check there isn't already an entry for today
    fh.readBytes(charBuf,6);
    charBuf[6] = '\0';
    if ( strncmp(dateStamp(),charBuf,6)==0 ) {
      diagMess("days Total_kWh already exists");
      return 1;
    }
    // otherwise choose the largest
    fh.readBytes(charBuf,9);
    thisTotal = (double)fh.parseFloat();
    while ( fh.read()!='\r' );        // remove "\n\r" from end of line
    fh.read();                      // ????
    if (thisTotal > pvEnergyTotal) pvEnergyTotal = thisTotal;
  }
  fh.close();
  return 1;
}

/* To correctly track today's total energy:
1. Assume that the inverter has outages, so pvEnergyToday drops to 0 each time
2. Assume that the interface does not have concurrent outages

we need 3 variables:
thisEnergyToday (current scan reading)
prevEnergyToday (previous scan reading)           reset at midnight
sumEnergyToday  (accumulated max scan readings)   reset at midnight
*/

byte updateTotal() {
  if (dayStored) return 1;
  fh = SPIFFS.open("/TotalNRG.csv", "a");
  if (!fh) return 0;
  pvEnergyTotal += sumEnergyToday+thisEnergyToday;
  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();
  dayStored=true;
  return 1;
}
