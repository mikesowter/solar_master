#include <arduino.h>
#include <fs.h>

void diagMess(const char* mess);
char* dateStamp();
char* timeStamp();
char* f8ds(float f);

extern FSInfo fs_info;
extern File fh;
extern double pvEnergyTotal, thisTotal, thisEnergyToday, prevEnergyToday, sumEnergyToday;
extern char charBuf[];
extern bool dayStored;

bool readTotal() {
  pvEnergyTotal = 0.0;
  fh = SPIFFS.open("/TotalNRG.csv", "r");
  if (!fh) {
    diagMess("/TotalNRG.csv failed to open");
    return false;
  }
  while (fh.available()>1) {
    // first check there isn't already an entry for today
    fh.readBytes(charBuf,6);
    charBuf[6] = '\0';
    if ( strncmp(dateStamp(),charBuf,6)==0 ) {
      diagMess("days Total_kWh already exists");
      return true;
    }
    // otherwise choose the largest
    fh.readBytes(charBuf,9);
    thisTotal = (double)fh.parseFloat();
    while ( fh.read()!='\r' );        // remove "\n\r" from end of line
    fh.read();                        // ????
    if (thisTotal > pvEnergyTotal) {
      pvEnergyTotal = thisTotal;
    }
  }
  fh.close();
  Serial.printf("\nTotal Energy: %f\n",pvEnergyTotal);
  return true;
}

/* To correctly track today's total energy:
1. Assume that the inverter has solar outages, so pvEnergyToday drops to 0
2. Assume that the interface does not have concurrent outages

we need 3 variables:
thisEnergyToday (current scan reading)
prevEnergyToday (previous scan reading)           reset at midnight
sumEnergyToday  (accumulated max scan readings)   reset at midnight
*/

bool updateTotal() {
  if (dayStored) return 1;
  fh = SPIFFS.open("/TotalNRG.csv", "a");
  if (!fh) return false;
  pvEnergyTotal += sumEnergyToday + thisEnergyToday;
  strcpy(charBuf,dateStamp());
  strcat(charBuf,",");
  strcat(charBuf,timeStamp());
  strcat(charBuf,",");
  strcat(charBuf,f8ds(pvEnergyTotal));
  fh.println(charBuf);
  fh.close();
  dayStored = true;
  return true;
}
