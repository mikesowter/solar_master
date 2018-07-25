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
/* old logic for faulty Total readings:
  fh = SPIFFS.open("/TotalNRG.csv", "a");
  if (!fh) return 0;
  pvEnergyTotal += sumEnergyToday+thisEnergyToday;
  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();  */

// new logic to add Total as of day of commissioning new inverter: 36,796kWh
  fh = SPIFFS.open("/TotalNRG.csv", "a");
  if (!fh) return 0;
  pvEnergyTotal += 36796.0;
  strcpy(charBuf,dateStamp());
  strcat(charBuf," ");
  strcat(charBuf,timeStamp());
  strcat(charBuf,p8d(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();
  dayStored=true;
  return 1;
}
