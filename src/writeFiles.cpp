#include <arduino.h>
#include <fs.h>
#include <TimeLib.h>

void diagMess(const char* mess);
char* i2sd(uint8_t b);
uint8_t openFile(const char* s);
void WriteQtr();

extern File fh;
extern char charBuf[],fileName[],todayName[];
extern float pvQtrMax,pvQtrMin,qtrEnergy,pvEnergyToday;
extern double thisEnergyToday,sumEnergyToday;

uint8_t storeData() {
  strcpy(fileName,todayName);
  if ( !openFile("a") ) return 0;
  WriteQtr();
  fh.close();
  return 1;
}

//----------------- open file for appending

uint8_t openFile(const char* s) {
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
  fh.print(i2sd(hour()));
  fh.print(":");
  fh.print(i2sd(minute()));
  fh.print(",");
  fh.print(pvQtrMin);
  fh.print(",");
  fh.print(pvQtrMax);
  fh.print(",");
  fh.println(sumEnergyToday + thisEnergyToday);
//  fh.println(pvEnergyToday);
  yield();
}
